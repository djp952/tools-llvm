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
#include "Location.h"

#include "Cursor.h"
#include "File.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Location Static Constructor (private)

static Location::Location()
{
	Null = Location::Create(TranslationUnitHandle::Null, TranslationUnitHandle::Null, clang_getNullLocation(), LocationKind::Spelling);
}

//---------------------------------------------------------------------------
// Location Constructor (protected)
//
// Arguments:
//
//	handle		- Underlying SourceLocationHandle instance
//	kind		- Kind of location represented

Location::Location(SourceLocationHandle^ handle, LocationKind kind) : m_handle(handle), m_kind(kind)
{
	CXFile				file;							// Extracted file object
	unsigned int		line = 0;						// Extracted location line number
	unsigned int		column = 0;						// Extracted location column number
	unsigned int		offset = 0;						// Extracted location character offset
	CXString			presumedfile{ __nullptr, 0 };	// LocationKind::Presumed file name

	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	// Create a cache to hold object references to alternate location kinds
	m_cache = gcnew array<Location^>(4);
	m_cache[static_cast<int>(kind)] = this;

	SourceLocationHandle::Reference	location(handle);	// Unwrap the safe handle

	// Initialize the location properties based on the kind of location requested
	switch(kind) {

		// Spelling - use clang_getSpellingLocation
		case LocationKind::Spelling:
			clang_getSpellingLocation(location, &file, &line, &column, &offset);
			break;

		// Expansion - use clang_getExpansionLocation
		case LocationKind::Expansion:
			clang_getExpansionLocation(location, &file, &line, &column, &offset);
			break;

		// File - use clang_getFileLocation
		case LocationKind::File:
			clang_getFileLocation(location, &file, &line, &column, &offset);
			break;

		// Presumed - use clang_getPresumedLocation (no offset is returned)
		case LocationKind::Presumed:
			clang_getPresumedLocation(location, &presumedfile, &line, &column);
			file = clang_getFile(location.TranslationUnit, clang_getCString(presumedfile));
			clang_disposeString(presumedfile);
			break;

		// Default to throwing an exception
		default: throw gcnew ArgumentOutOfRangeException("kind");
	}

	// Store the extracted file, line, column, and offset values in the members
	m_line = static_cast<int>(line);
	m_column = static_cast<int>(column);
	m_offset = static_cast<int>(offset);
	m_file = local::File::Create(handle->Owner, handle->TranslationUnit, file);
}

//---------------------------------------------------------------------------
// Location::operator == (static)

bool Location::operator==(Location^ lhs, Location^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	// clang_equalLocations returns non-zero if the locations are the same
	return (clang_equalLocations(SourceLocationHandle::Reference(lhs->m_handle), SourceLocationHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Location::operator != (static)

bool Location::operator!=(Location^ lhs, Location^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	// clang_equalLocations returns zero if the locations are different
	return (clang_equalLocations(SourceLocationHandle::Reference(lhs->m_handle), SourceLocationHandle::Reference(rhs->m_handle)) == 0);
}

//---------------------------------------------------------------------------
// Location::default[LocationKind]::get
//
// Gets a specific kind of location instance

Location^ Location::default::get(LocationKind kind)
{
	// Due to the use of kind as an array index, it has to be within boundaries
	if((kind < LocationKind::Spelling) || (kind > LocationKind::Presumed)) throw gcnew ArgumentOutOfRangeException("kind");

	// Check for a cached instance of the Location first
	Location^ cached = m_cache[static_cast<int>(kind)];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;
	
	// Create a new location and cache it to prevent multiple creations
	CXSourceLocation location = SourceLocationHandle::Reference(m_handle);
	m_cache[static_cast<int>(kind)] = Create(m_handle->Owner, m_handle->TranslationUnit, location, kind);
	return m_cache[static_cast<int>(kind)];
}

//---------------------------------------------------------------------------
// Location::Column::get
//
// Gets the file column number represented by this location

int Location::Column::get(void)
{
	return m_column;
}

//---------------------------------------------------------------------------
// Location::Cursor::get
//
// Gets the cursor at this location within the translation unit

local::Cursor^ Location::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr)) {

		SourceLocationHandle::Reference location(m_handle);
		m_cursor = local::Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursor(location.TranslationUnit, location));
	}

	return m_cursor;
}

//---------------------------------------------------------------------------
// Location::Create (internal, static)
//
// Creates a new Location instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	location	- Unmanaged CXSourceLocation instance
//	kind		- Kind of location being created

Location^ Location::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceLocation location, LocationKind kind)
{
	return gcnew Location(gcnew SourceLocationHandle(owner, transunit, location), kind);
}

//---------------------------------------------------------------------------
// Location::Equals
//
// Compares this Location instance to another Location instance
//
// Arguments:
//
//	rhs		- Right-hand Location instance to compare against

bool Location::Equals(Location^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// Location::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool Location::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a Location instance
	Location^ rhsref = dynamic_cast<Location^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// Location::File::get
//
// Gets the file associated with this location

local::File^ Location::File::get(void)
{
	return m_file;
}

//---------------------------------------------------------------------------
// Location::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int Location::GetHashCode(void)
{
	CXSourceLocation location = SourceLocationHandle::Reference(m_handle);

	// Calculate the hash value from the components of the unmanaged structure, these are the
	// fields involved for determining equality via clang_equalLocations
	return intptr_t(location.ptr_data[0]).GetHashCode() ^ intptr_t(location.ptr_data[1]).GetHashCode() ^
		location.int_data.GetHashCode();
}

//---------------------------------------------------------------------------
// Location::IsFromMainFile::get
//
// Gets a flag indicating if this location is in the main translation unit file

bool Location::IsFromMainFile::get(void)
{
	return (clang_Location_isFromMainFile(SourceLocationHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Location::IsInSystemHeader::get
//
// Gets a flag indicating if this location is in a system header file

bool Location::IsInSystemHeader::get(void)
{
	return (clang_Location_isInSystemHeader(SourceLocationHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Location::IsNull (static)
//
// Determines if the location is null
//
// Arguments:
//
//	rhs		- Location instance to be checked for null

bool Location::IsNull(Location^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (clang_equalLocations(SourceLocationHandle::Reference(rhs->m_handle), clang_getNullLocation()) != 0);
}

//---------------------------------------------------------------------------
// Location::Kind::get
//
// Gets the kind of location represented by this instance

LocationKind Location::Kind::get(void)
{
	return m_kind;
}

//---------------------------------------------------------------------------
// Location::Line::get
//
// Gets the file line number represented by this location

int Location::Line::get(void)
{
	return m_line;
}

//---------------------------------------------------------------------------
// Location::Offset::get
//
// Gets the file offset represented by this location

int Location::Offset::get(void)
{
	return m_offset;
}

//---------------------------------------------------------------------------
// Location::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Location::ToString(void)
{
	// [filename] ([line], [column])
	return String::Format("{0} ({1}, {2})", File->ToString(), m_line, m_column);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
