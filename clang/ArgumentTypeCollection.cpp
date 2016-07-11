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
#include "ArgumentTypeCollection.h"

#include "Type.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ArgumentTypeCollection Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TypeHandle instance

ArgumentTypeCollection::ArgumentTypeCollection(TypeHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// clang_getNumArgTypes will return -1 if this isn't something that would have argument
	// types, convert that to a zero element collection and don't throw an exception
	m_cache = gcnew array<Type^>(Math::Max(clang_getNumArgTypes(TypeHandle::Reference(m_handle)), 0));
}

//---------------------------------------------------------------------------
// ArgumentTypeCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Type^ ArgumentTypeCollection::default::get(int index)
{
	// Check for a cached instance of the Type first
	Type^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new Type and cache it to prevent multiple creations
	m_cache[index] = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getArgType(TypeHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// ArgumentTypeCollection::Count::get
//
// Gets the number of elements in the collection

int ArgumentTypeCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// ArgumentTypeCollection::Create (internal, static)
//
// Creates a new ArgumentTypeCollection
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	type		- Unmanaged CXType instance

TypeCollection^ ArgumentTypeCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type)
{
	return gcnew ArgumentTypeCollection(gcnew TypeHandle(owner, transunit, type));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
