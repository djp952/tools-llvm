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

#ifndef __COMPILATIONDATABASELOADEXCEPTION_H_
#define __COMPILATIONDATABASELOADEXCEPTION_H_
#pragma once

#include "CompilationDatabaseLoadErrorCode.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;
using namespace System::Runtime::Serialization;
using namespace System::Security;
using namespace System::Security::Permissions;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Class CompilationDatabaseLoadException
//
// Represents an error that occurred loading a compilation database
//---------------------------------------------------------------------------

[SerializableAttribute]
public ref class CompilationDatabaseLoadException sealed : public Exception
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// GetObjectData
	//
	// Overrides Exception::GetObjectData
	[SecurityCriticalAttribute]
	[PermissionSetAttribute(SecurityAction::LinkDemand, Unrestricted = true)]
	[PermissionSetAttribute(SecurityAction::InheritanceDemand, Unrestricted = true)]
	[SecurityPermissionAttribute(SecurityAction::Demand, SerializationFormatter = true)]
	virtual void GetObjectData(SerializationInfo^ info, StreamingContext context) override;

	//-----------------------------------------------------------------------
	// Properties

	// ErrorCode
	//
	// Exposes the CompilationDatabaseLoadErrorCode
	property CompilationDatabaseLoadErrorCode ErrorCode
	{
		CompilationDatabaseLoadErrorCode get(void);
	}

internal:

	// Instance Constructors
	//
	CompilationDatabaseLoadException(CXCompilationDatabase_Error code);
	CompilationDatabaseLoadException(CompilationDatabaseLoadErrorCode code);

private:

	// Serialization Constructor
	//
	[SecurityPermissionAttribute(SecurityAction::Demand, SerializationFormatter = true)]
	CompilationDatabaseLoadException(SerializationInfo^ info, StreamingContext context);

	//-----------------------------------------------------------------------
	// Member Variables

	CompilationDatabaseLoadErrorCode	m_code;			// Underlying error code
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMPILATIONDATABASEEXCEPTION_H_
