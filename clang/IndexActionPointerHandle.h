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

#ifndef __INDEXACTIONPOINTERHANDLE_H_
#define __INDEXACTIONPOINTERHANDLE_H_
#pragma once

#include "IndexAction.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::ConstrainedExecution;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

//---------------------------------------------------------------------------
// Class IndexActionPointerHandle (internal)
//
// ReferenceHandle derivation for indexer objects that require access to
// the containing IndexAction ref class instance.  All indexer objects
// are also stored as pointers, so there is no need to allocate a new
// pointer to store it like ReferenceHandle<> would.  As such, there is
// also no need for this handle to implement a destructor/finalizer pair
//---------------------------------------------------------------------------

template<typename _type>
ref class IndexActionPointerHandle
{
public:

	// Instance Constructor
	//
	IndexActionPointerHandle(SafeHandle^ owner, IndexAction^ action, _type pointer) : m_owner(owner), m_action(action), m_pointer(pointer)
	{
		if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");
		if(Object::ReferenceEquals(action, nullptr)) throw gcnew ArgumentNullException("action");
	}

	// Class Reference
	//
	// Used to access the underlying unmanaged type contained in a IndexActionPointerHandle
	ref class Reference
	{
	public:

		// Instance Constructor
		//
		Reference(IndexActionPointerHandle<_type>^ handle) : m_handle(handle)
		{
			if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

			// AddRef the safe handle that owns this reference
			m_handle->m_owner->DangerousAddRef(m_release);
 			if(!m_release) throw gcnew ObjectDisposedException(m_handle->m_owner->GetType()->Name);
		}

		// Destructor
		//
		~Reference() 
		{ 
			if(m_release) m_handle->m_owner->DangerousRelease();
		}

		// operator !
		//
		bool operator !() { return m_handle->m_pointer == __nullptr; }

		// pointer-to-member operator
		//
		_type operator->() { return m_handle->m_pointer; }

		// operator _type
		//
		operator _type() { return m_handle->m_pointer; }

		// IsNull
		//
		// Determines if the contained pointer is null
		property bool IsNull
		{
			bool get(void) { return m_handle->m_pointer == __nullptr; }
		}

	private:

		// m_handle
		//
		// Copy of the original handle passed in the constructor for ReferenceHandle()
		IndexActionPointerHandle<_type>^ m_handle;

		// m_release
		//
		// Flag to release the outer safe handle reference
		bool m_release;
	};

	// Owner
	//
	// Exposes the contained SafeHandle instance
	property SafeHandle^ Owner
	{
		SafeHandle^ get(void) { return m_owner; }
	}

	// IndexAction
	//
	// Exposes the contained IndexActionHandle instance
	property local::IndexAction^ IndexAction
	{
		local::IndexAction^ get(void) { return m_action; }
	}
	
private:

	//-----------------------------------------------------------------------
	// Member Variables

	_type					m_pointer;		// Pointer to unmanaged type
	SafeHandle^				m_owner;		// Controlling SafeHandle
	local::IndexAction^		m_action;		// IndexAction instance
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __INDEXACTIONPOINTERHANDLE_H_
