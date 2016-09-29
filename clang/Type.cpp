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
#include "Type.h"

#include "AutoGCHandle.h"
#include "ArgumentTypeCollection.h"
#include "CallingConvention.h"
#include "Cursor.h"
#include "CxxReferenceQualifier.h"
#include "EnumerateFieldsFunc.h"
#include "EnumerateFieldsResult.h"
#include "GCHandleRef.h"
#include "StringUtil.h"
#include "TemplateArgumentTypeCollection.h"
#include "TypeCollection.h"
#include "TypeFieldOffsets.h"
#include "TypeKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// EnumerateFieldsCallback (local)
//
// Callback for clang_Type_visitFields() that invokes a delegate function against
// each cursor instance that is returned
//
// Arguments:
//
//	cursor			- Current unmanaged CXCursor instance being enumerated
//	context			- Context pointer passed into clang_Type_visitFields

static CXVisitorResult EnumerateFieldsCallback(CXCursor cursor, CXClientData context)
{
	GCHandleRef<EnumerateFieldsFunc^> func(context);

	// Invoke the delegate and capture any exception that occurs
	try { return func->Invoke(cursor); }
	catch(Exception^ exception) { func->Exception = exception; return CXVisitorResult::CXVisit_Break; }
}

//---------------------------------------------------------------------------
// Type Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TypeHandle instance

Type::Type(TypeHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Type::operator == (static)

bool Type::operator==(Type^ lhs, Type^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	// clang_equalTypes returns non-zero if the types are the same
	return (clang_equalTypes(TypeHandle::Reference(lhs->m_handle), TypeHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::operator != (static)

bool Type::operator!=(Type^ lhs, Type^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	// clang_equalTypes returns zero if the types are different
	return (clang_equalTypes(TypeHandle::Reference(lhs->m_handle), TypeHandle::Reference(rhs->m_handle)) == 0);
}

//---------------------------------------------------------------------------
// Type::Alignment::get
//
// Gets the alignment of a type in bytes as per C++ standard

Nullable<__int64> Type::Alignment::get(void)
{
	// clang_Type_getAlignOf can return a negative value to indicate "not valid"
	long long value = clang_Type_getAlignOf(TypeHandle::Reference(m_handle));
	return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
}
	
//---------------------------------------------------------------------------
// Type::ArgumentTypes::get
//
// Gets a collection of argument types

TypeCollection^ Type::ArgumentTypes::get(void)
{
	if(Object::ReferenceEquals(m_argtypes, nullptr))
		m_argtypes = ArgumentTypeCollection::Create(m_handle->Owner, m_handle->TranslationUnit, TypeHandle::Reference(m_handle));

	return m_argtypes;
}

//---------------------------------------------------------------------------
// Type::ArrayElementType::get
//
// Gets the element type of an array type

Type^ Type::ArrayElementType::get(void)
{
	if(Object::ReferenceEquals(m_arraytype, nullptr))
		m_arraytype = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getArrayElementType(TypeHandle::Reference(m_handle)));

	return m_arraytype;
}

//---------------------------------------------------------------------------
// Type::ArraySize::get
//
// Gets the size of a constant array type

Nullable<__int64> Type::ArraySize::get(void)
{
	// clang_getArraySize can return a negative value to indicate "not valid"
	long long value = clang_getArraySize(TypeHandle::Reference(m_handle));
	return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
}

//---------------------------------------------------------------------------
// Type::CallingConvention::get
//
// Gets the calling convention for a function type

local::CallingConvention Type::CallingConvention::get(void)
{
	return local::CallingConvention(clang_getFunctionTypeCallingConv(TypeHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Type::Create (internal, static)
//
// Creates a new Type instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	type		- Unmanaged CXType instance

Type^ Type::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type)
{
	return gcnew Type(gcnew TypeHandle(owner, transunit, type));
}

//---------------------------------------------------------------------------
// Type::CanonicalType::get
//
// Gets the canonical type for this type

Type^ Type::CanonicalType::get(void)
{
	if(Object::ReferenceEquals(m_canonical, nullptr))
		m_canonical = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCanonicalType(TypeHandle::Reference(m_handle)));

	return m_canonical;
}

//---------------------------------------------------------------------------
// Type::ClassType::get
//
// Gets the class type for this type

Type^ Type::ClassType::get(void)
{
	if(Object::ReferenceEquals(m_classtype, nullptr))
		m_classtype = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Type_getClassType(TypeHandle::Reference(m_handle)));

	return m_classtype;
}

//---------------------------------------------------------------------------
// Type::CxxReferenceQualifier::get
//
// Gets the c++ reference qualifier (lvalue/rvalue) for the type

local::CxxReferenceQualifier Type::CxxReferenceQualifier::get(void)
{
	return local::CxxReferenceQualifier(clang_Type_getCXXRefQualifier(TypeHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Type::DeclarationCursor::get
//
// Gets the declaration cursor for this type

Cursor^ Type::DeclarationCursor::get(void)
{
	if(Object::ReferenceEquals(m_declaration, nullptr))
		m_declaration = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getTypeDeclaration(TypeHandle::Reference(m_handle)));

	return m_declaration;
}

//---------------------------------------------------------------------------
// Type::ElementCount::get
//
// Gets the number of elements in an array or vector type

Nullable<__int64> Type::ElementCount::get(void)
{
	// clang_getNumElements can return a negative value to indicate "not valid"
	long long value = clang_getNumElements(TypeHandle::Reference(m_handle));
	return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
}

//---------------------------------------------------------------------------
// Type::ElementType::get
//
// Gets the element type of an array, complex, or vector type

Type^ Type::ElementType::get(void)
{
	if(Object::ReferenceEquals(m_elementtype, nullptr))
		m_elementtype = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getElementType(TypeHandle::Reference(m_handle)));

	return m_elementtype;
}
	
//---------------------------------------------------------------------------
// Type::EnumerateFields
//
// Enumerates the direct field cursors of this type
//
// Arguments:
//
//	func	- Delegate function to invoke for every enumerated field cursor

void Type::EnumerateFields(Func<Cursor^, EnumerateFieldsResult>^ func)
{
	// If the delegate function is null, there is nothing to do
	if(Object::ReferenceEquals(func, nullptr)) return;

	TypeHandle::Reference type(m_handle);	// Keep the translation unit alive

	// Use a wrapper object for the translation unit reference and delegate
	EnumerateFieldsFunc^ enumfunc = EnumerateFieldsFunc::Create(m_handle->Owner, m_handle->TranslationUnit, type, func);
	clang_Type_visitFields(type, EnumerateFieldsCallback, AutoGCHandle(enumfunc));

	// Check if an exception occurred during enumeration and re-throw it
	Exception^ exception = enumfunc->Exception;
	if(!Object::ReferenceEquals(exception, nullptr)) throw exception;
}
	
//---------------------------------------------------------------------------
// Type::Equals
//
// Compares this Type instance to another Type instance
//
// Arguments:
//
//	rhs		- Right-hand Type instance to compare against

bool Type::Equals(Type^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// Type::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool Type::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a Type instance
	Type^ rhsref = dynamic_cast<Type^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// Type::FieldBitOffset::get
//
// Gets a special indexer-only class for accessing field offsets

TypeFieldOffsets^ Type::FieldBitOffset::get(void)
{
	if(Object::ReferenceEquals(m_fieldoffsets, nullptr)) 
		m_fieldoffsets = TypeFieldOffsets::Create(m_handle->Owner, m_handle->TranslationUnit, TypeHandle::Reference(m_handle));

	return m_fieldoffsets;
}

//---------------------------------------------------------------------------
// Type::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int Type::GetHashCode(void)
{
	CXType type = TypeHandle::Reference(m_handle);

	// Calculate the hash value from the components of the unmanaged structure, these are the
	// fields involved for determining equality via clang_equalTypes
	return intptr_t(type.data[0]).GetHashCode() ^ intptr_t(type.data[1]).GetHashCode();
}

//---------------------------------------------------------------------------
// Type::IsConstQualified::Get
//
// Gets a flag indicating if this type is const-qualified

bool Type::IsConstQualified::get(void)
{
	return (clang_isConstQualifiedType(TypeHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::IsPOD::get
//
// Gets a flag indicating if this type is a POD type

bool Type::IsPOD::get(void)
{
	return (clang_isPODType(TypeHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::IsRestrictQualified::Get
//
// Gets a flag indicating if this type is restrict-qualified (Obj-C)

bool Type::IsRestrictQualified::get(void)
{
	return (clang_isRestrictQualifiedType(TypeHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::IsVariadicFunction::get
//
// Gets a flag indicating if this type is a variadic function type

bool Type::IsVariadicFunction::get(void)
{
	return (clang_isFunctionTypeVariadic(TypeHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::IsVolatileQualified::Get
//
// Gets a flag indicating if this type is volatile-qualified

bool Type::IsVolatileQualified::get(void)
{
	return (clang_isVolatileQualifiedType(TypeHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Type::Kind::get
//
// Gets the kind of this type

TypeKind Type::Kind::get(void)
{
	CXType type = TypeHandle::Reference(m_handle);
	return TypeKind(type.kind);
}

//---------------------------------------------------------------------------
// Type::NamedType::get
//
// Gets the named type for this type

Type^ Type::NamedType::get(void)
{
	if(Object::ReferenceEquals(m_named, nullptr))
		m_named = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Type_getNamedType(TypeHandle::Reference(m_handle)));

	return m_named;
}

//---------------------------------------------------------------------------
// Type::ObjectiveCEncoding::get
//
// Gets the Objective-C encoding

String^ Type::ObjectiveCEncoding::get(void)
{
	if(Object::ReferenceEquals(m_objcencoding, nullptr))
		m_objcencoding = StringUtil::ToString(clang_Type_getObjCEncoding(TypeHandle::Reference(m_handle)));

	return m_objcencoding;
}

//---------------------------------------------------------------------------
// Type::PointeeType::get
//
// Gets the pointee type for a pointer type

Type^ Type::PointeeType::get(void)
{
	if(Object::ReferenceEquals(m_pointee, nullptr))
		m_pointee = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getPointeeType(TypeHandle::Reference(m_handle)));

	return m_pointee;
}

//---------------------------------------------------------------------------
// Type::ResultType::get
//
// Gets the result type for a function type
//
// Arguments:
//
//	NONE

Type^ Type::ResultType::get(void)
{
	if(Object::ReferenceEquals(m_resulttype, nullptr))
		m_resulttype = Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getResultType(TypeHandle::Reference(m_handle)));

	return m_resulttype;
}

//---------------------------------------------------------------------------
// Type::Size::get
//
// Gets the size of a type in bytes as per C++ standard

Nullable<__int64> Type::Size::get(void)
{
	// clang_Type_getSizeOf can return a negative value to indicate "not valid"
	long long value = clang_Type_getSizeOf(TypeHandle::Reference(m_handle));
	return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
}
	
//---------------------------------------------------------------------------
// Type::Spelling::get
//
// Gets the spelling of this type

String^ Type::Spelling::get(void)
{
	if(Object::ReferenceEquals(m_spelling, nullptr))
		m_spelling = StringUtil::ToString(clang_getTypeSpelling(TypeHandle::Reference(m_handle)));

	return m_spelling;
}

//---------------------------------------------------------------------------
// Type::TemplateArgumentTypes::get
//
// Gets a collection of template argument types

TypeCollection^ Type::TemplateArgumentTypes::get(void)
{
	if(Object::ReferenceEquals(m_templateargs, nullptr)) 
		m_templateargs = TemplateArgumentTypeCollection::Create(m_handle->Owner, m_handle->TranslationUnit, TypeHandle::Reference(m_handle));

	return m_templateargs;
}

//---------------------------------------------------------------------------
// Type::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Type::ToString(void)
{
	return this->Spelling;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
