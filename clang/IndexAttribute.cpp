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
#include "IndexAttribute.h"

#include "Cursor.h"
#include "IndexAttributeKind.h"
#include "IndexIBOutletCollectionAttribute.h"
#include "Location.h"
#include "LocationKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexAttribute Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexAttribute::IndexAttribute(IdxAttrInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexAttribute::Create (internal, static)
//
// Creates a new IndexAttribute instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the attribute information

IndexAttribute^ IndexAttribute::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxAttrInfo* info)
{
	// CXIdxIBOutletCollectionAttrInfo --> IndexIBOutletCollectionAttribute
	//
	const CXIdxIBOutletCollectionAttrInfo* iboattr = clang_index_getIBOutletCollectionAttrInfo(info);
	if(iboattr != __nullptr) return IndexIBOutletCollectionAttribute::Create(owner, action, iboattr);

	// default --> IndexAttribute
	//
	else return gcnew IndexAttribute(gcnew IdxAttrInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexAttribute::Cursor::get
//
// Gets the attribute cursor

local::Cursor^ IndexAttribute::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxAttrInfoHandle::Reference(m_handle)->cursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexAttribute::Kind::get
//
// Gets the attribute kind

IndexAttributeKind IndexAttribute::Kind::get(void)
{
	return IndexAttributeKind(IdxAttrInfoHandle::Reference(m_handle)->kind);
}

//---------------------------------------------------------------------------
// IndexAttribute::Location::get
//
// Gets the attribute location

local::Location^ IndexAttribute::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, clang_indexLoc_getCXSourceLocation(IdxAttrInfoHandle::Reference(m_handle)->loc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
