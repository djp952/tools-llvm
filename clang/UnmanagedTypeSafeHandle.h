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

#ifndef __UNMANAGEDTYPESAFEHANDLE_H_
#define __UNMANAGEDTYPESAFEHANDLE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Diagnostics;
using namespace System::Runtime::ConstrainedExecution;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class UnmanagedTypeSafeHandle (internal)
//
// Specialization of SafeHandle for use in this library, this specialization
// always accepts an rvalue-reference to an unmanaged resource and takes 
// ownership over it.  Handles can be linked to a parent safe handle instance
// to form a dependency chain that ensure the parent is kept alive as long
// as the child is alive by artificially incrementing it's reference count
// during construction and releasing it during finalization
//---------------------------------------------------------------------------

template<typename _type, void (*_dispose)(_type)>
ref class UnmanagedTypeSafeHandle : public SafeHandle
{
public:

	// Instance Constructor (standalone safe handle)
	//
	UnmanagedTypeSafeHandle(_type&& unmanaged) : SafeHandle(IntPtr::Zero, true)
	{
		// Allocate a new pointer to contain the unmanaged type and take ownership
		try { SetHandle(IntPtr(new _type(unmanaged))); memset(&unmanaged, 0, sizeof(_type)); }
		catch(Exception^) { _dispose(unmanaged); throw gcnew OutOfMemoryException(); }
	}

	// Instance Constructor
	//
	UnmanagedTypeSafeHandle(SafeHandle^ parent, _type&& unmanaged) : m_parent(parent), SafeHandle(IntPtr::Zero, true)
	{
		// Allocate a new pointer to contain the unmanaged type and take ownership
		try { SetHandle(IntPtr(new _type(unmanaged))); memset(&unmanaged, 0, sizeof(_type)); }
		catch(Exception^) { _dispose(unmanaged); throw gcnew OutOfMemoryException(); }

		// Check the parent reference for validity after taking ownership of the unmanaged resource
		if(Object::ReferenceEquals(parent, nullptr)) throw gcnew ArgumentNullException("parent");

		// Attempt to add a reference against the parent safe handle
		m_parent->DangerousAddRef(m_release);
		if(!m_release) throw gcnew ObjectDisposedException(m_parent->GetType()->Name);
	}

	// Class Reference
	//
	// Accesses the unmanaged type referred to by the safe handle
	ref class Reference
	{
	public:

		// Instance Constructor
		//
		Reference(UnmanagedTypeSafeHandle<_type, _dispose>^ handle) : m_handle(handle)
		{
			if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
			
			m_handle->DangerousAddRef(m_release);
			if(!m_release) throw gcnew ObjectDisposedException(UnmanagedTypeSafeHandle<_type, _dispose>::typeid->Name);
		}

		// Destructor
		//
		~Reference() { if(m_release) m_handle->DangerousRelease(); }

		// Pointer-to-member operator
		//
		_type operator->() { return *reinterpret_cast<_type*>(m_handle->DangerousGetHandle().ToPointer()); }

		// _type conversion operator
		//
		operator _type() { return *reinterpret_cast<_type*>(m_handle->DangerousGetHandle().ToPointer()); }

	private:

		// Contained UnmanagedTypeSafeHandle<> instance
		//
		UnmanagedTypeSafeHandle<_type, _dispose>^ m_handle;

		// Flag indicating if the safe handle should be released during destruction
		//
		bool m_release = false;
	};

	//-----------------------------------------------------------------------
	// Member Functions

	// ReleaseHandle (SafeHandle)
	//
	// Releases the contained unmanaged handle/resource
	[ReliabilityContractAttribute(Consistency::MayCorruptProcess, Cer::Success)]
	virtual bool ReleaseHandle(void) override
	{
		// Cast the handle back into an unmanaged type pointer
		_type* unmanaged = reinterpret_cast<_type*>(handle.ToPointer());

		_dispose(*unmanaged);		// Invoke the provided resource disposal function
		delete unmanaged;			// Release the unmanaged heap pointer

#ifdef _DEBUG
		// Dump diagnostic information to the debugger to trace handle lifetimes
		if(Object::ReferenceEquals(m_parent, nullptr)) Debug::WriteLine(String::Format("{0}: Disposed [0x{1:X8}]", this->GetType()->Name, handle));
		else Debug::WriteLine(String::Format("{0}: Disposed [0x{1:X8}, {2}=0x{3:X8}]", this->GetType()->Name, handle, m_parent->GetType()->Name, m_parent->DangerousGetHandle()));
#endif

		// If there is a parent and DangerousAddRef() succeeded, release it now
		if(!Object::ReferenceEquals(m_parent, nullptr) && (m_release)) m_parent->DangerousRelease();

		return true;
	}

	//-----------------------------------------------------------------------
	// Properties

	// IsInvalid (SafeHandle)
	//
	// Gets a value indicating whether the handle/resource value is invalid
	property bool IsInvalid
	{
		virtual bool get(void) override { return (handle == IntPtr::Zero); }
	}

private:

	//-----------------------------------------------------------------------
	// Member Variables

	SafeHandle^					m_parent;			// Parent SafeHandle
	bool						m_release;			// Flag to release parent
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __UNMANAGEDTYPESAFEHANDLE_H_
