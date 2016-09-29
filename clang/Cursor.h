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

#ifndef __CURSOR_H_
#define __CURSOR_H_
#pragma once

#include "TranslationUnitReferenceHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

namespace zuki::tools::llvm::clang {
namespace local = zuki::tools::llvm::clang;

// FORWARD DECLARATIONS
//
enum class	Availability;
ref class	CompletionString;
ref class	CursorCollection;
ref class	CursorComment;
value class CursorKind;
enum class	CursorVisibility;
enum class	CxxAccessSpecifier;
value class EnumConstant;
value class EnumerateChildrenResult;
value class EnumerateReferencesResult;
ref class	EvaluationResult;
ref class	Extent;
ref class	ExtentCollection;
ref class	File;
enum class	Language;
enum class	Linkage;
ref class	Location;
ref class	Module;
enum class	ObjectiveCDeclarationQualifier;
enum class	ObjectiveCPropertyAttribute;
ref class	ParsedComment;
ref class	PlatformAvailabilityCollection;
enum class	ReferenceNameExtent;
enum class	StorageClass;
ref class	StringCollection;
ref class	TemplateArgumentCollection;
ref class	TokenCollection;
ref class	Type;
ref class	UnifiedSymbolResolution;

//---------------------------------------------------------------------------
// Class Cursor
//
// Represents a reference to an element within the abstract syntax tree
//---------------------------------------------------------------------------

public ref class Cursor
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(Cursor^ lhs, Cursor^ rhs);

	// operator!= (static)
	//
	static bool operator!=(Cursor^ lhs, Cursor^ rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// EnumerateChildren
	//
	// Enumerates descendant cursors of this cursor
	void EnumerateChildren(Func<Cursor^, Cursor^, EnumerateChildrenResult>^ func);

	// EnumerateReferences
	//
	// Enumerates references to this cursor in a specific file
	void EnumerateReferences(String^ filename, Func<Cursor^, local::Extent^, EnumerateReferencesResult>^ func);

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this Cursor instance to another Cursor instance
	bool Equals(Cursor^ rhs);

	// Evaluate
	//
	//
	EvaluationResult^ Evaluate(void);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// GetReferenceNameExtents
	//
	// Creates a collection of extents that covers part of the referenced name
	ExtentCollection^ GetReferenceNameExtents(ReferenceNameExtent flags);

	// GetSpellingNameExtents
	//
	// Creates a collection of extent that covers part of the spelling (Objective-C)
	ExtentCollection^ GetSpellingNameExtents(void);

	// IsNull (static)
	//
	// Determines if the specified cursor object is null
	static bool IsNull(Cursor^ rhs);

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Properties

	// ArgumentCursors
	//
	// Gets a collection of non-variadic argument cursors
	property CursorCollection^ ArgumentCursors
	{
		CursorCollection^ get(void);
	}

	// Availability
	//
	// Gets the availability of the entity that this cursor refers to
	property local::Availability Availability
	{
		local::Availability get(void);
	}

	// BriefComment
	//
	// Gets the brief comment for this cursor
	property String^ BriefComment
	{
		String^ get(void);
	}

	// CanonicalCursor
	//
	// Gets the canonical cursor for the referred entity
	property Cursor^ CanonicalCursor
	{
		Cursor^ get(void);
	}

	// Comment
	//
	// Gets the raw comment for this cursor
	property CursorComment^ Comment
	{
		CursorComment^ get(void);
	}

	// CompletionString
	//
	// Gets a completion string for an arbitrary declaration or macro definition cursor
	property local::CompletionString^ CompletionString
	{
		local::CompletionString^ get(void);
	}

	// CxxAccessSpecifier
	//
	// Gets the access specifier of the entity pointed at by the cursor
	property local::CxxAccessSpecifier CxxAccessSpecifier
	{
		local::CxxAccessSpecifier get(void);
	}

	// CxxMangledSymbols
	//
	// Gets the strings representing the mangled symbols of a C++ constructor or destructor
	property StringCollection^ CxxMangledSymbols
	{
		StringCollection^ get(void);
	}

	// DefinitionCursor
	//
	// Gets a cursor that describes the definition of the entity
	property Cursor^ DefinitionCursor
	{
		Cursor^ get(void);
	}

	// DisplayName
	//
	// Gets the display name for the entity referenced by this cursor
	property String^ DisplayName
	{
		String^ get(void);
	}

	// EnumConstant
	//
	// Gets the numeric value of an enumeration constant
	property local::EnumConstant EnumConstant
	{
		local::EnumConstant get(void);
	}

	// EnumType
	//
	// Gets the integer type of an enum declaration
	property local::Type^ EnumType
	{
		local::Type^ get(void);
	}

	// Extent
	//
	// Gets the source range occupied by the entity pointed at by the cursor
	property local::Extent^ Extent
	{
		local::Extent^ get(void);
	}

	// FieldBitWidth
	//
	// Gets the width of a bit field declaration
	property Nullable<int> FieldBitWidth
	{
		Nullable<int> get(void);
	}

	// HasAttributes
	//
	// Gets a flag indicating if this cursor has attributes
	property bool HasAttributes
	{
		bool get(void);
	}

	// IBOutletCollectionType
	//
	// For cursors representing an iboutletcollection attribute, the collection element type
	property local::Type^ IBOutletCollectionType
	{
		local::Type^ get(void);
	}

	// ImportModule
	//
	// Gets the module associated with a ModuleImportDecl cursor
	property Module^ ImportModule
	{
		Module^ get(void);
	}

	// IncludedFile
	//
	// Gets the file that is included by the cursor
	property File^ IncludedFile
	{
		File^ get(void);
	}

	// IsAnonymousRecord
	//
	// Gets a flag indicating if this cursor is an anonymous record
	property bool IsAnonymousRecord
	{
		bool get(void);
	}

	// IsBitFieldMember
	//
	// Gets a flag indicating if this cursor is a bit field member of a record
	property bool IsBitFieldMember
	{
		bool get(void);
	}

	// IsCanonical
	//
	// Gets a flag indicating if this cursor is the canonical cursor for the entity
	property bool IsCanonical
	{
		bool get(void);
	}

	// IsCxxConstMethod
	//
	// Gets a flag indicating if a C++ member function or member function template is declared 'const'
	property bool IsCxxConstMethod
	{
		bool get(void);
	}

	// IsCxxConvertingConstructor
	//
	// Gets a flag indicating if a C++ constructor is a converting constructor
	property bool IsCxxConvertingConstructor
	{
		bool get(void);
	}

	// IsCxxCopyConstructor
	//
	// Gets a flag indicating if a C++ constructor is a copy constructor
	property bool IsCxxCopyConstructor
	{
		bool get(void);
	}

	// IsCxxDefaultConstructor
	//
	// Gets a flag indicating if a C++ constructor is a default constructor
	property bool IsCxxDefaultConstructor
	{
		bool get(void);
	}

	// IsCxxMethodDefaulted
	//
	// Gets a flag indicating if a C++ method is defaulted
	property bool IsCxxMethodDefaulted
	{
		bool get(void);
	}

	// IsCxxMoveConstructor
	//
	// Gets a flag indicating if a C++ constructor is a move constructor
	property bool IsCxxMoveConstructor
	{
		bool get(void);
	}

	// IsCxxMutableField
	//
	// Gets a flag indicating if a C++ field is declared 'mutable'
	property bool IsCxxMutableField
	{
		bool get(void);
	}

	// IsCxxPureVirtualMethod
	//
	// Gets a flag indicating if a C++ member function or member function template is pure virtual
	property bool IsCxxPureVirtualMethod
	{
		bool get(void);
	}

	// IsCxxStaticMethod
	//
	// Gets a flag indicating if a C++ member function or member function template is declared 'static'
	property bool IsCxxStaticMethod
	{
		bool get(void);
	}

	// IsCxxVirtualBase
	//
	// Gets a flag indicating if the cursor is a virtual base class
	property bool IsCxxVirtualBase
	{
		bool get(void);
	}

	// IsCxxVirtualMethod
	//
	// Gets a flag indicating if a C++ member function or member function template is virtual
	property bool IsCxxVirtualMethod
	{
		bool get(void);
	}

	// IsDefinition
	//
	// Gets a flag indicating if the declaration pointed to by this cursor is also a definition of that entity
	property bool IsDefinition
	{
		bool get(void);
	}

	// IsDynamicCall
	//
	// Gets a flag indicating if the given cursor is a dynamic call
	property bool IsDynamicCall
	{
		bool get(void);
	}

	// IsFunctionInlined
	//
	// Gets a flag indicating if the given cursor is an inlined function
	property bool IsFunctionInlined
	{
		bool get(void);
	}

	// IsMacroBuiltin
	//
	// Gets a flag indicating if the given cursor is a built in macro
	property bool IsMacroBuiltin
	{
		bool get(void);
	}

	// IsMacroFunctionLike
	//
	// Gets a flag indicating if the given cursor is a function like macro
	property bool IsMacroFunctionLike
	{
		bool get(void);
	}

	// IsObjectiveCOptional
	//
	// Gets a flag indicating if the declaration was affected by "@optional"
	property bool IsObjectiveCOptional
	{
		bool get(void);
	}

	// IsVariadic
	//
	// Gets a flag indicating if the given cursor is a variadic function or method
	property bool IsVariadic
	{
		bool get(void);
	}

	// Kind
	//
	// Gets the kind of this cursor
	property CursorKind Kind
	{
		CursorKind get(void);
	}

	// Language
	//
	// Gets the language of the entity referred to by a given cursor
	property local::Language Language
	{
		local::Language get(void);
	}

	// LexicalParentCursor
	//
	// Gets the lexical parent cursor of this cursor
	property Cursor^ LexicalParentCursor
	{
		Cursor^ get(void);
	}

	// Linkage
	//
	// Gets the the linkage of the entity referred to by the cursor
	property local::Linkage Linkage
	{
		local::Linkage get(void);
	}

	// Location
	//
	// Gets the location of the cursor in the translation unit
	property local::Location^ Location
	{
		local::Location^ get(void);
	}

	// MangledName
	//
	// Gets the mangled name for the entity referenced by this cursor
	property String^ MangledName
	{
		String^ get(void);
	}

	// MessageReceiverType
	//
	// Gets the type of an Objective-C message receiver
	property local::Type^ MessageReceiverType
	{
		local::Type^ get(void);
	}

	// ObjectiveCDeclarationQualifiers
	//
	// Gets the Objective-C declaration qualifier flags
	property ObjectiveCDeclarationQualifier ObjectiveCDeclarationQualifiers
	{
		ObjectiveCDeclarationQualifier get(void);
	}

	// ObjectiveCPropertyAttributes
	//
	// Gets the Objective-C property attributes
	property ObjectiveCPropertyAttribute ObjectiveCPropertyAttributes
	{
		ObjectiveCPropertyAttribute get(void);
	}

	// ObjectiveCSelectorIndex
	//
	// Gets the Objective-C selector index
	property Nullable<int> ObjectiveCSelectorIndex
	{
		Nullable<int> get(void);
	}

	// ObjectiveCTypeEncoding
	//
	// Gets the Objective-C type encoding
	property String^ ObjectiveCTypeEncoding
	{
		String^ get(void);
	}

	// OverloadedDeclarationCursors
	//
	// Gets a collection of overloaded declaration cursors
	property CursorCollection^ OverloadedDeclarationCursors
	{
		CursorCollection^ get(void);
	}

	// OverriddenCursors
	//
	// Gets a collection of cursors overridden by this cursor
	property CursorCollection^ OverriddenCursors
	{
		CursorCollection^ get(void);
	}

	// ParsedComment
	//
	// Gets the fully parsed comment/documentation for this cursor
	property local::ParsedComment^ ParsedComment
	{
		local::ParsedComment^ get(void);
	}

	// PlatformAvailability
	//
	// Gets a collection indicating the platform availability of the entity
	property PlatformAvailabilityCollection^ PlatformAvailability
	{
		PlatformAvailabilityCollection^ get(void);
	}

	// RecordOffset
	//
	// Gets the offset of a record field cursor
	property Nullable<__int64> RecordOffset
	{
		Nullable<__int64> get(void);
	}

	// ReferencedCursor
	//
	// Gets a cursor representing the entity that this cursor refers to
	property Cursor^ ReferencedCursor
	{
		Cursor^ get(void);
	}

	// ResultType
	//
	// Gets the result type of a function or method cursor
	property local::Type^ ResultType
	{
		local::Type^ get(void);
	}

	// SemanticParentCursor
	//
	// Gets the semantic parent cursor of this cursor
	property Cursor^ SemanticParentCursor
	{
		Cursor^ get(void);
	}

	// SpecializedTemplateCursor
	//
	// Gets the the cursor that represents the template that it specializes
	property Cursor^ SpecializedTemplateCursor
	{
		Cursor^ get(void);
	}

	// Spelling
	//
	// Gets the spelling of the entity pointed at by the cursor
	property String^ Spelling
	{
		String^ get(void);
	}

	// StorageClass
	//
	// Gets the storage class of the entity pointed at by the cursor
	property local::StorageClass StorageClass
	{
		local::StorageClass get(void);
	}

	// TemplateArguments
	//
	// Gets a collection of cursor template arguments
	property TemplateArgumentCollection^ TemplateArguments
	{
		TemplateArgumentCollection^ get(void);
	}

	// TemplateCursorKind
	//
	// Gets the kind of the cursor kind of the specializations would be generated by instantiating the template
	property CursorKind TemplateCursorKind
	{
		CursorKind get(void);
	}

	// UnderlyingTypedefType
	//
	// Gets the underlying type of a typedef cursor
	property local::Type^ UnderlyingTypedefType
	{
		local::Type^ get(void);
	}

	// Type
	//
	// Gets the cursor type
	property local::Type^ Type
	{
		local::Type^ get(void);
	}

	// UnifiedSymbolResolution
	//
	// Gets the Unified Symbol Resultion (USR) string for the entity
	property local::UnifiedSymbolResolution^ UnifiedSymbolResolution
	{
		local::UnifiedSymbolResolution^ get(void);
	}

	// Visibility
	//
	// Gets the visibility of the entity referred to by a cursor
	property CursorVisibility Visibility
	{
		CursorVisibility get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	// Null
	//
	// Represents a null cursor instance
	static initonly Cursor^ Null;

internal:

	//-----------------------------------------------------------------------
	// Internal Member Functions

	// Create
	//
	// Creates a new Cursor instance
	static Cursor^ Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXCursor cursor);

private:

	// CursorHandle
	//
	// TranslationUnitReferenceHandle specialization for CXCursor
	using CursorHandle = TranslationUnitReferenceHandle<CXCursor>;

	// Instance Constructor
	//
	Cursor(CursorHandle^ handle);

	// Static Constructor
	//
	static Cursor();

	//-----------------------------------------------------------------------
	// Member Variables

	CursorHandle^						m_handle;			// Underlying safe handle
	CursorComment^						m_rawcomment;		// Cached raw comment
	String^								m_briefcomment;		// Cached brief comment
	local::Extent^						m_extent;			// Cached extent reference
	local::Location^					m_location;			// Cached location reference
	PlatformAvailabilityCollection^		m_platavail;		// Cached platform availability
	String^								m_mangled;			// Cached mangled name
	String^								m_spelling;			// Cached cursor spelling
	local::UnifiedSymbolResolution^		m_usr;				// Cached cursor USR
	String^								m_displayname;		// Cached display name
	local::Type^						m_type;				// Cached cursor type
	Cursor^								m_canonical;		// Cached canonical cursor
	Cursor^								m_definition;		// Cached definition cursor
	Cursor^								m_lexical;			// Cached lexical cursor
	Cursor^								m_referenced;		// Cached referenced cursor
	Cursor^								m_semparent;		// Cached semantic parent cursor
	Cursor^								m_specialization;	// Cached specialization cursor
	local::Type^						m_underlying;		// Cached underling typedef type
	TemplateArgumentCollection^			m_templargs;		// Cached template arguments
	CursorCollection^					m_arguments;		// Cached argument collection
	local::Type^						m_enumtype;			// Cached enum type
	local::Type^						m_iboctype;			// Cached IBOutletCollection type
	File^								m_includedfile;		// Cached included file
	Module^								m_importmodule;		// Cached import module
	local::Type^						m_msgreceiver;		// Cached message receiver type
	CursorCollection^					m_overloadeddecls;	// Cached overloaded declarations
	CursorCollection^					m_overridden;		// Cached overridden cursors
	local::Type^						m_resulttype;		// Cached result type
	StringCollection^					m_manglings;		// Cached CXX name manglings
	String^								m_objcencoding;		// Cached Obj-C type encoding
	local::ParsedComment^				m_fullcomment;		// Cached parsed comment
	local::CompletionString^			m_completion;		// Cached completion string
};

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)

#endif	// __CURSOR_H_
