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

#ifndef __CURSORKIND_H_
#define __CURSORKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class CursorKind
//
// Describes the kind of entity that a cursor refers to
//---------------------------------------------------------------------------

public value class CursorKind
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(CursorKind lhs, CursorKind rhs);

	// operator!= (static)
	//
	static bool operator!=(CursorKind lhs, CursorKind rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this CursorKind to another CursorKind
	bool Equals(CursorKind rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	// ToString
	//
	// Gets the string representation of the cursor kind
	String^ ToString(String^ format);

	//-----------------------------------------------------------------------
	// Properties

	// IsAttribute
	//
	// True if this is an attribute kind
	property bool IsAttribute
	{
		bool get(void);
	}

	// IsDeclaration
	//
	// True if this is a declaration kind
	property bool IsDeclaration
	{
		bool get(void);
	}

	// IsExpression
	//
	// True if this is an expression kind
	property bool IsExpression
	{
		bool get(void);
	}

	// IsInvalid
	//
	// True if this is an invalid kind
	property bool IsInvalid
	{
		bool get(void);
	}

	// IsPreprocessing
	//
	// True if this is a preprocessing kind
	property bool IsPreprocessing
	{
		bool get(void);
	}

	// IsReference
	//
	// True if this is a reference kind
	property bool IsReference
	{
		bool get(void);
	}

	// IsStatement
	//
	// True if this is a statement kind
	property bool IsStatement
	{
		bool get(void);
	}

	// IsTranslationUnit
	//
	// True if this is a translation unit kind
	property bool IsTranslationUnit
	{
		bool get(void);
	}

	// IsUnexposed
	//
	// True if this is an unexposed kind
	property bool IsUnexposed
	{
		bool get(void);
	}

	//-----------------------------------------------------------------------
	// Fields

	static initonly CursorKind UnexposedDecl						= CursorKind(CXCursorKind::CXCursor_UnexposedDecl);
	static initonly CursorKind StructDecl							= CursorKind(CXCursorKind::CXCursor_StructDecl);
	static initonly CursorKind UnionDecl							= CursorKind(CXCursorKind::CXCursor_UnionDecl);
	static initonly CursorKind ClassDecl							= CursorKind(CXCursorKind::CXCursor_ClassDecl);
	static initonly CursorKind EnumDecl								= CursorKind(CXCursorKind::CXCursor_EnumDecl);
	static initonly CursorKind FieldDecl							= CursorKind(CXCursorKind::CXCursor_FieldDecl);
	static initonly CursorKind EnumConstantDecl						= CursorKind(CXCursorKind::CXCursor_EnumConstantDecl);
	static initonly CursorKind FunctionDecl							= CursorKind(CXCursorKind::CXCursor_FunctionDecl);
	static initonly CursorKind VarDecl								= CursorKind(CXCursorKind::CXCursor_VarDecl);
	static initonly CursorKind ParmDecl								= CursorKind(CXCursorKind::CXCursor_ParmDecl);
	static initonly CursorKind ObjectiveCInterfaceDecl				= CursorKind(CXCursorKind::CXCursor_ObjCInterfaceDecl);
	static initonly CursorKind ObjectiveCCategoryDecl				= CursorKind(CXCursorKind::CXCursor_ObjCCategoryDecl);
	static initonly CursorKind ObjectiveCProtocolDecl				= CursorKind(CXCursorKind::CXCursor_ObjCProtocolDecl);
	static initonly CursorKind ObjectiveCPropertyDecl				= CursorKind(CXCursorKind::CXCursor_ObjCPropertyDecl);
	static initonly CursorKind ObjectiveCIvarDecl					= CursorKind(CXCursorKind::CXCursor_ObjCIvarDecl);
	static initonly CursorKind ObjectiveCInstanceMethodDecl			= CursorKind(CXCursorKind::CXCursor_ObjCInstanceMethodDecl);
	static initonly CursorKind ObjectiveCClassMethodDecl			= CursorKind(CXCursorKind::CXCursor_ObjCClassMethodDecl);
	static initonly CursorKind ObjectiveCImplementationDecl			= CursorKind(CXCursorKind::CXCursor_ObjCImplementationDecl);
	static initonly CursorKind ObjectiveCCategoryImplDecl			= CursorKind(CXCursorKind::CXCursor_ObjCCategoryImplDecl);
	static initonly CursorKind TypedefDecl							= CursorKind(CXCursorKind::CXCursor_TypedefDecl);
	static initonly CursorKind CxxMethod							= CursorKind(CXCursorKind::CXCursor_CXXMethod);
	static initonly CursorKind Namespace							= CursorKind(CXCursorKind::CXCursor_Namespace);
	static initonly CursorKind LinkageSpec							= CursorKind(CXCursorKind::CXCursor_LinkageSpec);
	static initonly CursorKind Constructor							= CursorKind(CXCursorKind::CXCursor_Constructor);
	static initonly CursorKind Destructor							= CursorKind(CXCursorKind::CXCursor_Destructor);
	static initonly CursorKind ConversionFunction					= CursorKind(CXCursorKind::CXCursor_ConversionFunction);
	static initonly CursorKind TemplateTypeParameter				= CursorKind(CXCursorKind::CXCursor_TemplateTypeParameter);
	static initonly CursorKind NonTypeTemplateParameter				= CursorKind(CXCursorKind::CXCursor_NonTypeTemplateParameter);
	static initonly CursorKind TemplateTemplateParameter			= CursorKind(CXCursorKind::CXCursor_TemplateTemplateParameter);
	static initonly CursorKind FunctionTemplate						= CursorKind(CXCursorKind::CXCursor_FunctionTemplate);
	static initonly CursorKind ClassTemplate						= CursorKind(CXCursorKind::CXCursor_ClassTemplate);
	static initonly CursorKind ClassTemplatePartialSpecialization	= CursorKind(CXCursorKind::CXCursor_ClassTemplatePartialSpecialization);
	static initonly CursorKind NamespaceAlias						= CursorKind(CXCursorKind::CXCursor_NamespaceAlias);
	static initonly CursorKind UsingDirective						= CursorKind(CXCursorKind::CXCursor_UsingDirective);
	static initonly CursorKind UsingDeclaration						= CursorKind(CXCursorKind::CXCursor_UsingDeclaration);
	static initonly CursorKind TypeAliasDecl						= CursorKind(CXCursorKind::CXCursor_TypeAliasDecl);
	static initonly CursorKind ObjectiveCSynthesizeDecl				= CursorKind(CXCursorKind::CXCursor_ObjCSynthesizeDecl);
	static initonly CursorKind ObjectiveCDynamicDecl				= CursorKind(CXCursorKind::CXCursor_ObjCDynamicDecl);
	static initonly CursorKind CxxAccessSpecifier					= CursorKind(CXCursorKind::CXCursor_CXXAccessSpecifier);
	static initonly CursorKind ObjectiveCSuperClassRef				= CursorKind(CXCursorKind::CXCursor_ObjCSuperClassRef);
	static initonly CursorKind ObjectiveCProtocolRef				= CursorKind(CXCursorKind::CXCursor_ObjCProtocolRef);
	static initonly CursorKind ObjectiveCClassRef					= CursorKind(CXCursorKind::CXCursor_ObjCClassRef);
	static initonly CursorKind TypeRef								= CursorKind(CXCursorKind::CXCursor_TypeRef);
	static initonly CursorKind CxxBaseSpecifier						= CursorKind(CXCursorKind::CXCursor_CXXBaseSpecifier);
	static initonly CursorKind TemplateRef							= CursorKind(CXCursorKind::CXCursor_TemplateRef);
	static initonly CursorKind NamespaceRef							= CursorKind(CXCursorKind::CXCursor_NamespaceRef);
	static initonly CursorKind MemberRef							= CursorKind(CXCursorKind::CXCursor_MemberRef);
	static initonly CursorKind LabelRef								= CursorKind(CXCursorKind::CXCursor_LabelRef);
	static initonly CursorKind OverloadedDeclRef					= CursorKind(CXCursorKind::CXCursor_OverloadedDeclRef);
	static initonly CursorKind VariableRef							= CursorKind(CXCursorKind::CXCursor_VariableRef);
	static initonly CursorKind InvalidFile							= CursorKind(CXCursorKind::CXCursor_InvalidFile);
	static initonly CursorKind NoDeclFound							= CursorKind(CXCursorKind::CXCursor_NoDeclFound);
	static initonly CursorKind NotImplemented						= CursorKind(CXCursorKind::CXCursor_NotImplemented);
	static initonly CursorKind InvalidCode							= CursorKind(CXCursorKind::CXCursor_InvalidCode);
	static initonly CursorKind UnexposedExpr						= CursorKind(CXCursorKind::CXCursor_UnexposedExpr);
	static initonly CursorKind DeclRefExpr							= CursorKind(CXCursorKind::CXCursor_DeclRefExpr);
	static initonly CursorKind MemberRefExpr						= CursorKind(CXCursorKind::CXCursor_MemberRefExpr);
	static initonly CursorKind CallExpr								= CursorKind(CXCursorKind::CXCursor_CallExpr);
	static initonly CursorKind ObjectiveCMessageExpr				= CursorKind(CXCursorKind::CXCursor_ObjCMessageExpr);
	static initonly CursorKind BlockExpr							= CursorKind(CXCursorKind::CXCursor_BlockExpr);
	static initonly CursorKind IntegerLiteral						= CursorKind(CXCursorKind::CXCursor_IntegerLiteral);
	static initonly CursorKind FloatingLiteral						= CursorKind(CXCursorKind::CXCursor_FloatingLiteral);
	static initonly CursorKind ImaginaryLiteral						= CursorKind(CXCursorKind::CXCursor_ImaginaryLiteral);
	static initonly CursorKind StringLiteral						= CursorKind(CXCursorKind::CXCursor_StringLiteral);
	static initonly CursorKind CharacterLiteral						= CursorKind(CXCursorKind::CXCursor_CharacterLiteral);
	static initonly CursorKind ParenExpr							= CursorKind(CXCursorKind::CXCursor_ParenExpr);
	static initonly CursorKind UnaryOperator						= CursorKind(CXCursorKind::CXCursor_UnaryOperator);
	static initonly CursorKind ArraySubscriptExpr					= CursorKind(CXCursorKind::CXCursor_ArraySubscriptExpr);
	static initonly CursorKind BinaryOperator						= CursorKind(CXCursorKind::CXCursor_BinaryOperator);
	static initonly CursorKind CompoundAssignOperator				= CursorKind(CXCursorKind::CXCursor_CompoundAssignOperator);
	static initonly CursorKind ConditionalOperator					= CursorKind(CXCursorKind::CXCursor_ConditionalOperator);
	static initonly CursorKind CStyleCastExpr						= CursorKind(CXCursorKind::CXCursor_CStyleCastExpr);
	static initonly CursorKind CompoundLiteralExpr					= CursorKind(CXCursorKind::CXCursor_CompoundLiteralExpr);
	static initonly CursorKind InitListExpr							= CursorKind(CXCursorKind::CXCursor_InitListExpr);
	static initonly CursorKind AddrLabelExpr						= CursorKind(CXCursorKind::CXCursor_AddrLabelExpr);
	static initonly CursorKind StmtExpr								= CursorKind(CXCursorKind::CXCursor_StmtExpr);
	static initonly CursorKind GenericSelectionExpr					= CursorKind(CXCursorKind::CXCursor_GenericSelectionExpr);
	static initonly CursorKind GNUNullExpr							= CursorKind(CXCursorKind::CXCursor_GNUNullExpr);
	static initonly CursorKind CxxStaticCastExpr					= CursorKind(CXCursorKind::CXCursor_CXXStaticCastExpr);
	static initonly CursorKind CxxDynamicCastExpr					= CursorKind(CXCursorKind::CXCursor_CXXDynamicCastExpr);
	static initonly CursorKind CxxReinterpretCastExpr				= CursorKind(CXCursorKind::CXCursor_CXXReinterpretCastExpr);
	static initonly CursorKind CxxConstCastExpr						= CursorKind(CXCursorKind::CXCursor_CXXConstCastExpr);
	static initonly CursorKind CxxFunctionalCastExpr				= CursorKind(CXCursorKind::CXCursor_CXXFunctionalCastExpr);
	static initonly CursorKind CxxTypeidExpr						= CursorKind(CXCursorKind::CXCursor_CXXTypeidExpr);
	static initonly CursorKind CxxBoolLiteralExpr					= CursorKind(CXCursorKind::CXCursor_CXXBoolLiteralExpr);
	static initonly CursorKind CxxNullPtrLiteralExpr				= CursorKind(CXCursorKind::CXCursor_CXXNullPtrLiteralExpr);
	static initonly CursorKind CxxThisExpr							= CursorKind(CXCursorKind::CXCursor_CXXThisExpr);
	static initonly CursorKind CxxThrowExpr							= CursorKind(CXCursorKind::CXCursor_CXXThrowExpr);
	static initonly CursorKind CxxNewExpr							= CursorKind(CXCursorKind::CXCursor_CXXNewExpr);
	static initonly CursorKind CxxDeleteExpr						= CursorKind(CXCursorKind::CXCursor_CXXDeleteExpr);
	static initonly CursorKind UnaryExpr							= CursorKind(CXCursorKind::CXCursor_UnaryExpr);
	static initonly CursorKind ObjectiveCStringLiteral				= CursorKind(CXCursorKind::CXCursor_ObjCStringLiteral);
	static initonly CursorKind ObjectiveCEncodeExpr					= CursorKind(CXCursorKind::CXCursor_ObjCEncodeExpr);
	static initonly CursorKind ObjectiveCSelectorExpr				= CursorKind(CXCursorKind::CXCursor_ObjCSelectorExpr);
	static initonly CursorKind ObjectiveCProtocolExpr				= CursorKind(CXCursorKind::CXCursor_ObjCProtocolExpr);
	static initonly CursorKind ObjectiveCBridgedCastExpr			= CursorKind(CXCursorKind::CXCursor_ObjCBridgedCastExpr);
	static initonly CursorKind PackExpansionExpr					= CursorKind(CXCursorKind::CXCursor_PackExpansionExpr);
	static initonly CursorKind SizeOfPackExpr						= CursorKind(CXCursorKind::CXCursor_SizeOfPackExpr);
	static initonly CursorKind LambdaExpr							= CursorKind(CXCursorKind::CXCursor_LambdaExpr);
	static initonly CursorKind ObjectiveCBoolLiteralExpr			= CursorKind(CXCursorKind::CXCursor_ObjCBoolLiteralExpr);
	static initonly CursorKind ObjectiveCSelfExpr					= CursorKind(CXCursorKind::CXCursor_ObjCSelfExpr);
	static initonly CursorKind OMPArraySectionExpr					= CursorKind(CXCursorKind::CXCursor_OMPArraySectionExpr);
	static initonly CursorKind UnexposedStmt						= CursorKind(CXCursorKind::CXCursor_UnexposedStmt);
	static initonly CursorKind LabelStmt							= CursorKind(CXCursorKind::CXCursor_LabelStmt);
	static initonly CursorKind CompoundStmt							= CursorKind(CXCursorKind::CXCursor_CompoundStmt);
	static initonly CursorKind CaseStmt								= CursorKind(CXCursorKind::CXCursor_CaseStmt);
	static initonly CursorKind DefaultStmt							= CursorKind(CXCursorKind::CXCursor_DefaultStmt);
	static initonly CursorKind IfStmt								= CursorKind(CXCursorKind::CXCursor_IfStmt);
	static initonly CursorKind SwitchStmt							= CursorKind(CXCursorKind::CXCursor_SwitchStmt);
	static initonly CursorKind WhileStmt							= CursorKind(CXCursorKind::CXCursor_WhileStmt);
	static initonly CursorKind DoStmt								= CursorKind(CXCursorKind::CXCursor_DoStmt);
	static initonly CursorKind ForStmt								= CursorKind(CXCursorKind::CXCursor_ForStmt);
	static initonly CursorKind GotoStmt								= CursorKind(CXCursorKind::CXCursor_GotoStmt);
	static initonly CursorKind IndirectGotoStmt						= CursorKind(CXCursorKind::CXCursor_IndirectGotoStmt);
	static initonly CursorKind ContinueStmt							= CursorKind(CXCursorKind::CXCursor_ContinueStmt);
	static initonly CursorKind BreakStmt							= CursorKind(CXCursorKind::CXCursor_BreakStmt);
	static initonly CursorKind ReturnStmt							= CursorKind(CXCursorKind::CXCursor_ReturnStmt);
	static initonly CursorKind GCCAsmStmt							= CursorKind(CXCursorKind::CXCursor_GCCAsmStmt);
	static initonly CursorKind AsmStmt								= CursorKind(CXCursorKind::CXCursor_AsmStmt);
	static initonly CursorKind ObjectiveCAtTryStmt					= CursorKind(CXCursorKind::CXCursor_ObjCAtTryStmt);
	static initonly CursorKind ObjectiveCAtCatchStmt				= CursorKind(CXCursorKind::CXCursor_ObjCAtCatchStmt);
	static initonly CursorKind ObjectiveCAtFinallyStmt				= CursorKind(CXCursorKind::CXCursor_ObjCAtFinallyStmt);
	static initonly CursorKind ObjectiveCAtThrowStmt				= CursorKind(CXCursorKind::CXCursor_ObjCAtThrowStmt);
	static initonly CursorKind ObjectiveCAtSynchronizedStmt			= CursorKind(CXCursorKind::CXCursor_ObjCAtSynchronizedStmt);
	static initonly CursorKind ObjectiveCAutoreleasePoolStmt		= CursorKind(CXCursorKind::CXCursor_ObjCAutoreleasePoolStmt);
	static initonly CursorKind ObjectiveCForCollectionStmt			= CursorKind(CXCursorKind::CXCursor_ObjCForCollectionStmt);
	static initonly CursorKind CxxCatchStmt							= CursorKind(CXCursorKind::CXCursor_CXXCatchStmt);
	static initonly CursorKind CxxTryStmt							= CursorKind(CXCursorKind::CXCursor_CXXTryStmt);
	static initonly CursorKind CxxForRangeStmt						= CursorKind(CXCursorKind::CXCursor_CXXForRangeStmt);
	static initonly CursorKind SEHTryStmt							= CursorKind(CXCursorKind::CXCursor_SEHTryStmt);
	static initonly CursorKind SEHExceptStmt						= CursorKind(CXCursorKind::CXCursor_SEHExceptStmt);
	static initonly CursorKind SEHFinallyStmt						= CursorKind(CXCursorKind::CXCursor_SEHFinallyStmt);
	static initonly CursorKind MSAsmStmt							= CursorKind(CXCursorKind::CXCursor_MSAsmStmt);
	static initonly CursorKind NullStmt								= CursorKind(CXCursorKind::CXCursor_NullStmt);
	static initonly CursorKind DeclStmt								= CursorKind(CXCursorKind::CXCursor_DeclStmt);
	static initonly CursorKind OMPParallelDirective					= CursorKind(CXCursorKind::CXCursor_OMPParallelDirective);
	static initonly CursorKind OMPSimdDirective						= CursorKind(CXCursorKind::CXCursor_OMPSimdDirective);
	static initonly CursorKind OMPForDirective						= CursorKind(CXCursorKind::CXCursor_OMPForDirective);
	static initonly CursorKind OMPSectionsDirective					= CursorKind(CXCursorKind::CXCursor_OMPSectionsDirective);
	static initonly CursorKind OMPSectionDirective					= CursorKind(CXCursorKind::CXCursor_OMPSectionDirective);
	static initonly CursorKind OMPSingleDirective					= CursorKind(CXCursorKind::CXCursor_OMPSingleDirective);
	static initonly CursorKind OMPParallelForDirective				= CursorKind(CXCursorKind::CXCursor_OMPParallelForDirective);
	static initonly CursorKind OMPParallelSectionsDirective			= CursorKind(CXCursorKind::CXCursor_OMPParallelSectionsDirective);
	static initonly CursorKind OMPTaskDirective						= CursorKind(CXCursorKind::CXCursor_OMPTaskDirective);
	static initonly CursorKind OMPMasterDirective					= CursorKind(CXCursorKind::CXCursor_OMPMasterDirective);
	static initonly CursorKind OMPCriticalDirective					= CursorKind(CXCursorKind::CXCursor_OMPCriticalDirective);
	static initonly CursorKind OMPTaskyieldDirective				= CursorKind(CXCursorKind::CXCursor_OMPTaskyieldDirective);
	static initonly CursorKind OMPBarrierDirective					= CursorKind(CXCursorKind::CXCursor_OMPBarrierDirective);
	static initonly CursorKind OMPTaskwaitDirective					= CursorKind(CXCursorKind::CXCursor_OMPTaskwaitDirective);
	static initonly CursorKind OMPFlushDirective					= CursorKind(CXCursorKind::CXCursor_OMPFlushDirective);
	static initonly CursorKind SEHLeaveStmt							= CursorKind(CXCursorKind::CXCursor_SEHLeaveStmt);
	static initonly CursorKind OMPOrderedDirective					= CursorKind(CXCursorKind::CXCursor_OMPOrderedDirective);
	static initonly CursorKind OMPAtomicDirective					= CursorKind(CXCursorKind::CXCursor_OMPAtomicDirective);
	static initonly CursorKind OMPForSimdDirective					= CursorKind(CXCursorKind::CXCursor_OMPForSimdDirective);
	static initonly CursorKind OMPParallelForSimdDirective			= CursorKind(CXCursorKind::CXCursor_OMPParallelForSimdDirective);
	static initonly CursorKind OMPTargetDirective					= CursorKind(CXCursorKind::CXCursor_OMPTargetDirective);
	static initonly CursorKind OMPTeamsDirective					= CursorKind(CXCursorKind::CXCursor_OMPTeamsDirective);
	static initonly CursorKind OMPTaskgroupDirective				= CursorKind(CXCursorKind::CXCursor_OMPTaskgroupDirective);
	static initonly CursorKind OMPCancellationPointDirective		= CursorKind(CXCursorKind::CXCursor_OMPCancellationPointDirective);
	static initonly CursorKind OMPCancelDirective					= CursorKind(CXCursorKind::CXCursor_OMPCancelDirective);
	static initonly CursorKind OMPTargetDataDirective				= CursorKind(CXCursorKind::CXCursor_OMPTargetDataDirective);
	static initonly CursorKind OMPTaskLoopDirective					= CursorKind(CXCursorKind::CXCursor_OMPTaskLoopDirective);
	static initonly CursorKind OMPTaskLoopSimdDirective				= CursorKind(CXCursorKind::CXCursor_OMPTaskLoopSimdDirective);
	static initonly CursorKind OMPDistributeDirective				= CursorKind(CXCursorKind::CXCursor_OMPDistributeDirective);
	static initonly CursorKind TranslationUnit						= CursorKind(CXCursorKind::CXCursor_TranslationUnit);
	static initonly CursorKind UnexposedAttr						= CursorKind(CXCursorKind::CXCursor_UnexposedAttr);
	static initonly CursorKind IBActionAttr							= CursorKind(CXCursorKind::CXCursor_IBActionAttr);
	static initonly CursorKind IBOutletAttr							= CursorKind(CXCursorKind::CXCursor_IBOutletAttr);
	static initonly CursorKind IBOutletCollectionAttr				= CursorKind(CXCursorKind::CXCursor_IBOutletCollectionAttr);
	static initonly CursorKind CxxFinalAttr							= CursorKind(CXCursorKind::CXCursor_CXXFinalAttr);
	static initonly CursorKind CxxOverrideAttr						= CursorKind(CXCursorKind::CXCursor_CXXOverrideAttr);
	static initonly CursorKind AnnotateAttr							= CursorKind(CXCursorKind::CXCursor_AnnotateAttr);
	static initonly CursorKind AsmLabelAttr							= CursorKind(CXCursorKind::CXCursor_AsmLabelAttr);
	static initonly CursorKind PackedAttr							= CursorKind(CXCursorKind::CXCursor_PackedAttr);
	static initonly CursorKind PureAttr								= CursorKind(CXCursorKind::CXCursor_PureAttr);
	static initonly CursorKind ConstAttr							= CursorKind(CXCursorKind::CXCursor_ConstAttr);
	static initonly CursorKind NoDuplicateAttr						= CursorKind(CXCursorKind::CXCursor_NoDuplicateAttr);
	static initonly CursorKind CUDAConstantAttr						= CursorKind(CXCursorKind::CXCursor_CUDAConstantAttr);
	static initonly CursorKind CUDADeviceAttr						= CursorKind(CXCursorKind::CXCursor_CUDADeviceAttr);
	static initonly CursorKind CUDAGlobalAttr						= CursorKind(CXCursorKind::CXCursor_CUDAGlobalAttr);
	static initonly CursorKind CUDAHostAttr							= CursorKind(CXCursorKind::CXCursor_CUDAHostAttr);
	static initonly CursorKind CUDASharedAttr						= CursorKind(CXCursorKind::CXCursor_CUDASharedAttr);
	static initonly CursorKind VisibilityAttr						= CursorKind(CXCursorKind::CXCursor_VisibilityAttr);
	static initonly CursorKind DLLExport							= CursorKind(CXCursorKind::CXCursor_DLLExport);
	static initonly CursorKind DLLImport							= CursorKind(CXCursorKind::CXCursor_DLLImport);
	static initonly CursorKind PreprocessingDirective				= CursorKind(CXCursorKind::CXCursor_PreprocessingDirective);
	static initonly CursorKind MacroDefinition						= CursorKind(CXCursorKind::CXCursor_MacroDefinition);
	static initonly CursorKind MacroExpansion						= CursorKind(CXCursorKind::CXCursor_MacroExpansion);
	static initonly CursorKind MacroInstantiation					= CursorKind(CXCursorKind::CXCursor_MacroInstantiation);
	static initonly CursorKind InclusionDirective					= CursorKind(CXCursorKind::CXCursor_InclusionDirective);
	static initonly CursorKind ModuleImportDecl						= CursorKind(CXCursorKind::CXCursor_ModuleImportDecl);
	static initonly CursorKind TypeAliasTemplateDecl				= CursorKind(CXCursorKind::CXCursor_TypeAliasTemplateDecl);
	static initonly CursorKind OverloadCandidate					= CursorKind(CXCursorKind::CXCursor_OverloadCandidate);

internal:

	// Instance Constructor
	//
	CursorKind(CXCursorKind cursorkind);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXCursorKind			m_cursorkind;			// Underlying CXCursorKind
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __CURSORKIND_H_
