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
	public class SetupTeardown
	{
		[AssemblyInitialize()]
		public static void AssemblyInit(TestContext context)
		{
			// Create a directory-wide remapping file, a static version does not work
			// since clang checks the date/time stamps on the files
			CreateRemappingFile(new string[] {
				Path.Combine(Environment.CurrentDirectory, @"input\header1.h"),
				Path.Combine(Environment.CurrentDirectory, @"input\header2.h"),
				Path.Combine(Environment.CurrentDirectory, @"input\header3.h") },
				Path.Combine(Environment.CurrentDirectory, @"input\remap"));
		}

		/// <summary>
		/// Creates a clang-compatible remap file and the required temporary files
		/// </summary>
		/// <param name="infiles">Array of input file names</param>
		/// <param name="outfile">Output remap file</param>
		private static void CreateRemappingFile(string[] infiles, string outfile)
		{
			using (StreamWriter sw = SysFile.CreateText(outfile))
			{
				foreach (string infile in infiles)
				{
					FileInfo info = new FileInfo(infile);
					sw.Write(info.FullName + "\n");
					sw.Write(info.LastWriteTime.ToTimeT() + "\n");
					sw.Write(Path.GetTempFileName() + "\n");
				}

				sw.Flush();
			}
		}
	}
}
