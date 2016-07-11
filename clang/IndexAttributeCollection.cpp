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
#include "IndexAttributeCollection.h"

#include "IndexAttribute.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexAttributeCollection Constructor (private)
//
// Arguments:
//
//	handle			- IndexAttributeSetHandle for this collection
//	numattributes	- Number of attributes pointed to by the handle

IndexAttributeCollection::IndexAttributeCollection(IndexAttributeSetHandle^ handle, unsigned int numattributes) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// Create a cache to hold all created IndexAttribute collection elements
	m_cache = gcnew array<IndexAttribute^>(numattributes);
}

//---------------------------------------------------------------------------
// IndexAttributeCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

IndexAttribute^ IndexAttributeCollection::default::get(int index)
{
	// Check for a cached instance of the IndexAttribute first (will throw if index is out of range)
	IndexAttribute^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new IndexAttribute instance and cache it to prevent multiple creations
	m_cache[index] = IndexAttribute::Create(m_handle->Owner, m_handle->IndexAction, IndexAttributeSetHandle::Reference(m_handle)[index]);
	return m_cache[index];
}

//---------------------------------------------------------------------------
// IndexAttributeCollection::Count::get
//
// Gets the number of elements in the collection

int IndexAttributeCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// IndexAttributeCollection::Create (static, internal)
//
// Creates a new IndexAttributeCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	action			- IndexAction instance
//	attributes		- Pointer to the attribute set
//	numattributes	- Number of elements in the attribute set

IndexAttributeCollection^ IndexAttributeCollection::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxAttrInfo* const* attributes, unsigned int numattributes)
{
	return gcnew IndexAttributeCollection(gcnew IndexAttributeSetHandle(owner, action, attributes), numattributes);
}

//---------------------------------------------------------------------------
// IndexAttributeCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<IndexAttribute^>^ IndexAttributeCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<IndexAttribute^>(this);
}

//---------------------------------------------------------------------------
// IndexAttributeCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ IndexAttributeCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
