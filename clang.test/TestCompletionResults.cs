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
using zuki.tools.llvm.clang.extensions;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestCompletionResults
	{
		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_Dispose()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			TranslationUnit tu = Clang.CreateTranslationUnit(inpath);
			Assert.IsNotNull(tu);

			CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11);
			Assert.IsNotNull(results);

			CompletionResult result = results[0];
			Assert.IsNotNull(results);

			// The CompletionResults keep the translation unit alive, it should not
			// be disposed of when the translation unit is disposed of
			tu.Dispose();
			Assert.IsFalse(result.IsDisposed(() => { var v = result.Kind; }));
			Assert.IsFalse(results.IsDisposed(() => { var v = results.Contexts; }));

			// Dispose of the Results collection, which should also cause Result to throw as disposed
			results.Dispose();
			Assert.IsTrue(results.IsDisposed(() => { var v = results.ContainerUnifiedSymbolResolution; }));
			Assert.IsTrue(result.IsDisposed(() => { var v = result.String; }));
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_Enumerator()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11))
				{
					Assert.IsNotNull(results);
					Assert.AreNotEqual(0, results.Count);
					foreach (CompletionResult result in results) Assert.IsNotNull(result);
					foreach (Object obj in (System.Collections.IEnumerable)results) Assert.IsNotNull(obj);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_ContainerUnifiedSymbolResolution()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11))
				{
					Assert.IsNotNull(results);
					Assert.AreSame(results.ContainerUnifiedSymbolResolution, results.ContainerUnifiedSymbolResolution);
					Assert.AreEqual("c:@S@Z", results.ContainerUnifiedSymbolResolution);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_Contexts()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11))
				{
					Assert.IsNotNull(results);
					Assert.AreEqual(CompletionContext.DotMemberAccess, results.Contexts);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_Diagnostics()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11))
				{
					Assert.IsNotNull(results);
					Assert.IsNotNull(results.Diagnostics);
					Assert.AreSame(results.Diagnostics, results.Diagnostics);
					Assert.AreNotEqual(0, results.Diagnostics.Count);
					Assert.IsNotNull(results.Diagnostics[0]);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_GetContainerCursorKind()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11))
				{
					Assert.AreEqual(CursorKind.StructDecl, results.GetContainerCursorKind());
					bool iscomplete = false;
					results.GetContainerCursorKind(out iscomplete);
					Assert.IsTrue(iscomplete);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_ObjectiveCSelector()
		{
			string code = "@interface Test3 -(void)setFoo:(int)x withBar:(int)y; @end void foo3(Test3 *test3) { [test3 setFoo: 2 withBar: 4]; }";
			UnsavedFile[] unsavedfiles = new UnsavedFile[] { new UnsavedFile("virtual.m", code) };

			// This doesn't seem to work if you don't use TranslationUnitParseOptions::Incomplete
			using (TranslationUnit unit = Clang.CreateTranslationUnit("virtual.m", unsavedfiles, TranslationUnitParseOptions.Incomplete))
			{
				// There are three places that return an ObjectiveCSelector, pick the first one
				using (CompletionResultCollection results = unit.CompleteAt("virtual.m", 1, 102, unsavedfiles))
				{
					Assert.IsNotNull(results);
					Assert.IsNotNull(results.ObjectiveCSelector);
					Assert.AreSame(results.ObjectiveCSelector, results.ObjectiveCSelector);
					Assert.AreEqual("setFoo:", results.ObjectiveCSelector);
				}
			}
		}

		[TestMethod(), TestCategory("Code Completion")]
		public void CompletionResultsCollection_Result()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.IncludeBriefCommentsInCodeCompletion))
			{
				Assert.IsNotNull(tu);

				// get_Z().???
				using (CompletionResultCollection results = tu.CompleteAt(inpath, 22, 11, CompletionOptions.SortAlphabetical | CompletionOptions.IncludeBriefComments))
				{
					Assert.IsNotNull(results);
					Assert.AreEqual(18, results.Count);
					Assert.IsNotNull(results[0]);
					Assert.AreSame(results[0], results[0]);
					Assert.AreEqual(CursorKind.FieldDecl, results[0].Kind);
					Assert.IsNotNull(results[0].String);
					Assert.AreSame(results[0].String, results[0].String);
					CompletionString str = results[0].String;
					Assert.IsNotNull(str.Annotations);
					Assert.AreSame(str.Annotations, str.Annotations);
					Assert.AreEqual(1, str.Annotations.Count);
					Assert.AreSame(str.Annotations[0], str.Annotations[0]);
					Assert.AreEqual("some annotation", str.Annotations[0]);
					Assert.AreEqual(Availability.Available, str.Availability);
					Assert.IsNotNull(str.BriefComment);
					Assert.AreSame(str.BriefComment, str.BriefComment);
					Assert.AreEqual("Bbb.", str.BriefComment);
					Assert.AreSame(str.Chunks, str.Chunks);
					Assert.AreEqual(2, str.Chunks.Count);
					Assert.IsNotNull(str.Chunks[0]);
					Assert.AreSame(str.Chunks[0], str.Chunks[0]);
					Assert.IsNotNull(str.Chunks[0].CompletionString);
					Assert.AreSame(str.Chunks[0].CompletionString, str.Chunks[0].CompletionString);
					Assert.AreEqual(CompletionChunkKind.ResultType, str.Chunks[0].Kind);
					Assert.IsNotNull(str.Chunks[0].Text);
					Assert.AreSame(str.Chunks[0].Text, str.Chunks[0].Text);
					Assert.AreEqual("double", str.Chunks[0].Text);
					Assert.AreEqual("double", str.Chunks[0].ToString());
					Assert.IsNotNull(str.Chunks[1]);
					Assert.AreSame(str.Chunks[1], str.Chunks[1]);
					Assert.IsNotNull(str.Chunks[1].CompletionString);
					Assert.AreSame(str.Chunks[1].CompletionString, str.Chunks[1].CompletionString);
					Assert.AreEqual(CompletionChunkKind.TypedText, str.Chunks[1].Kind);
					Assert.IsNotNull(str.Chunks[1].Text);
					Assert.AreSame(str.Chunks[1].Text, str.Chunks[1].Text);
					Assert.AreEqual("member", str.Chunks[1].Text);
					Assert.AreEqual("member", str.Chunks[1].ToString());
					Assert.AreSame(str.ParentContext, str.ParentContext);
					Assert.AreEqual("Z", str.ParentContext);
					Assert.AreNotEqual(0, str.Priority);

					// enumerate annotations for code coverage
					foreach (String annotation in str.Annotations) Assert.IsNotNull(annotation);
					foreach (Object obj in (System.Collections.IEnumerable)str.Annotations) Assert.IsNotNull(obj);

					// enumerate chunks for code coverage
					foreach (CompletionChunk chunk in str.Chunks) Assert.IsNotNull(chunk);
					foreach (Object obj in (System.Collections.IEnumerable)str.Chunks) Assert.IsNotNull(obj);
				}
			}
		}
	}
}
