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

#ifndef __COMPLETIONSTRING_H_
#define __COMPLETIONSTRING_H_
#pragma once

#include "ReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local=zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
enum class	Availability;
ref class	CompletionChunkCollection;
ref class	StringCollection;

//---------------------------------------------------------------------------
// Class CompletionString
//
// Represents a code completion string instance
//---------------------------------------------------------------------------

public ref class CompletionString
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Annotations
	//
	// Gets a collection of annotations associated with the completion
	property StringCollection^ Annotations
	{
		StringCollection^ get(void);
	}

	// Availability
	//
	// Gets the availability of the entity that this completion refers to
	property local::Availability Availability
	{
		local::Availability get(void);
	}

	// BriefComment
	//
	// Gets the brief documentation comment attached to the completion declaration
	property String^ BriefComment
	{
		String^ get(void);
	}

	// Chunks
	//
	// Gets a collection of completion chunks
	property CompletionChunkCollection^ Chunks
	{
		CompletionChunkCollection^ get(void);
	}

	// ParentContext
	//
	// Gets the parent context of the completion
	property String^ ParentContext
	{
		String^ get(void);
	}

	// Priority
	//
	// Gets the priority of this code completion
	property unsigned int Priority
	{
		unsigned int get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new CompletionString instance
	static CompletionString^ Create(SafeHandle^ owner, CXCompletionString string);

private:

	// CompletionStringHandle
	//
	// ReferenceHandle specialization for a CXCompletionString
	using CompletionStringHandle = ReferenceHandle<CXCompletionString>;

	// Instance Constructor
	//
	CompletionString(CompletionStringHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	CompletionStringHandle^			m_handle;			// Safe handle instance
	StringCollection^				m_annotations;		// Cached annotations
	CompletionChunkCollection^		m_chunks;			// Cached chunk collection
	String^							m_briefcomment;		// Cached brief comment
	String^							m_parentcontext;	// Cached parent context
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPLETIONSTRING_H_
