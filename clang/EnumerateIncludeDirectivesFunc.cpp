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
#include "EnumerateIncludeDirectivesFunc.h"

#include "Cursor.h"
#include "Extent.h"
#include "EnumerateIncludeDirectivesResult.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesFunc Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance
//	func		- Delegate to invoke during enumeration

EnumerateIncludeDirectivesFunc::EnumerateIncludeDirectivesFunc(FileHandle^ handle, Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func) :
	m_handle(handle), m_func(func)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	// func can be nullptr
}

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesFunc::Create (static)
//
// Creates a new EnumerateIncludeDirectivesFunc
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	file		- Unmanaged CXFile instance
//	func		- Delegate to invoke during enumeration

EnumerateIncludeDirectivesFunc^ EnumerateIncludeDirectivesFunc::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXFile file, Func<Cursor^, Extent^, EnumerateIncludeDirectivesResult>^ func)
{
	return gcnew EnumerateIncludeDirectivesFunc(gcnew FileHandle(owner, transunit, file), func);
}

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesFunc::Exception::get
//
// Gets the optional exception instance from the enumeration operation

System::Exception^ EnumerateIncludeDirectivesFunc::Exception::get(void)
{
	return m_exception;
}

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesFunc::Exception::set
//
// Sets an optional exception instance from the enumeration operation

void EnumerateIncludeDirectivesFunc::Exception::set(System::Exception^ value)
{
	m_exception = value;
}

//---------------------------------------------------------------------------
// EnumerateIncludeDirectivesFunc::Invoke
//
// Invokes the contained delegate function for the provided cursor/extent pair
//
// Arguments:
//
//	cursor		- Unmanaged CXCursor instance being enumerated
//	extent		- Unmanaged CXSourceRange instance being enumerated

EnumerateIncludeDirectivesResult EnumerateIncludeDirectivesFunc::Invoke(const CXCursor& cursor, const CXSourceRange& extent)
{
	// If the provided delegate function was null, there is no reason to continue
	if(Object::ReferenceEquals(m_func, nullptr)) return EnumerateIncludeDirectivesResult::Break;

	// Invoke the provided delegate, passing in new Cursor and Extent
	return m_func(Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, cursor), Extent::Create(m_handle->Owner, m_handle->TranslationUnit, extent));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
