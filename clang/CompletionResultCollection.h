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

#ifndef __COMPLETIONRESULTCOLLECTION_H_
#define __COMPLETIONRESULTCOLLECTION_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
enum class	CompletionContext;
ref class	CompletionResult;
value class	CursorKind;
ref class	DiagnosticCollection;
ref class	TranslationUnitHandle;
ref class	UnifiedSymbolResolution;

//---------------------------------------------------------------------------
// Class CompletionResultCollection
//
// Represents the results from a code completion operation
//---------------------------------------------------------------------------

public ref class CompletionResultCollection : public IReadOnlyList<CompletionResult^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetContainerCursorKind
	//
	// Returns the cursor kind for the container for the code completion context
	CursorKind GetContainerCursorKind(void);
	CursorKind GetContainerCursorKind([OutAttribute()] bool% iscomplete);

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<CompletionResult^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property CompletionResult^ default[int] 
	{
		virtual CompletionResult^ get(int index);
	}

	// ContainerUnifiedSymbolResolution
	//
	// Gets the Unified Symbol Resultion (USR) string for the container
	property UnifiedSymbolResolution^ ContainerUnifiedSymbolResolution
	{
		UnifiedSymbolResolution^ get(void);
	}

	// Contexts
	//
	// Gets completions are appropriate for the context the given code completion
	property CompletionContext Contexts
	{
		CompletionContext get(void);
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get();
	}

	// Diagnostics
	//
	// Gets the collection of diagnostics for this completion result
	property DiagnosticCollection^ Diagnostics
	{
		DiagnosticCollection^ get(void);
	}

	// ObjectiveCSelector
	//
	// Gets the currently-entered selector for an Objective-C message send
	property String^ ObjectiveCSelector
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new CompletionResultCollection instance
	static CompletionResultCollection^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCodeCompleteResults*&& results);

private:

	// CodeCompleteResultsHandle
	//
	// Specialization of UnmanagedTypeSafeHandle for CXCodeCompleteResults*
	using CodeCompleteResultsHandle = UnmanagedTypeSafeHandle<CXCodeCompleteResults*, clang_disposeCodeCompleteResults>;

	// Instance Constructor
	//
	CompletionResultCollection(CodeCompleteResultsHandle^ handle, TranslationUnitHandle^ transunit);

	// Destructor
	//
	~CompletionResultCollection();

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;
		
	//-----------------------------------------------------------------------
	// Member Variables

	CodeCompleteResultsHandle^			m_handle;			// Underlying safe handle
	TranslationUnitHandle^				m_transunit;		// Translation unit instance
	bool								m_disposed;			// Object disposal flag
	array<CompletionResult^>^			m_cache;			// Element object cache
	UnifiedSymbolResolution^			m_usr;				// Cached container USR
	String^								m_objcselector;		// Cached obj-C selector
	DiagnosticCollection^				m_diags;			// Cached diagnostics
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __COMPLETIONRESULTCOLLECTION_H_
