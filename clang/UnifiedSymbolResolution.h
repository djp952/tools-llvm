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

#ifndef __UNIFIEDSYMBOLRESOLUTION_H_
#define __UNIFIEDSYMBOLRESOLUTION_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
ref class Cursor;

//---------------------------------------------------------------------------
// Class UnifiedSymbolResolution
//
// A string that identifies a particular entity(function, class, variable, etc.) 
// within a program.  
//---------------------------------------------------------------------------

public ref class UnifiedSymbolResolution
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(UnifiedSymbolResolution^ lhs, UnifiedSymbolResolution^ rhs);

	// operator!= (static)
	//
	static bool operator!=(UnifiedSymbolResolution^ lhs, UnifiedSymbolResolution^ rhs);

	// String^ conversion operator (implicit)
	//
	static operator String^(UnifiedSymbolResolution^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this UnifiedSymbolResolution instance to another UnifiedSymbolResolution instance
	bool Equals(UnifiedSymbolResolution^ rhs);

	// FromObjectiveCCategory (static)
	//
	// Construct a USR for a specified Objective-C category
	static UnifiedSymbolResolution^ FromObjectiveCCategory(String^ classname, String^ categoryname);

	// FromObjectiveCClass (static)
	//
	// Construct a USR for a specified Objective-C class
	static UnifiedSymbolResolution^ FromObjectiveCClass(String^ classname);

	// FromObjectiveCMethod (static)
	//
	// Construct a USR for a specified Objective-C method
	static UnifiedSymbolResolution^ FromObjectiveCMethod(String^ name, bool isinstance, UnifiedSymbolResolution^ classusr);

	// FromObjectiveCInstanceVariable (static)
	//
	// Construct a USR for a specified Objective-C instance variable
	static UnifiedSymbolResolution^ FromObjectiveCInstanceVariable(String^ name, UnifiedSymbolResolution^ classusr);

	// FromObjectiveCProperty (static)
	//
	// Construct a USR for a specified Objective-C property
	static UnifiedSymbolResolution^ FromObjectiveCProperty(String^ name, UnifiedSymbolResolution^ classusr);

	// FromObjectiveCProtocol (static)
	//
	// Construct a USR for a specified Objective-C protocol
	static UnifiedSymbolResolution^ FromObjectiveCProtocol(String^ protocolname);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create (static)
	//
	// Creates a new UnifiedSymbolResolution instance
	static UnifiedSymbolResolution^ Create(CXString&& string);
	static UnifiedSymbolResolution^ Create(const char* psz);

private:

	// Instance Constructor
	//
	UnifiedSymbolResolution(String^ usr);

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_usr;		// Converted string instance
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __UNIFIEDSYMBOLRESOLUTION_H_
