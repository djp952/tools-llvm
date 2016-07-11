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

#ifndef __COMPLETIONCONTEXT_H_
#define __COMPLETIONCONTEXT_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Enum CompletionContext
//
// Flags used to represent the context under which completion is occurring
//---------------------------------------------------------------------------

[FlagsAttribute]
public enum class CompletionContext
{
	Unexposed					= CXCompletionContext::CXCompletionContext_Unexposed,
	AnyType						= CXCompletionContext::CXCompletionContext_AnyType,
	AnyValue					= CXCompletionContext::CXCompletionContext_AnyValue,
	ObjectiveCObjectValue		= CXCompletionContext::CXCompletionContext_ObjCObjectValue,
	ObjectiveCSelectorValue		= CXCompletionContext::CXCompletionContext_ObjCSelectorValue,
	CxxClassTypeValue			= CXCompletionContext::CXCompletionContext_CXXClassTypeValue,
	DotMemberAccess				= CXCompletionContext::CXCompletionContext_DotMemberAccess,
	ArrowMemberAccess			= CXCompletionContext::CXCompletionContext_ArrowMemberAccess,
	ObjectiveCPropertyAccess	= CXCompletionContext::CXCompletionContext_ObjCPropertyAccess,
	EnumTag						= CXCompletionContext::CXCompletionContext_EnumTag,
	UnionTag					= CXCompletionContext::CXCompletionContext_UnionTag,
	StructTag					= CXCompletionContext::CXCompletionContext_StructTag,
	ClassTag					= CXCompletionContext::CXCompletionContext_ClassTag,
	Namespace					= CXCompletionContext::CXCompletionContext_Namespace,
	NestedNameSpecifier			= CXCompletionContext::CXCompletionContext_NestedNameSpecifier,
	ObjectiveCInterface			= CXCompletionContext::CXCompletionContext_ObjCInterface,
	ObjectiveCProtocol			= CXCompletionContext::CXCompletionContext_ObjCProtocol,
	ObjectiveCCategory			= CXCompletionContext::CXCompletionContext_ObjCCategory,
	ObjectiveCInstanceMessage	= CXCompletionContext::CXCompletionContext_ObjCInstanceMessage,
	ObjectiveCClassMessage		= CXCompletionContext::CXCompletionContext_ObjCClassMessage,
	ObjectiveCSelectorName		= CXCompletionContext::CXCompletionContext_ObjCSelectorName,
	MacroName					= CXCompletionContext::CXCompletionContext_MacroName,
	NaturalLanguage				= CXCompletionContext::CXCompletionContext_NaturalLanguage,
	Unknown						= CXCompletionContext::CXCompletionContext_Unknown,
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMPLETIONCONTEXT_H_
