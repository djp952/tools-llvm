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

#ifndef __COMPILECOMMAND_H_
#define __COMPILECOMMAND_H_
#pragma once

#include "ReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class CompileCommandArgumentCollection;
ref class CompileCommandSourceMappingCollection;

//---------------------------------------------------------------------------
// Class CompileCommand
//
// Represents a single instance of a clang compile database command
//---------------------------------------------------------------------------

public ref class CompileCommand
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Arguments
	//
	// Gets a collection of compiler invocation arguments
	property CompileCommandArgumentCollection^ Arguments
	{
		CompileCommandArgumentCollection^ get(void);
	}

	// Filename
	//
	// Gets the filename associated with the CompileCommand
	property String^ Filename
	{
		String^ get(void);
	}

	// SourceMappings
	//
	// Gets a collection of compiler invocation source mappings
	property CompileCommandSourceMappingCollection^ SourceMappings
	{
		CompileCommandSourceMappingCollection^ get(void);
	}

	// WorkingDirectory
	//
	// Gets the working directory where the CompileCommand was executed from
	property String^ WorkingDirectory
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new CompileCommand instance
	static CompileCommand^ Create(SafeHandle^ owner, CXCompileCommand&& command);

private:

	// CompileCommandHandle
	//
	// ReferenceHandle specialization for CXCompileCommand
	using CompileCommandHandle = ReferenceHandle<CXCompileCommand>;

	// Instance Constructor
	//
	CompileCommand(CompileCommandHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	CompileCommandHandle^					m_handle;		// Underlying safe handle
	CompileCommandArgumentCollection^		m_arguments;	// Argument collection
	CompileCommandSourceMappingCollection^	m_mappings;		// Source mappings
	String^									m_filename;		// Cached file name
	String^									m_workdir;		// Cached working dir
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPILECOMMAND_H_
