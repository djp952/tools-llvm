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
#include "CompletionChunk.h"

#include "CompletionChunkKind.h"
#include "CompletionString.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompletionChunk Constructor (private)
//
// Arguments:
//
//	handle		- Parent CompletionString safe handle
//	index		- Index into the chunk collection

CompletionChunk::CompletionChunk(CompletionStringHandle^ handle, unsigned int index) : m_handle(handle), m_index(index)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CompletionChunk::CompletionString::get
//
// Gets the completion string associated with a particular chunk within a completion string

local::CompletionString^ CompletionChunk::CompletionString::get(void)
{
	if(Object::ReferenceEquals(m_string, nullptr))
		m_string = local::CompletionString::Create(m_handle->Owner, clang_getCompletionChunkCompletionString(CompletionStringHandle::Reference(m_handle), m_index));

	return m_string;
}
	
//---------------------------------------------------------------------------
// CompletionChunk::Create (internal, static)
//
// Creates a new CompletionChunk instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	string		- Unmanaged CXCompletionString instance
//	index		- Index into the chunk collection

CompletionChunk^ CompletionChunk::Create(SafeHandle^ owner, CXCompletionString string, unsigned int index)
{
	return gcnew CompletionChunk(gcnew CompletionStringHandle(owner, string), index);
}

//---------------------------------------------------------------------------
// CompletionChunk::Kind::get
//
// Gets the kind of a particular chunk within a completion string

CompletionChunkKind CompletionChunk::Kind::get(void)
{
	return CompletionChunkKind(clang_getCompletionChunkKind(CompletionStringHandle::Reference(m_handle), m_index));
}
	
//---------------------------------------------------------------------------
// CompletionChunk::Text::get
//
// Gets the text associated with a particular chunk within a completion string

String^ CompletionChunk::Text::get(void)
{
	if(Object::ReferenceEquals(m_text, nullptr))
		m_text = StringUtil::ToString(clang_getCompletionChunkText(CompletionStringHandle::Reference(m_handle), m_index));

	return m_text;
}
	
//---------------------------------------------------------------------------
// CompletionChunk::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ CompletionChunk::ToString(void)
{
	return this->Text;
}
	
//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
