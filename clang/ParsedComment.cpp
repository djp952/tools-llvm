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
#include "ParsedComment.h"

#include "CommentKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// ParsedComment Constructor (private)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance
//	kind		- Comment kind (can be null or full here)

ParsedComment::ParsedComment(CommentHandle^ handle, CommentKind kind) : Comment(handle, kind)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// ParsedComment::Create (internal, static)
//
// Creates a ParsedComment instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	comment		- Unmanaged CXComment instance

ParsedComment^ ParsedComment::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment)
{
	// Determine what kind of comment this is, this can be a full or null comment only
	CXCommentKind kind = clang_Comment_getKind(comment);
	if((kind != CXCommentKind::CXComment_Null) && (kind != CXCommentKind::CXComment_FullComment)) throw gcnew ArgumentOutOfRangeException("kind");

	// Create a new ParsedComment instance around a new safe handle
	return gcnew ParsedComment(gcnew CommentHandle(owner, transunit, comment), CommentKind(kind));
}

//---------------------------------------------------------------------------
// ParsedComment::ToHtmlFragment
//
// Converts the full comment into an HTML fragment
//
// Arguments:
//
//	NONE

String^ ParsedComment::ToHtmlFragment(void)
{
	return StringUtil::ToString(clang_FullComment_getAsHTML(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// ParsedComment::ToXmlFragment
//
// Converts the full comment into an XML fragment
//
// Arguments:
//
//	NONE

String^ ParsedComment::ToXmlFragment(void)
{
	return StringUtil::ToString(clang_FullComment_getAsXML(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
