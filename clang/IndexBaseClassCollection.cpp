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
#include "IndexBaseClassCollection.h"

#include "IndexBaseClass.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexBaseClassCollection Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance
//	numbases	- Number of elements pointed to by the handle

IndexBaseClassCollection::IndexBaseClassCollection(IdxBaseClassInfoSetHandle^ handle, unsigned int numbases) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<IndexBaseClass^>(numbases);
}

//---------------------------------------------------------------------------
// IndexBaseClassCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

IndexBaseClass^ IndexBaseClassCollection::default::get(int index)
{
	// Check for a cached instance of the IndexBaseClass first, this will also
	// validate that the index is within the boundaries of the collection
	IndexBaseClass^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new IndexBaseClass and cache it to prevent multiple creations
	m_cache[index] = IndexBaseClass::Create(m_handle->Owner, m_handle->IndexAction, IdxBaseClassInfoSetHandle::Reference(m_handle)[index]);
	return m_cache[index];
}

//---------------------------------------------------------------------------
// IndexBaseClassCollection::Count::get
//
// Gets the number of elements in the collection

int IndexBaseClassCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// IndexBaseClassCollection::Create (internal, static)
//
// Creates a new IndexBaseClassCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the declaration information

IndexBaseClassCollection^ IndexBaseClassCollection::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxCXXClassDeclInfo* info)
{
	if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");
	if(info == __nullptr) throw gcnew ArgumentNullException("info");

	// CXIdxCXXClassDeclInfo can't be used as a ReferenceHandle again without causing an LNK2022,
	// so use the embedded pointer and pass along the number of elements in the base class set
	return gcnew IndexBaseClassCollection(gcnew IdxBaseClassInfoSetHandle(owner, action, info->bases), info->numBases);
}

//---------------------------------------------------------------------------
// IndexBaseClassCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<IndexBaseClass^>^ IndexBaseClassCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<IndexBaseClass^>(this);
}

//---------------------------------------------------------------------------
// IndexBaseClassCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ IndexBaseClassCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
