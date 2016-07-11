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
#include "EnumerateChildrenResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// EnumerateChildrenResult Constructor (internal)
//
// Arguments:
//
//	result		- CXChildVisitResult to represent with this class

EnumerateChildrenResult::EnumerateChildrenResult(CXChildVisitResult result) : m_result(result)
{
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::operator == (static)

bool EnumerateChildrenResult::operator==(EnumerateChildrenResult lhs, EnumerateChildrenResult rhs)
{
	return lhs.m_result == rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::operator != (static)

bool EnumerateChildrenResult::operator!=(EnumerateChildrenResult lhs, EnumerateChildrenResult rhs)
{
	return lhs.m_result != rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::operator CXChildVisitResult (static)

EnumerateChildrenResult::operator CXChildVisitResult(EnumerateChildrenResult rhs)
{
	return rhs.m_result;
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::Equals
//
// Compares this EnumerateChildrenResult to another EnumerateChildrenResult
//
// Arguments:
//
//	rhs		- Right-hand EnumerateChildrenResult to compare against

bool EnumerateChildrenResult::Equals(EnumerateChildrenResult rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool EnumerateChildrenResult::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a EnumerateChildrenResult instance
	EnumerateChildrenResult^ rhsref = dynamic_cast<EnumerateChildrenResult^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int EnumerateChildrenResult::GetHashCode(void)
{
	return static_cast<int>(m_result).GetHashCode();
}

//---------------------------------------------------------------------------
// EnumerateChildrenResult::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ EnumerateChildrenResult::ToString(void)
{
	switch(m_result) {

		case CXChildVisitResult::CXChildVisit_Break:	return gcnew String("Break");
		case CXChildVisitResult::CXChildVisit_Continue: return gcnew String("Continue");
		case CXChildVisitResult::CXChildVisit_Recurse:	return gcnew String("Recurse");
	}

	return String::Format("{0}", static_cast<int>(m_result));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
