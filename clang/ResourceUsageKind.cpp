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
#include "ResourceUsageKind.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// ResourceUsageKind Constructor (internal)
//
// Arguments:
//
//	kind		- CXTUResourceUsageKind value to be represented by this class

ResourceUsageKind::ResourceUsageKind(CXTUResourceUsageKind kind) : m_kind(kind)
{
}

//---------------------------------------------------------------------------
// ResourceUsageKind::operator == (static)

bool ResourceUsageKind::operator==(ResourceUsageKind lhs, ResourceUsageKind rhs)
{
	return lhs.m_kind == rhs.m_kind;
}

//---------------------------------------------------------------------------
// ResourceUsageKind::operator != (static)

bool ResourceUsageKind::operator!=(ResourceUsageKind lhs, ResourceUsageKind rhs)
{
	return lhs.m_kind != rhs.m_kind;
}

//---------------------------------------------------------------------------
// ResourceUsageKind::Equals
//
// Compares this ResourceUsageKind to another ResourceUsageKind
//
// Arguments:
//
//	rhs		- Right-hand ResourceUsageKind to compare against

bool ResourceUsageKind::Equals(ResourceUsageKind rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// ResourceUsageKind::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool ResourceUsageKind::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a ResourceUsageKind instance
	ResourceUsageKind^ rhsref = dynamic_cast<ResourceUsageKind^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// ResourceUsageKind::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int ResourceUsageKind::GetHashCode(void)
{
	return static_cast<int>(m_kind).GetHashCode();
}

//---------------------------------------------------------------------------
// ResourceUsageKind::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ ResourceUsageKind::ToString(void)
{
	// The strings from clang_getTUResourceUsageName can be used directly
	return StringUtil::ToString(clang_getTUResourceUsageName(m_kind), CP_UTF8);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
