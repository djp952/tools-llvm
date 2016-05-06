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
#include "TokenCollection.h"

#include "Cursor.h"
#include "ReadOnlyListEnumerator.h"
#include "Token.h"
#include "TranslationUnitHandle.h"

#pragma warning(push, 4)				// Enable maximum compiler warnings

BEGIN_ROOT_NAMESPACE(zuki::tools::llvm::clang)

//---------------------------------------------------------------------------
// DisposeTokenSet
//
// Unmanaged disposal function for an array of CXTokens
//
// Arguments:
//
//	tokens		- Token set to be disposed of

void DisposeTokenSet(CXToken* tokens)
{
	// clang_disposeTokens ignores everything but the pointer itself, this
	// wrapper makes it compatible with UnmanagedTypeSafeHandle<>
	clang_disposeTokens(__nullptr, tokens, 0);
}

//---------------------------------------------------------------------------
// TokenCollection Constructor (private)
//
// Arguments:
//
//	handle		- Underlying TokenSetHandle instance
//	transunit	- TranslationUnitHandle instance
//	count		- Number of elements in the collection

TokenCollection::TokenCollection(TokenSetHandle^ handle, TranslationUnitHandle^ transunit, unsigned int count) : m_handle(handle), m_transunit(transunit), m_count(count)
{
	if(Object::ReferenceEquals(handle, nullptr)) throw gcnew ArgumentNullException("handle");
	if(Object::ReferenceEquals(transunit, nullptr)) throw gcnew ArgumentNullException("transunit");

	// Create a cache to store element object references as they are created
	m_cache = gcnew array<Token^>(count);

	// Allocate an unmanaged annotation cursor array the same size as the token collection
	try { m_annotations = new CXCursor[count]; }
	catch(Exception^) { throw gcnew OutOfMemoryException(); }

	// Annotate the tokens into the allocated unmanaged CXCursor array
	clang_annotateTokens(TranslationUnitHandle::Reference(transunit), TokenSetHandle::Reference(m_handle), count, m_annotations);
}

//---------------------------------------------------------------------------
// TokenCollection Destructor

TokenCollection::~TokenCollection()
{
	if(m_disposed) return;

	// Dispose of all cached Token instances
	for each(Token^ token in m_cache) delete token;

	delete m_handle;					// Release the safe handle
	this->!TokenCollection();			// Release the unmanaged memory
	m_disposed = true;					// Object is now in a disposed state
}

//---------------------------------------------------------------------------
// TokenCollection Finalizer

TokenCollection::!TokenCollection()
{
	if(m_annotations != __nullptr) delete[] m_annotations;
	m_annotations = __nullptr;
}

//---------------------------------------------------------------------------
// TokenCollection::Create (internal, static)
//
// Creates a new TokenCollection instance
//
// Arguments:
//
//	owner			- Owning safe handle instance
//	transunit		- Parent TranslationUnitHandle instance
//	tokens			- Unmanaged pointer to the set of tokens
//	numtokens		- The number of CXToken structures in the set

TokenCollection^ TokenCollection::Create(SafeHandle^ owner, TranslationUnitHandle^ transunit, CXToken*&& tokens, unsigned int numtokens)
{
	return gcnew TokenCollection(gcnew TokenSetHandle(owner, std::move(tokens)), transunit, numtokens);
}

//---------------------------------------------------------------------------
// TokenCollection::default[int]::get
//
// Gets the element at the specified index in the read-only list

Token^ TokenCollection::default::get(int index)
{
	CHECK_DISPOSED(m_disposed);

	// Check for a cached instance of the Token first, this will also validate
	// that the index is within bounds and throw an appropriate exception
	Token^ cached = m_cache[index];
	if(!Object::ReferenceEquals(cached, nullptr)) return cached;

	// Create a new token and cache it to prevent multiple creations
	m_cache[index] = Token::Create(m_handle, m_transunit, TokenSetHandle::Reference(m_handle)[index], m_annotations[index]);
	return m_cache[index];
}

//---------------------------------------------------------------------------
// TokenCollection::Count::get
//
// Gets the number of elements in the collection

int TokenCollection::Count::get(void)
{
	CHECK_DISPOSED(m_disposed);
	
	// This reports the count provided during construction, not the number of
	// tokens actually in the set to workaround a clang API bug (See Extent::GetTokens)
	return static_cast<int>(m_count);
}

//---------------------------------------------------------------------------
// TokenCollection::GetEnumerator
//
// Returns a generic IEnumerator<T> for the member collection
//
// Arguments:
//
//	NONE

IEnumerator<Token^>^ TokenCollection::GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return gcnew ReadOnlyListEnumerator<Token^>(this);
}

//---------------------------------------------------------------------------
// TokenCollection::IEnumerable_GetEnumerator
//
// Returns a non-generic IEnumerator for the member collection
//
// Arguments:
//
//	NONE

System::Collections::IEnumerator^ TokenCollection::IEnumerable_GetEnumerator(void)
{
	CHECK_DISPOSED(m_disposed);
	return GetEnumerator();
}

//---------------------------------------------------------------------------

END_ROOT_NAMESPACE(zuki::tools::llvm::clang)

#pragma warning(pop)
