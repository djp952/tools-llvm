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

#ifndef __DIAGNOSTICFIXIT_H_
#define __DIAGNOSTICFIXIT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class DiagnosticHandle;
ref class Extent;
ref class TranslationUnitHandle;

//---------------------------------------------------------------------------
// Class DiagnosticFixIt
//
// Represents a single instance of a clang diagnostic fix-it instance
//---------------------------------------------------------------------------

public ref class DiagnosticFixIt
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Extent
	//
	// Gets the source range to be replaced with the fix-it text
	property local::Extent^ Extent
	{
		local::Extent^ get(void);
	}

	// ReplacementText
	//
	// Gets the replacement text associated with this fix-it
	property String^ ReplacementText
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions
	
	// Create (static)
	//
	// Creates a new DiagnosticFixIt instance
	static DiagnosticFixIt^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic, unsigned int index);

private:

	// Instance Constructor
	//
	DiagnosticFixIt(String^ replacement, local::Extent^ extent);

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_replacement;		// Text representing the fix-it
	local::Extent^			m_extent;			// Source range for the fix-it
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __DIAGNOSTICFIXIT_H_
