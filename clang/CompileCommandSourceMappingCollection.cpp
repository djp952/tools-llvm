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
#include "CompileCommandSourceMappingCollection.h"

#include "CompileCommandSourceMapping.h"
#include "ReadOnlyListEnumerator.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection Constructor (private)
//
// Arguments:
//
//	handle			- Underlying CompileCommandHandle instance

CompileCommandSourceMappingCollection::CompileCommandSourceMappingCollection(CompileCommandHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<CompileCommandSourceMapping^>(clang_CompileCommand_getNumMappedSources(CompileCommandHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

CompileCommandSourceMapping^ CompileCommandSourceMappingCollection::default::get(int index)
{
	// Check for a cached instance of the CompileCommandSourceMapping first, this will also
	// verify that index is within bounds and throw appropriately
	CompileCommandSourceMapping^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create and cache a new CompileCommandSourceMapping instance for this index and return it
	m_cache[index] = CompileCommandSourceMapping::Create(m_handle->Owner, CompileCommandHandle::Reference(m_handle), static_cast<unsigned int>(index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection::Count::get
//
// Gets the number of elements in the collection

int CompileCommandSourceMappingCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection::Create (static, internal)
//
// Creates a new CompileCommandSourceMappingCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	command			- Unmanaged CXCompileCommand instance

CompileCommandSourceMappingCollection^ CompileCommandSourceMappingCollection::Create(SafeHandle^ owner, CXCompileCommand command)
{
	return gcnew CompileCommandSourceMappingCollection(gcnew CompileCommandHandle(owner, command));
}

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<CompileCommandSourceMapping^>^ CompileCommandSourceMappingCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<CompileCommandSourceMapping^>(this);
}

//---------------------------------------------------------------------------
// CompileCommandSourceMappingCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CompileCommandSourceMappingCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
