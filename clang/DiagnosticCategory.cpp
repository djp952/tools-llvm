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
#include "DiagnosticCategory.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// DiagnosticCategory Constructor (internal)
//
// Arguments:
//
//	diagnostic		- Unmanaged CXDiagnostic structure

DiagnosticCategory::DiagnosticCategory(const CXDiagnostic& diagnostic)
{
	m_number = static_cast<int>(clang_getDiagnosticCategory(diagnostic));
	m_text = StringUtil::ToString(clang_getDiagnosticCategoryText(diagnostic));
}

//---------------------------------------------------------------------------
// DiagnosticCategory::operator == (static)

bool DiagnosticCategory::operator==(DiagnosticCategory lhs, DiagnosticCategory rhs)
{
	return lhs.m_number == rhs.m_number;
}

//---------------------------------------------------------------------------
// DiagnosticCategory::operator != (static)

bool DiagnosticCategory::operator!=(DiagnosticCategory lhs, DiagnosticCategory rhs)
{
	return lhs.m_number != rhs.m_number;
}

//---------------------------------------------------------------------------
// DiagnosticCategory::String^ (static)

DiagnosticCategory::operator String^(DiagnosticCategory^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return String::Empty;
	return rhs->Text;
}
	
//---------------------------------------------------------------------------
// DiagnosticCategory::Equals
//
// Compares this DiagnosticCategory to another DiagnosticCategory
//
// Arguments:
//
//	rhs		- Right-hand DiagnosticCategory to compare against

bool DiagnosticCategory::Equals(DiagnosticCategory rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// DiagnosticCategory::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool DiagnosticCategory::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a DiagnosticCategory instance
	DiagnosticCategory^ rhsref = dynamic_cast<DiagnosticCategory^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// DiagnosticCategory::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int DiagnosticCategory::GetHashCode(void)
{
	return m_number.GetHashCode();
}

//---------------------------------------------------------------------------
// DiagnosticCategory::Number::get
//
// Gets the category number

int DiagnosticCategory::Number::get(void)
{
	return m_number;
}

//---------------------------------------------------------------------------
// DiagnosticCategory::Text::get
//
// Gets the name/text for this category

String^ DiagnosticCategory::Text::get(void)
{
	return m_text;
}

//---------------------------------------------------------------------------
// DiagnosticCategory::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ DiagnosticCategory::ToString(void)
{
	return this->Text;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
