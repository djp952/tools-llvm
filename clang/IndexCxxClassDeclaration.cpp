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
#include "IndexCxxClassDeclaration.h"

#include "IndexBaseClassCollection.h"
#include "IndexDeclarationKind.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// IndexCxxDeclaration Constructor (internal)
//
// Arguments:
//
//	handle		- Reference handle instance
//	basehandle	- IdxDeclInfoHandle to pass to the base class

IndexCxxClassDeclaration::IndexCxxClassDeclaration(IdxCXXClassDeclInfoHandle^ handle, IdxDeclInfoHandle^ basehandle) 
	: m_handle(handle), IndexDeclaration(basehandle, IndexDeclarationKind::CxxClass)
{
}

//---------------------------------------------------------------------------
// IndexCxxDeclaration::BaseClasses::get
//
// Gets a read-only collection of base class declarations

IndexBaseClassCollection^ IndexCxxClassDeclaration::BaseClasses::get(void)
{
	if(Object::ReferenceEquals(m_bases, nullptr))
		m_bases = IndexBaseClassCollection::Create(m_handle->Owner, m_handle->IndexAction, IdxCXXClassDeclInfoHandle::Reference(m_handle));

	return m_bases;
}

//---------------------------------------------------------------------------
// IndexCxxDeclaration::Create (static)
//
// Creates a new IndexCxxClassDeclaration instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	action		- IndexAction instance
//	info		- Unmanaged CXIdxCXXClassDeclInfo pointer

IndexCxxClassDeclaration^ IndexCxxClassDeclaration::Create(SafeHandle^ owner, IndexAction^ action, const CXIdxCXXClassDeclInfo* info)
{
	if(info == __nullptr) throw gcnew ArgumentNullException("info");
	return gcnew IndexCxxClassDeclaration(gcnew IdxCXXClassDeclInfoHandle(owner, action, info), gcnew IdxDeclInfoHandle(owner, action, info->declInfo));
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
