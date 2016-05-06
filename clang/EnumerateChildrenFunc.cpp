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
#include "EnumerateChildrenFunc.h"

#include "Cursor.h"
#include "EnumerateChildrenResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateChildrenFunc Constructor
//
// Arguments:
//
//	handle		- Reference handle instance
//	func		- Predicate to invoke during enumeration

EnumerateChildrenFunc::EnumerateChildrenFunc(CursorHandle^ handle, Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func) : 
	m_handle(handle), m_func(func)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	// func can be nullptr
}

//---------------------------------------------------------------------------
// EnumerateChildrenFunc::Create (static)
//
// Creates a new EnumerateChildrenFunc instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	cursor		- Unmanaged CXCursor
//	func		- Predicate to invoke during enumeration

EnumerateChildrenFunc^ EnumerateChildrenFunc::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor, Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func)
{
	return gcnew EnumerateChildrenFunc(gcnew CursorHandle(owner, transunit, cursor), func);
}

//---------------------------------------------------------------------------
// EnumerateChildrenFunc::Exception::get
//
// Gets the optional exception instance from the enumeration operation

System::Exception^ EnumerateChildrenFunc::Exception::get(void)
{
	return m_exception;
}

//---------------------------------------------------------------------------
// EnumerateChildrenFunc::Exception::set
//
// Sets an optional exception instance from the enumeration operation

void EnumerateChildrenFunc::Exception::set(System::Exception^ value)
{
	m_exception = value;
}

//---------------------------------------------------------------------------
// EnumerateChildrenFunc::Invoke
//
// Invokes the contained delegate function for the provided cursor instance
//
// Arguments:
//
//	cursor		- Unmanaged CXCursor instance being enumerated
//	parent		- Unmanaged parent CXCursor instance

EnumerateChildrenResult EnumerateChildrenFunc::Invoke(const CXCursor& cursor, const CXCursor& parent)
{
	// If the provided delegate function was null, there is no reason to continue
	if(Object::ReferenceEquals(m_func, nullptr)) return EnumerateChildrenResult::Break;

	// Invoke the provided delegate, passing in new Cursor instances
	return m_func(Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, cursor), Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, parent));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
