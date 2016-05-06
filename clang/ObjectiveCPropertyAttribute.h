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

#ifndef __OBJECTIVECPROPERTYATTRIBUTE_H_
#define __OBJECTIVECPROPERTYATTRIBUTE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Enum ObjectiveCPropertyAttribute
//
// Property attributes ObjCPropertyDecl Cursor
//---------------------------------------------------------------------------

[FlagsAttribute]
public enum class ObjectiveCPropertyAttribute
{
	None				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_noattr,
	ReadOnly			= CXObjCPropertyAttrKind::CXObjCPropertyAttr_readonly,
	Getter				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_getter,
	Assign				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_assign,
	ReadWrite			= CXObjCPropertyAttrKind::CXObjCPropertyAttr_readwrite,
	Retain				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_retain,
	Copy				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_copy,
	NonAtomic			= CXObjCPropertyAttrKind::CXObjCPropertyAttr_nonatomic,
	Setter				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_setter,
	Atomic				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_atomic,
	Weak				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_weak,
	Strong				= CXObjCPropertyAttrKind::CXObjCPropertyAttr_strong,
	UnsafeUnretained	= CXObjCPropertyAttrKind::CXObjCPropertyAttr_unsafe_unretained,
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __OBJECTIVECPROPERTYATTRIBUTE_H_
