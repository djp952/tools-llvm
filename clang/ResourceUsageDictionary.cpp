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
#include "ResourceUsageDictionary.h"

#include "ResourceUsageKind.h"

#pragma warning(push, 4)					// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ResourceUsageDictionary Constructor (private)
//
// Arguments:
//
//	dictionary		- Dictionary of resource usage to be contained

ResourceUsageDictionary::ResourceUsageDictionary(Dictionary<ResourceUsageKind, unsigned long>^ dictionary) : 
	m_dictionary(dictionary), m_totalbytes(0)
{
	if(Object::ReferenceEquals(dictionary, nullptr)) throw gcnew ArgumentNullException("dictionary");

	// The total bytes is just the sum of all the individual resource usage amount values
	for each(unsigned long amount in dictionary->Values) m_totalbytes += amount;
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::Create (internal, static)
//
// Creates a new ResourceUsageDictionary instance
//
// Arguments:
//
//	resusage		- Unmanaged CXTUResourceUsage to take ownership of

ResourceUsageDictionary^ ResourceUsageDictionary::Create(CXTUResourceUsage&& resusage)
{
	try {

		// Convert the CXTUResourceUsage into a Dictionary<> instance
		Dictionary<ResourceUsageKind, unsigned long>^ dictionary = gcnew Dictionary<ResourceUsageKind, unsigned long>(resusage.numEntries);
		for(unsigned int index = 0; index < resusage.numEntries; index++)
			dictionary->Add(ResourceUsageKind(resusage.entries[index].kind), resusage.entries[index].amount);

		// Generate a new ResourceUsageDictionary instance around the Dictionary<>
		return gcnew ResourceUsageDictionary(dictionary);
	}

	// Always dispose of the CXTUResourceUsage information before leaving and reset
	// the provided rvalue reference, this function takes ownership of the structure
	finally { clang_disposeCXTUResourceUsage(resusage); memset(&resusage, 0, sizeof(CXTUResourceUsage)); }
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::default[ResourceUsageKind]::get
//
// Gets the element at the specified index in the read-only list

unsigned long ResourceUsageDictionary::default::get(ResourceUsageKind key)
{
	return m_dictionary->default[key];
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::ContainsKey
//
// Determines whether the dictionary contains an element that has the specified key
//
// Arguments:
//
//	key			- Key to locate in the dictionary

bool ResourceUsageDictionary::ContainsKey(ResourceUsageKind key)
{
	return m_dictionary->ContainsKey(key);
}
	
//---------------------------------------------------------------------------
// ResourceUsageDictionary::Count::get
//
// Gets the number of elements in the collection

int ResourceUsageDictionary::Count::get(void)
{
	return m_dictionary->Count;
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<KeyValuePair<ResourceUsageKind, unsigned long>>^ ResourceUsageDictionary::GetEnumerator(void)
{
	return m_dictionary->GetEnumerator();
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ ResourceUsageDictionary::IEnumerable_GetEnumerator(void)
{
	return m_dictionary->GetEnumerator();
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::Keys::get
//
// Gets an enumerable collection that contains the keys in the dictionary

IEnumerable<ResourceUsageKind>^ ResourceUsageDictionary::Keys::get(void)
{
	return m_dictionary->Keys;
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::TotalBytes
//
// Gets the total number of bytes used across all resource usage types

unsigned __int64 ResourceUsageDictionary::TotalBytes::get(void)
{
	return m_totalbytes;
}

//---------------------------------------------------------------------------
// ResourceUsageDictionary::TryGetValue
//
// Gets the value that is associated with the specified key
//
// Arguments:
//
//	key			- Key to locate in the dictionary
//	value		- [out] on success, contains the value associated with the key

bool ResourceUsageDictionary::TryGetValue(ResourceUsageKind key, [OutAttribute] unsigned long% value)
{
	return m_dictionary->TryGetValue(key, value);
}
	
//---------------------------------------------------------------------------
// ResourceUsageDictionary::Values::get
//
// Gets an enumerable collection that contains the values in the dictionary

IEnumerable<unsigned long>^ ResourceUsageDictionary::Values::get(void)
{
	return m_dictionary->Values;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
