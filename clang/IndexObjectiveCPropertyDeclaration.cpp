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
#include "IndexObjectiveCPropertyDeclaration.h"

#include "IndexDeclarationKind.h"
#include "IndexEntity.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexObjectiveCPropertyDeclaration Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	basehandle	- IdxDeclInfoHandle to pass to the base class

IndexObjectiveCPropertyDeclaration::IndexObjectiveCPropertyDeclaration(IdxObjCPropertyDeclInfoHandle^ handle, IdxDeclInfoHandle^ basehandle) 
	: m_handle(handle), IndexDeclaration(basehandle, IndexDeclarationKind::ObjectiveCProperty)
{
}

//---------------------------------------------------------------------------
// IndexObjectiveCPropertyDeclaration::Create (static)
//
// Creates a new IndexObjectiveCPropertyDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxObjCPropertyDeclInfo pointer

IndexObjectiveCPropertyDeclaration^ IndexObjectiveCPropertyDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCPropertyDeclInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexObjectiveCPropertyDeclaration(gcnew IdxObjCPropertyDeclInfoHandle(owner, action, info), 
		gcnew IdxDeclInfoHandle(owner, action, info->declInfo));
}

//---------------------------------------------------------------------------
// IndexObjectiveCPropertyDeclaration::GetterEntity::get
//
// Gets the entity associated with the property getter

IndexEntity^ IndexObjectiveCPropertyDeclaration::GetterEntity::get(void)
{
	if(Object::ReferenceEquals(m_getter, nullptr))
		m_getter = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCPropertyDeclInfoHandle::Reference(m_handle)->getter);

	return m_getter;
}

//---------------------------------------------------------------------------
// IndexObjectiveCPropertyDeclaration::SetterEntity::get
//
// Gets the entity assiciated with the property setter

IndexEntity^ IndexObjectiveCPropertyDeclaration::SetterEntity::get(void)
{
	if(Object::ReferenceEquals(m_setter, nullptr))
		m_setter = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCPropertyDeclInfoHandle::Reference(m_handle)->setter);

	return m_setter;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
