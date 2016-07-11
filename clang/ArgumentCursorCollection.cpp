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
#include "ArgumentCursorCollection.h"

#include "Cursor.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ArgumentCursorCollection Constructor (private)
//
// Arguments:
//
//	handle		- Underlying CursorHandle instance

ArgumentCursorCollection::ArgumentCursorCollection(CursorHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// clang_Cursor_getNumArguments will return -1 if this isn't something that would have
	// arguments, convert that to a zero element collection and don't throw an exception
	m_cache = gcnew array<Cursor^>(Math::Max(clang_Cursor_getNumArguments(CursorHandle::Reference(handle)), 0));
}

//---------------------------------------------------------------------------
// ArgumentCursorCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Cursor^ ArgumentCursorCollection::default::get(int index)
{
	// Check for a cached instance of the Cursor first
	Cursor^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new Cursor and cache it to prevent multiple creations
	m_cache[index] = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Cursor_getArgument(CursorHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// ArgumentCursorCollection::Count::get
//
// Gets the number of elements in the collection

int ArgumentCursorCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// ArgumentCursorCollection::Create (internal, static)
//
// Creates a new ArgumentCursorCollection
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	cursor		- Unmanaged CXCursor instance

CursorCollection^ ArgumentCursorCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor)
{
	return gcnew ArgumentCursorCollection(gcnew CursorHandle(owner, transunit, cursor));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
