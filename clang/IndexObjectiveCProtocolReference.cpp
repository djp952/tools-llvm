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
#include "IndexObjectiveCProtocolReference.h"

#include "Cursor.h"
#include "IndexEntity.h"
#include "Location.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReference Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexObjectiveCProtocolReference::IndexObjectiveCProtocolReference(IdxObjCProtocolRefInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReference::Create (internal, static)
//
// Creates a new IndexObjectiveCProtocolReference instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the protocol information

IndexObjectiveCProtocolReference^ IndexObjectiveCProtocolReference::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCProtocolRefInfo* info)
{
	if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");
	if(info == __nullptr) throw gcnew ArgumentNullException("info");

	return gcnew IndexObjectiveCProtocolReference(gcnew IdxObjCProtocolRefInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReference::Cursor::get
//
// Gets the declaration cursor

local::Cursor^ IndexObjectiveCProtocolReference::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxObjCProtocolRefInfoHandle::Reference(m_handle)->cursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReference::Location::get
//
// Gets the declaration location

local::Location^ IndexObjectiveCProtocolReference::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, 
		clang_indexLoc_getCXSourceLocation(IdxObjCProtocolRefInfoHandle::Reference(m_handle)->loc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReference::ProtocolEntity::get
//
// Gets the declaration entity

IndexEntity^ IndexObjectiveCProtocolReference::ProtocolEntity::get(void)
{
	if(Object::ReferenceEquals(m_entity, nullptr))
		m_entity = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCProtocolRefInfoHandle::Reference(m_handle)->protocol);

	return m_entity;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
