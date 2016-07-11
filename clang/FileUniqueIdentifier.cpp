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
#include "FileUniqueIdentifier.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// FileUniqueIdentifier Constructor (internal)
//
// Arguments:
//
//	uniqueid		- Unmanaged CXFileUniqueID structure

FileUniqueIdentifier::FileUniqueIdentifier(const CXFileUniqueID& uniqueid)
{
	m_data = gcnew array<unsigned long long>(3) { uniqueid.data[0], uniqueid.data[1], uniqueid.data[2] };
	if(m_data == nullptr) throw gcnew OutOfMemoryException();
}

//---------------------------------------------------------------------------
// FileUniqueIdentifier::operator == (static)

bool FileUniqueIdentifier::operator==(FileUniqueIdentifier lhs, FileUniqueIdentifier rhs)
{
	// Compare all three data elements of the unique identifier
	return ((lhs.m_data[0] == rhs.m_data[0]) && (lhs.m_data[1] == rhs.m_data[1]) && (lhs.m_data[2] == rhs.m_data[2]));
}

//---------------------------------------------------------------------------
// FileUniqueIdentifier::operator != (static)

bool FileUniqueIdentifier::operator!=(FileUniqueIdentifier lhs, FileUniqueIdentifier rhs)
{
	// Compare all three data elements of the unique identifier
	return ((lhs.m_data[0] != rhs.m_data[0]) || (lhs.m_data[1] != rhs.m_data[1]) ||	(lhs.m_data[2] != rhs.m_data[2]));
}

//---------------------------------------------------------------------------
// FileUniqueIdentifier::Equals
//
// Compares this FileUniqueIdentifier to another FileUniqueIdentifier
//
// Arguments:
//
//	rhs		- Right-hand FileUniqueIdentifer to compare against

bool FileUniqueIdentifier::Equals(FileUniqueIdentifier rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// FileUniqueIdentifier::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool FileUniqueIdentifier::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a FileUniqueIdentifier instance
	FileUniqueIdentifier^ rhsref = dynamic_cast<FileUniqueIdentifier^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// FileUniqueIdentifer::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int FileUniqueIdentifier::GetHashCode(void)
{
	// Construct a hash value based on the three individual unique identifier fields
	return m_data[0].GetHashCode() ^ m_data[1].GetHashCode() ^ m_data[2].GetHashCode();
}

//---------------------------------------------------------------------------
// FileUniqueIdentifier::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ FileUniqueIdentifier::ToString(void)
{
	return String::Format("{0:X}:{1:X}:{2:X}", m_data[0], m_data[1], m_data[2]);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
