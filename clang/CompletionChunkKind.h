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

#ifndef __COMPLETIONCHUNKKIND_H_
#define __COMPLETIONCHUNKKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum CompletionChunkKind
//
// Describes a single piece of text within a code-completion string
//---------------------------------------------------------------------------

public enum class CompletionChunkKind
{
	Optional			= CXCompletionChunkKind::CXCompletionChunk_Optional,
	TypedText			= CXCompletionChunkKind::CXCompletionChunk_TypedText,
	Text				= CXCompletionChunkKind::CXCompletionChunk_Text,
	Placeholder			= CXCompletionChunkKind::CXCompletionChunk_Placeholder,
	Informative			= CXCompletionChunkKind::CXCompletionChunk_Informative,
	CurrentParameter	= CXCompletionChunkKind::CXCompletionChunk_CurrentParameter,
	LeftParen			= CXCompletionChunkKind::CXCompletionChunk_LeftParen,
	RightParen			= CXCompletionChunkKind::CXCompletionChunk_RightParen,
	LeftBracket			= CXCompletionChunkKind::CXCompletionChunk_LeftBracket,
	RightBracket		= CXCompletionChunkKind::CXCompletionChunk_RightBracket,
	LeftBrace			= CXCompletionChunkKind::CXCompletionChunk_LeftBrace,
	RightBrace			= CXCompletionChunkKind::CXCompletionChunk_RightBrace,
	LeftAngle			= CXCompletionChunkKind::CXCompletionChunk_LeftAngle,
	RightAngle			= CXCompletionChunkKind::CXCompletionChunk_RightAngle,
	Comma				= CXCompletionChunkKind::CXCompletionChunk_Comma,
	ResultType			= CXCompletionChunkKind::CXCompletionChunk_ResultType,
	Colon				= CXCompletionChunkKind::CXCompletionChunk_Colon,
	SemiColon			= CXCompletionChunkKind::CXCompletionChunk_SemiColon,
	Equal				= CXCompletionChunkKind::CXCompletionChunk_Equal,
	HorizontalSpace		= CXCompletionChunkKind::CXCompletionChunk_HorizontalSpace,
	VerticalSpace		= CXCompletionChunkKind::CXCompletionChunk_VerticalSpace,
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPLETIONCHUNKKIND_H_
