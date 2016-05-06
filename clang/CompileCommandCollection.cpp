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
#include "CompileCommandCollection.h"

#include "CompileCommand.h"
#include "ReadOnlyListEnumerator.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompileCommandCollection Constructor
//
// Arguments:
//
//	handle		- Underlying CompileCommandsHandle instance

CompileCommandCollection::CompileCommandCollection(CompileCommandsHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_cache = gcnew array<CompileCommand^>(clang_CompileCommands_getSize(CompileCommandsHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompileCommandCollection Destructor

CompileCommandCollection::~CompileCommandCollection()
{
	if(m_disposed) return;

	// Dispose of all cached CompileCommand instances
	for each(CompileCommand^ command in m_cache) delete command;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// CompileCommandCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

CompileCommand^ CompileCommandCollection::default::get(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Check for a cached instance of the CompileCommand first
	CompileCommand^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// clang_CompileCommands_getCommand will return null if the index is out of range
	CXCompileCommand command = clang_CompileCommands_getCommand(CompileCommandsHandle::Reference(m_handle), static_cast<unsigned int>(index));
	if(command == __nullptr) throw gcnew ArgumentOutOfRangeException("index");

	// Create a new compile command and cache it to prevent multiple creations
	m_cache[index] = CompileCommand::Create(m_handle, std::move(command));
	return m_cache[index];
}

//---------------------------------------------------------------------------
// CompileCommandCollection::Count::get
//
// Gets the number of elements in the collection

int CompileCommandCollection::Count::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_cache->Length;
}

//---------------------------------------------------------------------------
// CompileCommandCollection::Create (static, internal)
//
// Creates a new CompileCommands instance
//
// Arguments:
//
//	commands		- Unmanaged CXCompileCommands instance to be wrapped

CompileCommandCollection^ CompileCommandCollection::Create(CXCompileCommands&& commands)
{
	return gcnew CompileCommandCollection(gcnew CompileCommandsHandle(std::move(commands)));
}

//---------------------------------------------------------------------------
// CompileCommandCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<CompileCommand^>^ CompileCommandCollection::GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return gcnew ReadOnlyListEnumerator<CompileCommand^>(this);
}

//---------------------------------------------------------------------------
// CompileCommandCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ CompileCommandCollection::IEnumerable_GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
