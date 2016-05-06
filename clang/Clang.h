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

#ifndef __CLANG_H_
#define __CLANG_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Runtime::Serialization::Json;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	CompilationDatabase;
ref class	Index;
ref class	LoadedDiagnosticCollection;
ref class	ModuleMapDescriptor;
ref class	RemappingCollection;
ref class	TranslationUnit;
enum class	TranslationUnitParseOptions;
ref class	UnsavedFile;
ref class	VirtualFileOverlay;

//---------------------------------------------------------------------------
// Class Clang
//
// Provides access to global CLANG functions
//---------------------------------------------------------------------------

public ref class Clang
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// CreateCompilationDatabase (static)
	//
	// Creates a new CompilationDatabase instance
	static CompilationDatabase^ CreateCompilationDatabase(String^ path);

	// CreateIndex (static)
	//
	// Creates a new Index instance
	static Index^ CreateIndex(void);
	static Index^ CreateIndex(bool excludeDeclarationsFromPCH);
	static Index^ CreateIndex(bool excludeDeclarationsFromPCH, bool displayDiagnostics);

	// CreateModuleMapDescriptor (static)
	//
	// Creates a new ModuleMapDescriptor instance
	static ModuleMapDescriptor^ CreateModuleMapDescriptor(void);
	static ModuleMapDescriptor^ CreateModuleMapDescriptor(String^ frameworkmodule, String^ umbrellaheader);
	
	// CreateRemappings (static)
	//
	// Creates a new collection of file remappings from existing YAML input files
	static RemappingCollection^ CreateRemappings(IEnumerable<String^>^ files);

	// CreateTranslationUnit (static)
	//
	// Create a TranslationUnit in a default Index by parsing a source code file
	static TranslationUnit^ CreateTranslationUnit(String^ filename);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, TranslationUnitParseOptions options);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, TranslationUnitParseOptions options);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options);
	static TranslationUnit^ CreateTranslationUnit(String^ filename, IEnumerable<String^>^ args, IEnumerable<UnsavedFile^>^ unsavedfiles, TranslationUnitParseOptions options);

	// CreateVirtualFileOverlay (static)
	//
	// Creates a new VirtualFileOverlay instance
	static VirtualFileOverlay^ CreateVirtualFileOverlay(void);
	
	// GetBuildSessionTimestamp (static)
	//
	// Gets the timestamp for use with Clang's -fbuild-session-timestamp= option
	static unsigned __int64 GetBuildSessionTimestamp(void);

	// LoadDiagnostics (static)
	//
	// Creates a diagnostic collection from a serialized file
	static LoadedDiagnosticCollection^ LoadDiagnostics(String^ path);

	// LoadRemappings (static)
	//
	// Loads an existing remapping collection from a metadata path
	static RemappingCollection^ LoadRemappings(String^ path);

	// LoadTranslationUnit (static)
	//
	// Loads a translation unit in a default index from a serialized abstract syntax tree
	static TranslationUnit^ LoadTranslationUnit(String^ path);

	// SetCrashRecovery (static)
	//
	// Enable/disable clang crash recovery
	static void SetCrashRecovery(bool enable);

	//-----------------------------------------------------------------------
	// Properties

	// Version (static)
	//
	// Gets the CLANG version
	property String^ Version
	{
		static String^ get(void);
	}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	static String^			s_version;			// CLANG version string
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __CLANG_H_
