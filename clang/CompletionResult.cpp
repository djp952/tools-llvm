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
#include "CompletionResult.h"

#include "CompletionString.h"
#include "CursorKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompletionResult Constructor (private)
//
// Arguments:
//
//	handle		- CompletionResult handle for this instance

CompletionResult::CompletionResult(CompletionResultHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CompletionResult::Create (internal, static)
//
// Creates a new Completion instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	result		- Unmanaged CXCompletionResult instance

CompletionResult^ CompletionResult::Create(SafeHandle^ owner, CXCompletionResult result)
{
	return gcnew CompletionResult(gcnew CompletionResultHandle(owner, result));
}

//---------------------------------------------------------------------------
// CompletionResult::Kind::get
//
// Gets the kind of entity that this completion refers to

CursorKind CompletionResult::Kind::get(void)
{
	return CursorKind(static_cast<CXCompletionResult>(CompletionResultHandle::Reference(m_handle)).CursorKind);
}

//---------------------------------------------------------------------------
// CompletionResult::String::get
//
// Gets the completion string associated with this completion result

CompletionString^ CompletionResult::String::get(void)
{
	if(Object::ReferenceEquals(m_string, nullptr))
		m_string = CompletionString::Create(m_handle->Owner, static_cast<CXCompletionResult>(CompletionResultHandle::Reference(m_handle)).CompletionString);

	return m_string;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
