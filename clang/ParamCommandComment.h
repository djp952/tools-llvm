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

#ifndef __PARAMCOMMANDCOMMENT_H_
#define __PARAMCOMMANDCOMMENT_H_
#pragma once

#include "BlockCommandComment.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
enum class ParameterDirection;

//---------------------------------------------------------------------------
// Class ParamCommandComment
//
// Specializes Command for CommentKind::ParamCommand comments
//---------------------------------------------------------------------------

public ref class ParamCommandComment sealed : public BlockCommandComment
{
public:

	//-----------------------------------------------------------------------
	// Member Functions

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// Direction
	//
	// Get the parameter passing direction
	property ParameterDirection Direction
	{
		ParameterDirection get(void);
	}

	// Index
	//
	// Gets the parameter index in the function prototype
	property Nullable<unsigned int> Index
	{
		Nullable<unsigned int> get(void);
	}

	// IsDirectionExplicit
	//
	// Gets a flag indicating if parameter passing direction was specified explicitly
	property bool IsDirectionExplicit
	{
		bool get(void);
	}

	// ParameterName
	//
	// Gets the parameter name
	property String^ ParameterName
	{
		String^ get(void);
	}

internal:

	// Instance Constructor
	//
	ParamCommandComment(CommentHandle^ handle);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_paramname;		// Cached parameter name
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __PARAMCOMMANDCOMMENT_H_
