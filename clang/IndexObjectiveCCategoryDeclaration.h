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

#ifndef __INDEXOBJECTIVECCATEGORYDECLARATION_H_
#define __INDEXOBJECTIVECCATEGORYDECLARATION_H_
#pragma once

#include "IndexActionPointerHandle.h"
#include "IndexObjectiveCContainerDeclaration.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class IndexAction;
ref class IndexEntity;
ref class IndexObjectiveCProtocolReferenceCollection;
ref class Location;

//---------------------------------------------------------------------------
// Class IndexObjectiveCCategoryDeclaration
//
// Represents information about an objective-C Category declaration
//---------------------------------------------------------------------------

public ref class IndexObjectiveCCategoryDeclaration sealed : public IndexObjectiveCContainerDeclaration
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

	// ProtocolReferences
	//
	// Gets a read-only collection of protocols referenced by this declaration
	property IndexObjectiveCProtocolReferenceCollection^ ProtocolReferences
	{
		IndexObjectiveCProtocolReferenceCollection^ get(void);
	}

internal:

	// Create (static)
	//
	// Creates a new IndexObjectiveCCategoryDeclaration instance
	static IndexObjectiveCCategoryDeclaration^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCCategoryDeclInfo* info);

private:

	// IdxObjCCategoryDeclInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxObjCCategoryDeclInfo*
	using IdxObjCCategoryDeclInfoHandle = IndexActionPointerHandle<const CXIdxObjCCategoryDeclInfo*>;

	// Instance Constructor
	//
	IndexObjectiveCCategoryDeclaration(IdxObjCCategoryDeclInfoHandle^ handle, IdxObjCContainerDeclInfoHandle^ conthandle, IdxDeclInfoHandle^ basehandle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxObjCCategoryDeclInfoHandle^		m_handle;		// Reference handle
	local::Cursor^						m_cursor;		// Cached cursor
	IndexEntity^						m_entity;		// Cached entity
	local::Location^					m_location;		// Cached location

	IndexObjectiveCProtocolReferenceCollection^		m_protocols;
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXOBJECTIVECCATEGORYDECLARATION_H_
