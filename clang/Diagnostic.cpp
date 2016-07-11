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
#include "Diagnostic.h"

#include "DiagnosticChildCollection.h"
#include "DiagnosticCategory.h"
#include "DiagnosticCollection.h"
#include "DiagnosticDisplayOptions.h"
#include "DiagnosticEmphasisCollection.h"
#include "DiagnosticFixItCollection.h"
#include "DiagnosticSeverity.h"
#include "Location.h"
#include "LocationKind.h"
#include "StringUtil.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

namespace zuki::tools::llvm::clang {

//---------------------------------------------------------------------------
// Diagnostic Constructor
//
// Arguments:
//
//	handle			- Underlying DiagnosticHandle instance

Diagnostic::Diagnostic(DiagnosticHandle^ handle) : m_handle(handle)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
}

//---------------------------------------------------------------------------
// Diagnostic::Category::get
//
// Gets the category of this diagnostic

DiagnosticCategory Diagnostic::Category::get(void)
{
	return DiagnosticCategory(static_cast<CXDiagnostic>(DiagnosticHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Diagnostic::Children::get
//
// Gets a collection of child diagnostics

DiagnosticCollection^ Diagnostic::Children::get(void)
{
	if(Object::ReferenceEquals(m_children, nullptr))
		m_children = DiagnosticChildCollection::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getChildDiagnostics(DiagnosticHandle::Reference(m_handle)));

	return m_children;
}

//---------------------------------------------------------------------------
// Diagnostic::Create (internal, static)
//
// Creates a new Diagnostic instance
//
// Arguments:
//
//	owner			- SafeHandle that owns the lifetime of this diagnostic
//	transunit		- TranslationUnitHandle instance
//	diagnostic		- Unmanaged CXDiagnostic instance

Diagnostic^ Diagnostic::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXDiagnostic diagnostic)
{
	return gcnew Diagnostic(gcnew DiagnosticHandle(owner, transunit, diagnostic));
}

//---------------------------------------------------------------------------
// Diagnostic::DisableOption::get
//
// Gets the command-line option that disables this diagnostic

String^ Diagnostic::DisableOption::get(void)
{
	if(Object::ReferenceEquals(m_disableopt, nullptr)) GetOptions();
	return m_disableopt;
}

//---------------------------------------------------------------------------
// Diagnostic::Emphases::get
//
// Gets a collection of emphasis extents associated with this diagnostic

DiagnosticEmphasisCollection^ Diagnostic::Emphases::get(void)
{
	if(Object::ReferenceEquals(m_emphases, nullptr)) 
		m_emphases = DiagnosticEmphasisCollection::Create(m_handle->Owner, m_handle->TranslationUnit, DiagnosticHandle::Reference(m_handle));

	return m_emphases;
}

//---------------------------------------------------------------------------
// Diagnostic::EnableOption::get
//
// Gets the command-line option that enables this diagnostic

String^ Diagnostic::EnableOption::get(void)
{
	if(Object::ReferenceEquals(m_enableopt, nullptr)) GetOptions();
	return m_enableopt;
}

//---------------------------------------------------------------------------
// Diagnostic::FixIts::get
//
// Gets a collection of fix-its associated with this diagnostic

DiagnosticFixItCollection^ Diagnostic::FixIts::get(void)
{
	if(Object::ReferenceEquals(m_fixits, nullptr)) 
		m_fixits = DiagnosticFixItCollection::Create(m_handle->Owner, m_handle->TranslationUnit, DiagnosticHandle::Reference(m_handle));

	return m_fixits;
}

//---------------------------------------------------------------------------
// Diagnostic::Format
//
// Format the given diagnostic in a manner that is suitable for display
//
// Arguments:
//
//	NONE

String^ Diagnostic::Format(void)
{
	return Format(static_cast<DiagnosticDisplayOptions>(-1));
}

//---------------------------------------------------------------------------
// Diagnostic::Format
//
// Format the given diagnostic in a manner that is suitable for display
//
// Arguments:
//
//	options		- Option flags indicating how the string should be formatted

String^ Diagnostic::Format(DiagnosticDisplayOptions options)
{
	// If the special -1 option was specified, ask clang to provide the default options
	if(options == static_cast<DiagnosticDisplayOptions>(-1))
		options = static_cast<DiagnosticDisplayOptions>(clang_defaultDiagnosticDisplayOptions());

	// Retrieve the formatted diagnostic string based on the requested options
	return StringUtil::ToString(clang_formatDiagnostic(DiagnosticHandle::Reference(m_handle), static_cast<unsigned int>(options)));
}

//---------------------------------------------------------------------------
// Diagnostic::GetOptions (private)
//
// Loads the enable/disable command line option strings for this diagnostic
//
// Arguments:
//
//	NONE

void Diagnostic::GetOptions(void)
{
	CXString			disableopt;			// Disable option string

	// clang_getDiagnosticOption returns both strings, and they both have to be disposed of
	m_enableopt = StringUtil::ToString(clang_getDiagnosticOption(DiagnosticHandle::Reference(m_handle), &disableopt));
	m_disableopt = StringUtil::ToString(std::move(disableopt));
}

//---------------------------------------------------------------------------
// Diagnostic::Location::get
//
// Gets the spelling source location of the diagnostic

local::Location^ Diagnostic::Location::get(void)
{
	if(Object::ReferenceEquals(m_location, nullptr))
		m_location = local::Location::Create(m_handle->Owner, m_handle->TranslationUnit, clang_getDiagnosticLocation(DiagnosticHandle::Reference(m_handle)), LocationKind::Spelling);

	return m_location;
}

//---------------------------------------------------------------------------
// Diagnostic::Severity::get
//
// Gets the severity level of the diagnostic

DiagnosticSeverity Diagnostic::Severity::get(void)
{
	return DiagnosticSeverity(clang_getDiagnosticSeverity(DiagnosticHandle::Reference(m_handle)));
}

//---------------------------------------------------------------------------
// Diagnostic::Spelling::get
//
// Gets the text associated with the diagnostic

String^ Diagnostic::Spelling::get(void)
{
	if(Object::ReferenceEquals(m_spelling, nullptr))
		m_spelling = StringUtil::ToString(clang_getDiagnosticSpelling(DiagnosticHandle::Reference(m_handle)));

	return m_spelling;
}

//---------------------------------------------------------------------------
// Diagnostic::ToString
//
// Overrides Object::ToString()
//
// Arguments:
//
//	NONE

String^ Diagnostic::ToString(void)
{
	// [Location]: [Severity]: [Spelling]
	return String::Format("{0}: {1}: {2}", this->Location->ToString(), this->Severity.ToString(), this->Spelling);
}

//---------------------------------------------------------------------------

} // zuki::tools::llvm::clang

#pragma warning(pop)
