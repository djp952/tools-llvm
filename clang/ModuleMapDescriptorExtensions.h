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

#ifndef __MODULEMAPDESCRIPTOREXTENSIONS_H_
#define __MODULEMAPDESCRIPTOREXTENSIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Collections::ObjectModel;
using namespace System::IO;
using namespace System::Runtime::CompilerServices;
using namespace System::Text;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class ModuleMapDescriptor;

BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// Class ModuleMapDescriptorExtensions
//
// Provides extension methods to supplement the ModuleMapDescriptor class
//---------------------------------------------------------------------------

[ExtensionAttribute]
public ref class ModuleMapDescriptorExtensions abstract sealed
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// Save
	//
	// Saves the module map descriptor to a file
	[ExtensionAttribute] static void Save(ModuleMapDescriptor^ descriptor, String^ path);
	[ExtensionAttribute] static void Save(ModuleMapDescriptor^ descriptor, String^ path, Encoding^ encoding);

	// WriteTo
	//
	// Writes the contents of the descriptor to a stream
	[ExtensionAttribute] static void WriteTo(ModuleMapDescriptor^ descriptor, Stream^ stream);
};

//---------------------------------------------------------------------------

END_NAMESPACE(extensions)
END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __MODULEMAPDESCRIPTOREXTENSIONS_H_