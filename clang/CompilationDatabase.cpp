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
#include "CompilationDatabase.h"

#include "CompileCommandCollection.h"
#include "StringUtil.h"

using namespace System::Linq;

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CompilationDatabase Constructor (private)
//
// Arguments:
//
//	handle			- Underlying CompilationDatabaseHandle instance

CompilationDatabase::CompilationDatabase(CompilationDatabaseHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// CompilationDatabase Destructor

CompilationDatabase::~CompilationDatabase()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// CompilationDatabase::Create (internal, static)
//
// Creates a new CompilationDatabase instance
//
// Arguments:
//
//	database	- Unmanaged CXCompilationDatabase instance to take ownership of

CompilationDatabase^ CompilationDatabase::Create(CXCompilationDatabase&& database)
{
	return gcnew CompilationDatabase(gcnew CompilationDatabaseHandle(std::move(database)));
}

//---------------------------------------------------------------------------
// CompilationDatabase::GetCompileCommands
//
// Gets the collection of all compile commands from the database
//
// Arguments:
//
//	NONE

CompileCommandCollection^ CompilationDatabase::GetCompileCommands(void)
{
	CHECK_DISPOSED(m_disposed);
	return CompileCommandCollection::Create(clang_CompilationDatabase_getAllCompileCommands(CompilationDatabaseHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// CompilationDatabase::GetCompileCommands
//
// Gets the collection of compile commands from the database for a file
//
// Arguments:
//
//	filename		- Name of the file in the database

CompileCommandCollection^ CompilationDatabase::GetCompileCommands(String^ filename)
{
	CHECK_DISPOSED(m_disposed);

	char* pszname = StringUtil::ToCharPointer(filename, CP_UTF8);

	try { return CompileCommandCollection::Create(clang_CompilationDatabase_getCompileCommands(CompilationDatabaseHandle::Reference(m_handle), pszname)); }
	finally { StringUtil::FreeCharPointer(pszname); }
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
