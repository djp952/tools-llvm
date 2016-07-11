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

#ifndef __COMPLETIONRESULT_H_
#define __COMPLETIONRESULT_H_
#pragma once

#include "ReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class	CompletionString;
value class	CursorKind;

//---------------------------------------------------------------------------
// Class CompletionResult
//
// Represents a result from a code completion operation
//---------------------------------------------------------------------------

public ref class CompletionResult
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Kind
	//
	// Gets the kind of entity that this completion refers to
	property CursorKind Kind
	{
		CursorKind get(void);
	}

	// String
	//
	// Gets the completion string associated with the completion result
	property CompletionString^ String
	{
		CompletionString^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new CompletionResult instance
	static CompletionResult^ Create(SafeHandle^ owner, CXCompletionResult result);

private:

	// CompletionResultHandle
	//
	// ReferenceHandle specialization for CXCompletionResult
	using CompletionResultHandle = ReferenceHandle<CXCompletionResult>;

	// Instance Constructor
	//
	CompletionResult(CompletionResultHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	CompletionResultHandle^			m_handle;	// Safe handle instance
	CompletionString^				m_string;	// Cached completion string
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMPLETIONRESULT_H_
