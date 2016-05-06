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

#ifndef __INDEXINCLUDEDFILE_H_
#define __INDEXINCLUDEDFILE_H_
#pragma once

#include "IndexActionPointerHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class File;
ref class IndexAction;
ref class Location;

//---------------------------------------------------------------------------
// Class IndexIncludedFile
//
// Represents information about an included file during indexing
//---------------------------------------------------------------------------

public ref class IndexIncludedFile
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// File
	//
	// Gets the file instance for this include
	property local::File^ File
	{
		local::File^ get(void);
	}

	// IsAngled
	//
	// Gets a flag indicating if the include directive was angled ("<>")
	property bool IsAngled
	{
		bool get(void);
	}

	// IsImport
	//
	// Gets a flag indicating if the directive was an #import rather than an #include
	property bool IsImport
	{
		bool get(void);
	}

	// IsModuleImport
	//
	// Gets a flag indicating if the directive was turned into a module import
	property bool IsModuleImport
	{
		bool get(void);
	}

	// Location
	//
	// Gets the file location of the include/import directive
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// Spelling
	//
	// Gets the file name as written in the include/import directive
	property String^ Spelling
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexIncludedFile instance
	static IndexIncludedFile^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxIncludedFileInfo* info);

private:

	// IdxIncludedFileInfoHandle
	//
	// IndexActionPointerHandle specialization
	using IdxIncludedFileInfoHandle = IndexActionPointerHandle<const CXIdxIncludedFileInfo*>;

	// Instance Constructor
	//
	IndexIncludedFile(IdxIncludedFileInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxIncludedFileInfoHandle^		m_handle;		// Reference handle
	String^							m_spelling;		// Cached spelling string
	local::File^					m_file;			// Cached file instance
	local::Location^				m_location;		// Cached location object
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXINCLUDEDFILE_H_
