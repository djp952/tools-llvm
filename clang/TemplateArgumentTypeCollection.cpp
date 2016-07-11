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
#include "TemplateArgumentTypeCollection.h"

#include "Type.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// TemplateArgumentTypeCollection Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TypeHandle instance

TemplateArgumentTypeCollection::TemplateArgumentTypeCollection(TypeHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// clang_Type_getNumTemplateArguments returns -1 if the type is not a class template, set the
	// collection to have zero elements in that case to avoid having to throw a special exception
	m_cache = gcnew array<Type^>(Math::Max(clang_Type_getNumTemplateArguments(TypeHandle::Reference(handle)), 0));
}

//---------------------------------------------------------------------------
// TemplateArgumentTypeCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Type^ TemplateArgumentTypeCollection::default::get(int index)
{
	// Check for a cached instance of the Type first
	Type^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new Type and cache it to prevent multiple creations
	m_cache[index] = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Type_getTemplateArgumentAsType(TypeHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// TemplateArgumentTypeCollection::Count::get
//
// Gets the number of elements in the collection

int TemplateArgumentTypeCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// TemplateArgumentTypeCollection::Create (internal, static)
//
// Creates a new TemplateArgumentTypeCollection
//
// Arguments:
//
//	owner		- Owning SafeHandle instance
//	transunit	- TranslationUnitHandle instance
//	type		- Unmanaged CXType

TypeCollection^ TemplateArgumentTypeCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type)
{
	return gcnew TemplateArgumentTypeCollection(gcnew TypeHandle(owner, transunit, type));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
