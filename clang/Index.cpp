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
#include "Index.h"

#include "ClangException.h"
#include "File.h"
#include "IndexAction.h"
#include "IndexDeclaration.h"
#include "IndexDiagnosticCollection.h"
#include "IndexEntityReference.h"
#include "IndexEventArgs.h"
#include "IndexGlobalOptions.h"
#include "IndexImportedASTFile.h"
#include "IndexIncludedFile.h"
#include "IndexOptions.h"
#include "StringUtil.h"
#include "TranslationUnit.h"
#include "TranslationUnitParseOptions.h"
#include "UnsavedFile.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Index Constructor (private)
//
// Arguments:
//
//	handle			- Underlying IndexHandle instance

Index::Index(IndexHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Index Destructor

Index::~Index()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// Index::Abort::add
//
// Adds a handler to the Abort event

void Index::Abort::add(IndexAbortHandler^ handler)
{
	m_abort = safe_cast<IndexAbortHandler^>(Delegate::Combine(m_abort, handler));
}

//---------------------------------------------------------------------------
// Index::Abort::raise
//
// Raises the Abort event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::Abort::raise(Object^ sender, IndexAbortEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_abort, nullptr)) m_abort(sender, args);
}

//---------------------------------------------------------------------------
// Index::Abort::remove
//
// Removes a handler from the Abort event

void Index::Abort::remove(IndexAbortHandler^ handler)
{
	m_abort = safe_cast<IndexAbortHandler^>(Delegate::Remove(m_abort, handler));
}

//---------------------------------------------------------------------------
// Index::Create (internal, static)
//
// Creates a new Index instance
//
// Arguments:
//
//	index		- Unmanaged CXIndex instance to take ownership of

Index^ Index::Create(CXIndex&& index)
{
	return gcnew Index(gcnew IndexHandle(std::move(index)));
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)

TranslationUnit^ Index::CreateTranslationUnit(String^ path)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, nullptr, nullptr, static_cast<TranslationUnitParseOptions>(-1));
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, args, nullptr, static_cast<TranslationUnitParseOptions>(-1));
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	unsavedfiles	- Collection of virtualc source file data

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, nullptr, unsavedfiles, static_cast<TranslationUnitParseOptions>(-1));
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Index::CreateTranslationUnit(String^ path, TranslationUnitParseOptions options)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, nullptr, nullptr, options);
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, args, unsavedfiles, static_cast<TranslationUnitParseOptions>(-1));
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, TranslationUnitParseOptions options)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, args, nullptr, options);
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options)
{
	CHECK_DISPOSED(m_disposed);
	return CreateTranslationUnit(path, nullptr, unsavedfiles, options);
}

//---------------------------------------------------------------------------
// Index::CreateTranslationUnit
//
// Create a TranslationUnit by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Index::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options)
{
	CXTranslationUnit			tu;						// Resultant translation unit structure
	int							numargs = 0;			// Number of argument strings
	int							numunsaved = 0;			// Number of UnsavedFile object instances
	bool						fullcmdline = false;	// Flag if args contains a full command line

	CHECK_DISPOSED(m_disposed);

	// If the special -1 option was specified, ask libclang to provide a default options mask
	if(options == static_cast<TranslationUnitParseOptions>(-1)) 
		options = static_cast<TranslationUnitParseOptions>(clang_defaultEditingTranslationUnitOptions());

	// The custom TranslationUnitParseOptions::ArgumentsAreFullCommandLine has to be checked and
	// removed from the flags before they are passed into clang -- this indicates a different method call
	else if((options & TranslationUnitParseOptions::ArgumentsAreFullCommandLine) == TranslationUnitParseOptions::ArgumentsAreFullCommandLine) {

		fullcmdline = true;
		options = TranslationUnitParseOptions(options & ~TranslationUnitParseOptions::ArgumentsAreFullCommandLine);
	}

	// Convert the managed path string into a standard C-style string (NULL is OK here)
	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try { 
		
		// Convert the enumerable range of string arguments into an unmanaged array and check
		// that there is at least one argument specified if necessary for argv[0]
		char** rgszargs = StringUtil::ToCharPointerArray(args, CP_UTF8, &numargs);
		if((fullcmdline) && (numargs == 0)) throw gcnew ArgumentNullException("args");

		try { 
			
			// Convert the enumerable range of UnsavedFile objects into an unmanaged array
			CXUnsavedFile* rgunsaved = UnsavedFile::UnsavedFilesToArray(unsavedfiles, &numunsaved);

			try {

				// Create the translation unit by parsing the specified file/unsaved file
				CXErrorCode result = (fullcmdline) ?
					clang_parseTranslationUnit2FullArgv(IndexHandle::Reference(m_handle), pszpath, rgszargs, numargs, rgunsaved, numunsaved, static_cast<unsigned int>(options), &tu) :
					clang_parseTranslationUnit2(IndexHandle::Reference(m_handle), pszpath, rgszargs, numargs, rgunsaved, numunsaved, static_cast<unsigned int>(options), &tu);
				if(result != CXError_Success) throw gcnew ClangException(result);

				// Pass ownership of the resultant translation unit to TranslationUnit
				return TranslationUnit::Create(m_handle, std::move(tu));

			} finally { UnsavedFile::FreeUnsavedFilesArray(rgunsaved, numunsaved); }

		} finally { StringUtil::FreeCharPointerArray(rgszargs);  }

	} finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// Index::Declaration::add
//
// Adds a handler to the Declaration event

void Index::Declaration::add(IndexDeclarationHandler^ handler)
{
	m_declaration = safe_cast<IndexDeclarationHandler^>(Delegate::Combine(m_declaration, handler));
}

//---------------------------------------------------------------------------
// Index::Declaration::raise
//
// Raises the Declaration event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::Declaration::raise(Object^ sender, IndexDeclarationEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_declaration, nullptr)) m_declaration(sender, args);
}

//---------------------------------------------------------------------------
// Index::Declaration::remove
//
// Removes a handler from the Declaration event

void Index::Declaration::remove(IndexDeclarationHandler^ handler)
{
	m_declaration = safe_cast<IndexDeclarationHandler^>(Delegate::Remove(m_declaration, handler));
}

//---------------------------------------------------------------------------
// Index::Diagnostics::add
//
// Adds a handler to the Diagnostics event

void Index::Diagnostics::add(IndexDiagnosticsHandler^ handler)
{
	m_diagnostics = safe_cast<IndexDiagnosticsHandler^>(Delegate::Combine(m_diagnostics, handler));
}

//---------------------------------------------------------------------------
// Index::Diagnostics::raise
//
// Raises the Diagnostics event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::Diagnostics::raise(Object^ sender, IndexDiagnosticsEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_diagnostics, nullptr)) m_diagnostics(sender, args);
}

//---------------------------------------------------------------------------
// Index::Diagnostics::remove
//
// Removes a handler from the Diagnostics event

void Index::Diagnostics::remove(IndexDiagnosticsHandler^ handler)
{
	m_diagnostics = safe_cast<IndexDiagnosticsHandler^>(Delegate::Remove(m_diagnostics, handler));
}

//---------------------------------------------------------------------------
// Index::EnteredMainFile::add
//
// Adds a handler to the EnteredMainFile event

void Index::EnteredMainFile::add(IndexEnteredMainFileHandler^ handler)
{
	m_enteredmainfile = safe_cast<IndexEnteredMainFileHandler^>(Delegate::Combine(m_enteredmainfile, handler));
}

//---------------------------------------------------------------------------
// Index::EnteredMainFile::raise
//
// Raises the EnteredMainFile event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::EnteredMainFile::raise(Object^ sender, IndexEnteredMainFileEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_enteredmainfile, nullptr)) m_enteredmainfile(sender, args);
}

//---------------------------------------------------------------------------
// Index::EnteredMainFile::remove
//
// Removes a handler from the EnteredMainFile event

void Index::EnteredMainFile::remove(IndexEnteredMainFileHandler^ handler)
{
	m_enteredmainfile = safe_cast<IndexEnteredMainFileHandler^>(Delegate::Remove(m_enteredmainfile, handler));
}

//---------------------------------------------------------------------------
// Index::EntityReference::add
//
// Adds a handler to the EntityReference event

void Index::EntityReference::add(IndexEntityReferenceHandler^ handler)
{
	m_entityreference = safe_cast<IndexEntityReferenceHandler^>(Delegate::Combine(m_entityreference, handler));
}

//---------------------------------------------------------------------------
// Index::Diagnostic::raise
//
// Raises the Diagnostic event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::EntityReference::raise(Object^ sender, IndexEntityReferenceEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_entityreference, nullptr)) m_entityreference(sender, args);
}

//---------------------------------------------------------------------------
// Index::EntityReference::remove
//
// Removes a handler from the Diagnostic event

void Index::EntityReference::remove(IndexEntityReferenceHandler^ handler)
{
	m_entityreference = safe_cast<IndexEntityReferenceHandler^>(Delegate::Remove(m_entityreference, handler));
}

//---------------------------------------------------------------------------
// Index::GlobalOptions::get
//
// Retrieves the current set of general index options

IndexGlobalOptions Index::GlobalOptions::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return static_cast<IndexGlobalOptions>(clang_CXIndex_getGlobalOptions(IndexHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Index::GlobalOptions::set
//
// Sets the current set of general index options

void Index::GlobalOptions::set(IndexGlobalOptions value)
{
	CHECK_DISPOSED(m_disposed);
	clang_CXIndex_setGlobalOptions(IndexHandle::Reference(m_handle), static_cast<unsigned int>(value));
}

//---------------------------------------------------------------------------
// Index::ImportedASTFile::add
//
// Adds a handler to the ImportedASTFile event

void Index::ImportedASTFile::add(IndexImportedASTFileHandler^ handler)
{
	m_importedastfile = safe_cast<IndexImportedASTFileHandler^>(Delegate::Combine(m_importedastfile, handler));
}

//---------------------------------------------------------------------------
// Index::ImportedASTFile::raise
//
// Raises the ImportedASTFile event
//
// Arguments:
//
//	sender			- Object raising the event
//	args		- Event arguments instance

void Index::ImportedASTFile::raise(Object^ sender, IndexImportedASTFileEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_importedastfile, nullptr)) m_importedastfile(sender, args);
}

//---------------------------------------------------------------------------
// Index::ImportedASTFile::remove
//
// Removes a handler from the ImportedASTFile event

void Index::ImportedASTFile::remove(IndexImportedASTFileHandler^ handler)
{
	m_importedastfile = safe_cast<IndexImportedASTFileHandler^>(Delegate::Remove(m_importedastfile, handler));
}

//---------------------------------------------------------------------------
// Index::IncludedFile::add
//
// Adds a handler to the IncludedFile event

void Index::IncludedFile::add(IndexIncludedFileHandler^ handler)
{
	m_includedfile = safe_cast<IndexIncludedFileHandler^>(Delegate::Combine(m_includedfile, handler));
}

//---------------------------------------------------------------------------
// Index::IncludedFile::raise
//
// Raises the IncludedFile event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::IncludedFile::raise(Object^ sender, IndexIncludedFileEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_includedfile, nullptr)) m_includedfile(sender, args);
}

//---------------------------------------------------------------------------
// Index::IncludedFile::remove
//
// Removes a handler from the IncludedFile event

void Index::IncludedFile::remove(IndexIncludedFileHandler^ handler)
{
	m_includedfile = safe_cast<IndexIncludedFileHandler^>(Delegate::Remove(m_includedfile, handler));
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file

void Index::IndexSourceFile(String^ filename)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, nullptr, IndexOptions::None, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, nullptr, IndexOptions::None, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	unsavedfiles	- Collection of virtualc source file data

void Index::IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, unsavedfiles, IndexOptions::None, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	options			- Options to control source code parsing behaviors

void Index::IndexSourceFile(String^ filename, IndexOptions options)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, nullptr, options, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, unsavedfiles, IndexOptions::None, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	options			- Options to control source code parsing behaviors

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IndexOptions options)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, nullptr, options, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors

void Index::IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, unsavedfiles, options, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control indexing behavior

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, unsavedfiles, options, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, nullptr, IndexOptions::None, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, nullptr, IndexOptions::None, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	unsavedfiles	- Collection of virtualc source file data
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, unsavedfiles, IndexOptions::None, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	options			- Options to control source code parsing behaviors
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IndexOptions options, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, nullptr, options, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, unsavedfiles, IndexOptions::None, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	options			- Options to control source code parsing behaviors
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IndexOptions options, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, args, nullptr, options, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	IndexSourceFile(filename, nullptr, unsavedfiles, options, state);
}

//---------------------------------------------------------------------------
// Index::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control indexing behavior
//	state			- Caller-provided state object for event handlers

void Index::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state)
{
	CHECK_DISPOSED(m_disposed);

	// Create a new IndexAction to execute the indexing operation against this Index instance
	msclr::auto_handle<IndexAction> action(IndexAction::Create(this, m_handle, clang_IndexAction_create(IndexHandle::Reference(m_handle))));
	action->IndexSourceFile(filename, args, unsavedfiles, options, state);
}

//---------------------------------------------------------------------------
// Index::IndexTranslationUnit
//
// Index a translation unit using any registered event handlers
//
// Arguments:
//
//	transunit		- TranslationUnit to be indexed
//	options			- Index operation options

void Index::IndexTranslationUnit(TranslationUnit^ transunit)
{
	CHECK_DISPOSED(m_disposed);
	return IndexTranslationUnit(transunit, IndexOptions::None, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexTranslationUnit
//
// Index a translation unit using any registered event handlers
//
// Arguments:
//
//	transunit		- TranslationUnit to be indexed
//	options			- Index operation options

void Index::IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options)
{
	CHECK_DISPOSED(m_disposed);
	return IndexTranslationUnit(transunit, options, nullptr);
}

//---------------------------------------------------------------------------
// Index::IndexTranslationUnit
//
// Index a translation unit using any registered event handlers
//
// Arguments:
//
//	transunit		- TranslationUnit to be indexed
//	options			- Index operation options
//	state			- Caller-provided state object for event handlers

void Index::IndexTranslationUnit(TranslationUnit^ transunit, Object^ state)
{
	CHECK_DISPOSED(m_disposed);
	return IndexTranslationUnit(transunit, IndexOptions::None, state);
}

//---------------------------------------------------------------------------
// Index::IndexTranslationUnit
//
// Index a translation unit using any registered event handlers
//
// Arguments:
//
//	transunit		- TranslationUnit to be indexed
//	options			- Index operation options
//	state			- Caller-provided state object for event handlers

void Index::IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options, Object^ state)
{
	CHECK_DISPOSED(m_disposed);

	// Create a new IndexAction to execute the indexing operation against this Index instance
	msclr::auto_handle<IndexAction> action(IndexAction::Create(this, m_handle, clang_IndexAction_create(IndexHandle::Reference(m_handle))));
	action->IndexTranslationUnit(transunit, options, state);
}

//---------------------------------------------------------------------------
// Index::LoadTranslationUnit
//
// Loads a translation unit from a serialized abstract syntax tree
//
// Arguments:
//
//	path		- Path to the serialized abstract syntax tree

TranslationUnit^ Index::LoadTranslationUnit(String^ path)
{
	CHECK_DISPOSED(m_disposed);
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");

	// Convert the managed path string into a standard C-style string
	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try {

		CXTranslationUnit	tu;			// Loaded translation unit instance

		// Create the new translation unit from the serialized abstract syntax tree file
		CXErrorCode result = clang_createTranslationUnit2(IndexHandle::Reference(m_handle), pszpath, &tu);
		if(result != CXError_Success) throw gcnew ClangException(result);

		return TranslationUnit::Create(m_handle, std::move(tu));
	}

	finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// Index::StartedTranslationUnit::add
//
// Adds a handler to the StartedTranslationUnit event

void Index::StartedTranslationUnit::add(IndexStartedTranslationUnitHandler^ handler)
{
	m_startedtransunit = safe_cast<IndexStartedTranslationUnitHandler^>(Delegate::Combine(m_startedtransunit, handler));
}

//---------------------------------------------------------------------------
// Index::StartedTranslationUnit::raise
//
// Raises the StartedTranslationUnit event
//
// Arguments:
//
//	sender		- Object raising the event
//	args		- Event arguments instance

void Index::StartedTranslationUnit::raise(Object^ sender, IndexEventArgs^ args)
{
	CHECK_DISPOSED(m_disposed);
	if(!Object::ReferenceEquals(m_startedtransunit, nullptr)) m_startedtransunit(sender, args);
}

//---------------------------------------------------------------------------
// Index::StartedTranslationUnit::remove
//
// Removes a handler from the StartedTranslationUnit event

void Index::StartedTranslationUnit::remove(IndexStartedTranslationUnitHandler^ handler)
{
	m_startedtransunit = safe_cast<IndexStartedTranslationUnitHandler^>(Delegate::Remove(m_startedtransunit, handler));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
