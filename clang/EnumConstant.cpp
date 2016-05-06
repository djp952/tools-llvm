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
#include "EnumConstant.h"

#include "TypeKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumConstant Constructor (internal)
//
// Arguments:
//
//	cursor			- Unmanaged CXCursor instance

EnumConstant::EnumConstant(const CXCursor& cursor) : m_svalue(0), m_uvalue(0), m_typekind(TypeKind::Invalid)
{
	// The cursor should report a kind of CXCursor_EnumConstantDecl 
	if(clang_getCursorKind(cursor) == CXCursorKind::CXCursor_EnumConstantDecl) {

		// Need to get the parent cursor to know if the type should be treated as
		// signed or unsigned when executing a ToString() operation on it
		CXCursor parent = clang_getCursorSemanticParent(cursor);
		if(clang_getCursorKind(parent) == CXCursorKind::CXCursor_EnumDecl)
			m_typekind = TypeKind(clang_getEnumDeclIntegerType(parent).kind);

		// Grab both the signed and unsigned values as indicated by clang
		m_svalue = clang_getEnumConstantDeclValue(cursor);
		m_uvalue = clang_getEnumConstantDeclUnsignedValue(cursor);
	}
}

//---------------------------------------------------------------------------
// EnumConstant::operator == (static)

bool EnumConstant::operator==(EnumConstant lhs, EnumConstant rhs)
{
	return ((lhs.m_svalue == rhs.m_svalue) && (lhs.m_uvalue == rhs.m_uvalue));
}

//---------------------------------------------------------------------------
// EnumConstant::operator != (static)

bool EnumConstant::operator!=(EnumConstant lhs, EnumConstant rhs)
{
	return ((lhs.m_svalue != rhs.m_svalue) || (lhs.m_uvalue != rhs.m_uvalue));
}

//---------------------------------------------------------------------------
// EnumConstant __int64 conversion operator (static, implicit)

EnumConstant::operator __int64(EnumConstant rhs)
{
	return rhs.ToInt64();
}

//---------------------------------------------------------------------------
// EnumConstant unsigned __int64 conversion operator (static, implicit)

EnumConstant::operator unsigned __int64(EnumConstant rhs)
{
	return rhs.ToUInt64();
}

//---------------------------------------------------------------------------
// EnumConstant int conversion operator (static, explicit)

EnumConstant::operator int(EnumConstant rhs)
{
	return rhs.ToInt32();
}

//---------------------------------------------------------------------------
// EnumConstant unsigned int conversion operator (static, explicit)

EnumConstant::operator unsigned int(EnumConstant rhs)
{
	return rhs.ToUInt32();
}

//---------------------------------------------------------------------------
// EnumConstant::Equals
//
// Compares this EnumConstant to another EnumConstant
//
// Arguments:
//
//	rhs		- Right-hand EnumConstant to compare against

bool EnumConstant::Equals(EnumConstant rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// EnumConstant::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool EnumConstant::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a EnumConstant instance
	EnumConstant^ rhsref = dynamic_cast<EnumConstant^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// EnumConstant::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int EnumConstant::GetHashCode(void)
{
	// The underlying enumeration data type must be checked to use the right value
	switch(m_typekind) {

		// Unsigned integer type - use m_uvalue
		//
		case CXTypeKind::CXType_Char_U:
		case CXTypeKind::CXType_UChar:
		case CXTypeKind::CXType_UShort:
		case CXTypeKind::CXType_UInt:
		case CXTypeKind::CXType_ULong:
		case CXTypeKind::CXType_ULongLong:
		case CXTypeKind::CXType_UInt128:
			return m_uvalue.GetHashCode();

		// Signed integer or unknown type - use m_svalue
		default: return m_svalue.GetHashCode();
	}
}

//---------------------------------------------------------------------------
// EnumConstant::ToInt32
//
// Converts the value to a 32-bit signed integer
//
// Arguments:
//
//	NONE

int EnumConstant::ToInt32(void)
{
	// Check for overflow/underflow of a 32-bit signed integer
	if(m_svalue > static_cast<__int64>(Int32::MaxValue)) throw gcnew OverflowException();
	if(m_svalue < static_cast<__int64>(Int32::MinValue)) throw gcnew OverflowException();

	return static_cast<int>(m_svalue);
}

//---------------------------------------------------------------------------
// EnumConstant::ToInt64
//
// Converts the value to a 64-bit signed integer
//
// Arguments:
//
//	NONE

__int64 EnumConstant::ToInt64(void)
{
	return m_svalue;
}

//---------------------------------------------------------------------------
// EnumConstant::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ EnumConstant::ToString(void)
{
	// In order to generate an accurate string representation, the underlying
	// enumeration data type must be checked.  Default to a signed value
	switch(m_typekind) {

		// Unsigned integer type - use m_uvalue
		//
		case CXTypeKind::CXType_Char_U:
		case CXTypeKind::CXType_UChar:
		case CXTypeKind::CXType_UShort:
		case CXTypeKind::CXType_UInt:
		case CXTypeKind::CXType_ULong:
		case CXTypeKind::CXType_ULongLong:
		case CXTypeKind::CXType_UInt128:
			return m_uvalue.ToString();

		// Signed integer or unknown type - use m_svalue
		default: return m_svalue.ToString();
	}
}

//---------------------------------------------------------------------------
// EnumConstant::ToUInt32
//
// Converts the value to a 32-bit unsigned integer
//
// Arguments:
//
//	NONE

unsigned int EnumConstant::ToUInt32(void)
{
	// Check for overflow/underflow of a 32-bit unsigned integer
	if(m_uvalue > static_cast<unsigned __int64>(UInt32::MaxValue)) throw gcnew OverflowException();
	if(m_uvalue < static_cast<unsigned __int64>(UInt32::MinValue)) throw gcnew OverflowException();

	return static_cast<unsigned int>(m_uvalue);
}

//---------------------------------------------------------------------------
// EnumConstant::ToUInt64
//
// Converts the value to a 64-bit unsigned integer
//
// Arguments:
//
//	NONE

unsigned __int64 EnumConstant::ToUInt64(void)
{
	return m_uvalue;
}

//---------------------------------------------------------------------------
// EnumConstant::UnsignedValue::get
//
// Gets the underlying unsigned integer value

unsigned __int64 EnumConstant::UnsignedValue::get(void)
{
	return m_uvalue;
}

//---------------------------------------------------------------------------
// EnumConstant::Value::get
//
// Gets the underlying signed integer value

__int64 EnumConstant::Value::get(void)
{
	return m_svalue;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
