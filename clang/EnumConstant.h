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

#ifndef __ENUMCONSTANT_H_
#define __ENUMCONSTANT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class EnumConstant
//
// Defines an enumeration cursor constant value, which can be treated as
// signed or unsigned depending on the enumeration type
//---------------------------------------------------------------------------

public value class EnumConstant
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(EnumConstant lhs, EnumConstant rhs);

	// operator!= (static)
	//
	static bool operator!=(EnumConstant lhs, EnumConstant rhs);

	// __int64 conversion operator (implicit)
	//
	static operator __int64(EnumConstant rhs);

	// unsigned __int64 conversion operator (implicit)
	//
	static operator unsigned __int64(EnumConstant rhs);

	// int conversion operator (explicit)
	//
	static explicit operator int(EnumConstant rhs);

	// unsigned int conversion operator (explicit)
	//
	static explicit operator unsigned int(EnumConstant rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this EnumConstant to another EnumConstant
	bool Equals(EnumConstant rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToInt32
	//
	// Converts the value to a 32-bit integer
	int ToInt32(void);

	// ToInt64
	//
	// Converts the value to a 64-bit integer
	__int64 ToInt64(void);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	// ToUInt32
	//
	// Converts the value to a 32-bit unsigned integer
	unsigned int ToUInt32(void);

	// ToUInt64
	//
	// Converts the value to a 64-bit unsigned integer
	unsigned __int64 ToUInt64(void);

	//-----------------------------------------------------------------------
	// Properties

	// UnsignedValue
	//
	// Gets the underlying unsigned integer value
	property unsigned __int64 UnsignedValue
	{
		unsigned __int64 get(void);
	}

	// Value
	//
	// Gets the underlying signed integer value
	property __int64 Value
	{
		__int64 get(void);
	}

internal:

	// Instance Constructor
	//
	EnumConstant(const CXCursor& cursor);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	long long				m_svalue;		// Signed constant value
	unsigned long long		m_uvalue;		// Unsigned constant value
	CXTypeKind				m_typekind;		// Enumeration type kind
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __ENUMCONSTANT_H_
