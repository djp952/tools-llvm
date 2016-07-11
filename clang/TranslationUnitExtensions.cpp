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
#include "TranslationUnitExtensions.h"

#include "CompletionOptions.h"
#include "CompletionResultCollection.h"
#include "Cursor.h"
#include "Extent.h"
#include "File.h"
#include "Location.h"
#include "LocationCollection.h"
#include "TokenCollection.h"
#include "TranslationUnit.h"
#include "UnsavedFile.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// TranslationUnitExtensions::IncludedFileCollector Constructor
//
// Arguments:
//
//	predicate		- Predicate delegate used to filter during collection adds

TranslationUnitExtensions::IncludedFileCollector::IncludedFileCollector(Func<File^, LocationCollection^, bool>^ predicate) : m_predicate(predicate)
{
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::IncludedFileCollector::EnumerateIncludedFileCallback
//
// Callback delegate passed into TranslationUnit::EnumerateIncludedFiles
//
// Arguments:
//
//	file		- File object of the include being enumerated
//	refchain	- File object reference chain

void TranslationUnitExtensions::IncludedFileCollector::EnumerateIncludedFileCallback(File^ file, LocationCollection^ refchain)
{
	// Determine if the enumerated included file instance should be collected or not
	bool addfile = (Object::ReferenceEquals(m_predicate, nullptr)) ? true : m_predicate(file, refchain);
	if(addfile) List::Add(Tuple::Create(file, refchain));
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	location		- Location at which to perform completion


CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, Location^ location)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(location, nullptr)) throw gcnew ArgumentNullException("location");
	
	return transunit->CompleteAt(location->File->Name, location->Line, location->Column);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	location		- Location at which to perform completion
//	unsavedfiles	- Collection of unsaved code files

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, Location^ location, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(location, nullptr)) throw gcnew ArgumentNullException("location");
	
	return transunit->CompleteAt(location->File->Name, location->Line, location->Column, unsavedfiles);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	location		- Location at which to perform completion
//	options			- Code completion options

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, Location^ location, CompletionOptions options)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(location, nullptr)) throw gcnew ArgumentNullException("location");
	
	return transunit->CompleteAt(location->File->Name, location->Line, location->Column, options);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	location		- Location at which to perform completion
//	unsavedfiles	- Collection of unsaved code files
//	options			- Code completion options

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, Location^ location, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(location, nullptr)) throw gcnew ArgumentNullException("location");
	
	return transunit->CompleteAt(location->File->Name, location->Line, location->Column, unsavedfiles, options);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	file			- File within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	return transunit->CompleteAt(file->Name, line, column);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	file			- File within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	unsavedfiles	- Collection of unsaved code files

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	return transunit->CompleteAt(file->Name, line, column, unsavedfiles);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	file			- File within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	options			- Code completion options

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, CompletionOptions options)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	return transunit->CompleteAt(file->Name, line, column, options);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::CompleteAt (static)
//
// Perform code completion at a given location in the translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	file			- File within the translation unit
//	line			- Line position within the file
//	column			- Column position within the file
//	unsavedfiles	- Collection of unsaved code files
//	options			- Code completion options

CompletionResultCollection^ TranslationUnitExtensions::CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	return transunit->CompleteAt(file->Name, line, column, unsavedfiles, options);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::FindIncludedFiles (static)
//
// Finds included files based on a predicate
//
// Arguments:
//
//	cursor		- Cursor instance being extended
//	file		- File in which to find the cursor references
//	predicate	- Predicate delegate used to filter the references

ReadOnlyCollection<Tuple<File^, LocationCollection^>^>^ TranslationUnitExtensions::FindIncludedFiles(TranslationUnit^ transunit, 
	Func<File^, LocationCollection^, bool>^ predicate)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");

	// Create an IncludedFileCollector instance and use that to invoke TranslationUnit::EnumerateIncludedFiles
	IncludedFileCollector^ collector = gcnew IncludedFileCollector(predicate);
	transunit->EnumerateIncludedFiles(gcnew Action<File^, LocationCollection^>(collector, &IncludedFileCollector::EnumerateIncludedFileCallback));

	return collector->AsReadOnly();			// Return the collection as read-only
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetExtent (static)
//
// Obtain an Extent from a translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance
//	filename		- File name to retrieve from the translation unit
//	startoffset		- Starting character offset within the file
//	endoffset		- Ending character offset within the file

Extent^ TranslationUnitExtensions::GetExtent(TranslationUnit^ transunit, String^ filename, int startoffset, int endoffset)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->GetFile(filename)->GetExtent(startoffset, endoffset);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetExtent (static)
//
// Obtain an Extent from this translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended
//	filename		- File name to retrieve from the translation unit
//	startline		- Starting line position within the file
//	startcolumn		- Starting column position within the file
//	endline			- Ending line position within the file
//	endcolumn		- Ending column position within the file

Extent^ TranslationUnitExtensions::GetExtent(TranslationUnit^ transunit, String^ filename, int startline, int startcolumn, int endline, int endcolumn)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->GetFile(filename)->GetExtent(startline, startcolumn, endline, endcolumn);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetIncludedFiles (static)
//
// Gets all translation unit included files
//
// Arguments:
//
//	transunit		- TranslationUnit instance being extended

ReadOnlyCollection<Tuple<File^, LocationCollection^>^>^ TranslationUnitExtensions::GetIncludedFiles(TranslationUnit^ transunit)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return FindIncludedFiles(transunit, nullptr);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetLocation (static)
//
// Obtain a Location from a translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance
//	filename		- File name to retrieve from the translation unit
//	offset			- Character offset within the file

Location^ TranslationUnitExtensions::GetLocation(TranslationUnit^ transunit, String^ filename, int offset)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->GetFile(filename)->GetLocation(offset);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetLocation (static)
//
// Obtain an Extent from this translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit instance
//	filename		- File name to retrieve from the translation unit
//	line			- Line position within the file
//	column			- Column position within the file

Location^ TranslationUnitExtensions::GetLocation(TranslationUnit^ transunit, String^ filename, int line, int column)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->GetFile(filename)->GetLocation(line, column);
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::GetTokens (static)
//
// Tokenizes an entire translation unit
//
// Arguments:
//
//	transunit		- TranslationUnit to be tokenized

TokenCollection^ TranslationUnitExtensions::GetTokens(TranslationUnit^ transunit)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->Cursor->Extent->GetTokens();
}

//---------------------------------------------------------------------------
// TranslationUnitExtensions::IsFileMultipleIncludeGuarded (static)
//
// Indicates if the given file is guarded against multiple inclusions
//
// Arguments:
//
//	transunit	- TranslationUnit object to extend
//	filename	- Name of the file within the translation unit

bool TranslationUnitExtensions::IsFileMultipleIncludeGuarded(TranslationUnit^ transunit, String^ filename)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");
	return transunit->GetFile(filename)->IsMultipleIncludeGuarded;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)
