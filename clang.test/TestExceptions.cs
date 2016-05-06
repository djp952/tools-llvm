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
using System.Runtime.Serialization.Formatters.Binary;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestExceptions
	{
		[TestMethod(), TestCategory("Exceptions"), TestCategory("Translation Units")]
		public void Exceptions_ClangException()
		{
			ClangException thrown = null;
			ClangException deserialized = null;

			// Generate a TranslationUnitLoadException
			try { Clang.LoadTranslationUnit("not_a_real_path"); }
			catch (ClangException ex) { thrown = ex; }

			Assert.IsNotNull(thrown);
			Assert.AreEqual(ClangErrorCode.Failure, thrown.ErrorCode);

			// Serialize and de-serialize the exception with a BinaryFormatter
			BinaryFormatter formatter = new BinaryFormatter();
			using (MemoryStream memstream = new MemoryStream())
			{
				formatter.Serialize(memstream, thrown);
				memstream.Seek(0, 0);
				deserialized = (ClangException)formatter.Deserialize(memstream);
			}

			// Check that the exceptions are equivalent
			Assert.AreEqual(thrown.ErrorCode, deserialized.ErrorCode);
			Assert.AreEqual(thrown.StackTrace, deserialized.StackTrace);
			Assert.AreEqual(thrown.ToString(), deserialized.ToString());
		}

		[TestMethod(), TestCategory("Exceptions"), TestCategory("Compilation Database")]
		public void Exceptions_CompilationDatabaseLoadException()
		{
			CompilationDatabaseLoadException thrown = null;
			CompilationDatabaseLoadException deserialized = null;

			// Generate a CompilationDatabaseLoadException
			try { Clang.CreateCompilationDatabase("not_a_real_path"); }
			catch (CompilationDatabaseLoadException ex) { thrown = ex; }

			Assert.IsNotNull(thrown);
			Assert.AreEqual(CompilationDatabaseLoadErrorCode.CannotLoad, thrown.ErrorCode);

            // Serialize and de-serialize the exception with a BinaryFormatter
            BinaryFormatter formatter = new BinaryFormatter();
            using (MemoryStream memstream = new MemoryStream())
            {
                formatter.Serialize(memstream, thrown);
                memstream.Seek(0, 0);
                deserialized = (CompilationDatabaseLoadException)formatter.Deserialize(memstream);
            }

			// Check that the exceptions are equivalent
			Assert.AreEqual(thrown.ErrorCode, deserialized.ErrorCode);
			Assert.AreEqual(thrown.StackTrace, deserialized.StackTrace);
			Assert.AreEqual(thrown.ToString(), deserialized.ToString());
		}

		[TestMethod(), TestCategory("Exceptions"), TestCategory("Diagnostics")]
		public void Exceptions_DiagnosticLoadException()
		{
			DiagnosticLoadException thrown = null;
			DiagnosticLoadException deserialized = null;

			// Generate a DiagnosticLoadException
			try { Clang.LoadDiagnostics("not_a_real_path"); }
			catch (DiagnosticLoadException ex) { thrown = ex; }

			Assert.IsNotNull(thrown);
			Assert.AreEqual(DiagnosticLoadErrorCode.CannotLoad, thrown.ErrorCode);

			// Serialize and de-serialize the exception with a BinaryFormatter
			BinaryFormatter formatter = new BinaryFormatter();
			using (MemoryStream memstream = new MemoryStream())
			{
				formatter.Serialize(memstream, thrown);
				memstream.Seek(0, 0);
				deserialized = (DiagnosticLoadException)formatter.Deserialize(memstream);
			}

			// Check that the exceptions are equivalent
			Assert.AreEqual(thrown.ErrorCode, deserialized.ErrorCode);
			Assert.AreEqual(thrown.StackTrace, deserialized.StackTrace);
			Assert.AreEqual(thrown.ToString(), deserialized.ToString());
		}

		[TestMethod(), TestCategory("Exceptions"), TestCategory("Translation Units")]
		public void Exceptions_TranslationUnitSaveException()
		{
			TranslationUnitSaveException thrown = null;
			TranslationUnitSaveException deserialized = null;

			// Generate a TranslationUnitSaveException
			try
			{ 
				string infile = Path.Combine(Environment.CurrentDirectory, @"input\hello.cpp");
				using (TranslationUnit tu = Clang.CreateTranslationUnit(infile))
				{
					tu.Save(@"Q:\not_a_directory\not_an_ast_file.bin");
				}
			}
			catch (TranslationUnitSaveException ex) { thrown = ex; }

			Assert.IsNotNull(thrown);
			Assert.AreEqual(TranslationUnitSaveErrorCode.Unknown, thrown.ErrorCode);

			// Serialize and de-serialize the exception with a BinaryFormatter
			BinaryFormatter formatter = new BinaryFormatter();
			using (MemoryStream memstream = new MemoryStream())
			{
				formatter.Serialize(memstream, thrown);
				memstream.Seek(0, 0);
				deserialized = (TranslationUnitSaveException)formatter.Deserialize(memstream);
			}

			// Check that the exceptions are equivalent
			Assert.AreEqual(thrown.ErrorCode, deserialized.ErrorCode);
			Assert.AreEqual(thrown.StackTrace, deserialized.StackTrace);
			Assert.AreEqual(thrown.ToString(), deserialized.ToString());
		}
	}
}
