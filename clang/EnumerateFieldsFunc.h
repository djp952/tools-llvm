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

#ifndef __ENUMERATEFIELDSFUNC_H_
#define __ENUMERATEFIELDSFUNC_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	Cursor;
value class	EnumerateFieldsResult;

//---------------------------------------------------------------------------
// Class EnumerateFieldsFunc (internal)
//
// Context object used to enumerate the fields of a type via a delegate
//---------------------------------------------------------------------------

ref class EnumerateFieldsFunc
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Invoke
	//
	// Invokes the contained delegate function for the specified cursor
	EnumerateFieldsResult Invoke(const CXCursor& cursor);

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

	// Create (static)
	//
	// Creates a new EnumerateFieldsFunc instance
	static EnumerateFieldsFunc^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type, Func<Cursor^, EnumerateFieldsResult>^ func);

private:

	// TypeHandle
	//
	// TranslationUnitReferenceHandle specialization for CXType
	using TypeHandle = TranslationUnitReferenceHandle<CXType>;

	// Instance Constructor
	//
	EnumerateFieldsFunc(TypeHandle^ handle, Func<Cursor^, EnumerateFieldsResult>^ func);

	//-----------------------------------------------------------------------
	// Member Variables
	
	TypeHandle^								m_handle;		// Owning Type instance
	Func<Cursor^, EnumerateFieldsResult>^	m_func;			// Delegate to invoke
	System::Exception^						m_exception;	// Exception instance
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __ENUMERATEFIELDSFUNC_H_
