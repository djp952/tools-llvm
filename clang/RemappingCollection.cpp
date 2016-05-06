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
#include "RemappingCollection.h"

#include "ReadOnlyListEnumerator.h"
#include "Remapping.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// RemappingCollection Constructor
//
// Arguments:
//
//	handle		- Underlying RemappingHandle instance

RemappingCollection::RemappingCollection(RemappingHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// Create a cache for the remapping instances, forcing a zero-length array if handle is null
	// (clang_remap_getNumFiles will cause an access violation if a null pointer is passed in)
	RemappingHandle::Reference remapping(m_handle);
	m_cache = gcnew array<Remapping^>((remapping == __nullptr) ? 0 : clang_remap_getNumFiles(remapping));
}

//---------------------------------------------------------------------------
// RemappingCollection Destructor

RemappingCollection::~RemappingCollection()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// RemappingCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Remapping^ RemappingCollection::default::get(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Check for a cached instance of the Remapping first
	Remapping^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new remapping and cache it to prevent multiple creations
	CXRemapping remapping = RemappingHandle::Reference(m_handle);
	m_cache[index] = Remapping::Create(remapping, static_cast<unsigned int>(index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// RemappingCollection::Count::get
//
// Gets the number of elements in the collection

int RemappingCollection::Count::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// RemappingCollection::Create (static, internal)
//
// Creates a new CompileCommands instance
//
// Arguments:
//
//	remapping		- Unmanaged CXRemapping instance to be wrapped

RemappingCollection^ RemappingCollection::Create(CXRemapping&& remapping)
{
	return gcnew RemappingCollection(gcnew RemappingHandle(std::move(remapping)));
}

//---------------------------------------------------------------------------
// RemappingCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Remapping^>^ RemappingCollection::GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return gcnew ReadOnlyListEnumerator<Remapping^>(this);
}

//---------------------------------------------------------------------------
// RemappingCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ RemappingCollection::IEnumerable_GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
