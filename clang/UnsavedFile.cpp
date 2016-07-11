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
#include "UnsavedFile.h"

#include "StringUtil.h"

using namespace System::Linq;

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// UnsavedFile Constructor
//
// Arguments:
//
//	filename		- Name to assign to the unsaved file content
//	content			- Unsaved file content as a string

UnsavedFile::UnsavedFile(String^ filename, String^ content) : m_filename(filename), m_content(content)
{
	if(Object::ReferenceEquals(filename, nullptr)) throw gcnew ArgumentNullException("filename");
	if(Object::ReferenceEquals(content, nullptr)) throw gcnew ArgumentNullException("content");
}

//---------------------------------------------------------------------------
// UnsavedFile Constructor
//
// Arguments:
//
//	filename		- Name to assign to the unsaved file content
//	contentreader	- TextReader from which to read the content

UnsavedFile::UnsavedFile(String^ filename, TextReader^ contentreader) : m_filename(filename)
{
	if(Object::ReferenceEquals(filename, nullptr)) throw gcnew ArgumentNullException("filename");
	if(Object::ReferenceEquals(contentreader, nullptr)) throw gcnew ArgumentNullException("contentreader");

	m_content = contentreader->ReadToEnd();
}

//---------------------------------------------------------------------------
// UnsavedFile::Content::get
//
// Gets the unsaved file content string

String^ UnsavedFile::Content::get(void)
{
	return m_content;
}

//---------------------------------------------------------------------------
// UnsavedFile::Content::set
//
// Sets the unsaved file content string

void UnsavedFile::Content::set(String^ value)
{
	m_content = value;
}

//---------------------------------------------------------------------------
// UnsavedFile::FileName::get
//
// Gets the name associated with the unsaved file

String^ UnsavedFile::FileName::get(void)
{
	return m_filename;
}

//---------------------------------------------------------------------------
// UnsavedFile::FileName::set
//
// Sets the name associated with the unsaved file

void UnsavedFile::FileName::set(String^ value)
{
	m_filename = value;
}

//---------------------------------------------------------------------------
// UnsavedFile::FreeUnsavedFilesArray (private, static)
//
// Releases unmanaged memory allocated with UnsavedFilesToArray
//
// Arguments:
//
//	rg			- Pointer returned from EnumerableUnsavedFileToArray
//	numunsaved	- Size of the array pointed to by rg

CXUnsavedFile* UnsavedFile::FreeUnsavedFilesArray(CXUnsavedFile* rg, int numunsaved)
{
	if(rg != __nullptr) {
		
		// The embedded string pointers have to be released
		for(int index = 0; index < numunsaved; index++) {

			StringUtil::FreeCharPointer(const_cast<char*>(rg[index].Filename));
			StringUtil::FreeCharPointer(const_cast<char*>(rg[index].Contents));
		}

		delete[] rg;
	}

	return __nullptr;
}

//---------------------------------------------------------------------------
// UnsavedFile::UnsavedFilesToArray (internal, static)
//
// Converts an enumerable collection of UnsavedFile objects into an unmanaged array
//
// Arguments:
//
//	unsavedfiles		- Enumerable collection of UnsavedFile objects
//	numunsaved			- Receives the number of items in the resultant array

CXUnsavedFile* UnsavedFile::UnsavedFilesToArray(IEnumerable<UnsavedFile^>^ unsavedfiles, int* numunsaved)
{
	CXUnsavedFile*				unsaved;				// Array to be returned to the caller

	// FreeUnsavedFilesArray needs to know the count, the caller has to maintain that
	if(numunsaved == __nullptr) throw gcnew ArgumentNullException("numunsaved");
	else *numunsaved = 0;

	// If a null reference was provided for the enumerable set, return null to the caller
	if(Object::ReferenceEquals(unsavedfiles, nullptr)) return __nullptr;

	// Convert the IEnumerable into a fixed-length array to ensure the size cannot change
	array<UnsavedFile^>^ files = Enumerable::ToArray(unsavedfiles);
	if(files->Length == 0) return __nullptr;

	// Allocate the array to be returned to the caller
	try { unsaved = new CXUnsavedFile[files->Length]; }
	catch(Exception^) { throw gcnew OutOfMemoryException(); }

	*numunsaved = files->Length;				// Set the size of the unmanaged array

	// Iterate over each UnsavedFile in the array and convert into CXUnsavedFiles, while this
	// does duplicate the strings in memory temporarily it allows UnsavedFile to not maintain
	// a CXUnsavedFile structure, eliminating another finalizer
	for(int index = 0; index < files->Length; index++) {

		unsaved[index].Filename = StringUtil::ToCharPointer(files[index]->FileName, CP_UTF8);
		unsaved[index].Contents = StringUtil::ToCharPointer(files[index]->Content, CP_UTF8);

	#ifdef _M_X64
		// On x64 size_t can exceed unsigned long, don't throw an exception just truncate it
		size_t length = (unsaved[index].Contents) ? strlen(unsaved[index].Contents) : 0;
		unsaved[index].Length = static_cast<unsigned long>((length > UInt32::MaxValue) ? static_cast<size_t>(UInt32::MaxValue) : length);
	#else
		// On x86 size_t and unsigned long are the same width
		unsaved[index].Length = (unsaved[index].Contents) ? strlen(unsaved[index].Contents) : 0;
	#endif
	}

	return unsaved;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
