//---------------------------------------------------------------------------
// Copyright (c) 2016 Michael G. Brehm
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "Module.h"

#include "File.h"
#include "StringUtil.h"
#include "TopLevelHeaderCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Module Static Constructor (private)

static Module::Module()
{
	Null = Module::Create(TranslationUnitHandle::Null, TranslationUnitHandle::Null, __nullptr);
}

//---------------------------------------------------------------------------
// Module Constructor (protected)
//
// Arguments:
//
//	handle		- Underlying ModuleHandle instance

Module::Module(ModuleHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Module::Create (internal, static)
//
// Creates a new Module instance
//
// Arguments:
//
//	owner		- SafeHandle instance that owns this module reference
//	transunit	- TranslationUnitHandle instance
//	module		- Unmanaged CXModule instance to be wrapped

Module^ Module::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXModule module)
{
	return gcnew Module(gcnew ModuleHandle(owner, transunit, module));
}

//---------------------------------------------------------------------------
// Module::File::get
//
// Gets the abstract syntax tree file from which this module came

local::File^ Module::File::get(void)
{
	if(Object::ReferenceEquals(m_file, nullptr))
		m_file = local::File::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Module_getASTFile(ModuleHandle::Reference(m_handle)));

	return m_file;
}

//---------------------------------------------------------------------------
// Module::FullName::get
//
// Gets the full name of the module

String^ Module::FullName::get(void)
{
	if(Object::ReferenceEquals(m_fullname, nullptr)) 
		m_fullname = StringUtil::ToString(clang_Module_getFullName(ModuleHandle::Reference(m_handle)));

	return m_fullname;
}

//---------------------------------------------------------------------------
// Module::IsNull (static)
//
// Determines if the module is null
//
// Arguments:
//
//	rhs		- Module instance to be checked for null

bool Module::IsNull(Module^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return static_cast<CXModule>(ModuleHandle::Reference(rhs->m_handle)) == __nullptr;
}

//---------------------------------------------------------------------------
// Module::IsSystem::get
//
// Indicates if this is a system module

bool Module::IsSystem::get(void)
{
	return (clang_Module_isSystem(ModuleHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Module::Name::get
//
// Gets the name of the module

String^ Module::Name::get(void)
{
	if(Object::ReferenceEquals(m_name, nullptr))
		m_name = StringUtil::ToString(clang_Module_getName(ModuleHandle::Reference(m_handle)));

	return m_name;
}

//---------------------------------------------------------------------------
// Module::ParentModule::get
//
// Gets the parent module

Module^ Module::ParentModule::get(void)
{
	if(Object::ReferenceEquals(m_parent, nullptr))
		m_parent = Module::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Module_getParent(ModuleHandle::Reference(m_handle)));

	return m_parent;
}

//---------------------------------------------------------------------------
// Module::TopLevelHeaders
//
// Gets a collection of top-level headers associated with this module

TopLevelHeaderCollection^ Module::TopLevelHeaders::get(void)
{
	if(Object::ReferenceEquals(m_headers, nullptr)) 
		m_headers = TopLevelHeaderCollection::Create(m_handle->Owner, m_handle->TranslationUnit, ModuleHandle::Reference(m_handle));

	return m_headers;
}

//---------------------------------------------------------------------------
// Module::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Module::ToString(void)
{
	return (IsNull(this)) ? "<null>" : this->FullName;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
