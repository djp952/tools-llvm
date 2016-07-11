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
#include "ExtentExtensions.h"

#include "Extent.h"
#include "Location.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// ExtentExtensions::GetCharacterLength (static)
//
// Gets the length of the extent in characters by subtracting the end location
// offset from the start location
//
// Arguments:
//
//	extent			- Extent instance being extended

int ExtentExtensions::GetCharacterLength(Extent^ extent)
{
	if(Object::ReferenceEquals(extent, nullptr)) throw gcnew ArgumentNullException("extent");

	if(Extent::IsNull(extent)) return 0;
	if(Location::IsNull(extent->Start) || Location::IsNull(extent->End)) return 0;

	// This returns a signed integer for convenience, but never return a negative
	return Math::Max(extent->End->Offset - extent->Start->Offset, 0);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)
