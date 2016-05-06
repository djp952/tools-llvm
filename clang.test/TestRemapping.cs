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
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestRemapping
	{
		[TestMethod(), TestCategory("Remappings")]
		public void Remapping_Create()
		{
			// Try non-existent files to ensure no exception is thrown
			using (RemappingCollection remappings = Clang.CreateRemappings(new string[] { "not_a_file1", "not_a_file2" }))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(0, remappings.Count);
			}

			// Try non-YAML files to ensure no exception is thrown
			using (RemappingCollection remappings = Clang.CreateRemappings(new string[] { Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp") }))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(0, remappings.Count);
			}

			// There are 2 YAML input files generated from clang using the command lines documented in
			// {clang}\test\ARCMT\designated-init-in-header\designated-init-in-header.m.  These 2 files are
			// fed through the API and the end result should be 2 temporary files that match the .result files

			// remapping[0] -> header1.h.result
			string header1_expected = SysFile.ReadAllText(Path.Combine(Environment.CurrentDirectory, @"input\remappings\header1.h.result"));
			Assert.IsFalse(String.IsNullOrEmpty(header1_expected));

			// remapping[1] -> file2.m.in.result
			string file2_expected = SysFile.ReadAllText(Path.Combine(Environment.CurrentDirectory, @"input\remappings\file2.m.in.result"));
			Assert.IsFalse(String.IsNullOrEmpty(file2_expected));

			string[] inpaths = new string[] { Path.Combine(Environment.CurrentDirectory, @"input\remappings\file1.remap"),
				Path.Combine(Environment.CurrentDirectory, @"input\remappings\file2.remap") };
			using (RemappingCollection remappings = Clang.CreateRemappings(inpaths))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(2, remappings.Count);

				// Exercise the enumerator and indexer while checking reference equivalence
				foreach (Remapping remapping in remappings) Assert.IsNotNull(remapping);
				for (int index = 0; index < remappings.Count; index++) Assert.AreSame(remappings[index], remappings[index]);

				// There is no guarantee of ordering in the collection, so each remapping needs to be found
				Remapping header1 = remappings.Single(r => r.OriginalFileName.Contains("header1"));
				Assert.IsNotNull(header1);
				Assert.IsTrue(SysFile.Exists(header1.TransformedFileName));
				Assert.AreEqual(header1_expected, SysFile.ReadAllText(header1.TransformedFileName));

				Remapping file2 = remappings.Single(r => r.OriginalFileName.Contains("file2"));
				Assert.IsNotNull(file2);
				Assert.IsTrue(SysFile.Exists(file2.TransformedFileName));
				Assert.AreEqual(file2_expected, SysFile.ReadAllText(file2.TransformedFileName));
			}
		}

		[TestMethod(), TestCategory("Remappings")]
		public void Remapping_Load()
		{
			// Try a path without a remap file to ensure no exception is thrown
			using (RemappingCollection remappings = Clang.LoadRemappings(Environment.SystemDirectory))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(0, remappings.Count);
			}

			// Try a path with a bad remap file to ensure no exception is thrown
			using (RemappingCollection remappings = Clang.LoadRemappings(Path.Combine(Environment.CurrentDirectory, @"input\badremappings")))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(0, remappings.Count);
			}

			string inpath = Path.Combine(Environment.CurrentDirectory, @"input");
			using (RemappingCollection remappings = Clang.LoadRemappings(inpath))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(3, remappings.Count);

				// exercise the enumerator and indexer while checking reference equivalence
				foreach (Remapping remapping in remappings) Assert.IsNotNull(remapping);
				for (int index = 0; index < remappings.Count; index++) Assert.AreSame(remappings[index], remappings[index]);

				// exercise generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)remappings) Assert.IsNotNull(obj);

			}
		}

		[TestMethod(), TestCategory("Remappings")]
		public void Remapping_OriginalFileName()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input");
			using (RemappingCollection remappings = Clang.LoadRemappings(inpath))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(3, remappings.Count);

				for (int index = 0; index < remappings.Count; index++)
				{
					Assert.IsNotNull(remappings[index]);
					Assert.AreSame(remappings[index], remappings[index]);
					Assert.IsFalse(String.IsNullOrEmpty(remappings[index].OriginalFileName));
					Assert.AreSame(remappings[index].OriginalFileName, remappings[index].OriginalFileName);
				}

				// The remappings aren't necessarily in the same order they were in the input file, but they should all be there
				string[] filenames = new string[] { Path.Combine(inpath, @"header1.h"), Path.Combine(inpath, @"header2.h"), Path.Combine(inpath, @"header3.h") };
				Assert.AreEqual(3, remappings.Select(r => r.OriginalFileName).Intersect(filenames).Count());
			}
		}

		[TestMethod(), TestCategory("Remappings")]
		public void Remapping_TransformedFileName()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input");
			using (RemappingCollection remappings = Clang.LoadRemappings(inpath))
			{
				Assert.IsNotNull(remappings);
				Assert.AreEqual(3, remappings.Count);

				for (int index = 0; index < remappings.Count; index++)
				{
					Assert.IsNotNull(remappings[index]);
					Assert.AreSame(remappings[index], remappings[index]);
					Assert.IsFalse(String.IsNullOrEmpty(remappings[index].TransformedFileName));
					Assert.AreSame(remappings[index].TransformedFileName, remappings[index].TransformedFileName);
				}
			}
		}
	}
}
