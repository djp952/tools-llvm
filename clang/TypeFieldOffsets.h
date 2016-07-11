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

#ifndef __TYPEFIELDOFFSETS_H_
#define __TYPEFIELDOFFSETS_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class TypeFieldOffsets
//
// Implements an indexer-only object used to wrap the type field offset
// function in such a way that it can be used as a property
//---------------------------------------------------------------------------

public ref class TypeFieldOffsets
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// default[String^]
	//
	// Gets the offset of a record field in bits
	property Nullable<__int64> default[String^] 
	{
		virtual Nullable<__int64> get(String^ fieldname);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new TypeFieldOffsets instance
	static TypeFieldOffsets^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type);

private:

	// TypeHandle
	//
	// TranslationUnitReferenceHandle specialization for CXType
	using TypeHandle = TranslationUnitReferenceHandle<CXType>;

	// Instance Constructor
	//
	TypeFieldOffsets(TypeHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	TypeHandle^				m_handle;		// Underlying safe handle
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TYPEFIELDOFFSETS_H_
