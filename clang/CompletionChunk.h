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

#ifndef __COMPLETIONCHUNK_H_
#define __COMPLETIONCHUNK_H_
#pragma once

#include "ReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local=zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
enum class  CompletionChunkKind;
ref class	CompletionString;
ref class	CompletionStringHandle;

//---------------------------------------------------------------------------
// Class CompletionChunk
//
// Represents a single chunk of a code completion string
//---------------------------------------------------------------------------

public ref class CompletionChunk
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// CompletionString
	//
	// Gets the completion string associated with a particular chunk within a completion string
	property local::CompletionString^ CompletionString
	{
		local::CompletionString^ get(void);
	}

	// Kind
	//
	// Gets the kind of a particular chunk within a completion string
	property CompletionChunkKind Kind
	{
		CompletionChunkKind get(void);
	}

	// Text
	//
	// Gets the text associated with a particular chunk within a completion string
	property String^ Text
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new CompletionChunk instance
	static CompletionChunk^ Create(SafeHandle^ owner, CXCompletionString string, unsigned int index);

private:

	// CompletionStringHandle
	//
	// ReferenceHandle specialization for a CXCompletionString
	using CompletionStringHandle = ReferenceHandle<CXCompletionString>;

	// Instance Constructor
	//
	CompletionChunk(CompletionStringHandle^ handle, unsigned int index);

	//-----------------------------------------------------------------------
	// Member Variables

	CompletionStringHandle^			m_handle;		// Parent safe handle
	const unsigned int				m_index;		// Index into the collection
	String^							m_text;			// Cached text property
	local::CompletionString^		m_string;		// Cached completion string
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPLETIONCHUNK_H_
