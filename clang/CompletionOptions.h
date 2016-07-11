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

#ifndef __COMPLETIONOPTIONS_H_
#define __COMPLETIONOPTIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Enum CompletionOptions
//
// Flags used to modify the behavior of code completion
//---------------------------------------------------------------------------

[FlagsAttribute]
public enum class CompletionOptions
{
	IncludeMacros			= CXCodeComplete_Flags::CXCodeComplete_IncludeMacros,
	IncludeCodePatterns		= CXCodeComplete_Flags::CXCodeComplete_IncludeCodePatterns,
	IncludeBriefComments	= CXCodeComplete_Flags::CXCodeComplete_IncludeBriefComments,

	// This is a custom flag put in to handle clang_sortCodeCompletionResults() at the time
	// of collection generation, before the pointers are referenced by managed objects
	SortAlphabetical		= 0x40000000,
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __COMPLETIONOPTIONS_H_
