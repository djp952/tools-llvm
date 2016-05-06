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
#include "EnumerateFieldsResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateFieldsResult Constructor (internal)
//
// Arguments:
//
//	result		- CXVisitorResult to represent with this class

EnumerateFieldsResult::EnumerateFieldsResult(CXVisitorResult result) : m_result(result)
{
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::operator == (static)

bool EnumerateFieldsResult::operator==(EnumerateFieldsResult lhs, EnumerateFieldsResult rhs)
{
	return lhs.m_result == rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::operator != (static)

bool EnumerateFieldsResult::operator!=(EnumerateFieldsResult lhs, EnumerateFieldsResult rhs)
{
	return lhs.m_result != rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::operator CXVisitorResult (static)

EnumerateFieldsResult::operator CXVisitorResult(EnumerateFieldsResult rhs)
{
	return rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::Equals
//
// Compares this EnumerateFieldsResult to another EnumerateFieldsResult
//
// Arguments:
//
//	rhs		- Right-hand EnumerateFieldsResult to compare against

bool EnumerateFieldsResult::Equals(EnumerateFieldsResult rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool EnumerateFieldsResult::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a EnumerateFieldsResult instance
	EnumerateFieldsResult^ rhsref = dynamic_cast<EnumerateFieldsResult^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int EnumerateFieldsResult::GetHashCode(void)
{
	return static_cast<int>(m_result).GetHashCode();
}

//---------------------------------------------------------------------------
// EnumerateFieldsResult::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ EnumerateFieldsResult::ToString(void)
{
	switch(m_result) {

		case CXVisitorResult::CXVisit_Break:	return gcnew String("Break");
		case CXVisitorResult::CXVisit_Continue: return gcnew String("Continue");
	}

	return String::Format("{0}", static_cast<int>(m_result));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
