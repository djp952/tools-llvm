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
using System.Runtime.Serialization;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestCompilationDatabase
	{
		/// <summary>
		/// Verfies a workaround that was put in place to prevent a potential loader lock problem.
		/// When invalid JSON is present in the compile_commands.json file, the internals of the
		/// compilation database cause Visual Studio to detect a loader deadlock, presumably due to
		/// the way the exception/error is being handled in clang/llvm in conjunction with this CLR 
		/// module.  To prevent that from happening, this library will attempt to load and parse the
		/// compile_commands.json file first, and throw a managed exception prior to calling libclang
		/// </summary>
		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabase_Workaround_JsonLoaderLock()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, @"input\badcompilecommands");

			// Test failure by trying to load the intentionally corrupted compile_commands.json
			try { using (CompilationDatabase cdb = Clang.CreateCompilationDatabase(inpath)) { Assert.Fail(); } }
			catch (Exception ex) 
			{
				// The exception thrown is expected to be of type SerializationException
				Assert.IsInstanceOfType(ex, typeof(SerializationException)); 
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabase_Dispose()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			CompilationDatabase database = Clang.CreateCompilationDatabase(inpath);
			Assert.IsNotNull(database);

			// Pull out the compile commands collection
			CompileCommandCollection commands = database.GetCompileCommands();
			Assert.IsNotNull(commands);

			// Dispose the compilation database instance
			database.Dispose();
			Assert.IsTrue(database.IsDisposed(() => { var v = database.GetCompileCommands(inpath); }));

			// The lifetime of a CompileCommandCollection should not depend on the database
			Assert.IsFalse(commands.IsDisposed(() => { var v = commands.Count; }));

			// Dispose the compile commands collection
			commands.Dispose();
			Assert.IsTrue(commands.IsDisposed(() => { var v = commands.Count; }));
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabase_GetCompileCommands()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			using (CompilationDatabase cdb = Clang.CreateCompilationDatabase(inpath))
			{
				Assert.IsNotNull(cdb);

				// Attempting to look up a non-existent file name should yield no commands
				CompileCommandCollection nonexistent = cdb.GetCompileCommands("this_file_does_not_exist");
				Assert.AreEqual(0, nonexistent.Count);

				// Attempting to look up an actual file name should yield command(s)
				CompileCommandCollection existent = cdb.GetCompileCommands(@"C:\home\john.doe\MyProject\project.cpp");
				Assert.AreNotEqual(0, existent.Count);
				Assert.AreSame(existent[0], existent[0]);

				// Attempting to look up all the files should yield three commands
				CompileCommandCollection allcommands = cdb.GetCompileCommands();
				Assert.AreEqual(3, allcommands.Count);
				Assert.AreSame(allcommands[0], allcommands[0]);
				Assert.AreSame(allcommands[1], allcommands[1]);
				Assert.AreSame(allcommands[2], allcommands[2]);
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompileCommand_Arguments()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			using (CompileCommandCollection commands = Clang.CreateCompilationDatabase(inpath).GetCompileCommands())
			{
				Assert.IsNotNull(commands);
				Assert.AreEqual(3, commands.Count);

				// There should be five arguments in the first compile command
				Assert.IsNotNull(commands[0].Arguments);
				Assert.AreEqual(5, commands[0].Arguments.Count);
				Assert.AreSame(commands[0].Arguments, commands[0].Arguments);

				// Just check the first command's arguments
				Assert.AreSame(commands[0].Arguments[0], commands[0].Arguments[0]);
				Assert.AreEqual("clang++", commands[0].Arguments[0]);
				Assert.AreSame(commands[0].Arguments[1], commands[0].Arguments[1]);
				Assert.AreEqual("-o", commands[0].Arguments[1]);
				Assert.AreSame(commands[0].Arguments[2], commands[0].Arguments[2]);
				Assert.AreEqual("project.o", commands[0].Arguments[2]);
				Assert.AreSame(commands[0].Arguments[3], commands[0].Arguments[3]);
				Assert.AreEqual("-c", commands[0].Arguments[3]);
				Assert.AreSame(commands[0].Arguments[4], commands[0].Arguments[4]);
				Assert.AreEqual(@"C:\home\john.doe\MyProject\project.cpp", commands[0].Arguments[4]);

				// Hit the enumerators for code coverage
				foreach (CompileCommand command in commands) Assert.IsNotNull(command);
				foreach (Object obj in (System.Collections.IEnumerable)commands) Assert.IsNotNull(obj);
				foreach (String argument in commands[0].Arguments) Assert.IsNotNull(argument);
				foreach (Object obj in (System.Collections.IEnumerable)commands[0].Arguments) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompileCommand_Filename()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			using (CompileCommandCollection commands = Clang.CreateCompilationDatabase(inpath).GetCompileCommands())
			{
				Assert.IsNotNull(commands);
				Assert.AreEqual(3, commands.Count);

				Assert.IsNotNull(commands[0].Filename);
				Assert.AreSame(commands[0].Filename, commands[0].Filename);
				Assert.AreEqual(@"C:\home\john.doe\MyProject\project.cpp", commands[0].Filename);

				Assert.IsNotNull(commands[1].Filename);
				Assert.AreSame(commands[1].Filename, commands[1].Filename);
				Assert.AreEqual(@"C:\home\john.doe\MyProject\project2.cpp", commands[1].Filename);

				Assert.IsNotNull(commands[2].Filename);
				Assert.AreSame(commands[2].Filename, commands[2].Filename);
				Assert.AreEqual(@"C:\home\john.doe\MyProject\project2.cpp", commands[2].Filename);
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompileCommand_SourceMappings()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			using (CompileCommandCollection commands = Clang.CreateCompilationDatabase(inpath).GetCompileCommands())
			{
				Assert.IsNotNull(commands);
				Assert.AreEqual(3, commands.Count);
				
				// Source mappings don't exist unless a custom compilation database is implemented, and I
				// don't see any way to specify one at runtime using this API, so ... there are none. ever.
				Assert.IsNotNull(commands[0].SourceMappings);
				Assert.AreEqual(0, commands[0].SourceMappings.Count);

				Assert.IsNotNull(commands[1].SourceMappings);
				Assert.AreEqual(0, commands[1].SourceMappings.Count);
			
				Assert.IsNotNull(commands[2].SourceMappings);
				Assert.AreEqual(0, commands[2].SourceMappings.Count);

				// Hit the enumerators for code coverage
				foreach (CompileCommandSourceMapping mapping in commands[0].SourceMappings) Assert.IsNotNull(mapping);
				foreach (Object obj in (System.Collections.IEnumerable)commands[0].SourceMappings) Assert.IsNotNull(obj);
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompileCommand_WorkingDirectory()
		{
			string inpath = Path.Combine(Environment.CurrentDirectory, "input");
			using (CompileCommandCollection commands = Clang.CreateCompilationDatabase(inpath).GetCompileCommands())
			{
				Assert.IsNotNull(commands);
				Assert.AreEqual(3, commands.Count);

				Assert.IsNotNull(commands[0].WorkingDirectory);
				Assert.AreSame(commands[0].WorkingDirectory, commands[0].WorkingDirectory);
				Assert.AreEqual(@"C:\home\john.doe\MyProject", commands[0].WorkingDirectory);

				Assert.IsNotNull(commands[1].WorkingDirectory);
				Assert.AreSame(commands[1].WorkingDirectory, commands[1].WorkingDirectory);
				Assert.AreEqual(@"C:\home\john.doe\MyProjectA", commands[1].WorkingDirectory);

				Assert.IsNotNull(commands[2].WorkingDirectory);
				Assert.AreSame(commands[2].WorkingDirectory, commands[2].WorkingDirectory);
				Assert.AreEqual(@"C:\home\john.doe\MyProjectB", commands[2].WorkingDirectory);
			}
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabaseLoadErrorCode_Equality()
		{
			CompilationDatabaseLoadErrorCode errorcode = CompilationDatabaseLoadErrorCode.CannotLoad;
			Assert.IsTrue(errorcode == CompilationDatabaseLoadErrorCode.CannotLoad);
			Assert.IsTrue(CompilationDatabaseLoadErrorCode.CannotLoad == errorcode);
			Assert.AreEqual(errorcode, CompilationDatabaseLoadErrorCode.CannotLoad);
			Assert.AreEqual(CompilationDatabaseLoadErrorCode.CannotLoad, errorcode);
			Assert.IsTrue(errorcode.Equals(CompilationDatabaseLoadErrorCode.CannotLoad));
			Assert.IsTrue(CompilationDatabaseLoadErrorCode.CannotLoad.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabaseLoadErrorCode_Inequality()
		{
			CompilationDatabaseLoadErrorCode errorcode = CompilationDatabaseLoadErrorCode.CannotLoad;
			Assert.IsTrue(errorcode != CompilationDatabaseLoadErrorCode.None);
			Assert.IsTrue(CompilationDatabaseLoadErrorCode.None != errorcode);
			Assert.AreNotEqual(errorcode, CompilationDatabaseLoadErrorCode.None);
			Assert.AreNotEqual(CompilationDatabaseLoadErrorCode.None, errorcode);
			Assert.IsFalse(errorcode.Equals(CompilationDatabaseLoadErrorCode.None));
			Assert.IsFalse(CompilationDatabaseLoadErrorCode.None.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabaseLoadErrorCode_GetHashCode()
		{
			Assert.AreNotEqual(0, CompilationDatabaseLoadErrorCode.CannotLoad.GetHashCode());

			// None is zero, so the hash code will also be zero
			Assert.AreEqual(0, CompilationDatabaseLoadErrorCode.None.GetHashCode());
		}

		[TestMethod(), TestCategory("Compilation Database")]
		public void CompilationDatabaseLoadErrorCode_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(CompilationDatabaseLoadErrorCode.None.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(CompilationDatabaseLoadErrorCode.CannotLoad.ToString()));
		}
	}
}
