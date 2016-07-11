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

#ifndef __INDEXOBJECTIVECPROTOCOLREFERENCE_H_
#define __INDEXOBJECTIVECPROTOCOLREFERENCE_H_
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
// Class IndexObjectiveCProtocolReference
//
// Represents an Objective-C protocol reference during indexing
//---------------------------------------------------------------------------

public ref class IndexObjectiveCProtocolReference
{
public:

	//-----------------------------------------------------------------------
	// Properties

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

	// ProtocolEntity
	//
	// Gets the entity associated with this declaration
	property IndexEntity^ ProtocolEntity
	{
		IndexEntity^ get(void);
	}

internal:

	// Create (static)
	//
	// Creates a new IndexObjectiveCProtocolReference instance
	static IndexObjectiveCProtocolReference^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCProtocolRefInfo* info);

private:

	// IdxObjCProtocolRefInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxObjCProtocolRefInfo*
	using IdxObjCProtocolRefInfoHandle = IndexActionPointerHandle<const CXIdxObjCProtocolRefInfo*>;

	// Instance Constructor
	//
	IndexObjectiveCProtocolReference(IdxObjCProtocolRefInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxObjCProtocolRefInfoHandle^	m_handle;		// Reference handle
	local::Cursor^					m_cursor;		// Cached cursor instance
	IndexEntity^					m_entity;		// Cached entity instance
	local::Location^				m_location;		// Cached location instance
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXOBJECTIVECPROTOCOLREFERENCE_H_
