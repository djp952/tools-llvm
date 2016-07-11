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
#include "TopLevelHeaderCollection.h"

#include "File.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// TopLevelHeaderCollection Constructor (private)
//
// Arguments:
//
//	handle			- Reference handle instance

TopLevelHeaderCollection::TopLevelHeaderCollection(ModuleHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	
	ModuleHandle::Reference module(handle);
	m_cache = gcnew array<File^>(clang_Module_getNumTopLevelHeaders(module.TranslationUnit, module));
}

//---------------------------------------------------------------------------
// TopLevelHeaderCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

File^ TopLevelHeaderCollection::default::get(int index)
{
	// Check for a cached instance of the File first (will throw if index is out of range)
	File^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new file instance and cache it to prevent multiple creations
	ModuleHandle::Reference module(m_handle);
	m_cache[index] = File::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Module_getTopLevelHeader(module.TranslationUnit, module, index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// TopLevelHeaderCollection::Count::get
//
// Gets the number of elements in the collection

int TopLevelHeaderCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// TopLevelHeaderCollection::Create (static, internal)
//
// Creates a new TopLevelHeaderCollection instance
//
// Arguments:
//
//	handle			- Owning safe handle instance
//	transunit		- TranslationUnitHandle instance
//	module			- Unmanaged CXModule instance

TopLevelHeaderCollection^ TopLevelHeaderCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXModule module)
{
	return gcnew TopLevelHeaderCollection(gcnew ModuleHandle(owner, transunit, module));
}

//---------------------------------------------------------------------------
// TopLevelHeaderCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<File^>^ TopLevelHeaderCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<File^>(this);
}

//---------------------------------------------------------------------------
// TopLevelHeaderCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ TopLevelHeaderCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
