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

#ifndef __OBJECTIVECDECLARATIONQUALIFIER_H_
#define __OBJECTIVECDECLARATIONQUALIFIER_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum ObjectiveCDeclarationQualifier
//
// 'Qualifiers' written next to the return and parameter types in Objective-C 
// method declarations
//---------------------------------------------------------------------------

[FlagsAttribute]
public enum class ObjectiveCDeclarationQualifier
{
	None		= CXObjCDeclQualifierKind::CXObjCDeclQualifier_None,
	In			= CXObjCDeclQualifierKind::CXObjCDeclQualifier_In,
	InOut		= CXObjCDeclQualifierKind::CXObjCDeclQualifier_Inout,
	Out			= CXObjCDeclQualifierKind::CXObjCDeclQualifier_Out,
	ByCopy		= CXObjCDeclQualifierKind::CXObjCDeclQualifier_Bycopy,
	ByRef		= CXObjCDeclQualifierKind::CXObjCDeclQualifier_Byref,
	OneWay		= CXObjCDeclQualifierKind::CXObjCDeclQualifier_Oneway,
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __OBJECTIVECDECLARATIONQUALIFIER_H_