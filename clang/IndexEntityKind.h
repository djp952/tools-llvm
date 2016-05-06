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

#ifndef __INDEXENTITYKIND_H_
#define __INDEXENTITYKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum IndexEntityKind
//
// Describes the kind of an indexer entity
//---------------------------------------------------------------------------

public value class IndexEntityKind
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(IndexEntityKind lhs, IndexEntityKind rhs);

	// operator!= (static)
	//
	static bool operator!=(IndexEntityKind lhs, IndexEntityKind rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this IndexEntityKind to another IndexEntityKind
	bool Equals(IndexEntityKind rhs);

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

	// IsObjectiveCContainer
	//
	// True if this is an Objective-c container
	property bool IsObjectiveCContainer
	{
		bool get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	static initonly IndexEntityKind Unexposed					= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Unexposed);
	static initonly IndexEntityKind Typedef						= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Typedef);
	static initonly IndexEntityKind Function					= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Function);
	static initonly IndexEntityKind Variable					= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Variable);
	static initonly IndexEntityKind Field						= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Field);
	static initonly IndexEntityKind EnumConstant				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_EnumConstant);
	static initonly IndexEntityKind ObjectiveCClass				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCClass);
	static initonly IndexEntityKind ObjectiveCProtocol			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCProtocol);
	static initonly IndexEntityKind ObjectiveCCategory			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCCategory);
	static initonly IndexEntityKind ObjectiveCInstanceMethod	= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCInstanceMethod);
	static initonly IndexEntityKind ObjectiveCClassMethod		= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCClassMethod);
	static initonly IndexEntityKind ObjectiveCProperty			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCProperty);
	static initonly IndexEntityKind ObjectiveCIvar				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_ObjCIvar);
	static initonly IndexEntityKind Enum						= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Enum);
	static initonly IndexEntityKind Struct						= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Struct);
	static initonly IndexEntityKind Union						= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_Union);
	static initonly IndexEntityKind CxxClass					= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXClass);
	static initonly IndexEntityKind CxxNamespace				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXNamespace);
	static initonly IndexEntityKind CxxNamespaceAlias			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXNamespaceAlias);
	static initonly IndexEntityKind CxxStaticVariable			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXStaticVariable);
	static initonly IndexEntityKind CxxStaticMethod				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXStaticMethod);
	static initonly IndexEntityKind CxxInstanceMethod			= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXInstanceMethod);
	static initonly IndexEntityKind CxxConstructor				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXConstructor);
	static initonly IndexEntityKind CxxDestructor				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXDestructor);
	static initonly IndexEntityKind CxxConversionFunction		= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXConversionFunction);
	static initonly IndexEntityKind CxxTypeAlias				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXTypeAlias);
	static initonly IndexEntityKind CxxInterface				= IndexEntityKind(CXIdxEntityKind::CXIdxEntity_CXXInterface);

internal:

	// Instance Constructor
	//
	IndexEntityKind(CXIdxEntityKind entitykind);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXIdxEntityKind			m_entitykind;		// Underlying CXIdxEntityKind
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXENTITYKIND_H_
