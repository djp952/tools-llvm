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
#include "IndexBaseClass.h"

#include "Cursor.h"
#include "IndexEntity.h"
#include "Location.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexBaseClass Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexBaseClass::IndexBaseClass(IdxBaseClassInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexBaseClass::BaseEntity::get
//
// Gets the declaration entity

IndexEntity^ IndexBaseClass::BaseEntity::get(void)
{
	if(Object::ReferenceEquals(m_entity, nullptr))
		m_entity = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxBaseClassInfoHandle::Reference(m_handle)->base);

	return m_entity;
}

//---------------------------------------------------------------------------
// IndexBaseClass::Create (internal, static)
//
// Creates a new IndexBaseClass instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the base class information

IndexBaseClass^ IndexBaseClass::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxBaseClassInfo* info)
{
	return gcnew IndexBaseClass(gcnew IdxBaseClassInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexBaseClass::Cursor::get
//
// Gets the declaration cursor

local::Cursor^ IndexBaseClass::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr)) {

		IdxBaseClassInfoHandle::Reference info(m_handle);
		m_cursor = (info.IsNull) ? local::Cursor::Null : local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, info->cursor);
	}

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexBaseClass::IsNull (static)
//
// Determines if the base class is null
//
// Arguments:
//
//	rhs		- IndexBaseClass instance to be checked for null

bool IndexBaseClass::IsNull(IndexBaseClass^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (IdxBaseClassInfoHandle::Reference(rhs->m_handle).IsNull);
}

//---------------------------------------------------------------------------
// IndexBaseClass::Location::get
//
// Gets the declaration location

local::Location^ IndexBaseClass::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) {
		
		IdxBaseClassInfoHandle::Reference info(m_handle);
		m_location = (info.IsNull) ? local::Location::Null : local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null,
			clang_indexLoc_getCXSourceLocation(info->loc), LocationKind::File);
	}

	return m_location;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
