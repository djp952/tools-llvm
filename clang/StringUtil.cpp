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
#include "StringUtil.h"

#include <vcclr.h>					// PtrToStringChars

using namespace System;
using namespace System::ComponentModel;
using namespace System::Linq;
using namespace System::Text;

#pragma warning(push, 4)			// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// StringUtil::FreeCharPointer
//
// Releases a C-style string allocated by ToCharPointer
//
// Arguments:
//
//	psz			- Unmanaged string pointer

char* StringUtil::FreeCharPointer(char* psz)
{
	if(psz != __nullptr) delete[] psz;
	return __nullptr;
}

//---------------------------------------------------------------------------
// StringUtil::FreeCharPointerArray (static)
//
// Releases unmanaged string array allocated by ToCharPointerArray
//
// Arguments:
//
//	strings		- Pointer returned from ToCharPointerArray

char** StringUtil::FreeCharPointerArray(char** strings)
{
	if(strings == __nullptr) return __nullptr;

	char** rgsz = strings;
	while(*rgsz) FreeCharPointer(*rgsz++);
	delete[] strings;

	return __nullptr;
}

//---------------------------------------------------------------------------
// StringUtil::ToCharPointer
//
// Converts a System::String into a C-style string using the specified code page
//
// Arguments:
//
//	string		- String to be converted
//	codepage	- Codepage to be used during conversion (CP_ACP, CP_UTF8, etc)

char* StringUtil::ToCharPointer(String^ string, UINT codepage)
{
	char*				psz;				// Pointer to be returned to the caller

	if(Object::ReferenceEquals(string, nullptr)) return __nullptr;

	// Pin the System::String so it can be passed around directly
	pin_ptr<const wchar_t> pinstring = PtrToStringChars(string);

	// Determine the number of characters required to convert the string
	int cch = WideCharToMultiByte(codepage, 0, pinstring, -1, __nullptr, 0, __nullptr, __nullptr);
	if(cch == 0) throw gcnew Win32Exception(GetLastError());

	// Allocate the required string space from the unmanaged HGLOBAL heap
	try { psz = new char[cch]; }
	catch(Exception^) { throw gcnew OutOfMemoryException(); }

	// Convert the Unicode System::String to the specified codepage
	WideCharToMultiByte(codepage, 0, pinstring, -1, psz, cch, __nullptr, __nullptr);
	return psz;
}

//---------------------------------------------------------------------------
// StringUtil::ToCharPointerArray (static)
//
// Converts an enumerable managed string collection into an unmanaged array
//
// Arguments:
//
//	strings		- Enumerable collection of System::String instances

char** StringUtil::ToCharPointerArray(IEnumerable<String^>^ strings)
{
	return StringUtil::ToCharPointerArray(strings, CP_ACP, __nullptr);
}

//---------------------------------------------------------------------------
// StringUtil::ToCharPointerArray (static)
//
// Converts an enumerable managed string collection into an unmanaged array
//
// Arguments:
//
//	strings		- Enumerable collection of System::String instances
//	numstrings	- Optional, receives the number of strings in the generated array

char** StringUtil::ToCharPointerArray(IEnumerable<String^>^ strings, int* numstrings)
{
	return StringUtil::ToCharPointerArray(strings, CP_ACP, numstrings);
}

//---------------------------------------------------------------------------
// StringUtil::ToCharPointerArray (static)
//
// Converts an enumerable managed string collection into an unmanaged array
//
// Arguments:
//
//	strings		- Enumerable collection of System::String instances
//	codepage	- Codepage to use when converting the strings (CP_ACP, CP_UTF8, etc)

char** StringUtil::ToCharPointerArray(IEnumerable<String^>^ strings, UINT codepage)
{
	return StringUtil::ToCharPointerArray(strings, codepage, __nullptr);
}

//---------------------------------------------------------------------------
// StringUtil::ToCharPointerArray (static)
//
// Converts an enumerable managed string collection into an unmanaged array
//
// Arguments:
//
//	strings		- Enumerable collection of System::String instances
//	codepage	- Codepage to use when converting the strings (CP_ACP, CP_UTF8, etc)
//	numstrings	- Optional, receives the number of strings in the generated array

char** StringUtil::ToCharPointerArray(IEnumerable<String^>^ strings, UINT codepage, int* numstrings)
{
	char**					rgsz;				// Array to be returned to the caller

	if(numstrings != __nullptr) *numstrings = 0;
	if(Object::ReferenceEquals(strings, nullptr)) return __nullptr;

	// Determine how many strings are in the enumerable collection
	int numpointers = Enumerable::Count(strings);
	if(numstrings != __nullptr) *numstrings = numpointers;

	// Allocate the outer string array
	try { rgsz = new char*[numpointers + 1]; }
	catch(Exception^) { throw gcnew OutOfMemoryException(); }

	// Iterate over the managed strings in the enumerable and allocate them as unmanaged strings,
	// setting the final entry to a NULL pointer
	int index = 0;
	for each(String^ string in strings) rgsz[index++] = ToCharPointer(string, codepage);
	rgsz[index] = __nullptr;

	return rgsz;
}

//---------------------------------------------------------------------------
// StringUtil::ToString
//
// Converts an unmanaged ANSI, UTF-7 or UTF-8 string into a managed String
//
// Arguments:
//
//	psz			- Pointer to the unmanaged string to convert
//	codepage	- Codepage to use for conversion (CP_ACP, CP_UTF7, CP_UTF8)

String^ StringUtil::ToString(const char* psz, UINT codepage)
{
	if(psz == __nullptr) return String::Empty;
	else if(codepage == CP_ACP) return gcnew String(psz);

	return ToString(psz, strlen(psz), codepage);
}

//---------------------------------------------------------------------------
// StringUtil::ToString
//
// Converts an unmanaged ANSI, UTF-7 or UTF-8 string into a managed String
//
// Arguments:
//
//	psz			- Pointer to the unmanaged string to convert
//	cb			- Size of the unmanaged string buffer, in bytes
//	codepage	- Codepage to use for conversion (CP_ACP, CP_UTF7, CP_UTF8)

// This version requires .NET Framework 4.6 or higher, but as the only thing
// in the entire project that does need it, I opted to replace it with something
// that will work with .NET Framework 4.5 (see below)
//
//String^ StringUtil::ToString(const char* psz, size_t cb, UINT codepage)
//{
//	if((psz == __nullptr) || (cb == 0)) return String::Empty;
//
//	// size_t can exceed int, which is the required input to Encoding::GetString()
//	if(cb > static_cast<size_t>(Int32::MaxValue)) throw gcnew OverflowException();
//
//	unsigned char* pb = reinterpret_cast<unsigned char*>(const_cast<char*>(psz));
//
//	// This function only allows CP_ACP, CP_UTF7 and CP_UTF8 code pages
//	if(codepage == CP_UTF7) return Encoding::UTF7->GetString(pb, static_cast<int>(cb));
//	else if(codepage == CP_UTF8) return Encoding::UTF8->GetString(pb, static_cast<int>(cb));
//	else throw gcnew ArgumentException("Invalid code page");
//}

String^ StringUtil::ToString(const char* psz, size_t cb, UINT codepage)
{
	if((psz == __nullptr) || (cb == 0)) return String::Empty;

	// size_t can exceed int, which is the required input to Encoding::GetString()
	if(cb > static_cast<size_t>(Int32::MaxValue)) throw gcnew OverflowException();

	// Determine the length of the required wide character string buffer
	int buffercch = MultiByteToWideChar(codepage, 0, psz, static_cast<int>(cb), __nullptr, 0);

	// Allocate the buffer as a managed array and pin it
	array<wchar_t>^ buffer = gcnew array<wchar_t>(buffercch);
	pin_ptr<wchar_t> pin = &buffer[0];

	// Convert the input string into Unicode and return a new String instance
	MultiByteToWideChar(codepage, 0, psz, static_cast<int>(cb), pin, buffercch);
	return gcnew String(buffer, 0, buffercch);
}

//---------------------------------------------------------------------------
// StringUtil::ToString
//
// Converts a CXString rvalue reference into a managed string and disposes it
//
// Arguments:
//
//	string		- CXString rvalue reference

String^ StringUtil::ToString(CXString&& string)
{
	// Convert the string into a System::String and then dispose it
	try { return ToString(clang_getCString(string), CP_UTF8); }
	finally { clang_disposeString(string); memset(&string, 0, sizeof(CXString)); }
}

//---------------------------------------------------------------------------
// StringUtil::ToString
//
// Converts a CXString lvalue into a managed string -- does not dispose it
//
// Arguments:
//
//	string		- CXString lvalue -- will not be disposed of

String^ StringUtil::ToString(const CXString& string)
{
	return ToString(clang_getCString(string), CP_UTF8);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
