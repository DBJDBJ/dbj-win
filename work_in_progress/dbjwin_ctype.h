#pragma once
/// (c) 2019-2022 by dbj.org   -- https://dbj.org/license_dbj/

#include "dbjwin_wallofmacros.h"
#include "dbjwin_dbg.h"

#if STRICT != 1
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/*
https://thewindowsupdate.com/2019/02/18/how-can-i-check-in-win32-whether-a-unicode-character-is-any-kind-of-digit/

Category flag vs implementation of equivalent CRT function

C runtime	Category flags	                            Win32 function (if
exist)
---------------------------------------------------------------------------------
isalnum	    C1_ALPHA | C1_UPPER | C1_LOWER | C1_DIGIT IsCharAlphaNumeric sort of
isalpha	    C1_ALPHA | C1_UPPER | C1_LOWER	            IsCharAlpha sort of
isblank	    C1_BLANK
iscntrl	    C1_CNTRL
isdigit	    C1_DIGIT
isgraph	    C1_ALPHA | C1_UPPER | C1_LOWER |
            C1_DIGIT | C1_PUNCT
islower	    C1_LOWER	                                IsCharLower
isprint 	C1_ALPHA | C1_UPPER | C1_LOWER |
            C1_DIGIT | C1_PUNCT | C1_BLANK
ispunct	    C1_PUNCT
isspace	    C1_SPACE
isupper	    C1_UPPER	                                IsCharUpper
isxdigit	C1_XDIGIT

*/

DBJWIN_EXTERN_C_BEGIN

DBJWIN_INLINE BOOL dbjwin_is_alnum_w(wchar_t ch) {
  return IsCharAlphaNumericW(ch);
}

DBJWIN_INLINE BOOL dbjwin_is_alpha_w(wchar_t ch) { return IsCharAlpha(ch); }

// is unicode  char a blank
DBJWIN_INLINE BOOL dbjwin_is_blank_w(wchar_t ch) {
  WORD type = 0;
  /*
  GetStringTypeEx is apparently recommneded but it pushes
  down the rabbit hole of using TCHAR
  */
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_BLANK);
}

// is unicode  char a digit
DBJWIN_INLINE BOOL dbjwin_is_control_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_CNTRL);
}

// is unicode  char a digit
DBJWIN_INLINE BOOL dbjwin_is_digit_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_DIGIT);
}

// unicode  char isgraph() implementation
DBJWIN_INLINE BOOL dbjwin_is_graph_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) &&
         (type & (C1_ALPHA | C1_UPPER | C1_LOWER | C1_DIGIT | C1_PUNCT));
}

// unicode  char isgraph() implementation
DBJWIN_INLINE BOOL dbjwin_is_lower_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_LOWER);
}

// unicode  char isprint() implementation
DBJWIN_INLINE BOOL dbjwin_is_print_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) &&
         (type &
          (C1_ALPHA | C1_UPPER | C1_LOWER | C1_DIGIT | C1_PUNCT | C1_BLANK));
}

// unicode  char ispunct() implementation
DBJWIN_INLINE BOOL dbjwin_is_punct_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_PUNCT);
}
// unicode  char isspace() implementation
DBJWIN_INLINE BOOL dbjwin_is_space_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_SPACE);
}

// unicode  char isupper() implementation
DBJWIN_INLINE BOOL dbjwin_is_upper_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_UPPER);
}

// unicode  char isxdigit() implementation
DBJWIN_INLINE BOOL dbjwin_is_xdigit_w(wchar_t ch) {
  WORD type = 0;
  return GetStringTypeW(CT_CTYPE1, &ch, 1, &type) && (type & C1_XDIGIT);
}

// ASCI versions ---------------------------------------------

// Is this really necessary or just casting might be enough?
// wchar_t wc_ = (wchar_t)'A' ;
DBJWIN_INLINE wchar_t dbjwin_ac2wc(const char chr_) {
  int slength = 1;
  int len = MultiByteToWideChar(CP_ACP, 0, &chr_, slength, 0, 0);
  wchar_t retval_ = L'\0';
  MultiByteToWideChar(CP_ACP, 0, &chr_, slength, &retval_, len);
  return retval_;
}

DBJWIN_INLINE BOOL dbjwin_is_alnum_a(char ch) {
  return dbjwin_is_alnum_w((wchar_t)ch);
}

DBJWIN_INLINE BOOL dbjwin_is_alpha_a(char ch) {
  return dbjwin_is_alpha_w((wchar_t)ch);
}

// is unicode  char a blank
DBJWIN_INLINE BOOL dbjwin_is_blank_a(char ch) {
  return dbjwin_is_blank_w((wchar_t)ch);
}

// is unicode  char a digit
DBJWIN_INLINE BOOL dbjwin_is_control_a(char ch) {
  return dbjwin_is_control_w((wchar_t)ch);
}

// is unicode  char a digit
DBJWIN_INLINE BOOL dbjwin_is_digit_a(char ch) {
  return dbjwin_is_digit_w((wchar_t)ch);
}

// unicode  char isgraph() implementation
DBJWIN_INLINE BOOL dbjwin_is_graph_a(char ch) {
  return dbjwin_is_graph_w((wchar_t)ch);
}

// unicode  char isgraph() implementation
DBJWIN_INLINE BOOL dbjwin_is_lower_a(char ch) {
  return dbjwin_is_lower_w((wchar_t)ch);
}

// unicode  char isprint() implementation
DBJWIN_INLINE BOOL dbjwin_is_print_a(char ch) {
  return dbjwin_is_print_w((wchar_t)ch);
}

// unicode  char ispunct() implementation
DBJWIN_INLINE BOOL dbjwin_is_punct_a(char ch) {
  return dbjwin_is_punct_w((wchar_t)ch);
}
// unicode  char isspace() implementation
DBJWIN_INLINE BOOL dbjwin_is_space_a(char ch) {
  return dbjwin_is_space_w((wchar_t)ch);
}

// unicode  char isupper() implementation
DBJWIN_INLINE BOOL dbjwin_is_upper_a(char ch) {
  return dbjwin_is_upper_w((wchar_t)ch);
}

// unicode  char isxdigit() implementation
DBJWIN_INLINE BOOL dbjwin_is_xdigit_a(char ch) {
  return dbjwin_is_xdigit_w((wchar_t)ch);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wimplicit-function-declaration"

int dbjwin_ctype_test(const int argc, const char* argv[]);

#ifdef DBWIN_CTYPE_TEST 

int dbjwin_ctype_test(const int argc, const char* argv[]) 
{
  DBJWIN_TEST(L'A' != dbjwin_ac2wc('A')); // deliberate failure

  DBJWIN_TEST(L'A' == dbjwin_ac2wc('A'));
  DBJWIN_TEST(L'\0' == dbjwin_ac2wc('\0'));

  DBJWIN_TEST(L'A' == (wchar_t)('A'));
  DBJWIN_TEST(L'\0' == (wchar_t)('\0'));

  DBJWIN_TEST(dbjwin_is_alnum_a('9')); 

  DBJWIN_TEST(dbjwin_is_alpha_a('A')); 

  // is unicode  char a blank
  DBJWIN_TEST(dbjwin_is_blank_a(' ')); 

  // is unicode  char a digit
  DBJWIN_TEST(dbjwin_is_control_a(0x1F)); 

  // is unicode  char a digit
  DBJWIN_TEST(dbjwin_is_digit_a('9')); 

  // unicode  char isgraph() implementation
   DBJWIN_TEST(dbjwin_is_graph_a('+')); 

  // unicode  char isgraph() implementation
   DBJWIN_TEST(dbjwin_is_lower_a('a')); 

  // unicode  char isprint() implementation
   DBJWIN_TEST(dbjwin_is_print_a('A')); 

  // unicode  char ispunct() implementation
   DBJWIN_TEST(dbjwin_is_punct_a(':')); 
  // unicode  char isspace() implementation
   DBJWIN_TEST(dbjwin_is_space_a(' ')); 

  // unicode  char isupper() implementation
   DBJWIN_TEST(dbjwin_is_upper_a('A')); 

  // unicode  char isxdigit() implementation
   DBJWIN_TEST(dbjwin_is_xdigit_a('A'));

  return 42;
}

#endif // DBWIN_CTYPE_TEST

#pragma clang diagnostic pop

DBJWIN_EXTERN_C_END