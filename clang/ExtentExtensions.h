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

#ifndef __EXTENTEXTENSIONS_H_
#define __EXTENTEXTENSIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;
using namespace System::Runtime::CompilerServices;

// FORWARD DECLARATIONS
//
namespace zuki::tools::llvm::clang { ref class Extent; }

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// Class ExtentExtensions
//
// Provides extension methods to supplement the Extension class. 
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class ExtentExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetLength
	//
	// Gets the length of the extent in characters based on the offsets
	[ExtensionAttribute]
	static int GetCharacterLength(Extent^ extent);
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)

#endif	// __EXTENTEXTENSIONS_H_
