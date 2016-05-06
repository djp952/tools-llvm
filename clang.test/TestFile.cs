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
using zuki.tools.llvm.clang.extensions;

using ClangFile = zuki.tools.llvm.clang.File;
using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestFile
	{
		static TranslationUnit s_hellotu;		// TranslationUnit instance (hello.cpp)
		static ClangFile s_hello;				// hello.cpp file instance
		static TranslationUnit s_includetu;		// TranslationUnit instance (include.cpp)
		static ClangFile s_include;				// include.cpp file instance

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			// Create a translation unit for hello.cpp and pull out the file instance
			string hellocpp = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			s_hellotu = Clang.CreateTranslationUnit(hellocpp, TranslationUnitParseOptions.DetailedPreprocessingRecord);
			Assert.IsNotNull(s_hellotu);

			s_hello = s_hellotu.GetFile(hellocpp);
			Assert.IsNotNull(s_hello);
			Assert.IsFalse(ClangFile.IsNull(s_hello));

			// Create a translation unit for include.cpp and pull out the file instance
			string includecpp = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			s_includetu = Clang.CreateTranslationUnit(includecpp, TranslationUnitParseOptions.DetailedPreprocessingRecord);
			Assert.IsNotNull(s_includetu);

			s_include = s_includetu.GetFile(includecpp);
			Assert.IsNotNull(s_include);
			Assert.IsFalse(ClangFile.IsNull(s_include));
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			if (s_includetu != null) s_includetu.Dispose();
			if (s_hellotu != null) s_hellotu.Dispose();
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Dispose()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			File file = null;

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				file = tu.GetFile(@"input\hello.cpp");
				Assert.IsNotNull(file);
				Assert.IsFalse(ClangFile.IsNull(file));
				Assert.IsFalse(file.IsDisposed(() => file.GetExtent(1, 1)));
			}

			// File instances should be disposed of with the translation unit
			Assert.IsTrue(file.IsDisposed(() => file.GetExtent(1, 1)));
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Equality()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (Index index = Clang.CreateIndex())
			{
				Assert.IsNotNull(index);

				// Create two separate translation units within the index
				TranslationUnit unit1 = index.CreateTranslationUnit(inpath);
				Assert.IsNotNull(unit1);
				TranslationUnit unit2 = index.CreateTranslationUnit(inpath);
				Assert.IsNotNull(unit2);

				// Get the file instance from both translation units
				ClangFile file1 = unit1.GetFile(inpath);
				Assert.AreEqual(inpath, file1.Name);
				ClangFile file2 = unit2.GetFile(inpath);
				Assert.AreEqual(inpath, file2.Name);

				// Check the equality of the files
				Assert.IsTrue(file1 == file2);
				Assert.AreEqual(file1, file2);
				Assert.IsTrue(file1.Equals(file2));
				Assert.IsTrue(file2.Equals(file1));

				// Check the equality of the unique identifiers
				Assert.IsTrue(file1.UniqueIdentifier == file2.UniqueIdentifier);
				Assert.IsFalse(file1.UniqueIdentifier != file2.UniqueIdentifier);
				Assert.AreEqual(file1.UniqueIdentifier, file2.UniqueIdentifier);
				Assert.IsTrue(file1.UniqueIdentifier.Equals(file2.UniqueIdentifier));
				Assert.IsTrue(file2.UniqueIdentifier.Equals(file1.UniqueIdentifier));

				// Verify the hash codes are the same and match the unique identifiers
				Assert.AreEqual(file1.GetHashCode(), file2.GetHashCode());
				Assert.AreEqual(file1.GetHashCode(), file1.UniqueIdentifier.GetHashCode());
				Assert.AreEqual(file2.GetHashCode(), file2.UniqueIdentifier.GetHashCode());

				// Check that the same file is returned using both the full path and short name
				file1 = unit1.GetFile(inpath);
				file2 = unit1.GetFile(@"input\hello.cpp");
				Assert.IsTrue(file1 == file2);

				// Check that the null file does not equal an actual file
				file1 = unit1.GetFile(inpath);
				file2 = unit1.GetFile("not_a_translation_unit_file.c");
				Assert.IsFalse(file1 == file2);
			}
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Inequality()
		{
			// Can use the precreated static File instances here
			Assert.IsTrue(s_hello != s_include);
			Assert.IsTrue(s_include != s_hello);
		}

		[TestMethod(), TestCategory("Files")]
		public void File_EnumerateIncludeDirectives()
		{
			// include.cpp has three #include directives (note that they only appear
			// if the translation unit was created with DetailedPreprocessingRecord)
			List<Tuple<Cursor, Extent>> directives = new List<Tuple<Cursor, Extent>>();
			s_include.EnumerateIncludeDirectives((c, p) =>
			{
				directives.Add(Tuple.Create(c, p));
				return EnumerateIncludeDirectivesResult.Continue;
			});

			Assert.AreEqual(3, directives.Count);

			// [0] -> header1
			Assert.IsNotNull(directives[0].Item1);
			Assert.IsFalse(Cursor.IsNull(directives[0].Item1));
			Assert.IsNotNull(directives[0].Item2);
			Assert.IsFalse(Extent.IsNull(directives[0].Item2));
			Assert.AreEqual("header1.h", directives[0].Item1.Spelling);

			// [1] -> header2
			Assert.IsNotNull(directives[1].Item1);
			Assert.IsFalse(Cursor.IsNull(directives[1].Item1));
			Assert.IsNotNull(directives[1].Item2);
			Assert.IsFalse(Extent.IsNull(directives[1].Item2));
			Assert.AreEqual("header2.h", directives[1].Item1.Spelling);
			
			// [2] -> header1
			Assert.IsNotNull(directives[2].Item1);
			Assert.IsFalse(Cursor.IsNull(directives[2].Item1));
			Assert.IsNotNull(directives[2].Item2);
			Assert.IsFalse(Extent.IsNull(directives[2].Item2));
			Assert.AreEqual("header1.h", directives[2].Item1.Spelling);

			// Make sure that exceptions thrown in the lambda are caught and exposed as intended
			try { s_include.EnumerateIncludeDirectives((c, p) => { throw new Exception("Bad thing"); }); }
			catch (Exception ex) { Assert.AreEqual("Bad thing", ex.Message); }
		}

		[TestMethod(), TestCategory("Files")]
		public void File_GetExtent()
		{
			// Verify that retrieving an extent does not return an Extent.Null
			Assert.IsFalse(Extent.IsNull(s_hello.GetExtent(10, 20)));
			Assert.IsFalse(Extent.IsNull(s_hello.GetExtent(1, 1, 10, 10)));
		}

		[TestMethod(), TestCategory("Files")]
		public void File_GetHashCode()
		{
			Assert.AreNotEqual(0, s_hello.GetHashCode());
			Assert.AreNotEqual(0, s_include.GetHashCode());
		}

		[TestMethod(), TestCategory("Files")]
		public void File_GetLocation()
		{
			// Verify that retrieving a location does not return a Location.Null
			Assert.IsFalse(Location.IsNull(s_hello.GetLocation(10)));
			Assert.IsFalse(Location.IsNull(s_hello.GetLocation(1, 1)));
		}

		[TestMethod(), TestCategory("Files")]
		public void File_IsMultipleIncludeGuarded()
		{
			// header1 is guarded
			ClangFile header1 = s_includetu.GetFile(@"input\header1.h");
			Assert.IsNotNull(header1);
			Assert.IsFalse(ClangFile.IsNull(header1));
			Assert.IsTrue(header1.IsMultipleIncludeGuarded);

			// header2 is guarded
			ClangFile header2 = s_includetu.GetFile(@"input\header2.h");
			Assert.IsNotNull(header2);
			Assert.IsFalse(ClangFile.IsNull(header2));
			Assert.IsTrue(header2.IsMultipleIncludeGuarded);

			// header3 is not guarded
			ClangFile header3 = s_includetu.GetFile(@"input\header3.h");
			Assert.IsNotNull(header3);
			Assert.IsFalse(ClangFile.IsNull(header3));
			Assert.IsFalse(header3.IsMultipleIncludeGuarded);
		}

		[TestMethod(), TestCategory("Files")]
		public void File_IsNull()
		{
			// Check IsNull is true for a nonexistent file in the translation unit
			Assert.IsTrue(ClangFile.IsNull(s_hellotu.GetFile("not_a_translation_unit_file.c")));

			// Check IsNull is false for an actual file in the translation unit,
			// use both the full path and just the file name
			Assert.IsFalse(ClangFile.IsNull(s_hellotu.GetFile(Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp"))));
			Assert.IsFalse(ClangFile.IsNull(s_hellotu.GetFile(@"input\hello.cpp")));
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Module()
		{
			// -fmodules converts #include directives into @import directives.  For this
			// test, clang should follow the module.modulemap and create modules for each header
			string[] args = new string[] { "-fmodules" };

			// include.cpp is the translation unit that has the module map
			using (TranslationUnit tu = Clang.CreateTranslationUnit(Path.Combine(Environment.CurrentDirectory, @"input\include.cpp"), args))
			{
				Assert.IsNotNull(tu);

				// .Module should return the same object reference every time
				File file = tu.GetFile(@"input\header1.h");
				Assert.AreSame(file.Module, file.Module);

				// header1.h -> testlib.header1
				File header1 = tu.GetFile(@"input\header1.h");
				Assert.IsNotNull(header1);
				Assert.IsFalse(File.IsNull(header1));
				Assert.IsNotNull(header1.Module);
				Assert.IsFalse(Module.IsNull(header1.Module));

				// header2.h -> testlib.header2
				File header2 = tu.GetFile(@"input\header2.h");
				Assert.IsNotNull(header2);
				Assert.IsFalse(File.IsNull(header2));
				Assert.IsNotNull(header2.Module);
				Assert.IsFalse(Module.IsNull(header2.Module));

				// header3.h -> testlib.header3
				File header3 = tu.GetFile(@"input\header3.h");
				Assert.IsNotNull(header3);
				Assert.IsFalse(File.IsNull(header3));
				Assert.IsNotNull(header3.Module);
				Assert.IsFalse(Module.IsNull(header3.Module));
			}
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Name()
		{
			// Check the name property, it should return the same reference each time
			// and in this case match the translation unit spelling
			Assert.AreSame(s_hello.Name, s_hello.Name);
			Assert.IsFalse(String.IsNullOrEmpty(s_hello.Name));
			Assert.AreEqual("hello.cpp", Path.GetFileName(s_hello.Name));
		}

		[TestMethod(), TestCategory("Files")]
		public void File_Null()
		{
			// File.Null should return the same reference object every time
			Assert.AreSame(File.Null, File.Null);

			File file = File.Null;
			Assert.IsNotNull(file);
			Assert.IsTrue(File.IsNull(file));

			Assert.IsTrue(Extent.IsNull(file.GetExtent(1, 1)));
			Assert.IsTrue(Extent.IsNull(file.GetExtent(1, 1, 1, 1)));
			Assert.AreEqual(0, file.GetHashCode());
			Assert.IsTrue(Location.IsNull(file.GetLocation(1)));
			Assert.IsTrue(Location.IsNull(file.GetLocation(1, 1)));
			Assert.IsTrue(Module.IsNull(file.Module));
			Assert.AreEqual(0, file.SkippedExtents.Count);
			Assert.AreEqual(false, file.IsMultipleIncludeGuarded);
			Assert.AreEqual(String.Empty, file.Name);
			Assert.AreEqual(DateTime.Parse("12/31/1969 7:00:00PM"), file.TimeStamp);
			Assert.AreEqual("<null>", file.ToString());
			Assert.AreEqual("0:0:0", file.UniqueIdentifier.ToString());
		}

		[TestMethod(), TestCategory("Files")]
		public void File_SkippedExtents()
		{
			// include.cpp should have 2 skipped extents
			Assert.AreSame(s_include.SkippedExtents, s_include.SkippedExtents);
			Assert.AreEqual(2, s_include.SkippedExtents.Count);
			Assert.AreSame(s_include.SkippedExtents[0], s_include.SkippedExtents[0]);
			Assert.IsNotNull(s_include.SkippedExtents[0]);
			Assert.IsFalse(Extent.IsNull(s_include.SkippedExtents[0]));
			Assert.AreSame(s_include.SkippedExtents[1], s_include.SkippedExtents[1]);
			Assert.IsNotNull(s_include.SkippedExtents[1]);
			Assert.IsFalse(Extent.IsNull(s_include.SkippedExtents[1]));

			// hello.cpp shouldn't have any
			Assert.AreSame(s_hello.SkippedExtents, s_hello.SkippedExtents);
			Assert.AreEqual(0, s_hello.SkippedExtents.Count);
		}

		[TestMethod(), TestCategory("Files")]
		public void File_TimeStamp()
		{
			// Check the timestamp against the actual file system file.  Use a filetime rounded
			// off to 10000000 (the granularity of the internal time_t conversion)
			long actualft = (SysFile.GetLastWriteTime(Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp")).ToFileTime() / 10000000) * 10000000;
			Assert.AreEqual(actualft, s_hello.TimeStamp.ToFileTime());
		}

		[TestMethod(), TestCategory("Files")]
		public void File_ToString()
		{
			// Check that .ToString() equals the value of .Name
			Assert.AreEqual(s_hello.Name, s_hello.ToString());
			Assert.AreEqual(s_include.Name, s_include.ToString());
		}

		[TestMethod(), TestCategory("Files")]
		public void File_UniqueIdentifier()
		{
			// Check the unique indentifier to ensure it's not zeroed out
			Assert.AreNotEqual(s_hello.UniqueIdentifier.ToString(), "0:0:0");
		}

		[TestMethod(), TestCategory("Files")]
		public void EnumerateIncludeDirectivesResult_Equality()
		{
			EnumerateIncludeDirectivesResult result = EnumerateIncludeDirectivesResult.Break;
			Assert.IsTrue(result == EnumerateIncludeDirectivesResult.Break);
			Assert.IsTrue(EnumerateIncludeDirectivesResult.Break == result);
			Assert.AreEqual(result, EnumerateIncludeDirectivesResult.Break);
			Assert.AreEqual(EnumerateIncludeDirectivesResult.Break, result);
			Assert.IsTrue(result.Equals(EnumerateIncludeDirectivesResult.Break));
			Assert.IsTrue(EnumerateIncludeDirectivesResult.Break.Equals(result));
		}

		[TestMethod(), TestCategory("Files")]
		public void EnumerateIncludeDirectivesResult_Inequality()
		{
			EnumerateIncludeDirectivesResult result = EnumerateIncludeDirectivesResult.Break;
			Assert.IsTrue(result != EnumerateIncludeDirectivesResult.Continue);
			Assert.IsTrue(EnumerateIncludeDirectivesResult.Continue != result);
			Assert.AreNotEqual(result, EnumerateIncludeDirectivesResult.Continue);
			Assert.AreNotEqual(EnumerateIncludeDirectivesResult.Continue, result);
			Assert.IsFalse(result.Equals(EnumerateIncludeDirectivesResult.Continue));
			Assert.IsFalse(EnumerateIncludeDirectivesResult.Continue.Equals(result));
		}

		[TestMethod(), TestCategory("Files")]
		public void EnumerateIncludeDirectivesResult_GetHashCode()
		{
			Assert.AreNotEqual(0, EnumerateIncludeDirectivesResult.Continue.GetHashCode());

			// Break is zero, so the hash code will also be zero
			Assert.AreEqual(0, EnumerateIncludeDirectivesResult.Break.GetHashCode());
		}

		[TestMethod(), TestCategory("Files")]
		public void EnumerateIncludeDirectivesResult_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateIncludeDirectivesResult.Break.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateIncludeDirectivesResult.Continue.ToString()));
		}

		//
		// EXTENSIONS
		//

		[TestMethod(), TestCategory("Files")]
		public void FileExtensions_EnumerateCursorReferences()
		{
			// hello.cpp returns the argument argc, it should be found as a reference
			Cursor cursor = s_hellotu.FindCursor("main").FindCursor("argc");
			Assert.IsNotNull(cursor);
			Assert.IsFalse(Cursor.IsNull(cursor));

			// Attempt to find at least one reference using the extension method
			Cursor found = Cursor.Null;
			s_hello.EnumerateCursorReferences(cursor, (c, e) => { found = c; return EnumerateReferencesResult.Break; });
			Assert.IsNotNull(found);
			Assert.IsFalse(Cursor.IsNull(found));
		}

		[TestMethod(), TestCategory("Files")]
		public void FileExtensions_FindCursorReferences()
		{
			// hello.cpp returns the argument argc, it should be found as a reference
			Cursor cursor = s_hellotu.FindCursor("main").FindCursor("argc");
			Assert.IsNotNull(cursor);
			Assert.IsFalse(Cursor.IsNull(cursor));

			// Attempt to find the parameter reference using the extension method
			var found = s_hello.FindCursorReferences(cursor, (c, e) => c.Kind == CursorKind.DeclRefExpr);
			Assert.IsNotNull(found);
			Assert.AreEqual(1, found.Count);
		}

		[TestMethod(), TestCategory("Files")]
		public void FileExtensions_FindIncludeDirectives()
		{
			// hello.cpp has #include "stdio.h"
			var found = s_hello.FindIncludeDirectives((c, e) => c.DisplayName == "stdio.h");
			Assert.IsNotNull(found);
			Assert.AreEqual(1, found.Count);
		}

		[TestMethod(), TestCategory("Files")]
		public void FileExtensions_GetCursorReferences()
		{
			// hello.cpp returns the argument argc, it should be found as a reference
			Cursor cursor = s_hellotu.FindCursor("main").FindCursor("argc");
			Assert.IsNotNull(cursor);
			Assert.IsFalse(Cursor.IsNull(cursor));

			// Attempt to find both references using the extension method
			var found = s_hello.GetCursorReferences(cursor);
			Assert.IsNotNull(found);
			Assert.AreNotEqual(0, found.Count);
		}

		[TestMethod(), TestCategory("Files")]
		public void FileExtensions_GetIncludeDirectives()
		{
			// hello.cpp has #include "stdio.h"
			var found = s_hello.GetIncludeDirectives();
			Assert.IsNotNull(found);
			Assert.AreEqual(1, found.Count);
		}
	}
}
