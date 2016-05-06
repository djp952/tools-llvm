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
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestClangErrorCode
	{
		[TestMethod(), TestCategory("Miscellaneous")]
		public void ClangErrorCode_Equality()
		{
			ClangErrorCode errorcode = ClangErrorCode.Failure;
			Assert.IsTrue(errorcode == ClangErrorCode.Failure);
			Assert.IsTrue(ClangErrorCode.Failure == errorcode);
			Assert.AreEqual(errorcode, ClangErrorCode.Failure);
			Assert.AreEqual(ClangErrorCode.Failure, errorcode);
			Assert.IsTrue(errorcode.Equals(ClangErrorCode.Failure));
			Assert.IsTrue(ClangErrorCode.Failure.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Miscellaneous")]
		public void ClangErrorCode_Inequality()
		{
			ClangErrorCode errorcode = ClangErrorCode.Failure;
			Assert.IsTrue(errorcode != ClangErrorCode.Success);
			Assert.IsTrue(ClangErrorCode.Success != errorcode);
			Assert.AreNotEqual(errorcode, ClangErrorCode.Success);
			Assert.AreNotEqual(ClangErrorCode.Success, errorcode);
			Assert.IsFalse(errorcode.Equals(ClangErrorCode.Success));
			Assert.IsFalse(ClangErrorCode.Success.Equals(errorcode));
		}

		[TestMethod(), TestCategory("Miscellaneous")]
		public void ClangErrorCode_GetHashCode()
		{
			Assert.AreNotEqual(0, ClangErrorCode.ASTReadError.GetHashCode());
			Assert.AreNotEqual(0, ClangErrorCode.Crashed.GetHashCode());
			Assert.AreNotEqual(0, ClangErrorCode.Failure.GetHashCode());
			Assert.AreNotEqual(0, ClangErrorCode.InvalidArguments.GetHashCode());

			// Success is zero, so the hash code will also be zero
			Assert.AreEqual(0, ClangErrorCode.Success.GetHashCode());
		}

		[TestMethod(), TestCategory("Miscellaneous")]
		public void ClangErrorCode_ToString()
		{
			Assert.IsFalse(String.IsNullOrEmpty(ClangErrorCode.ASTReadError.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(ClangErrorCode.Crashed.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(ClangErrorCode.Failure.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(ClangErrorCode.InvalidArguments.ToString()));
			Assert.IsFalse(String.IsNullOrEmpty(ClangErrorCode.Success.ToString()));
		}
	}
}
