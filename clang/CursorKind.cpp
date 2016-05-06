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
#include "CursorKind.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CursorKind Constructor (internal)
//
// Arguments:
//
//	CursorKind		- CXCursorKind to represent with this value class

CursorKind::CursorKind(CXCursorKind cursorkind) : m_cursorkind(cursorkind)
{
}

//---------------------------------------------------------------------------
// CursorKind::operator == (static)

bool CursorKind::operator==(CursorKind lhs, CursorKind rhs)
{
	return lhs.m_cursorkind == rhs.m_cursorkind;
}

//---------------------------------------------------------------------------
// CursorKind::operator != (static)

bool CursorKind::operator!=(CursorKind lhs, CursorKind rhs)
{
	return lhs.m_cursorkind != rhs.m_cursorkind;
}

//---------------------------------------------------------------------------
// CursorKind::Equals
//
// Compares this CursorKind to another CursorKind
//
// Arguments:
//
//	rhs		- Right-hand CursorKind to compare against

bool CursorKind::Equals(CursorKind rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// CursorKind::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool CursorKind::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a CursorKind instance
	CursorKind^ rhsref = dynamic_cast<CursorKind^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// CursorKind::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int CursorKind::GetHashCode(void)
{
	return static_cast<int>(m_cursorkind).GetHashCode();
}

//---------------------------------------------------------------------------
// CursorKind::IsAttribute::get
//
// True if this is an attribute kind

bool CursorKind::IsAttribute::get(void)
{
	return (clang_isAttribute(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsDeclaration::get
//
// True if this is a declaration kind

bool CursorKind::IsDeclaration::get(void)
{
	return (clang_isDeclaration(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsExpression::get
//
// True if this is an expression kind

bool CursorKind::IsExpression::get(void)
{
	return (clang_isExpression(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsInvalid::get
//
// True if this is an invalid kind

bool CursorKind::IsInvalid::get(void)
{
	return (clang_isInvalid(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsPreprocessing::get
//
// True if this is a preprocessing kind

bool CursorKind::IsPreprocessing::get(void)
{
	return (clang_isPreprocessing(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsReference::get
//
// True if this is a reference kind

bool CursorKind::IsReference::get(void)
{
	return (clang_isReference(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsStatement::get
//
// True if this is a statement kind

bool CursorKind::IsStatement::get(void)
{
	return (clang_isStatement(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsTranslationUnit::get
//
// True if this is a translation unit kind

bool CursorKind::IsTranslationUnit::get(void)
{
	return (clang_isTranslationUnit(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsUnexposed::get
//
// True if this is an unexposed kind

bool CursorKind::IsUnexposed::get(void)
{
	return (clang_isUnexposed(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ CursorKind::ToString(void)
{
	return StringUtil::ToString(clang_getCursorKindSpelling(m_cursorkind));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
