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

#ifndef __INDEXENTEREDMAINFILEEVENTARGS_H_
#define __INDEXENTEREDMAINFILEEVENTARGS_H_
#pragma once

#include "IndexEventArgs.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
ref class File;

//---------------------------------------------------------------------------
// Class IndexEnteredMainFileEventArgs
//
// Event arguments class for Index::EnteredMainFile
//---------------------------------------------------------------------------

public ref class IndexEnteredMainFileEventArgs : public IndexEventArgs
{
public:		
	
	//-----------------------------------------------------------------------
	// Fields

	// File
	//
	// The main file instance
	initonly local::File^ File;

internal:	
	
	// Instance Constructor
	//
	IndexEnteredMainFileEventArgs(Object^ state, local::File^ file);
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXENTEREDMAINFILEEVENTARGS_H_
