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
#include "TemplateArgument.h"

#include "TemplateArgumentKind.h"
#include "Type.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// TemplateArgument Constructor (private)
//
// Arguments:
//
//	handle			- Reference handle
//	index			- Index into the cursor template arguments

TemplateArgument::TemplateArgument(CursorHandle^ handle, unsigned int index)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

	CursorHandle::Reference cursor(handle);		// Unwrap and access the safe handle

	// Grab all of the properties for this argument during construction, no need to keep the handle
	m_kind = TemplateArgumentKind(clang_Cursor_getTemplateArgumentKind(cursor, index));
	m_type = local::Type::Create(handle->Owner, handle->TranslationUnit, clang_Cursor_getTemplateArgumentType(cursor, index));
	m_ivalue = (m_kind == TemplateArgumentKind::Integral) ? Nullable<__int64>(clang_Cursor_getTemplateArgumentValue(cursor, index)) : Nullable<__int64>();
	m_uvalue = (m_kind == TemplateArgumentKind::Integral) ? Nullable<unsigned __int64>(clang_Cursor_getTemplateArgumentUnsignedValue(cursor, index)) : Nullable<unsigned __int64>();
}

//---------------------------------------------------------------------------
// TemplateArgument::Create (internal, static)
//
// Creates a new TemplateArgument instance
//
// Arguments:
//
//	owner			- Owning SafeHandle instance
//	transunit		- TranslationUnitHandle instance
//	cursor			- Unmanaged CXCursor instance
//	index			- Index into the cursor template arguments

TemplateArgument^ TemplateArgument::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor, unsigned int index)
{
	return gcnew TemplateArgument(gcnew CursorHandle(owner, transunit, cursor), index);
}

//---------------------------------------------------------------------------
// TemplateArgument::Kind::get
//
// Gets the template argument kind

TemplateArgumentKind TemplateArgument::Kind::get(void)
{
	return m_kind;
}

//---------------------------------------------------------------------------
// TemplateArgument::Type::get
//
// Gets the template argument type

local::Type^ TemplateArgument::Type::get(void)
{
	return m_type;
}

//---------------------------------------------------------------------------
// TemplateArgument::UnsignedValue::get
//
// Gets the unsigned template argument value

Nullable<unsigned __int64> TemplateArgument::UnsignedValue::get(void)
{
	return m_uvalue;
}

//---------------------------------------------------------------------------
// TemplateArgument::Value::get
//
// Gets the signed template argument value

Nullable<__int64> TemplateArgument::Value::get(void)
{
	return m_ivalue;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
