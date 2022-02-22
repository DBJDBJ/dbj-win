#pragma once
/*
clang macros made slightly more confortable
2021 DBJ nicked from github "systemd" repo

NOTE: by executive order attribute macros are not all capital letters
      this there is a hope they are differentiated from real MACROS
*/
#include "dbjwin_dbg.h"

#define dbjwin_const_ __attribute__((__const__))
#define dbjwin_pure_ __attribute__((__pure__))
#define dbjwin_unused_ __attribute__((__unused__))
#define dbjwin_cleanup_(x) __attribute__((__cleanup__(x)))
#define dbjwin_printf_(a, b) __attribute__((__format__(printf, a, b)))
#define dbjwin_alloc_(...) __attribute__((__alloc_size__(__VA_ARGS__)))
#define dbjwin_sentinel_ __attribute__((__sentinel__))
#define dbjwin_section_(x) __attribute__((__section__(x)))
#define dbjwin_used_ __attribute__((__used__))
#define dbjwin_destructor_ __attribute__((__destructor__))
#define dbjwin_deprecated_ __attribute__((__deprecated__))
#define dbjwin_packed_ __attribute__((__packed__))
#define dbjwin_malloc_ __attribute__((__malloc__))
#define dbjwin_weak_ __attribute__((__weak__))
#define dbjwin_likely_(x) (__builtin_expect(!!(x), 1))
#define dbjwin_unlikely_(x) (__builtin_expect(!!(x), 0))
#define dbjwin_public_ __attribute__((__visibility__("default")))
#define dbjwin_hidden_ __attribute__((__visibility__("hidden")))
#define dbjwin_weakref_(x) __attribute__((__weakref__(#x)))
#define dbjwin_align_(x) __attribute__((__aligned__(x)))
#define dbjwin_alignas_(x) __attribute__((__aligned__(__alignof(x))))
#define dbjwin_alignptr_ __attribute__((__aligned__(sizeof(void *))))
#if __GNUC__ >= 7
#define dbjwin_fallthrough_ __attribute__((__fallthrough__))
#else
#define dbjwin_fallthrough_
#endif
/* Define C11 noreturn without <stdnoreturn.h> and even on older gcc
 * compiler versions */
#ifndef _noreturn_
#if __STDC_VERSION__ >= 201112L
#define dbjwin_noreturn_ _Noreturn
#else
#define dbjwin_noreturn_ __attribute__((__noreturn__))
#endif
#endif

/*
 * STRLEN - returns the length of a string literal, minus the trailing NUL byte.
 *          Contrary to strlen(), this is a constant expression.
 * @x: a string literal.
 */
#define DBJWIN_STRLEN(x) (sizeof("" x "") - 1)

/* Define C11 thread_local attribute even on older gcc compiler
 * version */
#ifndef thread_local
#ifndef __STDC_NO_THREADS__
#define thread_local _Thread_local
#else
#define thread_local __thread
#endif
#endif  // ! thread_local

