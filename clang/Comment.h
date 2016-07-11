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

#ifndef __COMMENT_H_
#define __COMMENT_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class	CommentCollection;
enum class	CommentKind;

//---------------------------------------------------------------------------
// Class Comment
//
// Represents a fully parsed cursor comment
//---------------------------------------------------------------------------

public ref class Comment
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Children
	//
	// Gets a collection of child comments
	property CommentCollection^ Children
	{
		CommentCollection^ get(void);
	}

	// Kind
	//
	// Gets the type of comment
	property CommentKind Kind
	{
		CommentKind get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a subclassed Comment instance
	static Comment^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment);

protected private:

	// CommentHandle
	//
	// TranslationUnitReferenceHandle specialization for CXComment
	using CommentHandle = TranslationUnitReferenceHandle<CXComment>;

	// Instance Constructor
	//
	Comment(CommentHandle^ handle, CommentKind kind);

	//-----------------------------------------------------------------------
	// Protected Member Variables

	CommentHandle^				m_handle;		// Underlying safe handle
	CommentKind					m_kind;			// Comment kind

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CommentCollection^			m_children;		// Child comment collection
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMMENT_H_
