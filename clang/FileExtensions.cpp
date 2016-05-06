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
#include "FileExtensions.h"

#include "Cursor.h"
#include "CursorExtensions.h"
#include "EnumerateIncludeDirectivesResult.h"
#include "EnumerateReferencesResult.h"
#include "Extent.h"
#include "File.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// FileExtensions::IncludeDirectiveCollector Constructor
//
// Arguments:
//
//	predicate		- Predicate delegate used to filter during collection adds

FileExtensions::IncludeDirectiveCollector::IncludeDirectiveCollector(Func<Cursor^, Extent^, bool>^ predicate) : m_predicate(predicate)
{
}

//---------------------------------------------------------------------------
// FileExtensions::IncludeDirectiveCollector::EnumerateIncludeDirectiveCallback
//
// Callback delegate passed into Cursor::EnumerateChildren
//
// Arguments:
//
//	cursor			- Current cursor being enumerated
//	extent			- Extent of the cursor being enumerated

EnumerateIncludeDirectivesResult FileExtensions::IncludeDirectiveCollector::EnumerateIncludeDirectiveCallback(Cursor^ cursor, Extent^ extent)
{
	// Determine if the enumerated cursor instance should be collected or not
	bool addcursor = (Object::ReferenceEquals(m_predicate, nullptr)) ? true : m_predicate(cursor, extent);
	if(addcursor) List::Add(Tuple::Create(cursor, extent));

	return EnumerateIncludeDirectivesResult::Continue;		// Continue the enumeration
}

//---------------------------------------------------------------------------
// FileExtensions::EnumerateCursorReferences (static)
//
// Enumerates references to a cursor in a file
//
// Arguments:
//
//	file		- File instance
//	cursor		- Cursor to find the references to
//	func		- Delegate function to invoke for every enumerated cursor/extent

void FileExtensions::EnumerateCursorReferences(File^ file, Cursor^ cursor, Func<Cursor^, Extent^, EnumerateReferencesResult>^ func)
{
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	// func can be null

	// If the file is null or no delegate callback was provided, don't do anything
	if((File::IsNull(file)) || Object::ReferenceEquals(func, nullptr)) return;

	// Call through Cursor, passing in the File object's name
	cursor->EnumerateReferences(file->Name, func);
}

//---------------------------------------------------------------------------
// FileExtensions::FindCursorReferences (static)
//
// Finds all references to a cursor in a file based on a predicate
//
// Arguments:
//
//	file			- File instance being extended
//	cursor		- Cursor to locate reference(s) to in the file
//	predicate	- Predicate delegate used to filter the references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FileExtensions::FindCursorReferences(File^ file, Cursor^ cursor, Func<Cursor^, Extent^, bool>^ predicate)
{
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");

	// Call through the CursorExtension method
	return CursorExtensions::FindReferences(cursor, file, predicate);
}

//---------------------------------------------------------------------------
// FileExtensions::FindIncludeDirectives (static)
//
// Finds all include directives in the file based on a predicate
//
// Arguments:
//
//	file		- File instance being extended
//	predicate	- Predicate delegate used to filter the references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FileExtensions::FindIncludeDirectives(File^ file, Func<Cursor^, Extent^, bool>^ predicate)
{
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	// Create a IncludeDirectiveCollector instance and use that to invoke File::EnumerateIncludeDirectives
	IncludeDirectiveCollector^ collector = gcnew IncludeDirectiveCollector(predicate);
	file->EnumerateIncludeDirectives(gcnew Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>(collector, &IncludeDirectiveCollector::EnumerateIncludeDirectiveCallback));

	return collector->AsReadOnly();			// Return the collection as read-only
}

//---------------------------------------------------------------------------
// FileExtensions::GetCursorReferences (static)
//
// Gets all references to a cursor in a file
//
// Arguments:
//
//	file			- File instance being extended
//	cursor			- Cursor to locate reference(s) to in the file

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FileExtensions::GetCursorReferences(File^ file, Cursor^ cursor)
{
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");

	// Call through the CursorExtension method
	return CursorExtensions::GetReferences(cursor, file);
}

//---------------------------------------------------------------------------
// FileExtensions::GetIncludeDirectives (static)
//
// Gets all include directives in the file
//
// Arguments:
//
//	file		- File instance being extended

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FileExtensions::GetIncludeDirectives(File^ file)
{
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");
	return FindIncludeDirectives(file, nullptr);
}

//---------------------------------------------------------------------------

END_NAMESPACE(extensions)
END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
