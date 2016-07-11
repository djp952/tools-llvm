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
#include "DiagnosticEmphasisCollection.h"

#include "Extent.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection Constructor
//
// Arguments:
//
//	handle		- Underlying DiagnosticHandle instance

DiagnosticEmphasisCollection::DiagnosticEmphasisCollection(DiagnosticHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<Extent^>(clang_getDiagnosticNumRanges(DiagnosticHandle::Reference(handle)));
}

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Extent^ DiagnosticEmphasisCollection::default::get(int index)
{
	// Check for a cached instance of the Extent first, this will throw if the
	// index is out of range, the cache array is the same size as the number of extents
	Extent^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new extent and cache it to prevent multiple creations
	m_cache[index] = Extent::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getDiagnosticRange(DiagnosticHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection::Count::get
//
// Gets the number of elements in the collection

int DiagnosticEmphasisCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection::Create (static, internal)
//
// Creates a new DiagnosticEmphasisCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	diagnostic	- Unmanaged CXDiagnostic instance

DiagnosticEmphasisCollection^ DiagnosticEmphasisCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic)
{
	return gcnew DiagnosticEmphasisCollection(gcnew DiagnosticHandle(owner, transunit, diagnostic));
}

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Extent^>^ DiagnosticEmphasisCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<Extent^>(this);
}

//---------------------------------------------------------------------------
// DiagnosticEmphasisCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ DiagnosticEmphasisCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
