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

#ifndef __PLATFORMAVAILABILITYCOLLECTION_H_
#define __PLATFORMAVAILABILITYCOLLECTION_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class PlatformAvailability;

//---------------------------------------------------------------------------
// Class PlatformAvailabilityCollection
//
// Collection of PlatformAvailability objects that indicate the availability
// of a cursor entity on a specific platform
//---------------------------------------------------------------------------

public ref class PlatformAvailabilityCollection : public IReadOnlyList<PlatformAvailability^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<PlatformAvailability^>^ GetEnumerator(void);

	//-----------------------------------------------------------------------
	// Properties

	// default[int]
	//
	// Gets the element at the specified index in the read-only list
	property PlatformAvailability^ default[int] 
	{
		virtual PlatformAvailability^ get(int index);
	}

	// AlwaysDeprecated
	//
	// Flag if the entity is deprecated on all platforms
	property bool AlwaysDeprecated
	{
		bool get(void);
	}

	// AlwaysDeprecatedMessage
	//
	// Message associated with the always deprecated flag
	property String^ AlwaysDeprecatedMessage
	{
		String^ get(void);
	}

	// AlwaysUnavailable
	//
	// Flag if the entity is unavailable on all platforms
	property bool AlwaysUnavailable
	{
		bool get(void);
	}

	// AlwaysUnavailableMessage
	//
	// Message associated with the always unavailable flag
	property String^ AlwaysUnavailableMessage
	{
		String^ get(void);
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get();
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new PlatformAvailabilityCollection instance
	static PlatformAvailabilityCollection^ Create(SafeHandle^ owner, CXCursor cursor);

private:

	// Instance Constructor
	//
	PlatformAvailabilityCollection(List<PlatformAvailability^>^ collection, bool deprecated, 
		String^ deprecatedmsg, bool unavailable, String^ unavailablemsg);

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;
		
	//-----------------------------------------------------------------------
	// Member Variables

	List<PlatformAvailability^>^	m_collection;		// Underlying collection
	bool							m_deprecated;		// Flag if always deprecated
	String^							m_deprecatedmsg;	// Always deprecated message
	bool							m_unavailable;		// Flag if always unavailable
	String^							m_unavailablemsg;	// Always unavailable message
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __PLATFORMAVAILABILITYCOLLECTION_H_
