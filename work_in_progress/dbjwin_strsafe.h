#pragma once
/*
 (c) 2020 - 2022 by dbj@dbj.org -- LICENSE_DBJ

 "normalization" of "crazy camel" strsafe names

 also the same with "pathcch.h" function, mashed in here too
*/
#include "dbjwin_dbg.h"
#include "dbjwin_wallofmacros.h"
// #include "strsafe_internal.h"

#ifndef STRICT
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <PathCch.h>
#include <strsafe.h>
#pragma comment(lib, "Pathcch.lib")

DBJWIN_EXTERN_C_BEGIN

/// ---------------------------------------------------------------------
/// pathcch functions
/// ---------------------------------------------------------------------
/*
returns HRESULT, must use SUCCEEDED or FAILED to check
example:
dbjwin_path_rename_extension( "c:\\readme.txt", sizeof("c:\\readme.txt"), "md"
); this is wide string only, undocumented as such
*/

#undef dbjwin_path_rename_extensionw
#define dbjwin_path_rename_extensionw(OUTBUF_, OUTBUF_SIZE_, NEW_EXTENSION_) \
  PathCchRenameExtension(OUTBUF_, OUTBUF_SIZE_, NEW_EXTENSION_)

/// ---------------------------------------------------------------------
/// strsafe functions
/// ---------------------------------------------------------------------

/*
for some reason strsafe uses its own Win32 typedefs and calls them "Deprecated"
thus we include windows.h
*/

// Used to check if the lenght of the string is smaller or equal to a limit
// if it is, S_OK is returned

#undef dbjwin_strlena
#define dbjwin_strlena StringCchLengthA

/*
returns HRESULT, must use SUCCEEDED or FAILED to check

fine, but consider: dbjwin_memccpy ... imp is bellow
*/

#undef dbjwin_strcata
#define dbjwin_strcata(DEST_, DEST_SIZE_, SRC_) \
  StringCchCatA(DEST_, DEST_SIZE_, SRC_)

DBJWIN_INLINE size_t dbjwin_strnlena(LPCSTR in_str_, size_t control_size_) {
  size_t rezult_ = 0;
  HRESULT hr_ = dbjwin_strlena(in_str_, control_size_, &rezult_);
#ifdef _DEBUG
  DBJWIN_ASSERT(SUCCEEDED(hr_));
#endif
  if (FAILED(hr_)) return 0;
  return rezult_;
}

#undef dbjwin_strlenw
#define dbjwin_strlenw StringCchLengthW

DBJWIN_INLINE size_t dbjwin_strnlenw(LPCWSTR in_str_, size_t control_size_) {
  size_t rezult_ = 0;
  HRESULT hr_ = dbjwin_strlenw(in_str_, control_size_, &rezult_);
#ifdef _DEBUG
  DBJWIN_ASSERT(SUCCEEDED(hr_));
#endif

  if (FAILED(hr_)) return 0;
  return rezult_;
}

/*
STRSAFEAPI StringCchPrintfA(  STRSAFE_LPSTR  pszDest,  size_t cchDest,
STRSAFE_LPCSTR pszFormat,  ...);
*/
#undef dbjwin_sprintfa
#define dbjwin_sprintfa StringCchPrintfA

#undef dbjwin_sprintfw
#define dbjwin_sprintfw StringCchPrintfW

// https://docs.microsoft.com/en-us/windows/win32/api/strsafe/nf-strsafe-stringcchvprintfa
/*

STRSAFEAPI StringCchVPrintfA(
  STRSAFE_LPSTR  pszDest,
  size_t         cchDest,
  STRSAFE_LPCSTR pszFormat,
  va_list        argList
);

*/
#undef dbjwin_vsprintfa
#define dbjwin_vsprintfa StringCchVPrintfA

#undef dbjwin_vsprintfw
#define dbjwin_vsprintfw StringCchVPrintfW

// STRSAFEAPI StringCchCopyNA([out] STRSAFE_LPSTR pszDest, [in] size_t cchDest,
//                           [in] STRSAFE_PCNZCH pszSrc, [in] size_t cchToCopy);
#undef dbjwin_strncpya
#define dbjwin_strncpya StringCchCopyNA

#undef dbjwin_strncpyw
#define dbjwin_strncpyw StringCchCopyNW

#pragma region memccpy implementation
// officialy not until C23?
// #if __STDC_VERSION__ <= 201710L
// #endif // __STDC_VERSION__ <= 201710L
//  but in here we do not care for CRT multi-decade confusions

/*

_memccpy for MSVC
https://developers.redhat.com/blog/2019/08/12/efficient-string-copying-and-concatenation-in-c

Example:

struct Book {
    int id;
    char name[0xFF];
};

static inline struct Book *book_populate(struct Book *bptr_,
                                         char *const new_name_) {
    assert(bptr_);
    bptr_->id = 13;
    memccpy(bptr_->name, new_name_, '\0', sizeof(bptr_->name) );
    return bptr_;
}

that memccpy above is *much faster* then snprintf(bptr_->name, 0xFF, "%s",
new_name_ );
*/
DBJWIN_INLINE void *__stdcall dbjwin_memccpy(void *dst, const void *src, int c,
                                             size_t n) {
  const unsigned char *s = (unsigned char *)src;
  unsigned char *ret = 0;
  for (ret = (unsigned char *)dst; n; ++ret, ++s, --n) {
    (*ret) = (*s);
    if ((unsigned char)*ret == (unsigned char)c) return (ret + 1);
  }
  return 0;
}

#pragma endregion  // memccpy implementation

DBJWIN_EXTERN_C_END