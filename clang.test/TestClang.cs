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
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestClang
	{
		[TestMethod(), TestCategory("Build System")]
		public void Clang_BuildSystemTimestamp()
		{
			// Make sure this returns something other than zero
			Assert.AreNotEqual(0UL, Clang.GetBuildSessionTimestamp());
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void Clang_CreateCompilationDatabase()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");

			// Test failure by specifying a directory highly unlikely to contain compile_commands.json
			try { using (CompilationDatabase cdb = Clang.CreateCompilationDatabase(Environment.SystemDirectory)) { Assert.Fail(); } }
			catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(CompilationDatabaseLoadException)); }

			// Test success by specifying the test files directory
			using (CompilationDatabase cdb = Clang.CreateCompilationDatabase(inpath)) { Assert.IsNotNull(cdb); }
		}

		[TestMethod(), TestCategory("Indexes")]
		public void Clang_CreateIndex()
		{
			// Exercise just the top-level creation methods
			using (Index index = Clang.CreateIndex()) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(true)) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(false)) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(true, true)) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(true, false)) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(false, true)) Assert.IsNotNull(index);
			using (Index index = Clang.CreateIndex(false, false)) Assert.IsNotNull(index);
		}

		[TestMethod(), TestCategory("Module Map Descriptors")]
		public void Clang_CreateModuleMapDescriptor()
		{
			// Exercise just the top-level creation methods
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor()) Assert.IsNotNull(descriptor);
			using (ModuleMapDescriptor descriptor = Clang.CreateModuleMapDescriptor("module", "header")) Assert.IsNotNull(descriptor);
		}

		[TestMethod(), TestCategory("Remappings")]
		public void Clang_CreateRemappings()
		{
			// Exercise just the top-level creation method, this is tested in more detail in TestRemappings.cs
			string[] inpaths = new string[] { Path.Combine(Environment.CurrentDirectory, @"input\remappings\file1.remap") };
			using (RemappingCollection remappings = Clang.CreateRemappings(inpaths)) Assert.IsNotNull(remappings);
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void Clang_CreateTranslationUnit()
		{
			// physical file
			string physical = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(physical));

			// unsaved files
			List<UnsavedFile> unsaved = new List<UnsavedFile>();
			unsaved.Add(new UnsavedFile("unsaved.c", "int main(void) { return 0; }"));

			// arguments
			string[] arguments = new string[] { "-E", "-I" + Environment.CurrentDirectory };

			// Exercise just the top-level creation methods, the resultant object(s) will
			// have their own specific test cases to exercise them
			using (TranslationUnit tu = Clang.CreateTranslationUnit(physical)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit(physical, arguments)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit("unsaved.c", unsaved)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit(physical, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit("unsaved.c", arguments, unsaved)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit(physical, arguments, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit("unsaved.c", unsaved, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
			using (TranslationUnit tu = Clang.CreateTranslationUnit("unsaved.c", arguments, unsaved, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
		}

		[TestMethod(), TestCategory("Virtual File Overlays")]
		public void Clang_CreateVirtualFileOverlay()
		{
			// Exercise just the top-level creation method
			using (VirtualFileOverlay overlay = Clang.CreateVirtualFileOverlay()) Assert.IsNotNull(overlay);
		}

		[TestMethod(), TestCategory("Miscellaneous")]
		public void Clang_GetVersion()
		{
			// This should return the same reference object each time
			Assert.AreSame(Clang.Version, Clang.Version);

			// This pretty much just needs to return something other than blank
			Assert.IsFalse(String.IsNullOrEmpty(Clang.Version));
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Clang_LoadDiagnostics()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\diagnostics.bin");
			Assert.IsTrue(SysFile.Exists(inpath));

			// Create a DiagnosticsCollection from a serialized file, there should be 6 diagnostics
			using (LoadedDiagnosticCollection diags = Clang.LoadDiagnostics(inpath))
			{
				Assert.IsNotNull(diags);
				Assert.AreEqual(6, diags.Count);
			}
		}

		[TestMethod(), TestCategory("Remappings")]
		public void Clang_LoadRemappings()
		{
			// Exercise just the top-level creation method, the input file should have been created
			// by the [AssemblyInitialize()] attribute in TestRemappings.cs
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input");
			using (RemappingCollection remappings = Clang.LoadRemappings(inpath)) Assert.IsNotNull(remappings);
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void Clang_LoadTranslationUnit()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			// Attempt to load a non-existent file
			try { Clang.LoadTranslationUnit(@"this_ast_file_does_not_exist"); Assert.Fail(); }
			catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ClangException)); }

			// Attempt to load an existing file that isn't a serialized translation unit
			try { Clang.LoadTranslationUnit(inpath); Assert.Fail(); }
			catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ClangException)); }

			// Get a temporary file name
			string outpath = Path.GetTempFileName();
			try
			{
				// Create a trivial translation unit form source and save it
				using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath)) unit.Save(outpath);

				// Attempt to load the translation unit from the serialized file
				using (TranslationUnit unit = Clang.LoadTranslationUnit(outpath)) Assert.IsNotNull(unit);
			}

			finally { SysFile.Delete(outpath); }
		}

		[TestMethod(), TestCategory("Miscellaneous")]
		public void Clang_SetCrashRecovery()
		{
			// There is no means to get this flag, so just turn it on and off
			Clang.SetCrashRecovery(true);
			Clang.SetCrashRecovery(false);
		}
	}
}
