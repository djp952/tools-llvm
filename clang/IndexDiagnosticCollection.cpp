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
#include "IndexDiagnosticCollection.h"

#include "Diagnostic.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexDiagnosticCollection Constructor (private)
//
// Arguments:
//
//	handle		- DiagnosticSetReferenceHandle instance for this collection

IndexDiagnosticCollection::IndexDiagnosticCollection(DiagnosticSetReferenceHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<Diagnostic^>(clang_getNumDiagnosticsInSet(DiagnosticSetReferenceHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// IndexDiagnosticCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Diagnostic^ IndexDiagnosticCollection::default::get(int index)
{
	// Check for a cached instance of the Diagnostic first, this will also
	// validate that the index is within the boundaries of the collection
	Diagnostic^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new diagnostic and cache it to prevent multiple creations
	m_cache[index] = Diagnostic::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getDiagnosticInSet(DiagnosticSetReferenceHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// IndexDiagnosticCollection::Count::get
//
// Gets the number of elements in the collection

int IndexDiagnosticCollection::Count::get(void)
{
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// IndexDiagnosticCollection::Create (static, internal)
//
// Creates a new IndexDiagnosticCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	diagnostics		- Unmanaged CXDiagnosticSet instance to take ownership of

IndexDiagnosticCollection^ IndexDiagnosticCollection::Create(SafeHandle^ owner, CXDiagnosticSet diagnostics)
{
	return gcnew IndexDiagnosticCollection(gcnew DiagnosticSetReferenceHandle(owner, TranslationUnitHandle::Null, diagnostics));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
