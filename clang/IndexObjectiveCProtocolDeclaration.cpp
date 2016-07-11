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
#include "IndexObjectiveCProtocolDeclaration.h"

#include "IndexDeclarationKind.h"
#include "IndexObjectiveCProtocolReferenceCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolDeclaration Constructor (internal)
//
// Arguments:
//
//	conthandle	- IdxObjCContainerDeclInfoHandle to pass to the base class
//	basehandle	- IdxDeclInfoHandle to pass to the base class
//	protocols	- Precreated IndexObjectiveCProtocolReferenceCollection instance

IndexObjectiveCProtocolDeclaration::IndexObjectiveCProtocolDeclaration(IdxObjCContainerDeclInfoHandle^ conthandle, IdxDeclInfoHandle^ basehandle, 
	IndexObjectiveCProtocolReferenceCollection^ protocols) : IndexObjectiveCContainerDeclaration(conthandle, basehandle, IndexDeclarationKind::ObjectiveCProtocol), m_protocols(protocols)
{
	if(Object::ReferenceEquals(protocols, nullptr)) throw gcnew ArgumentNullException("protocols");
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolDeclaration::Create (static)
//
// Creates a new IndexObjectiveCProtocolDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxObjCContainerDeclInfo pointer
//	listinfo	- Unmanaged CXIdxObjCProtocolRefListInfo pointer

IndexObjectiveCProtocolDeclaration^ IndexObjectiveCProtocolDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCContainerDeclInfo* info, const CXIdxObjCProtocolRefListInfo* listinfo)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");

	// This class requires the protocol collection be precreated since it relies upon the pointer
	return gcnew IndexObjectiveCProtocolDeclaration(gcnew IdxObjCContainerDeclInfoHandle(owner, action, info), 
		gcnew IdxDeclInfoHandle(owner, action, info->declInfo), IndexObjectiveCProtocolReferenceCollection::Create(owner, action, listinfo));
}

//---------------------------------------------------------------------------
// IndexObjectiveCProtocolDeclaration::ProtocolReferences::get
//
// Accesses the contained IndexObjectiveCProtocolCollection

IndexObjectiveCProtocolReferenceCollection^ IndexObjectiveCProtocolDeclaration::ProtocolReferences::get(void)
{
	return m_protocols;
}
	
//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
