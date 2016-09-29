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

#ifndef __EVALUATIONRESULTKIND_H_
#define __EVALUATIONRESULTKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Enum EvaluationResultKind
//
// Describes the kind of a cursor evaluation result
//---------------------------------------------------------------------------

public enum class EvaluationResultKind
{
  Integer					= CXEvalResultKind::CXEval_Int,
  Float						= CXEvalResultKind::CXEval_Float,
  ObjectiveCStringLiteral	= CXEvalResultKind::CXEval_ObjCStrLiteral,
  StringLiteral				= CXEvalResultKind::CXEval_StrLiteral,
  CoreFoundationString		= CXEvalResultKind::CXEval_CFStr,
  Other						= CXEvalResultKind::CXEval_Other,
  Unexposed					= CXEvalResultKind::CXEval_UnExposed,
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __EVALUATIONRESULTKIND_H_
