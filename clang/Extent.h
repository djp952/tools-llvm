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

#ifndef __EXTENT_H_
#define __EXTENT_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class	Location;
enum class	LocationKind;
ref class	TokenCollection;

//---------------------------------------------------------------------------
// Class Extent
//
// Describes a range of source locations within the source code
//---------------------------------------------------------------------------

public ref class Extent
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(Extent^ lhs, Extent^ rhs);

	// operator!= (static)
	//
	static bool operator!=(Extent^ lhs, Extent^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this Extent instance to another Extent instance
	bool Equals(Extent^ rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// GetTokens
	//
	// Tokenizes the extent
	TokenCollection^ GetTokens(void);

	// IsNull (static)
	//
	// Determines if the specified Extent is null
	static bool IsNull(Extent^ rhs);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	// ToString
	//
	// Generates a string representation based on specific location types
	String^ ToString(LocationKind kind);

	//-----------------------------------------------------------------------
	// Properties

	// End
	//
	// Gets the end location of the extent
	property Location^ End
	{
		Location^ get(void);
	}

	// Start
	//
	// Gets the start location of the extent
	property Location^ Start
	{
		Location^ get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Represents a null extent
	static initonly Extent^ Null;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Contains
	//
	// Determines if the specified source location is contained within this extent
	bool Contains(const CXSourceLocation& location);

	// Create (static)
	//
	// Creates a new Extent instance
	static Extent^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceRange extent);

private:

	// SourceRangeHandle
	//
	// TranslationUnitReferenceHandle specialization for CXSourceRange
	using SourceRangeHandle = TranslationUnitReferenceHandle<CXSourceRange>;

	// Instance Constructors
	//
	Extent(SourceRangeHandle^ handle);

	// Static Constructor
	//
	static Extent();

	//-----------------------------------------------------------------------
	// Member Variables

	SourceRangeHandle^			m_handle;		// Underlying safe handle
	Location^					m_start;		// Cached start location
	Location^					m_end;			// Cached end location
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __EXTENT_H_
