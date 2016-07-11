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

#ifndef __HTMLTAGCOMMENT_H_
#define __HTMLTAGCOMMENT_H_
#pragma once

#include "InlineContentComment.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
enum class CommentKind;

//---------------------------------------------------------------------------
// Class HtmlTagComment
//
// Specialization for html tag comments
//---------------------------------------------------------------------------

public ref class HtmlTagComment : public InlineContentComment
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

	// TagName
	//
	// Gets the HTML tag name
	property String^ TagName
	{
		String^ get(void);
	}

internal:

	// Instance Constructor
	//
	HtmlTagComment(CommentHandle^ handle, CommentKind kind);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^				m_tagname;			// Cached HTML tag name string
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __HTMLTAGCOMMENT_H_
