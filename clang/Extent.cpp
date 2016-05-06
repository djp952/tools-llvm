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
#include "Extent.h"

#include "Location.h"
#include "LocationKind.h"
#include "TokenCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Extent Static Constructor (private)

static Extent::Extent()
{
	Null = Extent::Create(TranslationUnitHandle::Null, TranslationUnitHandle::Null, clang_getNullRange());
}

//---------------------------------------------------------------------------
// Extent Constructor (private)
//
// Arguments:
//
//	handle		- Underlying SourceRangeHandle instance

Extent::Extent(SourceRangeHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Extent::operator == (static)

bool Extent::operator==(Extent^ lhs, Extent^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	// clang_equalRanges returns non-zero if the locations are the same
	return (clang_equalRanges(SourceRangeHandle::Reference(lhs->m_handle), SourceRangeHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Extent::operator != (static)

bool Extent::operator!=(Extent^ lhs, Extent^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	// clang_equalRanges returns zero if the locations are different
	return (clang_equalRanges(SourceRangeHandle::Reference(lhs->m_handle), SourceRangeHandle::Reference(rhs->m_handle)) == 0);
}

//---------------------------------------------------------------------------
// Extent::Contains (internal)
//
// Determines if the specified source location is within this extent
//
// Arguments:
//
//	location		- Source location to check

bool Extent::Contains(const CXSourceLocation& location)
{
	SourceRangeHandle::Reference	extent(m_handle);		// Unwrap the underlying safe handle
	CXFile							files[3];				// CXFile instances
	unsigned int					offsets[3];				// Location offsets within the files

	// Get the null location for comparison purposes
	CXSourceLocation null = clang_getNullLocation();

	// Get the start and end locations for this extent
	CXSourceLocation start = clang_getRangeStart(extent);
	CXSourceLocation end = clang_getRangeEnd(extent);

	// If any of the locations are null, return false
	if(clang_equalLocations(start, null) != 0) return false;
	if(clang_equalLocations(end, null) != 0) return false;
	if(clang_equalLocations(location, null) != 0) return false;

	// Get the file instances and offsets for all the locations
	clang_getSpellingLocation(start, &files[0], __nullptr, __nullptr, &offsets[0]);
	clang_getSpellingLocation(end, &files[1], __nullptr, __nullptr, &offsets[1]);
	clang_getSpellingLocation(location, &files[2], __nullptr, __nullptr, &offsets[2]);

	// If any of the locations are in different files, return false
	if(clang_File_isEqual(files[0], files[1]) == 0) return false;
	if(clang_File_isEqual(files[1], files[2]) == 0) return false;

	// Same files, use the offsets within those files to determine result
	return ((offsets[0] <= offsets[2]) && (offsets[1] >= offsets[2]));
}
	
//---------------------------------------------------------------------------
// Extent::Create (internal, static)
//
// Creates a new Extent instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	extent		- Unmanaged CXSourceRange instance to be wrapped

Extent^ Extent::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceRange extent)
{
	return gcnew Extent(gcnew SourceRangeHandle(owner, transunit, extent));
}

//---------------------------------------------------------------------------
// Extent::End
//
// Gets the location of the end of the extent

Location^ Extent::End::get(void)
{
	if(Object::ReferenceEquals(m_end, nullptr))
		m_end = Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getRangeEnd(SourceRangeHandle::Reference(m_handle)), LocationKind::Spelling);
	
	return m_end;
}

//---------------------------------------------------------------------------
// Extent::Equals
//
// Compares this Extent instance to another Extent instance
//
// Arguments:
//
//	rhs		- Right-hand Extent instance to compare against

bool Extent::Equals(Extent^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// Extent::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool Extent::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a Extent instance
	Extent^ rhsref = dynamic_cast<Extent^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// Extent::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int Extent::GetHashCode(void)
{
	CXSourceRange extent = SourceRangeHandle::Reference(m_handle);

	// Calculate the hash value from the components of the unmanaged structure, these are the
	// fields involved for determining equality via clang_equalRanges
	return intptr_t(extent.ptr_data[0]).GetHashCode() ^ intptr_t(extent.ptr_data[1]).GetHashCode() ^
		extent.begin_int_data.GetHashCode() ^ extent.end_int_data.GetHashCode();
}

//---------------------------------------------------------------------------
// Extent::GetTokens
//
// Tokenizes the extent
//
// Arguments:
//
//	NONE

TokenCollection^ Extent::GetTokens(void)
{
	CXToken*						tokens = __nullptr;		// Unmanaged token buffer
	unsigned int					numtokens = 0;			// Number of unmanaged tokens
	SourceRangeHandle::Reference	extent(m_handle);		// Unwrap the safe handle

	// Tokenize the entire extent and generate a new TokenCollection from it
	clang_tokenize(extent.TranslationUnit, extent, &tokens, &numtokens);

#pragma message("CLANG WORKAROUND: https://llvm.org/bugs/show_bug.cgi?id=9069")
	// Clang may return one too many tokens, check the last one's location.  Removing it from the
	// collection size won't cause it to leak, clang_disposeTokens doesn't actually check the count
	if((numtokens > 0) && (!Contains(clang_getTokenLocation(extent.TranslationUnit, tokens[numtokens - 1])))) numtokens--;
	return TokenCollection::Create(m_handle->Owner, m_handle->TranslationUnit, std::move(tokens), numtokens);
}

//---------------------------------------------------------------------------
// Extent::IsNull (static)
//
// Determines if the extent is null
//
// Arguments:
//
//	rhs		- Extent to be checked for null

bool Extent::IsNull(Extent^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (clang_Range_isNull(SourceRangeHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Extent::Start
//
// Gets the location of the start of the extent

Location^ Extent::Start::get(void)
{
	if(Object::ReferenceEquals(m_start, nullptr))
		m_start = Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getRangeStart(SourceRangeHandle::Reference(m_handle)), LocationKind::Spelling);

	return m_start;
}

//---------------------------------------------------------------------------
// Extent::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Extent::ToString(void)
{
	return ToString(LocationKind::Spelling);
}

//---------------------------------------------------------------------------
// Extent::ToString
//
// Generates a string representation based on specific location types
//
// Arguments:
//
//	kind		- LocationKind to use when accessing the start and end locations

String^ Extent::ToString(LocationKind kind)
{
	// [file] ([line], [column]) - [file] ([line], [column])
	return String::Format("{0} - {1}", this->Start[kind]->ToString(), this->End[kind]->ToString());
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
