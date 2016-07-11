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
#include "CompileCommandSourceMapping.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompileCommandSourceMapping Constructor (private)
//
// Arguments:
//
//	path			- Mapped source path string
//	context			- Mapped source content string

CompileCommandSourceMapping::CompileCommandSourceMapping(String^ path, String^ content) : m_path(path), m_content(content)
{
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");
	if(Object::ReferenceEquals(content, nullptr)) throw gcnew ArgumentNullException("content");
}

//---------------------------------------------------------------------------
// CompileCommandSourceMapping::Content::get
//
// Get the mapped source content for the compiler invocation

String^ CompileCommandSourceMapping::Content::get(void)
{
	return m_content;
}

//---------------------------------------------------------------------------
// CompileCommandSourceMapping::Create (internal, static)
//
// Creates a new CompileCommandSourceMapping instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	command			- Unmanaged CXCompileCommand to retrieve mapped source
//	index			- Index of the mapped source in the compile command

CompileCommandSourceMapping^ CompileCommandSourceMapping::Create(SafeHandle^ owner, CXCompileCommand command, unsigned int index)
{
	UNREFERENCED_PARAMETER(owner);

	return gcnew CompileCommandSourceMapping(StringUtil::ToString(clang_CompileCommand_getMappedSourcePath(command, index)), 
		StringUtil::ToString(clang_CompileCommand_getMappedSourceContent(command, index)));
}

//---------------------------------------------------------------------------
// CompileCommandSourceMapping::Path::get
//
// Get the mapped source path for the compiler invocation

String^ CompileCommandSourceMapping::Path::get(void)
{
	return m_path;
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
