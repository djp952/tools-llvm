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

#include "stdafx.h"
#include "TranslationUnitSaveErrorCode.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode Constructor (internal)
//
// Arguments:
//
//	code		- CXSaveError value to be represented by this class

TranslationUnitSaveErrorCode::TranslationUnitSaveErrorCode(CXSaveError code) : m_code(code)
{
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::operator == (static)

bool TranslationUnitSaveErrorCode::operator==(TranslationUnitSaveErrorCode lhs, TranslationUnitSaveErrorCode rhs)
{
	return lhs.m_code == rhs.m_code;
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::operator != (static)

bool TranslationUnitSaveErrorCode::operator!=(TranslationUnitSaveErrorCode lhs, TranslationUnitSaveErrorCode rhs)
{
	return lhs.m_code != rhs.m_code;
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::operator CXSaveError (static)

TranslationUnitSaveErrorCode::operator CXSaveError(TranslationUnitSaveErrorCode rhs)
{
	return rhs.m_code;
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::Equals
//
// Compares this TranslationUnitSaveErrorCode to another TranslationUnitSaveErrorCode
//
// Arguments:
//
//	rhs		- Right-hand TranslationUnitSaveErrorCode to compare against

bool TranslationUnitSaveErrorCode::Equals(TranslationUnitSaveErrorCode rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool TranslationUnitSaveErrorCode::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a TranslationUnitSaveErrorCode instance
	TranslationUnitSaveErrorCode^ rhsref = dynamic_cast<TranslationUnitSaveErrorCode^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int TranslationUnitSaveErrorCode::GetHashCode(void)
{
	return static_cast<int>(m_code).GetHashCode();
}

//---------------------------------------------------------------------------
// TranslationUnitSaveErrorCode::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ TranslationUnitSaveErrorCode::ToString(void)
{
	// There are only a few generic error codes to deal with, use static text based
	// on the documentation in the libclang header file, can always be adjusted later
	switch(m_code) {

		case CXSaveError::CXSaveError_None:
			return gcnew String("No error");

		case CXSaveError::CXSaveError_Unknown:
			return gcnew String("An unknown error occurred while attempting to save the file");

		case CXSaveError::CXSaveError_TranslationErrors:
			return gcnew String("Errors during translation prevented this attempt to save the translation unit");

		case CXSaveError::CXSaveError_InvalidTU:
			return gcnew String("The translation unit is invalid");
	}

	return gcnew String("Unknown translation unit save error");
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
