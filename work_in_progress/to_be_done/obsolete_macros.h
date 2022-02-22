// macros elephant graveyard

// avoid strdup/strndup garden path
// https://stackoverflow.com/questions/46013382/c-strndup-implicit-declaration?
#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else // ! __STDC_ALLOC_LIB__
#define _POSIX_C_SOURCE 200809L
#endif // ! __STDC_ALLOC_LIB__

#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/// -------------------------------------------------------------------------------
#undef  DBJ_PERROR 
#ifndef NDEBUG
#define DBJ_PERROR (perror(__FILE__ " # " _CRT_STRINGIZE(__LINE__))) 
#else
#define DBJ_PERROR (perror(" "))
#endif // NDEBUG

#undef DBJ_FERROR
#ifdef _DEBUG
#define DBJ_FERROR( FP_) \
do { \
if (ferror(FP_) != 0) {\
	DBJ_PERROR ;\
	clearerr_s(FP_);\
} \
} while(0)
#else
#define DBJ_FERROR( FP_ )
#endif // _DEBUG

///-----------------------------------------------------------------------------------------
#ifdef _KERNEL_MODE
#define DBJ_NONPAGESECTION __declspec(code_seg("$dbj_fwk_kerneltext$"))
#else
#define DBJ_NONPAGESECTION
#endif // _KERNEL_MODE

// -------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
	// -----------------------------------------------------------------------------
	/*
	dbj_terror == terminating error
	NOTE: std::exit *is* different to C API exit()
	NOTE: all the bets are of so no point of using some logging
	*/
	static inline void dbj_terror(const char* msg_, const char* file_, const int line_)
	{
		/// DBJ_ASSERT(msg_ && file_ && line_);
		/// all the bets are of so no point of using some logging
		perror("\n\n" DBJ_ERR_PROMPT("\n\nTerminating error!"));
		exit(EXIT_FAILURE);
	}

	/*
	2020 SEP 03 -- it turns out (again) we need to initialize WIN10 terminal
	to show us VT100 colours
	*/
	static inline void dbj_win_vt100_initor_() {
		// and ... this is it ... awfull really ;)
		// ps: make sure it is not empty string!
		system(" ");
	}

#ifdef __cplusplus
} // extern "C" 
#endif

#undef DBJWIN_ASSERT
#ifdef _DEBUG
#define DBJWIN_ASSERT _ASSERTE
#else
#define	DBJWIN_ASSERT(X_) ((void)(X_))
#endif

//
// CAUTION! DBJWIN_VERIFY affects release builds too
//  _ASSERT_AND_INVOKE_WATSON asserts in debug builds
//  in release builds it invokes watson
#undef DBJWIN_VERIFY
#define DBJWIN_VERIFY(x) _ASSERT_AND_INVOKE_WATSON(x)

// https://devblogs.microsoft.com/oldnewthing/20041025-00/?p=37483
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#undef  HINST_THISCOMPONENT
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)