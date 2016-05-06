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
using System.Linq;

namespace zuki.tools.llvm.clang.test
{
	static class Extensions
	{
		/// <summary>
		/// Converts a DateTime into a clang compatible time_t
		/// </summary>
		/// <param name="datetime">DateTime instance to be converted</param>
		public static long ToTimeT(this DateTime datetime)
		{
			return ((datetime.ToFileTime() - 116444736000000000) / 10000000);
		}

		/// <summary>
		/// Creates a translation unit from a string.  Taken from the clang python bindings - util.py::get_tu
		/// </summary>
		/// <param name="index">Index instance being extended</param>
		/// <param name="code">Code string</param>
		/// <param name="language">Language for the code string (c, cpp, objc)</param>
		/// <param name="allwarnings">Enable all compiler warnings</param>
		/// <param name="arguments">Arguments to pass into clang</param>
		public static TranslationUnit CreateTranslationUnitFromString(this Index index, string code, Language language, bool allwarnings, IEnumerable<string> arguments)
		{
			// Do all the same stuff the python helper function does, going to be using their tests ...
			string name = "t.c";
			List<string> args = new List<string>(arguments);

			if (language == Language.CPlusPlus) { name = "t.cpp"; args.Add("-std=c++11"); }
			else if (language == Language.ObjectiveC) { name = "t.m"; }
			else if (language != Language.C) throw new Exception("Unknown language: " + language);

			if (allwarnings) { args.Add("-Wall"); args.Add("-Wextra"); }

			return index.CreateTranslationUnit(name, args, new UnsavedFile[] { new UnsavedFile(name, code) }, TranslationUnitParseOptions.DetailedPreprocessingRecord);
		}

		/// <summary>
		/// Convenience overload for CreateTranslationUnitFromString
		/// </summary>
		/// <param name="index">Index instance being extended</param>
		/// <param name="code">Code string (C syntax)</param>
		public static TranslationUnit CreateTranslationUnitFromString(this Index index, string code)
		{
			return index.CreateTranslationUnitFromString(code, Language.C, false, Enumerable.Empty<string>());
		}

		/// <summary>
		/// Convenience overload for CreateTranslationUnitFromString
		/// </summary>
		/// <param name="index">Index instance being extended</param>
		/// <param name="code">Code string</param>
		/// <param name="language">Language of the code string</param>
		public static TranslationUnit CreateTranslationUnitFromString(this Index index, string code, Language language)
		{
			return index.CreateTranslationUnitFromString(code, language, false, Enumerable.Empty<string>());
		}

		/// <summary>
		/// Convenience overload for CreateTranslationUnitFromString
		/// </summary>
		/// <param name="index">Index instance being extended</param>
		/// <param name="code">Code string (C syntax)</param>
		/// <param name="arguments">Arguments to pass to clang</param>
		public static TranslationUnit CreateTranslationUnitFromString(this Index index, string code, IEnumerable<string> arguments)
		{
			return index.CreateTranslationUnitFromString(code, Language.C, false, arguments);
		}

		/// <summary>
		/// Convenience overload for CreateTranslationUnitFromString
		/// </summary>
		/// <param name="index">Index instance being extended</param>
		/// <param name="code">Code string</param>
		/// <param name="language">Language of the code string</param>
		/// <param name="arguments">Arguments to pass to clang</param>
		public static TranslationUnit CreateTranslationUnitFromString(this Index index, string code, Language language, IEnumerable<string> arguments)
		{
			return index.CreateTranslationUnitFromString(code, language, false, arguments);
		}

		/// <summary>
		/// Locates a cursor within a Cursor by spelling
		/// </summary>
		/// <param name="tu">Cursor instance being extended</param>
		/// <param name="spelling">Spelling of the cursor to find</param>
		public static Cursor FindCursor(this Cursor cursor, string spelling)
		{
			return FindCursor(cursor, spelling, false);
		}

		/// <summary>
		/// Locates a cursor within a Cursor by spelling
		/// </summary>
		/// <param name="cursor">Cursor instance being extended</param>
		/// <param name="spelling">Spelling of the cursor to find</param>
		/// <param name="recursive">Recursive search flag</param>
		public static Cursor FindCursor(this Cursor cursor, string spelling, bool recursive)
		{
			Cursor result = Cursor.Null;

			cursor.EnumerateChildren((c, p) =>
			{
				if (String.Compare(c.Spelling, spelling) == 0) { result = c; return EnumerateChildrenResult.Break; }
				else return (recursive) ? EnumerateChildrenResult.Recurse : EnumerateChildrenResult.Continue;
			});

			return result;
		}

		/// <summary>
		/// Locates a cursor within a TranslationUnit by spelling
		/// </summary>
		/// <param name="tu">TranslationUnit instance being extended</param>
		/// <param name="spelling">Spelling of the cursor to find</param>
		public static Cursor FindCursor(this TranslationUnit tu, string spelling)
		{
			return FindCursor(tu.Cursor, spelling, false);
		}

		/// <summary>
		/// Locates a cursor within a TranslationUnit by spelling
		/// </summary>
		/// <param name="tu">TranslationUnit instance being extended</param>
		/// <param name="spelling">Spelling of the cursor to find</param>
		/// <param name="recursive">Recursive search flag</param>
		public static Cursor FindCursor(this TranslationUnit tu, string spelling, bool recursive)
		{
			return FindCursor(tu.Cursor, spelling, recursive);
		}

		/// <summary>
		/// Determines if a method/property access results in an ObjectDisposedException
		/// </summary>
		/// <param name="func">Action to invoke</param>
		public static bool IsDisposed(this Object obj, Action func)
		{
			try { func(); return false; }
			catch (ObjectDisposedException) { return true; }
		}

		/// <summary>
		/// Finds a child comment instance based on a predicate
		/// </summary>
		/// <param name="instance">Comment instance being extended</param>
		/// <param name="predicate">Search predicate</param>
		public static Comment FindComment(this Comment instance, Func<Comment, bool> predicate)
		{
			Comment result = null;

			if (Object.ReferenceEquals(instance, null)) throw new ArgumentNullException("instance");

			if (predicate(instance)) return instance;
			foreach (Comment child in instance.Children)
			{
				result = child.FindComment(predicate);
				if (!Object.ReferenceEquals(result, null)) break;
			}

			return result;
		}
	}
}
