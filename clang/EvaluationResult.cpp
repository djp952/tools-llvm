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
#include "EvaluationResult.h"

#include "EvaluationResultKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// EvaluationResult Constructor (private)
//
// Arguments:
//
//	handle			- Underlying EvaluationResultHandle instance

EvaluationResult::EvaluationResult(EvaluationResultHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	m_kind = static_cast<EvaluationResultKind>(clang_EvalResult_getKind(EvaluationResultHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// EvaluationResult Destructor

EvaluationResult::~EvaluationResult()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// EvaluationResult::Create (internal, static)
//
// Creates a new EvaluationResult instance
//
// Arguments:
//
//	result			- Unmanaged CXEvalResult instance to take ownership of

EvaluationResult^ EvaluationResult::Create(CXEvalResult&& result)
{
	return gcnew EvaluationResult(gcnew EvaluationResultHandle(std::move(result)));
}

//---------------------------------------------------------------------------
// EvaluationResult::FloatValue::get
//
// Gets the evaluation result for floating point result types

Nullable<double> EvaluationResult::FloatValue::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_kind == EvaluationResultKind::Float) ? Nullable<double>(clang_EvalResult_getAsDouble(EvaluationResultHandle::Reference(m_handle))) : Nullable<double>();
}

//---------------------------------------------------------------------------
// EvaluationResult::IntegerValue::get
//
// Gets the evaluation result for integer result types

Nullable<int> EvaluationResult::IntegerValue::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_kind == EvaluationResultKind::Integer) ? Nullable<int>(clang_EvalResult_getAsInt(EvaluationResultHandle::Reference(m_handle))) : Nullable<int>();
}

//---------------------------------------------------------------------------
// EvaluationResult::Kind::get
//
// Gets the evaluation result kind

EvaluationResultKind EvaluationResult::Kind::get(void)
{
	return m_kind;
}

//---------------------------------------------------------------------------
// EvaluationResult::StringValue::get
//
// Gets the evaluation result for string result types

String^ EvaluationResult::StringValue::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return (m_kind == EvaluationResultKind::CoreFoundationString || m_kind == EvaluationResultKind::ObjectiveCStringLiteral || m_kind == EvaluationResultKind::StringLiteral) ? 
		StringUtil::ToString(clang_EvalResult_getAsStr(EvaluationResultHandle::Reference(m_handle)), CP_UTF8) : nullptr;
}

//---------------------------------------------------------------------------
// EvaluationResult::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ EvaluationResult::ToString(void)
{
	CHECK_DISPOSED(m_disposed);

	EvaluationResultHandle::Reference handle(m_handle);

	switch(m_kind) {

		// Integer result
		//
		case EvaluationResultKind::Integer:
			return Convert::ToString(clang_EvalResult_getAsInt(handle));

		// Floating point result
		//
		case EvaluationResultKind::Float:
			return Convert::ToString(clang_EvalResult_getAsDouble(handle));

		// String result
		//
		case EvaluationResultKind::CoreFoundationString:
		case EvaluationResultKind::ObjectiveCStringLiteral:
		case EvaluationResultKind::StringLiteral:
			return StringUtil::ToString(clang_EvalResult_getAsStr(handle), CP_UTF8);

		default: return String::Empty;
	}
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
