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
using Microsoft.VisualStudio.TestTools.UnitTesting;
using zuki.tools.llvm.clang.extensions;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestComment
	{
		static Index s_index;

		[ClassInitialize()]
		public static void ClassInit(TestContext context)
		{
			s_index = Clang.CreateIndex();
			Assert.IsNotNull(s_index);
		}

		[ClassCleanup()]
		public static void ClassCleanup()
		{
			if (s_index != null) s_index.Dispose();
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_BlockCommandComment()
		{
			string code = "/// \\brief Aaa.\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// \brief
				BlockCommandComment brief = (BlockCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.BlockCommand);
				Assert.IsNotNull(brief);
				Assert.AreSame(brief.Arguments, brief.Arguments);
				Assert.AreEqual(0, brief.Arguments.Count);
				Assert.AreSame(brief.Children, brief.Children);
				Assert.AreEqual(1, brief.Children.Count);
				Assert.AreSame(brief.CommandName, brief.CommandName);
				Assert.AreEqual("brief", brief.CommandName);
				Assert.AreSame(brief.Paragraph, brief.Paragraph);
				Assert.AreEqual("brief", brief.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_HtmlStartTagComment()
		{
			string code = "/// <br/><a href=\"http://example.com/\">Aaa</a>\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// <br>
				HtmlStartTagComment openbr = (HtmlStartTagComment)parsed.FindComment((c) => c.Kind == CommentKind.HtmlStartTag &&
					((HtmlStartTagComment)c).TagName == "br");
				Assert.IsNotNull(openbr);
				Assert.AreEqual(CommentKind.HtmlStartTag, openbr.Kind);
				Assert.AreSame(openbr.Attributes, openbr.Attributes);
				Assert.AreEqual(0, openbr.Attributes.Count);
				Assert.AreSame(openbr.Children, openbr.Children);
				Assert.AreEqual(0, openbr.Children.Count);
				Assert.AreSame(openbr.TagName, openbr.TagName);
				Assert.AreEqual("br", openbr.TagName);
				Assert.IsTrue(openbr.IsSelfClosing);
				Assert.IsFalse(openbr.HasTrailingNewline);
				Assert.AreEqual("<br/>", openbr.ToString());

				// <a>
				HtmlStartTagComment opena = (HtmlStartTagComment)parsed.FindComment((c) => c.Kind == CommentKind.HtmlStartTag &&
					((HtmlStartTagComment)c).TagName == "a");
				Assert.IsNotNull(opena);
				Assert.AreEqual(CommentKind.HtmlStartTag, opena.Kind);
				Assert.AreSame(opena.Attributes, opena.Attributes);
				Assert.AreEqual(1, opena.Attributes.Count);
				Assert.IsTrue(opena.Attributes.ContainsKey("href"));
				Assert.AreSame(opena.Attributes["href"], opena.Attributes["href"]);
				Assert.AreEqual("http://example.com/", opena.Attributes["href"]);

				string s;
				Assert.IsTrue(opena.Attributes.TryGetValue("href", out s));

				// Enumerators for code coverage
				foreach (KeyValuePair<string, string> pair in opena.Attributes) Assert.IsNotNull(pair);
				foreach (string key in opena.Attributes.Keys) Assert.IsNotNull(key);
				foreach (string value in opena.Attributes.Values) Assert.IsNotNull(value);

				// Generic enumerators for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)opena.Attributes) Assert.IsNotNull(obj);
				foreach (Object obj in (System.Collections.IEnumerable)opena.Attributes.Keys) Assert.IsNotNull(obj);
				foreach (Object obj in (System.Collections.IEnumerable)opena.Attributes.Values) Assert.IsNotNull(obj);

				Assert.AreSame(opena.Children, opena.Children);
				Assert.AreEqual(0, opena.Children.Count);
				Assert.AreSame(opena.TagName, opena.TagName);
				Assert.AreEqual("a", opena.TagName);
				Assert.AreEqual("<a href=\"http://example.com/\">", opena.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_HtmlEndTagComment()
		{
			string code = "/// <br/><a href=\"http://example.com/\">Aaa</a>\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// </a>
				HtmlEndTagComment closea = (HtmlEndTagComment)parsed.FindComment((c) => c.Kind == CommentKind.HtmlEndTag);
				Assert.IsNotNull(closea);
				Assert.AreEqual(CommentKind.HtmlEndTag, closea.Kind);
				Assert.AreSame(closea.Children, closea.Children);
				Assert.AreEqual(false, closea.HasTrailingNewline);
				Assert.AreSame(closea.TagName, closea.TagName);
				Assert.AreEqual("a", closea.TagName);
				Assert.AreEqual("</a>", closea.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_InlineCommandComment()
		{
			string code = "/// \\c Aaa.\nvoid checkme();";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// \c Aaa.
				InlineCommandComment command = (InlineCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.InlineCommand);
				Assert.IsNotNull(command);
				Assert.AreEqual(CommentKind.InlineCommand, command.Kind);
				Assert.AreSame(command.Arguments, command.Arguments);
				Assert.AreEqual(1, command.Arguments.Count);
				Assert.AreEqual(command.Arguments[0], command.Arguments[0]);
				Assert.AreEqual("Aaa.", command.Arguments[0]);
				Assert.AreSame(command.Children, command.Children);
				Assert.AreEqual(0, command.Children.Count);
				Assert.AreSame(command.CommandName, command.CommandName);
				Assert.AreEqual("c", command.CommandName);
				Assert.AreEqual(false, command.HasTrailingNewline);
				Assert.AreEqual(InlineCommandRenderKind.Monospaced, command.RenderKind);
				Assert.AreEqual("c", command.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_NullComment()
		{
			ParsedComment comment = Cursor.Null.ParsedComment;
			Assert.IsNotNull(comment);
			Assert.AreEqual(CommentKind.Null, comment.Kind);
			Assert.AreSame(comment.Children, comment.Children);
			Assert.AreEqual(0, comment.Children.Count);
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_ParagraphComment()
		{
			string code = "/// \\brief Aaa.\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// (whitespace)
				ParagraphComment white = (ParagraphComment)parsed.FindComment((c) => c.Kind == CommentKind.Paragraph &&
					((ParagraphComment)c).IsWhitespace == true);
				Assert.IsNotNull(white);
				Assert.AreEqual(CommentKind.Paragraph, white.Kind);
				Assert.AreSame(white.Children, white.Children);
				Assert.AreEqual(1, white.Children.Count);
				Assert.IsTrue(white.IsWhitespace);
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_ParamCommandComment()
		{
			string code = "/// \\param x2 Bbb.\n/// \\param x1 Aaa.\nvoid checkme(int x1, int x2);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// x2
				ParamCommandComment x2 = (ParamCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.ParamCommand &&
					((ParamCommandComment)c).ParameterName == "x2");
				Assert.IsNotNull(x2);
				Assert.AreEqual(CommentKind.ParamCommand, x2.Kind);
				Assert.AreSame(x2.Arguments, x2.Arguments);
				Assert.AreEqual(1, x2.Arguments.Count);
				Assert.AreSame(x2.Arguments[0], x2.Arguments[0]);
				Assert.AreEqual("x2", x2.Arguments[0]);
				Assert.AreSame(x2.Children, x2.Children);
				Assert.AreEqual(1, x2.Children.Count);
				Assert.AreSame(x2.CommandName, x2.CommandName);
				Assert.AreEqual("param", x2.CommandName);
				Assert.AreEqual(ParameterDirection.In, x2.Direction);
				Assert.AreEqual((uint)1, x2.Index);
				Assert.IsFalse(x2.IsDirectionExplicit);
				Assert.AreSame(x2.Paragraph, x2.Paragraph);
				Assert.AreSame(x2.ParameterName, x2.ParameterName);
				Assert.AreEqual("x2", x2.ParameterName);
				Assert.AreEqual("x2", x2.ToString());

				// x1
				ParamCommandComment x1 = (ParamCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.ParamCommand &&
					((ParamCommandComment)c).ParameterName == "x1");
				Assert.IsNotNull(x1);
				Assert.AreEqual(CommentKind.ParamCommand, x1.Kind);
				Assert.AreSame(x1.Arguments, x1.Arguments);
				Assert.AreEqual(1, x1.Arguments.Count);
				Assert.AreSame(x1.Arguments[0], x1.Arguments[0]);
				Assert.AreEqual("x1", x1.Arguments[0]);
				Assert.AreSame(x1.Children, x1.Children);
				Assert.AreEqual(1, x1.Children.Count);
				Assert.AreSame(x1.CommandName, x1.CommandName);
				Assert.AreEqual("param", x1.CommandName);
				Assert.AreEqual(ParameterDirection.In, x1.Direction);
				Assert.AreEqual((uint)0, x1.Index);
				Assert.IsFalse(x1.IsDirectionExplicit);
				Assert.AreSame(x1.Paragraph, x1.Paragraph);
				Assert.AreSame(x1.ParameterName, x1.ParameterName);
				Assert.AreEqual("x1", x1.ParameterName);
				Assert.AreEqual("x1", x1.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_ParsedComment()
		{
			string code = "/// Aaa.\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment comment = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(comment);
				Assert.AreEqual(CommentKind.Parsed, comment.Kind);
				Assert.AreSame(comment.Children, comment.Children);
				Assert.AreEqual(1, comment.Children.Count);

				// Generic enumerator for code coverage
				foreach (Object obj in (System.Collections.IEnumerable)comment.Children) Assert.IsNotNull(obj);

				Assert.IsFalse(String.IsNullOrEmpty(comment.ToHtmlFragment()));
				Assert.IsFalse(String.IsNullOrEmpty(comment.ToXmlFragment()));
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_TextComment()
		{
			string code = "/// Aaa.\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// Aaa.
				TextComment text = (TextComment)parsed.FindComment((c) => c.Kind == CommentKind.Text);
				Assert.IsNotNull(text);
				Assert.AreEqual(CommentKind.Text, text.Kind);
				Assert.AreSame(text.Text, text.Text);
				Assert.AreEqual(" Aaa.", text.Text);
				Assert.IsFalse(text.HasTrailingNewline);
				Assert.IsFalse(text.IsWhitespace);
				Assert.AreEqual(0, text.Children.Count);
				Assert.AreEqual(" Aaa.", text.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_TParamCommandComment()
		{
			string code = "/// \\tparam T2 Bbb\n/// \\tparam T1 Aaa\ntemplate < typename T1, typename T2>\nvoid checkme(T1 aaa, T2 bbb);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code, Language.CPlusPlus))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// T1
				TParamCommandComment t1 = (TParamCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.TParamCommand &&
					((TParamCommandComment)c).ParameterName == "T1");
				Assert.IsNotNull(t1);
				Assert.AreEqual(CommentKind.TParamCommand, t1.Kind);
				Assert.AreSame(t1.Arguments, t1.Arguments);
				Assert.AreEqual(1, t1.Arguments.Count);
				Assert.AreSame(t1.Arguments[0], t1.Arguments[0]);
				Assert.AreEqual("T1", t1.Arguments[0]);
				Assert.AreSame(t1.Children, t1.Children);
				Assert.AreEqual(1, t1.Children.Count);
				Assert.AreSame(t1.CommandName, t1.CommandName);
				Assert.AreEqual("tparam", t1.CommandName);
				Assert.AreSame(t1.Index, t1.Index);
				Assert.IsNotNull(t1.Index[0]);
				Assert.IsNotNull(t1.Index[1]);
				Assert.AreEqual((uint)1, t1.Index.NestingDepth);
				Assert.AreSame(t1.Paragraph, t1.Paragraph);
				Assert.AreSame(t1.ParameterName, t1.ParameterName);
				Assert.AreEqual("T1", t1.ParameterName);
				Assert.AreEqual("T1", t1.ToString());

				// T2
				TParamCommandComment t2 = (TParamCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.TParamCommand &&
					((TParamCommandComment)c).ParameterName == "T2");
				Assert.IsNotNull(t2);
				Assert.AreEqual(CommentKind.TParamCommand, t2.Kind);
				Assert.AreSame(t2.Arguments, t2.Arguments);
				Assert.AreEqual(1, t2.Arguments.Count);
				Assert.AreSame(t2.Arguments[0], t2.Arguments[0]);
				Assert.AreEqual("T2", t2.Arguments[0]);
				Assert.AreSame(t2.Children, t2.Children);
				Assert.AreEqual(1, t2.Children.Count);
				Assert.AreSame(t2.CommandName, t2.CommandName);
				Assert.AreEqual("tparam", t2.CommandName);
				Assert.AreSame(t2.Index, t2.Index);
				Assert.IsNotNull(t2.Index[0]);
				Assert.IsNotNull(t2.Index[1]);
				Assert.AreEqual((uint)1, t2.Index.NestingDepth);
				Assert.AreSame(t2.Paragraph, t2.Paragraph);
				Assert.AreSame(t2.ParameterName, t2.ParameterName);
				Assert.AreEqual("T2", t2.ParameterName);
				Assert.AreEqual("T2", t2.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_VerbatimBlockCommandComment()
		{
			string code = "/// \\verbatim\n/// <a href=\"http://example.com/\">Aaa</a>\n/// \\endverbatim\nvoid checkme();";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// verbatim
				VerbatimBlockCommandComment verbatim = (VerbatimBlockCommandComment)parsed.FindComment((c) => c.Kind == CommentKind.VerbatimBlockCommand);
				Assert.IsNotNull(verbatim);
				Assert.AreEqual(CommentKind.VerbatimBlockCommand, verbatim.Kind);
				Assert.AreSame(verbatim.Arguments, verbatim.Arguments);
				Assert.AreEqual(0, verbatim.Arguments.Count);
				Assert.AreSame(verbatim.Children, verbatim.Children);
				Assert.AreEqual(1, verbatim.Children.Count);
				Assert.AreSame(verbatim.CommandName, verbatim.CommandName);
				Assert.AreEqual("verbatim", verbatim.CommandName);
				Assert.AreSame(verbatim.Paragraph, verbatim.Paragraph);
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_VerbatimBlockLineComment()
		{
			string code = "/// \\verbatim\n/// <a href=\"http://example.com/\">Aaa</a>\n/// \\endverbatim\nvoid checkme();";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// verbatim
				VerbatimBlockLineComment line = (VerbatimBlockLineComment)parsed.FindComment((c) => c.Kind == CommentKind.VerbatimBlockLine);
				Assert.IsNotNull(line);
				Assert.AreEqual(CommentKind.VerbatimBlockLine, line.Kind);
				Assert.AreSame(line.Children, line.Children);
				Assert.AreEqual(0, line.Children.Count);
				Assert.AreSame(line.Text, line.Text);
				Assert.AreEqual(" <a href=\"http://example.com/\">Aaa</a>", line.Text);
				Assert.AreEqual(" <a href=\"http://example.com/\">Aaa</a>", line.ToString());
			}
		}

		[TestMethod(), TestCategory("Comments")]
		public void Comment_VerbatimLineComment()
		{
			string code = "/// \\def foo_def\n/// \\fn foo_fn\nvoid checkme(void);";
			using (TranslationUnit tu = s_index.CreateTranslationUnitFromString(code))
			{
				ParsedComment parsed = tu.FindCursor("checkme").ParsedComment;
				Assert.IsNotNull(parsed);

				// foo_def
				VerbatimLineComment foodef = (VerbatimLineComment)parsed.FindComment((c) => c.Kind == CommentKind.VerbatimLine &&
					((VerbatimLineComment)c).Text == " foo_def");
				Assert.IsNotNull(foodef);
				Assert.AreEqual(CommentKind.VerbatimLine, foodef.Kind);
				Assert.AreSame(foodef.Arguments, foodef.Arguments);
				Assert.AreEqual(0, foodef.Arguments.Count);
				Assert.AreSame(foodef.Children, foodef.Children);
				Assert.AreEqual(0, foodef.Children.Count);
				Assert.AreSame(foodef.CommandName, foodef.CommandName);
				Assert.AreEqual("def", foodef.CommandName);
				Assert.AreSame(foodef.Paragraph, foodef.Paragraph);
				Assert.AreSame(foodef.Text, foodef.Text);
				Assert.AreEqual(" foo_def", foodef.Text);
				Assert.AreEqual(" foo_def", foodef.ToString());

				// foo_fn
				VerbatimLineComment foofn = (VerbatimLineComment)parsed.FindComment((c) => c.Kind == CommentKind.VerbatimLine &&
					((VerbatimLineComment)c).Text == " foo_fn");
				Assert.IsNotNull(foofn);
				Assert.AreEqual(CommentKind.VerbatimLine, foofn.Kind);
				Assert.AreSame(foofn.Arguments, foofn.Arguments);
				Assert.AreEqual(0, foofn.Arguments.Count);
				Assert.AreSame(foofn.Children, foofn.Children);
				Assert.AreEqual(0, foofn.Children.Count);
				Assert.AreSame(foofn.CommandName, foofn.CommandName);
				Assert.AreEqual("fn", foofn.CommandName);
				Assert.AreSame(foofn.Paragraph, foofn.Paragraph);
				Assert.AreSame(foofn.Text, foofn.Text);
				Assert.AreEqual(" foo_fn", foofn.Text);
				Assert.AreEqual(" foo_fn", foofn.ToString());
			}
		}
	}
}
