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

#ifndef __MODULEMAPDESCRIPTOR_H_
#define __MODULEMAPDESCRIPTOR_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class ModuleMapDescriptor
//
// Encapsulates information about a module.modulemap file
//---------------------------------------------------------------------------

public ref class ModuleMapDescriptor
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ToByteArray
	//
	// Converts the underlying character data into a byte array
	array<Byte>^ ToByteArray(void);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// FrameworkModule
	//
	// Gets/sets the framework module name that that module.map describes
	property String^ FrameworkModule
	{
		String^ get(void);
		void set(String^ value);
	}

	// UmbrellaHeader
	//
	// Gets/sets the umbrella header name that the module map describes
	property String^ UmbrellaHeader
	{
		String^ get(void);
		void set(String^ value);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new ModuleMapDescriptor instance
	static ModuleMapDescriptor^ Create(CXModuleMapDescriptor&& descriptor, String^ frameworkmodule, String^ umbrellaheader);

private:

	// ModuleMapDescriptorHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXModuleMapDescriptor
	using ModuleMapDescriptorHandle = UnmanagedTypeSafeHandle<CXModuleMapDescriptor, clang_ModuleMapDescriptor_dispose>;

	// Instance Constructor
	//
	ModuleMapDescriptor(ModuleMapDescriptorHandle^ handle, String^ frameworkmodule, String^ umbrellaheader);

	// Destructor
	//
	~ModuleMapDescriptor();

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal flag
	ModuleMapDescriptorHandle^	m_handle;			// Underlying safe handle
	String^						m_module;			// Cached module name
	String^						m_header;			// Cached header name
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __MODULEMAPDESCRIPTOR_H_
