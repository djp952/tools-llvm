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
using ClangFile = zuki.tools.llvm.clang.File;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestTranslationUnit
	{
		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_Dispose()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			// Create a basic index object
			Index index = Clang.CreateIndex();
			Assert.IsNotNull(index);

			// Create a trivial translation unit within the index
			TranslationUnit unit = index.CreateTranslationUnit(inpath);
			Assert.IsNotNull(unit);

			// Dispose of the index instance
			index.Dispose();

			// A translation unit keeps the index alive internally, so even
			// after the index has been disposed, it should still work
			Assert.IsFalse(ClangFile.IsNull(unit.GetFile(@"input\hello.cpp")));

			// Dispose of the translation unit instance and check for the exception
			unit.Dispose();
			try { unit.GetFile(@"input\hello.cpp"); Assert.Fail(); }
			catch (Exception ex) { Assert.IsInstanceOfType(ex, typeof(ObjectDisposedException)); }
		}


		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_CompleteAt()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			// unsaved files
			List<UnsavedFile> unsaved = new List<UnsavedFile>();
			unsaved.Add(new UnsavedFile("unsaved.c", "int myfunc(void) { return 0; }"));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// Just briefly exercise the creation methods, Completion has it's own unit tests
				using (CompletionResultCollection results = tu.CompleteAt("hello.cpp", 1, 1)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt("hello.cpp", 1, 1, unsaved)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt("hello.cpp", 1, 1, CompletionOptions.IncludeCodePatterns)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt("hello.cpp", 1, 1, unsaved, CompletionOptions.IncludeCodePatterns)) Assert.IsNotNull(results);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_Cursor()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// Property should return the same reference object each time
				Assert.AreSame(tu.Cursor, tu.Cursor);

				// This cursor should be a TranslationUnit cursor
				Assert.IsNotNull(tu.Cursor);
				Assert.IsFalse(Cursor.IsNull(tu.Cursor));
				Assert.AreEqual(CursorKind.TranslationUnit, tu.Cursor.Kind);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_Diagnostics()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				// Diagnostics have their own unit test category, just ensure they are there

				Assert.IsNotNull(tu);
				Assert.AreSame(tu.Diagnostics, tu.Diagnostics);
				Assert.IsNotNull(tu.Diagnostics);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_EnumerateIncludedFiles()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			List<Tuple<ClangFile, LocationCollection>> includedfiles = new List<Tuple<ClangFile, LocationCollection>>();
			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);
				tu.EnumerateIncludedFiles((file, locations) => includedfiles.Add(Tuple.Create(file, locations)));

				// There should be five included files: include.cpp, header1.h, header3.h, header2.h, header3.h
				Assert.AreEqual(5, includedfiles.Count);
				Assert.AreEqual("include.cpp", Path.GetFileName(includedfiles[0].Item1.Name));
				Assert.AreEqual("header1.h", Path.GetFileName(includedfiles[1].Item1.Name));
				Assert.AreEqual("header3.h", Path.GetFileName(includedfiles[2].Item1.Name));
				Assert.AreEqual("header2.h", Path.GetFileName(includedfiles[3].Item1.Name));
				Assert.AreEqual("header3.h", Path.GetFileName(includedfiles[4].Item1.Name));

				foreach (Tuple<ClangFile, LocationCollection> t in includedfiles)
				{
					foreach (Location l in t.Item2)
					{
						Assert.IsNotNull(l);
						Assert.IsFalse(Location.IsNull(l));
					}

					// Generic enumerator for code coverage
					foreach (Object obj in (System.Collections.IEnumerable)t.Item2) Assert.IsNotNull(obj);
				}

				// Make sure that exceptions thrown in the lambda are caught and exposed as intended
				try { tu.EnumerateIncludedFiles((f, l) => { throw new Exception("Bad thing"); }); }
				catch (Exception ex) { Assert.AreEqual("Bad thing", ex.Message); }
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_GetFile()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// Get the file using the short name
				File shortfile = tu.GetFile(@"input\hello.cpp");
				Assert.IsNotNull(shortfile);
				Assert.IsFalse(File.IsNull(shortfile));
				Assert.AreEqual(@"input\hello.cpp", shortfile.Name);

				// Get the file using the long name
				File longfile = tu.GetFile(inpath);
				Assert.IsNotNull(longfile);
				Assert.IsFalse(File.IsNull(longfile));
				Assert.AreEqual(@"input\hello.cpp", longfile.Name);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_ResourceUsage()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));
			
			using (Index index = Clang.CreateIndex())
			{
				Assert.IsNotNull(index);
				using (TranslationUnit unit = index.CreateTranslationUnit(inpath))
				{
					Assert.IsNotNull(unit.ResourceUsage);
					Assert.AreSame(unit.ResourceUsage, unit.ResourceUsage);

					// There should be some level of resource usage
					Assert.AreNotEqual(0, unit.ResourceUsage.Count);
					Assert.AreNotEqual(0, unit.ResourceUsage[ResourceUsageKind.Preprocessor]);
					Assert.AreNotEqual(0, unit.ResourceUsage.TotalBytes);

					// Ensure that all the string lookups succeed for the keys
					foreach (var usage in unit.ResourceUsage) Assert.IsFalse(String.IsNullOrEmpty(usage.Key.ToString()));

					// Exercise the ResourceUsageDictionary for code coverage
					Assert.IsTrue(unit.ResourceUsage.ContainsKey(ResourceUsageKind.Preprocessor));
					foreach (Object obj in (System.Collections.IEnumerable)unit.ResourceUsage) Assert.IsNotNull(obj);
					foreach (ResourceUsageKind kind in unit.ResourceUsage.Keys) Assert.IsNotNull(kind);
					foreach (uint resval in unit.ResourceUsage.Values) Assert.AreNotEqual(0, resval);

					uint value = 0;
					unit.ResourceUsage.TryGetValue(ResourceUsageKind.Preprocessor, out value);
					Assert.AreNotEqual(0, value);
				}
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_Save()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			// Get a temporary file name for the serialized translation unit
			string outpath = Path.GetTempFileName();
			try
			{
				// Create a trivial translation unit form source and save it
				using (TranslationUnit unit = Clang.CreateTranslationUnit(inpath)) unit.Save(outpath);
				Assert.IsTrue(SysFile.Exists(outpath));
				Assert.IsTrue(new FileInfo(outpath).Length > 0);
			}

			finally { SysFile.Delete(outpath); }
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_Spelling()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				// The spelling of the translation unit should match the input file
				Assert.IsNotNull(tu);
				Assert.AreSame(tu.Spelling, tu.Spelling);
				Assert.IsNotNull(tu.Spelling);
				Assert.AreEqual(inpath, tu.Spelling);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnit_ToString()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				// ToString() should equal the spelling string, but is not required
				// to return the same string object reference
				Assert.IsNotNull(tu);
				Assert.AreEqual(tu.Spelling, tu.ToString());
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitSaveErrorCode_Equality()
		{
			TranslationUnitSaveErrorCode errorcode = TranslationUnitSaveErrorCode.InvalidTranslationUnit;
			Assert.IsTrue(errorcode == TranslationUnitSaveErrorCode.InvalidTranslationUnit);
			Assert.IsTrue(TranslationUnitSaveErrorCode.InvalidTranslationUnit == errorcode);
			Assert.AreEqual(errorcode, TranslationUnitSaveErrorCode.InvalidTranslationUnit);
			Assert.AreEqual(TranslationUnitSaveErrorCode.InvalidTranslationUnit, errorcode);
			Assert.IsTrue(errorcode.Equals(TranslationUnitSaveErrorCode.InvalidTranslationUnit));
			Assert.IsTrue(TranslationUnitSaveErrorCode.InvalidTranslationUnit.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitSaveErrorCode_Inequality()
		{
			TranslationUnitSaveErrorCode errorcode = TranslationUnitSaveErrorCode.InvalidTranslationUnit;
			Assert.IsTrue(errorcode != TranslationUnitSaveErrorCode.Unknown);
			Assert.IsTrue(TranslationUnitSaveErrorCode.Unknown != errorcode);
			Assert.AreNotEqual(errorcode, TranslationUnitSaveErrorCode.Unknown);
			Assert.AreNotEqual(TranslationUnitSaveErrorCode.Unknown, errorcode);
			Assert.IsFalse(errorcode.Equals(TranslationUnitSaveErrorCode.Unknown));
			Assert.IsFalse(TranslationUnitSaveErrorCode.Unknown.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitSaveErrorCode_GetHashCode()
		{
			Assert.AreNotEqual(0, TranslationUnitSaveErrorCode.InvalidTranslationUnit.GetHashCode());
			Assert.AreNotEqual(0, TranslationUnitSaveErrorCode.TranslationErrors.GetHashCode());
			Assert.AreNotEqual(0, TranslationUnitSaveErrorCode.Unknown.GetHashCode());

			// None is zero, so the hash code will also be zero
			Assert.AreEqual(0, TranslationUnitSaveErrorCode.None.GetHashCode());
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitSaveErrorCode_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(TranslationUnitSaveErrorCode.InvalidTranslationUnit.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(TranslationUnitSaveErrorCode.None.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(TranslationUnitSaveErrorCode.TranslationErrors.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(TranslationUnitSaveErrorCode.Unknown.ToString()));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void ResourceUsageKind_Equality()
		{
			ResourceUsageKind kind = ResourceUsageKind.ASTContext;
			Assert.IsTrue(kind == ResourceUsageKind.ASTContext);
			Assert.IsTrue(ResourceUsageKind.ASTContext == kind);
			Assert.AreEqual(kind, ResourceUsageKind.ASTContext);
			Assert.AreEqual(ResourceUsageKind.ASTContext, kind);
			Assert.IsTrue(kind.Equals(ResourceUsageKind.ASTContext));
			Assert.IsTrue(ResourceUsageKind.ASTContext.Equals(kind));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void ResourceUsageKind_Inequality()
		{
			ResourceUsageKind kind = ResourceUsageKind.ASTContext;
			Assert.IsTrue(kind != ResourceUsageKind.SourceManager);
			Assert.IsTrue(ResourceUsageKind.SourceManager != kind);
			Assert.AreNotEqual(kind, ResourceUsageKind.SourceManager);
			Assert.AreNotEqual(ResourceUsageKind.SourceManager, kind);
			Assert.IsFalse(kind.Equals(ResourceUsageKind.SourceManager));
			Assert.IsFalse(ResourceUsageKind.SourceManager.Equals(kind));
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void ResourceUsageKind_GetHashCode()
		{
			// There are a lot of these, just check a couple
			Assert.AreNotEqual(0, ResourceUsageKind.ASTContext.GetHashCode());
			Assert.AreNotEqual(0, ResourceUsageKind.SourceManager.GetHashCode());
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void ResourceUsageKind_ToString()
		{
			// There are a lot of these, just check a couple
			Assert.IsFalse(String.IsNullOrEmpty(ResourceUsageKind.ASTContext.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(ResourceUsageKind.SourceManager.ToString()));
		}

		//
		// EXTENSIONS
		//

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_CompleteAt()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\completion.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				// Grab a location instance to use with the extension methods
				Location location = tu.GetLocation(inpath, 22, 11);

				// Create a dummy virtual/unsaved file to pass into the extension methods
				UnsavedFile[] unsavedfiles = new UnsavedFile[] { new UnsavedFile("virtual.h", "#define UNITTEST 1") };

				// Hit all the extension methods and make sure they don't throw
				using (CompletionResultCollection results = tu.CompleteAt(location)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location, unsavedfiles)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location, CompletionOptions.IncludeMacros)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location, unsavedfiles, CompletionOptions.IncludeMacros)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location.File, 22, 11)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location.File, 22, 11, unsavedfiles)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location.File, 22, 11, CompletionOptions.IncludeMacros)) Assert.IsNotNull(results);
				using (CompletionResultCollection results = tu.CompleteAt(location.File, 22, 11, unsavedfiles, CompletionOptions.IncludeMacros)) Assert.IsNotNull(results);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_FindIncludedFiles()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				var results = tu.FindIncludedFiles((f, l) => { return f.Name.Contains("header1"); });
				Assert.IsNotNull(results);
				Assert.AreNotEqual(0, results.Count);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_GetExtent()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				Extent extent = tu.GetExtent(inpath, 1, 10);
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreEqual(1, extent.Start.Offset);
				Assert.AreEqual(10, extent.End.Offset);

				extent = tu.GetExtent(inpath, 5, 1, 5, 10);
				Assert.IsNotNull(extent);
				Assert.IsFalse(Extent.IsNull(extent));
				Assert.AreEqual(5, extent.Start.Line);
				Assert.AreEqual(1, extent.Start.Column);
				Assert.AreEqual(5, extent.End.Line);
				Assert.AreEqual(10, extent.End.Column);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_GetIncludedFiles()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				var files = tu.GetIncludedFiles();
				Assert.IsNotNull(files);
				Assert.AreNotEqual(0, files.Count);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_GetLocation()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				Location location = tu.GetLocation(inpath, 10);
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(10, location.Offset);

				location = tu.GetLocation(inpath, 5, 6);
				Assert.IsNotNull(location);
				Assert.IsFalse(Location.IsNull(location));
				Assert.AreEqual(5, location.Line);
				Assert.AreEqual(6, location.Column);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_GetTokens()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				var tokens = tu.GetTokens();
				Assert.IsNotNull(tokens);
				Assert.AreNotEqual(0, tokens.Count);
			}
		}

		[TestMethod(), TestCategory("Translation Units")]
		public void TranslationUnitExtensions_IsFileMultipleGuardIncluded()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\include.cpp");
			Assert.IsTrue(SysFile.Exists(inpath));

			using (TranslationUnit tu = Clang.CreateTranslationUnit(inpath))
			{
				Assert.IsNotNull(tu);

				Assert.IsTrue(tu.IsFileMultipleIncludeGuarded(Path.Combine(Environment.CurrentDirectory, @"input\header1.h")));
				Assert.IsTrue(tu.IsFileMultipleIncludeGuarded(Path.Combine(Environment.CurrentDirectory, @"input\header2.h")));
				Assert.IsFalse(tu.IsFileMultipleIncludeGuarded(Path.Combine(Environment.CurrentDirectory, @"input\header3.h")));
			}
		}
	}
}
