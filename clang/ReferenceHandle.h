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

#ifndef __REFERENCEHANDLE_H_
#define __REFERENCEHANDLE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::InteropServices;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class ReferenceHandle
//
// A reference handle is a handle that refers to an unmanaged resource whose
// lifetime is controlled externally by a SafeHandle instance.  For example
// a pointer or reference into an unmananged structure.  To access the resource
// via the handle, the child class Reference must be used, which will addref
// the parent safe handle before exposing the resource to the caller
//---------------------------------------------------------------------------

template<typename _type>
ref class ReferenceHandle
{
public:

	// Instance Constructor
	//
	ReferenceHandle(SafeHandle^ owner, _type reference) : m_owner(owner)
	{
		if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");

		try { m_pointer = new _type(reference); }
		catch(Exception^) { throw gcnew OutOfMemoryException(); }
	}

	// Class Reference
	//
	// Used to access the underlying unmanaged type contained in a ReferenceHandle
	ref class Reference
	{
	public:

		// Instance Constructor
		//
		Reference(ReferenceHandle<_type>^ handle) : m_handle(handle)
		{
			if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

			// AddRef the safe handle that owns this reference during construction
			m_handle->m_owner->DangerousAddRef(m_release);
 			if(!m_release) throw gcnew ObjectDisposedException(m_handle->m_owner->GetType()->Name);
		}

		// Destructor
		//
		~Reference() 
		{ 
			if(m_release) m_handle->m_owner->DangerousRelease(); 
		}

		// pointer-to-member operator
		//
		_type operator->() { return *m_handle->m_pointer; }

		// operator _type
		//
		operator _type() { return *m_handle->m_pointer; }

	private:

		// m_handle
		//
		// Contained ReferenceHandle<> instance
		ReferenceHandle<_type>^ m_handle;

		// m_release
		//
		// Flag to release the owning safe handle on destruction
		bool m_release;
	};

	// Owner
	//
	// Exposes the contained SafeHandle instance
	property SafeHandle^ Owner
	{
		SafeHandle^ get(void) { return m_owner; }
	}

private:

	// Destructor
	//
	~ReferenceHandle()
	{
		if(m_disposed) return;
		this->!ReferenceHandle();
		m_disposed = true;
	}

	// Finalizer
	//
	!ReferenceHandle()
	{
		if(m_pointer) delete m_pointer;
		m_pointer = __nullptr;
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool				m_disposed;			// Object disposal flag
	_type*				m_pointer;			// Pointer to unmanaged type
	SafeHandle^			m_owner;			// Controlling SafeHandle
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __REFERENCEHANDLE_H_
