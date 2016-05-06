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
#include "EnumerateIncludedFileFunc.h"

#include "File.h"
#include "LocationCollection.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateIncludedFileFunc Constructor
//
// Arguments:
//
//	handle		- Parent translation unit handle
//	action		- Delegate to invoke with the currently enumerated inclusion

EnumerateIncludedFileFunc::EnumerateIncludedFileFunc(TranslationUnitHandle^ handle, Action<File^, LocationCollection^>^ action) : 
	m_handle(handle), m_action(action)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	// action can be null
}

//---------------------------------------------------------------------------
// EnumerateIncludedFileFunc::Exception::get
//
// Gets the optional exception instance from the enumeration operation

System::Exception^ EnumerateIncludedFileFunc::Exception::get(void)
{
	return m_exception;
}

//---------------------------------------------------------------------------
// EnumerateIncludedFileFunc::Exception::set
//
// Sets an optional exception instance from the enumeration operation

void EnumerateIncludedFileFunc::Exception::set(System::Exception^ value)
{
	m_exception = value;
}

//---------------------------------------------------------------------------
// EnumerateIncludedFileFunc::Invoke
//
// Invokes the contained delegate function for the provided cursor instance
//
// Arguments:
//
//	file			- Included file instance
//	references		- Array of locations where the inclusion is referenced
//	numreferences	- Number of locations in the provided array

void EnumerateIncludedFileFunc::Invoke(CXFile file, CXSourceLocation* references, unsigned int numreferences)
{
	// If the provided delegate function was null, there is no reason to continue
	if(Object::ReferenceEquals(m_action, nullptr)) return;

	// Invoke the provided delegate, passing in a new IncludedFile instance
	return m_action(File::Create(m_handle, m_handle, file), LocationCollection::Create(m_handle, m_handle, references, numreferences));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
