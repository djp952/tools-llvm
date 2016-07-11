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
#include "TypeExtensions.h"

#include "Cursor.h"
#include "EnumerateFieldsResult.h"
#include "Type.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// TypeExtensions::FieldCollector Constructor
//
// Arguments:
//
//	predicate	- Predicate delegate used to filter during collection adds

TypeExtensions::FieldCollector::FieldCollector(Func<Cursor^, bool>^ predicate) : m_predicate(predicate)
{
}

//---------------------------------------------------------------------------
// TypeExtensions::ChildCollector::EnumerateFieldsCallback
//
// Callback delegate passed into Type::EnumerateFields
//
// Arguments:
//
//	cursor			- Current cursor being enumerated
//	parent			- Parent cursor to the enumerated cursor

EnumerateFieldsResult TypeExtensions::FieldCollector::EnumerateFieldsCallback(Cursor^ cursor)
{
	// Determine if the enumerated cursor instance should be collected or not
	bool addcursor = (Object::ReferenceEquals(m_predicate, nullptr)) ? true : m_predicate(cursor);
	if(addcursor) List<Cursor^>::Add(cursor);

	return EnumerateFieldsResult::Continue;		// Continue the enumeration
}

//---------------------------------------------------------------------------
// TypeExtensions::FindFields (static)
//
// Finds field cursors of this type based on a predicate
//
// Arguments:
//
//	type			- Type instance being extended
//	predicate		- Predicate delegate used to filter the children

ReadOnlyCollection<Cursor^>^ TypeExtensions::FindFields(Type^ type, Func<Cursor^, bool>^ predicate)
{
	if(Object::ReferenceEquals(type, nullptr)) throw gcnew ArgumentNullException("type");

	// Create a FieldCollector instance and use that to invoke Type::EnumerateFields
	FieldCollector^ collector = gcnew FieldCollector(predicate);
	type->EnumerateFields(gcnew Func<Cursor^, EnumerateFieldsResult>(collector, &FieldCollector::EnumerateFieldsCallback));

	return collector->AsReadOnly();				// Return the collection as read-only
}

//---------------------------------------------------------------------------
// TypeExtensions::GetFields (static)
//
// Gets all field cursors of this type
//
// Arguments:
//
//	type			- Type instance being extended

ReadOnlyCollection<Cursor^>^ TypeExtensions::GetFields(Type^ type)
{
	if(Object::ReferenceEquals(type, nullptr)) throw gcnew ArgumentNullException("type");
	return FindFields(type, nullptr);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)
