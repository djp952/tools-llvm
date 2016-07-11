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
#include "TParamCommandComment.h"

#include "CommentKind.h"
#include "StringUtil.h"
#include "TParamCommandIndex.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// TParamCommandComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

TParamCommandComment::TParamCommandComment(CommentHandle^ handle) : BlockCommandComment(handle, CommentKind::TParamCommand)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_indexes = TParamCommandIndex::Create(handle->Owner, handle->TranslationUnit, CommentHandle::Reference(handle));
}

//---------------------------------------------------------------------------
// TParamCommandComment::Index::get
//
// Gets the index in the template parameter list at a given nesting depth

TParamCommandIndex^ TParamCommandComment::Index::get(void)
{
	return m_indexes;
}

//---------------------------------------------------------------------------
// TParamCommandComment::ParameterName::get
//
// Gets the template parameter name

String^ TParamCommandComment::ParameterName::get(void)
{
	if(Object::ReferenceEquals(m_paramname, nullptr))
		m_paramname = StringUtil::ToString(clang_TParamCommandComment_getParamName(CommentHandle::Reference(m_handle)));

	return m_paramname;
}
	
//---------------------------------------------------------------------------
// TParamCommandComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ TParamCommandComment::ToString(void)
{
	return this->ParameterName;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
