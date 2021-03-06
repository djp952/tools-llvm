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

#ifndef __LOADEDDIAGNOSTICCOLLECTION_H_
#define __LOADEDDIAGNOSTICCOLLECTION_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"
#include "DiagnosticCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class Diagnostic;
ref class DiagnosticSetHandle;

//---------------------------------------------------------------------------
// Class LoadedDiagnosticCollection
//
// Specialization of DiagnosticCollection for diagnostics loaded from a
// serialized diagnostics file.  This collection is disposable.
//---------------------------------------------------------------------------

public ref class LoadedDiagnosticCollection : public DiagnosticCollection
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
	// Creates a new LoadedDiagnosticCollection instance
	static LoadedDiagnosticCollection^ Create(CXDiagnosticSet&& diagnostics);

private:

	// DiagnosticSetHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXDiagnosticSet
	using DiagnosticSetHandle = UnmanagedTypeSafeHandle<CXDiagnosticSet, clang_disposeDiagnosticSet>;

	// Instance Constructor
	//
	LoadedDiagnosticCollection(DiagnosticSetHandle^ handle);

	// Destructor
	//
	~LoadedDiagnosticCollection();

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;		// Object disposal flag
	DiagnosticSetHandle^		m_handle;		// Diagnostic set handle
	array<Diagnostic^>^			m_cache;		// Collection element cache
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __LOADEDDIAGNOSTICCOLLECTION_H_
