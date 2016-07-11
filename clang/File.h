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

#ifndef __FILE_H_
#define __FILE_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class	Cursor;
value class EnumerateIncludeDirectivesResult;
ref class	Extent;
ref class	ExtentCollection;
value class	FileUniqueIdentifier;
ref class	Location;
ref class	Module;

//---------------------------------------------------------------------------
// Class File
//
// Represents a particular source file that is part of a translation unit
//---------------------------------------------------------------------------

public ref class File
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(File^ lhs, File^ rhs);

	// operator!= (static)
	//
	static bool operator!=(File^ lhs, File^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// EnumerateIncludeDirectives
	//
	// Enumerates the #import/#include directives in this specific file
	void EnumerateIncludeDirectives(Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func);

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this File instance to another File instance
	bool Equals(File^ rhs);

	// GetExtent
	//
	// Obtain an extent from this file
	Extent^ GetExtent(int startoffset, int endoffset);
	Extent^ GetExtent(int startline, int startcolumn, int endline, int endcolumn);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// GetLocation
	//
	// Obtain a location from this file
	Location^ GetLocation(int offset);
	Location^ GetLocation(int line, int column);

	// IsNull (static)
	//
	// Determines if the specified file object is null
	static bool IsNull(File^ rhs);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// IsMultipleIncludeGuarded
	//
	// Indicates if the given file is guarded against multiple inclusions
	property bool IsMultipleIncludeGuarded
	{
		bool get(void);
	}

	// Module
	//
	// Gets the module associated with this file
	property local::Module^ Module
	{
		local::Module^ get(void);
	}

	// Name
	//
	// Gets the name of the file
	property String^ Name
	{
		String^ get(void);
	}

	// SkippedExtents
	//
	// Gets a collection of all extents skipped by the preprocessor
	property ExtentCollection^ SkippedExtents
	{
		ExtentCollection^ get(void);
	}

	// TimeStamp
	//
	// Gets the last modification time of the file
	property DateTime TimeStamp
	{
		DateTime get(void);
	}

	// UniqueIdentifier
	//
	// Gets the unique identifier for this file
	property FileUniqueIdentifier UniqueIdentifier
	{
		FileUniqueIdentifier get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Represents a null file instance
	static initonly File^ Null;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new File instance
	static File^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXFile file);

private:

	// FileHandle
	//
	// TranslationUnitReferenceHandle specialization for CXFile
	using FileHandle = TranslationUnitReferenceHandle<CXFile>;

	// Instance Constructor
	//
	File(FileHandle^ handle);

	// Static Constructor
	//
	static File();

	//-----------------------------------------------------------------------
	// Member Variables

	FileHandle^					m_handle;		// Underlying safe handle
	String^						m_name;			// Cached file name
	local::Module^				m_module;		// Cached module instance
	ExtentCollection^			m_skipped;		// Cached skipped extents
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __FILE_H_
