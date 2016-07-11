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

#ifndef __PLATFORMAVAILABILITY_H_
#define __PLATFORMAVAILABILITY_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class PlatformAvailability
//
// Represents the platform availability of a cursor entity
//---------------------------------------------------------------------------

public ref class PlatformAvailability
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// Deprecated
	//
	// Gets the version where the entity was deprecated
	property Version^ Deprecated
	{
		Version^ get(void);
	}

	// Introduced
	//
	// Gets the version where the entity was introduced
	property Version^ Introduced
	{
		Version^ get(void);
	}

	// Message
	//
	// Gets an optional suggested replacement message
	property String^ Message
	{
		String^ get(void);
	}

	// Obsoleted
	//
	// Gets the version where the entity was obsoleted
	property Version^ Obsoleted
	{
		Version^ get(void);
	}

	// Platform
	//
	// Gets the description of the platform
	property String^ Platform
	{
		String^ get(void);
	}

	// Unavailable
	//
	// Gets a flag indicating if the entity is unavailable on the platform
	property bool Unavailable
	{
		bool get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new PlatformAvailability instance
	static PlatformAvailability^ Create(const CXPlatformAvailability& availability);

private:

	// Instance Constructor
	//
	PlatformAvailability(const CXPlatformAvailability& availability);

	//-----------------------------------------------------------------------
	// Private Member Functions

	// ConvertVersion
	//
	// Converts a CXVersion structure into a Version object
	static Version^ ConvertVersion(const CXVersion& version);

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_platform;				// Platform name
	Version^				m_introduced;			// Version where introduced
	Version^				m_deprecated;			// Version where deprecated
	Version^				m_obsoleted;			// Version where obsoleted
	bool					m_unavailable;			// Flag if unavailable
	String^					m_message;				// Optional message text
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __PLATFORMAVAILABILITY_H_
