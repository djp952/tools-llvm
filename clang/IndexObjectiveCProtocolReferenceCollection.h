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

#ifndef __INDEXOBJECTIVECPROTOCOLREFERENCECOLLECTION_H_
#define __INDEXOBJECTIVECPROTOCOLREFERENCECOLLECTION_H_
#pragma once

#include "IndexActionPointerHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class IndexAction;
ref class IndexObjectiveCProtocolReference;

//---------------------------------------------------------------------------
// Class IndexObjectiveCProtocolReferenceCollection
//
// Implements a read-only collection of IndexObjectiveCProtocolReference objects
//---------------------------------------------------------------------------

public ref class IndexObjectiveCProtocolReferenceCollection : public IReadOnlyList<IndexObjectiveCProtocolReference^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<IndexObjectiveCProtocolReference^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property IndexObjectiveCProtocolReference^ default[int] 
	{
		virtual IndexObjectiveCProtocolReference^ get(int index);
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get();
	}

internal:

	// Create (static)
	//
	// Creates a new IndexObjectiveCProtocolReferenceCollection instance
	static IndexObjectiveCProtocolReferenceCollection^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxObjCProtocolRefListInfo* listinfo);

private:

	// IdxObjCProtocolRefListInfoHandle
	//
	// IndexActionPointerHandle specialization for CXIdxObjCProtocolRefListInfo*
	using IdxObjCProtocolRefListInfoHandle = IndexActionPointerHandle<const CXIdxObjCProtocolRefListInfo*>;

	// Instance Constructor
	//
	IndexObjectiveCProtocolReferenceCollection(IdxObjCProtocolRefListInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;

	//-----------------------------------------------------------------------
	// Member Variables

	IdxObjCProtocolRefListInfoHandle^			m_handle;	// Reference handle
	array<IndexObjectiveCProtocolReference^>^	m_cache;	// Element cache
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXOBJECTIVECPROTOCOLREFERENCECOLLECTION_H_
