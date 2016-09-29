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
#include "CursorKind.h"

#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// CursorKind Constructor (internal)
//
// Arguments:
//
//	CursorKind		- CXCursorKind to represent with this value class

CursorKind::CursorKind(CXCursorKind cursorkind) : m_cursorkind(cursorkind)
{
}

//---------------------------------------------------------------------------
// CursorKind::operator == (static)

bool CursorKind::operator==(CursorKind lhs, CursorKind rhs)
{
	return lhs.m_cursorkind == rhs.m_cursorkind;
}

//---------------------------------------------------------------------------
// CursorKind::operator != (static)

bool CursorKind::operator!=(CursorKind lhs, CursorKind rhs)
{
	return lhs.m_cursorkind != rhs.m_cursorkind;
}

//---------------------------------------------------------------------------
// CursorKind::Equals
//
// Compares this CursorKind to another CursorKind
//
// Arguments:
//
//	rhs		- Right-hand CursorKind to compare against

bool CursorKind::Equals(CursorKind rhs)
{
	return (*this == rhs);
}

//---------------------------------------------------------------------------
// CursorKind::Equals
//
// Overrides Object::Equals()
//
// Arguments:
//
//	rhs		- Right-hand object instance to compare against

bool CursorKind::Equals(Object^ rhs)
{
	if(Object::ReferenceEquals(rhs, nullptr)) return false;

	// Convert the provided object into a CursorKind instance
	CursorKind^ rhsref = dynamic_cast<CursorKind^>(rhs);
	if(rhsref == nullptr) return false;

	return (*this == *rhsref);
}

//---------------------------------------------------------------------------
// CursorKind::GetHashCode
//
// Overrides Object::GetHashCode()
//
// Arguments:
//
//	NONE

int CursorKind::GetHashCode(void)
{
	return static_cast<int>(m_cursorkind).GetHashCode();
}

//---------------------------------------------------------------------------
// CursorKind::IsAttribute::get
//
// True if this is an attribute kind

bool CursorKind::IsAttribute::get(void)
{
	return (clang_isAttribute(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsDeclaration::get
//
// True if this is a declaration kind

bool CursorKind::IsDeclaration::get(void)
{
	return (clang_isDeclaration(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsExpression::get
//
// True if this is an expression kind

bool CursorKind::IsExpression::get(void)
{
	return (clang_isExpression(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsInvalid::get
//
// True if this is an invalid kind

bool CursorKind::IsInvalid::get(void)
{
	return (clang_isInvalid(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsPreprocessing::get
//
// True if this is a preprocessing kind

bool CursorKind::IsPreprocessing::get(void)
{
	return (clang_isPreprocessing(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsReference::get
//
// True if this is a reference kind

bool CursorKind::IsReference::get(void)
{
	return (clang_isReference(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsStatement::get
//
// True if this is a statement kind

bool CursorKind::IsStatement::get(void)
{
	return (clang_isStatement(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsTranslationUnit::get
//
// True if this is a translation unit kind

bool CursorKind::IsTranslationUnit::get(void)
{
	return (clang_isTranslationUnit(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::IsUnexposed::get
//
// True if this is an unexposed kind

bool CursorKind::IsUnexposed::get(void)
{
	return (clang_isUnexposed(m_cursorkind) != 0);
}

//---------------------------------------------------------------------------
// CursorKind::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ CursorKind::ToString(void)
{
	return StringUtil::ToString(clang_getCursorKindSpelling(m_cursorkind));
}

//---------------------------------------------------------------------------
// CursorKind::ToString
//
// Gets the string representation of the cursor kind
//
// Arguments:
//
//	format		- Format specifier

String^ CursorKind::ToString(String^ format)
{
	// GITHUB ISSUE #1 (https://github.com/djp952/tools-llvm/issues/1)
	//
	// This overload for ToString() was added to allow the name of the CursorKind value
	// type to be exposed since the generalized strings from clang aren't always usable 
	// due to embedded spaces and special characters

	// "S" / "s" --> SHORT NAME
	//
	if(String::IsNullOrEmpty(format) || (format->ToUpper() != "S")) return ToString();

	switch(m_cursorkind) {

		case CXCursor_UnexposedDecl:							return "UnexposedDecl";
		case CXCursor_StructDecl:								return "StructDecl";
		case CXCursor_UnionDecl:								return "UnionDecl";
		case CXCursor_ClassDecl:								return "ClassDecl";
		case CXCursor_EnumDecl:									return "EnumDecl";
		case CXCursor_FieldDecl:								return "FieldDecl";
		case CXCursor_EnumConstantDecl:							return "EnumConstantDecl";
		case CXCursor_FunctionDecl:								return "FunctionDecl";
		case CXCursor_VarDecl:									return "VarDecl";
		case CXCursor_ParmDecl:									return "ParmDecl";
		case CXCursor_ObjCInterfaceDecl:						return "ObjectiveCInterfaceDecl";
		case CXCursor_ObjCCategoryDecl:							return "ObjectiveCCategoryDecl";
		case CXCursor_ObjCProtocolDecl:							return "ObjectiveCProtocolDecl";
		case CXCursor_ObjCPropertyDecl:							return "ObjectiveCPropertyDecl";
		case CXCursor_ObjCIvarDecl:								return "ObjectiveCIvarDecl";
		case CXCursor_ObjCInstanceMethodDecl:					return "ObjectiveCInstanceMethodDecl";
		case CXCursor_ObjCClassMethodDecl:						return "ObjectiveCClassMethodDecl";
		case CXCursor_ObjCImplementationDecl:					return "ObjectiveCImplementationDecl";
		case CXCursor_ObjCCategoryImplDecl:						return "ObjectiveCCategoryImplDecl";
		case CXCursor_TypedefDecl:								return "TypedefDecl";
		case CXCursor_CXXMethod:								return "CxxMethod";
		case CXCursor_Namespace:								return "Namespace";
		case CXCursor_LinkageSpec:								return "LinkageSpec";
		case CXCursor_Constructor:								return "Constructor";
		case CXCursor_Destructor:								return "Destructor";
		case CXCursor_ConversionFunction:						return "ConversionFunction";
		case CXCursor_TemplateTypeParameter:					return "TemplateTypeParameter";
		case CXCursor_NonTypeTemplateParameter:					return "NonTypeTemplateParameter";
		case CXCursor_TemplateTemplateParameter:				return "TemplateTemplateParameter";
		case CXCursor_FunctionTemplate:							return "FunctionTemplate";
		case CXCursor_ClassTemplate:							return "ClassTemplate";
		case CXCursor_ClassTemplatePartialSpecialization:		return "ClassTemplatePartialSpecialization";
		case CXCursor_NamespaceAlias:							return "NamespaceAlias";
		case CXCursor_UsingDirective:							return "UsingDirective";
		case CXCursor_UsingDeclaration:							return "UsingDeclaration";
		case CXCursor_TypeAliasDecl:							return "TypeAliasDecl";
		case CXCursor_ObjCSynthesizeDecl:						return "ObjectiveCSynthesizeDecl";
		case CXCursor_ObjCDynamicDecl:							return "ObjectiveCDynamicDecl";
		case CXCursor_CXXAccessSpecifier:						return "CxxAccessSpecifier";
		case CXCursor_ObjCSuperClassRef:						return "ObjectiveCSuperClassRef";
		case CXCursor_ObjCProtocolRef:							return "ObjectiveCProtocolRef";
		case CXCursor_ObjCClassRef:								return "ObjectiveCClassRef";
		case CXCursor_TypeRef:									return "TypeRef";
		case CXCursor_CXXBaseSpecifier:							return "CxxBaseSpecifier";
		case CXCursor_TemplateRef:								return "TemplateRef";
		case CXCursor_NamespaceRef:								return "NamespaceRef";
		case CXCursor_MemberRef:								return "MemberRef";
		case CXCursor_LabelRef:									return "LabelRef";
		case CXCursor_OverloadedDeclRef:						return "OverloadedDeclRef";
		case CXCursor_VariableRef:								return "VariableRef";
		case CXCursor_InvalidFile:								return "InvalidFile";
		case CXCursor_NoDeclFound:								return "NoDeclFound";
		case CXCursor_NotImplemented:							return "NotImplemented";
		case CXCursor_InvalidCode:								return "InvalidCode";
		case CXCursor_UnexposedExpr:							return "UnexposedExpr";
		case CXCursor_DeclRefExpr:								return "DeclRefExpr";
		case CXCursor_MemberRefExpr:							return "MemberRefExpr";
		case CXCursor_CallExpr:									return "CallExpr";
		case CXCursor_ObjCMessageExpr:							return "ObjectiveCMessageExpr";
		case CXCursor_BlockExpr:								return "BlockExpr";
		case CXCursor_IntegerLiteral:							return "IntegerLiteral";
		case CXCursor_FloatingLiteral:							return "FloatingLiteral";
		case CXCursor_ImaginaryLiteral:							return "ImaginaryLiteral";
		case CXCursor_StringLiteral:							return "StringLiteral";
		case CXCursor_CharacterLiteral:							return "CharacterLiteral";
		case CXCursor_ParenExpr:								return "ParenExpr";
		case CXCursor_UnaryOperator:							return "UnaryOperator";
		case CXCursor_ArraySubscriptExpr:						return "ArraySubscriptExpr";
		case CXCursor_BinaryOperator:							return "BinaryOperator";
		case CXCursor_CompoundAssignOperator:					return "CompoundAssignOperator";
		case CXCursor_ConditionalOperator:						return "ConditionalOperator";
		case CXCursor_CStyleCastExpr:							return "CStyleCastExpr";
		case CXCursor_CompoundLiteralExpr:						return "CompoundLiteralExpr";
		case CXCursor_InitListExpr:								return "InitListExpr";
		case CXCursor_AddrLabelExpr:							return "AddrLabelExpr";
		case CXCursor_StmtExpr:									return "StmtExpr";
		case CXCursor_GenericSelectionExpr:						return "GenericSelectionExpr";
		case CXCursor_GNUNullExpr:								return "GNUNullExpr";
		case CXCursor_CXXStaticCastExpr:						return "CxxStaticCastExpr";
		case CXCursor_CXXDynamicCastExpr:						return "CxxDynamicCastExpr";
		case CXCursor_CXXReinterpretCastExpr:					return "CxxReinterpretCastExpr";
		case CXCursor_CXXConstCastExpr:							return "CxxConstCastExpr";
		case CXCursor_CXXFunctionalCastExpr:					return "CxxFunctionalCastExpr";
		case CXCursor_CXXTypeidExpr:							return "CxxTypeidExpr";
		case CXCursor_CXXBoolLiteralExpr:						return "CxxBoolLiteralExpr";
		case CXCursor_CXXNullPtrLiteralExpr:					return "CxxNullPtrLiteralExpr";
		case CXCursor_CXXThisExpr:								return "CxxThisExpr";
		case CXCursor_CXXThrowExpr:								return "CxxThrowExpr";
		case CXCursor_CXXNewExpr:								return "CxxNewExpr";
		case CXCursor_CXXDeleteExpr:							return "CxxDeleteExpr";
		case CXCursor_UnaryExpr:								return "UnaryExpr";
		case CXCursor_ObjCStringLiteral:						return "ObjectiveCStringLiteral";
		case CXCursor_ObjCEncodeExpr:							return "ObjectiveCEncodeExpr";
		case CXCursor_ObjCSelectorExpr:							return "ObjectiveCSelectorExpr";
		case CXCursor_ObjCProtocolExpr:							return "ObjectiveCProtocolExpr";
		case CXCursor_ObjCBridgedCastExpr:						return "ObjectiveCBridgedCastExpr";
		case CXCursor_PackExpansionExpr:						return "PackExpansionExpr";
		case CXCursor_SizeOfPackExpr:							return "SizeOfPackExpr";
		case CXCursor_LambdaExpr:								return "LambdaExpr";
		case CXCursor_ObjCBoolLiteralExpr:						return "ObjectiveCBoolLiteralExpr";
		case CXCursor_ObjCSelfExpr:								return "ObjectiveCSelfExpr";
		case CXCursor_OMPArraySectionExpr:						return "OMPArraySectionExpr";
		case CXCursor_ObjCAvailabilityCheckExpr:				return "ObjectiveCAvailabilityCheckExpr";
		case CXCursor_UnexposedStmt:							return "UnexposedStmt";
		case CXCursor_LabelStmt:								return "LabelStmt";
		case CXCursor_CompoundStmt:								return "CompoundStmt";
		case CXCursor_CaseStmt:									return "CaseStmt";
		case CXCursor_DefaultStmt:								return "DefaultStmt";
		case CXCursor_IfStmt:									return "IfStmt";
		case CXCursor_SwitchStmt:								return "SwitchStmt";
		case CXCursor_WhileStmt:								return "WhileStmt";
		case CXCursor_DoStmt:									return "DoStmt";
		case CXCursor_ForStmt:									return "ForStmt";
		case CXCursor_GotoStmt:									return "GotoStmt";
		case CXCursor_IndirectGotoStmt:							return "IndirectGotoStmt";
		case CXCursor_ContinueStmt:								return "ContinueStmt";
		case CXCursor_BreakStmt:								return "BreakStmt";
		case CXCursor_ReturnStmt:								return "ReturnStmt";
		//case CXCursor_GCCAsmStmt:								return "GCCAsmStmt";
		case CXCursor_AsmStmt:									return "AsmStmt";
		case CXCursor_ObjCAtTryStmt:							return "ObjectiveCAtTryStmt";
		case CXCursor_ObjCAtCatchStmt:							return "ObjectiveCAtCatchStmt";
		case CXCursor_ObjCAtFinallyStmt:						return "ObjectiveCAtFinallyStmt";
		case CXCursor_ObjCAtThrowStmt:							return "ObjectiveCAtThrowStmt";
		case CXCursor_ObjCAtSynchronizedStmt:					return "ObjectiveCAtSynchronizedStmt";
		case CXCursor_ObjCAutoreleasePoolStmt:					return "ObjectiveCAutoreleasePoolStmt";
		case CXCursor_ObjCForCollectionStmt:					return "ObjectiveCForCollectionStmt";
		case CXCursor_CXXCatchStmt:								return "CxxCatchStmt";
		case CXCursor_CXXTryStmt:								return "CxxTryStmt";
		case CXCursor_CXXForRangeStmt:							return "CxxForRangeStmt";
		case CXCursor_SEHTryStmt:								return "SEHTryStmt";
		case CXCursor_SEHExceptStmt:							return "SEHExceptStmt";
		case CXCursor_SEHFinallyStmt:							return "SEHFinallyStmt";
		case CXCursor_MSAsmStmt:								return "MSAsmStmt";
		case CXCursor_NullStmt:									return "NullStmt";
		case CXCursor_DeclStmt:									return "DeclStmt";
		case CXCursor_OMPParallelDirective:						return "OMPParallelDirective";
		case CXCursor_OMPSimdDirective:							return "OMPSimdDirective";
		case CXCursor_OMPForDirective:							return "OMPForDirective";
		case CXCursor_OMPSectionsDirective:						return "OMPSectionsDirective";
		case CXCursor_OMPSectionDirective:						return "OMPSectionDirective";
		case CXCursor_OMPSingleDirective:						return "OMPSingleDirective";
		case CXCursor_OMPParallelForDirective:					return "OMPParallelForDirective";
		case CXCursor_OMPParallelSectionsDirective:				return "OMPParallelSectionsDirective";
		case CXCursor_OMPTaskDirective:							return "OMPTaskDirective";
		case CXCursor_OMPMasterDirective:						return "OMPMasterDirective";
		case CXCursor_OMPCriticalDirective:						return "OMPCriticalDirective";
		case CXCursor_OMPTaskyieldDirective:					return "OMPTaskyieldDirective";
		case CXCursor_OMPBarrierDirective:						return "OMPBarrierDirective";
		case CXCursor_OMPTaskwaitDirective:						return "OMPTaskwaitDirective";
		case CXCursor_OMPFlushDirective:						return "OMPFlushDirective";
		case CXCursor_SEHLeaveStmt:								return "SEHLeaveStmt";
		case CXCursor_OMPOrderedDirective:						return "OMPOrderedDirective";
		case CXCursor_OMPAtomicDirective:						return "OMPAtomicDirective";
		case CXCursor_OMPForSimdDirective:						return "OMPForSimdDirective";
		case CXCursor_OMPParallelForSimdDirective:				return "OMPParallelForSimdDirective";
		case CXCursor_OMPTargetDirective:						return "OMPTargetDirective";
		case CXCursor_OMPTeamsDirective:						return "OMPTeamsDirective";
		case CXCursor_OMPTaskgroupDirective:					return "OMPTaskgroupDirective";
		case CXCursor_OMPCancellationPointDirective:			return "OMPCancellationPointDirective";
		case CXCursor_OMPCancelDirective:						return "OMPCancelDirective";
		case CXCursor_OMPTargetDataDirective:					return "OMPTargetDataDirective";
		case CXCursor_OMPTaskLoopDirective:						return "OMPTaskLoopDirective";
		case CXCursor_OMPTaskLoopSimdDirective:					return "OMPTaskLoopSimdDirective";
		case CXCursor_OMPDistributeDirective:					return "OMPDistributeDirective";
		case CXCursor_OMPTargetEnterDataDirective:				return "OMPTargetEnterDataDirective";
		case CXCursor_OMPTargetExitDataDirective:				return "OMPTargetExitDataDirective";
		case CXCursor_OMPTargetParallelDirective:				return "OMPTargetParallelDirective";
		case CXCursor_OMPTargetParallelForDirective:			return "OMPTargetParallelForDirective";
		case CXCursor_OMPTargetUpdateDirective:					return "OMPTargetUpdateDirective";
		case CXCursor_OMPDistributeParallelForDirective:		return "OMPDistributeParallelForDirective";
		case CXCursor_OMPDistributeParallelForSimdDirective:	return "OMPDistributeParallelForSimdDirective";
		case CXCursor_OMPDistributeSimdDirective:				return "OMPDistributeSimdDirective";
		case CXCursor_OMPTargetParallelForSimdDirective:		return "OMPTargetParallelForSimdDirective";
		case CXCursor_TranslationUnit:							return "TranslationUnit";
		case CXCursor_UnexposedAttr:							return "UnexposedAttr";
		case CXCursor_IBActionAttr:								return "IBActionAttr";
		case CXCursor_IBOutletAttr:								return "IBOutletAttr";
		case CXCursor_IBOutletCollectionAttr:					return "IBOutletCollectionAttr";
		case CXCursor_CXXFinalAttr:								return "CxxFinalAttr";
		case CXCursor_CXXOverrideAttr:							return "CxxOverrideAttr";
		case CXCursor_AnnotateAttr:								return "AnnotateAttr";
		case CXCursor_AsmLabelAttr:								return "AsmLabelAttr";
		case CXCursor_PackedAttr:								return "PackedAttr";
		case CXCursor_PureAttr:									return "PureAttr";
		case CXCursor_ConstAttr:								return "ConstAttr";
		case CXCursor_NoDuplicateAttr:							return "NoDuplicateAttr";
		case CXCursor_CUDAConstantAttr:							return "CUDAConstantAttr";
		case CXCursor_CUDADeviceAttr:							return "CUDADeviceAttr";
		case CXCursor_CUDAGlobalAttr:							return "CUDAGlobalAttr";
		case CXCursor_CUDAHostAttr:								return "CUDAHostAttr";
		case CXCursor_CUDASharedAttr:							return "CUDASharedAttr";
		case CXCursor_VisibilityAttr:							return "VisibilityAttr";
		case CXCursor_DLLExport:								return "DLLExport";
		case CXCursor_DLLImport:								return "DLLImport";
		case CXCursor_PreprocessingDirective:					return "PreprocessingDirective";
		case CXCursor_MacroDefinition:							return "MacroDefinition";
		case CXCursor_MacroExpansion:							return "MacroExpansion";
		//case CXCursor_MacroInstantiation:						return "MacroInstantiation";
		case CXCursor_InclusionDirective:						return "InclusionDirective";
		case CXCursor_ModuleImportDecl:							return "ModuleImportDecl";
		case CXCursor_TypeAliasTemplateDecl:					return "TypeAliasTemplateDecl";
		case CXCursor_StaticAssert:								return "StaticAssert";
		case CXCursor_OverloadCandidate:						return "OverloadCandidate";
	}

	return "Unknown";
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
