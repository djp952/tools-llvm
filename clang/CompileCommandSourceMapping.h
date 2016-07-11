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

#ifndef __COMPILECOMMANDSOURCEMAPPING_H_
#define __COMPILECOMMANDSOURCEMAPPING_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class CompileCommandSourceMapping
//
// Represents a single instance of a compilation database mapped source
//---------------------------------------------------------------------------

public ref class CompileCommandSourceMapping
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Content
	//
	// Get the mapped source content for the compiler invocation
	property String^ Content
	{
		String^ get(void);
	}

	// Path
	//
	// Get the mapped source path for the compiler invocation
	property String^ Path
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new CompileCommandSourceMapping instance
	static CompileCommandSourceMapping^ Create(SafeHandle^ owner, CXCompileCommand command, unsigned int index);

private:

	// Instance Constructor
	//
	CompileCommandSourceMapping(String^ path, String^ content);

	//-----------------------------------------------------------------------
	// Member Variables
	
	String^						m_path;			// Mapped source path
	String^						m_content;		// Mapped source content
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMPILECOMMANDSOURCEMAPPING_H_
