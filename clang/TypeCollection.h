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

#ifndef __TYPECOLLECTION_H_
#define __TYPECOLLECTION_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class Type;

//---------------------------------------------------------------------------
// Class TypeCollection
//
// Implements an IReadOnlyList<> based enumerable collection of types
//---------------------------------------------------------------------------

public ref class TypeCollection abstract : public IReadOnlyList<Type^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<Type^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property Type^ default[int] 
	{
		virtual Type^ get(int index) = 0;
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get() = 0;
	}

protected private:

	// Instance Constructors
	//
	TypeCollection();

private:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TYPECOLLECTION_H_
