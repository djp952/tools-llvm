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

#ifndef __TRANSLATIONUNITREFERENCEHANDLE_H_
#define __TRANSLATIONUNITREFERENCEHANDLE_H_
#pragma once

#include "TranslationUnitHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::ConstrainedExecution;
using namespace System::Runtime::InteropServices;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class TranslationUnitReferenceHandle (internal)
//
// ReferenceHandle derivation for abstract syntax tree objects that require
// access to the containing translation unit
//
// This class cannot derive from ReferenceHandle<> as you run into a lot of
// linker problems when you derive a template class from a template class,
// it was easier in the end to just duplicate what ReferenceHandle<> does
//---------------------------------------------------------------------------

template<typename _type>
ref class TranslationUnitReferenceHandle
{
public:

	// Instance Constructor
	//
	TranslationUnitReferenceHandle(SafeHandle^ owner, TranslationUnitHandle^ transunit, _type reference) : m_outer(owner), m_inner(transunit)
	{
		if(Object::ReferenceEquals(owner, nullptr)) throw gcnew ArgumentNullException("owner");
		if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");

		// Create a local copy of the reference type on the unmanaged heap
		try { m_pointer = new _type(reference); }
		catch(Exception^) { throw gcnew OutOfMemoryException(); }
	}

	// Class Reference
	//
	// Used to access the underlying unmanaged type contained in a TranslationUnitReferenceHandle
	ref class Reference
	{
	public:

		// Instance Constructor
		//
		Reference(TranslationUnitReferenceHandle<_type>^ handle) : m_handle(handle)
		{
			if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");

			// AddRef the outer safe handle that owns this reference
			m_handle->m_outer->DangerousAddRef(m_releaseouter);
 			if(!m_releaseouter) throw gcnew ObjectDisposedException(m_handle->m_outer->GetType()->Name);

			// AddRef the inner translation unit handle only if it's actually a different handle
			if(!Object::ReferenceEquals(m_handle->m_outer, m_handle->m_inner)) {
				
				m_handle->m_inner->DangerousAddRef(m_releaseinner);

				// If the AddRef() on the inner translation unit handle failed the outer safe handle
				// has to be released prior to throwing the exception -- destructor will not get called
				if(!m_releaseinner) {

					m_handle->m_outer->DangerousRelease();
					throw gcnew ObjectDisposedException(m_handle->m_inner->GetType()->Name);
				}
			}
		}

		// Destructor
		//
		~Reference() 
		{ 
			if(m_releaseinner) m_handle->m_inner->DangerousRelease(); 
			if(m_releaseouter) m_handle->m_outer->DangerousRelease();
		}

		// pointer-to-member operator
		//
		_type operator->() { return *m_handle->m_pointer; }

		// operator _type
		//
		operator _type() { return *m_handle->m_pointer; }

		// TranslationUnit
		//
		// Gets the unmanaged CXTranslationUnit structure associated with the reference
		property CXTranslationUnit TranslationUnit
		{
			CXTranslationUnit get(void)
			{
				return *reinterpret_cast<CXTranslationUnit*>(m_handle->m_inner->DangerousGetHandle().ToPointer());
			}
		}

	private:

		// m_handle
		//
		// Copy of the original handle passed in the constructor for ReferenceHandle()
		TranslationUnitReferenceHandle<_type>^ m_handle;

		// m_releaseinner
		//
		// Flag to release the inner translation unit handle reference
		bool m_releaseinner;

		// m_releaseouter
		//
		// Flag to release the outer safe handle reference
		bool m_releaseouter;
	};

	// Owner
	//
	// Exposes the contained SafeHandle instance
	property SafeHandle^ Owner
	{
		SafeHandle^ get(void) { return m_outer; }
	}

	// TranslationUnit
	//
	// Exposes the contained TranslationUnitHandle instance
	property TranslationUnitHandle^ TranslationUnit
	{
		TranslationUnitHandle^ get(void) { return m_inner; }
	}

private:

	// Destructor
	//
	~TranslationUnitReferenceHandle()
	{
		if(m_disposed) return;
		this->!TranslationUnitReferenceHandle();
		m_disposed = true;
	}

	// Finalizer
	//
	!TranslationUnitReferenceHandle()
	{
		if(m_pointer) delete m_pointer;
		m_pointer = __nullptr;
	}

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;		// Object disposal flag
	_type*						m_pointer;		// Pointer to unmanaged type
	SafeHandle^					m_outer;		// Controlling SafeHandle
	TranslationUnitHandle^		m_inner;		// TranslationUnitHandle
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TRANSLATIONUNITREFERENCEHANDLE_H_
