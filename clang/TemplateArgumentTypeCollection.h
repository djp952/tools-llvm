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

#ifndef __TEMPLATEARGUMENTTYPECOLLECTION_H_
#define __TEMPLATEARGUMENTTYPECOLLECTION_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"
#include "TypeCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class Type;

//---------------------------------------------------------------------------
// Class TemplateArgumentTypeCollection (internal)
//
// Implements an IReadOnlyList<> based enumerable collection of types
//---------------------------------------------------------------------------

ref class TemplateArgumentTypeCollection : public TypeCollection
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property Type^ default[int] 
	{
		virtual Type^ get(int index) override;
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get() override;
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new TemplateArgumentTypeCollection instance
	static TypeCollection^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type);

private:

	// TypeHandle
	//
	// TranslationUnitReferenceHandle specialization for CXType
	using TypeHandle = TranslationUnitReferenceHandle<CXType>;

	// Instance Constructor
	//
	TemplateArgumentTypeCollection(TypeHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	TypeHandle^				m_handle;		// Underlying safe handle
	array<Type^>^			m_cache;		// Cached Type elements
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TEMPLATEARGUMENTTYPECOLLECTION_H_
