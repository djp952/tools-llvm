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
#include "DiagnosticFixIt.h"

#include "Extent.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// DiagnosticFixIt Constructor (private)
//
// Arguments:
//
//	replacement		- The fix-it replacement text
//	extent			- The source range to which the fix-it applies

DiagnosticFixIt::DiagnosticFixIt(String^ replacement, local::Extent^ extent) : m_replacement(replacement), m_extent(extent)
{
	if(Object::ReferenceEquals(replacement, nullptr)) throw gcnew ArgumentNullException("replacement");
	if(Object::ReferenceEquals(extent, nullptr)) throw gcnew ArgumentNullException("extent");
}

//---------------------------------------------------------------------------
// DiagnosticFixIt::Create (static)
//
// Creates a new DiagnosticFixIt instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnitHandle instance
//	diagnostic	- Unmanaged CXDiagnostic instance
//	index		- Index of the fix-it to be retrieved

DiagnosticFixIt^ DiagnosticFixIt::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic, unsigned int index)
{
	CXSourceRange				extent;				// Fix-it extent/source range

	// Retrieve the replacement text and extent information and wrap a DiagnosticFixIt around them
	String^ replacement = StringUtil::ToString(clang_getDiagnosticFixIt(diagnostic, index, &extent));
	return gcnew DiagnosticFixIt(replacement, local::Extent::Create(owner, transunit, extent));
}

//---------------------------------------------------------------------------
// DiagnosticFixIt::Extent::get
//
// Gets the source range to be replaced with the fix-it text

local::Extent^ DiagnosticFixIt::Extent::get(void)
{
	return m_extent;
}

//---------------------------------------------------------------------------
// DiagnosticFixIt::ReplacementText::get
//
// Gets the replacement text associated with this fix-it

String^ DiagnosticFixIt::ReplacementText::get(void)
{
	return m_replacement;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
