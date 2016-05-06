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

#ifndef __GCHANDLEREF_H_
#define __GCHANDLEREF_H_
#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Template Class GCHandleRef (Native)
//
// GCHandleRef<T> implements a mechanism of making the conversion from a void*
// or an intptr_t back into an object reference less intrusive
//---------------------------------------------------------------------------

template<class T>
class GCHandleRef
{
public:

	// Instance Constructors
	//
	GCHandleRef(const void *pv) : m_ptr(reinterpret_cast<intptr_t>(pv)) {}
	GCHandleRef(intptr_t ptr) : m_ptr(ptr) {}

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// Object^ Conversion Operator
	//
	operator Object^() const { return GCHandle::FromIntPtr(IntPtr(m_ptr)).Target; }

	// T Conversion Operator
	//
	operator T() const { return static_cast<T>(GCHandle::FromIntPtr(IntPtr(m_ptr)).Target); }
	
	// Pointer-To-Member Operator
	//
	T operator ->() const { return operator T(); }

	// Equality Operator
	//
	bool operator ==(T rhs) { return (operator T() == rhs); }

	// Inequality Operator
	//
	bool operator !=(T rhs) { return (operator T() != rhs); }

	//-----------------------------------------------------------------------
	// Properties

	// IsAllocated
	//
	// Used with weak GCHandle references to make sure the object is allocated
	// before attempting to access it via the pointer-to-member operator
	__declspec(property(get=getIsAllocated)) bool IsAllocated;
	bool getIsAllocated(void) const { return GCHandle::FromIntPtr(IntPtr(m_ptr)).IsAllocated; }

private:

	GCHandleRef(const GCHandleRef&)=delete;
	GCHandleRef& operator=(const GCHandleRef&)=delete;

	//-----------------------------------------------------------------------
	// Member Variables

	intptr_t				m_ptr;			// Serialized GCHandle data
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __GCHANDLEREF_H_
