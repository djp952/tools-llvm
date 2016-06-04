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

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestCursor
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

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Equality()
		{
			string code = "int x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor1 = unit.FindCursor("x");
				var x = cursor1.CompletionString.Annotations;
				Assert.IsNotNull(cursor1);
				Assert.IsFalse(Cursor.IsNull(cursor1));

				Cursor cursor2 = unit.FindCursor("x");
				Assert.IsNotNull(cursor2);
				Assert.IsFalse(Cursor.IsNull(cursor2));

				Assert.AreEqual(cursor1, cursor2);
				Assert.AreEqual(cursor2, cursor1);
				Assert.IsTrue(cursor1 == cursor2);
				Assert.IsTrue(cursor2 == cursor1);
				Assert.IsTrue(cursor1.Equals(cursor2));
				Assert.IsTrue(cursor2.Equals(cursor1));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Inequality()
		{
			string code = "int x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor1 = unit.FindCursor("x");
				Assert.IsNotNull(cursor1);
				Assert.IsFalse(Cursor.IsNull(cursor1));

				Cursor cursor2 = unit.FindCursor("y");
				Assert.IsNotNull(cursor2);
				Assert.IsFalse(Cursor.IsNull(cursor2));

				Assert.AreNotEqual(cursor1, cursor2);
				Assert.AreNotEqual(cursor2, cursor1);
				Assert.IsTrue(cursor1 != cursor2);
				Assert.IsTrue(cursor2 != cursor1);
				Assert.IsFalse(cursor1.Equals(cursor2));
				Assert.IsFalse(cursor2.Equals(cursor1));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Availability()
		{
			string code = "int oldfunc(void) __attribute__((deprecated)); int newfunc(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("oldfunc");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Availability.Deprecated, cursor.Availability);

				cursor = unit.FindCursor("newfunc");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Availability.Available, cursor.Availability);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ArgumentCursors()
		{
			string code = "int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(cursor.ArgumentCursors, cursor.ArgumentCursors);
				Assert.AreEqual(2, cursor.ArgumentCursors.Count);
				Assert.AreSame(cursor.ArgumentCursors[0], cursor.ArgumentCursors[0]);
				Assert.AreSame(cursor.ArgumentCursors[1], cursor.ArgumentCursors[1]);

				// 0 --> argc
				Assert.AreEqual("argc", cursor.ArgumentCursors[0].Spelling);

				// 1 --> argv
				Assert.AreEqual("argv", cursor.ArgumentCursors[1].Spelling);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_BriefComment()
		{
			string code = "/**\r\n* \\brief This is a brief comment for class c.\r\n*/\r\nclass c {};";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// The reference object returned should be the same each time
				Assert.AreSame(cursor.BriefComment, cursor.BriefComment);
				Assert.AreEqual("This is a brief comment for class c.", cursor.BriefComment);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_CanonicalCursor()
		{
			string code = "typedef struct s; typedef struct s { int x; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Get both cursors that refer to s, one is a forward declaration
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "s");
				Assert.AreEqual(2, cursors.Count);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));
				Assert.IsFalse(Cursor.IsNull(cursors[1].Item1));

				// .CanonicalCursor should return the same object reference every time
				Assert.AreSame(cursors[0].Item1.CanonicalCursor, cursors[0].Item1.CanonicalCursor);

				// The cursors themselves should not be equal
				Assert.IsFalse(cursors[0].Item1 == cursors[1].Item1);

				// The canonical cursors should be equal
				Assert.IsTrue(cursors[0].Item1.CanonicalCursor == cursors[1].Item1.CanonicalCursor);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Comment()
		{
			string code = "/**\r\n* \\brief This is a brief comment for class c.\r\n*/\r\nclass c {};";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// The reference objects returned should be the same each time
				Assert.AreSame(cursor.Comment, cursor.Comment);
				Assert.AreSame(cursor.Comment.Text, cursor.Comment.Text);
				Assert.AreSame(cursor.Comment.Extent, cursor.Comment.Extent);

				// Not particularly interested in the values, just that they exist
				Assert.IsNotNull(cursor.Comment);
				Assert.IsFalse(String.IsNullOrEmpty(cursor.Comment.Text));
				Assert.IsNotNull(cursor.Comment.Extent);
				Assert.IsFalse(Extent.IsNull(cursor.Comment.Extent));

				// Code coverage: implicit String^ and ToString()
				Assert.AreEqual("/**\r\n* \\brief This is a brief comment for class c.\r\n*/", cursor.Comment);
				Assert.AreEqual("/**\r\n* \\brief This is a brief comment for class c.\r\n*/", cursor.Comment.ToString());
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_CompletionString()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// Completion string is unit tested as part of code completion, just check the
				// property coming from the Cursor object

				// 13, 9 == struct Z
				Cursor c = tu.GetLocation(inpath, 13, 9).Cursor;
				Assert.IsNotNull(c);
				Assert.IsFalse(Cursor.IsNull(c));
				Assert.IsNotNull(c.CompletionString);
				Assert.AreSame(c.CompletionString, c.CompletionString);
				Assert.AreNotEqual(0, c.CompletionString.Chunks);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_CxxAccessSpecifier()
		{
			string code = "class c { public: ~c() {} protected: c() {} private: int m_x; }; int main(void) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// Public: c::~c
				Cursor cursor = unit.FindCursor("c").FindCursor("~c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CxxAccessSpecifier.Public, cursor.CxxAccessSpecifier);

				// Protected: c::c
				cursor = unit.FindCursor("c").FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CxxAccessSpecifier.Protected, cursor.CxxAccessSpecifier);

				// Private: c::m_x
				cursor = unit.FindCursor("c").FindCursor("m_x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CxxAccessSpecifier.Private, cursor.CxxAccessSpecifier);

				// Invalid: main
				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CxxAccessSpecifier.Invalid, cursor.CxxAccessSpecifier);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_CxxMangledSymbols()
		{
			string code = "class c { public: ~c() {} protected: c() {} private: int m_x; }; int main(void) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c").FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.CxxMangledSymbols, cursor.CxxMangledSymbols);
				Assert.AreNotEqual(0, cursor.CxxMangledSymbols.Count);
				Assert.AreSame(cursor.CxxMangledSymbols[0], cursor.CxxMangledSymbols[0]);

				// Exercise the enumerator
				foreach (string s in cursor.CxxMangledSymbols) Assert.IsFalse(String.IsNullOrEmpty(s));
				foreach (Object obj in (System.Collections.IEnumerable)cursor.CxxMangledSymbols) Assert.IsNotNull(obj);

				cursor = unit.FindCursor("c").FindCursor("~c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreNotEqual(0, cursor.CxxMangledSymbols.Count);
				Assert.AreSame(cursor.CxxMangledSymbols[0], cursor.CxxMangledSymbols[0]);

				cursor = unit.FindCursor("c").FindCursor("m_x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(0, cursor.CxxMangledSymbols.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_DefinitionCursor()
		{
			string code = "typedef struct s; typedef struct s { int x; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Get both cursors that refer to s, one is a forward declaration
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "s");
				Assert.AreEqual(2, cursors.Count);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));
				Assert.IsFalse(Cursor.IsNull(cursors[1].Item1));

				Assert.AreSame(cursors[0].Item1.DefinitionCursor, cursors[0].Item1.DefinitionCursor);

				// The cursors themselves should not be equal
				Assert.IsFalse(cursors[0].Item1 == cursors[1].Item1);

				// The first cursor is a forward declaration, so should not be the definition
				Assert.IsFalse(cursors[0].Item1 == cursors[0].Item1.DefinitionCursor);

				// The second cursor is the definition cursor
				Assert.IsTrue(cursors[1].Item1 == cursors[0].Item1.DefinitionCursor);
				Assert.IsTrue(cursors[1].Item1 == cursors[1].Item1.DefinitionCursor);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_DisplayName()
		{
			string code = "class TestClass { void InstanceMethod(void) {} }; int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// TestClass --> TestClass
				Cursor cursor = unit.FindCursor("TestClass");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.DisplayName, cursor.DisplayName);
				Assert.AreEqual("TestClass", cursor.DisplayName);

				// TestClass::InstanceMethod --> InstanceMethod()
				cursor = unit.FindCursor("TestClass").FindCursor("InstanceMethod");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual("InstanceMethod()", cursor.DisplayName);

				// main --> main(int, char **)
				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual("main(int, char **)", cursor.DisplayName);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_EnumConstant()
		{
			string code = "enum TestIntEnum { IntTwo = 2, IntNegativeTwo = -2, };\r\n" +
				"enum TestUIntEnum : unsigned int { UIntTwo = 2, UIntNegativeTwo = -2, };\r\n" +
				"enum TestLongLongEnum : long long { LongLongTwo = 2, LongLongNegativeTwo = -2, LongLongReallyBig = 0x800000000, };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// TestIntEnum
				Cursor cursor = unit.FindCursor("TestIntEnum");
				Assert.IsNotNull(cursor);
				Cursor constant = cursor.FindCursor("IntTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(2, (int)constant.EnumConstant);
				Assert.AreEqual((ulong)2, constant.EnumConstant.UnsignedValue);
				Assert.AreEqual(2, constant.EnumConstant.Value);
				Assert.AreEqual("2", constant.EnumConstant.ToString());
				EnumConstant comp1 = constant.EnumConstant;
				constant = cursor.FindCursor("IntNegativeTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(-2, (int)constant.EnumConstant);
				Assert.AreEqual(unchecked((uint)-2), constant.EnumConstant.UnsignedValue);
				Assert.AreEqual(-2, constant.EnumConstant.Value);
				Assert.AreEqual("-2", constant.EnumConstant.ToString());

				// TestUIntEnum
				cursor = unit.FindCursor("TestUIntEnum");
				Assert.IsNotNull(cursor);
				constant = cursor.FindCursor("UIntTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(2U, (uint)constant.EnumConstant);
				Assert.AreEqual("2", constant.EnumConstant.ToString());
				constant = cursor.FindCursor("UIntNegativeTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(unchecked((uint)-2), (uint)constant.EnumConstant);
				Assert.AreEqual("4294967294", constant.EnumConstant.ToString());
				EnumConstant comp2 = constant.EnumConstant;

				// Code coverage
				//Assert.IsTrue(comp1 == comp1);			// comp1 = TestIntEnum::IntTwo
				//Assert.IsTrue(comp2 == comp2);			// comp2 = TestUIntEnum::UIntNegativeTwo
				Assert.IsTrue(comp1 != comp2);
				Assert.IsTrue(comp2 != comp1);
				Assert.IsTrue(comp1.Equals(comp1));
				Assert.IsTrue(comp2.Equals(comp2));
				Assert.IsFalse(comp1.Equals(comp2));
				Assert.IsFalse(comp2.Equals(comp1));
				Assert.AreEqual(comp1, comp1);
				Assert.AreEqual(comp2, comp2);
				Assert.AreNotEqual(comp1, comp2);
				Assert.AreNotEqual(comp2, comp1);
				Assert.AreNotEqual(0, comp1.GetHashCode());
				Assert.AreNotEqual(0, comp2.GetHashCode());
				Assert.AreNotEqual(comp1.GetHashCode(), comp2.GetHashCode());

				// TestLongLongEnum
				cursor = unit.FindCursor("TestLongLongEnum");
				Assert.IsNotNull(cursor);
				constant = cursor.FindCursor("LongLongTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(2L, constant.EnumConstant);
				constant = cursor.FindCursor("LongLongNegativeTwo");
				Assert.IsNotNull(constant);
				Assert.AreEqual(-2L, constant.EnumConstant);

				// TestLongLongEnum also has a 64-bit value, test value and overflow exceptions
				constant = cursor.FindCursor("LongLongReallyBig");
				Assert.IsNotNull(constant);
				Assert.AreEqual(0x800000000UL, constant.EnumConstant);

				try { int intval = (int)constant.EnumConstant; Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(OverflowException)); }

				try { uint uintval = (uint)constant.EnumConstant; Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(OverflowException)); }
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_EnumerateChildren()
		{
			string code = "class c { public: ~c() {} protected: c() {} private: int m_x; }; int main(void) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// Don't go too crazy with this, just make sure it's working, it's used
				// everywhere by the FindCursor extension method so it's well tested
				List<Cursor> cursors = new List<Cursor>();
				unit.Cursor.EnumerateChildren((c, p) => { cursors.Add(c); return EnumerateChildrenResult.Recurse; });
				Assert.AreNotEqual(0, cursors.Count);

				// Make sure that exceptions thrown in the lambda are caught and exposed as intended
				try { unit.Cursor.EnumerateChildren((c, p) => { throw new Exception("Bad thing"); }); }
				catch (Exception ex) { Assert.AreEqual("Bad thing", ex.Message); }
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_EnumerateReferences()
		{
			string code = "@class Foo; @interface Foo -(id)setWithInt:(int)i andFloat:(float)f; @end @implementation Foo -(id)setWithInt:(int)i andFloat:(float)f { return self; } @end\r\n" +
				"void test(Foo *foo) { [foo setWithInt:0 andFloat:0]; [foo setWithInt: 2 andFloat: 3]; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// Find @class Foo
				Cursor cursor = unit.FindCursor("Foo");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.ObjectiveCClassRef, cursor.Kind);

				// Use EnumerateFileReferences to create a List<> of cursors
				List<Cursor> cursors = new List<Cursor>();
				cursor.EnumerateReferences(unit.GetFile(unit.Spelling), (c, e) =>
				{
					cursors.Add(c);
					return EnumerateReferencesResult.Continue;
				});

				// There are 5 references to "Foo" in the unsaved file, and that's good enough

				// Make sure that exceptions thrown in the lambda are caught and exposed as intended
				try { cursor.EnumerateReferences(unit.GetFile(unit.Spelling), (c, e) => { throw new Exception("Bad thing"); }); }
				catch (Exception ex) { Assert.AreEqual("Bad thing", ex.Message); }
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_EnumType()
		{
			string code = "enum TestIntEnum { IntTwo = 2, IntNegativeTwo = -2, };\r\n" +
				"enum TestUIntEnum : unsigned int { UIntTwo = 2, UIntNegativeTwo = -2, };\r\n" +
				"enum TestLongLongEnum : long long { LongLongTwo = 2, LongLongNegativeTwo = -2, LongLongReallyBig = 0x800000000, };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// TestIntEnum
				Cursor cursor = unit.FindCursor("TestIntEnum");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.EnumDecl, cursor.Kind);
				Assert.AreSame(cursor.EnumType, cursor.EnumType);
				Assert.IsNotNull(cursor.EnumType);
				Assert.AreNotEqual(TypeKind.Invalid, cursor.EnumType.Kind);
				Assert.AreEqual(TypeKind.Int, cursor.EnumType.Kind);

				// TestUIntEnum
				cursor = unit.FindCursor("TestUIntEnum");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.EnumDecl, cursor.Kind);
				Assert.AreSame(cursor.EnumType, cursor.EnumType);
				Assert.IsNotNull(cursor.EnumType);
				Assert.AreNotEqual(TypeKind.Invalid, cursor.EnumType.Kind);
				Assert.AreEqual(TypeKind.UInt, cursor.EnumType.Kind);

				// TestLongLongEnum
				cursor = unit.FindCursor("TestLongLongEnum");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.EnumDecl, cursor.Kind);
				Assert.AreSame(cursor.EnumType, cursor.EnumType);
				Assert.IsNotNull(cursor.EnumType);
				Assert.AreNotEqual(TypeKind.Invalid, cursor.EnumType.Kind);
				Assert.AreEqual(TypeKind.LongLong, cursor.EnumType.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Extent()
		{
			string code = "class c { public: ~c() {} protected: c() {} private: int m_x; }; int main(void) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Extent, cursor.Extent);

				// Not really concerned about the values themselves, Extent has its own
				// set of unit tests, just ensure it's set to something
				Assert.IsNotNull(cursor.Extent);
				Assert.IsFalse(Extent.IsNull(cursor.Extent));
				Assert.IsFalse(Location.IsNull(cursor.Extent.Start));
				Assert.IsFalse(Location.IsNull(cursor.Extent.End));
				Assert.AreNotSame(0, cursor.Extent.Start.Offset);
				Assert.AreNotSame(0, cursor.Extent.End.Offset);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_FieldBitWidth()
		{
			string code = "typedef struct s { unsigned char bits1 : 2; unsigned char bits2 : 6; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor parent = unit.FindCursor("s");
				Assert.IsNotNull(parent);
				Assert.IsFalse(Cursor.IsNull(parent));
				Assert.IsTrue(parent.IsDefinition);

				// s::bits1
				Cursor cursor = parent.FindCursor("bits1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(2, cursor.FieldBitWidth);

				// s::bits2
				cursor = parent.FindCursor("bits2");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(6, cursor.FieldBitWidth);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_GetHashCode()
		{
			string code = "int x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor1 = unit.FindCursor("x");
				Assert.IsNotNull(cursor1);
				Assert.IsFalse(Cursor.IsNull(cursor1));
				Assert.AreNotEqual(0, cursor1.GetHashCode());

				Cursor cursor2 = unit.FindCursor("y");
				Assert.IsNotNull(cursor2);
				Assert.IsFalse(Cursor.IsNull(cursor2));
				Assert.AreNotEqual(0, cursor2.GetHashCode());

				// Hash codes should be different from each other and null cursor
				Assert.AreNotEqual(cursor1.GetHashCode(), cursor2.GetHashCode());
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_GetReferenceNameExtents()
		{
			string code = "template <typename T> struct Base { void func(); int operator[](T); }; struct Sub: public Base<int> { void func(); };\r\n" +
				"int main() { Sub inst; inst.Base<int>::operator[](1); return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// It took a while to find something that will work for this test ...
				var cursors = unit.FindCursor("main").FindChildren((c, p) => c.Kind == CursorKind.MemberRefExpr, true);
				Assert.IsNotNull(cursors);
				Assert.AreEqual(1, cursors.Count);

				// inst.Base<int>::operator[](1)
				Cursor cursor = cursors[0].Item1;
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// ReferenceNameExtent.WantSinglePiece
				var singlepiece = cursor.GetReferenceNameExtents(ReferenceNameExtent.WantSinglePiece);
				Assert.IsNotNull(singlepiece);
				Assert.AreEqual(1, singlepiece.Count);
				for (int index = 0; index < singlepiece.Count; index++) Assert.AreSame(singlepiece[index], singlepiece[index]);
				foreach (Extent extent in singlepiece)
				{
					Assert.IsNotNull(extent);
					Assert.IsFalse(Extent.IsNull(extent));
				}

				// Generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)singlepiece) Assert.IsNotNull(obj);

				Assert.AreEqual("operator[]", code.Substring(singlepiece[0].Start.Offset, singlepiece[0].GetCharacterLength()));

				// ReferenceNameExtent.WantQualifier
				var qualifier = cursor.GetReferenceNameExtents(ReferenceNameExtent.WantQualifier);
				Assert.IsNotNull(qualifier);
				Assert.AreEqual(4, qualifier.Count);
				for (int index = 0; index < qualifier.Count; index++) Assert.AreSame(qualifier[index], qualifier[index]);
				foreach (Extent extent in qualifier)
				{
					Assert.IsNotNull(extent);
					Assert.IsFalse(Extent.IsNull(extent));
				}

				// Generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)qualifier) Assert.IsNotNull(obj);

				Assert.AreEqual("Base<int>::", code.Substring(qualifier[0].Start.Offset, qualifier[0].GetCharacterLength()));
				Assert.AreEqual("operator", code.Substring(qualifier[1].Start.Offset, qualifier[1].GetCharacterLength()));
				Assert.AreEqual("[", code.Substring(qualifier[2].Start.Offset, qualifier[2].GetCharacterLength()));
				Assert.AreEqual("]", code.Substring(qualifier[3].Start.Offset, qualifier[3].GetCharacterLength()));

				// ReferenceNameExtent.WantTemplateArguments
				var templateargs = cursor.GetReferenceNameExtents(ReferenceNameExtent.WantTemplateArguments);
				Assert.IsNotNull(templateargs);
				Assert.AreEqual(3, templateargs.Count);
				for (int index = 0; index < templateargs.Count; index++) Assert.AreSame(templateargs[index], templateargs[index]);
				foreach (Extent extent in templateargs)
				{
					Assert.IsNotNull(extent);
					Assert.IsFalse(Extent.IsNull(extent));
				}

				// Generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)templateargs) Assert.IsNotNull(obj);

				Assert.AreEqual("operator", code.Substring(templateargs[0].Start.Offset, templateargs[0].GetCharacterLength()));
				Assert.AreEqual("[", code.Substring(templateargs[1].Start.Offset, templateargs[1].GetCharacterLength()));
				Assert.AreEqual("]", code.Substring(templateargs[2].Start.Offset, templateargs[2].GetCharacterLength()));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_GetSpellingNameExtents()
		{
			string code = "@interface Test3 -(void)setFoo:(int)x withBar:(int)y; @end void foo3(Test3 *test3) { [test3 setFoo:2 withBar:4]; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				ExtentCollection extents;

				// There should be 2 instances of cursors with spelling "setFoo:withBar:", both of which will have 2 spelling extents,
				// the cursor types/kinds involved aren't important for this unit test of GetSpellingNameExtents()
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "setFoo:withBar:", true);
				Assert.IsNotNull(cursors);
				Assert.AreEqual(2, cursors.Count);

				Assert.IsNotNull(cursors[0].Item1);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));
				extents = cursors[0].Item1.GetSpellingNameExtents();
				Assert.IsNotNull(extents);
				Assert.AreEqual(2, extents.Count);
				Assert.AreEqual("setFoo", code.Substring(extents[0].Start.Offset, extents[0].GetCharacterLength()));
				Assert.AreEqual("withBar", code.Substring(extents[1].Start.Offset, extents[1].GetCharacterLength()));

				Assert.IsNotNull(cursors[1].Item1);
				Assert.IsFalse(Cursor.IsNull(cursors[1].Item1));
				extents = cursors[1].Item1.GetSpellingNameExtents();
				Assert.IsNotNull(extents);
				Assert.AreEqual(2, extents.Count);
				Assert.AreEqual("setFoo", code.Substring(extents[0].Start.Offset, extents[0].GetCharacterLength()));
				Assert.AreEqual("withBar", code.Substring(extents[1].Start.Offset, extents[1].GetCharacterLength()));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IBOutletCollectionType()
		{
			string code = "@interface iface { __attribute__((iboutletcollection(iface))) id var; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// get the iface type for comparison
				Type type = unit.FindCursor("iface").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);

				// The IBOutletCollectionAttr cursor cannot be found by spelling, enumerate
				var cursors = unit.Cursor.FindChildren((c, p) => c.Kind == CursorKind.IBOutletCollectionAttr, true);

				// Expect to find one of these, and the IBOutletCollection type should be iface
				Assert.IsNotNull(cursors);
				Assert.AreEqual(1, cursors.Count);
				Assert.AreSame(cursors[0].Item1.IBOutletCollectionType, cursors[0].Item1.IBOutletCollectionType);
				Assert.AreEqual(type, cursors[0].Item1.IBOutletCollectionType);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ImportModule()
		{
			// This one is kind of a pain to get working, specify a specific module map in the
			// arguments and use virtual code to @import them, that creates ModuleImport cursors
			string mappath = Path.Combine(Environment.CurrentDirectory, @"input\module.modulemap");

			string code = "@import testlib.header1; @import testlib.header2;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC,
				new string[] { "-fmodules", "-fmodule-map-file=" + mappath }))
			{
				Cursor cursor = unit.FindCursor("testlib.header1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.ImportModule, cursor.ImportModule);
				Assert.IsNotNull(cursor.ImportModule);
				Assert.IsFalse(Module.IsNull(cursor.ImportModule));
				Assert.AreEqual("testlib.header1", cursor.ImportModule.FullName);

				cursor = unit.FindCursor("testlib.header2");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.ImportModule, cursor.ImportModule);
				Assert.IsNotNull(cursor.ImportModule);
				Assert.IsFalse(Module.IsNull(cursor.ImportModule));
				Assert.AreEqual("testlib.header2", cursor.ImportModule.FullName);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IncludedFile()
		{
			// This test needs a real file, use the include.cpp test code
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Cursor cursor = unit.FindCursor("header1.h");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.IncludedFile, cursor.IncludedFile);
				Assert.IsNotNull(cursor.IncludedFile);
				Assert.IsFalse(File.IsNull(cursor.IncludedFile));
				Assert.AreEqual("header1.h", Path.GetFileName(cursor.IncludedFile.Name));

				cursor = unit.FindCursor("header2.h");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.IncludedFile, cursor.IncludedFile);
				Assert.IsNotNull(cursor.IncludedFile);
				Assert.IsFalse(File.IsNull(cursor.IncludedFile));
				Assert.AreEqual("header2.h", Path.GetFileName(cursor.IncludedFile.Name));

				cursor = unit.FindCursor("header3.h");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.IncludedFile, cursor.IncludedFile);
				Assert.IsNotNull(cursor.IncludedFile);
				Assert.IsFalse(File.IsNull(cursor.IncludedFile));
				Assert.AreEqual("header3.h", Path.GetFileName(cursor.IncludedFile.Name));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsAnonymousRecord()
		{
			string code = "struct s { struct { int x1; int x2; }; union { int u1; int u2; }; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// The anonymous records cannot be located by spelling, use a recursive find
				var cursors = unit.Cursor.FindChildren((c, p) => c.IsAnonymousRecord == true, true);

				// Expectation is that there are 2 of these returned, that's sufficient
				// to verify that this property is working correctly
				Assert.IsNotNull(cursors);
				Assert.AreEqual(2, cursors.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsBitFieldMember()
		{
			string code = "typedef struct s { int x; unsigned char bits1 : 2; unsigned char bits2 : 6; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor parent = unit.FindCursor("s");
				Assert.IsNotNull(parent);
				Assert.IsFalse(Cursor.IsNull(parent));
				Assert.IsTrue(parent.IsDefinition);

				// s::bits2
				Cursor cursor = parent.FindCursor("bits2");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsBitFieldMember);

				// Negative: s::x
				cursor = parent.FindCursor("x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsBitFieldMember);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCanonical()
		{
			string code = "typedef struct s; typedef struct s { int x; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Get both cursors that refer to s, one is a forward declaration
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "s");
				Assert.AreEqual(2, cursors.Count);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));
				Assert.IsFalse(Cursor.IsNull(cursors[1].Item1));

				// The cursors themselves should not be equal
				Assert.IsFalse(cursors[0].Item1 == cursors[1].Item1);

				// The first cursor should be the canonical one
				Assert.IsTrue(cursors[0].Item1.IsCanonical);
				Assert.IsFalse(cursors[1].Item1.IsCanonical);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxConstMethod()
		{
			string code = "class c { public: void const_method(void) const {} void nonconst_method(void) {} };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c").FindCursor("const_method");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxConstMethod);

				// Negative: c::nonconst_method
				cursor = unit.FindCursor("c").FindCursor("nonconst_method");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxConstMethod);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxMutableField()
		{
			string code = "class c { private: int x; mutable int mx; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c").FindCursor("mx");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxMutableField);

				// Negative: c::x
				cursor = unit.FindCursor("c").FindCursor("x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxMutableField);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxPureVirtualMethod()
		{
			string code = "class c { public: void instance(void); virtual void pure_virtual(void) = 0; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// pure_virtual is pure virtual
				Cursor cursor = unit.FindCursor("c").FindCursor("pure_virtual");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxPureVirtualMethod);

				// instance is not
				cursor = unit.FindCursor("c").FindCursor("instance");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxVirtualMethod);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxStaticMethod()
		{
			string code = "class c { public: void instance(void); static void static_func(void); };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// static_func is static
				Cursor cursor = unit.FindCursor("c").FindCursor("static_func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxStaticMethod);

				// instance is not
				cursor = unit.FindCursor("c").FindCursor("instance");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxStaticMethod);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxVirtualBase()
		{
			string code = "class base { }; class virtual_base { }; class c : public base, virtual public virtual_base { };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// virtual_base is virtual
				Cursor cursor = unit.FindCursor("c").FindCursor("class virtual_base");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxVirtualBase);

				// base is not
				cursor = unit.FindCursor("c").FindCursor("class base");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxVirtualBase);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsCxxVirtualMethod()
		{
			string code = "class c { public: void instance(void); virtual void virtual_func(void); virtual void pure_virtual(void) = 0; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// virtual_func is virtual
				Cursor cursor = unit.FindCursor("c").FindCursor("virtual_func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxVirtualMethod);

				// pure_virtual is virtual
				cursor = unit.FindCursor("c").FindCursor("pure_virtual");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsCxxVirtualMethod);

				// instance is not
				cursor = unit.FindCursor("c").FindCursor("instance");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsCxxVirtualMethod);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsDefinition()
		{
			string code = "typedef struct s; typedef struct s { int x; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Get both cursors that refer to s, one is a forward declaration
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "s");
				Assert.AreEqual(2, cursors.Count);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));
				Assert.IsFalse(Cursor.IsNull(cursors[1].Item1));

				// The cursors themselves should not be equal
				Assert.IsFalse(cursors[0].Item1 == cursors[1].Item1);

				// The second cursor is the definition cursor for s
				Assert.IsFalse(cursors[0].Item1.IsDefinition);
				Assert.IsTrue(cursors[1].Item1.IsDefinition);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsDynamicCall()
		{
			string code = "void f0(id a) { int i; MyPoint pt = { 1, 2 }; [a print0]; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// Dynamic calls can't be found with spelling (at least not that I can tell),
				// search all the cursors in the test code and ensure exactly one is found
				var cursors = unit.Cursor.FindChildren((c, p) => c.IsDynamicCall == true, true);
				Assert.AreEqual(1, cursors.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsNull()
		{
			string code = "class c { public: void do_something(void) {} };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Assert.IsTrue(Cursor.IsNull(unit.FindCursor("not_a_cursor")));
				Assert.IsFalse(Cursor.IsNull(unit.FindCursor("c")));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsObjectiveCOptional()
		{
			string code = "@protocol P1\r\n+ (void)classMethodReq0;\r\n-(void)methodReq0;\r\n@optional\r\n+ (void)classMethodOpt1;\r\n" +
				"-(void)methodOpt1;\r\n@required\r\n+ (void)classMethodReq2;\r\n-(void)methodReq2;\r\n@end";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// classMethodOpt1 is @optional
				Cursor cursor = unit.FindCursor("P1").FindCursor("classMethodOpt1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsObjectiveCOptional);

				// methodOpt1 is @optional
				cursor = unit.FindCursor("P1").FindCursor("methodOpt1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsObjectiveCOptional);

				// classMethodReq0 is not
				cursor = unit.FindCursor("P1").FindCursor("classMethodReq0");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsObjectiveCOptional);

				// classMethodReq2 is not
				cursor = unit.FindCursor("P1").FindCursor("classMethodReq2");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsObjectiveCOptional);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_IsVariadic()
		{
			string code = "int main(int argc, char** argv); void variadic_func(int args, ...);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// variadic_func is variadic
				Cursor cursor = unit.FindCursor("variadic_func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsTrue(cursor.IsVariadic);

				// main is not
				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.IsFalse(cursor.IsVariadic);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Kind()
		{
			// Just test a couple of these
			string code = "struct s { int s_x; long s_y; }; class c { int c_x; }; int main(int argc, char** argv);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Assert.IsTrue(unit.Cursor.Kind.IsTranslationUnit);

				Cursor cursor = unit.FindCursor("s");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.StructDecl, cursor.Kind);
				Assert.IsFalse(cursor.Kind.IsAttribute);
				Assert.IsTrue(cursor.Kind.IsDeclaration);
				Assert.IsFalse(cursor.Kind.IsExpression);
				Assert.IsFalse(cursor.Kind.IsInvalid);
				Assert.IsFalse(cursor.Kind.IsPreprocessing);
				Assert.IsFalse(cursor.Kind.IsReference);
				Assert.IsFalse(cursor.Kind.IsStatement);
				Assert.IsFalse(cursor.Kind.IsTranslationUnit);
				Assert.IsFalse(cursor.Kind.IsUnexposed);

				cursor = unit.FindCursor("s").FindCursor("s_x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.FieldDecl, cursor.Kind);
				Assert.IsFalse(cursor.Kind.IsAttribute);
				Assert.IsTrue(cursor.Kind.IsDeclaration);
				Assert.IsFalse(cursor.Kind.IsExpression);
				Assert.IsFalse(cursor.Kind.IsInvalid);
				Assert.IsFalse(cursor.Kind.IsPreprocessing);
				Assert.IsFalse(cursor.Kind.IsReference);
				Assert.IsFalse(cursor.Kind.IsStatement);
				Assert.IsFalse(cursor.Kind.IsTranslationUnit);
				Assert.IsFalse(cursor.Kind.IsUnexposed);

				cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.ClassDecl, cursor.Kind);
				Assert.IsFalse(cursor.Kind.IsAttribute);
				Assert.IsTrue(cursor.Kind.IsDeclaration);
				Assert.IsFalse(cursor.Kind.IsExpression);
				Assert.IsFalse(cursor.Kind.IsInvalid);
				Assert.IsFalse(cursor.Kind.IsPreprocessing);
				Assert.IsFalse(cursor.Kind.IsReference);
				Assert.IsFalse(cursor.Kind.IsStatement);
				Assert.IsFalse(cursor.Kind.IsTranslationUnit);
				Assert.IsFalse(cursor.Kind.IsUnexposed);

				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.FunctionDecl, cursor.Kind);
				Assert.IsFalse(cursor.Kind.IsAttribute);
				Assert.IsTrue(cursor.Kind.IsDeclaration);
				Assert.IsFalse(cursor.Kind.IsExpression);
				Assert.IsFalse(cursor.Kind.IsInvalid);
				Assert.IsFalse(cursor.Kind.IsPreprocessing);
				Assert.IsFalse(cursor.Kind.IsReference);
				Assert.IsFalse(cursor.Kind.IsStatement);
				Assert.IsFalse(cursor.Kind.IsTranslationUnit);
				Assert.IsFalse(cursor.Kind.IsUnexposed);

				cursor = unit.FindCursor("main").FindCursor("argv");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.ParmDecl, cursor.Kind);
				Assert.IsFalse(cursor.Kind.IsAttribute);
				Assert.IsTrue(cursor.Kind.IsDeclaration);
				Assert.IsFalse(cursor.Kind.IsExpression);
				Assert.IsFalse(cursor.Kind.IsInvalid);
				Assert.IsFalse(cursor.Kind.IsPreprocessing);
				Assert.IsFalse(cursor.Kind.IsReference);
				Assert.IsFalse(cursor.Kind.IsStatement);
				Assert.IsFalse(cursor.Kind.IsTranslationUnit);
				Assert.IsFalse(cursor.Kind.IsUnexposed);
			}

			// GITHUB ISSUE #1 (https://github.com/djp952/tools-llvm/issues/1)
			//
			// Test the new "S" format specifier for short cursor kind names
			Assert.AreEqual("macro definition", CursorKind.MacroDefinition.ToString());
			Assert.AreEqual("MacroDefinition", CursorKind.MacroDefinition.ToString("S"));
			Assert.AreEqual("MacroDefinition", CursorKind.MacroDefinition.ToString("s"));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Language()
		{
			string code = "class c { int c_x; }; int main(int argc, char** argv);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// CPlusPlus: c
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Language.CPlusPlus, cursor.Language);

				// C: main
				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Language.C, cursor.Language);
			}

			code = "@protocol P1\r\n-(void)methodReq;\r\n@end";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// ObjectiveC: P1
				Cursor cursor = unit.FindCursor("P1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Language.ObjectiveC, cursor.Language);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_LexicalParentCursor()
		{
			string code = "class c { void func(void); }; void c::func(void) { return; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// c::func declaration, lexical parent should be c
				Cursor cursor = unit.FindCursor("c").FindCursor("func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.LexicalParentCursor, cursor.LexicalParentCursor);
				Assert.IsNotNull(cursor.LexicalParentCursor);
				Assert.IsFalse(Cursor.IsNull(cursor.LexicalParentCursor));
				Assert.AreEqual(unit.FindCursor("c"), cursor.LexicalParentCursor);

				// c::func implementation, lexical parent should be the translation unit cursor
				cursor = unit.FindCursor("func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.LexicalParentCursor, cursor.LexicalParentCursor);
				Assert.IsNotNull(cursor.LexicalParentCursor);
				Assert.IsFalse(Cursor.IsNull(cursor.LexicalParentCursor));
				Assert.AreEqual(unit.Cursor, cursor.LexicalParentCursor);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Linkage()
		{
			string code = "extern int test_external(void); static int test_internal(void); namespace { int test_uniqueexternal(void); } " +
				"int main(void) { int result = 0; return result; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// External: test_external
				Cursor cursor = unit.FindCursor("test_external");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Linkage.External, cursor.Linkage);

				// Internal: test_internal
				cursor = unit.FindCursor("test_internal");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Linkage.Internal, cursor.Linkage);

				// NoLinkage: main::result
				cursor = unit.FindCursor("main").FindCursor("result", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Linkage.NoLinkage, cursor.Linkage);

				// UniqueExternal: TestUniqueExternalLinkage
				cursor = unit.FindCursor("test_uniqueexternal", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(Linkage.UniqueExternal, cursor.Linkage);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Location()
		{
			string code = "class c { public: void do_something(void) {} };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// The reference object returned should be the same each time
				Assert.AreSame(cursor.Location, cursor.Location);

				// Not really concerned about the value, Location has its own set of tests
				Assert.IsNotNull(cursor.Location);
				Assert.IsFalse(Location.IsNull(cursor.Location));
				Assert.AreNotSame(0, cursor.Location.Offset);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_MangledName()
		{
			string code = "void func(void) {} int main(int argc, char** argv) { func(); return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// The name mangling can differ by platform, so just make sure it comes back
				Assert.AreSame(cursor.MangledName, cursor.MangledName);
				Assert.IsFalse(String.IsNullOrEmpty(cursor.MangledName));
				Assert.AreNotEqual(cursor.Spelling, cursor.MangledName);
				Assert.AreNotEqual(cursor.DisplayName, cursor.MangledName);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_MessageReceiverType()
		{
			string code = "@interface XYZPerson : NSObject - (void)sayHello; @end\r\n" +
				"@implementation XYZPerson - (void)sayHello { [self saySomething:@\"Hello, world!\"]; } - (void)saySomething:(NSString *)greeting { NSLog(@\"%@\", greeting); } @end";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("saySomething:", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(cursor.MessageReceiverType, cursor.MessageReceiverType);
				Assert.IsNotNull(cursor.MessageReceiverType);
				Assert.AreNotEqual(TypeKind.Invalid, cursor.MessageReceiverType.Kind);
				Assert.AreEqual("XYZPerson *", cursor.MessageReceiverType.Spelling);
				Assert.AreEqual(TypeKind.ObjectiveCObjectPointer, cursor.MessageReceiverType.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Null()
		{
			// Cursor.Null should return the same reference object every time
			Assert.AreSame(Cursor.Null, Cursor.Null);

			Cursor cursor = Cursor.Null;
			Assert.IsNotNull(cursor);

			// Ensure that all methods and properties return the expected objects/values
			Assert.AreEqual(0, cursor.ArgumentCursors.Count);
			Assert.AreEqual(Availability.Available, cursor.Availability);
			Assert.AreEqual(String.Empty, cursor.BriefComment);
			Assert.IsTrue(Extent.IsNull(cursor.Comment.Extent));
			Assert.AreEqual(String.Empty, cursor.Comment.Text);
			Assert.AreEqual(CxxAccessSpecifier.Invalid, cursor.CxxAccessSpecifier);
			Assert.AreEqual(0, cursor.CxxMangledSymbols.Count);
			Assert.AreEqual(String.Empty, cursor.DisplayName);
			Assert.AreEqual(0, (int)cursor.EnumConstant);
			Assert.AreEqual(TypeKind.Invalid, cursor.EnumType.Kind);
			Assert.IsTrue(Extent.IsNull(cursor.Extent));
			Assert.IsTrue(Cursor.IsNull(cursor.CanonicalCursor));
			Assert.IsTrue(Cursor.IsNull(cursor.DefinitionCursor));
			Assert.AreEqual(null, cursor.FieldBitWidth);
			Assert.AreNotEqual(0, cursor.GetHashCode());    // This returns a non-zero hash on purpose
			Assert.AreEqual(0, cursor.GetReferenceNameExtents(ReferenceNameExtent.WantSinglePiece).Count);
			Assert.AreEqual(0, cursor.GetSpellingNameExtents().Count);
			Assert.AreEqual(TypeKind.Invalid, cursor.IBOutletCollectionType.Kind);
			Assert.IsTrue(Module.IsNull(cursor.ImportModule));
			Assert.IsTrue(File.IsNull(cursor.IncludedFile));
			Assert.AreEqual(false, cursor.IsAnonymousRecord);
			Assert.AreEqual(false, cursor.IsBitFieldMember);
			Assert.AreEqual(true, cursor.IsCanonical);          // Null cursor is canonical
			Assert.AreEqual(false, cursor.IsCxxConstMethod);
			Assert.AreEqual(false, cursor.IsCxxMutableField);
			Assert.AreEqual(false, cursor.IsCxxPureVirtualMethod);
			Assert.AreEqual(false, cursor.IsCxxStaticMethod);
			Assert.AreEqual(false, cursor.IsCxxVirtualBase);
			Assert.AreEqual(false, cursor.IsCxxVirtualMethod);
			Assert.AreEqual(false, cursor.IsDefinition);
			Assert.AreEqual(false, cursor.IsDynamicCall);
			Assert.AreEqual(false, cursor.IsObjectiveCOptional);
			Assert.AreEqual(false, cursor.IsVariadic);
			Assert.AreEqual(CursorKind.InvalidFile, cursor.Kind);
			Assert.AreEqual(Language.Invalid, cursor.Language);
			Assert.IsTrue(Cursor.IsNull(cursor.LexicalParentCursor));
			Assert.AreEqual(Linkage.Invalid, cursor.Linkage);
			Assert.IsTrue(Location.IsNull(cursor.Location));
			Assert.AreEqual(String.Empty, cursor.MangledName, String.Empty);
			Assert.AreEqual(TypeKind.Invalid, cursor.MessageReceiverType.Kind);
			Assert.AreEqual(ObjectiveCDeclarationQualifier.None, cursor.ObjectiveCDeclarationQualifiers);
			Assert.AreEqual(ObjectiveCPropertyAttribute.None, cursor.ObjectiveCPropertyAttributes);
			Assert.AreEqual(null, cursor.ObjectiveCSelectorIndex);
			Assert.AreEqual(String.Empty, cursor.ObjectiveCTypeEncoding);
			Assert.AreEqual(0, cursor.OverloadedDeclarationCursors.Count);
			Assert.AreEqual(0, cursor.OverriddenCursors.Count);
			Assert.AreEqual(CommentKind.Null, cursor.ParsedComment.Kind);
			Assert.AreEqual(0, cursor.PlatformAvailability.Count);
			Assert.AreEqual(false, cursor.PlatformAvailability.AlwaysDeprecated);
			Assert.AreEqual(String.Empty, cursor.PlatformAvailability.AlwaysDeprecatedMessage);
			Assert.AreEqual(false, cursor.PlatformAvailability.AlwaysUnavailable);
			Assert.AreEqual(String.Empty, cursor.PlatformAvailability.AlwaysUnavailableMessage);
			Assert.AreEqual(null, cursor.RecordOffset);
			Assert.IsTrue(Cursor.IsNull(cursor.ReferencedCursor));
			Assert.AreEqual(TypeKind.Invalid, cursor.ResultType.Kind);
			Assert.IsTrue(Cursor.IsNull(cursor.SemanticParentCursor));
			Assert.IsTrue(Cursor.IsNull(cursor.SpecializedTemplateCursor));
			Assert.AreEqual(String.Empty, cursor.Spelling);
			Assert.AreEqual(StorageClass.Invalid, cursor.StorageClass);
			Assert.AreEqual(0, cursor.TemplateArguments.Count);
			Assert.AreEqual(CursorKind.NoDeclFound, cursor.TemplateCursorKind);
			Assert.AreEqual("<null>", cursor.ToString());
			Assert.AreEqual(TypeKind.Invalid, cursor.Type.Kind);
			Assert.AreEqual(TypeKind.Invalid, cursor.UnderlyingTypedefType.Kind);
			Assert.AreEqual(String.Empty, cursor.UnifiedSymbolResolution);  // implicit conversion
			Assert.AreEqual(CursorVisibility.Invalid, cursor.Visibility);
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ObjectiveCDeclarationQualifiers()
		{
			string code = "@protocol P +(oneway id) stuff : (byref id)arg : (bycopy oneway id)arg1; @end";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("stuff::", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(ObjectiveCDeclarationQualifier.OneWay, cursor.ObjectiveCDeclarationQualifiers);

				cursor = unit.FindCursor("arg", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(ObjectiveCDeclarationQualifier.ByRef, cursor.ObjectiveCDeclarationQualifiers);

				cursor = unit.FindCursor("arg1", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(ObjectiveCDeclarationQualifier.ByCopy | ObjectiveCDeclarationQualifier.OneWay, cursor.ObjectiveCDeclarationQualifiers);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ObjectiveCPropertyAttributes()
		{
			string code = "@interface Test @property (nonatomic, assign) void *ptr; @property (readwrite, assign) id x;@end\r\n";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("ptr", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(ObjectiveCPropertyAttribute.Assign | ObjectiveCPropertyAttribute.NonAtomic, cursor.ObjectiveCPropertyAttributes);

				cursor = unit.FindCursor("x", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(ObjectiveCPropertyAttribute.Assign | ObjectiveCPropertyAttribute.ReadWrite, cursor.ObjectiveCPropertyAttributes);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ObjectiveCSelectorIndex()
		{
			// The only way to seemingly get this to work is to grab a cursor from a specific location, if
			// you iterate to the cursor clang isn't initializing it properly for this operation (clang bug?)
			string code = "@interface Test3 -(void)setFoo:(int)x withBar:(int)y; @end void foo3(Test3 *test3) { [test3 setFoo:2 withBar:4]; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				// One would expect this to work since it gets the same cursor, but it does not ...
				// Cursor cursor = unit.FindCursor("setFoo:withBar:", true);

				// This, on the other hand, works fine and will get an ObjectiveCSelectorIndex
				Cursor cursor = unit.GetFile("t.m").GetLocation(1, 41).Cursor;

				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(1, cursor.ObjectiveCSelectorIndex);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ObjectiveCTypeEncoding()
		{
			// Note: There seems to be a bug in libclang that prevents this method from working
			// properly on typedefs (https://llvm.org/bugs/show_bug.cgi?id=15074), so use a function
			string code = "int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.ObjectiveCTypeEncoding, cursor.ObjectiveCTypeEncoding);

				// This differs on x86/x64 builds, use the size of IntPtr to detect
				if (IntPtr.Size == 4) Assert.AreEqual("i8i0^*4", cursor.ObjectiveCTypeEncoding);
				else if (IntPtr.Size == 8) Assert.AreEqual("i12i0^*4", cursor.ObjectiveCTypeEncoding);
				else Assert.Fail("Unsupported architecture");
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_OverloadedDeclarationCursors()
		{
			string code = "namespace outer { namespace inner { template<typename T> struct vector { typedef T* iterator; }; } }\r\n" +
				"namespace outer_alias = outer; struct X {}; using outer_alias::inner::vector;\r\n" +
				"struct X_vector : outer_alias::inner::vector<X> { using outer_alias::inner::vector<X>::iterator; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// There should be two cursors with overloaded declarations
				var cursors = unit.Cursor.FindChildren((c, p) => c.OverloadedDeclarationCursors.Count > 0, true);
				Assert.IsNotNull(cursors);
				Assert.AreEqual(2, cursors.Count);

				// using outer_alias::inner::vector
				Cursor cursor = cursors[0].Item1;
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.OverloadedDeclarationCursors, cursor.OverloadedDeclarationCursors);
				Assert.AreEqual(1, cursor.OverloadedDeclarationCursors.Count);
				Assert.AreSame(cursor.OverloadedDeclarationCursors[0], cursor.OverloadedDeclarationCursors[0]);
				Assert.IsNotNull(cursor.OverloadedDeclarationCursors[0]);
				Assert.IsFalse(Cursor.IsNull(cursor.OverloadedDeclarationCursors[0]));

				// using outer_alias::inner::vector<X>::iterator
				cursor = cursors[1].Item1;
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.OverloadedDeclarationCursors, cursor.OverloadedDeclarationCursors);
				Assert.AreEqual(1, cursor.OverloadedDeclarationCursors.Count);
				Assert.AreSame(cursor.OverloadedDeclarationCursors[0], cursor.OverloadedDeclarationCursors[0]);
				Assert.IsNotNull(cursor.OverloadedDeclarationCursors[0]);
				Assert.IsFalse(Cursor.IsNull(cursor.OverloadedDeclarationCursors[0]));

				// Hit the enumerators for code coverage
				foreach (Cursor c in cursor.OverloadedDeclarationCursors) Assert.IsNotNull(c);
				foreach (Object obj in (System.Collections.IEnumerable)cursor.OverloadedDeclarationCursors) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_OverriddenCursors()
		{
			string code = "class base { public: virtual int do_something(void) { return 1; } }; class derived : public base { public: virtual void do_something(void) { return2 ; } };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// base declaration shouldn't have any overridden cursors
				Cursor basedecl = unit.FindCursor("base").FindCursor("do_something");
				Assert.IsNotNull(basedecl);
				Assert.IsFalse(Cursor.IsNull(basedecl));
				Assert.AreSame(basedecl.OverriddenCursors, basedecl.OverriddenCursors);
				Assert.IsNotNull(basedecl.OverriddenCursors);
				Assert.AreEqual(0, basedecl.OverriddenCursors.Count);

				// derived declaration should have one overridden cursor
				Cursor derived = unit.FindCursor("derived").FindCursor("do_something");
				Assert.IsNotNull(derived);
				Assert.IsFalse(Cursor.IsNull(derived));
				Assert.AreSame(derived.OverriddenCursors, derived.OverriddenCursors);
				Assert.IsNotNull(derived.OverriddenCursors);
				Assert.AreEqual(1, derived.OverriddenCursors.Count);
				Assert.AreSame(derived.OverriddenCursors[0], derived.OverriddenCursors[0]);
				Assert.AreEqual(basedecl, derived.OverriddenCursors[0]);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ParsedComment()
		{
			// Comments have their own set of unit tests, this doesn't need to be comprehensive
			string code = "/**\r\n*\\brief This is the brief comment.\r\n*\r\n*This is the long comment.\r\n*/\r\n" +
				"int func_with_comment(void); int func_without_comment(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("func_with_comment");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(cursor.ParsedComment, cursor.ParsedComment);
				Assert.IsNotNull(cursor.ParsedComment);
				Assert.AreEqual(CommentKind.Parsed, cursor.ParsedComment.Kind);
				Assert.AreEqual(2, cursor.ParsedComment.Children.Count);
				Assert.AreSame(cursor.ParsedComment.Children[0], cursor.ParsedComment.Children[0]);

				// There should be one block command and one paragraph, don't dive deeper here
				Assert.AreEqual(CommentKind.BlockCommand, cursor.ParsedComment.Children[0].Kind);
				BlockCommandComment block = (BlockCommandComment)cursor.ParsedComment.Children[0];
				Assert.AreEqual(CommentKind.Paragraph, cursor.ParsedComment.Children[1].Kind);
				ParagraphComment paragraph = (ParagraphComment)cursor.ParsedComment.Children[1];

				// Make sure HTML and XML return something
				Assert.IsFalse(String.IsNullOrEmpty(cursor.ParsedComment.ToHtmlFragment()));
				Assert.IsFalse(String.IsNullOrEmpty(cursor.ParsedComment.ToXmlFragment()));

				// When there is no comment, .ParsedComment should return a null comment
				cursor = unit.FindCursor("func_without_comment");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.ParsedComment, cursor.ParsedComment);
				Assert.IsNotNull(cursor.ParsedComment);
				Assert.AreEqual(CommentKind.Null, cursor.ParsedComment.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_PlatformAvailability()
		{
			string code = "__attribute__((availability(macosx,introduced=10.5.1))) @interface WeakClass1 @end";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("WeakClass1");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(cursor.PlatformAvailability, cursor.PlatformAvailability);
				Assert.AreEqual(1, cursor.PlatformAvailability.Count);
				Assert.AreSame(cursor.PlatformAvailability[0], cursor.PlatformAvailability[0]);
				Assert.AreEqual(new Version(10, 5, 1), cursor.PlatformAvailability[0].Introduced);
				Assert.AreEqual(new Version(0, 0), cursor.PlatformAvailability[0].Deprecated);
				Assert.AreEqual("", cursor.PlatformAvailability[0].Message);
				Assert.AreEqual(new Version(0, 0), cursor.PlatformAvailability[0].Obsoleted);
				Assert.AreEqual("macosx", cursor.PlatformAvailability[0].Platform);
				Assert.IsFalse(cursor.PlatformAvailability[0].Unavailable);

				// Hit the enumerators for code coverage
				foreach (PlatformAvailability availability in cursor.PlatformAvailability) Assert.IsNotNull(availability);
				foreach (Object obj in (System.Collections.IEnumerable)cursor.PlatformAvailability) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_RecordOffset()
		{
			string code = "typedef struct s { int intval; long long longlongval; unsigned char bits1 : 2; unsigned char bits2 : 6; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor parent = unit.FindCursor("s").DefinitionCursor;
				Assert.IsNotNull(parent);
				Assert.IsFalse(Cursor.IsNull(parent));
				Assert.IsTrue(parent.IsDefinition);

				// s::intval
				Cursor record = parent.FindCursor("intval");
				Assert.IsNotNull(record);
				Assert.IsFalse(Cursor.IsNull(record));
				Assert.AreEqual(0U, record.RecordOffset);

				// s::longlongval
				record = parent.FindCursor("longlongval");
				Assert.IsNotNull(record);
				Assert.IsFalse(Cursor.IsNull(record));
				Assert.AreEqual(64U, record.RecordOffset);

				// s::bits1
				record = parent.FindCursor("bits1");
				Assert.IsNotNull(record);
				Assert.IsFalse(Cursor.IsNull(record));
				Assert.AreEqual(128U, record.RecordOffset);

				// s::bits2
				record = parent.FindCursor("bits2");
				Assert.IsNotNull(record);
				Assert.IsFalse(Cursor.IsNull(record));
				Assert.AreEqual(130U, record.RecordOffset);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ReferencedCursor()
		{
			string code = "void func(void) {} int main(int argc, char** argv) { func(); return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor func = unit.FindCursor("func");
				Assert.IsNotNull(func);
				Assert.IsFalse(Cursor.IsNull(func));

				Cursor main = unit.FindCursor("main");
				Assert.IsNotNull(main);
				Assert.IsFalse(Cursor.IsNull(main));

				// Find reference to func() in main()
				var cursors = main.FindChildren((c, p) => c.Spelling == "func" && c.Kind == CursorKind.CallExpr, true);
				Assert.AreEqual(1, cursors.Count);
				Assert.IsNotNull(cursors[0].Item1);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));

				// Make sure the referenced cursor is the function declaration cursor
				Assert.AreSame(cursors[0].Item1.ReferencedCursor, cursors[0].Item1.ReferencedCursor);
				Assert.IsNotNull(cursors[0].Item1.ReferencedCursor);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1.ReferencedCursor));
				Assert.AreEqual(func, cursors[0].Item1.ReferencedCursor);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ResultType()
		{
			string code = "int main(int argc, char** argv) { return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.ResultType, cursor.ResultType);
				Assert.IsNotNull(cursor.ResultType);
				Assert.AreNotEqual(TypeKind.Invalid, cursor.ResultType.Kind);
				Assert.AreEqual(TypeKind.Int, cursor.ResultType.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_SemanticParentCursor()
		{
			string code = "class c { void func(void); }; void c::func(void) { return; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor parent = unit.FindCursor("c");
				Assert.IsNotNull(parent);
				Assert.IsFalse(Cursor.IsNull(parent));

				// Find both "func" cursors, the semantic parent for each should be class "c"
				foreach (Tuple<Cursor, Cursor> cursors in unit.Cursor.FindChildren((c, p) => c.Spelling == "func", true))
				{
					Assert.IsNotNull(cursors.Item1);
					Assert.IsFalse(Cursor.IsNull(cursors.Item1));

					// The cursor should both have a sematic parent of class "c"
					Assert.AreSame(cursors.Item1.SemanticParentCursor, cursors.Item1.SemanticParentCursor);
					Assert.AreEqual(parent, cursors.Item1.SemanticParentCursor);
					Assert.IsTrue(parent == cursors.Item1.SemanticParentCursor);
				}
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_SpecializedTemplateCursor()
		{
			string code = "template <typename _t, int _i, bool _b> void func(void); template<> void func<double, -3, true>(void) { return; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// Find the func<> function declaration cursor
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "func" && c.Kind == CursorKind.FunctionDecl, true);
				Assert.AreEqual(1, cursors.Count);
				Assert.IsNotNull(cursors[0].Item1);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));

				// .SpecializedTemplateCursor should refer back to the template
				Assert.AreSame(cursors[0].Item1.SpecializedTemplateCursor, cursors[0].Item1.SpecializedTemplateCursor);
				Assert.IsNotNull(cursors[0].Item1.SpecializedTemplateCursor);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1.SpecializedTemplateCursor));
				Assert.AreEqual(CursorKind.FunctionTemplate, cursors[0].Item1.SpecializedTemplateCursor.Kind);
				Assert.AreEqual("func()", cursors[0].Item1.SpecializedTemplateCursor.DisplayName);
				Assert.AreEqual(unit.FindCursor("func"), cursors[0].Item1.SpecializedTemplateCursor);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Spelling()
		{
			// .Spelling is extraordinarily tested every time .FindCursor() is called, which is
			// in the bulk of these unit tests; just do a very quick check
			string code = "class c { public: void do_something(void) {} };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Spelling, cursor.Spelling);
				Assert.AreEqual("c", cursor.Spelling);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_StorageClass()
		{
			string code = "int nx; static int sx; extern int ex; __private_extern__ int pex; register int rx; auto int ax;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("nx");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.None, cursor.StorageClass);

				cursor = unit.FindCursor("sx");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.Static, cursor.StorageClass);

				cursor = unit.FindCursor("ex");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.Extern, cursor.StorageClass);

				cursor = unit.FindCursor("pex");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.PrivateExtern, cursor.StorageClass);

				cursor = unit.FindCursor("rx");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.Register, cursor.StorageClass);

				cursor = unit.FindCursor("ax");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(StorageClass.Auto, cursor.StorageClass);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_TemplateArguments()
		{
			string code = "template <typename _t, int _i, bool _b> void func(void); template<> void func<double, -3, true>(void) { return; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// Find the func<> function declaration cursor
				var cursors = unit.Cursor.FindChildren((c, p) => c.Spelling == "func" && c.Kind == CursorKind.FunctionDecl, true);
				Assert.AreEqual(1, cursors.Count);
				Assert.IsNotNull(cursors[0].Item1);
				Assert.IsFalse(Cursor.IsNull(cursors[0].Item1));

				Assert.AreSame(cursors[0].Item1.TemplateArguments, cursors[0].Item1.TemplateArguments);
				Assert.IsNotNull(cursors[0].Item1.TemplateArguments);

				TemplateArgumentCollection args = cursors[0].Item1.TemplateArguments;
				Assert.AreEqual(3, args.Count);
				foreach (TemplateArgument t in cursors[0].Item1.TemplateArguments) Assert.IsNotNull(t);
				foreach (Object obj in (System.Collections.IEnumerable)cursors[0].Item1.TemplateArguments) Assert.IsNotNull(obj);


				// args[0] -> double
				Assert.AreSame(args[0], args[0]);
				Assert.AreEqual(TemplateArgumentKind.Type, args[0].Kind);
				Assert.AreSame(args[0].Type, args[0].Type);
				Assert.IsNotNull(args[0].Type);
				Assert.AreNotEqual(TypeKind.Invalid, args[0].Type.Kind);
				Assert.AreEqual(null, args[0].UnsignedValue);
				Assert.AreEqual(null, args[0].Value);

				// args[1] -> int / literal -3
				Assert.AreSame(args[1], args[1]);
				Assert.AreEqual(TemplateArgumentKind.Integral, args[1].Kind);
				Assert.AreSame(args[1].Type, args[1].Type);
				Assert.IsNotNull(args[1].Type);
				Assert.AreEqual(TypeKind.Invalid, args[1].Type.Kind);
				Assert.AreEqual(unchecked((uint)-3), args[1].UnsignedValue);
				Assert.AreEqual(-3, args[1].Value);

				// args[2] -> bool / literal true
				Assert.AreSame(args[2], args[2]);
				Assert.AreEqual(TemplateArgumentKind.Integral, args[2].Kind);
				Assert.AreSame(args[2].Type, args[2].Type);
				Assert.IsNotNull(args[2].Type);
				Assert.AreEqual(TypeKind.Invalid, args[2].Type.Kind);
				Assert.AreEqual(1U, args[2].UnsignedValue);
				Assert.AreEqual(1, args[2].Value);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_TemplateCursorKind()
		{
			string code = "template<typename _t> class t_class {}; template<typename _t> void t_func(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("t_class");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.ClassDecl, cursor.TemplateCursorKind);

				cursor = unit.FindCursor("t_func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorKind.FunctionDecl, cursor.TemplateCursorKind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_ToString()
		{
			// Just make sure that .ToString() returns the same as .DisplayName
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath))
			{
				bool foundone = false;
				foreach (Tuple<Cursor, Cursor> cursor in unit.Cursor.GetChildren(true))
				{
					foundone = true;
					Assert.AreEqual(cursor.Item1.DisplayName, cursor.Item1.ToString());
					Assert.AreEqual(cursor.Item2.DisplayName, cursor.Item2.ToString());
				}

				Assert.IsTrue(foundone);            // Need to have found at least one
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Type()
		{
			string code = "void func(void) { return; } int main(int argc, char** argv) { int x = 123; char* p = 0; func(); return 0; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// Just check a few of the cursors to ensure that .Type is working
				Cursor cursor = unit.FindCursor("func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Type, cursor.Type);
				Assert.IsNotNull(cursor.Type);
				Assert.AreEqual(TypeKind.FunctionProto, cursor.Type.Kind);

				cursor = unit.FindCursor("main");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Type, cursor.Type);
				Assert.IsNotNull(cursor.Type);
				Assert.AreEqual(TypeKind.FunctionProto, cursor.Type.Kind);

				cursor = unit.FindCursor("main").FindCursor("x", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Type, cursor.Type);
				Assert.IsNotNull(cursor.Type);
				Assert.AreEqual(TypeKind.Int, cursor.Type.Kind);

				cursor = unit.FindCursor("main").FindCursor("func", true);
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.Type, cursor.Type);
				Assert.IsNotNull(cursor.Type);
				Assert.AreEqual(TypeKind.Void, cursor.Type.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_UnderlyingTypedefType()
		{
			string code = "typedef long long mytype;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("mytype");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				Assert.AreSame(cursor.UnderlyingTypedefType, cursor.UnderlyingTypedefType);
				Assert.IsNotNull(cursor.UnderlyingTypedefType);
				Assert.AreEqual(TypeKind.LongLong, cursor.UnderlyingTypedefType.Kind);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_UnifiedSymbolResolution()
		{
			// Just check a couple simple ones, USR has it's own set of unit tests
			string code = "enum Pizza { CHEESE, MUSHROOMS }; static int local_func(int x) { return x; }";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.ObjectiveC))
			{
				Cursor cursor = unit.FindCursor("Pizza");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.UnifiedSymbolResolution, cursor.UnifiedSymbolResolution);
				Assert.IsFalse(String.IsNullOrEmpty(cursor.UnifiedSymbolResolution));
				Assert.AreEqual("c:@E@Pizza", cursor.UnifiedSymbolResolution);

				cursor = unit.FindCursor("local_func");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreSame(cursor.UnifiedSymbolResolution, cursor.UnifiedSymbolResolution);
				Assert.IsFalse(String.IsNullOrEmpty(cursor.UnifiedSymbolResolution));
				Assert.AreEqual("c:t.m@F@local_func", cursor.UnifiedSymbolResolution);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void Cursor_Visibility()
		{
			string code = "__attribute((visibility(\"hidden\"))) int x; __attribute((visibility(\"protected\"))) int y; int z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Cursor cursor = unit.FindCursor("x");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorVisibility.Hidden, cursor.Visibility);

				cursor = unit.FindCursor("y");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorVisibility.Protected, cursor.Visibility);

				cursor = unit.FindCursor("z");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));
				Assert.AreEqual(CursorVisibility.Default, cursor.Visibility);

			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorKind_Equality()
		{
			CursorKind kind = CursorKind.EnumDecl;
			Assert.IsTrue(kind == CursorKind.EnumDecl);
			Assert.IsTrue(CursorKind.EnumDecl == kind);
			Assert.AreEqual(kind, CursorKind.EnumDecl);
			Assert.AreEqual(CursorKind.EnumDecl, kind);
			Assert.IsTrue(kind.Equals(CursorKind.EnumDecl));
			Assert.IsTrue(CursorKind.EnumDecl.Equals(kind));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorKind_Inequality()
		{
			CursorKind kind = CursorKind.EnumDecl;
			Assert.IsTrue(kind != CursorKind.VarDecl);
			Assert.IsTrue(CursorKind.VarDecl != kind);
			Assert.AreNotEqual(kind, CursorKind.VarDecl);
			Assert.AreNotEqual(CursorKind.VarDecl, kind);
			Assert.IsFalse(kind.Equals(CursorKind.VarDecl));
			Assert.IsFalse(CursorKind.VarDecl.Equals(kind));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorKind_GetHashCode()
		{
			// Just check a couple of these, there are a lot
			Assert.AreNotEqual(0, CursorKind.EnumDecl.GetHashCode());
			Assert.AreNotEqual(0, CursorKind.VarDecl.GetHashCode());
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorKind_ToString()
		{
			// Just check a couple of these, there are a lot
			Assert.IsFalse(String.IsNullOrEmpty(CursorKind.EnumDecl.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(CursorKind.VarDecl.ToString()));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateReferencesResult_Equality()
		{
			EnumerateReferencesResult result = EnumerateReferencesResult.Break;
			Assert.IsTrue(result == EnumerateReferencesResult.Break);
			Assert.IsTrue(EnumerateReferencesResult.Break == result);
			Assert.AreEqual(result, EnumerateReferencesResult.Break);
			Assert.AreEqual(EnumerateReferencesResult.Break, result);
			Assert.IsTrue(result.Equals(EnumerateReferencesResult.Break));
			Assert.IsTrue(EnumerateReferencesResult.Break.Equals(result));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateReferencesResult_Inequality()
		{
			EnumerateReferencesResult result = EnumerateReferencesResult.Break;
			Assert.IsTrue(result != EnumerateReferencesResult.Continue);
			Assert.IsTrue(EnumerateReferencesResult.Continue != result);
			Assert.AreNotEqual(result, EnumerateReferencesResult.Continue);
			Assert.AreNotEqual(EnumerateReferencesResult.Continue, result);
			Assert.IsFalse(result.Equals(EnumerateReferencesResult.Continue));
			Assert.IsFalse(EnumerateReferencesResult.Continue.Equals(result));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateReferencesResult_GetHashCode()
		{
			Assert.AreNotEqual(0, EnumerateReferencesResult.Continue.GetHashCode());

			// Break is zero, so the hash code will also be zero
			Assert.AreEqual(0, EnumerateReferencesResult.Break.GetHashCode());
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateReferencesResult_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateReferencesResult.Break.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateReferencesResult.Continue.ToString()));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateChildrenResult_Equality()
		{
			EnumerateChildrenResult result = EnumerateChildrenResult.Break;
			Assert.IsTrue(result == EnumerateChildrenResult.Break);
			Assert.IsTrue(EnumerateChildrenResult.Break == result);
			Assert.AreEqual(result, EnumerateChildrenResult.Break);
			Assert.AreEqual(EnumerateChildrenResult.Break, result);
			Assert.IsTrue(result.Equals(EnumerateChildrenResult.Break));
			Assert.IsTrue(EnumerateChildrenResult.Break.Equals(result));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateChildrenResult_Inequality()
		{
			EnumerateChildrenResult result = EnumerateChildrenResult.Break;
			Assert.IsTrue(result != EnumerateChildrenResult.Continue);
			Assert.IsTrue(EnumerateChildrenResult.Continue != result);
			Assert.AreNotEqual(result, EnumerateChildrenResult.Continue);
			Assert.AreNotEqual(EnumerateChildrenResult.Continue, result);
			Assert.IsFalse(result.Equals(EnumerateChildrenResult.Continue));
			Assert.IsFalse(EnumerateChildrenResult.Continue.Equals(result));
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateChildrenResult_GetHashCode()
		{
			Assert.AreNotEqual(0, EnumerateChildrenResult.Continue.GetHashCode());

			// Break is zero, so the hash code will also be zero
			Assert.AreEqual(0, EnumerateChildrenResult.Break.GetHashCode());
		}

		[TestMethod(), TestCategory("Cursors")]
		public void EnumerateChildrenResult_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateChildrenResult.Break.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateChildrenResult.Continue.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateChildrenResult.Recurse.ToString()));
		}
		
		//
		// EXTENSIONS
		//

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_EnumerateReferences()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				Cursor cursor = unit.FindCursor("main").FindCursor("argv");
				Assert.IsNotNull(cursor);
				Assert.IsFalse(Cursor.IsNull(cursor));

				// Attempt to find at least one reference using the extension method (File)
				Cursor found = Cursor.Null;
				cursor.EnumerateReferences(unit.GetFile(inpath), (c, e) => { found = c; return EnumerateReferencesResult.Break; });
				Assert.IsNotNull(found);
				Assert.IsFalse(Cursor.IsNull(found));
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_FindChildren()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Find all argument cursors for hello.cpp::main() (should be 2: argc and argv)
				var results = unit.FindCursor("main").FindChildren((c, p) => c.Kind == CursorKind.ParmDecl);
				Assert.IsNotNull(results);
				Assert.AreEqual(2, results.Count);

				// Recursively find all exposed "argc" cursors (should be 2 of them)
				results = unit.FindCursor("main").FindChildren((c, p) => c.DisplayName == "argc" && c.Kind != CursorKind.UnexposedExpr, true);
				Assert.IsNotNull(results);
				Assert.AreEqual(2, results.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_FindReferences()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Find the parameter declaration in the main file to argument main::argv (File)
				var references = unit.FindCursor("main").FindCursor("argv").FindReferences(unit.GetFile(inpath), (c, e) => c.Kind == CursorKind.ParmDecl);
				Assert.IsNotNull(references);
				Assert.AreEqual(1, references.Count);

				// Find the parameter declaration in the main file to argument main::argv (string)
				references = unit.FindCursor("main").FindCursor("argv").FindReferences(inpath, (c, e) => c.Kind == CursorKind.ParmDecl);
				Assert.IsNotNull(references);
				Assert.AreEqual(1, references.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_GetChildren()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Get all children of hello.cpp::main()
				var children = unit.FindCursor("main").GetChildren();
				Assert.IsNotNull(children);
				Assert.AreNotEqual(0, children.Count);

				// Get all children of hello.cpp::main() (recursive)
				children = unit.FindCursor("main").GetChildren(true);
				Assert.IsNotNull(children);
				Assert.AreNotEqual(0, children.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_GetReferences()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Get all references in the main file to argument main::argv (File)
				var references = unit.FindCursor("main").FindCursor("argv").GetReferences(unit.GetFile(inpath));
				Assert.IsNotNull(references);
				Assert.AreNotEqual(0, references.Count);

				// Get all references in the main file to argument main::argv (string)
				references = unit.FindCursor("main").FindCursor("argv").GetReferences(inpath);
				Assert.IsNotNull(references);
				Assert.AreNotEqual(0, references.Count);
			}
		}

		[TestMethod(), TestCategory("Cursors")]
		public void CursorExtensions_GetTokens()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit unit = s_index.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Tokenize hello.cpp::main()
				var tokens = unit.FindCursor("main").GetTokens();
				Assert.IsNotNull(tokens);
				Assert.AreNotEqual(0, tokens.Count);
			}
		}
	}
}
