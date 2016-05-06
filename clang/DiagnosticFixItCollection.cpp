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
#include "DiagnosticFixItCollection.h"

#include "DiagnosticFixIt.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// DiagnosticFixItCollection Constructor
//
// Arguments:
//
//	handle		- Underlying DiagnosticHandle instance

DiagnosticFixItCollection::DiagnosticFixItCollection(DiagnosticHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<DiagnosticFixIt^>(clang_getDiagnosticNumFixIts(DiagnosticHandle::Reference(handle)));
}

//---------------------------------------------------------------------------
// DiagnosticFixItCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

DiagnosticFixIt^ DiagnosticFixItCollection::default::get(int index)
{
	// Check for a cached instance of the DiagnosticFixIt first, this will throw if the
	// index is out of range, the cache array is the same size as the number of FixIts
	DiagnosticFixIt^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new DiagnosticFixIt and cache it to prevent multiple creations
	m_cache[index] = DiagnosticFixIt::Create(m_handle->Owner, m_handle->TranslationUnit, DiagnosticHandle::Reference(m_handle), static_cast<unsigned int>(index));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// DiagnosticFixItCollection::Count::get
//
// Gets the number of elements in the collection

int DiagnosticFixItCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// DiagnosticFixItCollection::Create (static, internal)
//
// Creates a new DiagnosticFixItCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	diagnostic	- Unmanaged CXDiagnostic instance

DiagnosticFixItCollection^ DiagnosticFixItCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic)
{
	return gcnew DiagnosticFixItCollection(gcnew DiagnosticHandle(owner, transunit, diagnostic));
}

//---------------------------------------------------------------------------
// DiagnosticFixItCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<DiagnosticFixIt^>^ DiagnosticFixItCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<DiagnosticFixIt^>(this);
}

//---------------------------------------------------------------------------
// DiagnosticFixItCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ DiagnosticFixItCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
