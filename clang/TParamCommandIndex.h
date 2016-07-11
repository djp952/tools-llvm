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

#ifndef __TPARAMCOMMANDINDEX_H_
#define __TPARAMCOMMANDINDEX_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class TParamCommandIndex
//
// Implements an indexer-only object used to wrap the index property of
// a TParamCommandComment instance
//---------------------------------------------------------------------------

public ref class TParamCommandIndex
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// default[unsigned int]
	//
	// Gets the template parameter index at the specified depth
	property Nullable<unsigned int> default[String^] 
	{
		Nullable<unsigned int> get(unsigned int nestingdepth);
	}

	// NestingDepth
	//
	// Gets the nesting depth of this parameter in the template parameter list
	property Nullable<unsigned int> NestingDepth
	{
		Nullable<unsigned int> get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new TParamCommandIndex instance
	static TParamCommandIndex^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment);

private:

	// CommentHandle
	//
	// TranslationUnitReferenceHandle specialization for CXComment
	using CommentHandle = TranslationUnitReferenceHandle<CXComment>;

	// Instance Constructor
	//
	TParamCommandIndex(CommentHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	CommentHandle^				m_handle;		// Underlying safe handle
	Nullable<unsigned int>		m_depth;		// Maximum nesting depth
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TPARAMCOMMANDINDEX_H_
