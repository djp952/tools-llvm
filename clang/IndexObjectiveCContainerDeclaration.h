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

#ifndef __INDEXOBJECTIVECCONTAINERDECLARATION_H_
#define __INDEXOBJECTIVECCONTAINERDECLARATION_H_
#pragma once

#include "IndexActionPointerHandle.h"
#include "IndexDeclaration.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class	IndexAction;
enum class	IndexObjectiveCContainerKind;

//---------------------------------------------------------------------------
// Class IndexObjectiveCContainerDeclaration
//
// Represents information about an objective-C container declaration
//---------------------------------------------------------------------------

public ref class IndexObjectiveCContainerDeclaration : public IndexDeclaration
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// ContainerKind
	//
	// Gets the kind of Objective-C container represented by this declaration
	property IndexObjectiveCContainerKind ContainerKind
	{
		IndexObjectiveCContainerKind get(void);
	}

internal:

	// Create (static)
	//
	// Creates a new IndexObjectiveCContainerDeclaration instance
	static IndexObjectiveCContainerDeclaration^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCContainerDeclInfo* info);

protected private:

	// IdxObjCContainerDeclInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxObjCContainerDeclInfo*
	using IdxObjCContainerDeclInfoHandle = IndexActionPointerHandle<const CXIdxObjCContainerDeclInfo*>;

	// Instance Constructor
	//
	IndexObjectiveCContainerDeclaration(IdxObjCContainerDeclInfoHandle^ handle, IdxDeclInfoHandle^ basehandle, IndexDeclarationKind kind);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxObjCContainerDeclInfoHandle^		m_handle;		// Reference handle
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXOBJECTIVECCONTAINERDECLARATION_H_
