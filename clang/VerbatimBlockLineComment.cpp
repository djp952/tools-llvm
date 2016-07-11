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

#include "stdafx.h"
#include "VerbatimBlockLineComment.h"

#include "CommentKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// VerbatimBlockLineComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

VerbatimBlockLineComment::VerbatimBlockLineComment(CommentHandle^ handle) : Comment(handle, CommentKind::VerbatimBlockLine)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// VerbatimBlockLineComment::Text::get
//
// Gets the text associated with a VerbatimBlockLineComment

String^ VerbatimBlockLineComment::Text::get(void)
{
	if(Object::ReferenceEquals(m_text, nullptr))
		m_text = StringUtil::ToString(clang_VerbatimBlockLineComment_getText(CommentHandle::Reference(m_handle)));

	return m_text;
}

//---------------------------------------------------------------------------
// VerbatimBlockLineComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ VerbatimBlockLineComment::ToString(void)
{
	return this->Text;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
