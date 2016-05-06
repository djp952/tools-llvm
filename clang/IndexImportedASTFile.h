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

#ifndef __INDEXIMPORTEDASTFILE_H_
#define __INDEXIMPORTEDASTFILE_H_
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
ref class Module;

//---------------------------------------------------------------------------
// Class IndexImportedASTFile
//
// Represents information about an AST file imported during indexing
//---------------------------------------------------------------------------

public ref class IndexImportedASTFile
{
public:

	//-----------------------------------------------------------------------
	// Properties

	// File
	//
	// Gets the file instance associated with this import
	property local::File^ File
	{
		local::File^ get(void);
	}

	// IsImplicit
	//
	// Gets a flag indicating if an inclusion directive was converted into a module
	property bool IsImplicit
	{
		bool get(void);
	}

	// Location
	//
	// Gets the location where the file is imported
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// Module
	//
	// Gets the imported module or a null module if the imported file is a PCH
	property local::Module^ Module
	{
		local::Module^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new IndexImportedASTFile instance
	static IndexImportedASTFile^ Create(SafeHandle^ owner, IndexAction^ action, const CXIdxImportedASTFileInfo* info);

private:

	// IdxImportedASTFileInfoHandle
	//
	// IndexActionPointerHandle specialization
	using IdxImportedASTFileInfoHandle = IndexActionPointerHandle<const CXIdxImportedASTFileInfo*>;

	// Instance Constructor
	//
	IndexImportedASTFile(IdxImportedASTFileInfoHandle^ handle);

	//-----------------------------------------------------------------------
	// Member Variables

	IdxImportedASTFileInfoHandle^	m_handle;		// Reference handle
	local::File^					m_file;			// Cached file object
	local::Module^					m_module;		// Cached module object
	local::Location^				m_location;		// Cached location object
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXIMPORTEDASTFILE_H_
