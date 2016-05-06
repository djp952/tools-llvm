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
#include "StringDictionary.h"

#pragma warning(push, 4)					// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// StringDictionary Constructor (private)
//
// Arguments:
//
//	dictionary		- Dictionary of strings to be contained

StringDictionary::StringDictionary(Dictionary<String^, String^>^ dictionary) : m_dictionary(dictionary)
{
	if(Object::ReferenceEquals(dictionary, nullptr)) throw gcnew ArgumentNullException("dictionary");
}

//---------------------------------------------------------------------------
// StringDictionary::Create (internal, static)
//
// Creates a new StringDictionary instance
//
// Arguments:
//
//	dictionary		- Dictionary<String^, String^> to contain

StringDictionary^ StringDictionary::Create(Dictionary<String^, String^>^ dictionary)
{
	return gcnew StringDictionary(dictionary);
}

//---------------------------------------------------------------------------
// StringDictionary::default[String^]::get
//
// Gets the element at the specified index in the read-only list

String^ StringDictionary::default::get(String^ key)
{
	return m_dictionary->default[key];
}

//---------------------------------------------------------------------------
// StringDictionary::ContainsKey
//
// Determines whether the dictionary contains an element that has the specified key
//
// Arguments:
//
//	key			- Key to locate in the dictionary

bool StringDictionary::ContainsKey(String^ key)
{
	return m_dictionary->ContainsKey(key);
}
	
//---------------------------------------------------------------------------
// StringDictionary::Count::get
//
// Gets the number of elements in the collection

int StringDictionary::Count::get(void)
{
	return m_dictionary->Count;
}

//---------------------------------------------------------------------------
// StringDictionary::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<KeyValuePair<String^, String^>>^ StringDictionary::GetEnumerator(void)
{
	return m_dictionary->GetEnumerator();
}

//---------------------------------------------------------------------------
// StringDictionary::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ StringDictionary::IEnumerable_GetEnumerator(void)
{
	return m_dictionary->GetEnumerator();
}

//---------------------------------------------------------------------------
// StringDictionary::Keys::get
//
// Gets an enumerable collection that contains the keys in the dictionary

IEnumerable<String^>^ StringDictionary::Keys::get(void)
{
	return m_dictionary->Keys;
}

//---------------------------------------------------------------------------
// StringDictionary::TryGetValue
//
// Gets the value that is associated with the specified key
//
// Arguments:
//
//	key			- Key to locate in the dictionary
//	value		- [out] on success, contains the value associated with the key

bool StringDictionary::TryGetValue(String^ key, [OutAttribute] String^% value)
{
	return m_dictionary->TryGetValue(key, value);
}
	
//---------------------------------------------------------------------------
// StringDictionary::Values::get
//
// Gets an enumerable collection that contains the values in the dictionary

IEnumerable<String^>^ StringDictionary::Values::get(void)
{
	return m_dictionary->Values;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
