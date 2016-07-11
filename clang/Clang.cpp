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
#include "Clang.h"

#include "CompilationDatabase.h"
#include "CompilationDatabaseLoadErrorCode.h"
#include "CompilationDatabaseLoadException.h"
#include "DiagnosticLoadException.h"
#include "Index.h"
#include "LoadedDiagnosticCollection.h"
#include "ModuleMapDescriptor.h"
#include "RemappingCollection.h"
#include "StringUtil.h"
#include "TranslationUnit.h"
#include "TranslationUnitParseOptions.h"
#include "UnsavedFile.h"
#include "VirtualFileOverlay.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Clang::CreateCompilationDatabase (static)
//
// Creates a new CompilationDatabase instance
//
// Arguments:
//
//	path		- Path to the directory containing the compilation database

CompilationDatabase^ Clang::CreateCompilationDatabase(String^ path)
{
	CXCompilationDatabase_Error		result;		// Result from compilation database operation

	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");
	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try { 

#pragma message("CLANG WORKAROUND: Invalid JSON causes loader lock problem")
		// If the compile_commands.json file has a problem, calling clang_CompilationDatabase_fromDirectory() will
		// cause a loader lock problem.  Avoid this by checking that the JSON will parse before calling clang
		String^ jsonfile = Path::Combine(path, "compile_commands.json");
		if(System::IO::File::Exists(jsonfile)) {

			DataContractJsonSerializer^ json = gcnew DataContractJsonSerializer(Object::typeid);
			MemoryStream^ ms = gcnew MemoryStream(System::IO::File::ReadAllBytes(jsonfile));
			
			// Attempt to read the JSON data from the memory stream to ensure that it's not malformed
			try { json->ReadObject(ms); }
			finally { delete ms; }
		}

		// Attempt to create the compilation database from the specified directory
		CXCompilationDatabase database = clang_CompilationDatabase_fromDirectory(pszpath, &result);
		if(result != CXCompilationDatabase_NoError) throw gcnew CompilationDatabaseLoadException(result);

		return CompilationDatabase::Create(std::move(database));
	}
	
	finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// Clang::CreateIndex (static)
//
// Creates a new Index instance
//
// Arguments:
//
//	NONE

Index^ Clang::CreateIndex(void)
{
	return CreateIndex(false, false);
}

//---------------------------------------------------------------------------
// Clang::CreateIndex (static)
//
// Creates a new Index instance
//
// Arguments:
//
//	excludeDeclarationsFromPCH	- Allow enumeration of "local" declarations

Index^ Clang::CreateIndex(bool excludeDeclarationsFromPCH)
{
	return CreateIndex(excludeDeclarationsFromPCH, false);
}

//---------------------------------------------------------------------------
// Clang::CreateIndex (static)
//
// Creates a new Index instance
//
// Arguments:
//
//	excludeDeclarationsFromPCH	- Allow enumeration of "local" declarations
//	displayDiagnostics			- Display diagnostic information

Index^ Clang::CreateIndex(bool excludeDeclarationsFromPCH, bool displayDiagnostics)
{
	return Index::Create(clang_createIndex((excludeDeclarationsFromPCH) ? 1 : 0, (displayDiagnostics) ? 1 : 0));
}

//---------------------------------------------------------------------------
// Clang::CreateModuleMapDescriptor (static)
//
// Creates a new ModuleMapDescriptor instance
//
// Arguments:
//
//	NONE

ModuleMapDescriptor^ Clang::CreateModuleMapDescriptor(void)
{
	return CreateModuleMapDescriptor(String::Empty, String::Empty);
}

//---------------------------------------------------------------------------
// Clang::CreateModuleMapDescriptor (static)
//
// Creates a new ModuleMapDescriptor instance
//
// Arguments:
//
//	frameworkmodule		- Initial value to assign as the framework module name
//	umbrellaheader		- Initial value to assign as the umbrella header name

ModuleMapDescriptor^ Clang::CreateModuleMapDescriptor(String^ frameworkmodule, String^ umbrellaheader)
{
	if(Object::ReferenceEquals(frameworkmodule, nullptr)) throw gcnew ArgumentNullException("frameworkmodule");
	if(Object::ReferenceEquals(umbrellaheader, nullptr)) throw gcnew ArgumentNullException("umbrellaheader");

	return ModuleMapDescriptor::Create(clang_ModuleMapDescriptor_create(0), frameworkmodule, umbrellaheader);
}

//---------------------------------------------------------------------------
// Clang::CreateRemappings (static)
//
// Creates a new collection of file remappings from existing YAML input files
//
// Arguments:
//
//	filepaths	- Enumerable range of input YAML file paths with remapping info

RemappingCollection^ Clang::CreateRemappings(IEnumerable<String^>^ filepaths)
{
	int					numpaths = 0;			// Number of paths provided

	if(Object::ReferenceEquals(filepaths, nullptr)) throw gcnew ArgumentNullException("filepaths");

	char** rgpaths = StringUtil::ToCharPointerArray(filepaths, CP_UTF8, &numpaths);

	try { return RemappingCollection::Create(clang_getRemappingsFromFileList(const_cast<const char**>(rgpaths), static_cast<unsigned int>(numpaths))); }
	finally { StringUtil::FreeCharPointerArray(rgpaths); }
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)

TranslationUnit^ Clang::CreateTranslationUnit(String^ path)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, args);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	unsavedfiles	- Collection of virtualc source file data

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, unsavedfiles);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, TranslationUnitParseOptions options)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, options);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, args, unsavedfiles);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, TranslationUnitParseOptions options)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, args, options);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, unsavedfiles, options);
}

//---------------------------------------------------------------------------
// Clang::CreateTranslationUnit (static)
//
// Create a TranslationUnit in a default Index by parsing source code
//
// Arguments:
//
//	path			- Path to the input source code file (optional)
//	args			- Arguments to pass to the libclang engine
//	unsavedfiles	- Collection of virtual source file data
//	options			- Options to control source code parsing behaviors

TranslationUnit^ Clang::CreateTranslationUnit(String^ path, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->CreateTranslationUnit(path, args, unsavedfiles, options);
}

//---------------------------------------------------------------------------
// Clang::CreateVirtualFileOverlay (static)
//
// Creates a new VirtualFileOverlay instance
//
// Arguments:
//
//	NONE

VirtualFileOverlay^ Clang::CreateVirtualFileOverlay(void)
{
	return VirtualFileOverlay::Create(clang_VirtualFileOverlay_create(0));
}

//---------------------------------------------------------------------------
// Clang::GetBuildSessionTimestamp (static)
//
// Gets the timestamp for use with Clang's -fbuild-session-timestamp= option
//
// Arguments:
//
//	NONE

unsigned __int64 Clang::GetBuildSessionTimestamp(void)
{
	return clang_getBuildSessionTimestamp();
}

//---------------------------------------------------------------------------
// Clang::LoadDiagnostics (static)
//
// Creates a new DiagnosticCollection instance from a serialized file
//
// Arguments:
//
//	path		- Path to the serialized diagnostics file to load

LoadedDiagnosticCollection^ Clang::LoadDiagnostics(String^ path)
{
	CXLoadDiag_Error		error;			// Error code
	CXString				message;		// Error message

	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");
	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try {

		// Attempt to load the serialized diagnostics set
		CXDiagnosticSet diagnostics = clang_loadDiagnostics(pszpath, &error, &message);
		
		// Create a new standalone LoadedDiagnosticCollection instance and return it
		if(diagnostics != __nullptr) return LoadedDiagnosticCollection::Create(std::move(diagnostics));
		else throw gcnew DiagnosticLoadException(error, std::move(message));
	}

	finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// Clang::LoadRemappings (static)
//
// Loads an existing remapping collection from a metadata path
//
// Arguments:
//
//	path		- Metadata path from which to create the remappings

RemappingCollection^ Clang::LoadRemappings(String^ path)
{
	// Convert the path into a regular C-style string to pass into the API
	char* pszpath = StringUtil::ToCharPointer(path, CP_UTF8);

	try { return RemappingCollection::Create(clang_getRemappings(pszpath)); }
	finally { StringUtil::FreeCharPointer(pszpath); }
}

//---------------------------------------------------------------------------
// Clang::LoadTranslationUnit (static)
//
// Loads a translation unit in a default Index from a serialized abstract syntax tree
//
// Arguments:
//
//	path		- Path to the serialized abstract syntax tree

TranslationUnit^ Clang::LoadTranslationUnit(String^ path)
{
	msclr::auto_handle<Index> index(CreateIndex());
	return index->LoadTranslationUnit(path);
}

//---------------------------------------------------------------------------
// Clang::SetCrashRecovery
//
// Enable/disable clang crash recovery
//
// Arguments:
//
//	enable		- Enable/disable flag

void Clang::SetCrashRecovery(bool value)
{
	// The current API doesn't expose what the current value is, just set it
	clang_toggleCrashRecovery((value) ? 1 : 0);
}

//---------------------------------------------------------------------------
// Clang::Version::get (static)
//
// Gets the CLANG version

String^ Clang::Version::get(void)
{
	if(Object::ReferenceEquals(s_version, nullptr)) s_version = StringUtil::ToString(clang_getClangVersion());
	return s_version;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
