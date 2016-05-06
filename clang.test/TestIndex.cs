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
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using SysFile = System.IO.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestIndex
	{
		[TestMethod(), TestCategory("Indexes")]
		public void Index_Dispose()
		{
			Index index = Clang.CreateIndex();
			Assert.IsNotNull(index);

			// Verify that the index object is not disposed of
			Assert.IsFalse(index.IsDisposed(() => { var v = index.GlobalOptions; }));

			// Dispose of the index and verify it was disposed
			index.Dispose();
			Assert.IsTrue(index.IsDisposed(() => { var v = index.GlobalOptions; }));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void Index_CreateTranslationUnit()
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
			using (Index index = Clang.CreateIndex())
			{
				Assert.IsNotNull(index);
				using (TranslationUnit tu = index.CreateTranslationUnit(physical)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit(physical, arguments)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit("unsaved.c", unsaved)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit(physical, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit("unsaved.c", arguments, unsaved)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit(physical, arguments, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit("unsaved.c", unsaved, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
				using (TranslationUnit tu = index.CreateTranslationUnit("unsaved.c", arguments, unsaved, TranslationUnitParseOptions.DetailedPreprocessingRecord)) Assert.IsNotNull(tu);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void Index_CreateTranslationUnit_FullArgv()
		{
			using (Index index = Clang.CreateIndex())
			{
				// Make sure that the ArgumentsAreFullCommandLine option throws if no arguments are specified
				try { using (TranslationUnit tu = index.CreateTranslationUnit("hello.cpp", TranslationUnitParseOptions.ArgumentsAreFullCommandLine)) Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ArgumentNullException)); }

				// I'm not entirely sure what value this option really holds since it doesn't do what I assumed it
				// did, so for now just make sure it doesn't blow up in the caller's face or make anything worse
				string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
				string[] args = new string[] { Path.Combine(Path.GetDirectoryName(inpath), "clang") };
				using (TranslationUnit tu = index.CreateTranslationUnit(inpath, args, TranslationUnitParseOptions.ArgumentsAreFullCommandLine))
				{
					Assert.IsNotNull(tu);
					Cursor cursor = tu.FindCursor("main");
					Assert.IsNotNull(cursor);
					Assert.IsFalse(Cursor.IsNull(cursor));
				}
			}
		}

		[TestMethod(), TestCategory("Indexes")]
		public void Index_GlobalOptions()
		{
			using (Index index = Clang.CreateIndex())
			{
				Assert.IsNotNull(index);

				// The global options should be equal when accessed multiple times
				Assert.AreEqual(index.GlobalOptions, index.GlobalOptions);

				// Get the default options, should be None (0)
				IndexGlobalOptions options = index.GlobalOptions;
				Assert.AreEqual(IndexGlobalOptions.None, options);

				// Change the global options and ensure the new value persisted
				index.GlobalOptions = IndexGlobalOptions.ThreadBackgroundPriorityForIndexing;
				Assert.AreEqual(IndexGlobalOptions.ThreadBackgroundPriorityForIndexing, index.GlobalOptions);
			}
		}

		[TestMethod(), TestCategory("Indexes")]
		public void Index_IndexSourceFile()
		{
			// The indexer doesn't work with threads under the managed debugger
			Assert.IsTrue(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));

			// This is just exercising the overloaded methods, the indexer itself is tested in TestIndexer.cs
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (Index index = Clang.CreateIndex())
			{
				// Some dummy objects to exercise the overloads
				string[] args = new string[] { "-fmodules" };
				UnsavedFile[] unsavedfiles = new UnsavedFile[] { new UnsavedFile("virtual.h", "#define UNITTEST 1") };

				index.IndexSourceFile(inpath);
				index.IndexSourceFile(inpath, IndexOptions.None);
				index.IndexSourceFile(inpath, new Object());
				index.IndexSourceFile(inpath, args);
				index.IndexSourceFile(inpath, unsavedfiles);
				index.IndexSourceFile(inpath, IndexOptions.None, new Object());
				index.IndexSourceFile(inpath, args, IndexOptions.None);
				index.IndexSourceFile(inpath, args, new Object());
				index.IndexSourceFile(inpath, args, unsavedfiles);
				index.IndexSourceFile(inpath, unsavedfiles, IndexOptions.None);
				index.IndexSourceFile(inpath, unsavedfiles, new Object());
				index.IndexSourceFile(inpath, args, IndexOptions.None, new Object());
				index.IndexSourceFile(inpath, args, unsavedfiles, IndexOptions.None);
				index.IndexSourceFile(inpath, args, unsavedfiles, new Object());
				index.IndexSourceFile(inpath, unsavedfiles, IndexOptions.None, new Object());
				index.IndexSourceFile(inpath, args, unsavedfiles, IndexOptions.None, new Object());
			}
		}

		[TestMethod(), TestCategory("Indexes")]
		public void Index_IndexTranslationUnit()
		{
			// The indexer doesn't work with threads under the managed debugger
			Assert.IsTrue(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));

			// This is just exercising the overloaded methods, the indexer itself is tested in TestIndexer.cs
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			using (TranslationUnit transunit = Clang.CreateTranslationUnit(inpath, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				using (Index index = Clang.CreateIndex())
				{
					index.IndexTranslationUnit(transunit);
					index.IndexTranslationUnit(transunit, IndexOptions.None);
					index.IndexTranslationUnit(transunit, new Object());
					index.IndexTranslationUnit(transunit, IndexOptions.None, new object());
				}
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void Index_LoadTranslationUnit()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (Index index = Clang.CreateIndex())
			{
				Assert.IsNotNull(index);

				// Attempt to load a non-existent file
				try { index.LoadTranslationUnit("this_ast_file_does_not_exist"); Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ClangException)); }

				// Attempt to load an existing file that isn't a serialized translation unit
				try { index.LoadTranslationUnit(inpath); Assert.Fail(); }
				catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ClangException)); }

				// Get a temporary file name
				string outpath = Path.GetTempFileName();
				try
				{
					// Create a trivial translation unit form source and save it
					using (TranslationUnit tu = index.CreateTranslationUnit(inpath))
					{
						Assert.IsNotNull(tu);
						tu.Save(outpath);
					}

					// Attempt to load the translation unit from the serialized file
					using (TranslationUnit tu = index.LoadTranslationUnit(outpath)) Assert.IsNotNull(tu);
				}

				finally { SysFile.Delete(outpath); }
			}
		}

		[TestMethod(), TestCategory("Indexes")]
		public void Index_EventHandlers()
		{
			Index index = Clang.CreateIndex();
			Assert.IsNotNull(index);

			// Create a bunch of dummy handlers
			IndexAbortHandler onabort = new IndexAbortHandler((s, e) => { ((List<string>)e.State).Add("onabort"); });
			IndexDeclarationHandler ondeclaration = new IndexDeclarationHandler((s, e) => { ((List<string>)e.State).Add("ondeclaration"); });
			IndexDiagnosticsHandler ondiagnostics = new IndexDiagnosticsHandler((s, e) => { ((List<string>)e.State).Add("ondiagnostics"); });
			IndexEnteredMainFileHandler onenteredmainfile = new IndexEnteredMainFileHandler((s, e) => { ((List<string>)e.State).Add("onenteredmainfile"); });
			IndexEntityReferenceHandler onentityreference = new IndexEntityReferenceHandler((s, e) => { ((List<string>)e.State).Add("onentityreference"); });
			IndexImportedASTFileHandler onimportedastfile = new IndexImportedASTFileHandler((s, e) => { ((List<string>)e.State).Add("onimportedastfile"); });
			IndexIncludedFileHandler onincludedfile = new IndexIncludedFileHandler((s, e) => { ((List<string>)e.State).Add("onincludedfile"); });
			IndexStartedTranslationUnitHandler onstartedtranslationunit = new IndexStartedTranslationUnitHandler((s, e) => { ((List<string>)e.State).Add("onstartedtranslationunit"); });

			// Add and remove the handers to ensure that they are removed properly, the indexer unit
			// tests (TestIndexer.cs) will confirm that the handlers are working when added
			index.Abort += onabort;
			index.Abort -= onabort;
			index.Declaration += ondeclaration;
			index.Declaration -= ondeclaration;
			index.Diagnostics += ondiagnostics;
			index.Diagnostics -= ondiagnostics;
			index.EnteredMainFile += onenteredmainfile;
			index.EnteredMainFile -= onenteredmainfile;
			index.EntityReference += onentityreference;
			index.EntityReference -= onentityreference;
			index.ImportedASTFile += onimportedastfile;
			index.ImportedASTFile -= onimportedastfile;
			index.IncludedFile += onincludedfile;
			index.IncludedFile -= onincludedfile;
			index.StartedTranslationUnit += onstartedtranslationunit;
			index.StartedTranslationUnit -= onstartedtranslationunit;

			// Create a simple indexing session with a boolean state to ensure that none of the
			// handlers declared above are getting called after they have been removed
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			List<string> state = new List<string>();
			index.IndexSourceFile(inpath, (Object)state);
			Assert.AreEqual(0, state.Count);

			index.Dispose();
		}
	}
}
