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
#include "CompletionResultCollection.h"

#include "CompletionContext.h"
#include "CompletionResult.h"
#include "CompletionResultDiagnosticCollection.h"
#include "CursorKind.h"
#include "ReadOnlyListEnumerator.h"
#include "StringUtil.h"
#include "TranslationUnitHandle.h"
#include "UnifiedSymbolResolution.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompletionResultCollection Constructor (private)
//
// Arguments:
//
//	handle		- CodeCompleteResults safe handle
//	transunit	- Parent TranslationUnitHandle instance

CompletionResultCollection::CompletionResultCollection(CodeCompleteResultsHandle^ handle, TranslationUnitHandle^ transunit) : m_handle(handle), m_transunit(transunit)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");

	m_cache = gcnew array<CompletionResult^>(CodeCompleteResultsHandle::Reference(m_handle)->NumResults);
}

//---------------------------------------------------------------------------
// CompletionResultCollection Destructor

CompletionResultCollection::~CompletionResultCollection()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// CompletionResultCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

CompletionResult^ CompletionResultCollection::default::get(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Check for a cached instance of the CompletionResult first, this will also check
	// the boundaries of the collection and throw if necessary
	CompletionResult^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new CompletionResult and cache it to prevent multiple creations
	m_cache[index] = CompletionResult::Create(m_handle, CodeCompleteResultsHandle::Reference(m_handle)->Results[index]);
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CompletionResultCollection::ContainerUnifiedSymbolResolution::get
//
// Gets the Unified Symbol Resultion (USR) string for the container

UnifiedSymbolResolution^ CompletionResultCollection::ContainerUnifiedSymbolResolution::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_usr, nullptr))
		m_usr = UnifiedSymbolResolution::Create(clang_codeCompleteGetContainerUSR(CodeCompleteResultsHandle::Reference(m_handle)));

	return m_usr;
}

//---------------------------------------------------------------------------
// CompletionResultCollection::Contexts::get
//
// Gets completions are appropriate for the context the given code completion

CompletionContext CompletionResultCollection::Contexts::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return CompletionContext(clang_codeCompleteGetContexts(CodeCompleteResultsHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompletionResultCollection::Count::get
//
// Gets the number of elements in the collection

int CompletionResultCollection::Count::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CompletionResultCollection::Create (internal, static)
//
// Creates a new CompletionResultCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- Parent TranslationUnitHandle instance
//	results		- Unmanaged CXCodeCompleteResults instance

CompletionResultCollection^ CompletionResultCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCodeCompleteResults*&& results)
{
	return gcnew CompletionResultCollection(gcnew CodeCompleteResultsHandle(owner, std::move(results)), transunit);
}

//---------------------------------------------------------------------------
// CompletionResultCollection::Diagnostics::get
//
// Gets a collection of diagnostics for the code completion result

DiagnosticCollection^ CompletionResultCollection::Diagnostics::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_diags, nullptr)) 
		m_diags = CompletionResultDiagnosticCollection::Create(m_handle, m_transunit, CodeCompleteResultsHandle::Reference(m_handle));

	return m_diags;
}

//---------------------------------------------------------------------------
// Completion::GetContainerCursorKind
//
// Returns the cursor kind for the container for the code completion context
//
// Arguments:
//
//	NONE

CursorKind CompletionResultCollection::GetContainerCursorKind(void)
{
	bool iscomplete = false;			// Unused [out] argument

	CHECK_DISPOSED(m_disposed);
	return GetContainerCursorKind(iscomplete);
}

//---------------------------------------------------------------------------
// Completion::GetContainerCursorKind
//
// Returns the cursor kind for the container for the code completion context
//
// Arguments:
//
//	iscomplete	- Set to true if API has complete information about the container

CursorKind CompletionResultCollection::GetContainerCursorKind([OutAttribute()] bool% iscomplete)
{
	unsigned int isincomplete;			// Flag if the information is incomplete

	CHECK_DISPOSED(m_disposed);

	// Get the cursor kind and invert the isincomplete flag to a boolean iscomplete flag
	CursorKind result = CursorKind(clang_codeCompleteGetContainerKind(CodeCompleteResultsHandle::Reference(m_handle), &isincomplete));
	iscomplete = (isincomplete == 0);

	return result;
}

//---------------------------------------------------------------------------
// CompletionResultCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<CompletionResult^>^ CompletionResultCollection::GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return gcnew ReadOnlyListEnumerator<CompletionResult^>(this);
}

//---------------------------------------------------------------------------
// CompletionResultCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CompletionResultCollection::IEnumerable_GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return GetEnumerator();
}

//---------------------------------------------------------------------------
// CompletionResultCollection::ObjectiveCSelector::get
//
// Gets the currently-entered selector for an Objective-C message send

String^ CompletionResultCollection::ObjectiveCSelector::get(void)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(m_objcselector, nullptr))
		m_objcselector = StringUtil::ToString(clang_codeCompleteGetObjCSelector(CodeCompleteResultsHandle::Reference(m_handle)));

	return m_objcselector;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
