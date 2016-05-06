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
#include "TypeFieldOffsets.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// TypeFieldOffsets Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TypeHandle instance

TypeFieldOffsets::TypeFieldOffsets(TypeHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// TypeFieldOffsets::default[String^]::get
//
// Gets the offset of a record field in bits

Nullable<__int64> TypeFieldOffsets::default::get(String^ fieldname)
{
	char* pszfieldname = StringUtil::ToCharPointer(fieldname, CP_UTF8);

	try { 

		// clang_Type_getOffsetOf can return a negative value to indicate "not valid"
		long long value = clang_Type_getOffsetOf(TypeHandle::Reference(m_handle), pszfieldname);
		return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
	}

	finally { StringUtil::FreeCharPointer(pszfieldname); }
}

//---------------------------------------------------------------------------
// TypeFieldOffsets::Create (internal, static)
//
// Creates a new TypeFieldOffsets
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	type		- Unmanaged CXType instance

TypeFieldOffsets^ TypeFieldOffsets::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type)
{
	return gcnew TypeFieldOffsets(gcnew TypeHandle(owner, transunit, type));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
