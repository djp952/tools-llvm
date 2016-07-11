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

#include "stdafx.h"
#include "AutoGCHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// AutoGCHandle Constructor
//
// Arguments:
//
//	object		- Managed object to be wrapped in a normal GCHandle

AutoGCHandle::AutoGCHandle(Object^ object) : AutoGCHandle(object, GCHandleType::Normal)
{
}

//---------------------------------------------------------------------------
// AutoGCHandle Constructor
//
// Arguments:
//
//	object		- Managed object to be wrapped in a GCHandle
//	type		- Type of GCHandle to create for the object

AutoGCHandle::AutoGCHandle(Object^ object, GCHandleType type)
{
	m_ptr = reinterpret_cast<intptr_t>(GCHandle::ToIntPtr(GCHandle::Alloc(object, type)).ToPointer());
}

//-----------------------------------------------------------------------
// AutoGCHandle Destructor

AutoGCHandle::~AutoGCHandle() 
{ 
	GCHandle::FromIntPtr(IntPtr(m_ptr)).Free(); 
}

//-----------------------------------------------------------------------
// AutoGCHandle::operator const void*

AutoGCHandle::operator const void*() const 
{ 
	return reinterpret_cast<void*>(m_ptr); 
}

//---------------------------------------------------------------------------
// AutoGCHandle::operator void*

AutoGCHandle::operator void*() const 
{ 
	return reinterpret_cast<void*>(m_ptr); 
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

