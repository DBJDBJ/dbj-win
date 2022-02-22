#pragma once

#ifndef __clang__
#error Sorry, clang is mandatory
#endif  // __clang__

/*

  For no-console windows desktop apps we use as the last resort log file
  epsecially in release builds where there is no OutputDebugString

  CAVEAT EMPTOR: this is safe and slow approach. If speed is primary concern, do
not use

 (c) 2021 - 2022 by dbj@dbj.org --  https://dbj.org/license_dbj

include exactly once like this

#define DBJ_WIN32_LOGFILE_IMPLEMENTATION
#include <dbjwin_log_file.h>

*/

//#ifndef _WIN32_WINNT_WIN10
//#define _WIN32_WINNT_WIN10 0x0A00
//#endif  // _WIN32_WINNT_WIN10
//
//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT _WIN32_WINNT_WIN10
//#endif
//
//#ifndef WINVER
//#define WINVER _WIN32_WINNT_WIN10
//#endif

#if STRICT != 1
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

#include "dbjwin_dbg.h"
#include "dbjwin_strsafe.h"
#include "dbjwin_wallofmacros.h"

#ifdef __cplusplus
extern "C" {
#endif

#undef DBJWIN_TEST
#define DBJWIN_TEST(E_)                                                  \
  (void)(!!(E_) ? 0                                                      \
                : dbjwin_log_print(__FILE__, DBJWIN_STRINGIZE(__LINE__), \
                                   "DBJWIN TEST FAILED: %s", (#E_)))

// maybe primitive but very functional
#undef DBJWIN_LOG
#define DBJWIN_LOG(FMT_, ...) \
  dbjwin_log_print(__FILE__, DBJWIN_STRINGIZE(__LINE__), FMT_, __VA_ARGS__)

// single function API, risky ... but ...
void __stdcall dbjwin_log_print(const char* file_, const char* line_, char* str,
                                ...);

#ifdef DBJ_WIN32_LOGFILE_IMPLEMENTATION

// DBJ: added log file handle
HANDLE dbj_winlog_file_handle_ = 0;

__inline HANDLE __stdcall dbj_create_log_file(void) {
  int argc = 0;
  // be carefull with clever and short names like argw_
  // they are very lakely taken during decades of development
  LPWSTR* argw_ = CommandLineToArgvW(GetCommandLineW(), &argc);

  wchar_t buffy_[2048] = {L'\0'};
  LPWSTR log_file_name = memcpy(buffy_, argw_[0], 2048);
  DBJWIN_ASSERT(log_file_name);

#ifdef _DEBUG
  HRESULT hr_ =
#endif
      dbjwin_path_rename_extensionw(buffy_, 2048, L".log");

  DBJWIN_ASSERT(SUCCEEDED(hr_));

  HANDLE hFile = CreateFileW(log_file_name,          // name of the write
                             GENERIC_WRITE,          // open for writing
                             0,                      // do not share
                             NULL,                   // default security
                             CREATE_ALWAYS,          // create new file always
                             FILE_ATTRIBUTE_NORMAL,  // normal file
                             NULL);                  // no attr. template
  LocalFree(argw_);

  DBJWIN_ASSERT(hFile != INVALID_HANDLE_VALUE);

  DBJWIN_RPT("\nDBJWIN log file opened: %S\n", log_file_name);
  return hFile;
}

/*
this actually might be NOT slower than using some global CRITICAL_SECTION
and entering/deleting it only once ... why not measuring it?
*/
__inline void __stdcall dbjwin_logfilewriter_protector_function(
    CRITICAL_SECTION CS_, BOOL lock) {
  if (lock) {
    InitializeCriticalSection(&CS_);
    EnterCriticalSection(&CS_);
  } else {
    LeaveCriticalSection(&CS_);
    DeleteCriticalSection(&CS_);
  }
}

/* write direct to a log file */
__inline void __stdcall dbjwin_write_to_log_file(LPCSTR text_) {
  DBJWIN_ASSERT(text_);
  DBJWIN_ASSERT(dbj_winlog_file_handle_);

  DWORD dwBytesToWrite = (DWORD)strlen(text_);
  DWORD dwBytesWritten = 0;
#ifdef _DEBUG
  BOOL bErrorFlag =
#endif
      WriteFile(dbj_winlog_file_handle_,  // open file handle
                text_,                    // start of data to write
                dwBytesToWrite,           // number of bytes to write
                &dwBytesWritten,          // number of bytes that were written
                NULL);                    // no overlapped structure

  DBJWIN_ASSERT(FALSE != bErrorFlag);
}

// DBJ : just a bit less trivial
// NOTE: locking is applied insted of async call, thus
//       this is not very fast
void __stdcall dbjwin_log_print(const char* file_, const char* line_, char* str,
                                ...) {
  DBJWIN_ASSERT(file_ && line_);

  static CRITICAL_SECTION CS_;  // local to this function
  dbjwin_logfilewriter_protector_function(CS_, 1);

  char buffer[1024] = {0};
  va_list va = 0;
  va_start(va, str);
#ifdef _DEBUG
  HRESULT vsprintf_rezult =
#endif
      dbjwin_vsprintfa(buffer, 1024, str, va);
  DBJWIN_ASSERT(SUCCEEDED(vsprintf_rezult));
  va_end(va);

  dbjwin_write_to_log_file("\n");
  dbjwin_write_to_log_file(__TIME__);
  dbjwin_write_to_log_file("|");
  dbjwin_write_to_log_file(file_);
  dbjwin_write_to_log_file("|");
  dbjwin_write_to_log_file(line_);
  dbjwin_write_to_log_file("|");
  dbjwin_write_to_log_file(buffer);

  dbjwin_logfilewriter_protector_function(CS_, 0);
}

//-----------------------------------------------------------------
// note: yes I know, this is rigid and risky design/code
// note we do use _CRT_WARN
__attribute__((
    constructor)) __inline void __stdcall dbjwin_construct_dbg_infrastructure(void) {
  // 1. actually create the log file and get the HANDLE to it
  dbj_winlog_file_handle_ = dbj_create_log_file();
// 2. make it a default target for crtdbg infrastructure
//    BUT! Only for warning messages
#ifdef _DEBUG
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, dbj_winlog_file_handle_);
#endif

  dbjwin_write_to_log_file(
      "\n----------------------------------------------\n");
  dbjwin_write_to_log_file("DBJWIN LOG FILE      ");
  dbjwin_write_to_log_file(__TIMESTAMP__);
  dbjwin_write_to_log_file(
      "\n----------------------------------------------\n");

  // 3. wherever error or assert are used
  //    message box will popup,
  //    that is a default win32 app behaviour
#ifdef _DEBUG
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_WNDW);
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
#endif

  // NOTE: that all applies to the debug builds
}

__attribute__((
    destructor)) __inline void __stdcall dbjwin_destruct_dbg_infrastructure(void) {
  if (dbj_winlog_file_handle_) CloseHandle(dbj_winlog_file_handle_);
}

#endif  // DBJ_WIN32_LOGFILE_IMPLEMENTATION

#ifdef __cplusplus
}  // extern "C"
#endif
