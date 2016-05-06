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
#include "IndexIBOutletCollectionAttribute.h"

#include "Cursor.h"
#include "IndexEntity.h"
#include "Location.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexIBOutletCollectionAttribute Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	attrhandle	- IdxAttrInfoHandle to pass to the base class

IndexIBOutletCollectionAttribute::IndexIBOutletCollectionAttribute(IdxIBOutletCollectionAttrInfoHandle^ handle, IdxAttrInfoHandle^ attrhandle) 
	: m_handle(handle), IndexAttribute(attrhandle)
{
}

//---------------------------------------------------------------------------
// IndexIBOutletCollectionAttribute::ClassCursor::get
//
// Gets the declaration cursor

local::Cursor^ IndexIBOutletCollectionAttribute::ClassCursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxIBOutletCollectionAttrInfoHandle::Reference(m_handle)->classCursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexIBOutletCollectionAttribute::ClassEntity::get
//
// Gets the declaration entity

IndexEntity^ IndexIBOutletCollectionAttribute::ClassEntity::get(void)
{
	if(Object::ReferenceEquals(m_entity, nullptr))
		m_entity = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxIBOutletCollectionAttrInfoHandle::Reference(m_handle)->objcClass);

	return m_entity;
}

//---------------------------------------------------------------------------
// IndexIBOutletCollectionAttribute::ClassLocation::get
//
// Gets the declaration location

local::Location^ IndexIBOutletCollectionAttribute::ClassLocation::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, 
		clang_indexLoc_getCXSourceLocation(IdxIBOutletCollectionAttrInfoHandle::Reference(m_handle)->classLoc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexIBOutletCollectionAttribute::Create (static)
//
// Creates a new IndexIBOutletCollectionAttribute instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxIBOutletCollectionAttrInfo pointer

IndexIBOutletCollectionAttribute^ IndexIBOutletCollectionAttribute::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxIBOutletCollectionAttrInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexIBOutletCollectionAttribute(gcnew IdxIBOutletCollectionAttrInfoHandle(owner, action, info), 
		gcnew IdxAttrInfoHandle(owner, action, info->attrInfo));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
