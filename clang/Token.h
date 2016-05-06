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

#ifndef __TOKEN_H_
#define __TOKEN_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class	Cursor;
ref class	Extent;
ref class	Location;
enum class	TokenKind;

//---------------------------------------------------------------------------
// Class Token
//
// Represents a single token from the preprocessor
//---------------------------------------------------------------------------

public ref class Token
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Annotation
	//
	// Gets the annotation cursor for this token
	property Cursor^ Annotation
	{
		Cursor^ get(void);
	}

	// Extent
	//
	// Gets the source extent of this token
	property local::Extent^ Extent
	{
		local::Extent^ get(void);
	}

	// Location
	//
	// Gets the location of this token
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// Kind
	//
	// Gets the kind of this token
	property TokenKind Kind
	{
		TokenKind get(void);
	}

	// Spelling
	//
	// Gets the textual representation of the token
	property String^ Spelling
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new Token instance
	static Token^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXToken token, CXCursor annotation);

private:

	// TokenHandle
	//
	// TranslationUnitReferenceHandle specialization for CXToken
	using TokenHandle = TranslationUnitReferenceHandle<CXToken>;

	// Instance Constructor
	//
	Token(TokenHandle^ handle, Cursor^ annotation);

	//-----------------------------------------------------------------------
	// Member Variables

	TokenHandle^				m_handle;			// Underlying safe handle
	Cursor^						m_annotation;		// Annotation cursor
	local::Extent^				m_extent;			// Cached Extent instance
	local::Location^			m_location;			// Cached Location instance
	String^						m_spelling;			// Cached token spelling
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TOKEN_H_
