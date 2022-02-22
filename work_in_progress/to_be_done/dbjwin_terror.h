#pragma once

#ifndef DBJWIN_TERROR
/*
terror = terminating error

works in both debug and release builds, uses nothing; only cl.exe compiler intrisics
*/

#include <intrin.h>

extern "C"
__inline 
dbjwin_terror_( const char * msg_, const char * file_, const unsigned long line_)
{
    #ifdef _DEBUG
   __debugbreak();
   #endif
   __fastfail(FAST_FAIL_FATAL_APP_EXIT);
}

 #define DBJWIN_TERROR(x) ((void)((x) || (dbjwin_terror_(#x, __FILE__, __LINE__), 1)))

 #endif // DBJWIN_TERROR