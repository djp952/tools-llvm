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
#include "ModuleMapDescriptorExtensions.h"

#include "ModuleMapDescriptor.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)
BEGIN_NAMESPACE(extensions)

//---------------------------------------------------------------------------
// ModuleMapDescriptorExtensions::Save (static)
//
// Saves the module.modulemap to the specified file (UTF-8)
//
// Arguments:
//
//	descriptor	- ModuleMapDescriptor instance being extended
//	path		- Path to the output file to be generated

void ModuleMapDescriptorExtensions::Save(ModuleMapDescriptor^ descriptor, String^ path)
{
	if(Object::ReferenceEquals(descriptor, nullptr)) throw gcnew ArgumentNullException("descriptor");
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");

	return Save(descriptor, path, Encoding::UTF8);
}

//---------------------------------------------------------------------------
// ModuleMapDescriptorExtensions::Save (static)
//
// Saves the module.modulemap to the specified file
//
// Arguments:
//
//	descriptor	- ModuleMapDescriptor instance being extended
//	path		- Path to the output file to be generated
//	encoding	- Encoding to use when generating the file

void ModuleMapDescriptorExtensions::Save(ModuleMapDescriptor^ descriptor, String^ path, Encoding^ encoding)
{
	if(Object::ReferenceEquals(descriptor, nullptr)) throw gcnew ArgumentNullException("descriptor");
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");
	if(Object::ReferenceEquals(encoding, nullptr)) throw gcnew ArgumentNullException("encoding");

	// Attempt to (re)create the output file. File::Create() should throw if there is a problem,
	// but need to check for that before trying to use the StreamWriter instance
	StreamWriter^ writer = gcnew StreamWriter(File::Create(path), encoding);
	if(Object::ReferenceEquals(writer, nullptr)) throw gcnew IOException(path);

	writer->Write(descriptor->ToByteArray());		// Write the data
	writer->Flush();								// Flush output stream
	delete writer;									// Dispose stream writer
}

//---------------------------------------------------------------------------
// ModuleMapDescriptorExtensions::WriteTo (static)
//
// Writes the module.modulemap data to the specified stream
//
// Arguments:
//
//	descriptor	- ModuleMapDescriptor instance being extended
//	stream		- Stream to write the character data into

void ModuleMapDescriptorExtensions::WriteTo(ModuleMapDescriptor^ descriptor, Stream^ stream)
{
	if(Object::ReferenceEquals(descriptor, nullptr)) throw gcnew ArgumentNullException("descriptor");
	if(Object::ReferenceEquals(stream, nullptr)) throw gcnew ArgumentNullException("stream");

	array<Byte>^ characterdata = descriptor->ToByteArray();
	stream->Write(characterdata, 0, characterdata->Length);
}

//---------------------------------------------------------------------------

END_NAMESPACE(extensions)
END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
