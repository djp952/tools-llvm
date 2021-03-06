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

#ifndef __PARSEDCOMMENT_H_
#define __PARSEDCOMMENT_H_
#pragma once

#include "Comment.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::Xml::Schema;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
enum class	CommentKind;
ref class	TranslationUnitHandle;

//---------------------------------------------------------------------------
// Class ParsedComment
//
// Specializes Command for CommentKind::Parsed (Full) comments
//---------------------------------------------------------------------------

public ref class ParsedComment sealed : public Comment
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ToHtmlFragment
	//
	// Converts the comment into an HTML fragment
	String^ ToHtmlFragment(void);

	// ToXmlElement
	//
	// Converts the comment into an XML fragment
	String^ ToXmlFragment(void);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a ParsedComment instance
	static ParsedComment^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment);

private:

	// Instance Constructors
	//
	ParsedComment(CommentHandle^ handle, CommentKind kind);
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __PARSEDCOMMENT_H_
