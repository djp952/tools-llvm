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

#ifndef __TEMPLATEARGUMENT_H_
#define __TEMPLATEARGUMENT_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
enum class	TemplateArgumentKind;
ref class	Type;

//---------------------------------------------------------------------------
// Class TemplateArgument
//
// Represents a cursor template specialization argument
//---------------------------------------------------------------------------

public ref class TemplateArgument
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Kind
	//
	// Gets the template argument kind
	property TemplateArgumentKind Kind
	{
		TemplateArgumentKind get(void);
	}

	// Type
	//
	// Gets the template argument type
	property local::Type^ Type
	{
		local::Type^ get(void);
	}

	// UnsignedValue
	//
	// Gets the unsigned template argument value
	property Nullable<unsigned __int64> UnsignedValue
	{
		Nullable<unsigned __int64> get(void);
	}

	// Value
	//
	// Gets the signed template argument value
	property Nullable<__int64> Value
	{
		Nullable<__int64> get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new TemplateArgument instance
	static TemplateArgument^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor, unsigned int index);

private:

	// CursorHandle
	//
	// TranslationUnitReferenceHandle specialization for CXCursor
	using CursorHandle = TranslationUnitReferenceHandle<CXCursor>;

	// Instance Constructor
	//
	TemplateArgument(CursorHandle^ handle, unsigned int index);

	//-----------------------------------------------------------------------
	// Member Variables

	TemplateArgumentKind		m_kind;		// Kind of template argument
	local::Type^				m_type;		// Template argument type
	Nullable<__int64>			m_ivalue;	// Signed argument value
	Nullable<unsigned __int64>	m_uvalue;	// Unsigned argument value
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TEMPLATEARGUMENT_H_
