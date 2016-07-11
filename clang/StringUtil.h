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

#ifndef __STRINGUTIL_H_
#define __STRINGUTIL_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class StringUtil (internal)
//
// String manipulation helper functions
//---------------------------------------------------------------------------

ref class StringUtil abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// FreeCharPointer (static)
	//
	// Releases memory allocated by StringToCharPointer
	static char* FreeCharPointer(char* psz);

	// FreeCharPointerArray (static)
	//
	// Releases unmanaged string array allocated by EnumerableStringToCharPointerArray
	static char** FreeCharPointerArray(char** strings);

	// ToCharPointer
	//
	// Converts a System::String into an unmanaged C-style string
	static char* ToCharPointer(String^ string, UINT codepage);

	// ToCharPointerArray (static)
	//
	// Converts an enumerable managed string collection into an unmanaged array
	static char** ToCharPointerArray(IEnumerable<String^>^ strings);
	static char** ToCharPointerArray(IEnumerable<String^>^ strings, int* numstrings);
	static char** ToCharPointerArray(IEnumerable<String^>^ strings, UINT codepage);
	static char** ToCharPointerArray(IEnumerable<String^>^ strings, UINT codepage, int* numstrings);

	// ToString (static)
	//
	// Converts an unmanaged C-style string into a managed String
	static String^ ToString(const char* psz, UINT codepage);
	static String^ ToString(const char* psz, size_t cb, UINT codepage);

	// ToString (static)
	//
	// Converts a CXString into System::String instances
	static String^ ToString(CXString&& string);
	static String^ ToString(const CXString& string);
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __STRINGUTIL_H_
