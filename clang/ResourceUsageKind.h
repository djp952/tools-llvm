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

#ifndef __RESOURCEUSAGEKIND_H_
#define __RESOURCEUSAGEKIND_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

using namespace System;

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// Class ResourceUsageKind
//
// Categorizes how memory is being used by a translation unit
//---------------------------------------------------------------------------

public value class ResourceUsageKind
{
public:

	//-----------------------------------------------------------------------
	// Overloaded Operators

	// operator== (static)
	//
	static bool operator==(ResourceUsageKind lhs, ResourceUsageKind rhs);

	// operator!= (static)
	//
	static bool operator!=(ResourceUsageKind lhs, ResourceUsageKind rhs);

	//-----------------------------------------------------------------------
	// Member Functions

	// Equals
	//
	// Overrides Object::Equals()
	virtual bool Equals(Object^ rhs) override;

	// Equals
	//
	// Compares this ResourceUsageKind to another ResourceUsageKind
	bool Equals(ResourceUsageKind rhs);

	// GetHashCode
	//
	// Overrides Object::GetHashCode()
	virtual int GetHashCode(void) override;

	// ToString
	//
	// Overrides Object::ToString()
	virtual String^ ToString(void) override;

	//-----------------------------------------------------------------------
	// Fields

	static initonly ResourceUsageKind ASTContext						= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_AST);
	static initonly ResourceUsageKind ASTContextIdentifiers				= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_Identifiers);
	static initonly ResourceUsageKind ASTContextSelectors				= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_Selectors);
	static initonly ResourceUsageKind CodeCompletionGlobalResults		= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_GlobalCompletionResults);
	static initonly ResourceUsageKind SourceManagerContentCache			= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_SourceManagerContentCache);
	static initonly ResourceUsageKind ASTContextSideTables				= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_AST_SideTables);
	static initonly ResourceUsageKind SourceManager						= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_Membuffer_Malloc);
	static initonly ResourceUsageKind SourceManagerMapped				= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_Membuffer_MMap);
	static initonly ResourceUsageKind ExternalASTSource					= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_ExternalASTSource_Membuffer_Malloc);
	static initonly ResourceUsageKind ExternalASTSourceMapped			= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_ExternalASTSource_Membuffer_MMap);
	static initonly ResourceUsageKind Preprocessor						= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_Preprocessor);
	static initonly ResourceUsageKind PreprocessingRecord				= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_PreprocessingRecord);
	static initonly ResourceUsageKind SourceManagerDataStructures		= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_SourceManager_DataStructures);
	static initonly ResourceUsageKind PreprocessorHeaderSearchTable		= ResourceUsageKind(CXTUResourceUsageKind::CXTUResourceUsage_Preprocessor_HeaderSearch);

internal:

	// Instance Constructor
	//
	ResourceUsageKind(CXTUResourceUsageKind kind);

private:

	//-----------------------------------------------------------------------
	// Member Variables

	CXTUResourceUsageKind		m_kind;			// Underlying CXTUResourceUsageKind
};

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)

#endif	// __RESOURCEUSAGEKIND_H_
