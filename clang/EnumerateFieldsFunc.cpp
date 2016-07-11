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
#include "EnumerateFieldsFunc.h"

#include "Cursor.h"
#include "EnumerateFieldsResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// EnumerateFieldsFunc Constructor (private)
//
// Arguments:
//
//	handle		- Parent TypeHandle instance
//	func		- Delegate to invoke during enumeration

EnumerateFieldsFunc::EnumerateFieldsFunc(TypeHandle^ handle, Func<Cursor^, EnumerateFieldsResult>^ func) : m_handle(handle), m_func(func)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	// func can be null
}

//---------------------------------------------------------------------------
// EnumerateFieldsFunc::Create (static)
//
// Creates a new EnumerateFieldsResult instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	type		- Unmanaged CXType instance
//	func		- Delegate to invoke during enumeration

EnumerateFieldsFunc^ EnumerateFieldsFunc::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXType type, Func<Cursor^, EnumerateFieldsResult>^ func)
{
	return gcnew EnumerateFieldsFunc(gcnew TypeHandle(owner, transunit, type), func);
}

//---------------------------------------------------------------------------
// EnumerateFieldsFunc::Exception::get
//
// Gets the optional exception instance from the enumeration operation

System::Exception^ EnumerateFieldsFunc::Exception::get(void)
{
	return m_exception;
}

//---------------------------------------------------------------------------
// EnumerateFieldsFunc::Exception::set
//
// Sets an optional exception instance from the enumeration operation

void EnumerateFieldsFunc::Exception::set(System::Exception^ value)
{
	m_exception = value;
}

//---------------------------------------------------------------------------
// EnumerateFieldsFunc::Invoke
//
// Invokes the contained delegate function for the provided cursor instance
//
// Arguments:
//
//	cursor		- Unmanaged CXCursor instance with which to invoke the delegate

EnumerateFieldsResult EnumerateFieldsFunc::Invoke(const CXCursor& cursor)
{
	// If the provided delegate function was null, there is no reason to continue
	if(Object::ReferenceEquals(m_func, nullptr)) return EnumerateFieldsResult::Break;

	// Invoke the provided delegate, passing in a new Cursor instance
	return m_func(Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, cursor));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
