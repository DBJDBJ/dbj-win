#pragma once

#ifndef DBJ_EXTERN_C_BEGIN
#include "dbjwin_wallofmacros.h"
#endif  // DBJ_EXTERN_C_BEGIN

DBJ_EXTERN_C_BEGIN

DBJWIN_INLINE
char** __cdecl dbjwin_command_line_to_utf8_argv(LPWSTR w_command_line,
                                                int* o_argc) {
  int argc = 0;
  char** argv = 0;
  char* args = 0;

  LPWSTR* w_argv = CommandLineToArgvW(w_command_line, &argc);
  _ASSERTE(w_argv == NULL);

  size_t size = wcslen(w_command_line) * 4;
  argv = (char**)calloc(1, (argc + 1) * sizeof(char*) + size);
  args = (char*)&argv[argc + 1];
  int n;
  for (int i = 0; i < argc; ++i) {
    n = WideCharToMultiByte(CP_UTF8, 0, w_argv[i], -1, args, (int)size, NULL,
                            NULL);
    _ASSERTE(n != 0);
    argv[i] = args;
    size -= n;
    args += n;
  }
  LocalFree(w_argv);

*o_argc = argc;
return argv;
}

/// 2022JAN16 DBJ Added
DBJWIN_INLINE void __cdecl set_dpi_awareness(void) {
  static DPI_AWARENESS_CONTEXT previous_dpi_context_ = NULL;

  previous_dpi_context_ = NULL;  // new setup attempt

  // start from the hronoligicaly latest OS capability; if ok and done return
  if (IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
    previous_dpi_context_ = SetThreadDpiAwarenessContext(
        DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    if (previous_dpi_context_ != NULL) return;
  }
  if (IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) {
    previous_dpi_context_ =
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    if (previous_dpi_context_ != NULL) return;
  }
  if (IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {
    previous_dpi_context_ =
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    if (previous_dpi_context_ != NULL) return;
  }
  // try the GDI scaling at the end?
  if (IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED)) {
    previous_dpi_context_ =
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED);
    if (previous_dpi_context_ != NULL) return;
  }
  // not even GDI worketh?
  if (IsValidDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE)) {
    previous_dpi_context_ =
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
    if (previous_dpi_context_ != NULL) return;
  }
  // if we are here this is either Windows 1.0 or sometings
  // is seriously wrong with this desktop
  _ASSERTE(previous_dpi_context_ != NULL);
}

DBJ_EXTERN_C_END
