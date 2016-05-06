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
#include "ExtentCollection.h"

#include "Extent.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// ExtentCollection Constructor
//
// Arguments:
//
//	extents		- List<> containing the extents

ExtentCollection::ExtentCollection(List<Extent^>^ extents) : m_extents(extents)
{
	if(Object::ReferenceEquals(extents, nullptr)) throw gcnew ArgumentNullException("extents");
}

//---------------------------------------------------------------------------
// ExtentCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Extent^ ExtentCollection::default::get(int index)
{
	return m_extents[index];
}

//---------------------------------------------------------------------------
// ExtentCollection::Count::get
//
// Gets the number of elements in the collection

int ExtentCollection::Count::get(void)
{
	return m_extents->Count;
}

//---------------------------------------------------------------------------
// ExtentCollection::Create (static, internal)
//
// Creates a new ExtentCollection instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	extents		- Unmanaged CXSourceRangeList instance (rvalue reference)

ExtentCollection^ ExtentCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXSourceRangeList*&& extents)
{
	// If the provided CXSourceRangeList is null, use the empty collection
	if(extents == __nullptr) return ExtentCollection::Empty;

	try {

		// Create a new List<> to hold all of the generated Extent instances
		List<Extent^>^ extentlist = gcnew List<Extent^>(extents->count);

		// Load the List<> with new Extent objects for each item provided in the CXSourceRangeList
		for(unsigned int index = 0; index < extents->count; index++) extentlist->Add(Extent::Create(owner, transunit, extents->ranges[index]));
		return gcnew ExtentCollection(extentlist);
	}

	// This function takes ownership of the CXSourceRangeList, always release it
	finally { clang_disposeSourceRangeList(extents); extents = __nullptr; }
}

//---------------------------------------------------------------------------
// ExtentCollection::Create (static, internal)
//
// Creates a new ExtentCollection instance
//
// Arguments:
//
//	extents			- Existing List<Extent^>^ instance

ExtentCollection^ ExtentCollection::Create(List<Extent^>^ extents)
{
	// This overload was added to support operations where a list of extents
	// has to be manually generated, just pass the existing List<> to the constructor
	return gcnew ExtentCollection(extents);
}

//---------------------------------------------------------------------------
// ExtentCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Extent^>^ ExtentCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<Extent^>(this);
}

//---------------------------------------------------------------------------
// ExtentCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ ExtentCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
