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
#include "IndexObjectiveCCategoryDeclaration.h"

#include "Cursor.h"
#include "IndexDeclarationKind.h"
#include "IndexEntity.h"
#include "IndexObjectiveCProtocolReferenceCollection.h"
#include "Location.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexObjectiveCCategoryDeclaration Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	conthandle	- IdxObjCContainerDeclInfoHandle to pass to the base class
//	basehandle	- IdxDeclInfoHandle to pass to the base class

IndexObjectiveCCategoryDeclaration::IndexObjectiveCCategoryDeclaration(IdxObjCCategoryDeclInfoHandle^ handle, IdxObjCContainerDeclInfoHandle^ conthandle, IdxDeclInfoHandle^ basehandle) 
	: m_handle(handle), IndexObjectiveCContainerDeclaration(conthandle, basehandle, IndexDeclarationKind::ObjectiveCCategory)
{
}

//---------------------------------------------------------------------------
// IndexObjectiveCCategoryDeclaration::ClassCursor::get
//
// Gets the declaration cursor

local::Cursor^ IndexObjectiveCCategoryDeclaration::ClassCursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxObjCCategoryDeclInfoHandle::Reference(m_handle)->classCursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexObjectiveCCategoryDeclaration::ClassEntity::get
//
// Gets the declaration entity

IndexEntity^ IndexObjectiveCCategoryDeclaration::ClassEntity::get(void)
{
	if(Object::ReferenceEquals(m_entity, nullptr))
		m_entity = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCCategoryDeclInfoHandle::Reference(m_handle)->objcClass);

	return m_entity;
}

//---------------------------------------------------------------------------
// IndexObjectiveCCategoryDeclaration::ClassLocation::get
//
// Gets the declaration location

local::Location^ IndexObjectiveCCategoryDeclaration::ClassLocation::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, 
		clang_indexLoc_getCXSourceLocation(IdxObjCCategoryDeclInfoHandle::Reference(m_handle)->classLoc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexObjectiveCCategoryDeclaration::Create (static)
//
// Creates a new IndexObjectiveCCategoryDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxObjCCategoryDeclInfo pointer

IndexObjectiveCCategoryDeclaration^ IndexObjectiveCCategoryDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCCategoryDeclInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexObjectiveCCategoryDeclaration(gcnew IdxObjCCategoryDeclInfoHandle(owner, action, info), 
		gcnew IdxObjCContainerDeclInfoHandle(owner, action, info->containerInfo), gcnew IdxDeclInfoHandle(owner, action, info->containerInfo->declInfo));
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolReferenceCollection::ProtocolReferences::get
//
// Gets a read-only collection of protocols referenced by this declaration

IndexObjectiveCProtocolReferenceCollection^ IndexObjectiveCCategoryDeclaration::ProtocolReferences::get(void)
{
	if(Object::ReferenceEquals(m_protocols, nullptr))
		m_protocols = IndexObjectiveCProtocolReferenceCollection::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCCategoryDeclInfoHandle::Reference(m_handle)->protocols);

	return m_protocols;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
