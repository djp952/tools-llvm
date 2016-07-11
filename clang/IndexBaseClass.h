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

#ifndef __INDEXBASECLASS_H_
#define __INDEXBASECLASS_H_
#pragma once

#include "IndexActionPointerHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class IndexAction;
ref class IndexEntity;
ref class Location;

//---------------------------------------------------------------------------
// Class IndexBaseClass
//
// Represents a C++/Objective-C base class during indexing
//---------------------------------------------------------------------------

public ref class IndexBaseClass
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// IsNull (static)
	//
	// Determines if the specified base class object is null
	static bool IsNull(IndexBaseClass^ rhs);

	//-----------------------------------------------------------------------
	// Properties

	// BaseEntity
	//
	// Gets the entity associated with this declaration
	property IndexEntity^ BaseEntity
	{
		IndexEntity^ get(void);
	}

	// Cursor
	//
	// Gets the cursor associated with this declaration
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// Location
	//
	// Gets the location associated with this declaration
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

internal:

	// Create (static)
	//
	// Creates a new IndexBaseClass instance
	static IndexBaseClass^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxBaseClassInfo* info);

private:

	// IdxBaseClassInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxBaseClassInfo*
	using IdxBaseClassInfoHandle = IndexActionPointerHandle<const CXIdxBaseClassInfo*>;

	// Instance Constructor
	//
	IndexBaseClass(IdxBaseClassInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxBaseClassInfoHandle^			m_handle;		// Reference handle
	local::Cursor^					m_cursor;		// Cached cursor instance
	IndexEntity^					m_entity;		// Cached entity instance
	local::Location^				m_location;		// Cached location instance
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXBASECLASS_H_
