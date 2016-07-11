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
#include "IndexImportedASTFile.h"

#include "File.h"
#include "Location.h"
#include "LocationKind.h"
#include "Module.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexImportedASTFile Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexImportedASTFile::IndexImportedASTFile(IdxImportedASTFileInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexImportedASTFile::Create (internal, static)
//
// Creates a new IndexImportedASTFile instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the imported file information

IndexImportedASTFile^ IndexImportedASTFile::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxImportedASTFileInfo* info)
{
	return gcnew IndexImportedASTFile(gcnew IdxImportedASTFileInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexImportedASTFile::File::get
//
// Gets the imported file instance

local::File^ IndexImportedASTFile::File::get(void)
{
	if(Object::ReferenceEquals(m_file, nullptr))
		m_file = local::File::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxImportedASTFileInfoHandle::Reference(m_handle)->file);

	return m_file;
}

//---------------------------------------------------------------------------
// IndexImportedASTFile::IsImplicit::get
//
// Gets a flag indicating if an inclusion directive was converted into a module

bool IndexImportedASTFile::IsImplicit::get(void)
{
	return (IdxImportedASTFileInfoHandle::Reference(m_handle)->isImplicit != 0);
}

//---------------------------------------------------------------------------
// IndexImportedASTFile::Location::get
//
// Gets the location where the file is imported

local::Location^ IndexImportedASTFile::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, clang_indexLoc_getCXSourceLocation(IdxImportedASTFileInfoHandle::Reference(m_handle)->loc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexImportedASTFile::Module::get
//
// Gets the imported module or a null module if the imported file is a PCH

local::Module^ IndexImportedASTFile::Module::get(void)
{
	if(Object::ReferenceEquals(m_module, nullptr))
		m_module = local::Module::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxImportedASTFileInfoHandle::Reference(m_handle)->module);

	return m_module;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
