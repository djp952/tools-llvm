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

#ifndef __COMPILATIONDATABASE_H_
#define __COMPILATIONDATABASE_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class CompilationDatabaseHandle;
ref class CompileCommandCollection;

//---------------------------------------------------------------------------
// Class CompilationDatabase
//
// Holds all information used to compile files in a project
//---------------------------------------------------------------------------

public ref class CompilationDatabase
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetCompileCommands
	//
	// Gets the collection of compile commands from the database
	CompileCommandCollection^ GetCompileCommands(void);
	CompileCommandCollection^ GetCompileCommands(String^ filename);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new CompilationDatabase instance
	static CompilationDatabase^ Create(CXCompilationDatabase&& database);

private:

	// CompilationDatabaseHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXCompilationDatabase
	using CompilationDatabaseHandle = UnmanagedTypeSafeHandle<CXCompilationDatabase, clang_CompilationDatabase_dispose>;

	// Instance Constructor
	//
	CompilationDatabase(CompilationDatabaseHandle^ handle);

	// Destructor
	//
	~CompilationDatabase();

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;		// Object disposal flag
	CompilationDatabaseHandle^		m_handle;		// Underlying safe handle
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPILATIONDATABASE_H_
