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
#include "Comment.h"

#include "BlockCommandComment.h"
#include "CommentCollection.h"
#include "CommentKind.h"
#include "HTMLEndTagComment.h"
#include "HTMLStartTagComment.h"
#include "InlineCommandComment.h"
#include "NullComment.h"
#include "ParagraphComment.h"
#include "ParamCommandComment.h"
#include "TextComment.h"
#include "TParamCommandComment.h"
#include "VerbatimBlockCommandComment.h"
#include "VerbatimBlockLineComment.h"
#include "VerbatimLineComment.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Comment Constructor (protected)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance
//	kind		- CommentKind being instantiated

Comment::Comment(CommentHandle^ handle, CommentKind kind) : m_handle(handle), m_kind(kind)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Comment::Children::get
//
// Gets a collection of child parsed comment instances

CommentCollection^ Comment::Children::get(void)
{
	if(Object::ReferenceEquals(m_children, nullptr)) 
		m_children = CommentCollection::Create(m_handle->Owner, m_handle->TranslationUnit, CommentHandle::Reference(m_handle));

	return m_children;
}

//---------------------------------------------------------------------------
// Comment::Create (internal, static)
//
// Creates a subclassed Comment instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	comment		- Unmanaged CXComment instance

Comment^ Comment::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment)
{
	// Determine what kind of comment this is
	CommentKind kind = CommentKind(clang_Comment_getKind(comment));

	// Generate a safe handle for the comment to pass into the subclass
	CommentHandle^ handle = gcnew CommentHandle(owner, transunit, comment);

	switch(kind) {

		case CommentKind::Null:					return gcnew NullComment(handle);
		case CommentKind::Text:					return gcnew TextComment(handle);
		case CommentKind::InlineCommand: 		return gcnew InlineCommandComment(handle);
		case CommentKind::HtmlStartTag:			return gcnew HtmlStartTagComment(handle);
		case CommentKind::HtmlEndTag: 			return gcnew HtmlEndTagComment(handle);
		case CommentKind::Paragraph: 			return gcnew ParagraphComment(handle);
		case CommentKind::BlockCommand:			return gcnew BlockCommandComment(handle);
		case CommentKind::ParamCommand:			return gcnew ParamCommandComment(handle);
		case CommentKind::TParamCommand:		return gcnew TParamCommandComment(handle);
		case CommentKind::VerbatimBlockCommand:	return gcnew VerbatimBlockCommandComment(handle);
		case CommentKind::VerbatimBlockLine: 	return gcnew VerbatimBlockLineComment(handle);
		case CommentKind::VerbatimLine:			return gcnew VerbatimLineComment(handle);

		// ParsedComment is a special top-level comment that has to be constructed on its own
		//
		//case CommentKind::Parsed:				return gcnew ParsedComment(handle);
	}

	// Any other comment kind doesn't have a subclass, throw an exception
	throw gcnew ArgumentOutOfRangeException("kind");
}

//---------------------------------------------------------------------------
// Comment::Kind::get
//
// Gets the kind of the parsed comment

CommentKind Comment::Kind::get(void)
{
	return m_kind;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
