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
using Microsoft.VisualStudio.TestTools.UnitTesting;
using zuki.tools.llvm.clang.extensions;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestType
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

		[TestMethod(), TestCategory("Types")]
		public void Type_Equality()
		{
			string code = "int x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type typex = unit.FindCursor("x").Type;
				Assert.IsNotNull(typex);
				Assert.AreNotEqual(TypeKind.Invalid, typex.Kind);

				Type typey = unit.FindCursor("y").Type;
				Assert.IsNotNull(typey);
				Assert.AreNotEqual(TypeKind.Invalid, typey.Kind);

				Assert.IsTrue(typex.Equals(typey));
				Assert.IsTrue(typey.Equals(typex));
				Assert.AreEqual(typex, typey);
				Assert.AreEqual(typey, typex);
				Assert.IsTrue(typex == typey);
				Assert.IsTrue(typey == typex);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_Inequality()
		{
			string code = "int x; long y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type typex = unit.FindCursor("x").Type;
				Assert.IsNotNull(typex);
				Assert.AreNotEqual(TypeKind.Invalid, typex.Kind);

				Type typey = unit.FindCursor("y").Type;
				Assert.IsNotNull(typey);
				Assert.AreNotEqual(TypeKind.Invalid, typey.Kind);

				Assert.AreNotEqual(typex, typey);
				Assert.AreNotEqual(typey, typex);
				Assert.IsTrue(typex != typey);
				Assert.IsTrue(typey != typex);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_Alignment()
		{
			string code = "int x; long y; long long z;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// x is 32-bit aligned
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(4, type.Alignment);

				// y is 32-bit aligned
				type = unit.FindCursor("y").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(4, type.Alignment);

				// z is 64-bit aligned
				type = unit.FindCursor("z").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(8, type.Alignment);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ArgumentTypes()
		{
			string code = "void main(int argc, char** argv);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ArgumentTypes, type.ArgumentTypes);
				Assert.AreEqual(2, type.ArgumentTypes.Count);
				Assert.AreSame(type.ArgumentTypes[0], type.ArgumentTypes[0]);
				Assert.AreEqual("int", type.ArgumentTypes[0].Spelling);
				Assert.AreEqual(TypeKind.Int, type.ArgumentTypes[0].Kind);
				Assert.AreSame(type.ArgumentTypes[1], type.ArgumentTypes[1]);
				Assert.AreEqual("char **", type.ArgumentTypes[1].Spelling);
				Assert.AreEqual(TypeKind.Pointer, type.ArgumentTypes[1].Kind);
				Assert.AreEqual("char *", type.ArgumentTypes[1].PointeeType.Spelling);
				Assert.AreEqual(TypeKind.Pointer, type.ArgumentTypes[1].PointeeType.Kind);
				Assert.AreEqual("char", type.ArgumentTypes[1].PointeeType.PointeeType.Spelling);
				Assert.AreEqual(TypeKind.Char_S, type.ArgumentTypes[1].PointeeType.PointeeType.Kind);

				// Hit the enumerators for code coverage
				foreach (Type t in type.ArgumentTypes) Assert.IsNotNull(t);
				foreach (Object obj in (System.Collections.IEnumerable)type.ArgumentTypes) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ArrayElementType()
		{
			// ArraySize only works with arrays, not vectors
			string code = "int x; int rg[3]; int __attribute__((vector_size(16))) vec;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// rg is an array of type int
				Type type = unit.FindCursor("rg").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ArrayElementType, type.ArrayElementType);
				Assert.AreEqual(TypeKind.Int, type.ArrayElementType.Kind);

				// vec is not an array type
				type = unit.FindCursor("vec").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ArrayElementType, type.ArrayElementType);
				Assert.AreEqual(TypeKind.Invalid, type.ArrayElementType.Kind);

				// x is not an array type
				type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ArrayElementType, type.ArrayElementType);
				Assert.AreEqual(TypeKind.Invalid, type.ArrayElementType.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ArraySize()
		{
			// ArraySize only works with arrays, not vectors
			string code = "int x; int rg[3]; int __attribute__((vector_size(16))) vec;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// rg has 3 elements
				Type type = unit.FindCursor("rg").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(3, type.ArraySize);

				// vec is not an array type
				type = unit.FindCursor("vec").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(null, type.ArraySize);

				// x is not an array type
				type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(null, type.ArraySize);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_CallingConvention()
		{
			// I'm going to cop out here, I cannot figure out what arguments clang needs in order
			// for constructs like __attribute__((stdcall)) to actually work, so assume that
			// the property itself is fine if it can differentiate between cdecl and thiscall

			string code = "class c { void memberfunc(void); }; void main(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// __attribute__((thiscall))
				Type type = unit.FindCursor("c").FindCursor("memberfunc").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);

				// On x86, this will be thiscall whereas on x64 this will be C
				if (IntPtr.Size == 4) Assert.AreEqual(CallingConvention.ThisCall, type.CallingConvention);
				else if (IntPtr.Size == 8) Assert.AreEqual(CallingConvention.C, type.CallingConvention);
				else Assert.Fail("Unsupported architecture");

				// C
				type = unit.FindCursor("main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(CallingConvention.C, type.CallingConvention);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_CanonicalType()
		{
			string code = "typedef int mytype; mytype x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mytype is not a canonical type
				Type typex = unit.FindCursor("x").Type;
				Assert.IsNotNull(typex);
				Assert.AreNotEqual(TypeKind.Invalid, typex.Kind);
				Assert.AreSame(typex.CanonicalType, typex.CanonicalType);
				Assert.AreNotEqual(typex, typex.CanonicalType);

				// int is the canonical type for mytype
				Assert.AreNotEqual(TypeKind.Invalid, typex.CanonicalType.Kind);
				Assert.AreEqual(TypeKind.Int, typex.CanonicalType.Kind);

				// y and x should have the same canonical type (int)
				Type typey = unit.FindCursor("y").Type;
				Assert.IsNotNull(typey);
				Assert.AreNotEqual(TypeKind.Invalid, typey.Kind);
				Assert.AreEqual(typex.CanonicalType, typey.CanonicalType);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ClassType()
		{
			string code = "struct s { int x; }; int s::* ptr = &s::x;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Type structtype = unit.FindCursor("s").Type;
				Assert.IsNotNull(structtype);
				Assert.AreNotEqual(TypeKind.Invalid, structtype.Kind);

				Type ptrtype = unit.FindCursor("ptr").Type;
				Assert.IsNotNull(ptrtype);
				Assert.AreNotEqual(TypeKind.Invalid, ptrtype.Kind);
				Assert.AreSame(ptrtype.ClassType, ptrtype.ClassType);

				// .ClassType should equal s
				Assert.AreEqual(structtype, ptrtype.ClassType);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_CxxReferenceQualifier()
		{
			string code = "class c { int none(); int& lvalue() &; int&& rvalue() &&; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Cursor cursor = unit.FindCursor("c");

				Type type = cursor.FindCursor("none").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(CxxReferenceQualifier.None, type.CxxReferenceQualifier);

				type = cursor.FindCursor("lvalue").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(CxxReferenceQualifier.LValue, type.CxxReferenceQualifier);

				type = cursor.FindCursor("rvalue").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(CxxReferenceQualifier.RValue, type.CxxReferenceQualifier);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_DeclarationCursor()
		{
			string code = "typedef int mytype; mytype x; int y;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mytype x has a declaration cursor
				Type typex = unit.FindCursor("x").Type;
				Assert.IsNotNull(typex);
				Assert.AreNotEqual(TypeKind.Invalid, typex.Kind);
				Assert.IsNotNull(typex.DeclarationCursor);
				Assert.AreSame(typex.DeclarationCursor, typex.DeclarationCursor);
				Assert.AreEqual(unit.FindCursor("mytype"), typex.DeclarationCursor);

				// int y does not have a declaration cursor, but will not provide
				// a null cursor, it provides a nodeclfound cursor
				Type typey = unit.FindCursor("y").Type;
				Assert.IsNotNull(typey);
				Assert.AreNotEqual(TypeKind.Invalid, typey.Kind);
				Assert.IsNotNull(typey.DeclarationCursor);
				Assert.AreEqual(CursorKind.NoDeclFound, typey.DeclarationCursor.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ElementCount()
		{
			// ElementCount works with both arrays and vector types
			string code = "int x; int rg[3]; int __attribute__((vector_size(16))) vec;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// rg has 3 elements
				Type type = unit.FindCursor("rg").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(3, type.ElementCount);

				// vec has 4 elements: (4 * sizeof(int))
				type = unit.FindCursor("vec").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(4, type.ElementCount);

				// x is not an array/vector type
				type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(null, type.ElementCount);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ElementType()
		{
			string code = "int x; int rg[3]; int __attribute__((vector_size(16))) vec;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// rg is an array of type int
				Type type = unit.FindCursor("rg").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ElementType, type.ElementType);
				Assert.AreEqual(TypeKind.Int, type.ElementType.Kind);

				// vec is a vector of type int
				type = unit.FindCursor("vec").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ElementType, type.ElementType);
				Assert.AreEqual(TypeKind.Int, type.ElementType.Kind);

				// x is not an array/vector type
				type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ElementType, type.ElementType);
				Assert.AreEqual(TypeKind.Invalid, type.ElementType.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_EnumerateFields()
		{
			string code = "struct mystruct { int x; long y; long long z; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mystruct
				Type type = unit.FindCursor("mystruct").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Record, type.Kind);

				List<Cursor> fields = new List<Cursor>();
				type.EnumerateFields(cursor =>
				{
					fields.Add(cursor);
					return EnumerateFieldsResult.Continue;
				});

				Assert.AreEqual(3, fields.Count);
				Assert.AreEqual(TypeKind.Int, fields[0].Type.Kind);
				Assert.AreEqual(TypeKind.Long, fields[1].Type.Kind);
				Assert.AreEqual(TypeKind.LongLong, fields[2].Type.Kind);

				// Make sure that exceptions thrown in the lambda are caught and exposed as intended
				try { type.EnumerateFields((c) => { throw new Exception("Bad thing"); }); }
				catch (Exception ex) { Assert.AreEqual("Bad thing", ex.Message); }
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_FieldBitOffset()
		{
			string code = "struct mystruct { int x; long y; long long z; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mystruct
				Cursor cursor = unit.FindCursor("mystruct");
				Type type = cursor.Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Record, type.Kind);

				// FieldBitOffset is provided via an indexer subclass
				Assert.AreSame(type.FieldBitOffset, type.FieldBitOffset);
				Assert.AreEqual(0, type.FieldBitOffset["x"]);
				Assert.AreEqual(32, type.FieldBitOffset["y"]);
				Assert.AreEqual(64, type.FieldBitOffset["z"]);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_GetHashCode()
		{
			string code = "int x;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreNotEqual(0, type.GetHashCode());
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_IsConstQualified()
		{
			string code = "int x; const int cx = 123;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(false, type.IsConstQualified);

				type = unit.FindCursor("cx").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(true, type.IsConstQualified);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_IsPOD()
		{
			string code = "struct A { int i; }; class B : public A {}; struct C : B { void fn(){} }; struct D : C { D(){} };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				// A is POD
				Type type = unit.FindCursor("A").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsPOD);

				// B is POD
				type = unit.FindCursor("B").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsPOD);

				// C is POD
				type = unit.FindCursor("C").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsPOD);

				// D is not POD
				type = unit.FindCursor("D").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsFalse(type.IsPOD);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_IsRestrictQualified()
		{
			string code = "int* p; int* restrict rp;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// rp is restrict-qualified
				Type type = unit.FindCursor("rp").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsRestrictQualified);

				// p is not restrict-qualified
				type = unit.FindCursor("p").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsFalse(type.IsRestrictQualified);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_IsVariadicFunction()
		{
			string code = "int main(void); int printf(const char* format, ...);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// printf() is variadic
				Type type = unit.FindCursor("printf").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsVariadicFunction);

				// main is not
				type = unit.FindCursor("main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsFalse(type.IsVariadicFunction);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_IsVolatileQualified()
		{
			string code = "int x; volatile int vx;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// vx is volatile
				Type type = unit.FindCursor("vx").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsTrue(type.IsVolatileQualified);

				// x is not
				type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.IsFalse(type.IsVolatileQualified);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_Kind()
		{
			string code = "int x; long long y; unsigned long* z; void main(int argc, char** argv);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Int, type.Kind);

				type = unit.FindCursor("y").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.LongLong, type.Kind);

				type = unit.FindCursor("z").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Pointer, type.Kind);
				Assert.IsNotNull(type.PointeeType);
				Assert.AreNotEqual(TypeKind.Invalid, type.PointeeType.Kind);
				Assert.AreEqual(TypeKind.ULong, type.PointeeType.Kind);

				type = unit.FindCursor("main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.FunctionProto, type.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_PointeeType()
		{
			string code = "int x; int* px; int** ppx;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.PointeeType, type.PointeeType);
				Assert.AreEqual(TypeKind.Invalid, type.PointeeType.Kind);

				type = unit.FindCursor("px").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.PointeeType, type.PointeeType);
				Assert.AreEqual(TypeKind.Int, type.PointeeType.Kind);

				type = unit.FindCursor("ppx").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.PointeeType, type.PointeeType);
				Assert.AreEqual(TypeKind.Pointer, type.PointeeType.Kind);
				Assert.AreEqual(TypeKind.Int, type.PointeeType.PointeeType.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ResultType()
		{
			string code = "int x; void void_main(void); int int_main(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ResultType, type.ResultType);
				Assert.AreEqual(TypeKind.Invalid, type.ResultType.Kind);

				type = unit.FindCursor("void_main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ResultType, type.ResultType);
				Assert.AreEqual(TypeKind.Void, type.ResultType.Kind);

				type = unit.FindCursor("int_main").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.ResultType, type.ResultType);
				Assert.AreEqual(TypeKind.Int, type.ResultType.Kind);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_Size()
		{
			string code = "char x; int y; long long z; void func(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(1, type.Size);

				type = unit.FindCursor("y").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(4, type.Size);

				type = unit.FindCursor("z").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(8, type.Size);

				type = unit.FindCursor("func").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(1, type.Size);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_Spelling()
		{
			string code = "char x; int y; long long z; void func(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.Spelling, type.Spelling);
				Assert.AreEqual("char", type.Spelling);

				type = unit.FindCursor("y").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.Spelling, type.Spelling);
				Assert.AreEqual("int", type.Spelling);

				type = unit.FindCursor("z").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.Spelling, type.Spelling);
				Assert.AreEqual("long long", type.Spelling);

				type = unit.FindCursor("func").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.Spelling, type.Spelling);
				Assert.AreEqual("void (void)", type.Spelling);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_TemplateArgumentTypes()
		{
			string code = "template<class _x, typename _y> class TemplateClass { void func(_x x, _y y); }; TemplateClass<int, long> instance;";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				Type type = unit.FindCursor("instance").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreSame(type.TemplateArgumentTypes, type.TemplateArgumentTypes);
				Assert.AreEqual(2, type.TemplateArgumentTypes.Count);
				Assert.AreEqual(TypeKind.Int, type.TemplateArgumentTypes[0].Kind);
				Assert.AreEqual(TypeKind.Long, type.TemplateArgumentTypes[1].Kind);

				// Exercise the enumerator
				foreach (Type t in type.TemplateArgumentTypes) Assert.IsNotNull(t);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void Type_ToString()
		{
			// ToString is equivalent to .Spelling, but without the check to ensure
			// that the same object reference is returned every time (it's a method)

			string code = "char x; int y; long long z; void func(void);";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				Type type = unit.FindCursor("x").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(type.Spelling, type.ToString());
				Assert.AreEqual("char", type.ToString());

				type = unit.FindCursor("y").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(type.Spelling, type.ToString());
				Assert.AreEqual("int", type.ToString());

				type = unit.FindCursor("z").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(type.Spelling, type.ToString());
				Assert.AreEqual("long long", type.ToString());

				type = unit.FindCursor("func").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(type.Spelling, type.ToString());
				Assert.AreEqual("void (void)", type.ToString());
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeKind_Equality()
		{
			TypeKind kind = TypeKind.Bool;
			Assert.IsTrue(kind == TypeKind.Bool);
			Assert.IsTrue(TypeKind.Bool == kind);
			Assert.AreEqual(kind, TypeKind.Bool);
			Assert.AreEqual(TypeKind.Bool, kind);
			Assert.IsTrue(kind.Equals(TypeKind.Bool));
			Assert.IsTrue(TypeKind.Bool.Equals(kind));
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeKind_Inequality()
		{
			TypeKind kind = TypeKind.Bool;
			Assert.IsTrue(kind != TypeKind.Pointer);
			Assert.IsTrue(TypeKind.Pointer != kind);
			Assert.AreNotEqual(kind, TypeKind.Pointer);
			Assert.AreNotEqual(TypeKind.Pointer, kind);
			Assert.IsFalse(kind.Equals(TypeKind.Pointer));
			Assert.IsFalse(TypeKind.Pointer.Equals(kind));
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeKind_GetHashCode()
		{
			// Just check a couple of these, there are a lot
			Assert.AreNotEqual(0, TypeKind.Bool.GetHashCode());
			Assert.AreNotEqual(0, TypeKind.Pointer.GetHashCode());
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeKind_ToString()
		{
			// Just check a couple of these, there are a lot
			Assert.IsFalse(String.IsNullOrEmpty(TypeKind.Bool.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(TypeKind.Pointer.ToString()));
		}

		[TestMethod(), TestCategory("Types")]
		public void EnumerateFieldsResult_Equality()
		{
			EnumerateFieldsResult result = EnumerateFieldsResult.Break;
			Assert.IsTrue(result == EnumerateFieldsResult.Break);
			Assert.IsTrue(EnumerateFieldsResult.Break == result);
			Assert.AreEqual(result, EnumerateFieldsResult.Break);
			Assert.AreEqual(EnumerateFieldsResult.Break, result);
			Assert.IsTrue(result.Equals(EnumerateFieldsResult.Break));
			Assert.IsTrue(EnumerateFieldsResult.Break.Equals(result));
		}

		[TestMethod(), TestCategory("Types")]
		public void EnumerateFieldsResult_Inequality()
		{
			EnumerateFieldsResult result = EnumerateFieldsResult.Break;
			Assert.IsTrue(result != EnumerateFieldsResult.Continue);
			Assert.IsTrue(EnumerateFieldsResult.Continue != result);
			Assert.AreNotEqual(result, EnumerateFieldsResult.Continue);
			Assert.AreNotEqual(EnumerateFieldsResult.Continue, result);
			Assert.IsFalse(result.Equals(EnumerateFieldsResult.Continue));
			Assert.IsFalse(EnumerateFieldsResult.Continue.Equals(result));
		}

		[TestMethod(), TestCategory("Types")]
		public void EnumerateFieldsResult_GetHashCode()
		{
			Assert.AreNotEqual(0, EnumerateFieldsResult.Continue.GetHashCode());

			// Break is zero, so the hash code will also be zero
			Assert.AreEqual(0, EnumerateFieldsResult.Break.GetHashCode());
		}

		[TestMethod(), TestCategory("Types")]
		public void EnumerateFieldsResult_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateFieldsResult.Break.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(EnumerateFieldsResult.Continue.ToString()));
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeExtensions_FindFields()
		{
			string code = "struct mystruct { int x; long y; long long z; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mystruct
				Type type = unit.FindCursor("mystruct").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Record, type.Kind);

				// make sure the extension method doesn't blow up
				var result = type.FindFields((c) => c.DisplayName == "x" || c.DisplayName == "y");
				Assert.IsNotNull(result);
				Assert.AreNotEqual(0, result.Count);
			}
		}

		[TestMethod(), TestCategory("Types")]
		public void TypeExtensions_GetFields()
		{
			string code = "struct mystruct { int x; long y; long long z; };";
			using (TranslationUnit unit = s_index.CreateTranslationUnitFromString(code))
			{
				// mystruct
				Type type = unit.FindCursor("mystruct").Type;
				Assert.IsNotNull(type);
				Assert.AreNotEqual(TypeKind.Invalid, type.Kind);
				Assert.AreEqual(TypeKind.Record, type.Kind);

				// make sure the extension method doesn't blow up
				var result = type.GetFields();
				Assert.IsNotNull(result);
				Assert.AreNotEqual(0, result.Count);
			}
		}
	}
}
