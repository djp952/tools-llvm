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
#include "IndexEntity.h"

#include "Cursor.h"
#include "IndexAction.h"
#include "IndexAttributeCollection.h"
#include "IndexEntityCxxTemplateKind.h"
#include "IndexEntityKind.h"
#include "IndexEntityLanguage.h"
#include "StringUtil.h"
#include "UnifiedSymbolResolution.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexEntity Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexEntity::IndexEntity(IdxEntityInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexEntity::Create (internal, static)
//
// Creates a new IndexEntity instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- Owning index action handle
//	info		- Pointer to the Entity information

IndexEntity^ IndexEntity::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxEntityInfo* info)
{
	return gcnew IndexEntity(gcnew IdxEntityInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexEntity::Attributes::get
//
// Gets a read-only collection of attributes for this entity

IndexAttributeCollection^ IndexEntity::Attributes::get(void)
{
	if(Object::ReferenceEquals(m_attributes, nullptr)) {

		IdxEntityInfoHandle::Reference info(m_handle);

		// Create and cache a dummy zero-length attribute collection if the entity is null
		m_attributes = (info.IsNull) ? IndexAttributeCollection::Create(m_handle->Owner, m_handle->IndexAction, __nullptr, 0)
			: IndexAttributeCollection::Create(m_handle->Owner, m_handle->IndexAction, info->attributes, info->numAttributes);
	}

	return m_attributes;
}

//---------------------------------------------------------------------------
// IndexEntity::ClientData::get
//
// Gets a client object associated with this entity during indexing

Object^ IndexEntity::ClientData::get(void)
{
	IdxEntityInfoHandle::Reference info(m_handle);
	if(info.IsNull) return nullptr;

	// Get the object associated with this entity and return null if it hasn't been set yet
	CXIdxClientEntity object = clang_index_getClientEntity(info);
	if(object == __nullptr) return nullptr;

	return GCHandle::FromIntPtr(IntPtr(object)).Target;
}

//---------------------------------------------------------------------------
// IndexEntity::ClientData::set
//
// Associates a client object with this entity during indexing

void IndexEntity::ClientData::set(Object^ value)
{
	IdxEntityInfoHandle::Reference info(m_handle);
	if(info.IsNull) return;

	if(Object::ReferenceEquals(value, nullptr)) clang_index_setClientEntity(info, __nullptr);

	// The object has to be tracked somewhere to prevent garbage collection until this
	// index action has been completed -- ask the IndexAction to track it
	GCHandle gch = m_handle->IndexAction->TrackClientObject(value);
	clang_index_setClientEntity(info, GCHandle::ToIntPtr(gch).ToPointer());
}

//---------------------------------------------------------------------------
// IndexEntity::Cursor::get
//
// Gets the cursor associated with this entity

local::Cursor^ IndexEntity::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr)) {
		
		IdxEntityInfoHandle::Reference info(m_handle);
		m_cursor = (info.IsNull) ? local::Cursor::Null : local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, info->cursor);
	}

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexEntity::CxxTemplateKind::get
//
// Gets the entity C++ template kind, if any

IndexEntityCxxTemplateKind IndexEntity::CxxTemplateKind::get(void)
{
	IdxEntityInfoHandle::Reference info(m_handle);
	return (info.IsNull) ? IndexEntityCxxTemplateKind::NonTemplate : IndexEntityCxxTemplateKind(info->templateKind);
}

//---------------------------------------------------------------------------
// IndexEntity::IsNull (static)
//
// Determines if the entity is null
//
// Arguments:
//
//	rhs		- IndexEntity instance to be checked for null

bool IndexEntity::IsNull(IndexEntity^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (IdxEntityInfoHandle::Reference(rhs->m_handle).IsNull);
}

//---------------------------------------------------------------------------
// IndexEntity::Kind::get
//
// Get the entity kind

IndexEntityKind IndexEntity::Kind::get(void)
{
	IdxEntityInfoHandle::Reference info(m_handle);
	return (info.IsNull) ? IndexEntityKind::Unexposed : IndexEntityKind(info->kind);
}

//---------------------------------------------------------------------------
// IndexEntity::Language::get
//
// Gets the entity language

IndexEntityLanguage IndexEntity::Language::get(void)
{
	IdxEntityInfoHandle::Reference info(m_handle);
	return (info.IsNull) ? IndexEntityLanguage::None : IndexEntityLanguage(info->lang);
}

//---------------------------------------------------------------------------
// IndexEntity::Name::get
//
// Gets the name of the entity

String^ IndexEntity::Name::get(void)
{
	if(Object::ReferenceEquals(m_name, nullptr)) {
		
		IdxEntityInfoHandle::Reference info(m_handle);
		m_name = (info.IsNull) ? String::Empty : StringUtil::ToString(info->name, CP_UTF8);
	}

	return m_name;
}

//---------------------------------------------------------------------------
// IndexEntity::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ IndexEntity::ToString(void)
{
	return this->Name;
}

//---------------------------------------------------------------------------
// IndexEntity::UnifiedSymbolResolution::get
//
// Gets the Unified Symbol Resultion (USR) string for the entity

local::UnifiedSymbolResolution^ IndexEntity::UnifiedSymbolResolution::get(void)
{
	if(Object::ReferenceEquals(m_usr, nullptr)) {

		IdxEntityInfoHandle::Reference info(m_handle);
		m_usr = local::UnifiedSymbolResolution::Create((info.IsNull) ? __nullptr : info->USR);
	}

	return m_usr;
}
	
//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
