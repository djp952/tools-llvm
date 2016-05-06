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

#ifndef __DIAGNOSTICCATEGORY_H_
#define __DIAGNOSTICCATEGORY_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class DiagnosticCategory
//
// Defines the category of a diagnostic entity
//---------------------------------------------------------------------------

public value class DiagnosticCategory
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(DiagnosticCategory lhs, DiagnosticCategory rhs);

	// operator!= (static)
	//
	static bool operator!=(DiagnosticCategory lhs, DiagnosticCategory rhs);

	// String^ conversion operator (implicit)
	//
	static operator String^(DiagnosticCategory^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this DiagnosticCategory to another DiagnosticCategory
	bool Equals(DiagnosticCategory rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Number
	//
	// Gets the category number
	property int Number
	{
		int get(void);
	}

	// Text
	//
	// Gets the category name/text
	property String^ Text
	{
		String^ get(void);
	}

internal:

	// Instance Constructor
	//
	DiagnosticCategory(const CXDiagnostic& diagnostic);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	int						m_number;		// Category number
	String^					m_text;			// Category name/text string
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __DIAGNOSTICCATEGORY_H_
