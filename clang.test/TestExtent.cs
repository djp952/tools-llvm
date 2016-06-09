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
	public class TestExtent
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

		/// <summary>
		/// Verifies workaround for clang bug 9069: https://llvm.org/bugs/show_bug.cgi?id=9069
		/// Also see duplicate bug 17497: https://llvm.org/bugs/show_bug.cgi?id=17497
		/// </summary>
		[TestMethod(), TestCategory("Extents")]
		public void Extent_Workaround_9069()
		{
			// Bug 9069 involves a disconnect with the length of an extent as reported by a
			// cursor and the length of an extent provided as input to be tokenized.  As a 
			// result, clang may generate one too many tokens.  The workaround being tested
			// verifies the location of the final token in a GetTokens() operation to ensure
			// that it falls within the expected extent, otherwise it will be removed
			string code = "void abc() {  int a; if (a) {a++;}  }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Grab all the compound statement cursors in the translation unit
				var compounds = unit.Cursor.FindChildren((c, p) => c.Kind == CursorKind.CompoundStmt, true);

				// tokenize: {a++;}
				TokenCollection tokens = compounds[1].Item1.Extent.GetTokens();
					
				// There should be 5 tokens when this extent is tokenized, not 6
				Assert.AreEqual(5, tokens.Count);
				Assert.AreEqual("{", tokens[0].Spelling);
				Assert.AreEqual("a", tokens[1].Spelling);
				Assert.AreEqual("++", tokens[2].Spelling);
				Assert.AreEqual(";", tokens[3].Spelling);
				Assert.AreEqual("}", tokens[4].Spelling);
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_Equality()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// This is actually curiously difficult to test, there are only so many ways
				// to access an extent, and they are not considered equal if they are sourced
				// from different translation units, so just grab 2 unique cursor instances
				Extent extent1 = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent1);
				Assert.IsFalse(Extent.IsNull(extent1));
				
				Extent extent2 = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent2);
				Assert.IsFalse(Extent.IsNull(extent2));

				Assert.AreEqual(extent1, extent2);
				Assert.AreEqual(extent2, extent1);
				Assert.IsTrue(extent1 == extent2);
				Assert.IsTrue(extent2 == extent1);
				Assert.IsTrue(extent1.Equals(extent2));
				Assert.IsTrue(extent2.Equals(extent1));
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_Inequality()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Extent extentx = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extentx);
				Assert.IsFalse(Extent.IsNull(extentx));

				Extent extenty = unit.FindCursor("y").Extent;
				Assert.IsNotNull(extenty);
				Assert.IsFalse(Extent.IsNull(extenty));

				Extent extentz = unit.FindCursor("z").Extent;
				Assert.IsNotNull(extentz);
				Assert.IsFalse(Extent.IsNull(extentz));

				Assert.IsTrue(extentx != extenty);
				Assert.IsTrue(extenty != extentz);
				Assert.IsTrue(extentx != extentz);
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_End()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Extent extent = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.End, extent.End);
				Assert.IsNotNull(extent.End);
				Assert.IsFalse(Location.IsNull(extent.End));
				Assert.AreEqual(1, extent.End.Line);
				Assert.AreEqual(11, extent.End.Column);

				extent = unit.FindCursor("y").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.End, extent.End);
				Assert.IsNotNull(extent.End);
				Assert.IsFalse(Location.IsNull(extent.End));
				Assert.AreEqual(2, extent.End.Line);
				Assert.AreEqual(11, extent.End.Column);

				extent = unit.FindCursor("z").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.End, extent.End);
				Assert.IsNotNull(extent.End);
				Assert.IsFalse(Location.IsNull(extent.End));
				Assert.AreEqual(3, extent.End.Line);
				Assert.AreEqual(11, extent.End.Column);
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_GetHashCode()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Extent extentx = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extentx);
				Assert.IsFalse(Extent.IsNull(extentx));
				Assert.AreNotEqual(0, extentx.GetHashCode());

				Extent extenty = unit.FindCursor("y").Extent;
				Assert.IsNotNull(extenty);
				Assert.IsFalse(Extent.IsNull(extenty));
				Assert.AreNotEqual(0, extenty.GetHashCode());

				Extent extentz = unit.FindCursor("z").Extent;
				Assert.IsNotNull(extentz);
				Assert.IsFalse(Extent.IsNull(extentz));
				Assert.AreNotEqual(0, extentz.GetHashCode());

				Assert.AreNotEqual(extentx.GetHashCode(), extenty.GetHashCode());
				Assert.AreNotEqual(extenty.GetHashCode(), extentz.GetHashCode());
				Assert.AreNotEqual(extentx.GetHashCode(), extentz.GetHashCode());
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_GetTokens()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Verify entity x
				Cursor x = unit.FindCursor("x");
				Assert.AreEqual(1, x.Extent.Start.Line);
				Assert.AreEqual(1, x.Extent.Start.Column);
				Assert.AreEqual(1, x.Extent.End.Line);
				Assert.AreEqual(11, x.Extent.End.Column);

				// Verify entity x tokens
				TokenCollection xtokens = x.Extent.GetTokens();
				Assert.AreEqual(4, xtokens.Count);
				for (int index = 0; index < xtokens.Count; index++) Assert.AreSame(xtokens[index], xtokens[index]);
				Assert.AreEqual("int", xtokens[0].Spelling);
				Assert.AreEqual("x", xtokens[1].Spelling);
				Assert.AreEqual("=", xtokens[2].Spelling);
				Assert.AreEqual("10", xtokens[3].Spelling);
				foreach (Token t in xtokens) Assert.IsNotNull(t);
				foreach (Object obj in (System.Collections.IEnumerable)xtokens) Assert.IsNotNull(obj);

				// Verify entity y
				Cursor y = unit.FindCursor("y");
				Assert.AreEqual(2, y.Extent.Start.Line);
				Assert.AreEqual(1, y.Extent.Start.Column);
				Assert.AreEqual(2, y.Extent.End.Line);
				Assert.AreEqual(11, y.Extent.End.Column);

				// Verify entity y tokens
				TokenCollection ytokens = y.Extent.GetTokens();
				Assert.AreEqual(4, ytokens.Count);
				for (int index = 0; index < ytokens.Count; index++) Assert.AreSame(ytokens[index], ytokens[index]);
				Assert.AreEqual("int", ytokens[0].Spelling);
				Assert.AreEqual("y", ytokens[1].Spelling);
				Assert.AreEqual("=", ytokens[2].Spelling);
				Assert.AreEqual("11", ytokens[3].Spelling);
				foreach (Token t in ytokens) Assert.IsNotNull(t);
				foreach (Object obj in (System.Collections.IEnumerable)ytokens) Assert.IsNotNull(obj);

				// Verify entity z
				Cursor z = unit.FindCursor("z");
				Assert.AreEqual(3, z.Extent.Start.Line);
				Assert.AreEqual(1, z.Extent.Start.Column);
				Assert.AreEqual(3, z.Extent.End.Line);
				Assert.AreEqual(11, z.Extent.End.Column);

				// Verify entity z tokens
				TokenCollection ztokens = z.Extent.GetTokens();
				Assert.AreEqual(4, ztokens.Count);
				for (int index = 0; index < ztokens.Count; index++) Assert.AreSame(ztokens[index], ztokens[index]);
				Assert.AreEqual("int", ztokens[0].Spelling);
				Assert.AreEqual("z", ztokens[1].Spelling);
				Assert.AreEqual("=", ztokens[2].Spelling);
				Assert.AreEqual("12", ztokens[3].Spelling);
				foreach (Token t in ztokens) Assert.IsNotNull(t);
				foreach (Object obj in (System.Collections.IEnumerable)ztokens) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_IsNull()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// x is a real extent
				Extent extent = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));

				// abcd is not
				extent = unit.FindCursor("abcd").Extent;
				Assert.IsNotNull(extent);
				Assert.IsTrue(Extent.IsNull(extent));
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_Null()
		{
			Extent extent = Extent.Null;
			Assert.IsNotNull(extent);
			Assert.IsTrue(Extent.IsNull(extent));

			// Ensure that all methods and properties return the expected objects/values
			Assert.IsTrue(Location.IsNull(extent.End));
			Assert.AreEqual(0, extent.GetHashCode());
			Assert.AreEqual(0, extent.GetTokens().Count);
			Assert.IsTrue(Location.IsNull(extent.Start));
			Assert.AreEqual("<null> (0, 0) - <null> (0, 0)", extent.ToString());
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_Start()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Extent extent = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.Start, extent.Start);
				Assert.IsNotNull(extent.Start);
				Assert.IsFalse(Location.IsNull(extent.Start));
				Assert.AreEqual(1, extent.Start.Line);
				Assert.AreEqual(1, extent.Start.Column);

				extent = unit.FindCursor("y").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.Start, extent.Start);
				Assert.IsNotNull(extent.Start);
				Assert.IsFalse(Location.IsNull(extent.Start));
				Assert.AreEqual(2, extent.Start.Line);
				Assert.AreEqual(1, extent.Start.Column);

				extent = unit.FindCursor("z").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreSame(extent.Start, extent.Start);
				Assert.IsNotNull(extent.Start);
				Assert.IsFalse(Location.IsNull(extent.Start));
				Assert.AreEqual(3, extent.Start.Line);
				Assert.AreEqual(1, extent.Start.Column);
			}
		}

		[TestMethod(), TestCategory("Extents")]
		public void Extent_ToString()
		{
			string code = "int x = 10;\r\nint y = 11;\r\nint z = 12;\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// The expected format for .ToString() is Start.ToString() + " - " + End.ToString()
				// the location .ToString() method is tested as part of Location, not here, just
				// make sure that it's coming up with the expected combination of string parts
				Extent extent = unit.FindCursor("x").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreEqual(extent.Start.ToString() + " - " + extent.End.ToString(), extent.ToString());

				extent = unit.FindCursor("y").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreEqual(extent.Start.ToString() + " - " + extent.End.ToString(), extent.ToString());

				extent = unit.FindCursor("z").Extent;
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreEqual(extent.Start.ToString() + " - " + extent.End.ToString(), extent.ToString());
			}
		}
	}
}
