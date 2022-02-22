#pragma once
#ifndef DBJ_WINDOWS_INCLUDE_INC_
#define DBJ_WINDOWS_INCLUDE_INC_

#include "dbjwin_terror.h"
#include "dbjwin_wallofmacros.h"
/*
(c) 2019-2020 by dbj.org   -- LICENSE DBJ -- https://dbj.org/license_dbj/
*/
/*

DBJ: it is very questionable if this is rquire at all?

https://docs.microsoft.com/en-us/cpp/porting/modifying-winver-and-win32-winnt?view=vs-2019

for building we require WIN10 as minimum
*/
// #include <winsdkver.h>

// #ifndef WINVER
// #define WINVER 0x0A00
// #endif

// #ifndef _WIN32_WINNT
// #define _WIN32_WINNT 0x0A00
// #endif

//-------------------------------------------------------------------------------

// #ifdef __STDC_ALLOC_LIB__
// #define __STDC_WANT_LIB_EXT2__ 1
// #else
// #define _POSIX_C_SOURCE 200809L
// #endif

// #include <crtdbg.h>
// #include <errno.h>

#define NOMINMAX
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
// #include <processenv.h>

#ifdef NOMINMAX
#undef  min
#define min(x, y) ((x) < (y) ? (x) : (y))

#undef  max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif // NOMINIMAX

DBJWIN_EXTERN_C_BEGIN

DBJ_UNUSED_F
DBJWIN_INLINE const wchar_t* this_app_full_path_w()
{
	static wchar_t full_path[1024] = { L'\0' };
	if (full_path[0] == L'\0') {
		int rez = GetModuleFileNameW(
			(HMODULE)NULL, full_path, 1024
		);

		DBJ_ASSERT(rez != 0);
	}
	return full_path;
}

DBJ_UNUSED_F
DBJWIN_INLINE const char* this_app_full_path_a()
{
	static char full_path[1024] = { '\0' };
	if (full_path[0] == '\0') {
		int rez = GetModuleFileNameA(
			(HMODULE)NULL, full_path, 1024
		);
		DBJ_ASSERT(rez != 0);
	}
	return full_path;
}

// with no extension
DBJ_UNUSED_F DBJWIN_INLINE const wchar_t* this_base_namew()
{
	static wchar_t this_base_name[0xFF] = { L'\0' };

	if (this_base_name[0] == L'\0') {

		wchar_t this_file[1024] = { L'\0' };

		int rez = GetModuleFileNameW(
			(HMODULE)NULL, this_file, 1024
		);

		DBJ_ASSERT(rez != 0);

		wchar_t* last_slash = wcsrchr(this_file, L'\\');
		wchar_t* last_dot = wcsrchr(this_file, L'.');
		wchar_t* walker = last_slash + 1;

		DBJ_ASSERT(last_slash);
		DBJ_ASSERT(last_dot);
		DBJ_ASSERT(walker);

		int this_base_name_idx = 0;
		while (walker != last_dot) {
			this_base_name[this_base_name_idx++] = *walker;
			walker++;
		}
		this_base_name[this_base_name_idx] = L'\0';

		return this_base_name;
	}

	return this_base_name;
}

DBJ_UNUSED_F DBJWIN_INLINE const char* this_base_namea()
{
	static char this_base_name[0xFF] = { '\0' };

	if (this_base_name[0] == '\0') {

		char this_file[1024] = { '\0' };

		int rez = GetModuleFileNameA(
			(HMODULE)NULL, this_file, 1024
		);

		DBJ_ASSERT(rez != 0);

		char* last_slash = strchr(this_file, L'\\');
		char* last_dot = strchr(this_file, L'.');
		char* walker = last_slash + 1;

		DBJ_ASSERT(last_slash);
		DBJ_ASSERT(last_dot);
		DBJ_ASSERT(walker);

		int this_base_name_idx = 0;
		while (walker != last_dot) {
			this_base_name[this_base_name_idx++] = *walker;
			walker++;
		}
		this_base_name[this_base_name_idx] = '\0';

		return this_base_name;
	}

	return this_base_name;
}

/// --------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------
///  what is bellow was in a separate header
///  moved it in here as it is morwe or less always required 
///  wherever Windows is required
/// --------------------------------------------------------------------------------
/// --------------------------------------------------------------------------------


	//
	// we emulate _OSVERSIONINFOA and return osinfo_struct to the callers
	// reason is mainly, we do not like the *agrarian* notation
	//
	typedef struct osinfo_struct {
		long size_info_;
		long major;
		long minor;
		long build_num;
		long platform_id;
		char csd_version[128]; // Maintenance string for PSS usage
	} osinfo, * osinfo_ptr;

#define DBJ_OSINFO_EMPTY {0,0,0,0,0,{0}}

	// actually the sure-fire way to obtain windows version
	DBJ_UNUSED_F DBJWIN_ILINE osinfo get_win_version(void)
	{
		typedef LONG NTSTATUS; // , * PNTSTATUS{};
		typedef NTSTATUS(WINAPI* get_version_fp)(osinfo_ptr);

		static HMODULE hMod = ::GetModuleHandleA("ntdll.dll");
		if (hMod) {
			static get_version_fp fxPtr =
				(get_version_fp)::GetProcAddress(hMod, "RtlGetVersion");

			_ASSERTE(fxPtr != nullptr);

			if (fxPtr != nullptr) {
				osinfo osinfo_var_ = DBJ_OSINFO_EMPTY;
				osinfo_var_.size_info_ = sizeof(osinfo_var_);
				if ((0x00000000) == fxPtr(&osinfo_var_)) {
					return osinfo_var_;
				}
			}
		}
		static osinfo osinfo_empty_ = DBJ_OSINFO_EMPTY; //{ 0, 0, 0, 0, 0, {0} };
		return osinfo_empty_;
	} // get_win_version

#define DBJ_IS_OS_INFO_EMPTY(info_) (0 == info_.major)

/*
return true if Windows version is greater than or equal to the specified number

If the Windows version is equal to or greater than 10.0.14393
then ENABLE_VIRTUAL_TERMINAL_PROCESSING is supported.

*/
	DBJ_UNUSED_F DBJWIN_INLINE BOOL is_win_ver_or_greater(WORD major_ver, WORD minor_ver,
		WORD build_ver) {
		osinfo ver_info_ = get_win_version();

		if (DBJ_IS_OS_INFO_EMPTY(ver_info_))
			return false;

		if (ver_info_.major > major_ver)
			return true;
		else if (ver_info_.major < major_ver)
			return false;

		if (ver_info_.minor > minor_ver)
			return true;
		else if (ver_info_.minor < minor_ver)
			return false;

		//  we are here because both major and minor are equal
		if (ver_info_.build_num >= build_ver)
			return true;
		return false;
	}

#undef DBJ_IS_OS_INFO_EMPTY

DBJWIN_EXTERN_C_END

#endif // DBJ_WINDOWS_INCLUDE_INC_
