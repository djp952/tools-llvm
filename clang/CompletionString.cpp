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
#include "CompletionString.h"

#include "Availability.h"
#include "CompletionChunkCollection.h"
#include "StringCollection.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompletionString Constructor (private)
//
// Arguments:
//
//	handle		- Completion string safe handle instance

CompletionString::CompletionString(CompletionStringHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CompletionString::Annotations::get
//
// Gets a collection of annotations associated with the completion

StringCollection^ CompletionString::Annotations::get(void)
{
	if(Object::ReferenceEquals(m_annotations, nullptr)) {

		CompletionStringHandle::Reference string(m_handle);			// Unwrap the safe handle
		
		// Get the number of annotations associated with the completion and create a List<> instance
		unsigned int count = clang_getCompletionNumAnnotations(string);
		List<String^>^ annotations = gcnew List<String^>(count);

		// Convert all of the annotations into managed strings and set the cached string collection
		for(unsigned int index = 0; index < count; index++) annotations->Add(StringUtil::ToString(clang_getCompletionAnnotation(string, index)));
		m_annotations = StringCollection::Create(annotations);
	}

	return m_annotations;
}

//---------------------------------------------------------------------------
// CompletionString::Availability::get
//
// Gets the availability of the entity that this completion refers to

local::Availability CompletionString::Availability::get(void)
{
	return local::Availability(clang_getCompletionAvailability(CompletionStringHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompletionString::BriefComment::get
//
// Gets the brief documentation comment attached to the completion declaration

String^ CompletionString::BriefComment::get(void)
{
	if(Object::ReferenceEquals(m_briefcomment, nullptr))
		m_briefcomment = StringUtil::ToString(clang_getCompletionBriefComment(CompletionStringHandle::Reference(m_handle)));

	return m_briefcomment;
}
	
//---------------------------------------------------------------------------
// CompletionString::Chunks::get
//
// Gets a collection of completion chunks

CompletionChunkCollection^ CompletionString::Chunks::get(void)
{
	if(Object::ReferenceEquals(m_chunks, nullptr)) 
		m_chunks = CompletionChunkCollection::Create(m_handle->Owner, CompletionStringHandle::Reference(m_handle));

	return m_chunks;
}

//---------------------------------------------------------------------------
// CompletionString::Create (internal, static)
//
// Creates a new CompletionString instance
//
// Arguments:
//
//	owner		- Owning SafeHandle instance
//	string		- Reference to the CXCompletionString instance

CompletionString^ CompletionString::Create(SafeHandle^ owner, CXCompletionString string)
{
	return gcnew CompletionString(gcnew CompletionStringHandle(owner, string));
}

//---------------------------------------------------------------------------
// CompletionString::ParentContext::get
//
// Gets the parent context of the completion

String^ CompletionString::ParentContext::get(void)
{
	if(Object::ReferenceEquals(m_parentcontext, nullptr))
		m_parentcontext = StringUtil::ToString(clang_getCompletionParent(CompletionStringHandle::Reference(m_handle), /* deprecated */ __nullptr));

	return m_parentcontext;
}
	
//---------------------------------------------------------------------------
// CompletionString::Priority::get
//
// Gets the priority of this code completion

unsigned int CompletionString::Priority::get(void)
{
	return clang_getCompletionPriority(CompletionStringHandle::Reference(m_handle));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
