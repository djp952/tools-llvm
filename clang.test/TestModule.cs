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
using zuki.tools.llvm.clang;
using zuki.tools.llvm.clang.extensions;

using ClangFile = zuki.tools.llvm.clang.File;
using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestModule
	{
		static TranslationUnit s_tu;			// TranslationUnit instance
		static Module s_header1;				// testlib.header1
		static Module s_header2;				// testlib.header2 [system]
		static Module s_header3;				// testlib.header3

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			// -fmodules converts #include directives into @import directives.  For this
			// test, clang should follow the module.modulemap and create modules for each header
			string[] args = new string[] { "-fmodules" };
			
			// include.cpp is the translation unit that has the module map
			s_tu = Clang.CreateTranslationUnit(Path.Combine(Environment.CurrentDirectory, @"input\include.cpp"), args);
			Assert.IsNotNull(s_tu);

			// Pull out the three module instances
			s_header1 = s_tu.GetFile(@"input\header1.h").Module;
			Assert.IsNotNull(s_header1);
			Assert.IsFalse(Module.IsNull(s_header1));

			s_header2 = s_tu.GetFile(@"input\header2.h").Module;
			Assert.IsNotNull(s_header2);
			Assert.IsFalse(Module.IsNull(s_header2));

			s_header3 = s_tu.GetFile(@"input\header3.h").Module;
			Assert.IsNotNull(s_header3);
			Assert.IsFalse(Module.IsNull(s_header3));
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			if (s_tu != null) s_tu.Dispose();
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_File()
		{
			// .File property should return the same object reference every time
			Assert.AreSame(s_header1.File, s_header1.File);

			// The files must exist, but won't indicate the original header
			// file name, so just verify they come back as non-null
			Assert.IsNotNull(s_header1.File);
			Assert.IsFalse(File.IsNull(s_header1.File));

			Assert.IsNotNull(s_header2.File);
			Assert.IsFalse(File.IsNull(s_header2.File));

			Assert.IsNotNull(s_header3.File);
			Assert.IsFalse(File.IsNull(s_header3.File));
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_FullName()
		{
			// FullName property should return the same object reference each time
			Assert.AreSame(s_header1.FullName, s_header1.FullName);
			Assert.AreSame(s_header2.FullName, s_header2.FullName);
			Assert.AreSame(s_header3.FullName, s_header3.FullName);

			// Verify the full name of all headers contains the 'testlib' prefix
			Assert.AreEqual(s_header1.FullName, "testlib.header1");
			Assert.AreEqual(s_header2.FullName, "testlib.header2");
			Assert.AreEqual(s_header3.FullName, "testlib.header3");
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_IsNull()
		{
			Module module = Module.Null;
			Assert.IsNotNull(module);
			Assert.IsTrue(Module.IsNull(module));
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_IsSystem()
		{
			// Verify that only header2 is listed as a [system] module
			Assert.AreEqual(false, s_header1.IsSystem);
			Assert.AreEqual(true, s_header2.IsSystem);
			Assert.AreEqual(false, s_header3.IsSystem);
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_Name()
		{
			// Name property should return the same object reference each time
			Assert.AreSame(s_header1.Name, s_header1.Name);
			Assert.AreSame(s_header2.Name, s_header2.Name);
			Assert.AreSame(s_header3.Name, s_header3.Name);

			// Verify the short name of all headers do not contain the 'testlib' prefix
			Assert.AreEqual(s_header1.Name, "header1");
			Assert.AreEqual(s_header2.Name, "header2");
			Assert.AreEqual(s_header3.Name, "header3");
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_Null()
		{
			// Module.Null should return the same reference object every time
			Assert.AreSame(Module.Null, Module.Null);

			Module module = Module.Null;
			Assert.IsNotNull(module);

			// Ensure that all methods and properties return the expected objects/values
			Assert.IsTrue(File.IsNull(module.File));
			Assert.AreEqual(String.Empty, module.FullName);
			Assert.AreEqual(false, module.IsSystem);
			Assert.AreEqual(String.Empty, module.Name);
			Assert.IsTrue(Module.IsNull(module.ParentModule));
			Assert.AreEqual(0, module.TopLevelHeaders.Count);
			Assert.AreEqual("<null>", module.ToString());
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_ParentModule()
		{
			// .ParentModule should return the same object reference every time
			Assert.AreSame(s_header1.ParentModule, s_header1.ParentModule);

			// The expectation here is that all "header" modules have a parent
			// module named "testlib", and "testlib" has a null parent
			Assert.IsNotNull(s_header1.ParentModule);
			Assert.IsFalse(Module.IsNull(s_header1.ParentModule));
			Assert.AreEqual(s_header1.ParentModule.Name, "testlib");
			Assert.IsTrue(Module.IsNull(s_header1.ParentModule.ParentModule));

			Assert.IsNotNull(s_header2.ParentModule);
			Assert.IsFalse(Module.IsNull(s_header2.ParentModule));
			Assert.AreEqual(s_header2.ParentModule.Name, "testlib");
			Assert.IsTrue(Module.IsNull(s_header2.ParentModule.ParentModule));

			Assert.IsNotNull(s_header3.ParentModule);
			Assert.IsFalse(Module.IsNull(s_header3.ParentModule));
			Assert.AreEqual(s_header3.ParentModule.Name, "testlib");
			Assert.IsTrue(Module.IsNull(s_header3.ParentModule.ParentModule));
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_TopLevelHeaders()
		{
			// The top-level headers collection should always be the same reference

			Assert.IsNotNull(s_header1.TopLevelHeaders);
			Assert.AreSame(s_header1.TopLevelHeaders, s_header1.TopLevelHeaders);
			
			Assert.IsNotNull(s_header2.TopLevelHeaders);
			Assert.AreSame(s_header2.TopLevelHeaders, s_header2.TopLevelHeaders);
			
			Assert.IsNotNull(s_header3.TopLevelHeaders);
			Assert.AreSame(s_header3.TopLevelHeaders, s_header3.TopLevelHeaders);

			// Each module should have exactly one top-level header, and in this
			// case the File object returned should be the real header file name

			Assert.AreEqual(1, s_header1.TopLevelHeaders.Count);
			Assert.IsNotNull(s_header1.TopLevelHeaders[0]);
			Assert.IsFalse(File.IsNull(s_header1.TopLevelHeaders[0]));
			Assert.AreSame(s_header1.TopLevelHeaders[0], s_header1.TopLevelHeaders[0]);
			Assert.AreEqual("header1.h", Path.GetFileName(s_header1.TopLevelHeaders[0].Name));
			foreach (ClangFile f in s_header1.TopLevelHeaders) Assert.IsNotNull(f);

			Assert.AreEqual(1, s_header2.TopLevelHeaders.Count);
			Assert.IsNotNull(s_header2.TopLevelHeaders[0]);
			Assert.IsFalse(File.IsNull(s_header2.TopLevelHeaders[0]));
			Assert.AreSame(s_header2.TopLevelHeaders[0], s_header2.TopLevelHeaders[0]);
			Assert.AreEqual("header2.h", Path.GetFileName(s_header2.TopLevelHeaders[0].Name));
			foreach (ClangFile f in s_header2.TopLevelHeaders) Assert.IsNotNull(f);

			Assert.AreEqual(1, s_header3.TopLevelHeaders.Count);
			Assert.IsNotNull(s_header3.TopLevelHeaders[0]);
			Assert.IsFalse(File.IsNull(s_header3.TopLevelHeaders[0]));
			Assert.AreSame(s_header3.TopLevelHeaders[0], s_header3.TopLevelHeaders[0]);
			Assert.AreEqual("header3.h", Path.GetFileName(s_header3.TopLevelHeaders[0].Name));
			foreach (ClangFile f in s_header3.TopLevelHeaders) Assert.IsNotNull(f);

			// Hit the enumerators for code coverage
			foreach (File file in s_header1.TopLevelHeaders) Assert.IsNotNull(file);
			foreach (Object obj in (System.Collections.IEnumerable)s_header1.TopLevelHeaders) Assert.IsNotNull(obj);
		}

		[TestMethod(), TestCategory("Modules")]
		public void Module_ToString()
		{
			// ToString() should return the full name, but is not necessarily going
			// to be the same reference object, so check for equality only
			Assert.AreEqual(s_header1.FullName, s_header1.ToString());
			Assert.AreEqual(s_header2.FullName, s_header2.ToString());
			Assert.AreEqual(s_header3.FullName, s_header3.ToString());
		}
	}
}
