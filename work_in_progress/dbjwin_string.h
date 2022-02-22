#pragma once
/// (c) 2019-2022 by dbj.org   -- https://dbj.org/license_dbj/
#ifndef DBJWIN_STRING_INCLUDED
#define DBJWIN_STRING_INCLUDED

#include "dbjwin_strsafe.h"
#include "dbjwin_wallofmacros.h"

/*
   dbj win string is simply an char array in an struct

   struct { char data[0xFF]; } str1 = {""};

   This is stack space based thus free-ing is not required
   this can be sent into and out of functions and dbj components,
   WIN32 ANSI C runtime will do very fast copy elision in any case

   CAVEAT EMPTOR! if size is runtime value the resulting instance can not be
   initialized.

   int j = 0xFF ;
   struct { char data[j]; } str1 = {""}; // does not compile
   struct { char data[j]; } str1 ; // does compile

   obviously wchar_t variant can and will be added as soon as it is required
   same is for utf-8, utf-16 and utf-32

   Godbolt quick test: https://godbolt.org/z/EsoYz6jqK
*/

DBJWIN_EXTERN_C_BEGIN

#ifndef DBJWIN_COUNT_OF
#define DBJWIN_COUNT_OF(x) \
  ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))
#endif

// named struct is required for function arguments
// and function return types in particular
// NOTE: in C types can not be declared inside function parameter list
// thus one has to do typedef before function definition and use it accordingly
#undef DBJWIN_STRING_TYPE
#define DBJWIN_STRING_TYPE(SIZE_) \
  struct dbj_string_##SIZE_ {     \
    char data[SIZE_];             \
  }

// most usual types are predefined here
// type names defined like that can clash
// thus we will define few probably most common
typedef DBJWIN_STRING_TYPE(1024) dbjwin_string_1024;
typedef DBJWIN_STRING_TYPE(512) dbjwin_string_512;
typedef DBJWIN_STRING_TYPE(256) dbjwin_string_256;
typedef DBJWIN_STRING_TYPE(128) dbjwin_string_128;
typedef DBJWIN_STRING_TYPE(64) dbjwin_string_64;
typedef DBJWIN_STRING_TYPE(32) dbjwin_string_32;
typedef DBJWIN_STRING_TYPE(16) dbjwin_string_16;
typedef DBJWIN_STRING_TYPE(8) dbjwin_string_8;

/*
Following is dbj string variable declaration where
anonymous struct is used thus no name clashes

static const DBJWIN_STRING( str_global, 123 ) = {{"static const global dbj
string"}};

    printf("%s", str_global.data );

there is no need for a size member of a function, inside is just an array with
known size:

    printf("%zu", sizeof str_global.data );

that is mainly ok for local variables; also not passed in and out of functions

*/
#undef DBJWIN_STRING
#define DBJWIN_STRING(NAME_, SIZE_) \
  struct {                          \
    char data[SIZE_];               \
  } NAME_  // = {.size = SIZE_} <-- no can do for VLA, if SIZE_ is a runtime
           // value!

// -------------------------------------------------------------------------------
// size is not a strlen
#undef DBJWIN_STRING_CAPACITY
#define DBJWIN_STRING_CAPACITY(STR_) DBJWIN_COUNT_OF(STR_.data)

// the distance to the first '\0' found
// WARNING: strnlen is 0  if there is no zero terminator found in the data
#undef DBJWIN_STRING_LEN
#define DBJWIN_STRING_LEN(STR_) dbjwin_strnlena(STR_.data, sizeof(STR_.data))

// -------------------------------------------------------------------------------
// if you need to print the data it is best to zero it before assigning
// string literals to it shorter than the data capacity
#undef DBJWIN_STRING_ZERO
#define DBJWIN_STRING_ZERO(STR_) ((void)memset(STR_.data, 0, sizeof(STR_.data)))
// -------------------------------------------------------------------------------
#undef DBJWIN_STRING_ASSIGN
// STR_ is generic dbjwin string type
// S_ is string literal
// no pointers here, STR_ is a value
// Note we do not do the zeroing of the target
// neither we put '\0' after the assigned string literal
// we put it in the last slot od the data array
//
#define DBJWIN_STRING_ASSIGN(STR_, S_)                                  \
  do                                                                    \
    {                                                                   \
      (void)dbjwin_memccpy(STR_.data, S_, '\0', sizeof(STR_.data) - 1); \
      *(STR_.data + sizeof(STR_.data) - 1) = 0;                         \
    }                                                                   \
  while (0)

// -------------------------------------------------------------------------------
#undef DBJWIN_STRING_COPY
// STR1_ is any dbjwin string type
// STR2_ is any dbjwin string type
// they do not have to be of the same type
// result will be cut to the size of the STR1_
// no pointers here, both arge are values
// target data is overwritten
#define DBJWIN_STRING_COPY(STR1_, STR2_)                    \
  do                                                        \
    {                                                       \
      static_assert(DBJWIN_STRING_CAPACITY(STR1_) <=        \
                    DBJWIN_STRING_CAPACITY(STR2_));         \
      (void)dbjwin_memccpy(STR1_.data, STR2_.data, '\0',    \
                           sizeof(STR1_.data) - 1);         \
      *(STR_.data + DBJWIN_STRING_LEN(STR1_.data) - 1) = 0; \
    }                                                       \
  while (0)
// -------------------------------------------------------------------------------
#undef DBJWIN_STRING_ADD
// STR1_ is any dbjwin string type
// STR2_ is any other dbjwin string type
//
// result will be cut to the size of the STR1_
// there is no resize
// no pointers here, both args are values
//
// concept: obviously we could create string with the data
// size large enough to contain two arbitrary strings data
// but this is all stack based so that is not very wise
//
// if any of the strings does not contain zero terminator no job will be done
//
#define DBJWIN_STRING_ADD(STR1_, STR2_)                               \
  do                                                                  \
    {                                                                 \
      const size_t str1_len = DBJWIN_STRING_LEN(STR1_);               \
      if (str1_len > 0)                                               \
        (void)dbjwin_memccpy(STR1_.data + str1_len, STR2_.data, '\0', \
                             DBJWIN_STRING_CAPACITY(STR1_.data) - 1); \
      *(STR_.data + DBJWIN_STRING_LEN(STR1_.data) - 1) = 0;           \
    }                                                                 \
  while (0)

// -------------------------------------------------------------------------------
// this is important; null or not null is not good enough ;)
// Any field can be in 3 states: exist, does not exist and exist but **empty**
// For example in the SQL world that is perfectly normal
// In databases field is always there but can be empty too, or NULL or having a
// value Thus NULL != Empty dbjwin string will alway have data this is never
// null we define as empty an dbjwin string the one that has '\0' as the first
// char
//

#undef DBJWIN_STRING_SET_EMPTY
#define DBJWIN_STRING_SET_EMPTY(STR_) (STR_.data[0] = '\0')

#undef DBJWIN_STRING_IS_EMPTY
#define DBJWIN_STRING_IS_EMPTY(STR_) (STR_.data[0] == '\0')

DBJWIN_EXTERN_C_END

#endif  // DBJWIN_STRING_INCLUDED