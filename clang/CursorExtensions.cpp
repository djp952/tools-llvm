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
#include "CursorExtensions.h"

#include "Cursor.h"
#include "EnumerateChildrenResult.h"
#include "EnumerateReferencesResult.h"
#include "Extent.h"
#include "File.h"
#include "TokenCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// CursorExtensions::ChildCollector Constructor
//
// Arguments:
//
//	predicate		- Predicate delegate used to filter during collection adds
//	recursive		- Flag if this is a recursive find operation or not

CursorExtensions::ChildCollector::ChildCollector(Func<Cursor^, Cursor^, bool>^ predicate, bool recursive) :
	m_predicate(predicate), m_recursive(recursive)
{
}

//---------------------------------------------------------------------------
// CursorExtensions::ChildCollector::EnumerateChildrenCallback
//
// Callback delegate passed into Cursor::EnumerateChildren
//
// Arguments:
//
//	cursor			- Current cursor being enumerated
//	parent			- Parent cursor to the enumerated cursor

EnumerateChildrenResult CursorExtensions::ChildCollector::EnumerateChildrenCallback(Cursor^ cursor, Cursor^ parent)
{
	// Determine if the enumerated cursor instance should be collected or not
	bool addcursor = (Object::ReferenceEquals(m_predicate, nullptr)) ? true : m_predicate(cursor, parent);
	if(addcursor) List::Add(Tuple::Create(cursor, parent));

	// Return either Recurse or Continue based on if this is a recursive find operation
	return (m_recursive) ? EnumerateChildrenResult::Recurse : EnumerateChildrenResult::Continue;
}

//---------------------------------------------------------------------------
// CursorExtensions::ReferenceCollector Constructor
//
// Arguments:
//
//	predicate		- Predicate delegate used to filter during collection adds

CursorExtensions::ReferenceCollector::ReferenceCollector(Func<Cursor^, Extent^, bool>^ predicate) : m_predicate(predicate)
{
}

//---------------------------------------------------------------------------
// CursorExtensions::ReferenceCollector::EnumerateReferenceCallback
//
// Callback delegate passed into Cursor::EnumerateChildren
//
// Arguments:
//
//	cursor			- Current cursor being enumerated
//	extent			- Extent of the cursor being enumerated

EnumerateReferencesResult CursorExtensions::ReferenceCollector::EnumerateReferenceCallback(Cursor^ cursor, Extent^ extent)
{
	// Determine if the enumerated cursor instance should be collected or not
	bool addcursor = (Object::ReferenceEquals(m_predicate, nullptr)) ? true : m_predicate(cursor, extent);
	if(addcursor) List::Add(Tuple::Create(cursor, extent));

	return EnumerateReferencesResult::Continue;		// Continue the enumeration
}

//---------------------------------------------------------------------------
// CursorExtensions::EnumerateReferences
//
// Enumerates references to this cursor in a specific file
//
// Arguments:
//
//	cursor		- Cursor instance being extended
//	file		- File in which to search for the directives
//	func		- Delegate function to invoke for every enumerated cursor/extent

void CursorExtensions::EnumerateReferences(Cursor^ cursor, File^ file, Func<Cursor^, local::Extent^, EnumerateReferencesResult>^ func)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	cursor->EnumerateReferences(file->Name, func);
}

//---------------------------------------------------------------------------
// CursorExtensions::FindChildren (static)
//
// Finds child cursors of this cursor based on a predicate
//
// Arguments:
//
//	cursor			- Cursor instance being extended
//	predicate		- Predicate delegate used to filter the children

ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ CursorExtensions::FindChildren(Cursor^ cursor, Func<Cursor^, Cursor^, bool>^ predicate)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return FindChildren(cursor, predicate, false);
}

//---------------------------------------------------------------------------
// CursorExtensions::FindChildren (static)
//
// Finds child cursors of this cursor based on a predicate
//
// Arguments:
//
//	cursor			- Cursor instance being extended
//	predicate		- Predicate delegate used to filter the children
//	recursive		- Flag to recursively enumerate children or not

ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ CursorExtensions::FindChildren(Cursor^ cursor, Func<Cursor^, Cursor^, bool>^ predicate, bool recursive)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");

	// Create a ChildCollector instance and use that to invoke Cursor::EnumerateChildren
	ChildCollector^ collector = gcnew ChildCollector(predicate, recursive);
	cursor->EnumerateChildren(gcnew Func<Cursor^, Cursor^, EnumerateChildrenResult>(collector, &ChildCollector::EnumerateChildrenCallback));

	return collector->AsReadOnly();				// Return the collection as read-only
}

//---------------------------------------------------------------------------
// CursorExtensions::FindReferences (static)
//
// Finds all file references to this cursor based on a predicate
//
// Arguments:
//
//	cursor		- Cursor instance being extended
//	file		- File in which to find the cursor references
//	predicate	- Predicate delegate used to filter the references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ CursorExtensions::FindReferences(Cursor^ cursor, File^ file, Func<Cursor^, Extent^, bool>^ predicate)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	if(Object::ReferenceEquals(file, nullptr)) throw gcnew ArgumentNullException("file");

	return FindReferences(cursor, file->Name, predicate);
}

//---------------------------------------------------------------------------
// CursorExtensions::FindReferences (static)
//
// Finds all file references to this cursor based on a predicate
//
// Arguments:
//
//	cursor		- Cursor instance being extended
//	filename	- File in which to find the cursor references
//	predicate	- Predicate delegate used to filter the references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ CursorExtensions::FindReferences(Cursor^ cursor, String^ filename, Func<Cursor^, Extent^, bool>^ predicate)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");

	// Create a ReferenceCollector instance and use that to invoke Cursor::EnumerateFileReferences
	ReferenceCollector^ collector = gcnew ReferenceCollector(predicate);
	cursor->EnumerateReferences(filename, gcnew Func<Cursor^, Extent^, EnumerateReferencesResult>(collector, &ReferenceCollector::EnumerateReferenceCallback));

	return collector->AsReadOnly();			// Return the collection as read-only
}

//---------------------------------------------------------------------------
// CursorExtensions::GetChildren (static)
//
// Gets all child cursors of this cursor
//
// Arguments:
//
//	cursor			- Cursor instance being extended

ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ CursorExtensions::GetChildren(Cursor^ cursor)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return FindChildren(cursor, nullptr, false);
}

//---------------------------------------------------------------------------
// CursorExtensions::GetChildren (static)
//
// Gets all child cursors of this cursor
//
// Arguments:
//
//	cursor			- Cursor instance being extended
//	recursive		- Flag to recursively enumerate children or not

ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ CursorExtensions::GetChildren(Cursor^ cursor, bool recursive)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return FindChildren(cursor, nullptr, recursive);
}

//---------------------------------------------------------------------------
// CursorExtensions::GetReferences (static)
//
// Gets all file references to this cursor
//
// Arguments:
//
//	cursor			- Cursor instance being extended
//	file			- File in which to find the cursor references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ CursorExtensions::GetReferences(Cursor^ cursor, File^ file)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return FindReferences(cursor, file, nullptr);
}

//---------------------------------------------------------------------------
// CursorExtensions::GetReferences (static)
//
// Gets all file references to this cursor
//
// Arguments:
//
//	cursor			- Cursor instance being extended
//	filename		- File in which to find the cursor references

ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ CursorExtensions::GetReferences(Cursor^ cursor, String^ filename)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return FindReferences(cursor, filename, nullptr);
}

//---------------------------------------------------------------------------
// CursorExtensions::GetTokens (static)
//
// Tokenizes a cursor extent
//
// Arguments:
//
//	cursor		- Cursor to be tokenized

TokenCollection^ CursorExtensions::GetTokens(Cursor^ cursor)
{
	if(Object::ReferenceEquals(cursor, nullptr)) throw gcnew ArgumentNullException("cursor");
	return cursor->Extent->GetTokens();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)
