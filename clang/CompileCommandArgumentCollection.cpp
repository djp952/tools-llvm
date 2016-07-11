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
#include "CompileCommandArgumentCollection.h"

#include "ReadOnlyListEnumerator.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection Constructor
//
// Arguments:
//
//	handle			- Compile command handle instance

CompileCommandArgumentCollection::CompileCommandArgumentCollection(CompileCommandHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<String^>(clang_CompileCommand_getNumArgs(CompileCommandHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

String^ CompileCommandArgumentCollection::default::get(int index)
{
	// Check for a cached instance of the argument string first, this will also
	// verify that index is within bounds and throw appropriately
	String^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create and cache a new argument string instance for this index and return it
	m_cache[index] = StringUtil::ToString(clang_CompileCommand_getArg(CompileCommandHandle::Reference(m_handle), index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection::Count::get
//
// Gets the number of elements in the collection

int CompileCommandArgumentCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection::Create (static, internal)
//
// Creates a new CompileCommandArgumentCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	command			- Unmanaged CXCompileCommand instance

CompileCommandArgumentCollection^ CompileCommandArgumentCollection::Create(SafeHandle^ owner, CXCompileCommand command)
{
	return gcnew CompileCommandArgumentCollection(gcnew CompileCommandHandle(owner, command));
}

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<String^>^ CompileCommandArgumentCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<String^>(this);
}

//---------------------------------------------------------------------------
// CompileCommandArgumentCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CompileCommandArgumentCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
