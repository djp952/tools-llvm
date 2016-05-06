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
#include "IndexDeclaration.h"

#include "Cursor.h"
#include "IndexAttributeCollection.h"
#include "IndexContainer.h"
#include "IndexCxxClassDeclaration.h"
#include "IndexDeclarationFlags.h"
#include "IndexDeclarationKind.h"
#include "IndexEntity.h"
#include "IndexObjectiveCCategoryDeclaration.h"
#include "IndexObjectiveCContainerDeclaration.h"
#include "IndexObjectiveCInterfaceDeclaration.h"
#include "IndexObjectiveCPropertyDeclaration.h"
#include "IndexObjectiveCProtocolDeclaration.h"
#include "Location.h"
#include "LocationKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexDeclaration Constructor (protected)
//
// Arguments:
//
//	handle		- Reference handle instance
//	kind		- Kind of index declaration being constructed

IndexDeclaration::IndexDeclaration(IdxDeclInfoHandle^ handle, IndexDeclarationKind kind) : m_handle(handle), m_kind(kind)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexDeclaration::AsContainer
//
// Returns this declaration as an IndexContainer instance
//
// Arguments:
//
//	NONE

IndexContainer^ IndexDeclaration::AsContainer(void)
{
	return IndexContainer::Create(m_handle->Owner, m_handle->IndexAction, IdxDeclInfoHandle::Reference(m_handle)->declAsContainer);
}

//---------------------------------------------------------------------------
// IndexDeclaration::Attributes::get
//
// Gets a read-only collection of declaration attributes

IndexAttributeCollection^ IndexDeclaration::Attributes::get(void)
{
	if(Object::ReferenceEquals(m_attributes, nullptr)) {

		IdxDeclInfoHandle::Reference info(m_handle);
		m_attributes = IndexAttributeCollection::Create(m_handle->Owner, m_handle->IndexAction, info->attributes, info->numAttributes);
	}

	return m_attributes;
}

//---------------------------------------------------------------------------
// IndexDeclaration::Create (internal, static)
//
// Creates a new IndexDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the declaration information

IndexDeclaration^ IndexDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxDeclInfo* info)
{
	// Index declarations are polymorphic, but there is no key from the API to determine
	// what kind of index declaration this is.  Each kind has to be tried and inspected

	// CXIdxClassDeclInfo --> IndexCxxClassDeclaration
	//
	const CXIdxCXXClassDeclInfo* cxxclass = clang_index_getCXXClassDeclInfo(info);
	if(cxxclass != __nullptr) return IndexCxxClassDeclaration::Create(owner, action, cxxclass);

	// CXIdxObjCPropertyDeclInfo --> IndexObjectiveCPropertyDeclaration
	//
	const CXIdxObjCPropertyDeclInfo* objcprop = clang_index_getObjCPropertyDeclInfo(info);
	if(objcprop != __nullptr) return IndexObjectiveCPropertyDeclaration::Create(owner, action, objcprop);

	// CXIdxObjCCategoryDeclInfo --> IndexObjectiveCCategoryDeclaration
	//
	const CXIdxObjCCategoryDeclInfo* objccat = clang_index_getObjCCategoryDeclInfo(info);
	if(objccat != __nullptr) return IndexObjectiveCCategoryDeclaration::Create(owner, action, objccat);

	// CXIdxObjCInterfaceDeclInfo --> IndexObjectiveCInterfaceDeclaration
	//
	const CXIdxObjCInterfaceDeclInfo* objciface = clang_index_getObjCInterfaceDeclInfo(info);
	if(objciface != __nullptr) return IndexObjectiveCInterfaceDeclaration::Create(owner, action, objciface);

	// CXIdxObjCContainerDeclInfo --> IndexObjectiveCContainerDeclaration/IndexObjectiveCProtocolDeclaration
	//
	const CXIdxObjCContainerDeclInfo* objccont = clang_index_getObjCContainerDeclInfo(info);
	if(objccont != __nullptr) {
	
		// CXIdxObjCProtocolRefListInfo --> IndexObjectiveCProtocolDeclaration
		//
		const CXIdxObjCProtocolRefListInfo* objcprot = clang_index_getObjCProtocolRefListInfo(info);
		if(objcprot != __nullptr) return IndexObjectiveCProtocolDeclaration::Create(owner, action, objccont, objcprot);

		// CXIdxObjCContainerDeclInfo --> IndexObjectiveCContainerDeclaration
		//
		else return IndexObjectiveCContainerDeclaration::Create(owner, action, objccont);
	}

	// default --> IndexDeclaration
	//
	return gcnew IndexDeclaration(gcnew IdxDeclInfoHandle(owner, action, info), IndexDeclarationKind::None);
}

//---------------------------------------------------------------------------
// IndexDeclaration::Cursor::get
//
// Gets the declaration cursor

local::Cursor^ IndexDeclaration::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr))
		m_cursor = local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, IdxDeclInfoHandle::Reference(m_handle)->cursor);

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexDeclaration::Entity::get
//
// Gets the declaration entity

IndexEntity^ IndexDeclaration::Entity::get(void)
{
	if(Object::ReferenceEquals(m_entity, nullptr))
		m_entity = IndexEntity::Create(m_handle->Owner, m_handle->IndexAction, IdxDeclInfoHandle::Reference(m_handle)->entityInfo);

	return m_entity;
}

//---------------------------------------------------------------------------
// IndexDeclaration::Flags::get
//
// Gets the flags associated with this declaration

IndexDeclarationFlags IndexDeclaration::Flags::get(void)
{
	return IndexDeclarationFlags(IdxDeclInfoHandle::Reference(m_handle)->flags);
}

//---------------------------------------------------------------------------
// IndexDeclaration::IsContainer::get
//
// Gets a flag indicating if this declaration is a container

bool IndexDeclaration::IsContainer::get(void)
{
	return (IdxDeclInfoHandle::Reference(m_handle)->isContainer != 0);
}

//---------------------------------------------------------------------------
// IndexDeclaration::IsDefinition::get
//
// Gets a flag indicating if this declaration is a definition

bool IndexDeclaration::IsDefinition::get(void)
{
	return (IdxDeclInfoHandle::Reference(m_handle)->isDefinition != 0);
}

//---------------------------------------------------------------------------
// IndexDeclaration::IsImplicit::get
//
// Gets a flag indicating if the declaration exists in code or was created implicitly

bool IndexDeclaration::IsImplicit::get(void)
{
	return (IdxDeclInfoHandle::Reference(m_handle)->isImplicit != 0);
}

//---------------------------------------------------------------------------
// IndexDeclaration::IsRedeclaration::get
//
// Gets a flag indicating if this declaration is a redeclaration

bool IndexDeclaration::IsRedeclaration::get(void)
{
	return (IdxDeclInfoHandle::Reference(m_handle)->isRedeclaration != 0);
}

//---------------------------------------------------------------------------
// IndexDeclaration::Kind::get
//
// Gets the kind of declaration this instance represents

IndexDeclarationKind IndexDeclaration::Kind::get(void)
{
	return m_kind;
}

//---------------------------------------------------------------------------
// IndexDeclaration::LexicalContainer::get
//
// Gets the declaration lexical container

IndexContainer^ IndexDeclaration::LexicalContainer::get(void)
{
	if(Object::ReferenceEquals(m_lexcontainer, nullptr))
		m_lexcontainer = IndexContainer::Create(m_handle->Owner, m_handle->IndexAction, IdxDeclInfoHandle::Reference(m_handle)->lexicalContainer);

	return m_lexcontainer;
}

//---------------------------------------------------------------------------
// IndexDeclaration::Location::get
//
// Gets the declaration location

local::Location^ IndexDeclaration::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr)) m_location = local::Location::Create(m_handle->Owner, TranslationUnitHandle::Null, 
		clang_indexLoc_getCXSourceLocation(IdxDeclInfoHandle::Reference(m_handle)->loc), LocationKind::File);

	return m_location;
}

//---------------------------------------------------------------------------
// IndexDeclaration::SemanticContainer::get
//
// Gets the declaration semantic container

IndexContainer^ IndexDeclaration::SemanticContainer::get(void)
{
	if(Object::ReferenceEquals(m_semcontainer, nullptr))
		m_semcontainer = IndexContainer::Create(m_handle->Owner, m_handle->IndexAction, IdxDeclInfoHandle::Reference(m_handle)->semanticContainer);

	return m_semcontainer;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
