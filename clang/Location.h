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

#ifndef __LOCATION_H_
#define __LOCATION_H_
#pragma once

#include "LocationKind.h"
#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class Cursor;
ref class File;

//---------------------------------------------------------------------------
// Class Location
//
// Represents a particular location within a source file
//---------------------------------------------------------------------------

public ref class Location
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(Location^ lhs, Location^ rhs);

	// operator!= (static)
	//
	static bool operator!=(Location^ lhs, Location^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this Location instance to another Location instance
	bool Equals(Location^ rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// IsNull (static)
	//
	// Determines if the specified location object is null
	static bool IsNull(Location^ rhs);

	// ToString
	//
	// Overrides Object::ToString
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// default[LocationKind]
	//
	// Gets a specific kind of location instance
	property Location^ default[LocationKind]
	{
		Location^ get(LocationKind kind);
	}

	// Column
	//
	// Gets the file column number represented by this location
	property int Column
	{
		int get(void);
	}

	// Cursor
	//
	// Gets the cursor at this location within the translation unit
	property local::Cursor^ Cursor
	{
		local::Cursor^ get(void);
	}

	// File
	//
	// Gets the File associated with this location
	property local::File^ File
	{
		local::File^ get(void);
	}

	// IsFromMainFile
	//
	// Gets a flag indicating if this location is in the main translation unit file
	property bool IsFromMainFile
	{
		bool get(void);
	}

	// IsInSystemHeader
	//
	// Gets a flag indicating if this location is in a system header file
	property bool IsInSystemHeader
	{
		bool get(void);
	}

	// Kind
	//
	// Gets the kind of source location represented
	property LocationKind Kind
	{
		LocationKind get(void);
	}

	// Line
	//
	// Gets the file line number represented by this location
	property int Line
	{
		int get(void);
	}

	// Offset
	//
	// Gets the file offset represented by this location
	property int Offset
	{
		int get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Reppresents a null location
	static initonly Location^ Null;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions
	
	// Create (static)
	//
	// Creates a new Location instance
	static Location^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceLocation location, LocationKind kind);

private:

	// SourceLocationHandle
	//
	// TranslationUnitReferenceHandle specialization for CXSourceLocation
	using SourceLocationHandle = TranslationUnitReferenceHandle<CXSourceLocation>;

	// Instance Constructor
	//
	Location(SourceLocationHandle^ handle, LocationKind kind);

	// Static Constructor
	//
	static Location();

	//-----------------------------------------------------------------------
	// Member Variables

	SourceLocationHandle^	m_handle;		// Underlying safe handle instance
	LocationKind			m_kind;			// Kind of location represented
	local::File^			m_file;			// Location File instance
	int						m_line;			// Location line number
	int						m_column;		// Location column number
	int						m_offset;		// Location character offset
	local::Cursor^			m_cursor;		// Cached location cursor
	array<Location^>^		m_cache;		// Cached alternate locations
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __LOCATION_H_
