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

namespace zuki.tools.llvm.clang.test
{
	[TestClass()]
	public class TestUnifiedSymbolResolution
	{
		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_Equality()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr1 = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr1);
			Assert.IsFalse(String.IsNullOrEmpty(classusr1));

			// c:objc(cs)classname
			UnifiedSymbolResolution classusr2 = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr2);
			Assert.IsFalse(String.IsNullOrEmpty(classusr2));

			Assert.IsTrue(classusr1 == classusr2);
			Assert.IsTrue(classusr2 == classusr1);
			Assert.AreEqual(classusr1, classusr2);
			Assert.AreEqual(classusr2, classusr1);
			Assert.IsTrue(classusr1.Equals(classusr2));
			Assert.IsTrue(classusr2.Equals(classusr1));
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_Inequality()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));

			// c:objc(cs)classname@ivarname
			UnifiedSymbolResolution ivarusr = UnifiedSymbolResolution.FromObjectiveCInstanceVariable("ivarname", classusr);
			Assert.IsNotNull(ivarusr);
			Assert.IsFalse(String.IsNullOrEmpty(ivarusr));

			Assert.IsTrue(classusr != ivarusr);
			Assert.IsTrue(ivarusr != classusr);
			Assert.AreNotEqual(classusr, ivarusr);
			Assert.AreNotEqual(ivarusr, classusr);
			Assert.IsFalse(classusr.Equals(ivarusr));
			Assert.IsFalse(ivarusr.Equals(classusr));
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_ImplicitString()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreSame((String)classusr, (String)classusr);
			Assert.AreEqual((String)classusr, classusr.ToString());
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCCategory()
		{
			// c:objc(cy)classname@categoryname
			UnifiedSymbolResolution categoryusr = UnifiedSymbolResolution.FromObjectiveCCategory("classname", "categoryname");
			Assert.IsNotNull(categoryusr);
			Assert.IsFalse(String.IsNullOrEmpty(categoryusr));
			Assert.AreEqual("c:objc(cy)classname@categoryname", categoryusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCClass()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreEqual("c:objc(cs)classname", classusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCMethod()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreEqual("c:objc(cs)classname", classusr);

			// c:objc(cs)classname(im)imethodname
			UnifiedSymbolResolution imethodusr = UnifiedSymbolResolution.FromObjectiveCMethod("imethodname", true, classusr);
			Assert.IsNotNull(imethodusr);
			Assert.IsFalse(String.IsNullOrEmpty(imethodusr));
			Assert.AreEqual("c:objc(cs)classname(im)imethodname", imethodusr);

			// c:objc(cs)classname(cm)cmethodname
			UnifiedSymbolResolution cmethodusr = UnifiedSymbolResolution.FromObjectiveCMethod("cmethodname", false, classusr);
			Assert.IsNotNull(cmethodusr);
			Assert.IsFalse(String.IsNullOrEmpty(cmethodusr));
			Assert.AreEqual("c:objc(cs)classname(cm)cmethodname", cmethodusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCInstanceVariable()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreEqual("c:objc(cs)classname", classusr);

			// c:objc(cs)classname@ivarname
			UnifiedSymbolResolution ivarusr = UnifiedSymbolResolution.FromObjectiveCInstanceVariable("ivarname", classusr);
			Assert.IsNotNull(ivarusr);
			Assert.IsFalse(String.IsNullOrEmpty(ivarusr));
			Assert.AreEqual("c:objc(cs)classname@ivarname", ivarusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCProperty()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreEqual("c:objc(cs)classname", classusr);

			// c:objc(cs)classname(py)propname
			UnifiedSymbolResolution propertyusr = UnifiedSymbolResolution.FromObjectiveCProperty("propname", classusr);
			Assert.IsNotNull(propertyusr);
			Assert.IsFalse(String.IsNullOrEmpty(propertyusr));
			Assert.AreEqual("c:objc(cs)classname(py)propname", propertyusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_FromObjectiveCProtocol()
		{
			// c:objc(pl)protocolname
			UnifiedSymbolResolution protocolusr = UnifiedSymbolResolution.FromObjectiveCProtocol("protocolname");
			Assert.IsNotNull(protocolusr);
			Assert.IsFalse(String.IsNullOrEmpty(protocolusr));
			Assert.AreEqual("c:objc(pl)protocolname", protocolusr);
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_GetHashCode()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreNotEqual(0, classusr.GetHashCode());
		}

		[TestMethod, TestCategory("Unified Symbol Resolution")]
		public void UnifiedSymbolResolution_ToString()
		{
			// c:objc(cs)classname
			UnifiedSymbolResolution classusr = UnifiedSymbolResolution.FromObjectiveCClass("classname");
			Assert.IsNotNull(classusr);
			Assert.IsFalse(String.IsNullOrEmpty(classusr));
			Assert.AreEqual("c:objc(cs)classname", classusr.ToString());
		}
	}
}
