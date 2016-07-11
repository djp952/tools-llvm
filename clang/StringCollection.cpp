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
#include "StringCollection.h"

#include "ReadOnlyListEnumerator.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// StringCollection Constructor
//
// Arguments:
//
//	strings		- List<> containing the strings

StringCollection::StringCollection(List<String^>^ strings) : m_strings(strings)
{
	if(Object::ReferenceEquals(strings, nullptr)) throw gcnew ArgumentNullException("strings");
}

//---------------------------------------------------------------------------
// StringCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

String^ StringCollection::default::get(int index)
{
	return m_strings[index];
}

//---------------------------------------------------------------------------
// StringCollection::Count::get
//
// Gets the number of elements in the collection

int StringCollection::Count::get(void)
{
	return m_strings->Count;
}

//---------------------------------------------------------------------------
// StringCollection::Create (static, internal)
//
// Creates a new StringCollection instance
//
// Arguments:
//
//	stringset		- Pointer to an unmanaged CXStringSet instance

StringCollection^ StringCollection::Create(CXStringSet*&& stringset)
{
	// If a null pointer is provided, return an empty collection
	if(stringset == __nullptr) return StringCollection::Empty;

	try {

		// Create and populate a new List<> with managed string instances
		List<String^>^ strings = gcnew List<String^>();
		for(unsigned int index = 0; index < stringset->Count; index++) strings->Add(StringUtil::ToString(stringset->Strings[index]));

		return gcnew StringCollection(strings);
	}

	// This function takes ownership of the CXStringSet, always dispose of it
	finally { clang_disposeStringSet(stringset); stringset = __nullptr; }
}

//---------------------------------------------------------------------------
// StringCollection::Create (static, internal)
//
// Creates a new StringCollection instance
//
// Arguments:
//
//	strings			- Existing List<String> instance

StringCollection^ StringCollection::Create(List<String^>^ strings)
{
	if(Object::ReferenceEquals(strings, nullptr)) throw gcnew ArgumentNullException("strings");
	return gcnew StringCollection(strings);
}

//---------------------------------------------------------------------------
// StringCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<String^>^ StringCollection::GetEnumerator(void)
{
	return gcnew ReadOnlyListEnumerator<String^>(this);
}

//---------------------------------------------------------------------------
// StringCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ StringCollection::IEnumerable_GetEnumerator(void)
{
	return GetEnumerator();
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
