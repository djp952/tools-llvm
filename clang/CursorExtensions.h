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

#ifndef __CURSOREXTENSIONS_H_
#define __CURSOREXTENSIONS_H_
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
value class	EnumerateChildrenResult;
value class EnumerateReferencesResult;
ref class	Extent;
ref class	File;
ref class	TokenCollection;

BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// Class CursorExtensions
//
// Provides extension methods to supplement the Cursor class. 
//
// Extensions are used when a method doesn't need to access anything but the
// existing public interfaces, the idea being not to clutter up the library
// code with convenience operations
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class CursorExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// EnumerateReferences
	//
	// Enumerates references to this cursor in a specific file
	[ExtensionAttribute]
	static void EnumerateReferences(Cursor^ cursor, File^ file, Func<Cursor^, Extent^, EnumerateReferencesResult>^ func);

	// FindChildren
	//
	// Finds all child cursors of this cursor based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ FindChildren(Cursor^ cursor, Func<Cursor^, Cursor^, bool>^ predicate);

	// FindChildren
	//
	// Finds all child cursors of this cursor based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ FindChildren(Cursor^ cursor, Func<Cursor^, Cursor^, bool>^ predicate, bool recursive);

	// FindReferences
	//
	// Finds all file references to this cursor based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FindReferences(Cursor^ cursor, File^ file, Func<Cursor^, Extent^, bool>^ predicate);

	// FindReferences
	//
	// Finds all file references to this cursor based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ FindReferences(Cursor^ cursor, String^ filename, Func<Cursor^, Extent^, bool>^ predicate);

	// GetChildren
	//
	// Gets all child cursors of this cursor
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ GetChildren(Cursor^ cursor);

	// GetChildren
	//
	// Gets all child cursors of this cursor
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Cursor^>^>^ GetChildren(Cursor^ cursor, bool recursive);

	// GetReferences
	//
	// Gets all file references to this cursor
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ GetReferences(Cursor^ cursor, File^ file);

	// GetReferences
	//
	// Gets all file references to this cursor
	[ExtensionAttribute]
	static ReadOnlyCollection<Tuple<Cursor^, Extent^>^>^ GetReferences(Cursor^ cursor, String^ filename);

	// GetTokens
	//
	// Tokenizes a cursor extent
	[ExtensionAttribute]
	static TokenCollection^ GetTokens(Cursor^ cursor);

private:

	//-----------------------------------------------------------------------
	// Private Data Types

	// Class ChildCollector
	//
	// Specialization of List<> used when enumerating cursor children
	ref class ChildCollector : public List<Tuple<Cursor^, Cursor^>^>
	{
	public:

		// Instance Constructor
		//
		ChildCollector(Func<Cursor^, Cursor^, bool>^ predicate, bool recursive);

		// EnumerateChildrenCallback
		//
		// Callback delegate to provide to Cursor::EnumerateChildren
		EnumerateChildrenResult EnumerateChildrenCallback(Cursor^ cursor, Cursor^ parent);

	private:

		// Predicate used to filter collection additions
		//
		Func<Cursor^, Cursor^, bool>^ m_predicate;

		// Indicates if this is a recursive find or not
		//
		bool m_recursive;
	};

	// Class ReferenceCollector
	//
	// Specialization of List<> used when enumerating cursor children
	ref class ReferenceCollector : public List<Tuple<Cursor^, Extent^>^>
	{
	public:

		// Instance Constructor
		//
		ReferenceCollector(Func<Cursor^, Extent^, bool>^ predicate);

		// EnumerateReferencesCallback
		//
		// Callback delegate to provide to Cursor::EnumerateFileReferences
		EnumerateReferencesResult EnumerateReferenceCallback(Cursor^ cursor, Extent^ extent);

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

#endif	// __CURSOREXTENSIONS_H_
