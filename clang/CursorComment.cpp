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
#include "CursorComment.h"

#include "Extent.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CursorComment Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

CursorComment::CursorComment(CursorHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CursorComment::String^ (static)

CursorComment::operator String^(CursorComment^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return String::Empty;
	return rhs->Text;
}
	
//---------------------------------------------------------------------------
// CursorComment::Create (static, internal)
//
// Creates a new CursorComment instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	cursor		- Unmanaged CXCursor instance

CursorComment^ CursorComment::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor)
{
	return gcnew CursorComment(gcnew CursorHandle(owner, transunit, cursor));
}

//---------------------------------------------------------------------------
// CursorComment::Extent::get
//
// Gets the extent of the comment

local::Extent^ CursorComment::Extent::get(void)
{
	if(Object::ReferenceEquals(m_extent, nullptr))
		m_extent = local::Extent::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Cursor_getCommentRange(CursorHandle::Reference(m_handle)));

	return m_extent;
}

//---------------------------------------------------------------------------
// CursorComment::Text::get
//
// Gets the text associated with the cursor

String^ CursorComment::Text::get(void)
{
	if(Object::ReferenceEquals(m_text, nullptr))
		m_text = StringUtil::ToString(clang_Cursor_getRawCommentText(CursorHandle::Reference(m_handle)));

	return m_text;
}

//---------------------------------------------------------------------------
// CursorComment::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ CursorComment::ToString(void)
{
	return this->Text;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
