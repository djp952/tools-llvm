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
#include "ModuleMapDescriptor.h"

#include "ClangException.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// ModuleMapDescriptor Constructor (private)
//
// Arguments:
//
//	handle			- Underlying ModuleMapDescriptorHandle instance
//	frameworkmodule	- Initial string to assign for the framework module name
//	umbrellaheader	- Initial string to assign for the umbrella header name

ModuleMapDescriptor::ModuleMapDescriptor(ModuleMapDescriptorHandle^ handle, String^ frameworkmodule, String^ umbrellaheader) 
	: m_handle(handle), m_module(frameworkmodule), m_header(umbrellaheader)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	if(Object::ReferenceEquals(frameworkmodule, nullptr)) throw gcnew ArgumentNullException("frameworkmodule");
	if(Object::ReferenceEquals(umbrellaheader, nullptr)) throw gcnew ArgumentNullException("umbrellaheader");
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor Destructor

ModuleMapDescriptor::~ModuleMapDescriptor()
{
	if(m_disposed) return;

	delete m_handle;					// Release the safe handle
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::Create (internal, static)
//
// Creates a new ModuleMapDescriptor instance
//
// Arguments:
//
//	descriptor		- Unmanaged CXModuleMapDescriptor instance to take ownership of
//	frameworkmodule	- Initial string to assign for the framework module name
//	umbrellaheader	- Initial string to assign for the umbrella header name

ModuleMapDescriptor^ ModuleMapDescriptor::Create(CXModuleMapDescriptor&& descriptor, String^ frameworkmodule, String^ umbrellaheader)
{
	return gcnew ModuleMapDescriptor(gcnew ModuleMapDescriptorHandle(std::move(descriptor)), frameworkmodule, umbrellaheader);
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::FrameworkModule::get
//
// Gets the module.map framework module name

String^ ModuleMapDescriptor::FrameworkModule::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_module;
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::FrameworkModule::set
//
// Sets the module.map framework module name

void ModuleMapDescriptor::FrameworkModule::set(String^ value)
{
	CHECK_DISPOSED(m_disposed);
	if(Object::ReferenceEquals(value, nullptr)) throw gcnew ArgumentNullException("value");

	char* pszvalue = StringUtil::ToCharPointer(value, CP_UTF8);

	try { 
	
		CXErrorCode result = clang_ModuleMapDescriptor_setFrameworkModuleName(ModuleMapDescriptorHandle::Reference(m_handle), pszvalue);
		if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(result);;

		m_module = value;			// Cache the string value for the getter
	}

	finally { StringUtil::FreeCharPointer(pszvalue); }
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::ToByteArray
//
// Converts the underlying character data into a byte array
//
// Arguments:
//
//	NONE

array<Byte>^ ModuleMapDescriptor::ToByteArray(void)
{
	char*			buffer;				// [out] buffer pointer from clang
	unsigned int	buffersize;			// [out] buffer size from clang

	CHECK_DISPOSED(m_disposed);

	// Attempt to read the module.modulemap data into a character buffer
	CXErrorCode result = clang_ModuleMapDescriptor_writeToBuffer(ModuleMapDescriptorHandle::Reference(m_handle), 0, &buffer, &buffersize);
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
// ModuleMapDescriptor::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ ModuleMapDescriptor::ToString(void)
{
	char*			buffer;				// [out] buffer pointer from clang
	unsigned int	buffersize;			// [out] buffer size from clang

	if(m_disposed) return String::Empty;

	// Attempt to read the module.modulemap data into a character buffer
	CXErrorCode result = clang_ModuleMapDescriptor_writeToBuffer(ModuleMapDescriptorHandle::Reference(m_handle), 0, &buffer, &buffersize);
	if(result != CXErrorCode::CXError_Success) return String::Empty;

	try { return StringUtil::ToString(buffer, buffersize, CP_UTF8); }
	finally { clang_free(buffer); }
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::UmbrellaHeader::get
//
// Gets the module.map umbrella header name

String^ ModuleMapDescriptor::UmbrellaHeader::get(void)
{
	CHECK_DISPOSED(m_disposed);
	return m_header;
}

//---------------------------------------------------------------------------
// ModuleMapDescriptor::UmbrellaHeader::set
//
// Sets the module.map umbrella header name

void ModuleMapDescriptor::UmbrellaHeader::set(String^ value)
{
	CHECK_DISPOSED(m_disposed);
	if(Object::ReferenceEquals(value, nullptr)) throw gcnew ArgumentNullException("value");

	char* pszvalue = StringUtil::ToCharPointer(value, CP_UTF8);

	try { 
	
		CXErrorCode result = clang_ModuleMapDescriptor_setUmbrellaHeader(ModuleMapDescriptorHandle::Reference(m_handle), pszvalue);
		if(result != CXErrorCode::CXError_Success) throw gcnew ClangException(result);

		m_header = value;			// Cache the string value for the getter
	}

	finally { StringUtil::FreeCharPointer(pszvalue); }
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
