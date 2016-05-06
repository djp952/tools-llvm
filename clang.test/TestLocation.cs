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

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestLocation
	{
		static Index s_index;

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			s_index = Clang.CreateIndex();
			Assert.IsNotNull(s_index);
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			if (s_index != null) s_index.Dispose();
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Equality()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				File file = unit.GetFile(inpath);
				Assert.IsNotNull(file);
				Assert.IsFalse(File.IsNull(file));

				// "main" is at hello.cpp, line 3, column 5
				Location fileloc = file.GetLocation(3, 5);
				Assert.IsNotNull(fileloc);
				Assert.IsFalse(Location.IsNull(fileloc));

				Assert.AreEqual(fileloc, location);
				Assert.AreEqual(location, fileloc);
				Assert.IsTrue(fileloc == location);
				Assert.IsTrue(location == fileloc);
				Assert.IsTrue(fileloc.Equals(location));
				Assert.IsTrue(location.Equals(fileloc));
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Inequality()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location1 = unit.FindCursor("x").Location;
				Assert.IsNotNull(location1);
				Assert.IsFalse(Location.IsNull(location1));

				Location location2 = unit.FindCursor("y").Location;
				Assert.IsNotNull(location2);
				Assert.IsFalse(Location.IsNull(location2));

				Assert.AreNotEqual(location1, location2);
				Assert.AreNotEqual(location2, location1);
				Assert.IsTrue(location1 != location2);
				Assert.IsTrue(location2 != location1);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Column()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("x").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(5, location.Column);

				location = unit.FindCursor("y").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(6, location.Column);

				location = unit.FindCursor("z").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(11, location.Column);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Cursor()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				Cursor cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(location.Cursor, location.Cursor);
				Assert.AreEqual(cursor, location.Cursor);
				Assert.IsTrue(cursor == location.Cursor);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_File()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				File file = unit.GetFile(inpath);
				Assert.IsNotNull(file);
				Assert.IsFalse(File.IsNull(file));

				Assert.AreSame(location.File, location.File);
				Assert.AreEqual(file, location.File);
				Assert.IsTrue(file == location.File);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_GetHashCode()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("x").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreNotEqual(0, location.GetHashCode());

				location = unit.FindCursor("y").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreNotEqual(0, location.GetHashCode());

				location = unit.FindCursor("z").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreNotEqual(0, location.GetHashCode());
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Indexer()
		{
			// Spelling and conversions to other location kinds
			string code = "int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// default location kind is Spelling
				Assert.AreSame(location, location[LocationKind.Spelling]);

				Assert.AreSame(location[LocationKind.Spelling], location[LocationKind.Spelling]);
				Assert.IsNotNull(location[LocationKind.Spelling]);

				Assert.AreSame(location[LocationKind.Expansion], location[LocationKind.Expansion]);
				Assert.IsNotNull(location[LocationKind.Expansion]);

				Assert.AreSame(location[LocationKind.File], location[LocationKind.File]);
				Assert.IsNotNull(location[LocationKind.File]);

				Assert.AreSame(location[LocationKind.Presumed], location[LocationKind.Presumed]);
				Assert.IsNotNull(location[LocationKind.Presumed]);

				// Test the exception case
				try { var l = location[(LocationKind)12345]; Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ArgumentOutOfRangeException)); }
			}

			// Expansion
			code = "#define MYMACRO(_x) int _x;\r\nMYMACRO(myvariable);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("myvariable", true).Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// Expansion location will be t.c, line 2, column 1 (MYMACRO)
				Location expansion = location[LocationKind.Expansion];
				Assert.IsNotNull(expansion);
				Assert.IsFalse(Location.IsNull(expansion));
				Assert.AreEqual("t.c", expansion.File.Name);
				Assert.AreEqual(2, expansion.Line);
				Assert.AreEqual(1, expansion.Column);
			}

			// File
			code = "#define MYMACRO(_x) int _x;\r\nMYMACRO(myvariable);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("myvariable", true).Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// File location will be t.c, line 2, column 9 (myvariable)
				Location file = location[LocationKind.File];
				Assert.IsNotNull(file);
				Assert.IsFalse(Location.IsNull(file));
				Assert.AreEqual("t.c", file.File.Name);
				Assert.AreEqual(2, file.Line);
				Assert.AreEqual(9, file.Column);
			}

			// Presumed
			code = "#123 \"t.c\" 1\r\n\r\nstatic int func(void)\r\n{\r\nreturn 0;\r\n}\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("func").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// Presumed location will be t.c, line 124, column 12
				Location presumed = location[LocationKind.Presumed];
				Assert.IsNotNull(presumed);
				Assert.IsFalse(Location.IsNull(presumed));
				Assert.AreEqual("t.c", presumed.File.Name);
				Assert.AreEqual(124, presumed.Line);
				Assert.AreEqual(12, presumed.Column);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_IsFromMainFile()
		{
			// include.cpp has include files
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// main() is in the main file
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.IsTrue(location.IsFromMainFile);

				// f() is not in the main file
				location = unit.FindCursor("f").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.IsFalse(location.IsFromMainFile);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_IsInSystemHeader()
		{
			// include.cpp has include files, header3 is tagged as a system header
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// f() is in header3, marked with #pragma clang system_header
				Location location = unit.FindCursor("f").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.IsTrue(location.IsInSystemHeader);

				// main() is in the main file
				location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.IsFalse(location.IsInSystemHeader);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_IsNull()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// x's location is not null
				Location location1 = unit.FindCursor("x").Location;
				Assert.IsNotNull(location1);
				Assert.IsFalse(Location.IsNull(location1));

				// q's location is null
				Location location2 = unit.FindCursor("q").Location;
				Assert.IsNotNull(location2);
				Assert.IsTrue(Location.IsNull(location2));
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Kind()
		{
			// Spelling and conversions to other location kinds
			string code = "int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// default location kind is Spelling
				Assert.AreEqual(LocationKind.Spelling, location.Kind);

				// Get all kinds from the Location indexer
				Assert.AreEqual(LocationKind.Spelling, location[LocationKind.Spelling].Kind);
				Assert.AreEqual(LocationKind.Expansion, location[LocationKind.Expansion].Kind);
				Assert.AreEqual(LocationKind.File, location[LocationKind.File].Kind);
				Assert.AreEqual(LocationKind.Presumed, location[LocationKind.Presumed].Kind);
			}

		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Line()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("x").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(1, location.Line);

				location = unit.FindCursor("y").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(2, location.Line);

				location = unit.FindCursor("z").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(3, location.Line);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Null()
		{
			Location location = Location.Null;
			Assert.IsNotNull(location);
			Assert.IsTrue(Location.IsNull(location));

			Assert.AreEqual(0, location.Column);
			Assert.IsTrue(Cursor.IsNull(location.Cursor));
			Assert.IsTrue(File.IsNull(location.File));
			Assert.AreEqual(false, location.IsFromMainFile);
			Assert.AreEqual(false, location.IsInSystemHeader);
			Assert.AreEqual(LocationKind.Spelling, location.Kind);
			Assert.AreEqual(0, location.Line);
			Assert.AreEqual(0, location.Offset);
			Assert.AreEqual("<null> (0, 0)", location.ToString());

			// Check the results from the indexer
			Assert.IsTrue(Location.IsNull(Location.Null[LocationKind.Spelling]));
			Assert.IsTrue(Location.IsNull(Location.Null[LocationKind.Expansion]));
			Assert.IsTrue(Location.IsNull(Location.Null[LocationKind.File]));
			Assert.IsTrue(Location.IsNull(Location.Null[LocationKind.Presumed]));
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_Offset()
		{
			string code = "int x;\r\nlong y;\r\nlong long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Location location = unit.FindCursor("x").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(4, location.Offset);

				location = unit.FindCursor("y").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(13, location.Offset);

				location = unit.FindCursor("z").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(27, location.Offset);
			}
		}

		[TestMethod(), TestCategory("Locations")]
		public void Location_ToString()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Location location = unit.FindCursor("main").Location;
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));

				// "main" is at hello.cpp, line 3, column 5
				// [filename] ([line], [column])
				Assert.AreEqual(inpath + " (3, 5)", location.ToString());
			}
		}
	}
}
