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
#include "ClangErrorCode.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ClangErrorCode Constructor (internal)
//
// Arguments:
//
//	code		- CXErrorCode value to be represented by this class

ClangErrorCode::ClangErrorCode(CXErrorCode code) : m_code(code)
{
}

//---------------------------------------------------------------------------
// ClangErrorCode::operator == (static)

bool ClangErrorCode::operator==(ClangErrorCode lhs, ClangErrorCode rhs)
{
	return lhs.m_code == rhs.m_code;
}

//---------------------------------------------------------------------------
// ClangErrorCode::operator != (static)

bool ClangErrorCode::operator!=(ClangErrorCode lhs, ClangErrorCode rhs)
{
	return lhs.m_code != rhs.m_code;
}

//---------------------------------------------------------------------------
// ClangErrorCode::operator CXErrorCode (static)

ClangErrorCode::operator CXErrorCode(ClangErrorCode rhs)
{
	return rhs.m_code;
}

//---------------------------------------------------------------------------
// ClangErrorCode::Equals
//
// Compares this ClangErrorCode to another ClangErrorCode
//
// Arguments:
//
//	rhs		- Right-hand ClangErrorCode to compare against

bool ClangErrorCode::Equals(ClangErrorCode rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// ClangErrorCode::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool ClangErrorCode::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a ClangErrorCode instance
	ClangErrorCode^ rhsref = dynamic_cast<ClangErrorCode^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// ClangErrorCode::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int ClangErrorCode::GetHashCode(void)
{
	return static_cast<int>(m_code).GetHashCode();
}

//---------------------------------------------------------------------------
// ClangErrorCode::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ ClangErrorCode::ToString(void)
{
	// There are only a few generic error codes to deal with, use static text based
	// on the documentation in the libclang CXErrorCode header file
	switch(m_code) {

		case CXErrorCode::CXError_Success:
			return gcnew String("No error");

		case CXErrorCode::CXError_Failure:
			return gcnew String("General failure. No further details are available");

		case CXErrorCode::CXError_Crashed:
			return gcnew String("libclang crashed while performing the requested operation");

		case CXErrorCode::CXError_InvalidArguments:
			return gcnew String("The function detected that the arguments violate the function");

		case CXErrorCode::CXError_ASTReadError:
			return gcnew String("An AST deserialization error has occurred");
	}

	return gcnew String("Unknown libclang error");
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
