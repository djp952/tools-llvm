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
#include "IndexIncludedFile.h"

#include "File.h"
#include "Location.h"
#include "LocationKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexIncludedFile Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexIncludedFile::IndexIncludedFile(IdxIncludedFileInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexIncludedFile::Create (internal, static)
//
// Creates a new IndexIncludedFile instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the included file information

IndexIncludedFile^ IndexIncludedFile::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxIncludedFileInfo* info)
{
	return gcnew IndexIncludedFile(gcnew IdxIncludedFileInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexIncludedFile::File::get
//
// Gets the file instance for this include

local::File^ IndexIncludedFile::File::get(void)
{
	if(Object::ReferenceEquals(m_file, nullptr))
		m_file = local::File::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxIncludedFileInfoHandle::Reference(m_handle)->file);

	return m_file;
}

//---------------------------------------------------------------------------
// IndexIncludedFile::IsAngled::get
//
// Gets a flag indicating if the file was included with angle brackets

bool IndexIncludedFile::IsAngled::get(void)
{
	return (IdxIncludedFileInfoHandle::Reference(m_handle)->isAngled != 0);
}

//---------------------------------------------------------------------------
// IndexIncludedFile::IsImport::get
//
// Gets a flag indicating if the file is an #import rather than an #include

bool IndexIncludedFile::IsImport::get(void)
{
	return (IdxIncludedFileInfoHandle::Reference(m_handle)->isImport != 0);
}

//---------------------------------------------------------------------------
// IndexIncludedFile::IsModuleImport::get
//
// Gets the File instance associated with the included file

bool IndexIncludedFile::IsModuleImport::get(void)
{
	return (IdxIncludedFileInfoHandle::Reference(m_handle)->isModuleImport != 0);
}

//---------------------------------------------------------------------------
// IndexIncludedFile::Location::get
//
// Gets the location of the include/import directive "#" in the source file

local::Location^ IndexIncludedFile::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, clang_indexLoc_getCXSourceLocation(IdxIncludedFileInfoHandle::Reference(m_handle)->hashLoc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexIncludedFile::Spelling::get
//
// Gets the file name as written in the include/import directive

String^ IndexIncludedFile::Spelling::get(void)
{
	if(Object::ReferenceEquals(m_spelling, nullptr))
		m_spelling = StringUtil::ToString(IdxIncludedFileInfoHandle::Reference(m_handle)->filename, CP_UTF8);

	return m_spelling;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
