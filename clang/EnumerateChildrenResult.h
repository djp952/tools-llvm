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

#ifndef __ENUMERATECHILDRENRESULT_H_
#define __ENUMERATECHILDRENRESULT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class EnumerateChildrenResult
//
// Return value from a EnumerateChildrenFunc delegate
//---------------------------------------------------------------------------

public value class EnumerateChildrenResult
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(EnumerateChildrenResult lhs, EnumerateChildrenResult rhs);

	// operator!= (static)
	//
	static bool operator!=(EnumerateChildrenResult lhs, EnumerateChildrenResult rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this EnumerateChildrenResult to another EnumerateChildrenResult
	bool Equals(EnumerateChildrenResult rhs);

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
	// Terminates the cursor traversal
	static initonly EnumerateChildrenResult Break	= EnumerateChildrenResult(CXChildVisitResult::CXChildVisit_Break);

	// Continue
	//
	// Continues the cursor traversal with the next sibling
	static initonly EnumerateChildrenResult Continue = EnumerateChildrenResult(CXChildVisitResult::CXChildVisit_Continue);

	// Recurse
	//
	// Recursively traverse the children of this cursor
	static initonly EnumerateChildrenResult Recurse = EnumerateChildrenResult(CXChildVisitResult::CXChildVisit_Recurse);

internal:

	// Instance Constructor
	//
	EnumerateChildrenResult(CXChildVisitResult result);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXChildVisitResult
	//
	// Exposes the value as a CXChildVisitResult enumeration value
	static operator CXChildVisitResult(EnumerateChildrenResult rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXChildVisitResult			m_result;		// Underlying CXChildVisitResult
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __ENUMERATECHILDRENRESULT_H_
