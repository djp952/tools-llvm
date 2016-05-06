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

using System;
using System.IO;
using System.Text;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using zuki.tools.llvm.clang.extensions;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestModuleMapDescriptor
	{
		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptor_Dispose()
		{
			ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor();
			Assert.IsFalse(descriptor.IsDisposed(() => { var v = descriptor.FrameworkModule; }));

			// Not much to this one, it's just a simple reference object.  Make
			// sure that once disposed it throws ObjectDisposedException
			descriptor.Dispose();
			Assert.IsTrue(descriptor.IsDisposed(() => { var v = descriptor.FrameworkModule; }));
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptor_FrameworkModule()
		{
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor())
			{
				Assert.IsNotNull(descriptor);
				Assert.AreSame(descriptor.FrameworkModule, descriptor.FrameworkModule);

				descriptor.FrameworkModule = "testmodule";
				Assert.AreEqual("testmodule", descriptor.FrameworkModule);
			}
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptor_ToByteArray()
		{
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor())
			{
				Assert.IsNotNull(descriptor);
				descriptor.FrameworkModule = "testmodule";
				descriptor.UmbrellaHeader = "testheader";

				byte[] output = descriptor.ToByteArray();
				Assert.AreNotEqual(0, output.Length);
			}
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptor_ToString()
		{
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor("testmodule", "testheader"))
			{
				Assert.IsNotNull(descriptor);
				Assert.AreEqual("testmodule", descriptor.FrameworkModule);
				Assert.AreEqual("testheader", descriptor.UmbrellaHeader);

				string output = descriptor.ToString();
				Assert.IsFalse(String.IsNullOrEmpty(output));
			}
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptor_UmbrellaHeader()
		{
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor())
			{
				Assert.IsNotNull(descriptor);
				Assert.AreSame(descriptor.UmbrellaHeader, descriptor.UmbrellaHeader);

				descriptor.UmbrellaHeader = "testheader";
				Assert.AreEqual("testheader", descriptor.UmbrellaHeader);
			}
		}

		//
		// EXTENSIONS
		//

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptorExtensions_Save()
		{
			// Temporary file name in which to write the module map descriptor
			string outpath = Path.GetTempFileName();

			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor("testmodule", "testheader"))
			{
				Assert.IsNotNull(descriptor);

				// The following checks verify the encoding of the output file by passing ASCII as the default
				// encoding to StreamReader, but allowing it to be overridden by the byte order marks.  If ASCII
				// is not provided as the default encoding, .CurrentEncoding will return UTF8 for both ASCII as UTF8

				// Save the file as default (UTF-8)
				descriptor.Save(outpath);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UTF8Encoding));
				}

				// Save the file as ASCII
				descriptor.Save(outpath, Encoding.ASCII);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(ASCIIEncoding));
				}

				// Save the file as UTF-8
				descriptor.Save(outpath, Encoding.UTF8);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UTF8Encoding));
				}

				// Save the file as UTF-16
				descriptor.Save(outpath, Encoding.Unicode);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UnicodeEncoding));
				}
			}
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void ModuleMapDescriptorExtensions_WriteTo()
		{
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor("testmodule", "testheader"))
			{
				Assert.IsNotNull(descriptor);

				// Dealing with encoding will be the problem of the caller when invoking .WriteTo, just
				// dump the descriptor byte data into the memory stream and make sure it was written

				using (MemoryStream ms = new MemoryStream())
				{
					descriptor.WriteTo(ms);
					Assert.AreNotEqual(0, ms.Length);
				}
			}
		}
	}
}
