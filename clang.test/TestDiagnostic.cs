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

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestDiagnostic
	{
		private static TranslationUnit s_tu;

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			string infile = Path.Combine(Environment.CurrentDirectory, @"input\diagnostics.cpp");
			s_tu = Clang.CreateTranslationUnit(infile);
			Assert.IsNotNull(s_tu);

			// .Diagnostics property should return the same object reference every time
			Assert.IsNotNull(s_tu.Diagnostics);
			Assert.AreSame(s_tu.Diagnostics, s_tu.Diagnostics);

			// There should be 6 top-level diagnostics - see diagnostics-expected.txt
			Assert.AreEqual(6, s_tu.Diagnostics.Count);
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			s_tu.Dispose();
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostics_Dispose()
		{
			//Assert.Fail("Unit test needs to be rewritten");

			// TranslationUnit diagnostics should be disposed of with the translation unit
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\diagnostics.cpp");
			TranslationUnit tu = Clang.CreateTranslationUnit(inpath);
			DiagnosticCollection diags = tu.Diagnostics;
			Diagnostic diag = diags[0];
			Assert.IsFalse(diag.IsDisposed(() => { string s = diag.Spelling; }));
			Assert.IsFalse(diags.IsDisposed(() => { int x = diags.Count; }));

			tu.Dispose();
			Assert.IsTrue(diag.IsDisposed(() => { Location l = diag.Location; }));
			Assert.IsTrue(diags.IsDisposed(() => { foreach(Diagnostic d in diags); }));

			// Serialized diagnostics should be disposed of with the parent DiagnosticCollection
			inpath = Path.Combine(Environment.CurrentDirectory, @"input\diagnostics.bin");
			var loadeddiags = Clang.LoadDiagnostics(inpath);
			Assert.IsNotNull(loadeddiags);
			diag = loadeddiags[0];
			Assert.IsFalse(diag.IsDisposed(() => { string s = diag.Spelling; }));
			Assert.IsFalse(diags.IsDisposed(() => { int x = diags.Count; }));

			loadeddiags.Dispose();
			Assert.IsTrue(diag.IsDisposed(() => { Location l = diag.Location; }));
			Assert.IsTrue(diags.IsDisposed(() => { foreach (Diagnostic d in diags) ; }));
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Category()
		{
			Assert.IsNotNull(s_tu.Diagnostics);

			// .Category returns a value type that has a reference type property
			// ensure that it returns the same instance of that each time
			DiagnosticCategory category = s_tu.Diagnostics[0].Category;
			Assert.AreSame(category.Text, category.Text);

			// Hit GetHashCode for coverage
			Assert.AreNotEqual(0, category.GetHashCode());

			// Check some equality/inequality for code coverage
			Assert.AreEqual(s_tu.Diagnostics[0].Category, s_tu.Diagnostics[0].Category);
			Assert.IsTrue(s_tu.Diagnostics[0].Category.Equals(s_tu.Diagnostics[0].Category));
			Assert.IsTrue(s_tu.Diagnostics[0].Category == s_tu.Diagnostics[0].Category);
			Assert.AreNotEqual(s_tu.Diagnostics[0].Category, s_tu.Diagnostics[4].Category);
			Assert.IsTrue(s_tu.Diagnostics[0].Category != s_tu.Diagnostics[4].Category);

			// Check GetHashCode for code coverage
			Assert.AreNotEqual(0, s_tu.Diagnostics[0].Category);
			Assert.AreNotEqual(0, s_tu.Diagnostics[4].Category);

			// [0] -> control reaches end of non-void function
			// = Semantic Issue
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.AreEqual("Semantic Issue", s_tu.Diagnostics[0].Category);    // implicit String^
			Assert.AreEqual("Semantic Issue", s_tu.Diagnostics[0].Category.ToString());
			Assert.AreEqual("Semantic Issue", s_tu.Diagnostics[0].Category.Text);
			Assert.AreEqual(2, s_tu.Diagnostics[0].Category.Number);

			// [4] -> use of GNU old-style field designator extension
			// = Parse Issue
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.AreEqual("Parse Issue", s_tu.Diagnostics[4].Category);       // implicit String^
			Assert.AreEqual("Parse Issue", s_tu.Diagnostics[4].Category.ToString());
			Assert.AreEqual("Parse Issue", s_tu.Diagnostics[4].Category.Text);
			Assert.AreEqual(4, s_tu.Diagnostics[4].Category.Number);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Children()
		{
			Assert.IsNotNull(s_tu);

			// There is only one file used for the translation unit, this has the name
			string infile = s_tu.Spelling;

			// .Children should return the same object reference each time it's accessed
			Assert.AreSame(s_tu.Diagnostics[0].Children, s_tu.Diagnostics[0].Children);
			
			// [1]
			Assert.IsNotNull(s_tu.Diagnostics[1].Children);
			Assert.AreEqual(1, s_tu.Diagnostics[1].Children.Count);

			// [1][0] -> diagnostics.cpp:1:5: note: previous definition is here
			Assert.IsNotNull(s_tu.Diagnostics[1].Children[0]);
			Assert.AreSame(s_tu.Diagnostics[1].Children[0], s_tu.Diagnostics[1].Children[0]);
			Assert.AreEqual(DiagnosticSeverity.Note, s_tu.Diagnostics[1].Children[0].Severity);
			Assert.AreEqual(infile, s_tu.Diagnostics[1].Children[0].Location.File.Name);
			Assert.AreEqual(1, s_tu.Diagnostics[1].Children[0].Location.Line);
			Assert.AreEqual(5, s_tu.Diagnostics[1].Children[0].Location.Column);
			Assert.AreEqual("previous definition is here", s_tu.Diagnostics[1].Children[0].Spelling);

			// [2]
			Assert.IsNotNull(s_tu.Diagnostics[2].Children);
			Assert.AreEqual(1, s_tu.Diagnostics[2].Children.Count);

			// [2][0] -> diagnostics.cpp:3:11: note: expanded from macro 'A'
			Assert.IsNotNull(s_tu.Diagnostics[2].Children[0]);
			Assert.AreSame(s_tu.Diagnostics[2].Children[0], s_tu.Diagnostics[2].Children[0]);
			Assert.AreEqual(DiagnosticSeverity.Note, s_tu.Diagnostics[2].Children[0].Severity);
			Assert.AreEqual(infile, s_tu.Diagnostics[2].Children[0].Location.File.Name);
			Assert.AreEqual(3, s_tu.Diagnostics[2].Children[0].Location.Line);
			Assert.AreEqual(11, s_tu.Diagnostics[2].Children[0].Location.Column);
			Assert.AreEqual("expanded from macro 'A'", s_tu.Diagnostics[2].Children[0].Spelling);

			// [3]
			Assert.IsNotNull(s_tu.Diagnostics[3].Children);
			Assert.AreEqual(2, s_tu.Diagnostics[3].Children.Count);

			// [3][0] -> diagnostics.cpp:4:7: note: previous definition is here
			Assert.IsNotNull(s_tu.Diagnostics[3].Children[0]);
			Assert.AreSame(s_tu.Diagnostics[3].Children[0], s_tu.Diagnostics[3].Children[0]);
			Assert.AreEqual(DiagnosticSeverity.Note, s_tu.Diagnostics[3].Children[0].Severity);
			Assert.AreEqual(infile, s_tu.Diagnostics[3].Children[0].Location.File.Name);
			Assert.AreEqual(4, s_tu.Diagnostics[3].Children[0].Location.Line);
			Assert.AreEqual(7, s_tu.Diagnostics[3].Children[0].Location.Column);
			Assert.AreEqual("previous definition is here", s_tu.Diagnostics[3].Children[0].Spelling);

			// [3][1] -> diagnostics.cpp:3:11: note: expanded from macro 'A'
			Assert.IsNotNull(s_tu.Diagnostics[3].Children[1]);
			Assert.AreSame(s_tu.Diagnostics[3].Children[1], s_tu.Diagnostics[3].Children[1]);
			Assert.AreEqual(DiagnosticSeverity.Note, s_tu.Diagnostics[3].Children[1].Severity);
			Assert.AreEqual(infile, s_tu.Diagnostics[3].Children[1].Location.File.Name);
			Assert.AreEqual(3, s_tu.Diagnostics[3].Children[1].Location.Line);
			Assert.AreEqual(11, s_tu.Diagnostics[3].Children[1].Location.Column);
			Assert.AreEqual("expanded from macro 'A'", s_tu.Diagnostics[3].Children[1].Spelling);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_DisableOption()
		{
			Assert.IsNotNull(s_tu.Diagnostics);

			// .DisableOption should return the same object reference each time
			Assert.AreSame(s_tu.Diagnostics[0].DisableOption, s_tu.Diagnostics[0].DisableOption);

			// [0] -> -Wno-return-type
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.AreEqual("-Wno-return-type", s_tu.Diagnostics[0].DisableOption);

			// [4] -> -Wno-gnu-designator
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.AreEqual("-Wno-gnu-designator", s_tu.Diagnostics[4].DisableOption);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Emphases()
		{
			Assert.IsNotNull(s_tu);

			// .Emphases should return the same object reference each time it's accessed
			Assert.AreSame(s_tu.Diagnostics[0].Emphases, s_tu.Diagnostics[0].Emphases);

			// [1] -> diagnostics.cpp (2, 1) - diagnostics.cpp (2, 5)
			Assert.IsNotNull(s_tu.Diagnostics[1].Emphases);
			Assert.AreEqual(1, s_tu.Diagnostics[1].Emphases.Count);
			Assert.AreEqual(2, s_tu.Diagnostics[1].Emphases[0].Start.Line);
			Assert.AreEqual(1, s_tu.Diagnostics[1].Emphases[0].Start.Column);
			Assert.AreEqual(2, s_tu.Diagnostics[1].Emphases[0].End.Line);
			Assert.AreEqual(5, s_tu.Diagnostics[1].Emphases[0].End.Column);

			// [2] -> diagnostics.cpp (4, 11) - diagnostics.cpp (4, 12)
			Assert.IsNotNull(s_tu.Diagnostics[2].Emphases);
			Assert.AreEqual(1, s_tu.Diagnostics[2].Emphases.Count);
			Assert.AreEqual(4, s_tu.Diagnostics[2].Emphases[0].Start.Line);
			Assert.AreEqual(11, s_tu.Diagnostics[2].Emphases[0].Start.Column);
			Assert.AreEqual(4, s_tu.Diagnostics[2].Emphases[0].End.Line);
			Assert.AreEqual(12, s_tu.Diagnostics[2].Emphases[0].End.Column);

			// [5] -> diagnostics.cpp (7, 20) - diagnostics.cpp (7, 27)
			Assert.IsNotNull(s_tu.Diagnostics[5].Emphases);
			Assert.AreEqual(1, s_tu.Diagnostics[5].Emphases.Count);
			Assert.AreEqual(7, s_tu.Diagnostics[5].Emphases[0].Start.Line);
			Assert.AreEqual(20, s_tu.Diagnostics[5].Emphases[0].Start.Column);
			Assert.AreEqual(7, s_tu.Diagnostics[5].Emphases[0].End.Line);
			Assert.AreEqual(27, s_tu.Diagnostics[5].Emphases[0].End.Column);

			// Hit the enumerators for code coverage
			foreach (Extent emphasis in s_tu.Diagnostics[1].Emphases) Assert.IsNotNull(emphasis);
			foreach (Object obj in (System.Collections.IEnumerable)s_tu.Diagnostics[1].Emphases) Assert.IsNotNull(obj);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_EnableOption()
		{
			Assert.IsNotNull(s_tu.Diagnostics);

			// .EnableOption should return the same object reference each time
			Assert.AreSame(s_tu.Diagnostics[0].EnableOption, s_tu.Diagnostics[0].EnableOption);

			// [0] -> -Wreturn-type
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.AreEqual("-Wreturn-type", s_tu.Diagnostics[0].EnableOption);

			// [4] -> -Wgnu-designator
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.AreEqual("-Wgnu-designator", s_tu.Diagnostics[4].EnableOption);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_FixIts()
		{
			Assert.IsNotNull(s_tu);

			// .FixIts should return the same object reference each time it's accessed
			Assert.AreSame(s_tu.Diagnostics[0].FixIts, s_tu.Diagnostics[0].FixIts);

			// [4]
			Assert.IsNotNull(s_tu.Diagnostics[4].FixIts);
			Assert.AreEqual(1, s_tu.Diagnostics[4].FixIts.Count);

			// [4][0] -> .f0 = 
			Assert.IsNotNull(s_tu.Diagnostics[4].FixIts[0]);
			Assert.AreSame(s_tu.Diagnostics[4].FixIts[0], s_tu.Diagnostics[4].FixIts[0]);
			Assert.AreSame(s_tu.Diagnostics[4].FixIts[0].Extent, s_tu.Diagnostics[4].FixIts[0].Extent);
			Assert.AreEqual(6, s_tu.Diagnostics[4].FixIts[0].Extent.Start.Line);
			Assert.AreEqual(7, s_tu.Diagnostics[4].FixIts[0].Extent.Start.Column);
			Assert.AreEqual(6, s_tu.Diagnostics[4].FixIts[0].Extent.End.Line);
			Assert.AreEqual(11, s_tu.Diagnostics[4].FixIts[0].Extent.End.Column);
			Assert.AreSame(s_tu.Diagnostics[4].FixIts[0].ReplacementText, s_tu.Diagnostics[4].FixIts[0].ReplacementText);
			Assert.AreEqual(".f0 = ", s_tu.Diagnostics[4].FixIts[0].ReplacementText);

			// Hit the enumerators for code coverage
			foreach (DiagnosticFixIt fixit in s_tu.Diagnostics[4].FixIts) Assert.IsNotNull(fixit);
			foreach (Object obj in (System.Collections.IEnumerable)s_tu.Diagnostics[4].FixIts) Assert.IsNotNull(obj);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Format()
		{
			Assert.IsNotNull(s_tu);
			
			// Not really concerned with what the API will return, just that something is returned
			foreach(Diagnostic diag in s_tu.Diagnostics)
			{
				Assert.IsNotNull(diag);

				Assert.IsFalse(String.IsNullOrEmpty(diag.Format()));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplayCategoryId)));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplayCategoryName)));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplayColumn)));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplayOption)));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplaySourceLocation)));
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplaySourceRanges)));

				// Kitchen sink ... all of them
				Assert.IsFalse(String.IsNullOrEmpty(diag.Format(DiagnosticDisplayOptions.DisplayCategoryId | DiagnosticDisplayOptions.DisplayCategoryName |
					DiagnosticDisplayOptions.DisplayColumn | DiagnosticDisplayOptions.DisplayOption | DiagnosticDisplayOptions.DisplaySourceLocation |
					DiagnosticDisplayOptions.DisplaySourceRanges)));
			}
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Location()
		{
			Assert.IsNotNull(s_tu);

			// There is only one file used for the translation unit, this has the name
			string infile = s_tu.Spelling;

			// .Location should return the same reference object each time
			Assert.AreSame(s_tu.Diagnostics[0].Location, s_tu.Diagnostics[0].Location);

			// [0] -> diagnostics.cpp:1:11
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.IsNotNull(s_tu.Diagnostics[0].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[0].Location));
			Assert.IsNotNull(s_tu.Diagnostics[0].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[0].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[0].Location.File.Name);
			Assert.AreEqual(1, s_tu.Diagnostics[0].Location.Line);
			Assert.AreEqual(11, s_tu.Diagnostics[0].Location.Column);

			// [1] -> diagnostics.cpp:2:6
			Assert.IsNotNull(s_tu.Diagnostics[1]);
			Assert.IsNotNull(s_tu.Diagnostics[1].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[1].Location));
			Assert.IsNotNull(s_tu.Diagnostics[1].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[1].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[1].Location.File.Name);
			Assert.AreEqual(2, s_tu.Diagnostics[1].Location.Line);
			Assert.AreEqual(6, s_tu.Diagnostics[1].Location.Column);

			// [2] -> diagnostics.cpp:4:7
			Assert.IsNotNull(s_tu.Diagnostics[2]);
			Assert.IsNotNull(s_tu.Diagnostics[2].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[2].Location));
			Assert.IsNotNull(s_tu.Diagnostics[2].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[2].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[2].Location.File.Name);
			Assert.AreEqual(4, s_tu.Diagnostics[2].Location.Line);
			Assert.AreEqual(7, s_tu.Diagnostics[2].Location.Column);

			// [3] -> diagnostics.cpp:6:1
			Assert.IsNotNull(s_tu.Diagnostics[3]);
			Assert.IsNotNull(s_tu.Diagnostics[3].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[3].Location));
			Assert.IsNotNull(s_tu.Diagnostics[0].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[3].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[3].Location.File.Name);
			Assert.AreEqual(6, s_tu.Diagnostics[3].Location.Line);
			Assert.AreEqual(1, s_tu.Diagnostics[3].Location.Column);

			// [4] -> diagnostics.cpp:6:7
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.IsNotNull(s_tu.Diagnostics[4].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[4].Location));
			Assert.IsNotNull(s_tu.Diagnostics[4].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[4].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[4].Location.File.Name);
			Assert.AreEqual(6, s_tu.Diagnostics[4].Location.Line);
			Assert.AreEqual(7, s_tu.Diagnostics[4].Location.Column);

			// [5] -> diagnostics.cpp:7:16
			Assert.IsNotNull(s_tu.Diagnostics[5]);
			Assert.IsNotNull(s_tu.Diagnostics[5].Location);
			Assert.IsFalse(Location.IsNull(s_tu.Diagnostics[5].Location));
			Assert.IsNotNull(s_tu.Diagnostics[5].Location.File);
			Assert.IsFalse(File.IsNull(s_tu.Diagnostics[5].Location.File));
			Assert.AreEqual(infile, s_tu.Diagnostics[5].Location.File.Name);
			Assert.AreEqual(7, s_tu.Diagnostics[5].Location.Line);
			Assert.AreEqual(16, s_tu.Diagnostics[5].Location.Column);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Severity()
		{
			Assert.IsNotNull(s_tu.Diagnostics);

			// [0] -> warning
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.AreEqual(DiagnosticSeverity.Warning, s_tu.Diagnostics[0].Severity);

			// [1] -> error
			Assert.IsNotNull(s_tu.Diagnostics[1]);
			Assert.AreEqual(DiagnosticSeverity.Error, s_tu.Diagnostics[1].Severity);

			// [2] -> error
			Assert.IsNotNull(s_tu.Diagnostics[2]);
			Assert.AreEqual(DiagnosticSeverity.Error, s_tu.Diagnostics[2].Severity);

			// [3] -> error
			Assert.IsNotNull(s_tu.Diagnostics[3]);
			Assert.AreEqual(DiagnosticSeverity.Error, s_tu.Diagnostics[3].Severity);

			// [4] -> warning
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.AreEqual(DiagnosticSeverity.Warning, s_tu.Diagnostics[4].Severity);

			// [5] -> error
			Assert.IsNotNull(s_tu.Diagnostics[5]);
			Assert.AreEqual(DiagnosticSeverity.Error, s_tu.Diagnostics[5].Severity);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_Spelling()
		{
			Assert.IsNotNull(s_tu.Diagnostics);

			// .Spelling property should return the same object reference each time
			Assert.AreSame(s_tu.Diagnostics[0].Spelling, s_tu.Diagnostics[0].Spelling);

			// [0] -> control reaches end of non-void function
			Assert.IsNotNull(s_tu.Diagnostics[0]);
			Assert.AreEqual("control reaches end of non-void function", s_tu.Diagnostics[0].Spelling);

			// [1] -> functions that differ only in their return type cannot be overloaded
			Assert.IsNotNull(s_tu.Diagnostics[1]);
			Assert.AreEqual("functions that differ only in their return type cannot be overloaded", s_tu.Diagnostics[1].Spelling);

			// [2] -> cannot initialize a variable of type 'void *' with an rvalue of type 'int'
			Assert.IsNotNull(s_tu.Diagnostics[2]);
			Assert.AreEqual("cannot initialize a variable of type 'void *' with an rvalue of type 'int'", s_tu.Diagnostics[2].Spelling);

			// [3] -> redefinition of 'x'
			Assert.IsNotNull(s_tu.Diagnostics[3]);
			Assert.AreEqual("redefinition of 'x'", s_tu.Diagnostics[3].Spelling);

			// [4] -> use of GNU old-style field designator extension
			Assert.IsNotNull(s_tu.Diagnostics[4]);
			Assert.AreEqual("use of GNU old-style field designator extension", s_tu.Diagnostics[4].Spelling);

			// [5] -> cannot initialize a variable of type 'int' with an rvalue of type 'const char *'
			Assert.IsNotNull(s_tu.Diagnostics[5]);
			Assert.AreEqual("cannot initialize a variable of type 'int' with an rvalue of type 'const char *'", s_tu.Diagnostics[5].Spelling);
		}

		[TestMethod(), TestCategory("Diagnostics")]
		public void Diagnostic_ToString()
		{
			Assert.IsNotNull(s_tu);

			// ToString() is a method so the resultant string doesn't have to the same
			// object reference each time, and the representation is arbitratry so just
			// make sure that it comes back with something for each diagnostic
			foreach (Diagnostic diag in s_tu.Diagnostics)
			{
				Assert.IsNotNull(diag);
				Assert.IsFalse(String.IsNullOrEmpty(diag.ToString()));
			}

			// Hit the generic enumerator for code coverage
			foreach (Object obj in (System.Collections.IEnumerable)s_tu.Diagnostics) Assert.IsNotNull(obj);
		}
	}
}
