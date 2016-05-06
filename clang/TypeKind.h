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

#ifndef __TYPEKIND_H_
#define __TYPEKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class TypeKind
//
// Describes the kind of a type
//---------------------------------------------------------------------------

public value class TypeKind
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(TypeKind lhs, TypeKind rhs);

	// operator!= (static)
	//
	static bool operator!=(TypeKind lhs, TypeKind rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this TypeKind to another TypeKind
	bool Equals(TypeKind rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Fields

	static initonly TypeKind Invalid					= TypeKind(CXTypeKind::CXType_Invalid);
	static initonly TypeKind Unexposed					= TypeKind(CXTypeKind::CXType_Unexposed);
	static initonly TypeKind Void						= TypeKind(CXTypeKind::CXType_Void);
	static initonly TypeKind Bool						= TypeKind(CXTypeKind::CXType_Bool);
	static initonly TypeKind Char_U						= TypeKind(CXTypeKind::CXType_Char_U);
	static initonly TypeKind UChar						= TypeKind(CXTypeKind::CXType_UChar);
	static initonly TypeKind Char16						= TypeKind(CXTypeKind::CXType_Char16);
	static initonly TypeKind Char32						= TypeKind(CXTypeKind::CXType_Char32);
	static initonly TypeKind UShort						= TypeKind(CXTypeKind::CXType_UShort);
	static initonly TypeKind UInt						= TypeKind(CXTypeKind::CXType_UInt);
	static initonly TypeKind ULong						= TypeKind(CXTypeKind::CXType_ULong);
	static initonly TypeKind ULongLong					= TypeKind(CXTypeKind::CXType_ULongLong);
	static initonly TypeKind UInt128					= TypeKind(CXTypeKind::CXType_UInt128);
	static initonly TypeKind Char_S						= TypeKind(CXTypeKind::CXType_Char_S);
	static initonly TypeKind SChar						= TypeKind(CXTypeKind::CXType_SChar);
	static initonly TypeKind WChar						= TypeKind(CXTypeKind::CXType_WChar);
	static initonly TypeKind Short						= TypeKind(CXTypeKind::CXType_Short);
	static initonly TypeKind Int						= TypeKind(CXTypeKind::CXType_Int);
	static initonly TypeKind Long						= TypeKind(CXTypeKind::CXType_Long);
	static initonly TypeKind LongLong					= TypeKind(CXTypeKind::CXType_LongLong);
	static initonly TypeKind Int128						= TypeKind(CXTypeKind::CXType_Int128);
	static initonly TypeKind Float						= TypeKind(CXTypeKind::CXType_Float);
	static initonly TypeKind Double						= TypeKind(CXTypeKind::CXType_Double);
	static initonly TypeKind LongDouble					= TypeKind(CXTypeKind::CXType_LongDouble);
	static initonly TypeKind NullPtr					= TypeKind(CXTypeKind::CXType_NullPtr);
	static initonly TypeKind Overload					= TypeKind(CXTypeKind::CXType_Overload);
	static initonly TypeKind Dependent					= TypeKind(CXTypeKind::CXType_Dependent);
	static initonly TypeKind ObjectiveCId				= TypeKind(CXTypeKind::CXType_ObjCId);
	static initonly TypeKind ObjectiveCClass			= TypeKind(CXTypeKind::CXType_ObjCClass);
	static initonly TypeKind ObjectiveCSel				= TypeKind(CXTypeKind::CXType_ObjCSel);
	static initonly TypeKind Complex					= TypeKind(CXTypeKind::CXType_Complex);
	static initonly TypeKind Pointer					= TypeKind(CXTypeKind::CXType_Pointer);
	static initonly TypeKind BlockPointer				= TypeKind(CXTypeKind::CXType_BlockPointer);
	static initonly TypeKind LValueReference			= TypeKind(CXTypeKind::CXType_LValueReference);
	static initonly TypeKind RValueReference			= TypeKind(CXTypeKind::CXType_RValueReference);
	static initonly TypeKind Record						= TypeKind(CXTypeKind::CXType_Record);
	static initonly TypeKind Enum						= TypeKind(CXTypeKind::CXType_Enum);
	static initonly TypeKind Typedef					= TypeKind(CXTypeKind::CXType_Typedef);
	static initonly TypeKind ObjectiveCInterface		= TypeKind(CXTypeKind::CXType_ObjCInterface);
	static initonly TypeKind ObjectiveCObjectPointer	= TypeKind(CXTypeKind::CXType_ObjCObjectPointer);
	static initonly TypeKind FunctionNoProto			= TypeKind(CXTypeKind::CXType_FunctionNoProto);
	static initonly TypeKind FunctionProto				= TypeKind(CXTypeKind::CXType_FunctionProto);
	static initonly TypeKind ConstantArray				= TypeKind(CXTypeKind::CXType_ConstantArray);
	static initonly TypeKind Vector						= TypeKind(CXTypeKind::CXType_Vector);
	static initonly TypeKind IncompleteArray			= TypeKind(CXTypeKind::CXType_IncompleteArray);
	static initonly TypeKind VariableArray				= TypeKind(CXTypeKind::CXType_VariableArray);
	static initonly TypeKind DependentSizedArray		= TypeKind(CXTypeKind::CXType_DependentSizedArray);
	static initonly TypeKind MemberPointer				= TypeKind(CXTypeKind::CXType_MemberPointer);
	static initonly TypeKind Auto						= TypeKind(CXTypeKind::CXType_Auto);

internal:

	// Instance Constructor
	//
	TypeKind(CXTypeKind typekind);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXTypeKind
	//
	// Exposes the value as a CXTypeKind enumeration value
	static operator CXTypeKind(TypeKind rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXTypeKind			m_typekind;			// Underlying CXTypeKind
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TYPEKIND_H_
