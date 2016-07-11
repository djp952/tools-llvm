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

#ifndef __MODULE_H_
#define __MODULE_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class File;
ref class TopLevelHeaderCollection;

//---------------------------------------------------------------------------
// Class Module
//
// Represents a module within a translation unit
//---------------------------------------------------------------------------

public ref class Module
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// IsNull (static)
	//
	// Determines if the specified module object is null
	static bool IsNull(Module^ rhs);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// File
	//
	// Gets the file from which this module came
	property local::File^ File
	{
		local::File^ get(void);
	}

	// FullName
	//
	// Gets the full name of the module
	property String^ FullName
	{
		String^ get(void);
	}

	// IsSystem
	//
	// Indicates if this is a system module
	property bool IsSystem
	{
		bool get(void);
	}

	// Name
	//
	// Gets the name of the module
	property String^ Name
	{
		String^ get(void);
	}

	// ParentModule
	//
	// Gets the parent module instance
	property Module^ ParentModule
	{
		Module^ get(void);
	}

	// TopLevelHeaders
	//
	// Gets a collection of top-level headers associated with this module
	property TopLevelHeaderCollection^ TopLevelHeaders
	{
		TopLevelHeaderCollection^ get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Represents a null module instance
	static initonly Module^ Null;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new Module instance
	static Module^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXModule module);

private:

	// ModuleHandle
	//
	// TranslationUnitReferenceHandle specialization for CXModule
	using ModuleHandle = TranslationUnitReferenceHandle<CXModule>;

	// Instance Constructor
	//
	Module(ModuleHandle^ handle);

	// Static Constructor
	//
	static Module();

	//-----------------------------------------------------------------------
	// Member Variables

	ModuleHandle^				m_handle;		// Underlying safe handle
	TopLevelHeaderCollection^	m_headers;		// Header file collection
	String^						m_name;			// Cached module name
	String^						m_fullname;		// Cached module full name
	local::File^				m_file;			// Cached file instance
	Module^						m_parent;		// Cached parent module
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __MODULE_H_
