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
#include "TemplateArgumentCollection.h"

#include "TemplateArgument.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// TemplateArgumentCollection Constructor (private)
//
// Arguments:
//
//	handle		- Underlying cursor object handle

TemplateArgumentCollection::TemplateArgumentCollection(CursorHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// clang_Cursor_getNumTemplateArguments will return -1 if this isn't something that would have
	// template arguments, convert that to a zero element collection and don't throw an exception
	m_cache = gcnew array<TemplateArgument^>(Math::Max(clang_Cursor_getNumTemplateArguments(CursorHandle::Reference(handle)), 0));
}

//---------------------------------------------------------------------------
// TemplateArgumentCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

TemplateArgument^ TemplateArgumentCollection::default::get(int index)
{
	// Check for a cached instance of the TemplateArgument first
	TemplateArgument^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new TemplateArgument and cache it to prevent multiple creations
	m_cache[index] = TemplateArgument::Create(m_handle->Owner, m_handle->TranslationUnit, CursorHandle::Reference(m_handle), static_cast<unsigned int>(index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// TemplateArgumentCollection::Count::get
//
// Gets the number of elements in the collection

int TemplateArgumentCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// TemplateArgumentCollection::Create (internal, static)
//
// Creates a new TemplateArgumentCollection
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	cursor		- Unmanaged CXCursor instance

TemplateArgumentCollection^ TemplateArgumentCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor)
{
	return gcnew TemplateArgumentCollection(gcnew CursorHandle(owner, transunit, cursor));
}

//---------------------------------------------------------------------------
// TemplateArgumentCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<TemplateArgument^>^ TemplateArgumentCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<TemplateArgument^>(this);
}

//---------------------------------------------------------------------------
// TemplateArgumentCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ TemplateArgumentCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
