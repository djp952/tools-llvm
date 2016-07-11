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

#ifndef __TYPEEXTENSIONS_H_
#define __TYPEEXTENSIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Runtime::CompilerServices;

// FORWARD DECLARATIONS
//
namespace zuki::tools::llvm::clang { ref class Cursor; }
namespace zuki::tools::llvm::clang { value class EnumerateFieldsResult; }
namespace zuki::tools::llvm::clang { ref class Type; }

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// Class TypeExtensions
//
// Provides extension methods to supplement the Type class. 
//
// Extensions are used when a method doesn't need to access anything but the
// existing public interfaces, the idea being not to clutter up the library
// code with convenience operations
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class TypeExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// FindFields
	//
	// Finds all field cursors of this cursor based on a predicate
	[ExtensionAttribute]
	static ReadOnlyCollection<Cursor^>^ FindFields(Type^ type, Func<Cursor^, bool>^ predicate);

	// GetFields
	//
	// Gets all fields cursors of this type
	[ExtensionAttribute]
	static ReadOnlyCollection<Cursor^>^ GetFields(Type^ type);

private:

	//-----------------------------------------------------------------------
	// Private Data Types

	// Class FieldCollector
	//
	// Specialization of List<> used when enumerating type fields
	ref class FieldCollector : public List<Cursor^>
	{
	public:

		// Instance Constructor
		//
		FieldCollector(Func<Cursor^, bool>^ predicate);

		// EnumerateFieldsCallback
		//
		// Callback delegate to provide to Cursor::EnumerateChildren
		EnumerateFieldsResult EnumerateFieldsCallback(Cursor^ cursor);

	private:

		// Predicate used to filter collection additions
		//
		Func<Cursor^, bool>^ m_predicate;
	};
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)

#endif	// __TYPEEXTENSIONS_H_
