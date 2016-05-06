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
#include "CompilationDatabaseLoadErrorCode.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode Constructor (internal)
//
// Arguments:
//
//	code		- CXCompilationDatabase_Error value to be represented by this class

CompilationDatabaseLoadErrorCode::CompilationDatabaseLoadErrorCode(CXCompilationDatabase_Error code) : m_code(code)
{
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::operator == (static)

bool CompilationDatabaseLoadErrorCode::operator==(CompilationDatabaseLoadErrorCode lhs, CompilationDatabaseLoadErrorCode rhs)
{
	return lhs.m_code == rhs.m_code;
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::operator != (static)

bool CompilationDatabaseLoadErrorCode::operator!=(CompilationDatabaseLoadErrorCode lhs, CompilationDatabaseLoadErrorCode rhs)
{
	return lhs.m_code != rhs.m_code;
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::operator CXCompilationDatabase_Error (static)

CompilationDatabaseLoadErrorCode::operator CXCompilationDatabase_Error(CompilationDatabaseLoadErrorCode rhs)
{
	return rhs.m_code;
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::Equals
//
// Compares this CompilationDatabaseLoadErrorCode to another CompilationDatabaseLoadErrorCode
//
// Arguments:
//
//	rhs		- Right-hand CompilationDatabaseLoadErrorCode to compare against

bool CompilationDatabaseLoadErrorCode::Equals(CompilationDatabaseLoadErrorCode rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool CompilationDatabaseLoadErrorCode::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a CompilationDatabaseLoadErrorCode instance
	CompilationDatabaseLoadErrorCode^ rhsref = dynamic_cast<CompilationDatabaseLoadErrorCode^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int CompilationDatabaseLoadErrorCode::GetHashCode(void)
{
	return static_cast<int>(m_code).GetHashCode();
}

//---------------------------------------------------------------------------
// CompilationDatabaseLoadErrorCode::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ CompilationDatabaseLoadErrorCode::ToString(void)
{
	// There are only a couple generic error codes to deal with, use static text based
	// on the documentation in the libclang header file, can always be adjusted later
	switch(m_code) {

		case CXCompilationDatabase_Error::CXCompilationDatabase_NoError:
			return gcnew String("No error");

		case CXCompilationDatabase_Error::CXCompilationDatabase_CanNotLoadDatabase:
			return gcnew String("Compilation database cannot be loaded");
	}

	return gcnew String("Unknown compilation database error");
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
