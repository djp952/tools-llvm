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

#ifndef __ENUMERATECHILDRENFUNC_H_
#define __ENUMERATECHILDRENFUNC_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	Cursor;
value class	EnumerateChildrenResult;

//---------------------------------------------------------------------------
// Class EnumerateChildrenFunc (internal)
//
// Context object used to enumerate descendant cursors
//---------------------------------------------------------------------------

ref class EnumerateChildrenFunc
{
public:
	
	//-----------------------------------------------------------------------
	// Member Functions

	// Invoke
	//
	// Invokes the contained delegate function for the specified cursor
	EnumerateChildrenResult Invoke(const CXCursor& cursor, const CXCursor& parent);

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
	// Creates a new EnumerateChildrenFunc instance
	static EnumerateChildrenFunc^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor, Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func);

private:

	// CursorHandle
	//
	// TranslationUnitReferenceHandle specialization for CXCursor
	using CursorHandle = TranslationUnitReferenceHandle<CXCursor>;

	// Instance Constructor
	//
	EnumerateChildrenFunc(CursorHandle^ handle, Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func);

	//-----------------------------------------------------------------------
	// Member Variables
	
	CursorHandle^										m_handle;
	Func<Cursor^, Cursor^, EnumerateChildrenResult>^	m_func;
	System::Exception^									m_exception;
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __ENUMERATECHILDRENFUNC_H_
