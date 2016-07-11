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
#include "CompletionChunkCollection.h"

#include "CompletionChunk.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompletionChunkCollection Constructor (private)
//
// Arguments:
//
//	handle		- Parent CompletionString safe handle instance

CompletionChunkCollection::CompletionChunkCollection(CompletionStringHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// Create a cache to store created collection elements, based on the number of chunks available
	m_cache = gcnew array<CompletionChunk^>(clang_getNumCompletionChunks(CompletionStringHandle::Reference(handle)));
}

//---------------------------------------------------------------------------
// CompletionChunkCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

CompletionChunk^ CompletionChunkCollection::default::get(int index)
{
	// Check for a cached instance of the CompletionChunk first, this will also
	// verify that index is within bounds and throw appropriately
	CompletionChunk^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create and cache a new CompletionChunk instance for this index and return it
	m_cache[index] = CompletionChunk::Create(m_handle->Owner, CompletionStringHandle::Reference(m_handle), static_cast<unsigned int>(index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CompletionChunkCollection::Count::get
//
// Gets the number of elements in the collection

int CompletionChunkCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CompletionChunkCollection::Create (internal, static)
//
// Creates a new CompletionChunkCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	string		- Unmanaged CXCompletionString instance

CompletionChunkCollection^ CompletionChunkCollection::Create(SafeHandle^ owner, CXCompletionString string)
{
	return gcnew CompletionChunkCollection(gcnew CompletionStringHandle(owner, string));
}

//---------------------------------------------------------------------------
// CompletionChunkCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<CompletionChunk^>^ CompletionChunkCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<CompletionChunk^>(this);
}

//---------------------------------------------------------------------------
// CompletionChunkCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CompletionChunkCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
