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

#ifndef __VIRTUALFILEOVERLAY_H_
#define __VIRTUALFILEOVERLAY_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::IO;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class VirtualFileOverlay
//
// Encapsulates information about overlaying virtual file/directories over 
// the real file system
//---------------------------------------------------------------------------

public ref class VirtualFileOverlay
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// AddFileMapping
	//
	// Maps an absolute virtual file path to an absolute real one
	void AddFileMapping(String^ virtualpath, String^ physicalpath);

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

	// CaseSensitive
	//
	// Gets/sets the case sensitivity flag for the overlay
	property bool CaseSensitive
	{
		bool get(void);
		void set(bool value);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new VirtualFileOverlay instance
	static VirtualFileOverlay^ Create(CXVirtualFileOverlay&& overlay);

private:

	// VirtualFileOverlayHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXVirtualFileOverlay
	using VirtualFileOverlayHandle = UnmanagedTypeSafeHandle<CXVirtualFileOverlay, clang_VirtualFileOverlay_dispose>;

	// Instance Constructor
	//
	VirtualFileOverlay(VirtualFileOverlayHandle^ handle);

	// Destructor
	//
	~VirtualFileOverlay();

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal flag
	VirtualFileOverlayHandle^	m_handle;			// Underlying safe handle
	bool						m_casesensitive;	// Case sensitivity flag
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __VIRTUALFILEOVERLAY_H_
