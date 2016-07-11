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

#ifndef __ENUMERATEREFERENCESRESULT_H_
#define __ENUMERATEREFERENCESRESULT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class EnumerateReferencesResult
//
// Return value from a EnumerateReferencesFunc delegate
//---------------------------------------------------------------------------

public value class EnumerateReferencesResult
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(EnumerateReferencesResult lhs, EnumerateReferencesResult rhs);

	// operator!= (static)
	//
	static bool operator!=(EnumerateReferencesResult lhs, EnumerateReferencesResult rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this EnumerateReferencesResult to another EnumerateReferencesResult
	bool Equals(EnumerateReferencesResult rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Fields

	// Break
	//
	// Terminates the enumeration operation
	static initonly EnumerateReferencesResult Break	= EnumerateReferencesResult(CXVisitorResult::CXVisit_Break);

	// Continue
	//
	// Continues the enumeration operation
	static initonly EnumerateReferencesResult Continue = EnumerateReferencesResult(CXVisitorResult::CXVisit_Continue);

internal:

	// Instance Constructor
	//
	EnumerateReferencesResult(CXVisitorResult result);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXVisitorResult
	//
	// Exposes the value as a CXVisitorResult enumeration value
	static operator CXVisitorResult(EnumerateReferencesResult rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXVisitorResult				m_result;		// Underlying CXVisitorResult
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __ENUMERATEREFERENCESRESULT_H_
