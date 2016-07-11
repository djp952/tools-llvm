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

#ifndef __TRANSLATIONUNITSAVEERRORCODE_H_
#define __TRANSLATIONUNITSAVEERRORCODE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class TranslationUnitSaveErrorCode
//
// Describes an error that occurred in a call to clang_saveTranslationUnit()
//---------------------------------------------------------------------------

public value class TranslationUnitSaveErrorCode
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(TranslationUnitSaveErrorCode lhs, TranslationUnitSaveErrorCode rhs);

	// operator!= (static)
	//
	static bool operator!=(TranslationUnitSaveErrorCode lhs, TranslationUnitSaveErrorCode rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this TranslationUnitSaveErrorCode to another TranslationUnitSaveErrorCode
	bool Equals(TranslationUnitSaveErrorCode rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Fields

	static initonly TranslationUnitSaveErrorCode None					= TranslationUnitSaveErrorCode(CXSaveError::CXSaveError_None);
	static initonly TranslationUnitSaveErrorCode Unknown				= TranslationUnitSaveErrorCode(CXSaveError::CXSaveError_Unknown);
	static initonly TranslationUnitSaveErrorCode TranslationErrors		= TranslationUnitSaveErrorCode(CXSaveError::CXSaveError_TranslationErrors);
	static initonly TranslationUnitSaveErrorCode InvalidTranslationUnit = TranslationUnitSaveErrorCode(CXSaveError::CXSaveError_InvalidTU);

internal:

	// Instance Constructor
	//
	TranslationUnitSaveErrorCode(CXSaveError code);

	//-----------------------------------------------------------------------
	// Internal Operators

	// operator CXSaveError
	//
	// Exposes the value as a CXSaveError enumeration value
	static operator CXSaveError(TranslationUnitSaveErrorCode rhs);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXSaveError					m_code;		// Underling error code
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __TRANSLATIONUNITSAVEERRORCODE_H_
