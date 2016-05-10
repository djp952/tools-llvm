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
using zuki.tools.llvm.clang.extensions;

namespace zuki.tools.llvm.clang.Samples
{
	/// <summary>
	/// Illustrates basic use of the clang unsaved files functionality.  Unsaved files allow
	/// the caller to create and use virtual source code files as part of an indexing operation
	/// </summary>
	public static class UnsavedFiles
	{
		/// <summary>
		/// Runs the sample
		/// </summary>
		public static void Run()
		{
			// Some trivial virtual code to illustrate unsaved files
			string virtual_h = "#ifndef __VIRTUAL_H_\n#define __VIRTUAL_H_\n#define MYINT 123\n#endif";
			string virtual_c = "#include \"virtual.h\"\nint main(void) { return MYINT; }\n";

			// To create unsaved files for indexing, create instances of UnsavedFile objects,
			// providing file names and content for each.  Calls that accept unsaved files all take
			// in IEnumerable<UnsavedFile>, so you can use an array or a List<UnsavedFile> or the results
			// from a Linq Enumerable operation.  When specifying header files, it's best to give it a
			// fully qualified path name, or simply prefix it with "./" to indicate the root
			List<UnsavedFile> unsavedfiles = new List<UnsavedFile>();
			unsavedfiles.Add(new UnsavedFile("./virtual.h", virtual_h));
			unsavedfiles.Add(new UnsavedFile("virtual.c", virtual_c));

			// Create a translation unit specifying just the unsaved files.  Specifying DetailedPreprocessingRecord
			// allows for examination of preprocessor items, like the #define macro in the virtual code above.  Note
			// that the file name provided should either match the 'main' file/virtual file or can be null for
			// operations like creating a precompiled header (see PrecompiledHeader sample)
			//
			// When using unsaved header files, you will typically need to give libclang a specific include directory
			// to look for with the -I argument.  For this sample, use the "./" directory indicated for virtual.h
			string[] arguments = new string[] { "-I./" };
			using (TranslationUnit transunit = Clang.CreateTranslationUnit("virtual.c", arguments, unsavedfiles, TranslationUnitParseOptions.DetailedPreprocessingRecord))
			{
				// Find the macro expansion MYINT from virtual.c using the Cursor.FindChildren extension
				// method (zuki.tools.llvm.clang.extensions namespace) -- expect to only find one here
				var cursors = transunit.Cursor.FindChildren((cursor, parent) => cursor.Kind == CursorKind.MacroExpansion && cursor.DisplayName == "MYINT");
				if (cursors.Count != 1) throw new Exception("You seem to have misplaced your cursor");

				// Pull out the macro expansion cursor, Tuple.Item1 is the found cursor, Tuple.Item2 is the parent cursor
				Cursor myintexpansion = cursors[0].Item1;

				// From the macro expansion, we can get the location and file in which it was defined, thus illustrating
				// that both virtual.h and virtual.c were indexed by libclang without ever saving anything to disk
				Cursor myintdeclaration = myintexpansion.DefinitionCursor;

				// Setting a debugger breakpoint and examining the translation unit objects is probably the best way to
				// see what's available and what to expect, so ...
				//
				Debugger.Break();
				//
				// Examine myintexpansion here, you should see an Extent of "virtual.c (2, 25) - virtual.c (2, 30)"
				// Examine myintdeclaration here, you should see an Extent of "./virtual.h (3, 9) - ./virtual.h (3, 18)"
			}
		}
	}
}
