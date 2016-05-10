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
using System.Diagnostics;
using zuki.tools.llvm.clang;

namespace zuki.tools.llvm.clang.Samples
{
	/// <summary>
	/// Illustrates basic use of the clang event driven indexer as exposed by the
	/// zuki.tools.llvm.clang package.  The indexer allows the caller to process a 
	/// source file or an existing translation unit by handling events on the Index object.
	/// 
	/// NOTE: As of libclang v3.8, there is an incompatibility with the threading model of
	/// the indexer and the Visual Studio managed debugger.  To work around this incompatibility 
	/// set a system-wide environment variable called LIBCLANG_NOTHREADS and close/reopen Visual 
	/// Studio.  This environment variable must be set before libclang.dll is loaded.
	/// 
	/// NOTE: As of libclang v3.8, there is an outstanding bug that can cause heap corruption
	/// during an indexer session (https://llvm.org/bugs/show_bug.cgi?id=11864).  The version
	/// of libclang.dll provided with the zuki.tools.llvm.clang package has this bug patched
	/// but may not be when using the official libclang release or another fork.
	/// </summary>
	public static class EventDrivenIndexer
	{
		/// <summary>
		/// State object to pass into the indexer.  The state object, if specified, will
		/// be passed into every indexer event handler as a property on the event arguments.
		/// Note that an Exception field is specified here; exceptions thrown from an indexer
		/// event handler will be intentionally eaten by the zuki.tools.llvm.clang library
		/// and not propagated back to the caller.  My suggestion is to use the state object
		/// to track if an exception has occurred and stop the indexing the next time that
		/// the Abort event handler is invoked.
		/// </summary>
		private class IndexerState
		{
			public List<string> TraceLog = new List<string>();
			public int NumDeclarations = 0;
			public Exception Exception = null;
		};

		/// <summary>
		/// Runs the sample
		/// </summary>
		/// <param name="inputfile">Path to the input .C/.CPP/.M file to index</param>
		public static void Run(string inputfile)
		{
			// Workaround for threading incompatibility, ensure that LIBCLANG_NOTHREADS
			// is set when the process is being debugged (or unit tested with vstest.console)
			if (Debugger.IsAttached)
			{
				Debug.Assert(Environment.GetEnvironmentVariables().Contains("LIBCLANG_NOTHREADS"));
			}

			// Create a state object to pass into all event handlers, be cautious of types that derive
			// from IEnumerable<string> (like this), as Index.IndexSourceFile()/Index.IndexTranslationUnit()
			// optionally accept an IEnumerable<string> argument to indicate libclang command line arguments
			IndexerState state = new IndexerState();

			using (Index index = Clang.CreateIndex())
			{
				// Add handlers for the indexer event(s) you wish to be notified about
				index.Abort += OnAbort;
				index.Declaration += OnDeclaration;
				index.Diagnostics += OnDiagnostics;
				index.EnteredMainFile += OnEnteredMainFile;
				index.EntityReference += OnEntityReference;
				index.ImportedASTFile += OnImportedASTFile;
				index.IncludedFile += OnIncludedFile;
				index.StartedTranslationUnit += OnStartedTranslationUnit;

				// Invoke the indexer against the provided source file, passing in the optional state object
				index.IndexSourceFile(inputfile, IndexOptions.DetailedPreprocessingRecord, state);

				// Dump the trace log to the console
				foreach (string entry in state.TraceLog) Console.WriteLine(entry);

				// Check if an exception aborted the operation
				if (state.Exception != null) Console.WriteLine("ERROR: " + state.Exception.Message);
			}
		}

		/// <summary>
		/// Invoked periodically by the indexer to allow the caller to abort the operation
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnAbort(object sender, IndexAbortEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;
			state.TraceLog.Add("OnAbort");

			// Check if an exception has been set on the state object and abort the operation
			if (state.Exception != null)
			{
				state.TraceLog.Add("\tAborting due to exception: " + state.Exception.Message);
				e.Abort = true;
			}
		}

		/// <summary>
		/// Invoked when a declaration is being indexed
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnDeclaration(object sender, IndexDeclarationEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;

			// e.Declaration contains information about the declaration being indexed, this is one
			// of the primary handlers you are likely to use during an indexing session, in concert
			// with OnEntityReference (below)
			state.TraceLog.Add("OnDeclaration --> " + e.Declaration.Entity.Name);

			// IndexDeclaration is one of the rare places in zuki.tools.llvm.clang where an object
			// can be cast into a more derived/specialized instance.  Use the .Kind property to
			// determine if this is the case.  C++ declarations may be CxxClass kinds, and Objective-C
			// declarations may be ObjectiveCCategory, ObjectiveCInterface, ObjectiveCProperty, etc.
			if (e.Declaration.Kind == IndexDeclarationKind.CxxClass)
			{
				IndexCxxClassDeclaration cxxclass = (IndexCxxClassDeclaration)e.Declaration;
				state.TraceLog.Add("\t(CxxClass)");
			}

			// Increment the number of declarations found and set an exception if five have
			// been found to illustrate the recommendation of checking this in OnAbort() as
			// thrown exceptions from these event handlers will be ignored by the library
			state.NumDeclarations++;
			try { if (state.NumDeclarations >= 5) throw new Exception("Maximum number of declarations reached"); }
			catch (Exception ex) { state.Exception = ex; throw; }
		}

		/// <summary>
		/// Invoked at the end of indexing to provide a collection of diagnostic messages generated
		/// during the indexing session (warnings, errors, notes, etc)
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnDiagnostics(object sender, IndexDiagnosticsEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;
			state.TraceLog.Add("OnDiagnostics");

			// e.Diagnostics is an enumerable collection of diagnostic objects for the index session
			foreach (var diag in e.Diagnostics) state.TraceLog.Add("\t" + diag.ToString());
		}

		/// <summary>
		/// Invoked when the indexer has started processing the main file of a translation unit
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnEnteredMainFile(object sender, IndexEnteredMainFileEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;

			// e.File contains a reference to informarion about the main translation unit file
			state.TraceLog.Add("OnEnteredMainFile --> " + e.File.Name);
		}

		/// <summary>
		/// Invoked when a previously indexed entity has been referenced, for example a function call
		/// or accessing a variable
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnEntityReference(object sender, IndexEntityReferenceEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;
			state.TraceLog.Add("OnEntityReference --> " + e.EntityReference.ReferencedEntity.Name);
		}

		/// <summary>
		/// Invoked when an Abstract Syntax Tree (AST) precompiled header or module has been imported
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnImportedASTFile(object sender, IndexImportedASTFileEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;
			state.TraceLog.Add("OnImportedASTFile --> " + e.ImportedASTFile.File.Name);
		}

		/// <summary>
		// Invoked when the indexer has arrived at an inclusion directive
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnIncludedFile(object sender, IndexIncludedFileEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;

			// e.IncludedFile contains a reference to information about the file being included.  Note
			// that if the file could not be found, this will still be invoked but e.IncludedFile.File
			// will return a special null File object instance (File.IsNull(object) == true)
			state.TraceLog.Add("OnIncludedFile --> " + e.IncludedFile.Spelling);

			if (!File.IsNull(e.IncludedFile.File)) state.TraceLog.Add("\tResolved as " + e.IncludedFile.File.Name);
			else state.TraceLog.Add("\t(file was not found)");
		}

		/// <summary>
		/// Invoked when the indexer has started a new translation unit
		/// </summary>
		/// <param name="sender">Object raising this event</param>
		/// <param name="e">Event arguments</param>
		private static void OnStartedTranslationUnit(object sender, IndexEventArgs e)
		{
			// e.State contains a reference to the optional state object, or null if not specified
			IndexerState state = (IndexerState)e.State;
			state.TraceLog.Add("OnStartedTranslationUnit");
		}
	}
}
