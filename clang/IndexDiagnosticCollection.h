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

#ifndef __INDEXDIAGNOSTICCOLLECTION_H_
#define __INDEXDIAGNOSTICCOLLECTION_H_
#pragma once

#include "DiagnosticCollection.h"
#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class Diagnostic;

//---------------------------------------------------------------------------
// Class IndexDiagnosticCollection
//
// Specialization of DiagnosticCollection for code completions
//---------------------------------------------------------------------------

public ref class IndexDiagnosticCollection : public DiagnosticCollection
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property Diagnostic^ default[int]
	{
		virtual Diagnostic^ get(int index) override;
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get() override;
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexDiagnosticCollection instance
	static IndexDiagnosticCollection^ Create(SafeHandle^ owner, CXDiagnosticSet diagnostics);

private:

	// DiagnosticSetReferenceHandle
	//
	// TranslationUnitReferenceHandle specialization for CXDiagnosticSet
	using DiagnosticSetReferenceHandle = TranslationUnitReferenceHandle<CXDiagnosticSet>;

	// Instance Constructor
	//
	IndexDiagnosticCollection(DiagnosticSetReferenceHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	DiagnosticSetReferenceHandle^	m_handle;		// Diagnostic set handle
	array<Diagnostic^>^				m_cache;		// Collection element cache
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXDIAGNOSTICCOLLECTION_H_
