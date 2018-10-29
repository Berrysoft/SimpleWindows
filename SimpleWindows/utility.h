#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#ifndef UNICODE
#error This library only supports Unicode charset.
#endif // !UNICODE

#if !defined(SW_CXX11) && (__cplusplus >= 201103L || _MSVC_LANG >= 201103L)
#define SW_CXX11
#endif

#if !defined(SW_CXX14) && (__cplusplus >= 201402L || _MSVC_LANG >= 201402L)
#define SW_CXX14
#endif

#if !defined(SW_CXX17) && (__cplusplus >= 201703L || _MSVC_LANG >= 201703L)
#define SW_CXX17
#endif

#ifndef SW_CONSTEXPR
#if defined(SW_CXX14) || _MSC_VER >= 1910
#define SW_CONSTEXPR constexpr
#define SW_CONSTEXPR_DECL constexpr
#elif defined(SW_CXX11) || _MSC_VER >= 1900
#define SW_CONSTEXPR inline
#define SW_CONSTEXPR_DECL constexpr
#else
#define SF_CONSTEXPR inline
#define SF_CONSTEXPR_DECL const
#endif
#endif // !SW_CONSTEXPR

#ifndef SW_NOEXCEPT
#if defined(SW_CXX11) || _MSC_VER >= 1900
#define SW_NOEXCEPT noexcept
#else
#define SW_NOEXCEPT throw()
#endif
#endif // !SW_NOEXCEPT

#ifndef SW_NULLPTR
#if defined(SW_CXX11) || _MSC_VER >= 1600
#define SW_NULLPTR nullptr
#elif defined(NULL)
#define SW_NULLPTR NULL
#else
#define SW_NULLPTR 0
#endif
#endif // !SW_NULLPTR

#ifdef _DEBUG
#ifndef SW_ASSERT
#define SW_ASSERT(expr) _ASSERTE(expr)
#endif // !SW_ASSERT
#ifndef SW_ASSERT_EXPR
#define SW_ASSERT_EXPR(expr, msg) _ASSERT_EXPR(expr, msg)
#endif // !SW_ASSERT_EXPR
#else
#ifndef SW_ASSERT
#define SW_ASSERT(expr) (expr)
#endif // !SW_ASSERT
#ifndef SW_ASSERT_EXPR
#define SW_ASSERT_EXPR(expr, msg) (expr)
#endif // !SW_ASSERT_EXPR
#endif // _DEBUG
