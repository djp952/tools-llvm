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

#ifndef __INDEXENTITY_H_
#define __INDEXENTITY_H_
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
enum class	IndexEntityCxxTemplateKind;
value class	IndexEntityKind;
enum class	IndexEntityLanguage;
ref class	UnifiedSymbolResolution;

//---------------------------------------------------------------------------
// Class IndexEntity
//
// Represents information about an entity during indexing
//---------------------------------------------------------------------------

public ref class IndexEntity
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// IsNull (static)
	//
	// Determines if the specified container object is null
	static bool IsNull(IndexEntity^ rhs);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Attributes
	//
	// Gets a read-only collection of entity attributes
	property IndexAttributeCollection^ Attributes
	{
		IndexAttributeCollection^ get(void);
	}

	// ClientData
	//
	// Gets/sets an object to associate with this entity
	property Object^ ClientData
	{
		Object^ get(void);
		void set(Object^ value);
	}

	// Cursor
	//
	// Gets the cursor associated with this entity
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// CxxTemplateKind
	//
	// Gets the entity C++ template kind, if any
	property IndexEntityCxxTemplateKind CxxTemplateKind
	{
		IndexEntityCxxTemplateKind get(void);
	}

	// Kind
	//
	// Get the entity kind
	property IndexEntityKind Kind
	{
		IndexEntityKind get(void);
	}

	// Language
	//
	// Gets the entity language
	property IndexEntityLanguage Language
	{
		IndexEntityLanguage get(void);
	}

	// Name
	//
	// Gets the entity name
	property String^ Name
	{
		String^ get(void);
	}

	// UnifiedSymbolResolution
	//
	// Gets the Unified Symbol Resultion (USR) string for the entity
	property local::UnifiedSymbolResolution^ UnifiedSymbolResolution
	{
		local::UnifiedSymbolResolution^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexEntity instance
	static IndexEntity^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxEntityInfo* info);

private:

	// IdxEntityInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxEntityInfo*
	using IdxEntityInfoHandle = IndexActionPointerHandle<const CXIdxEntityInfo*>;

	// Instance Constructor
	//
	IndexEntity(IdxEntityInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxEntityInfoHandle^				m_handle;		// Reference handle
	String^								m_name;			// Cached entity name
	local::UnifiedSymbolResolution^		m_usr;			// Cached entity USR
	IndexAttributeCollection^			m_attributes;	// Cached attributes
	local::Cursor^						m_cursor;		// Cached cursor
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXENTITY_H_
