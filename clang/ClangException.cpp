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
#include "ClangException.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ClangException Constructor (internal)
//
// Arguments:
//
//	code		- The CXErrorCode to associate with the exception

ClangException::ClangException(CXErrorCode code) : ClangException(ClangErrorCode(code))
{
}

//---------------------------------------------------------------------------
// ClangException Constructor (internal)
//
// Arguments:
//
//	code		- The ClangErrorCode to associate with the exception

ClangException::ClangException(ClangErrorCode code) : m_code(code), Exception(code.ToString())
{
}

//---------------------------------------------------------------------------
// ClangException Constructor (private)
//
// Arguments:
//
//	info		- Serialization information
//	context		- Serialization context

ClangException::ClangException(SerializationInfo^ info, StreamingContext context) : Exception(info, context)
{
	m_code = ClangErrorCode(static_cast<CXErrorCode>(info->GetInt32("@m_code")));
}

//---------------------------------------------------------------------------
// ClangException::ErrorCode::get
//
// Gets the error code associated with this exception

ClangErrorCode ClangException::ErrorCode::get(void)
{
	return m_code;
}

//---------------------------------------------------------------------------
// ClangException::GetObjectData
//
// Overrides Exception::GetObjectData
//
// Arguments:
//
//	info		- Serialization information
//	context		- Serialization context

void ClangException::GetObjectData(SerializationInfo^ info, StreamingContext context)
{
	if(Object::ReferenceEquals(info, nullptr)) throw gcnew ArgumentNullException("info");
	info->AddValue("@m_code", static_cast<int>(m_code));
	Exception::GetObjectData(info, context);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
