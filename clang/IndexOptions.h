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

#ifndef __INDEXOPTIONS_H_
#define __INDEXOPTIONS_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum IndexOptions
//
// Flags that control how indexing is performed
//---------------------------------------------------------------------------

[FlagsAttribute]
public enum class IndexOptions
{
	None								= CXIndexOptFlags::CXIndexOpt_None,
	SuppressRedundantReferences			= CXIndexOptFlags::CXIndexOpt_SuppressRedundantRefs,
	IndexFunctionLocalSymbols			= CXIndexOptFlags::CXIndexOpt_IndexFunctionLocalSymbols,
	IndexImplicitTemplateInstantiations	= CXIndexOptFlags::CXIndexOpt_IndexImplicitTemplateInstantiations,
	SuppressWarnings					= CXIndexOptFlags::CXIndexOpt_SuppressWarnings,
	SkipParsedBodiesInSession			= CXIndexOptFlags::CXIndexOpt_SkipParsedBodiesInSession,

	// This is a custom flag used to set CXTranslationUnit_DetailedPreprocessingRecord during
	// indexing.  This is the only translation unit flag that's obeyed by the indexer when a
	// translation unit is not requested after indexing is complete
	DetailedPreprocessingRecord			= 0x20000000,

	// This is a custom flag put in to handle clang_indexSourceFile() without requiring a complete
	// duplicate set of overloaded IndexAction creation methods, the method signature is identical 
	// but the interpretation changes -- I think it should have been a flag in the clang API
	ArgumentsAreFullCommandLine			= 0x40000000,
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __INDEXOPTIONS_H_
