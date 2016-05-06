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

#ifndef __TOKENCOLLECTION_H_
#define __TOKENCOLLECTION_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FUNCTION PROTOTYPES
//
void DisposeTokenSet(CXToken*);

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class Token;
ref class TranslationUnitHandle;

//---------------------------------------------------------------------------
// Class TokenCollection
//
// Implements an IReadOnlyList<> based enumerable collection of tokens
//---------------------------------------------------------------------------

public ref class TokenCollection : public IReadOnlyList<Token^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<Token^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property Token^ default[int] 
	{
		virtual Token^ get(int index);
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get();
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates and annotates new TokenCollection instance
	static TokenCollection^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXToken*&& tokens, unsigned int numtokens);

private:

	// TokenSetHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXToken*
	using TokenSetHandle = UnmanagedTypeSafeHandle<CXToken*, DisposeTokenSet>;

	// Instance Constructor
	//
	TokenCollection(TokenSetHandle^ handle, TranslationUnitHandle^ transunit, unsigned int count);

	// Destructor / Finalizer
	//
	~TokenCollection();
	!TokenCollection();

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;
		
	//-----------------------------------------------------------------------
	// Member Variables

	TokenSetHandle^			m_handle;			// Underlying safe handle
	TranslationUnitHandle^	m_transunit;		// Translation unit instance
	bool					m_disposed;			// Object disposal flag
	CXCursor*				m_annotations;		// Annotation cursors
	const unsigned int		m_count;			// Number of elements in the collection
	array<Token^>^			m_cache;			// Cached collection elements
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TOKENCOLLECTION_H_
