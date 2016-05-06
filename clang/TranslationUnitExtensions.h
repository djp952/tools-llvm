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

#ifndef __TRANSLATIONUNITEXTENSIONS_H_
#define __TRANSLATIONUNITEXTENSIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Runtime::CompilerServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
enum class	CompletionOptions;
ref class	CompletionResultCollection;
ref class	Extent;
ref class	File;
ref class	Location;
ref class	LocationCollection;
ref class	TokenCollection;
ref class	TranslationUnit;
ref class	UnsavedFile;

BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// Class TranslationUnitExtensions
//
// Provides extension methods to supplement the TranslationUnit class. 
//
// Extensions are used when a method doesn't need to access anything but the
// existing public interfaces, the idea being not to clutter up the library
// code with convenience operations
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class TranslationUnitExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// CompleteAt
	//
	// Perform code completion at a given location in the translation unit
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, Location^ location);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, Location^ location, IEnumerable<UnsavedFile^>^ unsavedfiles);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, Location^ location, CompletionOptions options);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, Location^ location, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, CompletionOptions options);
	[ExtensionAttribute]
	static CompletionResultCollection^ CompleteAt(TranslationUnit^ transunit, File^ file, int line, int column, IEnumerable<UnsavedFile^>^ unsavedfiles, CompletionOptions options);

	// FindIncludedFiles
	//
	// Finds all included files based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<File^, LocationCollection^>^>^ FindIncludedFiles(TranslationUnit^ transunit, Func<File^, LocationCollection^, bool>^ predicate);

	// GetExtent
	//
	// Obtain an Extent from a translation unit file
	[ExtensionAttribute]
	static Extent^ GetExtent(TranslationUnit^ transunit, String^ filename, int startoffset, int endoffset);
	[ExtensionAttribute]
	static Extent^ GetExtent(TranslationUnit^ transunit, String^ filename, int startline, int startcolumn, int endline, int endcolumn);

	// GetIncludedFiles
	//
	// Gets all included files
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<File^, LocationCollection^>^>^ GetIncludedFiles(TranslationUnit^ transunit);

	// GetLocation
	//
	// Obtain a Location from a translation unit file
	[ExtensionAttribute]
	static Location^ GetLocation(TranslationUnit^ transunit, String^ filename, int offset);
	[ExtensionAttribute]
	static Location^ GetLocation(TranslationUnit^ transunit, String^ filename, int line, int column);

	// GetTokens
	//
	// Tokenizes an entire translation unit
	[ExtensionAttribute]
	static TokenCollection^ GetTokens(TranslationUnit^ transunit);

	// IsFileMultipleIncludeGuarded
	//
	// Indicates if the given file is guarded against multiple inclusions
	[ExtensionAttribute]
	static bool IsFileMultipleIncludeGuarded(TranslationUnit^ transunit, String^ filename);

private:

	//-----------------------------------------------------------------------
	// Private Data Types

	// Class IncludedFileCollector
	//
	// Specialization of List<> used when enumerating included files
	ref class IncludedFileCollector : public List<Tuple<File^, LocationCollection^>^>
	{
	public:

		// Instance Constructor
		//
		IncludedFileCollector(Func<File^, LocationCollection^, bool>^ predicate);

		// EnumerateIncludedFileCallback
		//
		// Callback delegate to provide to TranslationUnit::EnumerateIncludedFiles
		void EnumerateIncludedFileCallback(File^ file, LocationCollection^ refchain);

	private:

		// Predicate used to filter collection additions
		//
		Func<File^, LocationCollection^, bool>^ m_predicate;
	};
};

//---------------------------------------------------------------------------

END_NAMESPACE(extensions)
END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TRANSLATIONUNITEXTENSIONS_H_
