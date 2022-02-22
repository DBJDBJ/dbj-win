#pragma once
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <crtdbg.h>

#ifdef _DEBUG
/*
this means we need to debug from Visual Studio (or VS Code using inner
terminal), to actually see the debug output which in turn means we do not need
to build console testing apps which in turn is moving us closer to real life
runtimes inside services or win32 desktop apps or some such thing
*/
__inline void DBJ_DBG_PRINT(const char* format_, ...) {
  char buffy[1024] = {0};  // magical number; not exemplary
  va_list args = {0};
  va_start(args, format_);
  int nBuf = _vsnprintf_s(buffy, 1024, 1024, format_, args);
  if (nBuf > -1)
    OutputDebugStringA(buffy);
  else
    OutputDebugStringA(__FILE__ " OutputDebugStringA buffer overflow\n");
  va_end(args);
}
#else
#define DBJ_DBG_PRINT(format_, ...) ((void)0)
#endif

// TERROR == Terminating Errpr
#define DBJ_TERROR(format_, ...)         \
  do {                                   \
    DBJ_DBG_PRINT(format_, __VA_ARGS__); \
    exit(1 /*EXIT_FAILURE*/);            \
  } while (0);

#ifndef SUPR4996
/*
SUPRESS this pesky warning 4996

(c) 2021 by dbj@dbj.org

do not have to put it on separate previous line from the offending call
sometimes one must put it immediately before . Example


if ( SUPR4996 sscanf(line, "%[^=] = \"%[^\"]\"", key, value) == 2
        || SUPR4996 sscanf(line, "%[^=] = '%[^\']'", key, value) == 2)
{
}

only that will silence the cl.exe compiler

*/
#ifdef _MSC_VER
#define SUPR4996 __pragma(warning(suppress : 4996))
#else
#define SUPR4996
#endif

#endif  // ! SUPR4996