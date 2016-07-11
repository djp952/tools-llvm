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
#include "IndexEntityKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexEntityKind Constructor (internal)
//
// Arguments:
//
//	entitykind		- CXIdxEntityKind to represent with this value class

IndexEntityKind::IndexEntityKind(CXIdxEntityKind entitykind) : m_entitykind(entitykind)
{
}

//---------------------------------------------------------------------------
// IndexEntityKind::operator == (static)

bool IndexEntityKind::operator==(IndexEntityKind lhs, IndexEntityKind rhs)
{
	return lhs.m_entitykind == rhs.m_entitykind;
}

//---------------------------------------------------------------------------
// IndexEntityKind::operator != (static)

bool IndexEntityKind::operator!=(IndexEntityKind lhs, IndexEntityKind rhs)
{
	return lhs.m_entitykind != rhs.m_entitykind;
}

//---------------------------------------------------------------------------
// IndexEntityKind::Equals
//
// Compares this IndexEntityKind to another IndexEntityKind
//
// Arguments:
//
//	rhs		- Right-hand IndexEntityKind to compare against

bool IndexEntityKind::Equals(IndexEntityKind rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// IndexEntityKind::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool IndexEntityKind::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a IndexEntityKind instance
	IndexEntityKind^ rhsref = dynamic_cast<IndexEntityKind^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// IndexEntityKind::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int IndexEntityKind::GetHashCode(void)
{
	return static_cast<int>(m_entitykind).GetHashCode();
}

//---------------------------------------------------------------------------
// IndexEntityKind::IsObjectiveCContainer::get
//
// True if this is an Objective-c container

bool IndexEntityKind::IsObjectiveCContainer::get(void)
{
	return (clang_index_isEntityObjCContainerKind(m_entitykind) != 0);
}

//---------------------------------------------------------------------------
// IndexEntityKind::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ IndexEntityKind::ToString(void)
{
	switch(m_entitykind) {

		case CXIdxEntityKind::CXIdxEntity_Unexposed:				return "Unexposed";
		case CXIdxEntityKind::CXIdxEntity_Typedef:					return "Typedef";
		case CXIdxEntityKind::CXIdxEntity_Function:					return "Function";
		case CXIdxEntityKind::CXIdxEntity_Variable:					return "Variable";
		case CXIdxEntityKind::CXIdxEntity_Field:					return "Field";
		case CXIdxEntityKind::CXIdxEntity_EnumConstant:				return "EnumConstant";
		case CXIdxEntityKind::CXIdxEntity_ObjCClass:				return "ObjectiveCClass";
		case CXIdxEntityKind::CXIdxEntity_ObjCProtocol:				return "ObjectiveCProtocol";
		case CXIdxEntityKind::CXIdxEntity_ObjCCategory:				return "ObjectiveCCategory";
		case CXIdxEntityKind::CXIdxEntity_ObjCInstanceMethod:		return "ObjectiveCInstanceMethod";
		case CXIdxEntityKind::CXIdxEntity_ObjCClassMethod:			return "ObjectiveCClassMethod";
		case CXIdxEntityKind::CXIdxEntity_ObjCProperty:				return "ObjectiveCProperty";
		case CXIdxEntityKind::CXIdxEntity_ObjCIvar:					return "ObjectiveCIvar";
		case CXIdxEntityKind::CXIdxEntity_Enum:						return "Enum";
		case CXIdxEntityKind::CXIdxEntity_Struct:					return "Struct";
		case CXIdxEntityKind::CXIdxEntity_Union:					return "Union";
		case CXIdxEntityKind::CXIdxEntity_CXXClass:					return "CxxClass";
		case CXIdxEntityKind::CXIdxEntity_CXXNamespace:				return "CxxNamespace";
		case CXIdxEntityKind::CXIdxEntity_CXXNamespaceAlias:		return "CxxNamespaceAlias";
		case CXIdxEntityKind::CXIdxEntity_CXXStaticVariable:		return "CxxStaticVariable";
		case CXIdxEntityKind::CXIdxEntity_CXXStaticMethod:			return "CxxStaticMethod";
		case CXIdxEntityKind::CXIdxEntity_CXXInstanceMethod:		return "CxxInstanceMethod";
		case CXIdxEntityKind::CXIdxEntity_CXXConstructor:			return "CxxConstructor";
		case CXIdxEntityKind::CXIdxEntity_CXXDestructor:			return "CxxDestructor";
		case CXIdxEntityKind::CXIdxEntity_CXXConversionFunction:	return "CxxConversionFunction";
		case CXIdxEntityKind::CXIdxEntity_CXXTypeAlias:				return "CxxTypeAlias";
		case CXIdxEntityKind::CXIdxEntity_CXXInterface:				return "CxxInterface"; 
	}

	return String::Empty;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
