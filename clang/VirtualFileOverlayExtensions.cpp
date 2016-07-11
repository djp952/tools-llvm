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
#include "VirtualFileOverlayExtensions.h"

#include "VirtualFileOverlay.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang::extensions {

//---------------------------------------------------------------------------
// VirtualFileOverlayExtensions::Save (static)
//
// Saves the virtual file overlay data to the specified file (UTF-8)
//
// Arguments:
//
//	overlay		- VirtualFileOverlay instance being extended
//	path		- Path to the output file to be generated

void VirtualFileOverlayExtensions::Save(VirtualFileOverlay^ overlay, String^ path)
{
	if(Object::ReferenceEquals(overlay, nullptr)) throw gcnew ArgumentNullException("overlay");
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");

	return Save(overlay, path, Encoding::UTF8);
}

//---------------------------------------------------------------------------
// VirtualFileOverlayExtensions::Save (static)
//
// Saves the virtual file overlay data to the specified file
//
// Arguments:
//
//	overlay		- VirtualFileOverlay instance being extended
//	path		- Path to the output file to be generated
//	encoding	- Encoding to use when generating the file

void VirtualFileOverlayExtensions::Save(VirtualFileOverlay^ overlay, String^ path, Encoding^ encoding)
{
	if(Object::ReferenceEquals(overlay, nullptr)) throw gcnew ArgumentNullException("overlay");
	if(Object::ReferenceEquals(path, nullptr)) throw gcnew ArgumentNullException("path");
	if(Object::ReferenceEquals(encoding, nullptr)) throw gcnew ArgumentNullException("encoding");

	// Attempt to (re)create the output file. File::Create() should throw if there is a problem,
	// but need to check for that before trying to use the StreamWriter instance
	StreamWriter^ writer = gcnew StreamWriter(File::Create(path), encoding);
	if(Object::ReferenceEquals(writer, nullptr)) throw gcnew IOException(path);

	writer->Write(overlay->ToByteArray());			// Write the data
	writer->Flush();								// Flush output stream
	delete writer;									// Dispose stream writer
}

//---------------------------------------------------------------------------
// VirtualFileOverlayExtensions::WriteTo (static)
//
// Writes the virtual file overlay data to the specified stream
//
// Arguments:
//
//	overlay		- VirtualFileOverlay instance being extended
//	stream		- Stream to write the character data into

void VirtualFileOverlayExtensions::WriteTo(VirtualFileOverlay^ overlay, Stream^ stream)
{
	if(Object::ReferenceEquals(overlay, nullptr)) throw gcnew ArgumentNullException("overlay");
	if(Object::ReferenceEquals(stream, nullptr)) throw gcnew ArgumentNullException("stream");

	array<Byte>^ characterdata = overlay->ToByteArray();
	stream->Write(characterdata, 0, characterdata->Length);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang::extensions

#pragma warning(pop)
