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
#include "InlineCommandComment.h"

#include "CommentKind.h"
#include "InlineCommandRenderKind.h"
#include "StringCollection.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// InlineCommandComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

InlineCommandComment::InlineCommandComment(CommentHandle^ handle) : InlineContentComment(handle, CommentKind::InlineCommand)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// InlineCommandComment::Arguments::get
//
// Gets a collection of word-like argument texts

StringCollection^ InlineCommandComment::Arguments::get(void)
{
	if(Object::ReferenceEquals(m_arguments, nullptr)) {

		CommentHandle::Reference comment(m_handle);		// Unwrap the safe handle

		// Determine how many strings will be in the collection and create the backing List<>
		unsigned int numstrings = clang_InlineCommandComment_getNumArgs(comment);
		List<String^>^ strings = gcnew List<String^>(numstrings);

		// Load the collection with each argument text string
		for(unsigned int index = 0; index < numstrings; index++)
			strings->Add(StringUtil::ToString(clang_InlineCommandComment_getArgText(comment, index)));

		m_arguments = StringCollection::Create(strings);
	}

	return m_arguments;
}

//---------------------------------------------------------------------------
// InlineCommandComment::CommandName::get
//
// Gets the name of the inline command

String^ InlineCommandComment::CommandName::get(void)
{
	if(Object::ReferenceEquals(m_commandname, nullptr))
		m_commandname = StringUtil::ToString(clang_InlineCommandComment_getCommandName(CommentHandle::Reference(m_handle)));

	return m_commandname;
}

//---------------------------------------------------------------------------
// InlineCommandComment::RenderKind::get
//
// Gets the most appropriate rendering kind for this comment

InlineCommandRenderKind InlineCommandComment::RenderKind::get(void)
{
	return InlineCommandRenderKind(clang_InlineCommandComment_getRenderKind(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// InlineCommandComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ InlineCommandComment::ToString(void)
{
	return this->CommandName;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
