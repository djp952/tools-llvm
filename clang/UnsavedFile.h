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

#ifndef __UNSAVEDFILE_H_
#define __UNSAVEDFILE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Text;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class UnsavedFile
//
// Provides the contents of a file that has not yet been saved to disk
//---------------------------------------------------------------------------

public ref class UnsavedFile
{
public:

	// Instance Constructors
	//
	UnsavedFile(String^ filename, String^ content);
	UnsavedFile(String^ filename, TextReader^ contentreader);

	//-----------------------------------------------------------------------
	// Properties

	// Content
	//
	// Gets/sets the buffer containing the unsaved contents of this file
	property String^ Content
	{
		String^ get(void);
		void set(String^ value);
	}

	// FileName
	//
	// Gets/sets the file whose contents have not yet been saved
	property String^ FileName
	{
		String^ get(void);
		void set(String^ value);
	}

internal:

	//-----------------------------------------------------------------------
	// Private Member Functions

	// FreeUnsavedFilesArray (static)
	//
	// Releases unmanaged memory allocated with UnsavedFilesToArray
	static CXUnsavedFile* FreeUnsavedFilesArray(CXUnsavedFile* rg, int numunsaved);

	// UnsavedFilesToArray (static)
	//
	// Converts an enumerable collection of UnsavedFile objects into an unmanaged array
	static CXUnsavedFile* UnsavedFilesToArray(IEnumerable<UnsavedFile^>^ unsavedfiles, int* numunsaved);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^				m_filename;			// Unsaved file name
	String^				m_content;			// Unsaved file content
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __UNSAVEDFILE_H_
