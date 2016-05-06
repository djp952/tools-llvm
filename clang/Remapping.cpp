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
#include "Remapping.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Remapping Constructor (private)
//
// Arguments:
//
//	original	- Original file name
//	transformed	- Transformed file name

Remapping::Remapping(String^ original, String^ transformed) : m_original(original), m_transformed(transformed)
{
	if(Object::ReferenceEquals(original, nullptr)) throw gcnew ArgumentNullException("original");
	if(Object::ReferenceEquals(transformed, nullptr)) throw gcnew ArgumentNullException("transformed");
}

//---------------------------------------------------------------------------
// Remapping::Create (internal, static)
//
// Creates a new Remapping instance
//
// Arguments:
//
//	remapping		- Unmanaged CXRemapping instance
//	index			- Index to retrieve from the remapping

Remapping^ Remapping::Create(const CXRemapping& remapping, unsigned int index)
{
	CXString			original;			// Original file name
	CXString			transformed;		// Transformed file name

	clang_remap_getFilenames(remapping, index, &original, &transformed);
	return gcnew Remapping(StringUtil::ToString(std::move(original)), StringUtil::ToString(std::move(transformed)));
}

//---------------------------------------------------------------------------
// Remapping::OriginalFileName::get
//
// Gets the original file name

String^ Remapping::OriginalFileName::get(void)
{
	return m_original;
}

//---------------------------------------------------------------------------
// Remapping::TransformedFileName::get
//
// Gets the transformed file name

String^ Remapping::TransformedFileName::get(void)
{
	return m_transformed;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
