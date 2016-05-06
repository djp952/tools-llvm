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
#include "EnumerateReferencesResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateReferencesResult Constructor (internal)
//
// Arguments:
//
//	result		- CXVisitorResult to represent with this class

EnumerateReferencesResult::EnumerateReferencesResult(CXVisitorResult result) : m_result(result)
{
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::operator == (static)

bool EnumerateReferencesResult::operator==(EnumerateReferencesResult lhs, EnumerateReferencesResult rhs)
{
	return lhs.m_result == rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::operator != (static)

bool EnumerateReferencesResult::operator!=(EnumerateReferencesResult lhs, EnumerateReferencesResult rhs)
{
	return lhs.m_result != rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::operator CXVisitorResult (static)

EnumerateReferencesResult::operator CXVisitorResult(EnumerateReferencesResult rhs)
{
	return rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::Equals
//
// Compares this FindCursorResult to another FindCursorResult
//
// Arguments:
//
//	rhs		- Right-hand EnumerateReferencesResult to compare against

bool EnumerateReferencesResult::Equals(EnumerateReferencesResult rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool EnumerateReferencesResult::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a EnumerateReferencesResult instance
	EnumerateReferencesResult^ rhsref = dynamic_cast<EnumerateReferencesResult^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int EnumerateReferencesResult::GetHashCode(void)
{
	return static_cast<int>(m_result).GetHashCode();
}

//---------------------------------------------------------------------------
// EnumerateReferencesResult::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ EnumerateReferencesResult::ToString(void)
{
	switch(m_result) {

		case CXVisitorResult::CXVisit_Break:	return gcnew String("Break");
		case CXVisitorResult::CXVisit_Continue:	return gcnew String("Continue");
	}

	return String::Format("{0}", static_cast<int>(m_result));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
