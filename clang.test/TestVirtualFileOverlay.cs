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
	public class TestVirtualFileOverlay
	{
		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_Dispose()
		{
			VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay();
			Assert.IsFalse(overlay.IsDisposed(() => overlay.CaseSensitive = true));

			// Not much to this one, it's just a simple reference object.  Make
			// sure that once disposed it throws ObjectDisposedException
			overlay.Dispose();
			Assert.IsTrue(overlay.IsDisposed(() => overlay.CaseSensitive = false));
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_AddFileMapping()
		{
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);

				// Throw a couple fully qualified paths at the overlay class and check it doesn't throw
				overlay.AddFileMapping(Path.Combine(Environment.CurrentDirectory, "virtual"), Path.Combine(Environment.CurrentDirectory, "physical"));

				// Throw a couple relative paths at the overlay class and check it doesn't throw
				overlay.AddFileMapping("virtual", "physical");
			}
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_CaseSensitive()
		{
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);

				// The default value should be true
				Assert.IsTrue(overlay.CaseSensitive);

				// Change the case sensitivity to false and verify it stuck
				overlay.CaseSensitive = false;
				Assert.IsFalse(overlay.CaseSensitive);

				// Change the case sensitivity back to true and verify it stuck
				overlay.CaseSensitive = true;
				Assert.IsTrue(overlay.CaseSensitive);
			}
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_ToByteArray()
		{
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);
				overlay.AddFileMapping("virtual_file", "physical_file");
				Assert.AreNotEqual(0, overlay.ToByteArray().Length);
			}
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_ToString()
		{
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);
				overlay.AddFileMapping("virtual_file", "physical_file");
				Assert.IsFalse(String.IsNullOrEmpty(overlay.ToString()));
			}
		}

		//
		// EXTENSIONS
		//

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_Save()
		{
			// Temporary file name in which to write the module map descriptor
			string outpath = Path.GetTempFileName();

			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);

				// The following checks verify the encoding of the output file by passing ASCII as the default
				// encoding to StreamReader, but allowing it to be overridden by the byte order marks.  If ASCII
				// is not provided as the default encoding, .CurrentEncoding will return UTF8 for both ASCII as UTF8

				// Save the file as default (UTF-8)
				overlay.Save(outpath);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UTF8Encoding));
				}

				// Save the file as ASCII
				overlay.Save(outpath, Encoding.ASCII);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(ASCIIEncoding));
				}

				// Save the file as UTF-8
				overlay.Save(outpath, Encoding.UTF8);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UTF8Encoding));
				}

				// Save the file as UTF-16
				overlay.Save(outpath, Encoding.Unicode);
				using (StreamReader reader = new StreamReader(SysFile.OpenRead(outpath), Encoding.ASCII, true))
				{
					reader.Peek();
					Assert.IsInstanceOfType(reader.CurrentEncoding, typeof(UnicodeEncoding));
				}
			}
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void VirtualFileOverlay_WriteTo()
		{
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay())
			{
				Assert.IsNotNull(overlay);

				// Dealing with encoding will be the problem of the caller when invoking .WriteTo, just
				// dump the descriptor byte data into the memory stream and make sure it was written

				using (MemoryStream ms = new MemoryStream())
				{
					overlay.WriteTo(ms);
					Assert.AreNotEqual(0, ms.Length);
				}
			}
		}
	}
}
