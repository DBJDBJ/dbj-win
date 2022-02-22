#pragma once
#ifndef DBJWIN_WALL_OF_MACROS_INCLUDED
#define DBJWIN_WALL_OF_MACROS_INCLUDED
/* 
(c) 2019-2022 by dbj.org   -- https://dbj.org/license_dbj/ 

NOTICE here there is no includes. That makes this file benign. Keep it like that.
*/

#ifndef _MSC_VER
#error You are probably not using cl.exe or any compatible compiler ..
#endif  // ! _MSC_VER

#ifdef __clang__
#pragma clang system_header
#else
#error Sorry, by executive order clang-cl must be used ...
#endif // ! __clang__

#ifndef _WIN32
#error Sorry, it seems your compiler is OK but you are not buidling a WIN32 or WIN64 app...
#endif // _WIN32

// CAVEAT EMPTOR: since this is WIN32 exclusive teritory we build using MSVC 
//                which is a lie. 
//                clang-cl is a mandate
#if defined(__clang__)
#define DBJWIN_CLANG
#endif
//                GCC on WIN? MINGW? CYGWIN? Unlikely
#undef DBJWIN_GCC

#ifdef __cplusplus
#define DBJWIN_EXTERN_C_BEGIN extern "C" {
#define DBJWIN_EXTERN_C_END } 
#else
#define DBJWIN_EXTERN_C_BEGIN
#define DBJWIN_EXTERN_C_END  
#endif

// -----------------------------------------------------------------------------
// https://stackoverflow.com/a/2765211/10870835
// I am also using __inline due to the fact strsafe.h is using it
// that means `static inline` is not mandatory and
// macros are not mandatory (perhaps)
//
// I am doing undef here as I do not want to have a single definition and then 
// overly complicate thing by including a "single to plevel header"
// thus this definition will apear elsewhere in the `dbjwin` lib
#undef DBJWIN_INLINE
#define DBJWIN_INLINE __inline

/*-------------------------------------------------------------------------------*/
#ifdef DBJWIN_CLANG
#define DBJWIN_PURE_FUNCTION __attribute__((const))
#define DBJWIN_CTOR_FUNCTION __attribute__((constructor))
#define DBJWIN_DTOR_FUNCTION __attribute__((destructor))
#define DBJWIN_NRET_FUNCTION __attribute__((noreturn))
#define      DBJWIN_UNUSED_F __attribute__((unused))
#else
#error Sorry must use clang
#endif

// Here's a better C version (from Google's Chromium project):
#undef   DBJWIN_COUNT_OF
#define DBJWIN_COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

/// https://stackoverflow.com/a/29253284/10870835

// NDEBUG *is* standard macro and has to exist.
// it is also crucial to assert.h
// wew do not use CRT assert but we do confirm to NDEBUG mandate
#if ! defined (_DEBUG) 
#if ! defined (NDEBUG)
#define NDEBUG
#endif
#endif

#undef DBJWIN_RELEASE_BUILD
#ifdef NDEBUG 
#define DBJWIN_RELEASE_BUILD
#endif

/// -------------------------------------------------------------------------------
/// stolen from vcruntime.h 
#define DBJWIN_STRINGIZE_(x) #x
#define DBJWIN_STRINGIZE(x) DBJWIN_STRINGIZE_(x)

#define DBJWIN_WIDE_(s) L##s
#define DBJWIN_WIDE(s) DBJWIN_WIDE_(s)

#define DBJWIN_CONCATENATE_(a, b) a##b
#define DBJWIN_CONCATENATE(a, b) DBJWIN_CONCATENATE_(a, b)

#define DBJWIN_EXPAND_(s) s
#define DBJWIN_EXPAND(s) DBJWIN_EXPAND_(s)

/// -------------------------------------------------------------------------------
/// this might be rellevant only to C++ compile time code
#ifdef _MSVC_LANG
// https://developercommunity.visualstudio.com/content/problem/195665/-line-cannot-be-used-as-an-argument-for-constexpr.html
#define DBJWIN_CONSTEXPR_LINE long(_DBJWIN_CONCATENATE(__LINE__, U))
#else
#define DBJWIN_CONSTEXPR_LINE __LINE__
#endif



// -----------------------------------------------------------------------------

/*
we use the macros bellow to create ever needed location info always
associated with the offending expression
timestamp included
*/
#undef  DBJWIN_FILE_LINE 
#define DBJWIN_FILE_LINE __FILE__ "(" _CRT_STRINGIZE(__LINE__) ")"

#undef  DBJWIN_FILE_LINE_TSTAMP
#define DBJWIN_FILE_LINE_TSTAMP __FILE__ "(" _CRT_STRINGIZE(__LINE__) ")[" __TIMESTAMP__ "] "

#undef  DBJWIN_FLT_PROMPT
#define DBJWIN_FLT_PROMPT(x) DBJWIN_FILE_LINE_TSTAMP _CRT_STRINGIZE(x)

/* will not compile if MSG_ is not string literal */
#undef  DBJWIN_ERR_PROMPT
#define DBJWIN_ERR_PROMPT(MSG_) DBJWIN_FILE_LINE_TSTAMP MSG_



#endif // DBJWIN_WALL_OF_MACROS_INCLUDED
