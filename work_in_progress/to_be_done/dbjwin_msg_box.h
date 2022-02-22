#pragma once
#omclude "dbjwin_wallofmacros.h"
#include "dbjwin_strsafe.h"
#include "dbjwin_windows_includer.h"

DBJWIN_EXTERN_C_BEGIN

	//  msg box no exit
	DBJWIN_INLINE void dbjwin_msg_boxA(const char * fmt_ , ...)
	{
		DBJ_ASSERT(fmt_);
		char buff[1024] = { 0 };
		va_list argptr;
		va_start(argptr, fmt_ );
		HRESULT hr_ = win_vsprintfa(buff, 1024, fmt_, argptr) ;
		DBJ_ASSERT( S_OK == hr_ ) ;
		va_end(argptr);
		MessageBoxA(NULL, (LPCSTR)buff, this_base_namea(), MB_OK);
	}

#undef DBJ_MSGA
#ifdef _DEBUG
#define DBJ_MSGA(m_) dbjwin_msg_boxA("%s\n\nFile : %s\nLine # %d", m_, __FILE__, __LINE__)
#else // ! _DEBUG
#define DBJ_MSGA(m_) 
#endif // ! _DEBUG

	DBJWIN_INLINE void dbjwin_msg_boxW(const wchar_t * fmt_ , ... )
	{
		DBJ_ASSERT(fmt_);
		WCHAR buff[1024] = { (WCHAR)0 };
		va_list argptr;
		va_start(argptr, fmt_);
		HRESULT hr_ = win_vsprintfw(buff, 1024, fmt_, argptr);
		DBJ_ASSERT(S_OK == hr_);
		va_end(argptr);
		MessageBoxW(NULL, (LPCWSTR)buff, this_base_namew(), MB_OK);
	}


	// X stands for eXpression
#undef FXW
#define FXW(f_, x_ ) dbjwin_msg_boxW(L"Expression:\n%s\n\nResult:\n" f_ , #x_, (x_))

#undef FXA
#define FXA(f_, x_ ) dbjwin_msg_boxA("Expression:\n%s\n\nResult:\n" f_ , #x_, (x_))

/*
--------------------------------------------------------------------------
*/
#undef  DBJWIN_ERROR_MESSAGE_SIZE
#define DBJWIN_ERROR_MESSAGE_SIZE 0xFF

	typedef struct dbjwin_error_msg_box_arg
	{
		bool display_error_message;
		bool do_process_exit;
		/*DWORD*/ int last_error /* from GetLastError()*/;
		LPCSTR error_source;
		LPCSTR file_;
		LONG   line_;
	}
	dbjwin_error_msg_box_arg;

	typedef struct dbjwin_error_msg {
		char data[DBJWIN_ERROR_MESSAGE_SIZE];
	} dbjwin_error_msg;


	/*
	--------------------------------------------------------------------------
	
	dbjwin_error_msg
		dbjwin_error_msg_box_exit(dbjwin_error_msg_box_arg);

	this is one of those "swiss army knife functions"
	to make it less minefield like use it through macros
	taming the complexity of calling it
	*/
	DBJWIN_INLINE dbjwin_error_msg
		dbjwin_error_msg_box_exit(
			dbjwin_error_msg_box_arg  argument_
		)
	{   // def value is empty string
		dbjwin_error_msg result_err_msg = { (char)'\0' };

		// if last error not given ask for it
		if (argument_.last_error < 1) argument_.last_error = GetLastError();

		argument_.error_source =
			(argument_.error_source == NULL ? "null" : argument_.error_source);

		// Retrieve the system error message for the last-error code
		char win_error_message[BUFSIZ] = {0} ;

#ifndef  NDEBUG
		int format_rezult =
#endif // ! NDEBUG
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				argument_.last_error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPSTR)win_error_message,
				0, NULL);

#ifndef  NDEBUG
		DBJ_ASSERT(format_rezult > 0);
		if (format_rezult < 1) {
			return result_err_msg; // empty msg
		}
#endif // ! NDEBUG

		// Display the error message, if requested
		if (argument_.display_error_message) {
			dbjwin_msg_boxA(
				"%s failed\n\nerror code %d\nsource: %s\nfile:%s\nline: %d\n\n%s",
				argument_.error_source, 
				argument_.last_error, 
				argument_.file_, 
				argument_.line_,
				(argument_.do_process_exit ? " Module will exit!" : "" )
			);
		}

// in any case save the error message to be returned, if exit was not asked that is
		strncpy_s(result_err_msg.data, WIN32_ERROR_MESSAGE_SIZE, (LPCSTR)lpMsgBuf, WIN32_ERROR_MESSAGE_SIZE);

		SetLastError(0); // yes this helps!

		// caller asked us to exit the process
		if (argument_.do_process_exit) {
			ExitProcess(argument_.last_error);
		}
		return result_err_msg;
	} // dbjwin_error_msg


	/*
	--------------------------------------------------------------------------
*/
// #define dbjwin_verify(x) ((void)((x) || (DoAssert(#x, __FILE__, __LINE__), 1)))
// #define dbjwin_verify(x) ((void)((x) || (DoAssert(#x, __FILE__, __LINE__), 1)))

#define DBJWIN_EXIT(x) dbjwin_error_msg_box_exit((dbjwin_error_msg_box_arg){TRUE, TRUE, 0, (#x)})

DBJWIN_EXTERN_C_END
