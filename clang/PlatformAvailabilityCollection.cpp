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
#include "PlatformAvailabilityCollection.h"

#include "PlatformAvailability.h"
#include "ReadOnlyListEnumerator.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection Constructor (private)
//
// Arguments:
//
//	collection		- Backing List<> collection instance
//	deprecated		- Flag if the entity is always deprecated
//	deprecatedmsg	- Message associated with the deprecated flag
//	unavailable		- Flag if the entity is always unavailable
//	unavailablemsg	- Message associated with the unavailable flag

PlatformAvailabilityCollection::PlatformAvailabilityCollection(List<PlatformAvailability^>^ collection, bool deprecated, 
	String^ deprecatedmsg, bool unavailable, String^ unavailablemsg) : m_collection(collection), m_deprecated(deprecated),
	m_deprecatedmsg(deprecatedmsg), m_unavailable(unavailable), m_unavailablemsg(unavailablemsg)
{
	if(Object::ReferenceEquals(collection, nullptr)) throw gcnew ArgumentNullException("collection");
	if(Object::ReferenceEquals(deprecatedmsg, nullptr)) throw gcnew ArgumentNullException("deprecatedmsg");
	if(Object::ReferenceEquals(unavailablemsg, nullptr)) throw gcnew ArgumentNullException("unavailablemsg");
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

PlatformAvailability^ PlatformAvailabilityCollection::default::get(int index)
{
	return m_collection[index];
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::AlwaysDeprecated::get
//
// Flag if the entity is deprecated on all platforms

bool PlatformAvailabilityCollection::AlwaysDeprecated::get(void)
{
	return m_deprecated;
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::AlwaysDeprecatedMessage::get
//
// Message associated with the always deprecated flag

String^ PlatformAvailabilityCollection::AlwaysDeprecatedMessage::get(void)
{
	return m_deprecatedmsg;
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::AlwaysUnavailable::get
//
// Flag if the entity is unavailable on all platforms

bool PlatformAvailabilityCollection::AlwaysUnavailable::get(void)
{
	return m_unavailable;
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::AlwaysUnavailableMessage::get
//
// Message associated with the always unavailable flag

String^ PlatformAvailabilityCollection::AlwaysUnavailableMessage::get(void)
{
	return m_unavailablemsg;
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::Count::get
//
// Gets the number of elements in the collection

int PlatformAvailabilityCollection::Count::get(void)
{
	return m_collection->Count;
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::Create (static, internal)
//
// Creates a new PlatformAvailabilityCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	cursor		- Unmanaged CXCursor instance

PlatformAvailabilityCollection^ PlatformAvailabilityCollection::Create(SafeHandle^ owner, CXCursor cursor)
{
	int								deprecated;			// Flag if the entity is deprecated
	CXString						deprecatedmsg;		// Deprecation message text
	int								unavailable;		// Flag if the entity is unavailable
	CXString						unavailablemsg;		// Unavailability message text
	List<PlatformAvailability^>^	collection;			// Generated collection 

	UNREFERENCED_PARAMETER(owner);

	// Get the general deprecation/unavailability flags and the number of specific platforms
	int numplatforms = clang_getCursorPlatformAvailability(cursor, &deprecated, &deprecatedmsg,
		&unavailable, &unavailablemsg, __nullptr, 0);

	// Convert the CXStrings into managed strings to get them disposed of immediately
	String^ deprecatedstr = StringUtil::ToString(std::move(deprecatedmsg));
	String^ unavailablestr = StringUtil::ToString(std::move(unavailablemsg));

	// Generate the collection that will hold the PlatformAvailability instances
	collection = gcnew List<PlatformAvailability^>(numplatforms);

	// The way the API works here is different and necessitates a snapshot collection
	// rather than a 'live' collection.  We have to allocate the array itself, but each
	// element within the array has to be disposed of individually before releasing that
	// outer array.  Creating a snapshot collection leaves all of the unmanaged memory 
	// work here in this function
	if(numplatforms > 0) {

		// Allocate and load a local array of CXPlatformAvailability structures
		CXPlatformAvailability* availabilityset = new CXPlatformAvailability[numplatforms];

		try {
		
			// Load the array of platform-specific availability to generate the collection
			clang_getCursorPlatformAvailability(cursor, __nullptr, __nullptr, __nullptr, __nullptr, availabilityset, numplatforms);

			// Iterate over each returned element and convert into a PlatformAvailability instance
			for(int index = 0; index < numplatforms; index++) {
				
				try { collection->Add(PlatformAvailability::Create(availabilityset[index])); }
				finally { clang_disposeCXPlatformAvailability(&availabilityset[index]); }
			}
		}

		finally { delete[] availabilityset; }
	}

	// Construct and return a new PlatformAvailabilityCollection instance
	return gcnew PlatformAvailabilityCollection(collection, (deprecated != 0), deprecatedstr, (unavailable != 0), unavailablestr);
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<PlatformAvailability^>^ PlatformAvailabilityCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<PlatformAvailability^>(this);
}

//---------------------------------------------------------------------------
// PlatformAvailabilityCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ PlatformAvailabilityCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
