#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

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
