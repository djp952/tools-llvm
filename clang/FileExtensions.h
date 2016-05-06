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

#ifndef __FILEEXTENSIONS_H_
#define __FILEEXTENSIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Runtime::CompilerServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class	Cursor;
value class	EnumerateIncludeDirectivesResult;
value class EnumerateReferencesResult;
ref class	Extent;
ref class	File;

BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// Class FileExtensions
//
// Provides extension methods to supplement the File class. 
//
// Extensions are used when a method doesn't need to access anything but the
// existing public interfaces, the idea being not to clutter up the library
// code with convenience operations
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class FileExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// EnumerateCursorReferences
	//
	// Enumerates references to a cursor in a file
	[ExtensionAttribute]
	static void EnumerateCursorReferences(File^ file, Cursor^ cursor, Func<Cursor^, Extent^, EnumerateReferencesResult>^ func);

	// FindCursorReferences
	//
	// Finds all references to a cursor in a file based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FindCursorReferences(File^ file, Cursor^ cursor, Func<Cursor^, Extent^, bool>^ predicate);

	// FindIncludeDirectives
	//
	// Finds all include directives in the file based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FindIncludeDirectives(File^ file, Func<Cursor^, Extent^, bool>^ predicate);

	// GetCursorReferences
	//
	// Gets all references to a cursor in a file
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ GetCursorReferences(File^ file, Cursor^ cursor);

	// GetIncludeDirectives
	//
	// Finds all include directives in the file
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ GetIncludeDirectives(File^ file);

private:

	//-----------------------------------------------------------------------
	// Private Data Types

	// Class IncludeDirectiveCollector
	//
	// Specialization of List<> used when enumerating cursor children
	ref class IncludeDirectiveCollector : public List<Tuple<Cursor^, Extent^>^>
	{
	public:

		// Instance Constructor
		//
		IncludeDirectiveCollector(Func<Cursor^, Extent^, bool>^ predicate);

		// EnumerateIncludeDirectiveCallback
		//
		// Callback delegate to provide to File::EnumerateIncludeDirectives
		EnumerateIncludeDirectivesResult EnumerateIncludeDirectiveCallback(Cursor^ cursor, Extent^ extent);

	private:

		// Predicate used to filter collection additions
		//
		Func<Cursor^, Extent^, bool>^ m_predicate;
	};
};

//---------------------------------------------------------------------------

END_NAMESPACE(extensions)
END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __FILEEXTENSIONS_H_
