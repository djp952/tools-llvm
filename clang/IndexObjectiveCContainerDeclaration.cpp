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
#include "IndexObjectiveCContainerDeclaration.h"

#include "IndexDeclarationKind.h"
#include "IndexObjectiveCContainerKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexObjectiveCContainerDeclaration Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	basehandle	- IdxDeclInfoHandle to pass to the base class
//	kind		- IndexDeclarationKind to pass to the base class

IndexObjectiveCContainerDeclaration::IndexObjectiveCContainerDeclaration(IdxObjCContainerDeclInfoHandle^ handle, IdxDeclInfoHandle^ basehandle, IndexDeclarationKind kind) 
	: m_handle(handle), IndexDeclaration(basehandle, kind)
{
}

//---------------------------------------------------------------------------
// IndexObjectiveCContainerDeclaration::Create (static)
//
// Creates a new IndexObjectiveCContainerDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxObjCContainerDeclInfo pointer

IndexObjectiveCContainerDeclaration^ IndexObjectiveCContainerDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCContainerDeclInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexObjectiveCContainerDeclaration(gcnew IdxObjCContainerDeclInfoHandle(owner, action, info), 
		gcnew IdxDeclInfoHandle(owner, action, info->declInfo), IndexDeclarationKind::ObjectiveCContainer);
}

//---------------------------------------------------------------------------
// IndexObjectiveCContainerDeclaration::ContainerKind
//
// Gets the kind of Objective-C container represented by this declaration

IndexObjectiveCContainerKind IndexObjectiveCContainerDeclaration::ContainerKind::get(void)
{
	return IndexObjectiveCContainerKind(IdxObjCContainerDeclInfoHandle::Reference(m_handle)->kind);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
