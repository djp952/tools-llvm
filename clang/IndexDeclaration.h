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

#ifndef __INDEXDECLARATION_H_
#define __INDEXDECLARATION_H_
#pragma once

#include "IndexActionPointerHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class	Cursor;
ref class	IndexAction;
ref class	IndexAttributeCollection;
ref class	IndexContainer;
enum class	IndexDeclarationFlags;
enum class	IndexDeclarationKind;
ref class	IndexEntity;
ref class	Location;

//---------------------------------------------------------------------------
// Class IndexDeclaration
//
// Represents information about a declaration during indexing
//---------------------------------------------------------------------------

public ref class IndexDeclaration
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// AsContainer
	//
	// Treats this declaration as a container object
	IndexContainer^ AsContainer(void);

	//-----------------------------------------------------------------------
	// Properties

	// Attributes
	//
	// Gets a read-only collection of declaration attributes
	property IndexAttributeCollection^ Attributes
	{
		IndexAttributeCollection^ get(void);
	}

	// Cursor
	//
	// Gets the declaration cursor
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// Entity
	//
	// Gets the entity associated with this declaration
	property IndexEntity^ Entity
	{
		IndexEntity^ get(void);
	}

	// Flags
	//
	// Gets the flags associated with this declaration
	property IndexDeclarationFlags Flags
	{
		IndexDeclarationFlags get(void);
	}

	// IsContainer
	//
	// Gets a flag indicating if this declaration is a container
	property bool IsContainer
	{
		bool get(void);
	}

	// IsDefinition
	//
	// Gets a flag indicating if this declaration is a definition
	property bool IsDefinition
	{
		bool get(void);
	}

	// IsImplicit
	//
	// Gets a flag indicating if the declaration exists in code or was created implicitly
	property bool IsImplicit
	{
		bool get(void);
	}

	// IsRedeclaration
	//
	// Gets a flag indicating if this declaration is a redeclaration
	property bool IsRedeclaration
	{
		bool get(void);
	}

	// Kind
	//
	// Gets the kind of index declaration this instance represents
	property IndexDeclarationKind Kind
	{
		IndexDeclarationKind get(void);
	}

	// LexicalContainer
	//
	// Gets the declaration lexical container
	property IndexContainer^ LexicalContainer
	{
		IndexContainer^ get(void);
	}

	// Location
	//
	// Gets the declaration location
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// SemanticContainer
	//
	// Gets the declaration semantic container
	property IndexContainer^ SemanticContainer
	{
		IndexContainer^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexDeclaration instance
	static IndexDeclaration^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxDeclInfo* info);

protected private:

	// IdxDeclInfoHandle
	//
	// IndexActionPointerHandle specialization
	using IdxDeclInfoHandle = IndexActionPointerHandle<const CXIdxDeclInfo*>;

	// Instance Constructor
	//
	IndexDeclaration(IdxDeclInfoHandle^ handle, IndexDeclarationKind kind);

	//-----------------------------------------------------------------------
	// Protected Member Variables

	IdxDeclInfoHandle^			m_handle;			// Reference handle
	IndexDeclarationKind		m_kind;				// Declaration kind

private:

	//-----------------------------------------------------------------------
	// Member Variables

	local::Cursor^				m_cursor;			// Cached cursor instance
	local::Location^			m_location;			// Cached location instance
	IndexEntity^				m_entity;			// Cached entity information
	IndexContainer^				m_semcontainer;		// Cached semantic cont.
	IndexContainer^				m_lexcontainer;		// Cached lexical cont.
	IndexAttributeCollection^	m_attributes;		// Cached attributes
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXDECLARATION_H_
