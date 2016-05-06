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
#include "PlatformAvailability.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// PlatformAvailability Constructor (private)
//
// Arguments:
//
//	availability	- Pointer to the CXPlatformAvailability structure

PlatformAvailability::PlatformAvailability(const CXPlatformAvailability& availability)
{
	// The contents of the CXPlatformAvailability have to be copied out, do
	// not dispose of the embedded CXStrings, clang_disposeCXPlatformAvailability does that
	m_platform = StringUtil::ToString(availability.Platform);
	m_introduced = ConvertVersion(availability.Introduced);
	m_deprecated = ConvertVersion(availability.Deprecated);
	m_obsoleted = ConvertVersion(availability.Obsoleted);
	m_unavailable = (availability.Unavailable != 0);
	m_message = StringUtil::ToString(availability.Message);
}

//---------------------------------------------------------------------------
// PlatformAvailability::ConvertVersion (private, static)
//
// Converts a CXVersion structure into a Version object
//
// Arguments:
//
//	version			- CXVersion instance to be converted into a Version

Version^ PlatformAvailability::ConvertVersion(const CXVersion& version)
{
	// Both CXVersion and Version use -1 for undefined version fields, but you
	// cannot pass a negative number to the Version constructor.  Version also
	// does not allow for -1 as the minor version number, it has to be zero

	// 0.0
	if(version.Major < 0) return gcnew Version();

	// MAJOR.0
	if(version.Minor < 0) return gcnew Version(version.Major, 0);

	// MAJOR.MINOR
	if(version.Subminor < 0) return gcnew Version(version.Major, version.Minor);

	// MAJOR.MINOR.SUBMINOR
	return gcnew Version(version.Major, version.Minor, version.Subminor);
}

//---------------------------------------------------------------------------
// PlatformAvailability::Create (internal, static)
//
// Creates a new PlatformAvailability instance
//
// Arguments:
//
//	availability	- Pointer to the CXPlatformAvailability structure

PlatformAvailability^ PlatformAvailability::Create(const CXPlatformAvailability& availability)
{
	return gcnew PlatformAvailability(availability);
}

//---------------------------------------------------------------------------
// PlatformAvailability::Deprecated::get
//
// Gets the version where the entity was deprecated

Version^ PlatformAvailability::Deprecated::get(void)
{
	return m_deprecated;
}

//---------------------------------------------------------------------------
// PlatformAvailability::Introduced::get
//
// Gets the version where the entity was introduced

Version^ PlatformAvailability::Introduced::get(void)
{
	return m_introduced;
}

//---------------------------------------------------------------------------
// PlatformAvailability::Message::get
//
// Gets an optional suggested replacement message

String^ PlatformAvailability::Message::get(void)
{
	return m_message;
}

//---------------------------------------------------------------------------
// PlatformAvailability::Obsoleted::get
//
// Gets the version where the entity was obsoleted

Version^ PlatformAvailability::Obsoleted::get(void)
{
	return m_obsoleted;
}

//---------------------------------------------------------------------------
// PlatformAvailability::Platform::get
//
// Gets the description of the platform

String^ PlatformAvailability::Platform::get(void)
{
	return m_platform;
}

//---------------------------------------------------------------------------
// PlatformAvailability::Unavailable::get
//
// Gets a flag indicating if the entity is unavailable on the platform

bool PlatformAvailability::Unavailable::get(void)
{
	return m_unavailable;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
