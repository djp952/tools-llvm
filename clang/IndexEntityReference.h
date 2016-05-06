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

#ifndef __INDEXENTITYREFERENCE_H_
#define __INDEXENTITYREFERENCE_H_
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
ref class	IndexContainer;
ref class	IndexEntity;
enum class	IndexEntityReferenceKind;
ref class	Location;

//---------------------------------------------------------------------------
// Class IndexEntityReference
//
// Represents information about an entity reference during indexing
//---------------------------------------------------------------------------

public ref class IndexEntityReference
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Container
	//
	// Gets the entity lexical container
	property IndexContainer^ Container
	{
		IndexContainer^ get(void);
	}

	// Cursor
	//
	// Gets the reference cursor
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// Kind
	//
	// Gets the kind of the entity reference
	property IndexEntityReferenceKind Kind
	{
		IndexEntityReferenceKind get(void);
	}

	// Location
	//
	// Gets the reference location
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// ParentEntity
	//
	// Gets information about the parent entity
	property IndexEntity^ ParentEntity
	{
		IndexEntity^ get(void);
	}

	// ReferencedEntity
	//
	// Gets the entity being referenced
	property IndexEntity^ ReferencedEntity
	{
		IndexEntity^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexEntityReference instance
	static IndexEntityReference^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxEntityRefInfo* info);

private:

	// IdxEntityRefInfoHandle
	//
	// IndexActionPointerHandle specialization
	using IdxEntityRefInfoHandle = IndexActionPointerHandle<const CXIdxEntityRefInfo*>;

	// Instance Constructor
	//
	IndexEntityReference(IdxEntityRefInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxEntityRefInfoHandle^		m_handle;		// Reference handle
	local::Cursor^				m_cursor;		// Cached cursor instance
	local::Location^			m_location;		// Cached location instance
	IndexEntity^				m_parent;		// Cached parent entity
	IndexEntity^				m_referenced;	// Cached referenced entity
	IndexContainer^				m_container;	// Cached container instance
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXENTITYREFERENCE_H_
