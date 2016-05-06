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

#ifndef __CLANGERRORCODE_H_
#define __CLANGERRORCODE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class ClangErrorCode
//
// Error codes returned by libclang routines
//---------------------------------------------------------------------------

public value class ClangErrorCode
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(ClangErrorCode lhs, ClangErrorCode rhs);

	// operator!= (static)
	//
	static bool operator!=(ClangErrorCode lhs, ClangErrorCode rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this ClangErrorCode to another ClangErrorCode
	bool Equals(ClangErrorCode rhs);

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

	static initonly ClangErrorCode Success			= ClangErrorCode(CXErrorCode::CXError_Success);
	static initonly ClangErrorCode Failure			= ClangErrorCode(CXErrorCode::CXError_Failure);
	static initonly ClangErrorCode Crashed			= ClangErrorCode(CXErrorCode::CXError_Crashed);
	static initonly ClangErrorCode InvalidArguments	= ClangErrorCode(CXErrorCode::CXError_InvalidArguments);
	static initonly ClangErrorCode ASTReadError		= ClangErrorCode(CXErrorCode::CXError_ASTReadError);

internal:

	// Instance Constructor
	//
	ClangErrorCode(CXErrorCode code);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXErrorCode
	//
	// Exposes the value as a CXErrorCode enumeration value
	static operator CXErrorCode(ClangErrorCode rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXErrorCode					m_code;			// Underling error code
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __CLANGERRORCODE_H_
