/*****************************************************************************
 *
 * File			: NtlConsole.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 12. 19	
 * Abstract		: console winodw manager
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_CONSOLE__
#define __NTL_CONSOLE__

#include <stdio.h>
#include <windows.h>


class CNtlConsole
{
private:

	HANDLE m_hConsole;

public:

	CNtlConsole() { m_hConsole = NULL; }

	// create the console
	bool   Create(const char* szTitle, bool bNoClose = false);
	
	// set color for output
	void   Color(WORD wColor = NULL);
	// write output to console
	void   Output(const char* szOutput = NULL, ...);
	
	// set and get title of console
	void   SetTitle(const char* szTitle);
	char*  GetTitle();

	// get HWND and/or HANDLE of console
	HWND   GetHWND();
	HANDLE GetHandle();

	// show/hide the console
	void   Show(bool bShow = true);
	// disable the [x] button of the console
	void   DisableClose(void);
	// clear all output
	void   Clear(void);

	// close the console and delete it
	void   Close(void);
};

#endif 