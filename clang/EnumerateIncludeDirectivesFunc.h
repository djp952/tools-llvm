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

#ifndef __ENUMERATEINCLUDEDIRECTIVESFUNC_H_
#define __ENUMERATEINCLUDEDIRECTIVESFUNC_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class	Cursor;
value class	EnumerateIncludeDirectivesResult;
ref class	Extent;

//---------------------------------------------------------------------------
// Class EnumerateIncludeDirectivesFunc (internal)
//
// Context object used to enumerate file include directive cursors
//---------------------------------------------------------------------------

ref class EnumerateIncludeDirectivesFunc
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Invoke
	//
	// Invokes the contained delegate function for the specified cursor/extent
	EnumerateIncludeDirectivesResult Invoke(const CXCursor& cursor, const CXSourceRange& extent);

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
	// Creates a new EnumerateIncludeDirectivesFunc
	static EnumerateIncludeDirectivesFunc^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXFile file, Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func);

private:

	// FileHandle
	//
	// TranslationUnitReferenceHandle specialization for CXFile
	using FileHandle = TranslationUnitReferenceHandle<CXFile>;

	// Instance Constructor
	//
	EnumerateIncludeDirectivesFunc(FileHandle^ handle, Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func);

	//-----------------------------------------------------------------------
	// Member Variables

	FileHandle^													m_handle;
	Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^	m_func;
	System::Exception^											m_exception;
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __ENUMERATEINCLUDEDIRECTIVESFUNC_H_
