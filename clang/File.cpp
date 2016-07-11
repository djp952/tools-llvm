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
#include "File.h"

#include "AutoGCHandle.h"
#include "Cursor.h"
#include "EnumerateIncludeDirectivesFunc.h"
#include "EnumerateIncludeDirectivesResult.h"
#include "Extent.h"
#include "ExtentCollection.h"
#include "FileUniqueIdentifier.h"
#include "GCHandleRef.h"
#include "Location.h"
#include "LocationKind.h"
#include "Module.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesCallback (local)
//
// Callback for clang_findIncludesInFile() that invokes a delegate function against
// each included cursor/extent pair that is returned
//
// Arguments:
//
//	conext			- Context object passed into the enumeration function
//	cursor			- Cursor instance for the enumerated directive
//	extent			- Extent instance for the enumerated directive

static CXVisitorResult EnumerateIncludeDirectivesCallback(void* context, CXCursor cursor, CXSourceRange extent)
{
	GCHandleRef<EnumerateIncludeDirectivesFunc^> func(context);

	// Invoke the delegate and capture any exception that occurs
	try { return func->Invoke(cursor, extent); }
	catch(Exception^ exception) { func->Exception = exception; return CXVisitorResult::CXVisit_Break; }
}

//---------------------------------------------------------------------------
// File Static Constructor (private)

static File::File()
{
	Null = File::Create(TranslationUnitHandle::Null, TranslationUnitHandle::Null, __nullptr);
}

//---------------------------------------------------------------------------
// File Constructor (protected)
//
// Arguments:
//
//	handle		- Underlying FileHandle instance

File::File(FileHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// File::operator == (static)

bool File::operator==(File^ lhs, File^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	// clang_File_isEqual returns non-zero if the files are the same
	return (clang_File_isEqual(FileHandle::Reference(lhs->m_handle), FileHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// File::operator != (static)

bool File::operator!=(File^ lhs, File^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	// clang_File_isEqual returns zero if the files are different
	return (clang_File_isEqual(FileHandle::Reference(lhs->m_handle), FileHandle::Reference(rhs->m_handle)) == 0);
}

//---------------------------------------------------------------------------
// File::Create (internal, static)
//
// Creates a new File instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	file		- Unmanaged CXFile instance

File^ File::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXFile file)
{
	return gcnew File(gcnew FileHandle(owner, transunit, file));
}

//---------------------------------------------------------------------------
// File::EnumerateIncludeDirectives
//
// Enumerates the #import/#include directives in this file
//
// Arguments:
//
//	func	- Delegate function to invoke for every enumerated cursor/extent

void File::EnumerateIncludeDirectives(Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func)
{
	// If the file or delegate function are null, there is nothing to do
	if((IsNull(this)) || (Object::ReferenceEquals(func, nullptr))) return;

	FileHandle::Reference	file(m_handle);				// Unwrap the safe handle

	// Use a wrapper object for the delegate and visit the included files
	EnumerateIncludeDirectivesFunc^ enumfunc = EnumerateIncludeDirectivesFunc::Create(m_handle->Owner, m_handle->TranslationUnit, file, func);
	AutoGCHandle handle(enumfunc);

	clang_findIncludesInFile(file.TranslationUnit, file, { handle, EnumerateIncludeDirectivesCallback });

	// Check if an exception occurred during enumeration and re-throw it
	Exception^ exception = enumfunc->Exception;
	if(!Object::ReferenceEquals(exception, nullptr)) throw exception;
}

//---------------------------------------------------------------------------
// File::Equals
//
// Compares this File instance to another File instance
//
// Arguments:
//
//	rhs		- Right-hand File instance to compare against

bool File::Equals(File^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// File::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool File::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a File instance
	File^ rhsref = dynamic_cast<File^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// File::GetExtent
//
// Obtain an Extent from this file
//
// Arguments:
//
//	file			- File instance within this translation unit
//	startoffset		- Starting character offset within the file
//	endoffset		- Ending character offset within the file

Extent^ File::GetExtent(int startoffset, int endoffset)
{
	// Create a new extent from the specified starting and ending offsets
	FileHandle::Reference file(m_handle);
	return Extent::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getRange(clang_getLocationForOffset(file.TranslationUnit, file, startoffset), 
		clang_getLocationForOffset(file.TranslationUnit, file, endoffset)));
}

//---------------------------------------------------------------------------
// File::GetExtent
//
// Obtain an Extent from this translation unit
//
// Arguments:
//
//	startline		- Starting line position within the file
//	startcolumn		- Starting column position within the file
//	endline			- Ending line position within the file
//	endcolumn		- Ending column position within the file

Extent^ File::GetExtent(int startline, int startcolumn, int endline, int endcolumn)
{
	// Create a new extent from the specified starting and ending positions
	FileHandle::Reference file(m_handle);
	return Extent::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getRange(clang_getLocation(file.TranslationUnit, file, startline, startcolumn), 
		clang_getLocation(file.TranslationUnit, file, endline, endcolumn)));
}

//---------------------------------------------------------------------------
// File::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int File::GetHashCode(void)
{
	// Use the unique identifier to create a hash code for the file
	return this->UniqueIdentifier.GetHashCode();
}

//---------------------------------------------------------------------------
// File::GetLocation
//
// Obtain a Location from this file
//
// Arguments:
//
//	offset			- Offset within the file

Location^ File::GetLocation(int offset)
{
	// Create a new (spelling) location from the specified offset
	FileHandle::Reference file(m_handle);
	return Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getLocationForOffset(file.TranslationUnit, file, offset), LocationKind::Spelling);
}

//---------------------------------------------------------------------------
// File::GetLocation
//
// Obtain a Location from this translation unit
//
// Arguments:
//
//	line			- Line position within the file
//	column			- Column position within the file

Location^ File::GetLocation(int line, int column)
{
	// Create a new (spelling) location from the specified position
	FileHandle::Reference file(m_handle);
	return Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getLocation(file.TranslationUnit, file, line, column), LocationKind::Spelling);
}

//---------------------------------------------------------------------------
// File::IsMultipleIncludeGuarded::get
//
// Indicates if the given file is guarded against multiple inclusions

bool File::IsMultipleIncludeGuarded::get(void)
{
	FileHandle::Reference file(m_handle);
	return (clang_isFileMultipleIncludeGuarded(file.TranslationUnit, file) != 0);
}

//---------------------------------------------------------------------------
// File::IsNull (static)
//
// Determines if the file is null
//
// Arguments:
//
//	rhs		- File instance to be checked for null

bool File::IsNull(File^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return static_cast<CXFile>(FileHandle::Reference(rhs->m_handle)) == __nullptr;
}

//---------------------------------------------------------------------------
// File::Module::get
//
// Gets the module associated with this file

local::Module^ File::Module::get(void)
{
	if(Object::ReferenceEquals(m_module, nullptr)) {

		FileHandle::Reference file(m_handle);
		m_module = local::Module::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getModuleForFile(file.TranslationUnit, file));
	}

	return m_module;
}

//---------------------------------------------------------------------------
// File::Name::get
//
// Gets the name of the file

String^ File::Name::get(void)
{
	if(Object::ReferenceEquals(m_name, nullptr))
		m_name = StringUtil::ToString(clang_getFileName(FileHandle::Reference(m_handle)));

	return m_name;
}

//---------------------------------------------------------------------------
// File::SkippedExtents::get
//
// Gets a collection of all extents skipped by the preprocessor

ExtentCollection^ File::SkippedExtents::get(void)
{
	if(Object::ReferenceEquals(m_skipped, nullptr)) {

		FileHandle::Reference file(m_handle);
		m_skipped = ExtentCollection::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getSkippedRanges(file.TranslationUnit, file));
	}

	return m_skipped;
}

//---------------------------------------------------------------------------
// File::TimeStamp::get
//
// Gets the last modification date/time stamp of the file

DateTime File::TimeStamp::get(void)
{
	return DateTime::FromFileTime(Int32x32To64(clang_getFileTime(FileHandle::Reference(m_handle)), 10000000) + 116444736000000000);
}

//---------------------------------------------------------------------------
// File::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ File::ToString(void)
{
	return (IsNull(this)) ? "<null>" : this->Name;
}

//---------------------------------------------------------------------------
// File::UniqueIdentifier::get
//
// Gets the unique identifier for this file

FileUniqueIdentifier File::UniqueIdentifier::get(void)
{
	CXFileUniqueID uniqueid{ /* data[3] */ { 0, 0, 0 } };

	clang_getFileUniqueID(FileHandle::Reference(m_handle), &uniqueid);
	return FileUniqueIdentifier(uniqueid);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
