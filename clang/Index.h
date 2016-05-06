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

#ifndef __INDEX_H_
#define __INDEX_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	File;
ref class	IndexAbortEventArgs;
ref class	IndexDeclarationEventArgs;
ref class	IndexDiagnosticsEventArgs;
ref class	IndexDiagnosticCollection;
ref class	IndexDeclaration;
ref class	IndexEnteredMainFileEventArgs;
ref class	IndexEntityReference;
ref class	IndexEntityReferenceEventArgs;
ref class	IndexEventArgs;
enum class	IndexGlobalOptions;
ref class	IndexImportedASTFile;
ref class	IndexImportedASTFileEventArgs;
ref class	IndexIncludedFile;
ref class	IndexIncludedFileEventArgs;
enum class	IndexOptions;
ref class	TranslationUnit;
enum class	TranslationUnitParseOptions;
ref class	UnsavedFile;

//-----------------------------------------------------------------------
// Delegate Types
//---------------------------------------------------------------------------

// IndexAbortHandler
//
// Delegate signature for the Index::Abort event
public delegate void IndexAbortHandler(Object^ sender, IndexAbortEventArgs^ e);

// IndexDeclarationHandler
//
// Delegate signature for the Index::Declaration event
public delegate void IndexDeclarationHandler(Object^ sender, IndexDeclarationEventArgs^ e);

// IndexDiagnosticsHandler
//
// Delegate signature for the Index::Diagnostics event
public delegate void IndexDiagnosticsHandler(Object^ sender, IndexDiagnosticsEventArgs^ e);

// IndexEnteredMainFileHandler
//
// Delegate signature for the Index::EnteredMainFile event
public delegate void IndexEnteredMainFileHandler(Object^ sender, IndexEnteredMainFileEventArgs^ e);

// IndexEntityReferenceHandler
//
// Delegate signature for the Index::EntityReference event
public delegate void IndexEntityReferenceHandler(Object^ sender, IndexEntityReferenceEventArgs^ e);

// IndexImportedASTFileHandler
//
// Delegate signature for the Index::ImportedASTFile event
public delegate void IndexImportedASTFileHandler(Object^ sender, IndexImportedASTFileEventArgs^ e);

// IndexIncludedFileHandler
//
// Delegate signature for the Index::IncludedFile event
public delegate void IndexIncludedFileHandler(Object^ sender, IndexIncludedFileEventArgs^ e);

// Index::StartedTranslationUnit
//
// Delegate signature for the Index::StartedTranslationUnit event
public delegate void IndexStartedTranslationUnitHandler(Object^ sender, IndexEventArgs^ e);

//---------------------------------------------------------------------------
// Class Index
//
// Provides a shared context for creating translation units
//---------------------------------------------------------------------------

public ref class Index
{
public:

	//-----------------------------------------------------------------------
	// Events

	// Abort
	//
	// Invoked periodically during an indexing operation to check for an abort
	event IndexAbortHandler^ Abort
	{
		public:		void add(IndexAbortHandler^ handler);
		internal:	void raise(Object^ sender, IndexAbortEventArgs^ args);
		public:		void remove(IndexAbortHandler^ handler);
	}

	// Declaration
	//
	// Called when an index declaration is processed
	event IndexDeclarationHandler^ Declaration
	{
		public:		void add(IndexDeclarationHandler^ handler);
		internal:	void raise(Object^ sender, IndexDeclarationEventArgs^ args);
		public:		void remove(IndexDeclarationHandler^ handler);
	}

	// Diagnostics
	//
	// Called at the end of indexing; passes the complete diagnostic set
	event IndexDiagnosticsHandler^ Diagnostics
	{
		public:		void add(IndexDiagnosticsHandler^ handler);
		internal:	void raise(Object^ sender, IndexDiagnosticsEventArgs^ args);
		public:		void remove(IndexDiagnosticsHandler^ handler);
	}

	// EnteredMainFile
	//
	// Called when the indexing operation enters the main file
	event IndexEnteredMainFileHandler^ EnteredMainFile
	{
		public:		void add(IndexEnteredMainFileHandler^ handler);
		internal:	void raise(Object^ sender, IndexEnteredMainFileEventArgs^ args);
		public:		void remove(IndexEnteredMainFileHandler^ handler);
	}

	// EntityReference
	//
	// Called to index a reference of an entity
	event IndexEntityReferenceHandler^ EntityReference
	{
		public:		void add(IndexEntityReferenceHandler^ handler);
		internal:	void raise(Object^ sender, IndexEntityReferenceEventArgs^ args);
		public:		void remove(IndexEntityReferenceHandler^ handler);
	}

	// ImportedASTFile
	//
	// Called when an AST file (PCH or module) gets imported
	event IndexImportedASTFileHandler^ ImportedASTFile
	{
		public:		void add(IndexImportedASTFileHandler^ handler);
		internal:	void raise(Object^ sender, IndexImportedASTFileEventArgs^ args);
		public:		void remove(IndexImportedASTFileHandler^ handler);
	}

	// IncludedFile
	//
	// Called when a file gets #included/#imported
	event IndexIncludedFileHandler^ IncludedFile
	{
		public:		void add(IndexIncludedFileHandler^ handler);
		internal:	void raise(Object^ sender, IndexIncludedFileEventArgs^ args);
		public:		void remove(IndexIncludedFileHandler^ handler);
	}

	// StartedTranslationUnit
	//
	// Called at the beginning of indexing a translation unit
	event IndexStartedTranslationUnitHandler^ StartedTranslationUnit
	{
		public:		void add(IndexStartedTranslationUnitHandler^ handler);
		internal:	void raise(Object^ sender, IndexEventArgs^ args);
		public:		void remove(IndexStartedTranslationUnitHandler^ handler);
	}

	//-----------------------------------------------------------------------
	// Member Functions

	// CreateTranslationUnit
	//
	// Create a TranslationUnit by parsing a source code file
	TranslationUnit^ CreateTranslationUnit(String^ filename);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles);
	TranslationUnit^ CreateTranslationUnit(String^ filename, TranslationUnitParseOptions options);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, TranslationUnitParseOptions options);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options);
	TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options);

	// IndexSourceFile
	//
	// Index the specified source file using any registered event handlers
	void IndexSourceFile(String^ filename);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args);
	void IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles);
	void IndexSourceFile(String^ filename, IndexOptions options);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IndexOptions options);
	void IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options);
	void IndexSourceFile(String^ filename, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, Object^ state);
	void IndexSourceFile(String^ filename, IndexOptions options, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IndexOptions options, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state);
	void IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state);

	// IndexTranslationUnit
	//
	// Index the translation unit using any registered event handlers
	void IndexTranslationUnit(TranslationUnit^ transunit);
	void IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options);
	void IndexTranslationUnit(TranslationUnit^ transunit, Object^ state);
	void IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options, Object^ state);

	// LoadTranslationUnit
	//
	// Loads a translation unit from a serialized abstract syntax tree
	TranslationUnit^ LoadTranslationUnit(String^ path);

	//-----------------------------------------------------------------------
	// Properties

	// GlobalOptions
	//
	// Gets/sets the general index option flags
	property IndexGlobalOptions GlobalOptions
	{
		IndexGlobalOptions get(void);
		void set(IndexGlobalOptions value);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new Index instance
	static Index^ Create(CXIndex&& index);

private:

	// IndexHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXIndex
	using IndexHandle = UnmanagedTypeSafeHandle<CXIndex, clang_disposeIndex>;

	// Instance Constructor
	//
	Index(IndexHandle^ handle);

	// Destructor
	//
	~Index();

	//-----------------------------------------------------------------------
	// Member Variables

	bool								m_disposed;				// Object disposal flag
	IndexHandle^						m_handle;				// Underlying safe handle

	// Event Handler Delegates
	//
	IndexAbortHandler^					m_abort;				// Abort delegate
	IndexDiagnosticsHandler^			m_diagnostics;			// Diagnostics delegate
	IndexEnteredMainFileHandler^		m_enteredmainfile;		// EnteredMainFile delegate
	IndexImportedASTFileHandler^		m_importedastfile;		// ImportedASTFile delegate
	IndexIncludedFileHandler^			m_includedfile;			// IncludedFile delegate
	IndexDeclarationHandler^			m_declaration;			// Declaration delegate
	IndexEntityReferenceHandler^		m_entityreference;		// EntityReference delegates
	IndexStartedTranslationUnitHandler^	m_startedtransunit;		// StartedTranslationUnit delegate
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEX_H_
