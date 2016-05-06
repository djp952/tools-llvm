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
#include "ParamCommandComment.h"

#include "CommentKind.h"
#include "ParameterDirection.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// ParamCommandComment Constructor (internal)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

ParamCommandComment::ParamCommandComment(CommentHandle^ handle) : BlockCommandComment(handle, CommentKind::ParamCommand)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// ParamCommandComment::Direction::get
//
// Get the parameter passing direction

ParameterDirection ParamCommandComment::Direction::get(void)
{
	return ParameterDirection(clang_ParamCommandComment_getDirection(CommentHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// ParamCommandComment::Index::get
//
// Gets the parameter index in the function prototype

Nullable<unsigned int> ParamCommandComment::Index::get(void)
{
	CommentHandle::Reference comment(m_handle);			// Unwrap the safe handle

	// The index is zero-based but may not be valid, use a Nullable<> value instead of
	// exposing something like an .IsIndexValid property the caller would have to check
	return (clang_ParamCommandComment_isParamIndexValid(comment) == 0) ? Nullable<unsigned int>() : 
		Nullable<unsigned int>(clang_ParamCommandComment_getParamIndex(comment));
}

//---------------------------------------------------------------------------
// ParamCommandComment::IsDirectionExplicit::get
//
// Gets a flag indicating if parameter passing direction was specified explicitly

bool ParamCommandComment::IsDirectionExplicit::get(void)
{
	return (clang_ParamCommandComment_isDirectionExplicit(CommentHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// ParamCommandComment::ParameterName::get
//
// Gets the parameter name

String^ ParamCommandComment::ParameterName::get(void)
{
	if(Object::ReferenceEquals(m_paramname, nullptr))
		m_paramname = StringUtil::ToString(clang_ParamCommandComment_getParamName(CommentHandle::Reference(m_handle)));

	return m_paramname;
}
	
//---------------------------------------------------------------------------
// ParamCommandComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ ParamCommandComment::ToString(void)
{
	return this->ParameterName;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
