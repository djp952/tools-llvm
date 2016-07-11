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
#include "CommentCollection.h"

#include "Comment.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CommentCollection Constructor (private)
//
// Arguments:
//
//	handle		- Comment reference handle

CommentCollection::CommentCollection(CommentHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<Comment^>(clang_Comment_getNumChildren(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CommentCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Comment^ CommentCollection::default::get(int index)
{
	// Check for a cached instance of the Comment first
	Comment^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new Comment and cache it to prevent multiple creations
	m_cache[index] = Comment::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Comment_getChild(CommentHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CommentCollection::Count::get
//
// Gets the number of elements in the collection

int CommentCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CommentCollection::Create (internal, static)
//
// Creates a new CommentCollection
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	comment		- Unmanaged CXComment instance

CommentCollection^ CommentCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment)
{
	return gcnew CommentCollection(gcnew CommentHandle(owner, transunit, comment));
}

//---------------------------------------------------------------------------
// CommentCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Comment^>^ CommentCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<Comment^>(this);
}

//---------------------------------------------------------------------------
// CommentCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CommentCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
