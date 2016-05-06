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
#include "TranslationUnit.h"

#include "AutoGCHandle.h"
#include "CompletionOptions.h"
#include "CompletionResultCollection.h"
#include "Cursor.h"
#include "DiagnosticCollection.h"
#include "EnumerateIncludedFileFunc.h"
#include "Extent.h"
#include "File.h"
#include "GCHandleRef.h"
#include "Location.h"
#include "LocationCollection.h"
#include "ResourceUsageDictionary.h"
#include "StringUtil.h"
#include "TokenCollection.h"
#include "TranslationUnitDiagnosticCollection.h"
#include "TranslationUnitHandle.h"
#include "TranslationUnitSaveException.h"
#include "TranslationUnitSaveOptions.h"
#include "UnsavedFile.h"

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateIncludedFileCallback (local)
//
// Callback for clang_getInclusions() that invokes a delegate function against
// each included file instance that is returned
//
// Arguments:
//
//	file			- Unmanaged CXFile instance
//	references		- Array of locations where the inclusion is referenced
//	numreferences	- Number of locations in the provided array
//	context			- Context pointer passed into clang_getInclusions

static void EnumerateIncludedFileCallback(CXFile file, CXSourceLocation* references, unsigned int numreferences, CXClientData context)
{
	GCHandleRef<EnumerateIncludedFileFunc^> func(context);

	// Invoke the delegate and capture any exception that occurs
	try { func->Invoke(file, references, numreferences); }
	catch(Exception^ exception) { func->Exception = exception; }
}

//---------------------------------------------------------------------------
// TranslationUnit Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TranslationUnitHandle instance

TranslationUnit::TranslationUnit(TranslationUnitHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// TranslationUnit Destructor

TranslationUnit::~TranslationUnit()
{
	if(m_disposed) return;

	delete m_diags;						// Dispose of the diagnostic collection
	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// TranslationUnit::CompleteAt
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	filename		- Name of the file within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file

CompletionResultCollection^ TranslationUnit::CompleteAt(String^ filename, int line, int column)
{
	CHECK_DISPOSED(m_disposed);
	return CompleteAt(filename, line, column, nullptr, static_cast<CompletionOptions>(-1));
}

//---------------------------------------------------------------------------
// TranslationUnit::CompleteAt
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	filename		- Name of the file within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	unsavedfiles	- Collection of unsaved code files

CompletionResultCollection^ TranslationUnit::CompleteAt(String^ filename, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	CHECK_DISPOSED(m_disposed);
	return CompleteAt(filename, line, column, unsavedfiles, static_cast<CompletionOptions>(-1));
}

//---------------------------------------------------------------------------
// TranslationUnit::CompleteAt
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	filename		- Name of the file within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	options			- Code completion options

CompletionResultCollection^ TranslationUnit::CompleteAt(String^ filename, int line, int column, CompletionOptions options)
{
	CHECK_DISPOSED(m_disposed);
	return CompleteAt(filename, line, column, nullptr, options);
}

//---------------------------------------------------------------------------
// TranslationUnit::CompleteAt
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	filename		- Name of the file within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	unsavedfiles	- Collection of unsaved code files
//	options			- Code completion options

CompletionResultCollection^ TranslationUnit::CompleteAt(String^ filename, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(filename, nullptr)) throw gcnew ArgumentNullException("filename");
	
	// Line and column are passed as signed integers for consisency with Location, but they cannot be negative
	if(line < 0) throw gcnew ArgumentOutOfRangeException("line");
	if(column < 0) throw gcnew ArgumentOutOfRangeException("column");

	// If the special -1 option was specified, ask libclang to provide a default options mask
	if(options == static_cast<CompletionOptions>(-1)) options = static_cast<CompletionOptions>(clang_defaultCodeCompleteOptions());

	// Convert the managed filename into a standard C-style string to pass into the API
	char* pszfilename = StringUtil::ToCharPointer(filename, CP_UTF8);

	try { 

		// Convert the enumerable range of UnsavedFile objects into an unmanaged array
		int	numunsaved = 0;
		CXUnsavedFile* rgunsaved = UnsavedFile::UnsavedFilesToArray(unsavedfiles, &numunsaved);

		try {
		
			// Create the code completion results without the custom sort alphabetical flag
			CXCodeCompleteResults* results = clang_codeCompleteAt(TranslationUnitHandle::Reference(m_handle), pszfilename,
				static_cast<unsigned int>(line), static_cast<unsigned int>(column), rgunsaved, numunsaved, 
				static_cast<unsigned int>(options & ~CompletionOptions::SortAlphabetical));

			// Handle the custom sort alphabetical flag before transferring ownership to the collection, the
			// pointers within the result set have to remain stable
			if((options & CompletionOptions::SortAlphabetical) == CompletionOptions::SortAlphabetical) {

				if(results != __nullptr) clang_sortCodeCompletionResults(results->Results, results->NumResults);
			}

			// Wrap the code completion results into a CompletionResultCollection instance
			return CompletionResultCollection::Create(m_handle, m_handle, std::move(results));
		} 

		finally { UnsavedFile::FreeUnsavedFilesArray(rgunsaved, numunsaved);  }
	}

	finally { StringUtil::FreeCharPointer(pszfilename); }
}

//---------------------------------------------------------------------------
// TranslationUnit::Create (internal, static)
//
// Creates a new TranslationUnit instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- Unmanaged CXTranslationUnit instance to take ownership of

TranslationUnit^ TranslationUnit::Create(SafeHandle^ owner, CXTranslationUnit&& transunit)
{
	return gcnew TranslationUnit(gcnew TranslationUnitHandle(owner, std::move(transunit)));
}

//---------------------------------------------------------------------------
// TranslationUnit::Cursor::get
//
// Gets the cursor that represents the given translation unit

local::Cursor^ TranslationUnit::Cursor::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle, m_handle, clang_getTranslationUnitCursor(TranslationUnitHandle::Reference(m_handle)));
	
	return m_cursor;
}

//---------------------------------------------------------------------------
// TranslationUnit::Diagnostics::get
//
// Gets a read only collection of translation unit diagnostics

DiagnosticCollection^ TranslationUnit::Diagnostics::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_diags, nullptr)) m_diags = TranslationUnitDiagnosticCollection::Create(m_handle);
	return m_diags;
}

//---------------------------------------------------------------------------
// TranslationUnit::EnumerateIncludedFiles
//
// Enumerates the included files of this translation unit
//
// Arguments:
//
//	action		- Delegate function to invoke for every enumerated inclusion

void TranslationUnit::EnumerateIncludedFiles(Action<File^, LocationCollection^>^ action)
{
	CHECK_DISPOSED(m_disposed);

	// Use a wrapper object for the delegate and visit the included files
	EnumerateIncludedFileFunc^ enumfunc = gcnew EnumerateIncludedFileFunc(m_handle, action);
	clang_getInclusions(TranslationUnitHandle::Reference(m_handle), EnumerateIncludedFileCallback, AutoGCHandle(enumfunc));

	// Check if an exception occurred during enumeration and re-throw it
	Exception^ exception = enumfunc->Exception;
	if(!Object::ReferenceEquals(exception, nullptr)) throw exception;
}
	
//---------------------------------------------------------------------------
// TranslationUnit::GetFile
//
// Gets a file from this translation unit
//
// Arguments:
//
//	filename		- File name to retrieve from the translation unit

File^ TranslationUnit::GetFile(String^ filename)
{
	CHECK_DISPOSED(m_disposed);
	if(Object::ReferenceEquals(filename, nullptr)) throw gcnew ArgumentNullException("filename");

	char* pszname = StringUtil::ToCharPointer(filename, CP_UTF8);

	try { return File::Create(m_handle, m_handle, clang_getFile(TranslationUnitHandle::Reference(m_handle), pszname)); }
	finally { StringUtil::FreeCharPointer(pszname); }
}

//---------------------------------------------------------------------------
// TranslationUnit::Handle::get
//
// Exposes the underlying TranslationUnitHandle

TranslationUnitHandle^ TranslationUnit::Handle::get(void)
{
	return m_handle;
}
	
//---------------------------------------------------------------------------
// TranslationUnit::ResourceUsage::get
//
// Gets the translation unit resource usage

ResourceUsageDictionary^ TranslationUnit::ResourceUsage::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_usage, nullptr))
		m_usage = ResourceUsageDictionary::Create(clang_getCXTUResourceUsage(TranslationUnitHandle::Reference(m_handle)));

	return m_usage;
}

//---------------------------------------------------------------------------
// TranslationUnit::Save
//
// Serializes the translation unit into an output file
//
// Arguments:
//
//	path		- Output file path

void TranslationUnit::Save(String^ path)
{
	CHECK_DISPOSED(m_disposed);
	return Save(path, static_cast<TranslationUnitSaveOptions>(-1));
}

//---------------------------------------------------------------------------
// TranslationUnit::Save
//
// Serializes the translation unit into an output file
//
// Arguments:
//
//	path		- Output file path
//	options		- Output file processing flags

void TranslationUnit::Save(String^ path, TranslationUnitSaveOptions options)
{
	CHECK_DISPOSED(m_disposed);
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");

	TranslationUnitHandle::Reference tu(m_handle);

	// If the special -1 option was specified, ask clang to provide the default options
	if(options == static_cast<TranslationUnitSaveOptions>(-1))
		options = static_cast<TranslationUnitSaveOptions>(clang_defaultSaveOptions(tu));

	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try {

		CXSaveError result = static_cast<CXSaveError>(clang_saveTranslationUnit(tu, pszpath, static_cast<unsigned int>(options)));
		if(result != CXSaveError::CXSaveError_None) throw gcnew TranslationUnitSaveException(result);
	
	} finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// TranslationUnit::Spelling::get
//
// Get the original translation unit source file name

String^ TranslationUnit::Spelling::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_spelling, nullptr)) 
		m_spelling = StringUtil::ToString(clang_getTranslationUnitSpelling(TranslationUnitHandle::Reference(m_handle)));

	return m_spelling;
}

//---------------------------------------------------------------------------
// TranslationUnit::ToString
//
// Overrides Object::ToString
//
// Arguments:
//
//	NONE

String^ TranslationUnit::ToString(void)
{
	return (m_disposed) ? "<disposed>" : this->Spelling;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
