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
#include "IndexAction.h"

#include "ClangException.h"
#include "File.h"
#include "Index.h"
#include "IndexAbortEventArgs.h"
#include "IndexDeclaration.h"
#include "IndexDeclarationEventArgs.h"
#include "IndexDiagnosticCollection.h"
#include "IndexDiagnosticsEventArgs.h"
#include "IndexEnteredMainFileEventArgs.h"
#include "IndexEntityReference.h"
#include "IndexEntityReferenceEventArgs.h"
#include "IndexEventArgs.h"
#include "IndexImportedASTFile.h"
#include "IndexImportedASTFileEventArgs.h"
#include "IndexIncludedFile.h"
#include "IndexIncludedFileEventArgs.h"
#include "IndexOptions.h"
#include "StringUtil.h"
#include "TranslationUnit.h"
#include "TranslationUnitHandle.h"
#include "UnsavedFile.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// DisposeIndexActionCallback
//
// Unmanaged disposal function for an IndexActionCallback (no-op)
//
// Arguments:
//
//	unnamed		- Dummy argument

void DisposeIndexActionCallback(nullptr_t)
{
	// This operation is a no-op, there is no such object as an index action
	// callback, it's a dummy construct used with UnmanagedTypeSafeHandle
}

//---------------------------------------------------------------------------
// IndexAction Constructor (private)
//
// Arguments:
//
//	index		- Parent index instance
//	handle		- IndexAction safe handle for this instance

IndexAction::IndexAction(Index^ index, IndexActionHandle^ handle) : m_index(index), m_handle(handle)
{
	if(Object::ReferenceEquals(index, nullptr)) throw gcnew ArgumentNullException("index");
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// Create the managed delegate callbacks as member variables to prevent garbage collection
	// from invalidating them while the index action is executing (CallingConvention::Cdecl)
	m_onabort = gcnew OnAbortQueryDelegate(this, &IndexAction::OnAbortQuery);
	m_ondiagnostics = gcnew OnDiagnosticsDelegate(this, &IndexAction::OnDiagnostics);
	m_onenteredmainfile = gcnew OnEnteredMainFileDelegate(this, &IndexAction::OnEnteredMainFile);
	m_onimportedastfile = gcnew OnImportedASTFileDelegate(this, &IndexAction::OnImportedASTFile);
	m_onincludedfile = gcnew OnIncludedFileDelegate(this, &IndexAction::OnIncludedFile);
	m_ondeclaration = gcnew OnIndexDeclarationDelegate(this, &IndexAction::OnIndexDeclaration);
	m_onentityreference = gcnew OnIndexEntityReferenceDelegate(this, &IndexAction::OnIndexEntityReference);
	m_onstartedtransunit = gcnew OnStartedTranslationUnitDelegate(this, &IndexAction::OnStartedTranslationUnit);

	// Create a List<> to hold tracking GCHandles for all client provided objects
	m_clientobjs = gcnew List<GCHandle>();
}

//---------------------------------------------------------------------------
// IndexAction Destructor (private)

IndexAction::~IndexAction()
{
	if(m_disposed) return;

	// Release the GCHandles allocated for all client objects in this IndexAction
	for each(GCHandle gch in m_clientobjs) gch.Free();

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// IndexAction::Create (internal, static)
//
// Creates a new IndexAction instance
//
// Arguments:
//
//	index		- Parent Index instance
//	owner		- Owning safe handle instance
//	action		- Unmanaged CXIndexAction instance to take ownership of

IndexAction^ IndexAction::Create(Index^ index, SafeHandle^ owner, CXIndexAction&& action)
{
	return gcnew IndexAction(index, gcnew IndexActionHandle(owner, std::move(action)));
}

//---------------------------------------------------------------------------
// IndexAction::IndexSourceFile
//
// Index the specified source file using any registered event handlers
//
// Arguments:
//
//	filename		- Path to the input source code file
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control indexing behavior
//	state			- Optional state object provided by the caller

void IndexAction::IndexSourceFile(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, IndexOptions options, Object^ state)
{
	int						numargs = 0;				// Number of argument strings
	int						numunsaved = 0;				// Number of UnsavedFile object instances
	bool					fullcmdline = false;		// Flag if args contains a full command line

	CHECK_DISPOSED(m_disposed);

	// The state object, if provided, will be passed into the callbacks as a GCHandle
	IntPtr gcstate = (Object::ReferenceEquals(state, nullptr)) ? IntPtr::Zero : GCHandle::ToIntPtr(GCHandle::Alloc(state, GCHandleType::Normal));

	try {

		// Create a new set of IndexerCallbacks using pointers to the member delegates
		IndexerCallbacks callbacks = {

			static_cast<OnAbortQueryPointer>(Marshal::GetFunctionPointerForDelegate(m_onabort).ToPointer()),
			static_cast<OnDiagnosticsPointer>(Marshal::GetFunctionPointerForDelegate(m_ondiagnostics).ToPointer()),
			static_cast<OnEnteredMainFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onenteredmainfile).ToPointer()),
			static_cast<OnIncludedFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onincludedfile).ToPointer()),
			static_cast<OnImportedASTFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onimportedastfile).ToPointer()),
			static_cast<OnStartedTranslationUnitPointer>(Marshal::GetFunctionPointerForDelegate(m_onstartedtransunit).ToPointer()),
			static_cast<OnIndexDeclarationPointer>(Marshal::GetFunctionPointerForDelegate(m_ondeclaration).ToPointer()),
			static_cast<OnIndexEntityReferencePointer>(Marshal::GetFunctionPointerForDelegate(m_onentityreference).ToPointer()),
		};

		// The custom IndexOptions::ArgumentsAreFullCommandLine has to be checked and removed from the flags 
		// before they are passed into clang -- this indicates a different method call
		if((options & IndexOptions::ArgumentsAreFullCommandLine) == IndexOptions::ArgumentsAreFullCommandLine) {

			fullcmdline = true;
			options = IndexOptions(options & ~IndexOptions::ArgumentsAreFullCommandLine);
		}

		// Convert the managed path string into a standard C-style string (NULL is OK here)
		char* pszfilename = StringUtil::ToCharPointer(filename, CP_UTF8);

		try { 
		
			// Convert the enumerable range of string arguments into an unmanaged array and check
			// that there is at least one argument specified if necessary for argv[0]
			char** rgszargs = StringUtil::ToCharPointerArray(args, CP_UTF8, &numargs);
			if((fullcmdline) && (numargs == 0)) throw gcnew ArgumentNullException("args");

			try { 
			
				// Convert the enumerable range of UnsavedFile objects into an unmanaged array
				CXUnsavedFile* rgunsaved = UnsavedFile::UnsavedFilesToArray(unsavedfiles, &numunsaved);

				try {

					// Index the source file using the provided arguments and unsaved file objects
					int result = (fullcmdline) ?
						clang_indexSourceFileFullArgv(IndexActionHandle::Reference(m_handle), gcstate.ToPointer(), &callbacks, sizeof(IndexerCallbacks), static_cast<unsigned int>(options), pszfilename, rgszargs, numargs, rgunsaved, numunsaved, __nullptr, 0) :
						clang_indexSourceFile(IndexActionHandle::Reference(m_handle), gcstate.ToPointer(), &callbacks, sizeof(IndexerCallbacks), static_cast<unsigned int>(options), pszfilename, rgszargs, numargs, rgunsaved, numunsaved, __nullptr, 0);

					if(result != CXError_Success) throw gcnew ClangException(CXErrorCode(result));

				} finally { UnsavedFile::FreeUnsavedFilesArray(rgunsaved, numunsaved); }

			} finally { StringUtil::FreeCharPointerArray(rgszargs);  }

		} finally { StringUtil::FreeCharPointer(pszfilename); }
	}

	// Release any GCHandle allocated against the caller's context object
	finally { if(gcstate != IntPtr::Zero) GCHandle::FromIntPtr(gcstate).Free(); }
}

//---------------------------------------------------------------------------
// IndexAction::IndexTranslationUnit
//
// Index a translation unit using any registered event handlers
//
// Arguments:
//
//	transunit		- TranslationUnit to be indexed
//	options			- Index operation options
//	state			- Optional state object provided by the caller

void IndexAction::IndexTranslationUnit(TranslationUnit^ transunit, IndexOptions options, Object^ state)
{
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");

	CHECK_DISPOSED(m_disposed);

	// The state object, if provided, will be passed into the callbacks as a GCHandle
	IntPtr gcstate = (Object::ReferenceEquals(state, nullptr)) ? IntPtr::Zero : GCHandle::ToIntPtr(GCHandle::Alloc(state, GCHandleType::Normal));

	try {

		// Create a new set of IndexerCallbacks using pointers to the member delegates
		IndexerCallbacks callbacks = {

			static_cast<OnAbortQueryPointer>(Marshal::GetFunctionPointerForDelegate(m_onabort).ToPointer()),
			static_cast<OnDiagnosticsPointer>(Marshal::GetFunctionPointerForDelegate(m_ondiagnostics).ToPointer()),
			static_cast<OnEnteredMainFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onenteredmainfile).ToPointer()),
			static_cast<OnIncludedFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onincludedfile).ToPointer()),
			static_cast<OnImportedASTFilePointer>(Marshal::GetFunctionPointerForDelegate(m_onimportedastfile).ToPointer()),
			static_cast<OnStartedTranslationUnitPointer>(Marshal::GetFunctionPointerForDelegate(m_onstartedtransunit).ToPointer()),
			static_cast<OnIndexDeclarationPointer>(Marshal::GetFunctionPointerForDelegate(m_ondeclaration).ToPointer()),
			static_cast<OnIndexEntityReferencePointer>(Marshal::GetFunctionPointerForDelegate(m_onentityreference).ToPointer()),
		};

		// Atempt to index the provided translation unit instance using the specified callback pointers
		int result = clang_indexTranslationUnit(IndexActionHandle::Reference(m_handle), gcstate.ToPointer(), &callbacks, sizeof(IndexerCallbacks),
			static_cast<unsigned int>(options), TranslationUnitHandle::Reference(transunit->Handle));
 		if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(static_cast<CXErrorCode>(result));
	}

	// Release any GCHandle allocated against the caller's context object
	finally { if(gcstate != IntPtr::Zero) GCHandle::FromIntPtr(gcstate).Free(); }
}

//---------------------------------------------------------------------------
// IndexAction::OnAbortQuery (private)
//
// Invoked periodically to check whether indexing should be aborted
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	reserved		- Unused

int IndexAction::OnAbortQuery(CXClientData context, void* reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	IndexAbortEventArgs^ args = gcnew IndexAbortEventArgs(state);
	m_index->Abort(m_index, args);

	return (args->Abort) ? -1 : 0;
}

//---------------------------------------------------------------------------
// IndexAction::OnDiagnostics (private)
//
// Called at the end of indexing; passes the complete diagnostic set
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	diagnostics		- Generated set of indexing diagnostics
//	reserved		- Unused

void IndexAction::OnDiagnostics(CXClientData context, CXDiagnosticSet diagnostics, void* reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->Diagnostics(m_index, gcnew IndexDiagnosticsEventArgs(state, IndexDiagnosticCollection::Create(handle.get(), diagnostics)));
}

//---------------------------------------------------------------------------
// IndexAction::OnEnteredMainFile (private)
//
// Called when the indexing operation enters the main file
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	mainfile		- File instance representing the main translation unit file
//	reserved		- Unused

CXIdxClientFile IndexAction::OnEnteredMainFile(CXClientData context, CXFile mainfile, void* reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->EnteredMainFile(m_index, gcnew IndexEnteredMainFileEventArgs(state, File::Create(handle.get(), TranslationUnitHandle::Null, mainfile)));

	return mainfile;
}

//---------------------------------------------------------------------------
// IndexAction::OnImportedASTFile (private)
//
// IndexAction::Called when an AST file (PCH or module) gets imported
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	info			- Information about the imported AST file 

CXIdxClientASTFile IndexAction::OnImportedASTFile(CXClientData context, const CXIdxImportedASTFileInfo* info)
{
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->ImportedASTFile(m_index, gcnew IndexImportedASTFileEventArgs(state, IndexImportedASTFile::Create(handle.get(), this, info)));

	return info->file;
}

//---------------------------------------------------------------------------
// IndexAction::OnIncludedFile (private)
//
// Called when a file gets #included/#imported
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	info			- Information about the #included/#imported file

CXIdxClientFile IndexAction::OnIncludedFile(CXClientData context, const CXIdxIncludedFileInfo* info)
{
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->IncludedFile(m_index, gcnew IndexIncludedFileEventArgs(state, IndexIncludedFile::Create(handle.get(), this, info)));

	return info->file;
}

//---------------------------------------------------------------------------
// IndexAction::OnIndexDeclaration (private)
//
// Called when an index declaration is processed
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	info			- Information about the declaration

void IndexAction::OnIndexDeclaration(CXClientData context, const CXIdxDeclInfo* info)
{
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->Declaration(m_index, gcnew IndexDeclarationEventArgs(state, IndexDeclaration::Create(handle.get(), this, info)));
}

//---------------------------------------------------------------------------
// IndexAction::OnIndexEntityReference (private)
//
// Called to index a reference of an entity
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	info			- Information about the entity reference

void IndexAction::OnIndexEntityReference(CXClientData context, const CXIdxEntityRefInfo* info)
{
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	msclr::auto_handle<IndexActionCallbackHandle> handle(gcnew IndexActionCallbackHandle(m_handle, __nullptr));
	m_index->EntityReference(m_index, gcnew IndexEntityReferenceEventArgs(state, IndexEntityReference::Create(handle.get(), this, info)));
}

//---------------------------------------------------------------------------
// IndexAction::OnStartedTranslationUnit (private)
//
// Called at the beginning of indexing a translation unit
//
// Arguments:
//
//	context			- Caller supplied indexing context
//	reserved		- Unused

CXIdxClientContainer IndexAction::OnStartedTranslationUnit(CXClientData context, void* reserved)
{
	UNREFERENCED_PARAMETER(reserved);
	CHECK_DISPOSED(m_disposed);

	// Convert the context back into the optional caller context object
	Object^ state = (context == __nullptr) ? nullptr : GCHandle::FromIntPtr(IntPtr(context)).Target;

	m_index->StartedTranslationUnit(m_index, gcnew IndexEventArgs(state));
	return __nullptr;
}

//---------------------------------------------------------------------------
// IndexAction::TrackClientObject (internal)
//
// Creates a GCHandle for a client object to keep it alive
//
// Arguments:
//
//	object		- The client object to keep alive

GCHandle IndexAction::TrackClientObject(Object^ object)
{
	// Cannot (should not?) allocate a GCHandle against a null object
	if(Object::ReferenceEquals(object, nullptr)) throw gcnew ArgumentNullException("object");

	GCHandle gch = GCHandle::Alloc(object, GCHandleType::Normal);
	m_clientobjs->Add(gch);

	return gch;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
