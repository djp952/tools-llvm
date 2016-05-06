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
#include "Token.h"

#include "Cursor.h"
#include "Extent.h"
#include "Location.h"
#include "LocationKind.h"
#include "StringUtil.h"
#include "TokenKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Token Constructor (private)
//
// Arguments:
//
//	handle			- Underlying TokenHandle instance
//	annotation		- Annotation cursor for this token

Token::Token(TokenHandle^ handle, Cursor^ annotation) : m_handle(handle), m_annotation(annotation)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	if(Object::ReferenceEquals(annotation, nullptr)) throw gcnew ArgumentNullException("annotation");
}

//---------------------------------------------------------------------------
// Token::Annotation::get
//
// Gets the annotation cursor for this token

Cursor^ Token::Annotation::get(void)
{
	return m_annotation;
}

//---------------------------------------------------------------------------
// Token::Create (internal, static)
//
// Creates a new Token instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	token		- Unmanaged CXToken instance
//	annotation	- Unmanaged CXCursor instance

Token^ Token::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXToken token, CXCursor annotation)
{
	// Create the token handle with the specified owner (typically a token set), but create the cursor
	// with the translation unit as the owner since it doesn't need to be disposed of when the token set is
	return gcnew Token(gcnew TokenHandle(owner, transunit, token), Cursor::Create(transunit, transunit, annotation));
}

//---------------------------------------------------------------------------
// Token::Extent::get
//
// Gets the source extent of this token

local::Extent^ Token::Extent::get(void)
{
	if(Object::ReferenceEquals(m_extent, nullptr)) {

		// Give ownership of the Extent to the TranslationUnit rather than the token set
		TokenHandle::Reference token(m_handle);
		m_extent = local::Extent::Create(m_handle->TranslationUnit, m_handle->TranslationUnit, clang_getTokenExtent(token.TranslationUnit, token));
	}

	return m_extent;
}

//---------------------------------------------------------------------------
// Token::Location::get
//
// Gets the source location of this token

local::Location^ Token::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) {

		// Give ownership of the Location to the TranslationUnit rather than the token set
		TokenHandle::Reference token(m_handle);
		m_location = local::Location::Create(m_handle->TranslationUnit, m_handle->TranslationUnit, clang_getTokenLocation(token.TranslationUnit, token), LocationKind::Spelling);
	}

	return m_location;
}

//---------------------------------------------------------------------------
// Token::Kind::get
//
// Gets the kind of this token

TokenKind Token::Kind::get(void)
{
	return TokenKind(clang_getTokenKind(TokenHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Token::Spelling::get
//
// Gets the textual representation of the token

String^ Token::Spelling::get(void)
{
	if(Object::ReferenceEquals(m_spelling, nullptr)) {

		TokenHandle::Reference token(m_handle);
		m_spelling = StringUtil::ToString(clang_getTokenSpelling(token.TranslationUnit, token));
	}

	return m_spelling;
}

//---------------------------------------------------------------------------
// Token::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Token::ToString(void)
{
	return this->Spelling;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
