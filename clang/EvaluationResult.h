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

#ifndef __EVALUATIONRESULT_H_
#define __EVALUATIONRESULT_H_
#pragma once

#include "UnmanagedTypeSafeHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

// FORWARD DECLARATIONS
//
enum class EvaluationResultKind;

//---------------------------------------------------------------------------
// Class EvaluationResult
//
// If cursor is a statement declaration tries to evaluate the statement  and
// if variable, tries to evaluate its initializer into its corresponding type
//---------------------------------------------------------------------------

public ref class EvaluationResult
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

	// FloatValue
	//
	// Gets the evaluation result for floating point result types
	property Nullable<double> FloatValue
	{
		Nullable<double> get(void);
	}

	// IntegerValue
	//
	// Gets the evaluation result for integer result types
	property Nullable<int> IntegerValue
	{
		Nullable<int> get(void);
	}

	// Kind
	//
	// Gets the evaluation result kind
	property EvaluationResultKind Kind
	{
		EvaluationResultKind get(void);
	}

	// StringValue
	//
	// Gets the evaluation result for string result types
	property String^ StringValue
	{
		String^ get(void);
	}

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new EvaluationResult instance
	static EvaluationResult^ Create(CXEvalResult&& result);

private:

	// EvaluationResultHandle
	//
	// UnmanagedTypeSafeHandle specialization for CXEvalResult
	using EvaluationResultHandle = UnmanagedTypeSafeHandle<CXEvalResult, clang_EvalResult_dispose>;

	// Instance Constructor
	//
	EvaluationResult(EvaluationResultHandle^ handle);

	// Destructor
	//
	~EvaluationResult();

	//-----------------------------------------------------------------------
	// Member Variables

	bool						m_disposed;			// Object disposal flag
	EvaluationResultHandle^		m_handle;			// Reference handle
	EvaluationResultKind		m_kind;				// Kind of evaluation result
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __EVALUATIONRESULT_H_
