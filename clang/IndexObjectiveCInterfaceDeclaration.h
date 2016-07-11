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

#ifndef __INDEXOBJECTIVECINTERFACEDECLARATION_H_
#define __INDEXOBJECTIVECINTERFACEDECLARATION_H_
#pragma once

#include "IndexActionPointerHandle.h"
#include "IndexObjectiveCContainerDeclaration.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class IndexAction;
ref class IndexBaseClass;
ref class IndexObjectiveCProtocolReferenceCollection;

//---------------------------------------------------------------------------
// Class IndexObjectiveCInterfaceDeclaration
//
// Represents information about an objective-C Interface declaration
//---------------------------------------------------------------------------

public ref class IndexObjectiveCInterfaceDeclaration sealed : public IndexObjectiveCContainerDeclaration
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// ProtocolReferences
	//
	// Gets a read-only collection of protocols referenced by this declaration
	property IndexObjectiveCProtocolReferenceCollection^ ProtocolReferences
	{
		IndexObjectiveCProtocolReferenceCollection^ get(void);
	}

	// SuperClass
	//
	// Gets information about the interface super class
	property IndexBaseClass^ SuperClass
	{
		IndexBaseClass^ get(void);
	}

internal:

	// Create (static)
	//
	// Creates a new IndexObjectiveCInterfaceDeclaration instance
	static IndexObjectiveCInterfaceDeclaration^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCInterfaceDeclInfo* info);

private:

	// IdxObjCInterfaceDeclInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxObjCInterfaceDeclInfo*
	using IdxObjCInterfaceDeclInfoHandle = IndexActionPointerHandle<const CXIdxObjCInterfaceDeclInfo*>;

	// Instance Constructor
	//
	IndexObjectiveCInterfaceDeclaration(IdxObjCInterfaceDeclInfoHandle^ handle, IdxObjCContainerDeclInfoHandle^ conthandle, IdxDeclInfoHandle^ basehandle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxObjCInterfaceDeclInfoHandle^		m_handle;		// Reference handle
	IndexBaseClass^						m_super;		// Cached superclass

	IndexObjectiveCProtocolReferenceCollection^		m_protocols;
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXOBJECTIVECINTERFACEDECLARATION_H_
