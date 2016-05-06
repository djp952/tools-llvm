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

#include "stdafx.h"
#include "UnifiedSymbolResolution.h"

#include "Cursor.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// UnifiedSymbolResolution Constructor (private)
//
// Arguments:
//
//	usr			- Unified symbol resolution string

UnifiedSymbolResolution::UnifiedSymbolResolution(String^ usr) : m_usr(usr)
{
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::operator == (static)

bool UnifiedSymbolResolution::operator==(UnifiedSymbolResolution^ lhs, UnifiedSymbolResolution^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	return (String::Compare(lhs->m_usr, rhs->m_usr) == 0);
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::operator != (static)

bool UnifiedSymbolResolution::operator!=(UnifiedSymbolResolution^ lhs, UnifiedSymbolResolution^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	return (String::Compare(lhs->m_usr, rhs->m_usr) != 0);
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::String^ (static)

UnifiedSymbolResolution::operator String^(UnifiedSymbolResolution^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return String::Empty;
	return rhs->m_usr;
}
	
//---------------------------------------------------------------------------
// UnifiedSymbolResolution::Create (internal, static)
//
// Creates a new UnifiedSymbolResolution instance
//
// Arguments:
//
//	string		- CXString from which to create the USR

UnifiedSymbolResolution^ UnifiedSymbolResolution::Create(CXString&& string)
{
	// Dispose of the CXString after conversion into System::String
	return gcnew UnifiedSymbolResolution(StringUtil::ToString(std::move(string)));
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::Create (internal, static)
//
// Creates a new UnifiedSymbolResolution instance
//
// Arguments:
//
//	psz			- C-Style string from which to create the USR

UnifiedSymbolResolution^ UnifiedSymbolResolution::Create(const char* psz)
{
	return gcnew UnifiedSymbolResolution(StringUtil::ToString(psz, CP_UTF8));
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::Equals
//
// Compares this UnifiedSymbolResolution instance to another UnifiedSymbolResolution instance
//
// Arguments:
//
//	rhs		- Right-hand UnifiedSymbolResolution instance to compare against

bool UnifiedSymbolResolution::Equals(UnifiedSymbolResolution^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool UnifiedSymbolResolution::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a UnifiedSymbolResolution instance
	UnifiedSymbolResolution^ rhsref = dynamic_cast<UnifiedSymbolResolution^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCCategory (static)
//
// Construct a USR for a specified Objective-C category
//
// Arguments:
//
//	classname		- Objective-C class name
//	categoryname	- Objective-C category name

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCCategory(String^ classname, String^ categoryname)
{
	if(Object::ReferenceEquals(classname, nullptr)) throw gcnew ArgumentNullException("classname");
	if(Object::ReferenceEquals(classname, nullptr)) throw gcnew ArgumentNullException("categoryname");

	char* pszclassname = StringUtil::ToCharPointer(classname, CP_UTF8);

	try {

		char* pszcategoryname = StringUtil::ToCharPointer(categoryname, CP_UTF8);

		try { return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCCategory(pszclassname, pszcategoryname)); }
		finally { StringUtil::FreeCharPointer(pszcategoryname); }
	}

	finally { StringUtil::FreeCharPointer(pszclassname); }
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCClass (static)
//
// Construct a USR for a specified Objective-C class
//
// Arguments:
//
//	classname	- Objective-C class name

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCClass(String^ classname)
{
	if(Object::ReferenceEquals(classname, nullptr)) throw gcnew ArgumentNullException("classname");

	char* pszclassname = StringUtil::ToCharPointer(classname, CP_UTF8);

	try { return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCClass(pszclassname)); }
	finally { StringUtil::FreeCharPointer(pszclassname); }
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCMethod (static)
//
// Construct a USR for a specified Objective-C method

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCMethod(String^ name, bool isinstance, UnifiedSymbolResolution^ classusr)
{
	if(Object::ReferenceEquals(name, nullptr)) throw gcnew ArgumentNullException("name");
	if(Object::ReferenceEquals(classusr, nullptr)) throw gcnew ArgumentNullException("classusr");

	char* pszname = StringUtil::ToCharPointer(name, CP_UTF8);
	try {

		char* pszclassusr = StringUtil::ToCharPointer(classusr, CP_UTF8);
		try {

			// Pass in a fake CXString structure to the API, using private_flags of zero to indicate it's not
			// controlled by clang and doesn't need to be disposed of / released
			return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCMethod(pszname, (isinstance) ? 1 : 0, { pszclassusr, 0 })); 
		} 

		finally { StringUtil::FreeCharPointer(pszclassusr); }
	}

	finally { StringUtil::FreeCharPointer(pszname); }
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCInstanceVariable (static)
//
// Construct a USR for a specified Objective-C instance variable

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCInstanceVariable(String^ name, UnifiedSymbolResolution^ classusr)
{
	if(Object::ReferenceEquals(name, nullptr)) throw gcnew ArgumentNullException("name");
	if(Object::ReferenceEquals(classusr, nullptr)) throw gcnew ArgumentNullException("classusr");

	char* pszname = StringUtil::ToCharPointer(name, CP_UTF8);
	try {

		char* pszclassusr = StringUtil::ToCharPointer(classusr, CP_UTF8);
		try {

			// Pass in a fake CXString structure to the API, using private_flags of zero to indicate it's not
			// controlled by clang and doesn't need to be disposed of / released
			return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCIvar(pszname, { pszclassusr, 0 }));
		} 

		finally { StringUtil::FreeCharPointer(pszclassusr); }
	}

	finally { StringUtil::FreeCharPointer(pszname); }	
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCProperty (static)
//
// Construct a USR for a specified Objective-C property

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCProperty(String^ name, UnifiedSymbolResolution^ classusr)
{
	if(Object::ReferenceEquals(name, nullptr)) throw gcnew ArgumentNullException("name");
	if(Object::ReferenceEquals(classusr, nullptr)) throw gcnew ArgumentNullException("classusr");

	char* pszname = StringUtil::ToCharPointer(name, CP_UTF8);
	try {

		char* pszclassusr = StringUtil::ToCharPointer(classusr, CP_UTF8);
		try {

			// Pass in a fake CXString structure to the API, using private_flags of zero to indicate it's not
			// controlled by clang and doesn't need to be disposed of / released
			return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCProperty(pszname, { pszclassusr, 0 }));
		} 

		finally { StringUtil::FreeCharPointer(pszclassusr); }
	}

	finally { StringUtil::FreeCharPointer(pszname); }	
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::FromObjectiveCProtocol (static)
//
// Construct a USR for a specified Objective-C protocol
//
// Arguments:
//
//	protocolname	- Objective-C protocol name

UnifiedSymbolResolution^ UnifiedSymbolResolution::FromObjectiveCProtocol(String^ protocolname)
{
	if(Object::ReferenceEquals(protocolname, nullptr)) throw gcnew ArgumentNullException("protocolname");

	char* pszprotocolname = StringUtil::ToCharPointer(protocolname, CP_UTF8);

	try { return UnifiedSymbolResolution::Create(clang_constructUSR_ObjCProtocol(pszprotocolname)); }
	finally { StringUtil::FreeCharPointer(pszprotocolname); }
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int UnifiedSymbolResolution::GetHashCode(void)
{
	return m_usr->GetHashCode();
}

//---------------------------------------------------------------------------
// UnifiedSymbolResolution::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ UnifiedSymbolResolution::ToString(void)
{
	return m_usr;
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
