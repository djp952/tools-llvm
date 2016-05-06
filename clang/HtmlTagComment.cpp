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
#include "HtmlTagComment.h"

#include "CommentKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// HtmlTagComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance
//	kind		- Derived kind of comment being constructed

HtmlTagComment::HtmlTagComment(CommentHandle^ handle, CommentKind kind) : InlineContentComment(handle, kind)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// HtmlTagComment::TagName::get
//
// Gets the HTML tag name

String^ HtmlTagComment::TagName::get(void)
{
	if(Object::ReferenceEquals(m_tagname, nullptr))
		m_tagname = StringUtil::ToString(clang_HTMLTagComment_getTagName(CommentHandle::Reference(m_handle)));

	return m_tagname;
}

//---------------------------------------------------------------------------
// HtmlTagComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ HtmlTagComment::ToString(void)
{
	return StringUtil::ToString(clang_HTMLTagComment_getAsString(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
