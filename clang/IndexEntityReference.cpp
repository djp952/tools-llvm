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
#include "IndexEntityReference.h"

#include "Cursor.h"
#include "IndexContainer.h"
#include "IndexEntity.h"
#include "IndexEntityReferenceKind.h"
#include "Location.h"
#include "LocationKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexEntityReference Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexEntityReference::IndexEntityReference(IdxEntityRefInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexEntityReference::Container::get
//
// Gets the lexical container

IndexContainer^ IndexEntityReference::Container::get(void)
{
	if(Object::ReferenceEquals(m_container, nullptr))
		m_container = IndexContainer::Create(m_handle->Owner, m_handle->IndexAction, IdxEntityRefInfoHandle::Reference(m_handle)->container);

	return m_container;
}

//---------------------------------------------------------------------------
// IndexEntityReference::Create (internal, static)
//
// Creates a new IndexEntityReference instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the entity reference information

IndexEntityReference^ IndexEntityReference::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxEntityRefInfo* info)
{
	return gcnew IndexEntityReference(gcnew IdxEntityRefInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexEntityReference::Cursor::get
//
// Gets the reference cursor

local::Cursor^ IndexEntityReference::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxEntityRefInfoHandle::Reference(m_handle)->cursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexEntityReference::Kind::get
//
// Gets the kind of the entity reference

IndexEntityReferenceKind IndexEntityReference::Kind::get(void)
{
	return IndexEntityReferenceKind(IdxEntityRefInfoHandle::Reference(m_handle)->kind);
}

//---------------------------------------------------------------------------
// IndexEntityReference::Location::get
//
// Gets the reference location

local::Location^ IndexEntityReference::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, clang_indexLoc_getCXSourceLocation(IdxEntityRefInfoHandle::Reference(m_handle)->loc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexEntityReference::ParentEntity::get
//
// Gets information about the parent entity

IndexEntity^ IndexEntityReference::ParentEntity::get(void)
{
	if(Object::ReferenceEquals(m_parent, nullptr))
		m_parent = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxEntityRefInfoHandle::Reference(m_handle)->parentEntity);

	return m_parent;
}

//---------------------------------------------------------------------------
// IndexEntityReference::ReferencedEntity::get
//
// Gets the entity being referenced

IndexEntity^ IndexEntityReference::ReferencedEntity::get(void)
{
	if(Object::ReferenceEquals(m_referenced, nullptr))
		m_referenced = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxEntityRefInfoHandle::Reference(m_handle)->referencedEntity);

	return m_referenced;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
