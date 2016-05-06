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

#ifndef __STDMOVE_H_
#define __STDMOVE_H_
#pragma once

#pragma warning(push, 4)				// Enable maximum compiler warnings

//---------------------------------------------------------------------------
// std::move replacement
//---------------------------------------------------------------------------

namespace std {

	// remove reference
	template<class _Ty> struct remove_reference
	{
		typedef _Ty type;
	};

	// remove reference
	template<class _Ty> struct remove_reference<_Ty&>
	{
		typedef _Ty type;
	};

	// remove rvalue reference
	template<class _Ty> struct remove_reference<_Ty&&>
	{
		typedef _Ty type;
	};

	// forward _Arg as movable
#if _MSC_VER >= 1900
	template<class _Ty> inline constexpr typename remove_reference<_Ty>::type&& move(_Ty&& _Arg) noexcept
#else
	template<class _Ty> inline typename remove_reference<_Ty>::type&& move(_Ty&& _Arg)
#endif
	{
		return (static_cast<typename remove_reference<_Ty>::type&&>(_Arg));
	}

}	// namespace std

//---------------------------------------------------------------------------

#pragma warning(pop)

#endif	// __STDMOVE_H_
