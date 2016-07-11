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

#ifndef __INDEXIBOUTLETCOLLECTIONATTRIBUTE_H_
#define __INDEXIBOUTLETCOLLECTIONATTRIBUTE_H_
#pragma once

#include "IndexActionPointerHandle.h"
#include "IndexAttribute.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class IndexAction;
ref class IndexEntity;
ref class Location;

//---------------------------------------------------------------------------
// Class IndexIBOutletCollectionAttribute
//
// Represents information about an attribute during indexing
//---------------------------------------------------------------------------

public ref class IndexIBOutletCollectionAttribute : public IndexAttribute
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// ClassCursor
	//
	// Gets the cursor associated with this declaration
	property local::Cursor^ ClassCursor
	{
		local::Cursor^ get(void);
	}

	// ClassEntity
	//
	// Gets the entity associated with this declaration
	property IndexEntity^ ClassEntity
	{
		IndexEntity^ get(void);
	}

	// ClassLocation
	//
	// Gets the location associated with this declaration
	property local::Location^ ClassLocation
	{
		local::Location^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexIBOutletCollectionAttribute instance
	static IndexIBOutletCollectionAttribute^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxIBOutletCollectionAttrInfo* info);

private:

	// IdxIBOutletCollectionAttrInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxIBOutletCollectionAttrInfo*
	using IdxIBOutletCollectionAttrInfoHandle = IndexActionPointerHandle<const CXIdxIBOutletCollectionAttrInfo*>;

	// Instance Constructor
	//
	IndexIBOutletCollectionAttribute(IdxIBOutletCollectionAttrInfoHandle^ handle, IdxAttrInfoHandle^ attrhandle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxIBOutletCollectionAttrInfoHandle^	m_handle;		// Reference handle
	local::Cursor^							m_cursor;		// Cached cursor
	IndexEntity^							m_entity;		// Cached entity
	local::Location^						m_location;		// Cached location
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXIBOUTLETCOLLECTIONATTRIBUTE_H_
