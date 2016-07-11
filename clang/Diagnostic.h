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

#ifndef __DIAGNOSTIC_H_
#define __DIAGNOSTIC_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
value class	DiagnosticCategory;
ref class	DiagnosticCollection;
enum class	DiagnosticDisplayOptions;
ref class	DiagnosticEmphasisCollection;
ref class	DiagnosticFixItCollection;
enum class	DiagnosticSeverity;
ref class	Location;

//---------------------------------------------------------------------------
// Class Diagnostic
//
// Represents a single instance of a clang diagnostic
//---------------------------------------------------------------------------

public ref class Diagnostic
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Format
	//
	// Format the given diagnostic in a manner that is suitable for display
	String^ Format(void);
	String^ Format(DiagnosticDisplayOptions options);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Category
	//
	// Gets the category of the diagnostic
	property DiagnosticCategory Category
	{
		DiagnosticCategory get(void);
	}

	// Children
	//
	// Gets a collection of child diagnostics
	property DiagnosticCollection^ Children
	{
		DiagnosticCollection^ get(void);
	}

	// DisableOption
	//
	// Gets the command-line option that disables this diagnostic
	property String^ DisableOption
	{
		String^ get(void);
	}

	// Emphases
	//
	// Gets a collection of emphases extents associated with the diagnostic
	property DiagnosticEmphasisCollection^ Emphases
	{
		DiagnosticEmphasisCollection^ get(void);
	}

	// EnableOption
	//
	// Gets the command-line option that enables this diagnostic
	property String^ EnableOption
	{
		String^ get(void);
	}

	// FixIts
	//
	// Gets a collection of fix-its (replacements) associated with the diagnostic
	property DiagnosticFixItCollection^ FixIts
	{
		DiagnosticFixItCollection^ get(void);
	}

	// Location
	//
	// Gets the source location for the diagnostic
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// Severity
	//
	// Gets the severity level of the diagnostic
	property DiagnosticSeverity Severity
	{
		DiagnosticSeverity get(void);
	}

	// Spelling
	//
	// Gets the text associated with this diagnostic
	property String^ Spelling
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new Diagnostic instance
	static Diagnostic^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic);

private:

	// DiagnosticHandle
	//
	// TranslationUnitReferenceHandle specialization for CXDiagnostic
	using DiagnosticHandle = TranslationUnitReferenceHandle<CXDiagnostic>;

	// Instance Constructor
	//
	Diagnostic(DiagnosticHandle^ handle);

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetOptions
	//
	// Loads the enable/disable command line option strings
	void GetOptions(void);

	//-----------------------------------------------------------------------
	// Member Variables

	DiagnosticHandle^				m_handle;		// Underlying safe handle
	String^							m_enableopt;	// Cached enable option
	String^							m_disableopt;	// Cached disable option
	DiagnosticCollection^			m_children;		// Cached child diagnostics
	local::Location^				m_location;		// Cached location instance
	String^							m_spelling;		// Cached spelling string
	DiagnosticEmphasisCollection^	m_emphases;		// Cached emphasis collection
	DiagnosticFixItCollection^		m_fixits;		// Cached fixit collection
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __DIAGNOSTIC_H_
