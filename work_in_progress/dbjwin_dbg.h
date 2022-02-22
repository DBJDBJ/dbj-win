#pragma once
/*
it seems in here we do not depend on CRT
name 'debug' is used over 'assert' to mean exactly that
*/
#ifndef __clang__
#error Sorry, clang only ... GCC might do to
#endif

#if STRICT != 1
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>  // debugapi.h used in here
#endif                // ! STRICT
#include <crtdbg.h>  // it was concluded 2022 JAN it is not feasible to try and build without CRT

#include "dbjwin_strsafe.h"
#include "dbjwin_wallofmacros.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBJWIN_ASSERT _ASSERTE

// terminating error with a message box
// error message number is coming from GetLastError()

#undef DBJWIN_LAST_TERROR_
#define DBJWIN_LAST_TERROR_(...) dbjwin_last_terror_(__FILE__, __LINE__)

DBJWIN_NRET_FUNCTION
__inline void __stdcall dbjwin_last_terror_(const char* file_,
                                            const long line_) {
  wchar_t sys_msg[1024] = {0};
  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL, GetLastError(),
                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sys_msg,
                 (sizeof(sys_msg) / sizeof(wchar_t)), NULL);

  wchar_t buf[2048] = {0};

  dbjwin_sprintfw(buf, (sizeof(buf) / sizeof(wchar_t)),
                  L"\bfile: %S\nline: %d\nSystem msg: %s\n", file_, line_,
                  sys_msg);

  MessageBoxW(NULL, buf, L"dbjwin terminating error! Exiting!", MB_OK);

  ExitProcess(0);
}

/*
unlike assert we do evaluate the expression always
regardles of the build
but we do the output the offending exception if this is a debug build
in any kind of build we exit if !!(exp) result is false
*/
#undef DBJWIN_VERIFY_
#undef DBJWIN_VERIFY

#define DBJWIN_VERIFY_(R_, F_, L_) \
  (void)(R_ ? 0 : dbjwin_last_terror_(F_, L_) )

#define DBJWIN_VERIFY(E_) \
  (SetLastError(0), DBJWIN_VERIFY_(!!(E_), __FILE__, __LINE__))

// if it has failed report on test failure; always proceed
// A || B -- if A is TRUE evaluation stops, ditto
// if !!(expr) is true this whole macro compiles to nothing
// if it is false the error message shows in the debugger output window
// in release mode _CrtDbgReportW compiles to nothing
// if it failes, debug break is called
// otherwise it proceeds because the first argument is not _CRT_ASSERT
// this work only in debug builds

#undef DBJWIN_DEBUG_TEST
#ifdef _DEBUG
#define DBJWIN_DEBUG_TEST(expr)                                               \
  (void)((!!(expr)) ||                                                  \
         (1 != _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL,       \
                             "%s | DBJWIN_TEST ERROR | %s\n", __TIME__, \
                             (#expr))) ||                               \
         (_CrtDbgBreak(), 0))
#else
#define DBJWIN_DEBUG_TEST(expr) ((void)(expr))
#endif

// NARROW version
// this work only in debug builds
#undef DBJWIN_RPT
#ifdef _DEBUG
#define DBJWIN_RPT(F_, ...) \
  _CrtDbgReport(_CRT_WARN, __FILE__, __LINE__, NULL, F_, __VA_ARGS__)
#else
#define DBJWIN_RPT(F_, ...) ((void)0)
#endif

#ifdef __cplusplus
}  // extern "C" {
#endif