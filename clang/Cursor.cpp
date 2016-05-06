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
#include "Cursor.h"

#include "ArgumentCursorCollection.h"
#include "AutoGCHandle.h"
#include "Availability.h"
#include "CompletionString.h"
#include "CursorCollection.h"
#include "CursorComment.h"
#include "CursorKind.h"
#include "CursorVisibility.h"
#include "CxxAccessSpecifier.h"
#include "EnumConstant.h"
#include "EnumerateChildrenFunc.h"
#include "EnumerateChildrenResult.h"
#include "EnumerateReferencesFunc.h"
#include "EnumerateReferencesResult.h"
#include "Extent.h"
#include "ExtentCollection.h"
#include "File.h"
#include "GCHandleRef.h"
#include "Language.h"
#include "Linkage.h"
#include "Location.h"
#include "Module.h"
#include "ObjectiveCDeclarationQualifier.h"
#include "ObjectiveCPropertyAttribute.h"
#include "OverloadedDeclarationCursorCollection.h"
#include "OverriddenCursorCollection.h"
#include "ParsedComment.h"
#include "PlatformAvailabilityCollection.h"
#include "ReferenceNameExtent.h"
#include "StorageClass.h"
#include "StringCollection.h"
#include "StringUtil.h"
#include "TemplateArgumentCollection.h"
#include "TokenCollection.h"
#include "Type.h"
#include "UnifiedSymbolResolution.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// EnumerateChildrenCallback (local)
//
// Callback for clang_visitChildren() that invokes a delegate function against
// each cursor instance that is returned
//
// Arguments:
//
//	cursor			- Current unmanaged CXCursor instance being enumerated
//	parent			- Parent CXCursor instance (unused)
//	context			- Context pointer passed into clang_visitChildren

static CXChildVisitResult EnumerateChildrenCallback(CXCursor cursor, CXCursor parent, CXClientData context)
{
	GCHandleRef<EnumerateChildrenFunc^> func(context);

	// Invoke the delegate and capture any exception that occurs
	try { return func->Invoke(cursor, parent); }
	catch(Exception^ exception) { func->Exception = exception; return CXChildVisitResult::CXChildVisit_Break; }
}

//---------------------------------------------------------------------------
// EnumerateReferencesCallback (local)
//
// Callback for clang_findReferencesInFile() that invokes a delegate function against
// each included cursor/extent pair that is returned
//
// Arguments:
//
//	conext			- Context object passed into the enumeration function
//	cursor			- Cursor instance for the enumerated directive
//	extent			- Extent instance for the enumerated directive

static CXVisitorResult EnumerateReferencesCallback(void* context, CXCursor cursor, CXSourceRange extent)
{
	GCHandleRef<EnumerateReferencesFunc^> func(context);

	// Invoke the delegate and capture any exception that occurs
	try { return func->Invoke(cursor, extent); }
	catch(Exception^ exception) { func->Exception = exception; return CXVisitorResult::CXVisit_Break; }
}

//---------------------------------------------------------------------------
// Cursor Static Constructor (private)

static Cursor::Cursor()
{
	Null = Cursor::Create(TranslationUnitHandle::Null, TranslationUnitHandle::Null, clang_getNullCursor());
}

//---------------------------------------------------------------------------
// Cursor Constructor (protected)
//
// Arguments:
//
//	handle		- Underlying CursorHandle instance

Cursor::Cursor(CursorHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Cursor::operator == (static)

bool Cursor::operator==(Cursor^ lhs, Cursor^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return true;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return false;

	// clang_equalCursors returns non-zero if the cursors are the same
	return (clang_equalCursors(CursorHandle::Reference(lhs->m_handle), CursorHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::operator != (static)

bool Cursor::operator!=(Cursor^ lhs, Cursor^ rhs)
{
	if(Object::ReferenceEquals(lhs, rhs)) return false;
	if(Object::ReferenceEquals(lhs, nullptr) || Object::ReferenceEquals(rhs, nullptr)) return true;

	// clang_equalCursors returns zero if the cursors are different
	return (clang_equalCursors(CursorHandle::Reference(lhs->m_handle), CursorHandle::Reference(rhs->m_handle)) == 0);
}

//---------------------------------------------------------------------------
// Cursor::ArgumentCursors::get
//
// Gets a collection of non-variadic argument cursors

CursorCollection^ Cursor::ArgumentCursors::get(void)
{
	if(Object::ReferenceEquals(m_arguments, nullptr)) 
		m_arguments = ArgumentCursorCollection::Create(m_handle->Owner, m_handle->TranslationUnit, CursorHandle::Reference(m_handle));

	return m_arguments;
}

//---------------------------------------------------------------------------
// Cursor::Availability::get
//
// Gets the availability of the entity that this cursor refers to

local::Availability Cursor::Availability::get(void)
{
	return local::Availability(clang_getCursorAvailability(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::BriefComment::get
//
// Gets the brief comment for this cursor

String^ Cursor::BriefComment::get(void)
{
	if(Object::ReferenceEquals(m_briefcomment, nullptr))
		m_briefcomment = StringUtil::ToString(clang_Cursor_getBriefCommentText(CursorHandle::Reference(m_handle)));

	return m_briefcomment;
}

//---------------------------------------------------------------------------
// Cursor::CanonicalCursor::get
//
// Gets the canonical cursor for the referred entity

Cursor^ Cursor::CanonicalCursor::get(void)
{
	if(Object::ReferenceEquals(m_canonical, nullptr))
		m_canonical = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCanonicalCursor(CursorHandle::Reference(m_handle)));

	return m_canonical;
}

//---------------------------------------------------------------------------
// Cursor::Comment::get
//
// Gets the raw comment for this cursor

CursorComment^ Cursor::Comment::get(void)
{
	if(Object::ReferenceEquals(m_rawcomment, nullptr)) 
		m_rawcomment = CursorComment::Create(m_handle->Owner, m_handle->TranslationUnit, CursorHandle::Reference(m_handle));

	return m_rawcomment;
}

//---------------------------------------------------------------------------
// Cursor::CompletionString::get
//
// Gets a completion string for an arbitrary declaration or macro definition cursor

local::CompletionString^ Cursor::CompletionString::get(void)
{
	if(Object::ReferenceEquals(m_completion, nullptr))
		m_completion = local::CompletionString::Create(m_handle->Owner, clang_getCursorCompletionString(CursorHandle::Reference(m_handle)));

	return m_completion;
}

//---------------------------------------------------------------------------
// Cursor::Create (internal, static)
//
// Creates a new Cursor instance
//
// Arguments:
//
//	owner		- Owning safe handle instance
//	transunit	- TranslationUnit handle instance
//	cursor		- Unmanaged CXCursor instance

Cursor^ Cursor::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor)
{
	return gcnew Cursor(gcnew CursorHandle(owner, transunit, cursor));
}

//---------------------------------------------------------------------------
// Cursor::CxxAccessSpecifier::get
//
// Gets the access specifier of the entity pointed at by the cursor

local::CxxAccessSpecifier Cursor::CxxAccessSpecifier::get(void)
{
	return local::CxxAccessSpecifier(clang_getCXXAccessSpecifier(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::CxxMangledSymbols::get
//
// Gets the strings representing the mangled symbols of a C++ constructor or destructor

StringCollection^ Cursor::CxxMangledSymbols::get(void)
{
	if(Object::ReferenceEquals(m_manglings, nullptr))
		m_manglings = StringCollection::Create(clang_Cursor_getCXXManglings(CursorHandle::Reference(m_handle)));

	return m_manglings;
}

//---------------------------------------------------------------------------
// Cursor::DefinitionCursor::get
//
// Gets a cursor that describes the definition of the entity

Cursor^ Cursor::DefinitionCursor::get(void)
{
	if(Object::ReferenceEquals(m_definition, nullptr))
		m_definition = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorDefinition(CursorHandle::Reference(m_handle)));

	return m_definition;
}

//---------------------------------------------------------------------------
// Cursor::DisplayName::get
//
// Gets the display name for the entity referenced by this cursor

String^ Cursor::DisplayName::get(void)
{
	if(Object::ReferenceEquals(m_displayname, nullptr))
		m_displayname = StringUtil::ToString(clang_getCursorDisplayName(CursorHandle::Reference(m_handle)));

	return m_displayname;
}

//---------------------------------------------------------------------------
// Cursor::EnumConstant::get
//
// Gets the numeric value of an enumeration constant

local::EnumConstant Cursor::EnumConstant::get(void)
{
	CXCursor cursor = CursorHandle::Reference(m_handle);
	return local::EnumConstant(cursor);
}

//---------------------------------------------------------------------------
// Cursor::EnumerateChildren
//
// Enumerate descendant cursors of this cursor
//
// Arguments:
//
//	func	- Delegate function to invoke for every enumerated cursor

void Cursor::EnumerateChildren(Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func)
{
	// If the cursor or delegate function are null, there is nothing to do
	if((IsNull(this)) || (Object::ReferenceEquals(func, nullptr))) return;

	CursorHandle::Reference cursor(m_handle);		// Keep the translation unit alive

	// Use a wrapper object for the cursor handle and callback delegate
	EnumerateChildrenFunc^ enumfunc = EnumerateChildrenFunc::Create(m_handle->Owner, m_handle->TranslationUnit, cursor, func);
	clang_visitChildren(cursor, EnumerateChildrenCallback, AutoGCHandle(enumfunc));

	// Check if an exception occurred during enumeration and re-throw it
	Exception^ exception = enumfunc->Exception;
	if(!Object::ReferenceEquals(exception, nullptr)) throw exception;
}
	
//---------------------------------------------------------------------------
// Cursor::EnumerateReferences
//
// Enumerates references to this cursor in a specific file
//
// Arguments:
//
//	filename	- File in which to search for the directives
//	func		- Delegate function to invoke for every enumerated cursor/extent

void Cursor::EnumerateReferences(String^ filename, Func<Cursor^, local::Extent^, EnumerateReferencesResult>^ func)
{
	if(Object::ReferenceEquals(filename, nullptr)) throw gcnew ArgumentNullException("filename");
	if(Object::ReferenceEquals(func, nullptr)) return;

	CursorHandle::Reference cursor(m_handle);			// Keep the translation unit alive

	// This operation accepts a string file name rather than a File object so that the internal
	// FileHandle doesn't need to be exposed; use clang_getFile instead to retrieve a CXFile
	char* pszname = StringUtil::ToCharPointer(filename, CP_UTF8);

	try {

		// Use a wrapper object for the cursor handle and callback delegate
		EnumerateReferencesFunc^ enumfunc = EnumerateReferencesFunc::Create(m_handle->Owner, m_handle->TranslationUnit, cursor, func);
		AutoGCHandle handle(enumfunc);
		clang_findReferencesInFile(cursor, clang_getFile(cursor.TranslationUnit, pszname), { handle, EnumerateReferencesCallback });

		// Check if an exception occurred during enumeration and re-throw it
		Exception^ exception = enumfunc->Exception;
		if(!Object::ReferenceEquals(exception, nullptr)) throw exception;
	}

	finally { StringUtil::FreeCharPointer(pszname); }
}

//---------------------------------------------------------------------------
// Cursor::EnumType::get
//
// Gets the integer type of an enum declaration

local::Type^ Cursor::EnumType::get(void)
{
	if(Object::ReferenceEquals(m_enumtype, nullptr))
		m_enumtype = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getEnumDeclIntegerType(CursorHandle::Reference(m_handle)));

	return m_enumtype;
}

//---------------------------------------------------------------------------
// Cursor::Equals
//
// Compares this Cursor instance to another Cursor instance
//
// Arguments:
//
//	rhs		- Right-hand Cursor instance to compare against

bool Cursor::Equals(Cursor^ rhs)
{
	return (this == rhs);
}

//---------------------------------------------------------------------------
// Cursor::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool Cursor::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a Cursor instance
	Cursor^ rhsref = dynamic_cast<Cursor^>(rhs);
	if(rhsref == nullptr) return false;

	return (this == rhsref);
}

//---------------------------------------------------------------------------
// Cursor::Extent::get
//
// Gets the source range occupied by the entity pointed at by the cursor

local::Extent^ Cursor::Extent::get(void)
{
	if(Object::ReferenceEquals(m_extent, nullptr)) 
		m_extent = local::Extent::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorExtent(CursorHandle::Reference(m_handle)));

	return m_extent;
}

//---------------------------------------------------------------------------
// Cursor::FieldBitWidth::get
//
// Gets the width of a bit field declaration

Nullable<int> Cursor::FieldBitWidth::get(void)
{
	int value = clang_getFieldDeclBitWidth(CursorHandle::Reference(m_handle));
	return (value < 0) ? Nullable<int>() : Nullable<int>(value);
}
	
//---------------------------------------------------------------------------
// Cursor::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int Cursor::GetHashCode(void)
{
	return static_cast<int>(clang_hashCursor(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::GetReferenceNameExtents
//
// Creates a collection of extents that covers part of the referenced name
//
// Arguments:
//
//	flags		- Flags to control the name retrieval

ExtentCollection^ Cursor::GetReferenceNameExtents(ReferenceNameExtent flags)
{
	List<local::Extent^>^ extents = gcnew List<local::Extent^>();

	CursorHandle::Reference cursor(m_handle);		// Keep the translation unit alive

	// In order to provide a collection and not require the caller to pass in the piece index,
	// collect all of the piece extents and wrap an ExtentCollection around them
	unsigned int piece = 0;
	CXSourceRange extent = clang_getCursorReferenceNameRange(cursor, static_cast<unsigned int>(flags), piece++);
	while(clang_Range_isNull(extent) == 0) {

		extents->Add(local::Extent::Create(m_handle->Owner, m_handle->TranslationUnit, extent));
		extent = clang_getCursorReferenceNameRange(cursor, static_cast<unsigned int>(flags), piece++);
	}

	return ExtentCollection::Create(extents);
}

//---------------------------------------------------------------------------
// Cursor::GetSpellingNameExtents
//
// Creates a collection of extents that cover part of the spelling (Objective-C)
//
// NOTE: This is a method rather than a property because in the future it may behave
// like clang_getCursorReferenceNameRange and allow/require flags to be provided
//
// Arguments:
//
//	NONE

ExtentCollection^ Cursor::GetSpellingNameExtents(void)
{
	List<local::Extent^>^ extents = gcnew List<local::Extent^>();

	CursorHandle::Reference cursor(m_handle);		// Keep the translation unit alive

	// In order to provide a collection and not require the caller to pass in the piece index,
	// collect all of the piece extents and wrap an ExtentCollection around them
	unsigned int piece = 0;
	CXSourceRange extent = clang_Cursor_getSpellingNameRange(cursor, piece++, /* reserved */ 0);
	while(clang_Range_isNull(extent) == 0) {

		extents->Add(local::Extent::Create(m_handle->Owner, m_handle->TranslationUnit, extent));
		extent = clang_Cursor_getSpellingNameRange(cursor, piece++, /* reserved */ 0);
	}

	return ExtentCollection::Create(extents);
}

//---------------------------------------------------------------------------
// Cursor::IBOutletCollectionType::get
//
// For cursors representing an iboutletcollection attribute, the collection element type

local::Type^ Cursor::IBOutletCollectionType::get(void)
{
	if(Object::ReferenceEquals(m_iboctype, nullptr))
		m_iboctype = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getIBOutletCollectionType(CursorHandle::Reference(m_handle)));

	return m_iboctype;
}

//---------------------------------------------------------------------------
// Cursor::ImportModule::get
//
// Gets the module associated with a ModuleImportDecl cursor

Module^ Cursor::ImportModule::get(void)
{
	if(Object::ReferenceEquals(m_importmodule, nullptr))
		m_importmodule = Module::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Cursor_getModule(CursorHandle::Reference(m_handle)));

	return m_importmodule;
}

//---------------------------------------------------------------------------
// Cursor::IncludedFile::get
//
// Gets the file that is included by the cursor

File^ Cursor::IncludedFile::get(void)
{
	if(Object::ReferenceEquals(m_includedfile, nullptr))
		m_includedfile = File::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getIncludedFile(CursorHandle::Reference(m_handle)));

	return m_includedfile;
}

//---------------------------------------------------------------------------
// Cursor::IsAnonymousRecord
//
// Gets a flag indicating if this cursor is an anonymous record

bool Cursor::IsAnonymousRecord::get(void)
{
	return (clang_Cursor_isAnonymous(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsBitFieldMember
//
// Gets a flag indicating if this cursor is a bitfield member of a record

bool Cursor::IsBitFieldMember::get(void)
{
	return (clang_Cursor_isBitField(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCanonical::get
//
// Gets a flag indicating if this cursor is the canonical cursor for the entity

bool Cursor::IsCanonical::get(void)
{
	CursorHandle::Reference cursor(m_handle);		// Unwrap the safe handle

	// There is no clang API to determine this, get the canonical cursor
	// reference and compare it with this cursor
	CXCursor canonical = clang_getCanonicalCursor(cursor);
	return (clang_equalCursors(cursor, canonical) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCxxConstMethod::get
//
// Gets a flag indicating if a C++ member function or member function template is declared 'const'

bool Cursor::IsCxxConstMethod::get(void)
{
	return (clang_CXXMethod_isConst(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCxxMutableField::get
//
// Gets a flag indicating if a C++ field is declared 'mutable'

bool Cursor::IsCxxMutableField::get(void)
{
	return (clang_CXXField_isMutable(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCxxPureVirtualMethod::get
//
// Gets a flag indicating if a C++ member function or member function template is pure virtual

bool Cursor::IsCxxPureVirtualMethod::get(void)
{
	return (clang_CXXMethod_isPureVirtual(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCxxStaticMethod::get
//
// Gets a flag indicating if a C++ member function or member function template is declared 'static'

bool Cursor::IsCxxStaticMethod::get(void)
{
	return (clang_CXXMethod_isStatic(CursorHandle::Reference(m_handle)) != 0);

}

//---------------------------------------------------------------------------
// Cursor::IsCxxVirtualBase::get
//
// Gets a flag indicating if the cursor is a virtual base class

bool Cursor::IsCxxVirtualBase::get(void)
{
	return (clang_isVirtualBase(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsCxxVirtualMethod::get
//
// Gets a flag indicating if a C++ member function or member function template is virtual

bool Cursor::IsCxxVirtualMethod::get(void)
{
	return (clang_CXXMethod_isVirtual(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsDefinition::get
//
// Gets a flag indicating if the declaration pointed to by this cursor is also a definition of that entity

bool Cursor::IsDefinition::get(void)
{
	return (clang_isCursorDefinition(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsDynamicCall::get
//
// Gets a flag indicating if the given cursor is a dynamic call

bool Cursor::IsDynamicCall::get(void)
{
	return (clang_Cursor_isDynamicCall(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsObjectiveCOptional::get
//
// Gets a flag indicating if the declaration was affected by "@optional"

bool Cursor::IsObjectiveCOptional::get(void)
{
	return (clang_Cursor_isObjCOptional(CursorHandle::Reference(m_handle)) != 0);
}
	
//---------------------------------------------------------------------------
// Cursor::IsVariadic::get
//
// Gets a flag indicating if the given cursor is a variadic function or method

bool Cursor::IsVariadic::get(void)
{
	return (clang_Cursor_isVariadic(CursorHandle::Reference(m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::IsNull (static)
//
// Determines if the cursor is null
//
// Arguments:
//
//	rhs		- Cursor instance to be checked for null

bool Cursor::IsNull(Cursor^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) throw gcnew ArgumentNullException("rhs");
	return (clang_Cursor_isNull(CursorHandle::Reference(rhs->m_handle)) != 0);
}

//---------------------------------------------------------------------------
// Cursor::Kind::get
//
// Gets the kind of this cursor

CursorKind Cursor::Kind::get(void)
{
	return CursorKind(clang_getCursorKind(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::Language::get
//
// Gets the language of the entity referred to by a given cursor

local::Language Cursor::Language::get(void)
{
	return local::Language(clang_getCursorLanguage(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::LexicalParentCursor::get
//
// Gets the lexical parent cursor of this cursor

Cursor^ Cursor::LexicalParentCursor::get(void)
{
	if(Object::ReferenceEquals(m_lexical, nullptr))
		m_lexical = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorLexicalParent(CursorHandle::Reference(m_handle)));

	return m_lexical;
}

//---------------------------------------------------------------------------
// Cursor::Linkage::get
//
// Gets the the linkage of the entity referred to by the cursor

local::Linkage Cursor::Linkage::get(void)
{
	return local::Linkage(clang_getCursorLinkage(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::Location::get
//
// Gets the location of the cursor in the translation unit

local::Location^ Cursor::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorLocation(CursorHandle::Reference(m_handle)), LocationKind::Spelling);
	return m_location;
}

//---------------------------------------------------------------------------
// Cursor::MangledName::get
//
// Gets the mangled name for the entity referenced by this cursor

String^ Cursor::MangledName::get(void)
{
	if(Object::ReferenceEquals(m_mangled, nullptr))
		m_mangled = StringUtil::ToString(clang_Cursor_getMangling(CursorHandle::Reference(m_handle)));

	return m_mangled;
}

//---------------------------------------------------------------------------
// Cursor::MessageReceiverType::get
//
// Gets the type of an Objective-C message receiver

local::Type^ Cursor::MessageReceiverType::get(void)
{
	if(Object::ReferenceEquals(m_msgreceiver, nullptr))
		m_msgreceiver = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Cursor_getReceiverType(CursorHandle::Reference(m_handle)));

	return m_msgreceiver;
}

//---------------------------------------------------------------------------
// Cursor::ObjectiveCDeclarationQualifiers::get
//
// Gets the Objective-C declaration qualifier flags

ObjectiveCDeclarationQualifier Cursor::ObjectiveCDeclarationQualifiers::get(void)
{
	return ObjectiveCDeclarationQualifier(clang_Cursor_getObjCDeclQualifiers(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::ObjectiveCPropertyAttributes::get
//
// Gets the Objective-C property attributes

ObjectiveCPropertyAttribute Cursor::ObjectiveCPropertyAttributes::get(void)
{
	return ObjectiveCPropertyAttribute(clang_Cursor_getObjCPropertyAttributes(CursorHandle::Reference(m_handle), /* reserved */ 0));
}

//---------------------------------------------------------------------------
// Cursor::ObjectiveCSelectorIndex::get
//
// Gets the Objective-C selector index

Nullable<int> Cursor::ObjectiveCSelectorIndex::get(void)
{
	// clang_Cursor_getObjCSelectorIndex can return a negative value if this operation
	// isn't valid for this cursor, in that case return a null Nullable<int>
	int value = clang_Cursor_getObjCSelectorIndex(CursorHandle::Reference(m_handle));
	return (value >= 0) ? Nullable<int>(value) : Nullable<int>();
}

//---------------------------------------------------------------------------
// Cursor::ObjectiveCTypeEncoding::get
//
// Gets the Objective-C type encoding

String^ Cursor::ObjectiveCTypeEncoding::get(void)
{
	if(Object::ReferenceEquals(m_objcencoding, nullptr))
		m_objcencoding = StringUtil::ToString(clang_getDeclObjCTypeEncoding(CursorHandle::Reference(m_handle)));

	return m_objcencoding;
}

//---------------------------------------------------------------------------
// Cursor::OverloadedDeclarationCursors::get
//
// Gets a collection of overloaded declaration cursors

CursorCollection^ Cursor::OverloadedDeclarationCursors::get(void)
{
	if(Object::ReferenceEquals(m_overloadeddecls, nullptr))
		m_overloadeddecls = OverloadedDeclarationCursorCollection::Create(m_handle->Owner, m_handle->TranslationUnit, CursorHandle::Reference(m_handle));

	return m_overloadeddecls;
}

//---------------------------------------------------------------------------
// Cursor::OverriddenCursors::get
//
// Gets a collection of cursors overridden by this cursor

CursorCollection^ Cursor::OverriddenCursors::get(void)
{
	if(Object::ReferenceEquals(m_overridden, nullptr)) {

		CXCursor*			cursors;		// Unmanaged array of CXCursors
		unsigned int		count;			// Size of the unmanaged array

		// CursorCollection does not take ownership of the array, it has to be disposed of here
		clang_getOverriddenCursors(CursorHandle::Reference(m_handle), &cursors, &count);
		try { m_overridden = OverriddenCursorCollection::Create(m_handle->Owner, m_handle->TranslationUnit, cursors, count); }
		finally { clang_disposeOverriddenCursors(cursors); }
	}

	return m_overridden;
}

//---------------------------------------------------------------------------
// Cursor::ParsedComment::get
//
// Gets the fully parsed comment/documentation for this cursor

local::ParsedComment^ Cursor::ParsedComment::get(void)
{
	if(Object::ReferenceEquals(m_fullcomment, nullptr))
		m_fullcomment = local::ParsedComment::Create(m_handle->Owner, m_handle->TranslationUnit, clang_Cursor_getParsedComment(CursorHandle::Reference(m_handle)));

	return m_fullcomment;
}

//---------------------------------------------------------------------------
// Cursor::PlatformAvailability::get
//
// Gets a collection indicating the platform availability of the entity

PlatformAvailabilityCollection^ Cursor::PlatformAvailability::get(void)
{
	if(Object::ReferenceEquals(m_platavail, nullptr)) 
		m_platavail = PlatformAvailabilityCollection::Create(m_handle->Owner, CursorHandle::Reference(m_handle));

	return m_platavail;
}

//---------------------------------------------------------------------------
// Cursor::RecordOffset::get
//
// Gets the offset of a record field cursor

Nullable<__int64> Cursor::RecordOffset::get(void)
{
	// clang_Cursor_getOffsetOfField can return a negative value for "not valid"
	long long value = clang_Cursor_getOffsetOfField(CursorHandle::Reference(m_handle));
	return (value < 0) ? Nullable<__int64>() : Nullable<__int64>(value);
}

//---------------------------------------------------------------------------
// Cursor::ReferencedCursor::get
//
// Gets a cursor representing the entity that this cursor refers to

Cursor^ Cursor::ReferencedCursor::get(void)
{
	if(Object::ReferenceEquals(m_referenced, nullptr))
		m_referenced = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorReferenced(CursorHandle::Reference(m_handle)));

	return m_referenced;
}

//---------------------------------------------------------------------------
// Cursor::ResultType::get
//
// Gets the result type of a function or method cursor

local::Type^ Cursor::ResultType::get(void)
{
	if(Object::ReferenceEquals(m_resulttype, nullptr))
		m_resulttype = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorResultType(CursorHandle::Reference(m_handle)));

	return m_resulttype;
}

//---------------------------------------------------------------------------
// Cursor::SemanticParentCursor::get
//
// Gets the semantic parent cursor of this cursor

Cursor^ Cursor::SemanticParentCursor::get(void)
{
	if(Object::ReferenceEquals(m_semparent, nullptr))
		m_semparent = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorSemanticParent(CursorHandle::Reference(m_handle)));

	return m_semparent;
}

//---------------------------------------------------------------------------
// Cursor::SpecializedTemplateCursor::get
//
// Gets the the cursor that represents the template that it specializes

Cursor^ Cursor::SpecializedTemplateCursor::get(void)
{
	if(Object::ReferenceEquals(m_specialization, nullptr))
		m_specialization = Cursor::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getSpecializedCursorTemplate(CursorHandle::Reference(m_handle)));

	return m_specialization;
}
	
//---------------------------------------------------------------------------
// Cursor::Spelling::get
//
// Gets the spelling of the entity pointed at by the cursor

String^ Cursor::Spelling::get(void)
{
	if(Object::ReferenceEquals(m_spelling, nullptr))
		m_spelling = StringUtil::ToString(clang_getCursorSpelling(CursorHandle::Reference(m_handle)));

	return m_spelling;
}

//---------------------------------------------------------------------------
// Cursor::StorageClass::get
//
// Gets the storage class of the entity pointed at by the cursor

local::StorageClass Cursor::StorageClass::get(void)
{
	return local::StorageClass(clang_Cursor_getStorageClass(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::GetTemplateArguments::get
//
// Gets a collection of template arguments

TemplateArgumentCollection^ Cursor::TemplateArguments::get(void)
{
	if(Object::ReferenceEquals(m_templargs, nullptr)) 
		m_templargs = TemplateArgumentCollection::Create(m_handle->Owner, m_handle->TranslationUnit, CursorHandle::Reference(m_handle));

	return m_templargs;
}

//---------------------------------------------------------------------------
// Cursor::TemplateCursorKind::get
//
// Gets the kind of the cursor kind of the specializations would be generated by instantiating the template

CursorKind Cursor::TemplateCursorKind::get(void)
{
	return CursorKind(clang_getTemplateCursorKind(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Cursor::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Cursor::ToString(void)
{
	return (IsNull(this)) ? "<null>" : this->DisplayName;
}

//---------------------------------------------------------------------------
// Cursor::Type::get
//
// Gets the type of the cursor

local::Type^ Cursor::Type::get(void)
{
	if(Object::ReferenceEquals(m_type, nullptr))
		m_type = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getCursorType(CursorHandle::Reference(m_handle)));

	return m_type;
}

//---------------------------------------------------------------------------
// Cursor::UnderlyingTypedefType::get
//
// Gets the underlying type of a typedef cursor

local::Type^ Cursor::UnderlyingTypedefType::get(void)
{
	if(Object::ReferenceEquals(m_underlying, nullptr))
		m_underlying = local::Type::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getTypedefDeclUnderlyingType(CursorHandle::Reference(m_handle)));

	return m_underlying;
}

//---------------------------------------------------------------------------
// Cursor::UnifiedSymbolResolution::get
//
// Gets the Unified Symbol Resultion (USR) string for the entity

local::UnifiedSymbolResolution^ Cursor::UnifiedSymbolResolution::get(void)
{
	if(Object::ReferenceEquals(m_usr, nullptr))
		m_usr = local::UnifiedSymbolResolution::Create(clang_getCursorUSR(CursorHandle::Reference(m_handle)));

	return m_usr;
}
	
//---------------------------------------------------------------------------
// Cursor::Visibility::get
//
// Gets the visibility of the entity referred to by a cursor

CursorVisibility Cursor::Visibility::get(void)
{
	return CursorVisibility(clang_getCursorVisibility(CursorHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
