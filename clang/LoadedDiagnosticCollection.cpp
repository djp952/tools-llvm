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
#include "LoadedDiagnosticCollection.h"

#include "Diagnostic.h"
#include "TranslationUnitHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// LoadedDiagnosticCollection Constructor (private)
//
// Arguments:
//
//	handle		- DiagnosticSetHandle instance for this collection

LoadedDiagnosticCollection::LoadedDiagnosticCollection(DiagnosticSetHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<Diagnostic^>(clang_getNumDiagnosticsInSet(DiagnosticSetHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// LoadedDiagnosticCollection Destructor

LoadedDiagnosticCollection::~LoadedDiagnosticCollection()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// LoadedDiagnosticCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Diagnostic^ LoadedDiagnosticCollection::default::get(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Check for a cached instance of the Diagnostic first, this will also
	// validate that the index is within the boundaries of the collection
	Diagnostic^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new diagnostic and cache it to prevent multiple creations, there
	// is no translation unit associated with the loaded diagnostics to use here
	m_cache[index] = Diagnostic::Create(m_handle, TranslationUnitHandle::Null, clang_getDiagnosticInSet(DiagnosticSetHandle::Reference(m_handle), static_cast<unsigned int>(index)));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// LoadedDiagnosticCollection::Count::get
//
// Gets the number of elements in the collection

int LoadedDiagnosticCollection::Count::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// LoadedDiagnosticCollection::Create (static, internal)
//
// Creates a new LoadedDiagnosticCollection instance
//
// Arguments:
//
//	diagnostics		- Unmanaged CXDiagnosticSet instance to take ownership of

LoadedDiagnosticCollection^ LoadedDiagnosticCollection::Create(CXDiagnosticSet&& diagnostics)
{
	// Loaded diagnostics are owned by the diagnostic set but reference the null translation unit
	// to provide context; use a self-disposing diagnostic set handle instance
	return gcnew LoadedDiagnosticCollection(gcnew DiagnosticSetHandle(std::move(diagnostics)));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
