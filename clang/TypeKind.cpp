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
#include "TypeKind.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// TypeKind Constructor (internal)
//
// Arguments:
//
//	typekind		- CXTypeKind to represent with this value class

TypeKind::TypeKind(CXTypeKind typekind) : m_typekind(typekind)
{
}

//---------------------------------------------------------------------------
// TypeKind::operator == (static)

bool TypeKind::operator==(TypeKind lhs, TypeKind rhs)
{
	return lhs.m_typekind == rhs.m_typekind;
}

//---------------------------------------------------------------------------
// TypeKind::operator != (static)

bool TypeKind::operator!=(TypeKind lhs, TypeKind rhs)
{
	return lhs.m_typekind != rhs.m_typekind;
}

//---------------------------------------------------------------------------
// TypeKind::operator CXTypeKind (static)

TypeKind::operator CXTypeKind(TypeKind rhs)
{
	return rhs.m_typekind;
}

//---------------------------------------------------------------------------
// TypeKind::Equals
//
// Compares this TypeKind to another TypeKind
//
// Arguments:
//
//	rhs		- Right-hand TypeKind to compare against

bool TypeKind::Equals(TypeKind rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// TypeKind::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool TypeKind::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a TypeKind instance
	TypeKind^ rhsref = dynamic_cast<TypeKind^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// TypeKind::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int TypeKind::GetHashCode(void)
{
	return static_cast<int>(m_typekind).GetHashCode();
}

//---------------------------------------------------------------------------
// TypeKind::ToString
//
// Overrides Object::ToString()

String^ TypeKind::ToString(void)
{
	return StringUtil::ToString(clang_getTypeKindSpelling(m_typekind));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
