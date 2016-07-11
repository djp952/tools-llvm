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
#include "IndexObjectiveCProtocolReferenceCollection.h"

#include "IndexObjectiveCProtocolReference.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexObjectiveCProtocolReferenceCollection::IndexObjectiveCProtocolReferenceCollection(IdxObjCProtocolRefListInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	IdxObjCProtocolRefListInfoHandle::Reference info(m_handle);
	m_cache = gcnew array<IndexObjectiveCProtocolReference^>(info->numProtocols);
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

IndexObjectiveCProtocolReference^ IndexObjectiveCProtocolReferenceCollection::default::get(int index)
{
	// Check for a cached instance of the IndexObjectiveCProtocolReference first, this will also
	// validate that the index is within the boundaries of the collection
	IndexObjectiveCProtocolReference^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new IndexObjectiveCProtocolReference and cache it to prevent multiple creations
	IdxObjCProtocolRefListInfoHandle::Reference info(m_handle);
	m_cache[index] = IndexObjectiveCProtocolReference::Create(m_handle->Owner, m_handle->IndexAction, info->protocols[index]);
	return m_cache[index];
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::Count::get
//
// Gets the number of elements in the collection

int IndexObjectiveCProtocolReferenceCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::Create (internal, static)
//
// Creates a new IndexObjectiveCProtocolReferenceCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the protocol list

IndexObjectiveCProtocolReferenceCollection^ IndexObjectiveCProtocolReferenceCollection::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCProtocolRefListInfo* info)
{
	if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");
	if(info == __nullptr) throw gcnew ArgumentNullException("info");

	return gcnew IndexObjectiveCProtocolReferenceCollection(gcnew IdxObjCProtocolRefListInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<IndexObjectiveCProtocolReference^>^ IndexObjectiveCProtocolReferenceCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<IndexObjectiveCProtocolReference^>(this);
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ IndexObjectiveCProtocolReferenceCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
