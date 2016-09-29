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

#ifndef __CALLINGCONVENTION_H_
#define __CALLINGCONVENTION_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Enum CallingConvention
//
// Describes the calling convention of a function type
//---------------------------------------------------------------------------

public enum class CallingConvention
{
	Default			= CXCallingConv::CXCallingConv_Default,
	C				= CXCallingConv::CXCallingConv_C,
	StdCall			= CXCallingConv::CXCallingConv_X86StdCall,
	FastCall		= CXCallingConv::CXCallingConv_X86FastCall,
	ThisCall		= CXCallingConv::CXCallingConv_X86ThisCall,
	Pascal			= CXCallingConv::CXCallingConv_X86Pascal,
	AAPCS			= CXCallingConv::CXCallingConv_AAPCS,
	AAPCSVFP		= CXCallingConv::CXCallingConv_AAPCS_VFP,
	IntelOclBicc	= CXCallingConv::CXCallingConv_IntelOclBicc,
	Win64			= CXCallingConv::CXCallingConv_X86_64Win64,
	SysV			= CXCallingConv::CXCallingConv_X86_64SysV,
	VectorCall		= CXCallingConv::CXCallingConv_X86VectorCall,
	Swift			= CXCallingConv::CXCallingConv_Swift,
	PreserveMost	= CXCallingConv::CXCallingConv_PreserveMost,
	PreserveAll		= CXCallingConv::CXCallingConv_PreserveAll,
	Invalid			= CXCallingConv::CXCallingConv_Invalid,
	Unexposed		= CXCallingConv::CXCallingConv_Unexposed,
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __CALLINGCONVENTION_H_
