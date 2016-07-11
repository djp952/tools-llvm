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

#ifndef __INDEXACTION_H_
#define __INDEXACTION_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {

// FUNCTION PROTOTYPES
//
void DisposeIndexActionCallback(nullptr_t);

// FORWARD DECLARATIONS
//
ref class	Index;
enum class	IndexOptions;
ref class	TranslationUnit;
ref class	UnsavedFile;

//---------------------------------------------------------------------------
// Class IndexAction (internal)
//
// Represents an indexing action/session, to be applied to one or multiple
// translation units
//---------------------------------------------------------------------------

ref class IndexAction
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// IndexSourceFile
	//
	// Index the specified source file using any registered event handlers
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state);

	// IndexTranslationUnit
	//
	// Index the translation unit using any registered event handlers
	void IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options, Object^ state);

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexAction instance
	static IndexAction^ Create(Index^ index, SafeHandle^ owner, CXIndexAction&& action);

	// TrackClientObject
	//
	// Adds a GCHandle against a client object to keep it alive
	GCHandle TrackClientObject(Object^ object);

private:

	// IndexerCallback Function Pointers
	//
	using OnAbortQueryPointer				= int(__cdecl*)(CXClientData, void*);
	using OnDiagnosticsPointer				= void(__cdecl*)(CXClientData, CXDiagnosticSet, void*);
	using OnEnteredMainFilePointer			= CXIdxClientFile(__cdecl*)(CXClientData, CXFile, void*);
	using OnImportedASTFilePointer			= CXIdxClientASTFile(__cdecl*)(CXClientData, const CXIdxImportedASTFileInfo*);
	using OnIncludedFilePointer				= CXIdxClientFile(__cdecl*)(CXClientData, const CXIdxIncludedFileInfo*);
	using OnIndexDeclarationPointer			= void(__cdecl*)(CXClientData, const CXIdxDeclInfo*);
	using OnIndexEntityReferencePointer		= void(__cdecl*)(CXClientData, const CXIdxEntityRefInfo*);
	using OnStartedTranslationUnitPointer	= CXIdxClientContainer(__cdecl*)(CXClientData, void*);

	// Managed IndexerCallback Delegates
	//
	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate int OnAbortQueryDelegate(CXClientData, void*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate void OnDiagnosticsDelegate(CXClientData, CXDiagnosticSet, void*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate CXIdxClientFile OnEnteredMainFileDelegate(CXClientData, CXFile, void*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate CXIdxClientASTFile OnImportedASTFileDelegate(CXClientData, const CXIdxImportedASTFileInfo*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate CXIdxClientFile OnIncludedFileDelegate(CXClientData, const CXIdxIncludedFileInfo*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate void OnIndexDeclarationDelegate(CXClientData, const CXIdxDeclInfo*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate void OnIndexEntityReferenceDelegate(CXClientData, const CXIdxEntityRefInfo*);

	[UnmanagedFunctionPointer(CallingConvention::Cdecl)]
	delegate CXIdxClientContainer OnStartedTranslationUnitDelegate(CXClientData, void*);

	// IndexActionHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXIndexAction
	using IndexActionHandle = UnmanagedTypeSafeHandle<CXIndexAction, clang_IndexAction_dispose>;

	// IndexActionCallbackHandle
	//
	// UnmanagedTypeSafeHandle specialization for a dummy safe handle used with callbacks
	// to provide lifetime control of the objects created during the callback
	using IndexActionCallbackHandle = UnmanagedTypeSafeHandle<nullptr_t, DisposeIndexActionCallback>;

	// Instance Constructor
	//
	IndexAction(Index^ index, IndexActionHandle^ handle);

	// Destructor
	//
	~IndexAction();

	//-----------------------------------------------------------------------
	// Private Member Functions

	// OnAbortQuery
	//
	// Invoked periodically to check whether indexing should be aborted
	int OnAbortQuery(CXClientData context, void* reserved);
	
	// OnDiagnostics
	//
	// Called at the end of indexing; passes the complete diagnostic set
	void OnDiagnostics(CXClientData context, CXDiagnosticSet diagnostics, void* reserved);
	
	// OnEnteredMainFile
	//
	// Called when the indexing operation enters the main file
	CXIdxClientFile OnEnteredMainFile(CXClientData context, CXFile mainfile, void* reserved);
	
	// OnImportedASTFile
	//
	// Called when an AST file (PCH or module) gets imported
	CXIdxClientASTFile OnImportedASTFile(CXClientData context, const CXIdxImportedASTFileInfo* info);
	
	// OnIncludedFile
	//
	// Called when a file gets #included/#imported
	CXIdxClientFile OnIncludedFile(CXClientData context, const CXIdxIncludedFileInfo* info);
	
	// OnIndexDeclarationCallback
	//
	// Called to index a declaration
	void OnIndexDeclaration(CXClientData context, const CXIdxDeclInfo* info);
	
	// OnIndexEntityReferenceCallback
	//
	// Called to index a reference of an entity
	void OnIndexEntityReference(CXClientData context, const CXIdxEntityRefInfo* info);
	
	// OnStartedTranslationUnitCallback
	//
	// Called at the beginning of indexing a translation unit
	CXIdxClientContainer OnStartedTranslationUnit(CXClientData context, void* reserved);

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;		// Object disposal flag
	IndexActionHandle^			m_handle;		// IndexAction safe handle
	Index^						m_index;		// Parent index instance
	List<GCHandle>^				m_clientobjs;	// Collection of client objects

	// Callback Delegates
	//
	OnAbortQueryDelegate^				m_onabort;				// IndexerCallbacks::abortQuery
	OnDiagnosticsDelegate^				m_ondiagnostics;		// IndexerCallbacks::diagnostic
	OnEnteredMainFileDelegate^			m_onenteredmainfile;	// IndexerCallbacks::enteredMainFile
	OnImportedASTFileDelegate^			m_onimportedastfile;	// IndexerCallbacks::importedASTFile
	OnIncludedFileDelegate^				m_onincludedfile;		// IndexerCallbacks::ppIncludedFile
	OnIndexDeclarationDelegate^			m_ondeclaration;		// IndexerCallbacks::indexDeclaration
	OnIndexEntityReferenceDelegate^		m_onentityreference;	// IndexerCallbacks::indexEntityReference
	OnStartedTranslationUnitDelegate^	m_onstartedtransunit;	// IndexerCallbacks::startedTranslationUnit
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXACTION_H_
