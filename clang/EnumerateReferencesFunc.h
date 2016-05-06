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

#ifndef __ENUMERATEREFERENCESFUNC_H_
#define __ENUMERATEREFERENCESFUNC_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	Cursor;
ref class	Extent;
value class	EnumerateReferencesResult;

//---------------------------------------------------------------------------
// Class EnumerateReferencesFunc (internal)
//
// Context object used to enumerate cursor file references
//---------------------------------------------------------------------------

ref class EnumerateReferencesFunc
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Invoke
	//
	// Invokes the contained delegate function for the specified cursor/extent
	EnumerateReferencesResult Invoke(const CXCursor& cursor, const CXSourceRange& extent);

	//-----------------------------------------------------------------------
	// Properties

	// Exception
	//
	// Gets/Sets an exception instance for the enumeration operation
	property System::Exception^ Exception
	{
		System::Exception^ get(void);
		void set(System::Exception^ value);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new EnumerateReferencesFunc instance
	static EnumerateReferencesFunc^ Create(SafeHandle^ owner, TranslationUnitHandle^ handle, CXCursor cursor, Func<Cursor^, Extent^, EnumerateReferencesResult>^ func);

private:

	// CursorHandle
	//
	// TranslationUnitReferenceHandle specialization for CXCursor
	using CursorHandle = TranslationUnitReferenceHandle<CXCursor>;

	// Instance Constructor
	//
	EnumerateReferencesFunc(CursorHandle^ handle, Func<Cursor^, Extent^, EnumerateReferencesResult>^ func);

	//-----------------------------------------------------------------------
	// Member Variables

	CursorHandle^										m_handle;
	Func<Cursor^, Extent^, EnumerateReferencesResult>^	m_func;
	System::Exception^									m_exception;
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __ENUMERATEREFERENCESFUNC_H_
