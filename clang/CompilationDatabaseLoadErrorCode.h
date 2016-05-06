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

#ifndef __COMPILATIONDATABASELOADERRORCODE_H_
#define __COMPILATIONDATABASELOADERRORCODE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class CompilationDatabaseLoadErrorCode
//
// Describes the error that occurred loading a compilation database
//---------------------------------------------------------------------------

public value class CompilationDatabaseLoadErrorCode
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(CompilationDatabaseLoadErrorCode lhs, CompilationDatabaseLoadErrorCode rhs);

	// operator!= (static)
	//
	static bool operator!=(CompilationDatabaseLoadErrorCode lhs, CompilationDatabaseLoadErrorCode rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this CompilationDatabaseLoadErrorCode to another CompilationDatabaseLoadErrorCode
	bool Equals(CompilationDatabaseLoadErrorCode rhs);

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

	static initonly CompilationDatabaseLoadErrorCode None		= CompilationDatabaseLoadErrorCode(CXCompilationDatabase_Error::CXCompilationDatabase_NoError);
	static initonly CompilationDatabaseLoadErrorCode CannotLoad	= CompilationDatabaseLoadErrorCode(CXCompilationDatabase_Error::CXCompilationDatabase_CanNotLoadDatabase);

internal:

	// Instance Constructor
	//
	CompilationDatabaseLoadErrorCode(CXCompilationDatabase_Error code);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXCompilationDatabase_Error
	//
	// Exposes the value as a CXCompilationDatabase_Error enumeration value
	static operator CXCompilationDatabase_Error(CompilationDatabaseLoadErrorCode rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXCompilationDatabase_Error		m_code;			// Underling error code
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPILATIONDATABASELOADERRORCODE_H_
