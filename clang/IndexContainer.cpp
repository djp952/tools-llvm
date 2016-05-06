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
#include "IndexContainer.h"

#include "Cursor.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// IndexContainer Constructor (private)
//
// Arguments:
//
//	handle		- Reference handle instance

IndexContainer::IndexContainer(IdxContainerInfoHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// IndexContainer::ClientData::get
//
// Gets a client object associated with this container during indexing

Object^ IndexContainer::ClientData::get(void)
{
	IdxContainerInfoHandle::Reference info(m_handle);
	if(info.IsNull) return nullptr;

	// Get the object associated with this entity and return null if it hasn't been set yet
	CXIdxClientEntity object = clang_index_getClientContainer(info);
	if(object == __nullptr) return nullptr;

	return GCHandle::FromIntPtr(IntPtr(object)).Target;
}

//---------------------------------------------------------------------------
// IndexContainer::ClientData::set
//
// Associates a client object with this container during indexing

void IndexContainer::ClientData::set(Object^ value)
{
	IdxContainerInfoHandle::Reference info(m_handle);
	if(info.IsNull) return;

	// Convert null object references into a null pointer to associate with the container
	if(Object::ReferenceEquals(value, nullptr)) clang_index_setClientContainer(info, __nullptr);

	// The object has to be tracked somewhere to prevent garbage collection until this
	// index action has been completed -- ask the IndexAction to track it
	GCHandle gch = m_handle->IndexAction->TrackClientObject(value);
	clang_index_setClientContainer(info, GCHandle::ToIntPtr(gch).ToPointer());
}

//---------------------------------------------------------------------------
// IndexContainer::Create (internal, static)
//
// Creates a new IndexEntityReference instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Pointer to the container reference information

IndexContainer^ IndexContainer::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxContainerInfo* info)
{
	return gcnew IndexContainer(gcnew IdxContainerInfoHandle(owner, action, info));
}

//---------------------------------------------------------------------------
// IndexContainer::Cursor::get
//
// Gets the container cursor

local::Cursor^ IndexContainer::Cursor::get(void)
{
	if(Object::ReferenceEquals(m_cursor, nullptr)) {

		IdxContainerInfoHandle::Reference info(m_handle);
		m_cursor = (info.IsNull) ? local::Cursor::Null : local::Cursor::Create(m_handle->Owner, TranslationUnitHandle::Null, info->cursor);
	}

	return m_cursor;
}

//---------------------------------------------------------------------------
// IndexContainer::IsNull (static)
//
// Determines if the container is null
//
// Arguments:
//
//	rhs		- IndexContainer instance to be checked for null

bool IndexContainer::IsNull(IndexContainer^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (IdxContainerInfoHandle::Reference(rhs->m_handle).IsNull);
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
