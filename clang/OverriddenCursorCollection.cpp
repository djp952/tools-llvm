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
#include "OverriddenCursorCollection.h"

#include "Cursor.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// OverriddenCursorCollection Constructor (private)
//
// Arguments:
//
//	cursors		- Existing List<> of Cursor instances to wrap

OverriddenCursorCollection::OverriddenCursorCollection(List<Cursor^>^ cursors) : m_cursors(cursors)
{
	if(Object::ReferenceEquals(cursors, nullptr)) throw gcnew ArgumentNullException("cursors");
}

//---------------------------------------------------------------------------
// OverriddenCursorCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Cursor^ OverriddenCursorCollection::default::get(int index)
{
	return m_cursors->default[index];
}

//---------------------------------------------------------------------------
// OverriddenCursorCollection::Count::get
//
// Gets the number of elements in the collection

int OverriddenCursorCollection::Count::get(void)
{
	return m_cursors->Count;
}

//---------------------------------------------------------------------------
// OverriddenCursorCollection::Create (internal, static)
//
// Creates a new OverriddenCursorCollection from an existing unmanaged CXCursor array
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	cursors		- Array of unmanaged CXCursor structures
//	count		- Number of elements in the CXCursor array

CursorCollection^ OverriddenCursorCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor* cursors, unsigned int count)
{
	// Create a List<> of cursors from the provided CXCursor instances
	List<Cursor^>^ list = gcnew List<Cursor^>(count);
	for(unsigned int index = 0; index < count; index++) list->Add(Cursor::Create(owner, transunit, cursors[index]));

	// Generate the OverriddenCursorCollection instance using the List<>
	return gcnew OverriddenCursorCollection(list);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
