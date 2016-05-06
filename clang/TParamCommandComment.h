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

#ifndef __TPARAMCOMMANDCOMMENT_H_
#define __TPARAMCOMMANDCOMMENT_H_
#pragma once

#include "BlockCommandComment.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

// FORWARD DECLARATIONS
//
ref class TParamCommandIndex;

//---------------------------------------------------------------------------
// Class TParamCommandComment
//
// Specializes Command for CommentKind::TParamCommand comments
//---------------------------------------------------------------------------

public ref class TParamCommandComment sealed : public BlockCommandComment
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

	// Index
	//
	// Gets the index in the template parameter list at a given nesting depth
	property TParamCommandIndex^ Index
	{
		TParamCommandIndex^ get(void);
	}

	// ParameterName
	//
	// Gets the template parameter name
	property String^ ParameterName
	{
		String^ get(void);
	}

internal:

	// Instance Constructor
	//
	TParamCommandComment(CommentHandle^ handle);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	String^					m_paramname;		// Cached parameter name
	TParamCommandIndex^		m_indexes;			// Cached parameter indexes
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __TPARAMCOMMANDCOMMENT_H_
