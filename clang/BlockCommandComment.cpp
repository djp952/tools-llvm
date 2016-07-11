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
#include "BlockCommandComment.h"

#include "CommentKind.h"
#include "ParagraphComment.h"
#include "StringCollection.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// BlockCommandComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

BlockCommandComment::BlockCommandComment(CommentHandle^ handle) : BlockCommandComment(handle, CommentKind::BlockCommand)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// BlockCommandComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance
//	kind		- Kind of derived comment being constructed

BlockCommandComment::BlockCommandComment(CommentHandle^ handle, CommentKind kind) : BlockContentComment(handle, kind)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// BlockCommandComment::Arguments::get
//
// Gets a collection of word-like argument texts

StringCollection^ BlockCommandComment::Arguments::get(void)
{
	if(Object::ReferenceEquals(m_arguments, nullptr)) {

		CommentHandle::Reference comment(m_handle);		// Unwrap the safe handle

		// Determine how many strings will be in the collection and create the backing List<>
		unsigned int numstrings = clang_BlockCommandComment_getNumArgs(comment);
		List<String^>^ strings = gcnew List<String^>(numstrings);

		// Load the collection with each argument text string
		for(unsigned int index = 0; index < numstrings; index++)
			strings->Add(StringUtil::ToString(clang_BlockCommandComment_getArgText(comment, index)));

		m_arguments = StringCollection::Create(strings);
	}

	return m_arguments;
}

//---------------------------------------------------------------------------
// BlockCommandComment::CommandName::get
//
// Gets the name of the block command

String^ BlockCommandComment::CommandName::get(void)
{
	if(Object::ReferenceEquals(m_commandname, nullptr))
		m_commandname = StringUtil::ToString(clang_BlockCommandComment_getCommandName(CommentHandle::Reference(m_handle)));

	return m_commandname;
}

//---------------------------------------------------------------------------
// BlockCommandComment::Paragraph
//
// Gets the paragraph argument of the block command

ParagraphComment^ BlockCommandComment::Paragraph::get(void)
{
	if(Object::ReferenceEquals(m_paragraph, nullptr)) {

		// Looks like this always creates a ParagraphComment in the clang code, so return one explicitly rather than a generic Comment
		m_paragraph = gcnew ParagraphComment(gcnew CommentHandle(m_handle->Owner, m_handle->TranslationUnit, clang_BlockCommandComment_getParagraph(CommentHandle::Reference(m_handle))));
	}

	return m_paragraph;
}

//---------------------------------------------------------------------------
// BlockCommandComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ BlockCommandComment::ToString(void)
{
	return this->CommandName;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
