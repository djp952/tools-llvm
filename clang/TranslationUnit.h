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

#ifndef __TRANSLATIONUNIT_H_
#define __TRANSLATIONUNIT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
enum class	CompletionOptions;
ref class	CompletionResultCollection;
ref class	Cursor;
ref class	DiagnosticCollection;
ref class	Extent;
ref class	File;
ref class	Location;
ref class	LocationCollection;
ref class	ResourceUsageDictionary;
ref class	TokenCollection;
ref class	TranslationUnitHandle;
enum class	TranslationUnitSaveOptions;
ref class	UnsavedFile;

//---------------------------------------------------------------------------
// Class TranslationUnit
//
// Represents a source code translation unit
//
// Reparsing is not available (clang_reparseTranslationUnit) in this
// implementation.  Reparsing the translation unit invalidates everything
// within it, but the original CXTranslationUnit handle remains the same.
// It's prohibitively difficult to automatically dispose of all resources
// that pointed into (e.g. diagnostics) or were spawned from (e.g. cursors)
// the translation unit.  Given that the end result of a reparse operation
// is essentially a new translation unit, I see little value in the additional
// housekeeping that would be required.
//---------------------------------------------------------------------------

public ref class TranslationUnit
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// CompleteAt
	//
	// Perform code completion at a given location in the translation unit
	CompletionResultCollection^ CompleteAt(String^ filename, int line, int column);
	CompletionResultCollection^ CompleteAt(String^ filename, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles);
	CompletionResultCollection^ CompleteAt(String^ filename, int line, int column, CompletionOptions options);
	CompletionResultCollection^ CompleteAt(String^ filename, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options);

	// EnumerateIncludedFiles
	//
	// Enumerates the included files of this translation unit
	void EnumerateIncludedFiles(Action<File^, LocationCollection^>^ action);

	// GetFile
	//
	// Gets a File instance from this translation unit
	File^ GetFile(String^ filename);

	// Save
	//
	// Serializes the translation unit into an output file
	void Save(String^ path);
	void Save(String^ path, TranslationUnitSaveOptions options);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Cursor
	//
	// Gets the cursor that represents the given translation unit
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// Diagnostics
	//
	// Gets a read only collection of translation unit diagnostics
	property DiagnosticCollection^ Diagnostics
	{
		DiagnosticCollection^ get(void);
	}

	// ResourceUsage
	//
	// Gets the translation unit resource usage
	property ResourceUsageDictionary^ ResourceUsage
	{
		ResourceUsageDictionary^ get(void);
	}

	// Spelling
	//
	// Gets the original translation unit file name
	property String^ Spelling
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new TranslationUnit instance
	static TranslationUnit^ Create(SafeHandle^ owner, CXTranslationUnit&& transunit);

	//-----------------------------------------------------------------------
	// Internal Properties

	// Handle
	//
	// Exposes the underlying TranslationUnitHandle
	property TranslationUnitHandle^ Handle
	{
		TranslationUnitHandle^ get(void);
	}

private:

	// Instance Constructor
	//
	TranslationUnit(TranslationUnitHandle^ handle);

	// Destructor
	//
	~TranslationUnit();

	//-----------------------------------------------------------------------
	// Member Variables

	bool							m_disposed;		// Object disposal flag
	TranslationUnitHandle^			m_handle;		// Underlying safe handle
	local::Cursor^					m_cursor;		// Translation unit cursor
	DiagnosticCollection^			m_diags;		// Cached diagnostics
	String^							m_spelling;		// Cached spelling string
	ResourceUsageDictionary^		m_usage;		// Cached resource usage
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TRANSLATIONUNIT_H_
