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
#include "CompileCommand.h"

#include "CompileCommandArgumentCollection.h"
#include "CompileCommandSourceMappingCollection.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// CompileCommand Constructor
//
// Arguments:
//
//	handle			- Underlying CompileCommandHandle instance

CompileCommand::CompileCommand(CompileCommandHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CompileCommand::Arguments::get
//
// Gets a collection of compiler invocation arguments

CompileCommandArgumentCollection^ CompileCommand::Arguments::get(void)
{
	if(Object::ReferenceEquals(m_arguments, nullptr))
		m_arguments = CompileCommandArgumentCollection::Create(m_handle->Owner, CompileCommandHandle::Reference(m_handle));

	return m_arguments;
}

//---------------------------------------------------------------------------
// CompileCommand::Create (internal, static)
//
// Creates a new CompileCommand instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	command			- Unmanaged CXCompileCommand to take ownership of

CompileCommand^ CompileCommand::Create(SafeHandle^ owner, CXCompileCommand&& command)
{
	return gcnew CompileCommand(gcnew CompileCommandHandle(owner, std::move(command)));
}

//---------------------------------------------------------------------------
// CompileCommand::Filename::get
//
// Gets the filename associated with the CompileCommand

String^ CompileCommand::Filename::get(void)
{
	if(Object::ReferenceEquals(m_filename, nullptr))
		m_filename = StringUtil::ToString(clang_CompileCommand_getFilename(CompileCommandHandle::Reference(m_handle)));

	return m_filename;
}

//---------------------------------------------------------------------------
// CompileCommand::SourceMappings::get
//
// Gets a collection of compiler invocation source mappings

CompileCommandSourceMappingCollection^ CompileCommand::SourceMappings::get(void)
{
	if(Object::ReferenceEquals(m_mappings, nullptr))
		m_mappings = CompileCommandSourceMappingCollection::Create(m_handle->Owner, CompileCommandHandle::Reference(m_handle));

	return m_mappings;
}

//---------------------------------------------------------------------------
// CompileCommand::WorkingDirectory::get
//
// Gets the working directory where the CompileCommand was executed from

String^ CompileCommand::WorkingDirectory::get(void)
{
	if(Object::ReferenceEquals(m_workdir, nullptr))
		m_workdir = StringUtil::ToString(clang_CompileCommand_getDirectory(CompileCommandHandle::Reference(m_handle)));

	return m_workdir;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
