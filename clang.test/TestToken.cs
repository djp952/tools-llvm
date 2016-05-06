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
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestToken
	{
		private static TokenCollection s_tokens;

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			// TokenCollection will keep the parent TranslationUnit alive internally, and
			// the TranslationUnit will keep the Index alive, so just grab a TokenCollection
			string code = "int g_x = 10; // global variable\r\nint* g_px = &g_x; // pointer to a global variable\r\nvoid myfunc(void) { return; }";
			s_tokens = Clang.CreateIndex().CreateTranslationUnitFromString(code).Cursor.Extent.GetTokens();
			Assert.IsNotNull(s_tokens);
			Assert.AreNotEqual(0, s_tokens.Count);
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			s_tokens.Dispose();
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Dispose()
		{
			// TokenCollection will keep the parent TranslationUnit alive internally, so
			// when the TranslationUnit is disposed the expectation is that the token
			// collection will remain alive and accessible
			TokenCollection tokens;

			string code = "int x = 123;";
			using (TranslationUnit unit = Clang.CreateIndex().CreateTranslationUnitFromString(code))
			{
				Assert.IsNotNull(unit);
				tokens = unit.Cursor.Extent.GetTokens();
				Assert.IsNotNull(tokens);
				Assert.AreNotEqual(0, tokens.Count);
			}

			// Collection should not be disposed
			Assert.IsFalse(tokens.IsDisposed(() => { var v = tokens.Count; }));
			Assert.AreNotEqual(0, tokens.Count);
			Assert.IsNotNull(tokens[0]);

			// Now dispose of the token collection and ensure it's dead
			tokens.Dispose();
			Assert.IsTrue(tokens.IsDisposed(() => { var v = tokens.Count; }));
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Annotation()
		{
			Assert.IsNotNull(s_tokens);

			// Ensure that every token in the collection has an annotation
			foreach (Token token in s_tokens)
			{
				// Punctuation and Comment do not get annotations
				Assert.AreSame(token.Annotation, token.Annotation);
				if ((token.Kind != TokenKind.Punctuation) && (token.Kind != TokenKind.Comment))
				{
					// .Annotation should refer to a valid Cursor object
					Assert.IsNotNull(token.Annotation);
					Assert.IsFalse(Cursor.IsNull(token.Annotation));
				}
			}
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Extent()
		{
			Assert.IsNotNull(s_tokens);

			// Ensure that every token in the collection has an extent
			foreach (Token token in s_tokens)
			{
				// .Extent should refer to a valid Extent object
				Assert.AreSame(token.Extent, token.Extent);
				Assert.IsNotNull(token.Extent);
				Assert.IsFalse(Extent.IsNull(token.Extent));
			}
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Location()
		{
			Assert.IsNotNull(s_tokens);

			// Ensure that every token in the collection has a location
			foreach (Token token in s_tokens)
			{
				// .Location should refer to a valid Location object
				Assert.AreSame(token.Location, token.Location);
				Assert.IsNotNull(token.Location);
				Assert.IsFalse(Location.IsNull(token.Location));
			}
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Kind()
		{
			Assert.IsNotNull(s_tokens);

			// TokenKind.Comment
			Token token = s_tokens.First(t => t.Spelling == "// global variable");
			Assert.IsNotNull(token);
			Assert.AreEqual(TokenKind.Comment, token.Kind);

			// TokenKind.Identifier
			token = s_tokens.First(t => t.Spelling == "g_x");
			Assert.IsNotNull(token);
			Assert.AreEqual(TokenKind.Identifier, token.Kind);

			// TokenKind.Keyword
			token = s_tokens.First(t => t.Spelling == "int");
			Assert.IsNotNull(token);
			Assert.AreEqual(TokenKind.Keyword, token.Kind);

			// TokenKind.Literal
			token = s_tokens.First(t => t.Spelling == "10");
			Assert.IsNotNull(token);
			Assert.AreEqual(TokenKind.Literal, token.Kind);

			// TokenKind.Punctuation
			token = s_tokens.First(t => t.Spelling == ";");
			Assert.IsNotNull(token);
			Assert.AreEqual(TokenKind.Punctuation, token.Kind);
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_Spelling()
		{
			Assert.IsNotNull(s_tokens);

			// Ensure that every token in the collection has a spelling string
			foreach (Token token in s_tokens)
			{
				// .Spelling should be set to something non-null
				Assert.AreSame(token.Spelling, token.Spelling);
				Assert.IsFalse(String.IsNullOrEmpty(token.Spelling));
			}
		}

		[TestMethod(), TestCategory("Tokens")]
		public void Token_ToString()
		{
			Assert.IsNotNull(s_tokens);

			// Ensure that every token in the collection has a ToString() representation
			foreach (Token token in s_tokens)
			{
				// .ToString() is a method, not required to return the same object reference
				// every time it's called, but it should equal the token Spelling
				string str = token.ToString();
				Assert.IsFalse(String.IsNullOrEmpty(str));
				Assert.AreEqual(token.Spelling, str);
			}
		}
	}
}
