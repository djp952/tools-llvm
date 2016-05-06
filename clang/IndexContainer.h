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

#ifndef __INDEXCONTAINER_H_
#define __INDEXCONTAINER_H_
#pragma once

#include "IndexActionPointerHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class IndexAction;

//---------------------------------------------------------------------------
// Class IndexContainer
//
// Represents information about an entity container during indexing
//---------------------------------------------------------------------------

public ref class IndexContainer
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// IsNull (static)
	//
	// Determines if the specified container object is null
	static bool IsNull(IndexContainer^ rhs);

	//-----------------------------------------------------------------------
	// Properties

	// ClientData
	//
	// Gets/sets an object to associate with this container
	property Object^ ClientData
	{
		Object^ get(void);
		void set(Object^ value);
	}

	// Cursor
	//
	// Gets the reference cursor
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexContainer instance
	static IndexContainer^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxContainerInfo* info);

private:

	// IdxDeclInfoHandle
	//
	// IndexActionPointerHandle specialization
	using IdxContainerInfoHandle = IndexActionPointerHandle<const CXIdxContainerInfo*>;

	// Instance Constructor
	//
	IndexContainer(IdxContainerInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxContainerInfoHandle^		m_handle;		// Reference handle
	local::Cursor^				m_cursor;		// Cached cursor instance
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXCONTAINER_H_
