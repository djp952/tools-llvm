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

#ifndef __TYPE_H_
#define __TYPE_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class	ArrayType;
enum class	CallingConvention;
ref class	Cursor;
enum class	CxxReferenceQualifier;
value class EnumerateFieldsResult;
ref class	TypeCollection;
ref class	TypeFieldOffsets;
value class	TypeKind;

//---------------------------------------------------------------------------
// Class Type
//
// The type of an element in the abstract syntax tree
//---------------------------------------------------------------------------

public ref class Type
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(Type^ lhs, Type^ rhs);

	// operator!= (static)
	//
	static bool operator!=(Type^ lhs, Type^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// EnumerateFields
	//
	// Enumerates the direct field cursor of this type
	void EnumerateFields(Func<Cursor^, EnumerateFieldsResult>^ func);

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this Type instance to another Type instance
	bool Equals(Type^ rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Alignment
	//
	// Gets the alignment of a type in bytes as per C++ standard
	property Nullable<__int64> Alignment
	{
		Nullable<__int64> get(void);
	}

	// ArgumentTypes
	//
	// Gets a collection of template argument types
	property TypeCollection^ ArgumentTypes
	{
		TypeCollection^ get(void);
	}

	// ArrayElementType
	//
	// Gets the element type of an array type
	property Type^ ArrayElementType
	{
		Type^ get(void);
	}

	// ArraySize
	//
	// Gets the size of a constant array type
	property Nullable<__int64> ArraySize
	{
		Nullable<__int64> get(void);
	}

	// CallingConvention
	//
	// Gets the calling convention for a function type
	property local::CallingConvention CallingConvention
	{
		local::CallingConvention get(void);
	}

	// CanonicalType
	//
	// Gets the canonical type for this type
	property Type^ CanonicalType
	{
		Type^ get(void);
	}

	// ClassType
	//
	// Gets the class type for this type
	property Type^ ClassType
	{
		Type^ get(void);
	}

	// CxxReferenceQualifier
	//
	// Gets the c++ reference qualifier (lvalue/rvalue) for the type
	property local::CxxReferenceQualifier CxxReferenceQualifier
	{
		local::CxxReferenceQualifier get(void);
	}

	// DeclarationCursor
	//
	// Gets the declaration cursor of this type
	property Cursor^ DeclarationCursor
	{
		Cursor^ get(void);
	}

	// ElementCount
	//
	// Gets the number of elements in an array or vector type
	property Nullable<__int64> ElementCount
	{
		Nullable<__int64> get(void);
	}

	// ElementType
	//
	// Gets the element type of an array, complex, or vector type
	property Type^ ElementType
	{
		Type^ get(void);
	}

	// FieldBitOffset
	//
	// Gets a special indexer-only class for accessing field offsets
	property TypeFieldOffsets^ FieldBitOffset
	{
		TypeFieldOffsets^ get(void);
	}

	// IsConstQualified
	//
	// Gets a flag indicating if this type is const-qualified
	property bool IsConstQualified
	{
		bool get(void);
	}

	// IsPOD
	//
	// Gets a flag indicating if this type is a plain old data type
	property bool IsPOD
	{
		bool get(void);
	}

	// IsRestrictQualified
	//
	// Gets a flag indicating if this type is restrict-qualified (Obj-C)
	property bool IsRestrictQualified
	{
		bool get(void);
	}

	// IsVariadicFunction
	//
	// Gets a flag indicating if this type is a variadic function type
	property bool IsVariadicFunction
	{
		bool get(void);
	}

	// IsVolatileQualified
	//
	// Gets a flag indicating if this type is volatile-qualified
	property bool IsVolatileQualified
	{
		bool get(void);
	}

	// Kind
	//
	// Gets the kind of this type
	property TypeKind Kind
	{
		TypeKind get(void);
	}

	// NamedType
	//
	// Gets the named type for this type
	property Type^ NamedType
	{
		Type^ get(void);
	}

	// ObjectiveCEncoding
	//
	// Gets the Objective-C encoding
	property String^ ObjectiveCEncoding
	{
		String^ get(void);
	}

	// PointeeType
	//
	// Gets the pointee type for pointer types
	property Type^ PointeeType
	{
		Type^ get(void);
	}

	// ResultType
	//
	// Gets the result type for a function type
	property Type^ ResultType
	{
		Type^ get(void);
	}

	// Size
	//
	// Gets the size of a type in bytes as per C++ standard
	property Nullable<__int64> Size
	{
		Nullable<__int64> get(void);
	}

	// Spelling
	//
	// Gets the spelling of this Type
	property String^ Spelling
	{
		String^ get(void);
	}

	// TemplateArgumentTypes
	//
	// Gets a collection of template argument types
	property TypeCollection^ TemplateArgumentTypes
	{
		TypeCollection^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new Type instance
	static Type^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type);

private:

	// TypeHandle
	//
	// TranslationUnitReferenceHandle specialization for CXType
	using TypeHandle = TranslationUnitReferenceHandle<CXType>;

	// Instance Constructor
	//
	Type(TypeHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	TypeHandle^				m_handle;			// Underlying safe handle
	String^					m_spelling;			// Cached type spelling
	TypeCollection^			m_argtypes;			// Cached argument types
	Type^					m_arraytype;		// Cached array element type
	Type^					m_canonical;		// Cached canonical type
	Type^					m_classtype;		// Cached class type
	Cursor^					m_declaration;		// Cached declaration cursor
	Type^					m_elementtype;		// Cached element type
	Type^					m_pointee;			// Cached pointee type
	Type^					m_resulttype;		// Cached result type
	TypeCollection^			m_templateargs;		// Cached template arg types
	TypeFieldOffsets^		m_fieldoffsets;		// Cached field offsets
	Type^					m_named;			// Cached named type
	String^					m_objcencoding;		// Cached Obj-C encoding
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TYPE_H_
