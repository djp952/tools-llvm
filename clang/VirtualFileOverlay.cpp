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
#include "VirtualFileOverlay.h"

#include "ClangException.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// VirtualFileOverlay Constructor (private)
//
// Arguments:
//
//	handle			- Underlying VirtualFileOverlayHandle instance

VirtualFileOverlay::VirtualFileOverlay(VirtualFileOverlayHandle^ handle) : m_handle(handle), m_casesensitive(true)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// VirtualFileOverlay Destructor

VirtualFileOverlay::~VirtualFileOverlay()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::AddFileMapping
//
// Maps an absolute virtual file path to an absolute real one
//
// Arguments:
//
//	virtualpath		- Virtual path (must be canonical)
//	physicalpath	- Physical path

void VirtualFileOverlay::AddFileMapping(String^ virtualpath, String^ physicalpath)
{
	CHECK_DISPOSED(m_disposed);

	if(Object::ReferenceEquals(virtualpath, nullptr)) throw gcnew ArgumentNullException("virtualpath");
	if(Object::ReferenceEquals(physicalpath, nullptr)) throw gcnew ArgumentNullException("physicalpath");

	// Both input strings must be canonical, attempt to complete them as necessary
	virtualpath = (gcnew Uri(Path::GetFullPath(virtualpath)))->LocalPath;
	physicalpath = (gcnew Uri(Path::GetFullPath(physicalpath)))->LocalPath;

	char* pszvirtual = StringUtil::ToCharPointer(virtualpath, CP_UTF8);

	try {

		char* pszphysical = StringUtil::ToCharPointer(physicalpath, CP_UTF8);

		try {

			// Attempt to add the file mapping to the underlying virtual file overlay instance
			CXErrorCode result = clang_VirtualFileOverlay_addFileMapping(VirtualFileOverlayHandle::Reference(m_handle), pszvirtual, pszphysical);
			if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(result);
		}

		finally { StringUtil::FreeCharPointer(pszphysical); }
	}

	finally { StringUtil::FreeCharPointer(pszvirtual); }
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::CaseSensitive::get
//
// Gets the overlay case sensitivity flag

bool VirtualFileOverlay::CaseSensitive::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_casesensitive;
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::CaseSensitive::set
//
// Sets the overlay case sensitivity flag

void VirtualFileOverlay::CaseSensitive::set(bool value)
{
	CHECK_DISPOSED(m_disposed);

	CXErrorCode result = clang_VirtualFileOverlay_setCaseSensitivity(VirtualFileOverlayHandle::Reference(m_handle), (value) ? -1 : 0);
	if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(result);

	m_casesensitive = value;
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::Create (internal, static)
//
// Creates a new VirtualFileOverlay instance
//
// Arguments:
//
//	overlay		- Unmanaged CXVirtualFileOverlay instance to take ownership of

VirtualFileOverlay^ VirtualFileOverlay::Create(CXVirtualFileOverlay&& overlay)
{
	return gcnew VirtualFileOverlay(gcnew VirtualFileOverlayHandle(std::move(overlay)));
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::ToByteArray
//
// Converts the underlying character data into a byte array
//
// Arguments:
//
//	NONE

array<Byte>^ VirtualFileOverlay::ToByteArray(void)
{
	char*			buffer;				// [out] buffer pointer from clang
	unsigned int	buffersize;			// [out] buffer size from clang

	CHECK_DISPOSED(m_disposed);

	// Attempt to read the virtual overlay file data into a character buffer
	CXErrorCode result = clang_VirtualFileOverlay_writeToBuffer(VirtualFileOverlayHandle::Reference(m_handle), 0, &buffer, &buffersize);
	if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(result);

	try { 
	
		// Allocate the resultant array, and if it's zero-length, we're done
		array<Byte>^ bytearray = gcnew array<Byte>(buffersize);
		if(buffersize == 0) return bytearray;

		// Pin the array in memory and copy the information from the clang buffer
		pin_ptr<unsigned __int8> pinarray = &bytearray[0];
		memcpy(pinarray, buffer, buffersize);

		return bytearray;
	}
	
	finally { clang_free(buffer); }
}

//---------------------------------------------------------------------------
// VirtualFileOverlay::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ VirtualFileOverlay::ToString(void)
{
	char*			buffer;				// [out] buffer pointer from clang
	unsigned int	buffersize;			// [out] buffer size from clang

	if(m_disposed) return String::Empty;

	// Attempt to read the virtual overlay file data into a character buffer
	CXErrorCode result = clang_VirtualFileOverlay_writeToBuffer(VirtualFileOverlayHandle::Reference(m_handle), 0, &buffer, &buffersize);
	if(result != CXErrorCode::CXError_Success) return String::Empty;

	try { return StringUtil::ToString(buffer, buffersize, CP_UTF8); }
	finally { clang_free(buffer); }
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
