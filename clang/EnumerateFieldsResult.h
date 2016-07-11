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

#ifndef __ENUMERATEFIELDRESULT_H_
#define __ENUMERATEFIELDSRESULT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class EnumerateFieldsResult
//
// Return value from a EnumerateFieldsFunc delegate
//---------------------------------------------------------------------------

public value class EnumerateFieldsResult
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(EnumerateFieldsResult lhs, EnumerateFieldsResult rhs);

	// operator!= (static)
	//
	static bool operator!=(EnumerateFieldsResult lhs, EnumerateFieldsResult rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this EnumerateFieldsResult to another EnumerateFieldsResult
	bool Equals(EnumerateFieldsResult rhs);

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
	// Terminate the current enumeration
	static initonly EnumerateFieldsResult Break	= EnumerateFieldsResult(CXVisitorResult::CXVisit_Break);

	// Continue
	//
	// Continue the current enumeration
	static initonly EnumerateFieldsResult Continue = EnumerateFieldsResult(CXVisitorResult::CXVisit_Continue);

internal:

	// Instance Constructor
	//
	EnumerateFieldsResult(CXVisitorResult result);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXVisitorResult
	//
	// Exposes the value as a CXVisitorResult enumeration value
	static operator CXVisitorResult(EnumerateFieldsResult rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXVisitorResult			m_result;		// Underlying CXVisitorResult
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __ENUMERATEFIELDSRESULT_H_
