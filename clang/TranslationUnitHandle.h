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

#ifndef __TRANSLATIONUNITHANDLE_H_
#define __TRANSLATIONUNITHANDLE_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class TranslationUnitHandle (internal)
//
// SafeHandle wrapper around an unmanaged CXTranslationUnit instance
//---------------------------------------------------------------------------

ref class TranslationUnitHandle : public UnmanagedTypeSafeHandle<CXTranslationUnit, clang_disposeTranslationUnit>
{
public:

	// Instance Constructor
	//
	TranslationUnitHandle(SafeHandle^ parent, CXTranslationUnit&& transunit) : UnmanagedTypeSafeHandle(parent, std::move(transunit))
	{
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Handle representing a null translation unit
	static initonly TranslationUnitHandle^ Null = gcnew TranslationUnitHandle(__nullptr);

private:

	// Instance Constructor
	//
	TranslationUnitHandle(nullptr_t) : UnmanagedTypeSafeHandle(CXTranslationUnit(__nullptr))
	{
	}
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TRANSLATIONUNITHANDLE_H_
