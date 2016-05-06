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
#include "HtmlStartTagComment.h"

#include "CommentKind.h"
#include "StringDictionary.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// HtmlStartTagComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

HtmlStartTagComment::HtmlStartTagComment(CommentHandle^ handle) : HtmlTagComment(handle, CommentKind::HtmlStartTag)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// HtmlStartTagComment::Attributes::get
//
// Gets a dictionary of HTML tag attributes

StringDictionary^ HtmlStartTagComment::Attributes::get(void)
{
	if(Object::ReferenceEquals(m_attributes, nullptr)) {

		CommentHandle::Reference comment(m_handle);			// Unwrap the safe handle

		// Determine how many attributes will be in the dictionary and create the backing Dictionary<>
		unsigned int numattributes = clang_HTMLStartTag_getNumAttrs(comment);
		Dictionary<String^, String^>^ attributes = gcnew Dictionary<String^, String^>(numattributes);

		// Load the dictionary with each attribute name and value
		for(unsigned int index = 0; index < numattributes; index++)
			attributes->Add(StringUtil::ToString(clang_HTMLStartTag_getAttrName(comment, index)), 
				StringUtil::ToString(clang_HTMLStartTag_getAttrValue(comment, index)));

		m_attributes = StringDictionary::Create(attributes);
	}

	return m_attributes;
}

//---------------------------------------------------------------------------
// HtmlStartTagComment::IsSelfClosing::get
//
// Gets a flag indicating if this HTML start tag is self-closing

bool HtmlStartTagComment::IsSelfClosing::get(void)
{
	return (clang_HTMLStartTagComment_isSelfClosing(CommentHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
