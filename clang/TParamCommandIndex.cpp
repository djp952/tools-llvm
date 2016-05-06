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
#include "TParamCommandIndex.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// TParamCommandIndex Constructor (private)
//
// Arguments:
//
//	handle		- Underlying CommentHandle instance

TParamCommandIndex::TParamCommandIndex(CommentHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	CommentHandle::Reference comment(handle);			// Unwrap the safe handle

	// The depth and index properties may not be valid for this command, use a Nullable<>
	m_depth = (clang_TParamCommandComment_isParamPositionValid(comment) == 0) ? Nullable<unsigned int>() :
		Nullable<unsigned int>(clang_TParamCommandComment_getDepth(comment));
}

//---------------------------------------------------------------------------
// TParamCommandIndex::default[unsigned int]::get
//
// Gets the template parameter index at the specified depth

Nullable<unsigned int> TParamCommandIndex::default::get(unsigned int nestingdepth)
{
	// If the depth/index as invalid or the depth is out of range, return a null value
	if(!m_depth.HasValue || nestingdepth > m_depth.Value) return Nullable<unsigned int>();

	// Get the parameter index at the specified nesting depth
	return clang_TParamCommandComment_getIndex(CommentHandle::Reference(m_handle), nestingdepth);
}

//---------------------------------------------------------------------------
// TParamCommandIndex::Create (internal, static)
//
// Creates a new TParamCommandIndex
//
// Arguments:
//
//	owner		- Owning SafeHandle instance
//	transunit	- TranslationUnitHandle instance
//	command		- Unmanaged CXComment instance

TParamCommandIndex^ TParamCommandIndex::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXComment comment)
{
	return gcnew TParamCommandIndex(gcnew CommentHandle(owner, transunit, comment));
}

//---------------------------------------------------------------------------
// TParamCommandIndex::NestingDepth::get
//
// Gets the nesting depth of this parameter in the template parameter list

Nullable<unsigned int> TParamCommandIndex::NestingDepth::get(void)
{
	return m_depth;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
