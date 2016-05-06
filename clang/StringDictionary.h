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

#ifndef __STRINGDICTIONARY_H_
#define __STRINGDICTIONARY_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class StringDictionary
//
// Implements an IReadOnlyDictionary<> based enumerable collection of strings
//---------------------------------------------------------------------------

public ref class StringDictionary : public IReadOnlyDictionary<String^, String^>
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ContainsKey
	//
	// Determines whether the dictionary contains an element that has the specified key
	virtual bool ContainsKey(String^ key);

	// GetEnumerator
	//
	// Returns a generic IEnumerator<T> for the member collection
	virtual IEnumerator<KeyValuePair<String^, String^>>^ GetEnumerator(void);

	// TryGetValue
	//
	// Gets the value that is associated with the specified key
	virtual bool TryGetValue(String^ key, [OutAttribute] String^% value);

	//-----------------------------------------------------------------------
	// Properties

	// default[String^]
	//
	// Gets the element at the specified index in the read-only list
	property String^ default[String^] 
	{
		virtual String^ get(String^ key);
	}

	// Count
	//
	// Gets the number of elements in the collection
	property int Count
	{
		virtual int get();
	}

	// Keys
	//
	// Gets an enumerable collection that contains the keys in the dictionary
	property IEnumerable<String^>^ Keys
	{
		virtual IEnumerable<String^>^ get(void);
	}

	// Values
	//
	// Gets an enumerable collection that contains the values in the dictionary
	property IEnumerable<String^>^ Values
	{
		virtual IEnumerable<String^>^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new StringDictionary instance
	static StringDictionary^ Create(Dictionary<String^, String^>^ dictionary);

private:

	// Instance Constructor
	//
	StringDictionary(Dictionary<String^, String^>^ dictionary);

	//-----------------------------------------------------------------------
	// Private Member Functions

	// GetEnumerator (IEnumerable)
	//
	// Returns a non-generic IEnumerator for the member collection
	virtual System::Collections::IEnumerator^ IEnumerable_GetEnumerator(void) sealed = System::Collections::IEnumerable::GetEnumerator;
		
	//-----------------------------------------------------------------------
	// Member Variables

	Dictionary<String^, String^>^	m_dictionary;	// Contained Dictionary<>
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __RESOURCEUSAGEDICTIONARY_H_
