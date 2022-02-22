#pragma once
// (c) 2022 dbj at dbj dot org

#ifndef STRICT
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif  // ! STRICT

#include "dbjwin_dbg.h"
#include "dbjwin_wallofmacros.h"

DBJWIN_EXTERN_C_BEGIN

__inline void* __stdcall dbjwin_alloc(ARR_SZE_) {
  void* rez_ = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ARR_SZE_);
  if (rez_ == NULL) DBJWIN_LAST_TERROR();
  return rez_;
}

__inline void __stdcall dbjwin_free(void * ptr_) {
  BOOL result = HeapFree(GetProcessHeap(), 0, ptr_);
  if (result == FALSE) DBJWIN_LAST_TERROR();
}

/*
-------------------------------------------------------------------------------------
 from https://news.ycombinator.com/item?id=11305190

 usage:
  dbjwin_autofree_ char *s1 = dbjwin_alloc(0xFF);
  */
#define dbjwin_autofree_ __attribute((cleanup(dbjcapi_autofree_func_)))

// take great care dbjwin_alloc was used
static inline void dbjcapi_autofree_func_(void *ptr_ptr) {
  DBJWIN_ASSERT(ptr_ptr);  //
  if (!ptr_ptr) return;    // added

  void *ptr = *(void **)ptr_ptr;
  dbjwin_free(ptr);
}

DBJWIN_EXTERN_C_END
