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

#ifndef __COMMENTKIND_H_
#define __COMMENTKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum CommentKind
//
// Represents the type of a comment abstract syntax tree node
//---------------------------------------------------------------------------

public enum class CommentKind
{
	Null					= CXCommentKind::CXComment_Null,
	Text					= CXCommentKind::CXComment_Text,
	InlineCommand			= CXCommentKind::CXComment_InlineCommand,
	HtmlStartTag			= CXCommentKind::CXComment_HTMLStartTag,
	HtmlEndTag				= CXCommentKind::CXComment_HTMLEndTag,
	Paragraph				= CXCommentKind::CXComment_Paragraph,
	BlockCommand			= CXCommentKind::CXComment_BlockCommand,
	ParamCommand			= CXCommentKind::CXComment_ParamCommand,
	TParamCommand			= CXCommentKind::CXComment_TParamCommand,
	VerbatimBlockCommand	= CXCommentKind::CXComment_VerbatimBlockCommand,
	VerbatimBlockLine		= CXCommentKind::CXComment_VerbatimBlockLine,
	VerbatimLine			= CXCommentKind::CXComment_VerbatimLine,
	Parsed					= CXCommentKind::CXComment_FullComment,
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMMENTKIND_H_
