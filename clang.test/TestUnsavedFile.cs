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
using Microsoft.VisualStudio.TestTools.UnitTesting;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestUnsavedFile
	{
		[TestMethod, TestCategory("Unsaved Files")]
		public void UnsavedFile_Constructor()
		{
			// UnsavedFile(String^ filename, String^ content);
			UnsavedFile file = new UnsavedFile("main.c", "int main(int argc, char** argv) { return 0; }");
			Assert.IsNotNull(file);
			Assert.IsFalse(String.IsNullOrEmpty(file.FileName));
			Assert.IsFalse(String.IsNullOrEmpty(file.Content));

			// UnsavedFile(String^ filename, TextReader^ contentreader);
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (StreamReader sr = SysFile.OpenText(inpath))
			{
				file = new UnsavedFile("hello.cpp", sr);
				Assert.IsNotNull(file);
				Assert.IsFalse(String.IsNullOrEmpty(file.FileName));
				Assert.IsFalse(String.IsNullOrEmpty(file.Content));
			}
		}

		[TestMethod, TestCategory("Unsaved Files")]
		public void UnsavedFile_Content()
		{
			// UnsavedFile(String^ filename, String^ content);
			UnsavedFile file = new UnsavedFile("main.c", "int main(int argc, char** argv) { return 0; }");
			Assert.IsNotNull(file);
			Assert.AreSame(file.Content, file.Content);
			Assert.IsFalse(String.IsNullOrEmpty(file.Content));
			Assert.AreEqual("int main(int argc, char** argv) { return 0; }", file.Content);

			// UnsavedFile(String^ filename, TextReader^ contentreader);
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (StreamReader sr = SysFile.OpenText(inpath))
			{
				file = new UnsavedFile("hello.cpp", sr);
				Assert.IsNotNull(file);
				Assert.AreSame(file.Content, file.Content);
				Assert.IsFalse(String.IsNullOrEmpty(file.Content));
				Assert.AreEqual(SysFile.ReadAllText(inpath), file.Content);
			}

			file.Content = "int x = 123;";
			Assert.AreEqual("int x = 123;", file.Content);
		}

		[TestMethod, TestCategory("Unsaved Files")]
		public void UnsavedFile_FileName()
		{
			// UnsavedFile(String^ filename, String^ content);
			UnsavedFile file = new UnsavedFile("main.c", "int main(int argc, char** argv) { return 0; }");
			Assert.IsNotNull(file);
			Assert.AreSame(file.FileName, file.FileName);
			Assert.IsFalse(String.IsNullOrEmpty(file.FileName));
			Assert.AreEqual("main.c", file.FileName);

			// UnsavedFile(String^ filename, TextReader^ contentreader);
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (StreamReader sr = SysFile.OpenText(inpath))
			{
				file = new UnsavedFile("hello.cpp", sr);
				Assert.IsNotNull(file);
				Assert.AreSame(file.FileName, file.FileName);
				Assert.IsFalse(String.IsNullOrEmpty(file.FileName));
				Assert.AreEqual("hello.cpp", file.FileName);
			}

			file.FileName = "unsaved.c";
			Assert.AreEqual("unsaved.c", file.FileName);
		}
	}
}
