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
#include "IndexObjectiveCInterfaceDeclaration.h"

#include "IndexBaseClass.h"
#include "IndexDeclarationKind.h"
#include "IndexObjectiveCProtocolReferenceCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexObjectiveCInterfaceDeclaration Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	conthandle	- IdxObjCContainerDeclInfoHandle to pass to the base class
//	basehandle	- IdxDeclInfoHandle to pass to the base class

IndexObjectiveCInterfaceDeclaration::IndexObjectiveCInterfaceDeclaration(IdxObjCInterfaceDeclInfoHandle^ handle, IdxObjCContainerDeclInfoHandle^ conthandle, IdxDeclInfoHandle^ basehandle) 
	: m_handle(handle), IndexObjectiveCContainerDeclaration(conthandle, basehandle, IndexDeclarationKind::ObjectiveCInterface)
{
}

//---------------------------------------------------------------------------
// IndexObjectiveCInterfaceDeclaration::Create (static)
//
// Creates a new IndexObjectiveCInterfaceDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxObjCInterfaceDeclInfo pointer

IndexObjectiveCInterfaceDeclaration^ IndexObjectiveCInterfaceDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCInterfaceDeclInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexObjectiveCInterfaceDeclaration(gcnew IdxObjCInterfaceDeclInfoHandle(owner, action, info), 
		gcnew IdxObjCContainerDeclInfoHandle(owner, action, info->containerInfo), gcnew IdxDeclInfoHandle(owner, action, info->containerInfo->declInfo));
}

//---------------------------------------------------------------------------
// IndexObjectiveCInterfaceDeclaration::ProtocolReferences::get
//
// Gets a read-only collection of protocols referenced by this declaration

IndexObjectiveCProtocolReferenceCollection^ IndexObjectiveCInterfaceDeclaration::ProtocolReferences::get(void)
{
	if(Object::ReferenceEquals(m_protocols, nullptr))
		m_protocols = IndexObjectiveCProtocolReferenceCollection::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCInterfaceDeclInfoHandle::Reference(m_handle)->protocols);

	return m_protocols;
}

//---------------------------------------------------------------------------
// IndexObjectiveCInterfaceDeclaration::SuperClass::get
//
// Gets information about the interface super class

IndexBaseClass^ IndexObjectiveCInterfaceDeclaration::SuperClass::get(void)
{
	if(Object::ReferenceEquals(m_super, nullptr))
		m_super = IndexBaseClass::Create(m_handle->Owner, m_handle->IndexAction, IdxObjCInterfaceDeclInfoHandle::Reference(m_handle)->superInfo);

	return m_super;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
