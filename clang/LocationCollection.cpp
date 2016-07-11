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
#include "LocationCollection.h"

#include "Location.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// LocationCollection Constructor
//
// Arguments:
//
//	locations	- List<> containing the locations

LocationCollection::LocationCollection(List<Location^>^ locations) : m_locations(locations)
{
	if(Object::ReferenceEquals(locations, nullptr)) throw gcnew ArgumentNullException("locations");
}

//---------------------------------------------------------------------------
// LocationCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Location^ LocationCollection::default::get(int index)
{
	return m_locations[index];
}

//---------------------------------------------------------------------------
// LocationCollection::Count::get
//
// Gets the number of elements in the collection

int LocationCollection::Count::get(void)
{
	return m_locations->Count;
}

//---------------------------------------------------------------------------
// LocationCollection::Create (static, internal)
//
// Creates a new LocationCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	transunit		- Parent TranslationUnitHandle instance
//	locations		- Pointer to an array of CXSourceLocation structures
//	count			- Number of CXSourceLocation structures in the array

LocationCollection^ LocationCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceLocation* locations, unsigned int count)
{
	// If the provided CXSourceLocation array is null or zero-length, use the empty collection
	if((locations == __nullptr) || (count == 0)) return LocationCollection::Empty;

	// Create a new List<> to hold all of the generated Location instances
	List<Location^>^ locationlist = gcnew List<Location^>(count);

	// Load the List<> with new Location objects for each element of the array
	for(unsigned int index = 0; index < count; index++) locationlist->Add(Location::Create(owner, transunit, locations[index], LocationKind::Spelling));
	return gcnew LocationCollection(locationlist);
}

//---------------------------------------------------------------------------
// LocationCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Location^>^ LocationCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<Location^>(this);
}

//---------------------------------------------------------------------------
// LocationCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ LocationCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
