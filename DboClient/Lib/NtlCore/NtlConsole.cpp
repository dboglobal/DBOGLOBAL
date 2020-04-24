#include "precomp_ntlcore.h"
#include "NtlConsole.h"


bool CNtlConsole::Create(const char* szTitle, bool bNoClose /*= false*/)
{
	// Has console been already created?
	if(m_hConsole != NULL)
		return false;
	
	// Allocate a new console for our app
	if(!AllocConsole())
		return false;
	
	// Create the actual console
	m_hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if(m_hConsole == INVALID_HANDLE_VALUE)
		return false;
	
	if(SetConsoleMode(m_hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT ) == 0)
		return false;
	
	// if true, disable the [x] button of the console
	if(bNoClose)
		DisableClose();
				
	// set the console title
	if(szTitle != NULL)
		SetConsoleTitle(szTitle);

	return true;
}
	
void CNtlConsole::Color(WORD wColor /*= NULL*/)
{
	if(m_hConsole == NULL)
		return;

	// no color specified, reset to defaults (white font on black background)
	if(wColor != NULL)
		SetConsoleTextAttribute(m_hConsole, wColor );
	// change font and/or background color
	else
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white text on black bg
}

void CNtlConsole::Output(const char* szOutput /*= NULL*/, ...)
{
	if(m_hConsole == NULL)
		return;

	DWORD		dwWritten;
	char		out[256];
	va_list		va;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	// if not parameter set, write a new line
	if(szOutput == NULL) 
		sprintf_s(out, 256, "\n");
	// process arguments
	else
	{
		va_start(va, szOutput);
		vsprintf_s(out, 256, szOutput, va);
		va_end(va);	
	}		
#else
	if(szOutput == NULL) 
		sprintf(out,"\n");
	// process arguments
	else
	{
		va_start(va, szOutput);
		vsprintf(out, szOutput, va);
		va_end(va);	
	}
#endif
   
	// write to the console
	WriteConsole(m_hConsole,out,(DWORD)strlen(out),&dwWritten,0);
}

void CNtlConsole::SetTitle(const char* szTitle)
{
	SetConsoleTitle(szTitle);
}

char* CNtlConsole::GetTitle(void)
{
	static char szWindowTitle[256] = "";
	GetConsoleTitle(szWindowTitle,sizeof(szWindowTitle));

	return szWindowTitle;
}

HWND CNtlConsole::GetHWND(void)
{
	if(m_hConsole == NULL) 
		return NULL;

	// try to find our console window and return its HWND
	return FindWindow("ConsoleWindowClass",GetTitle());
}

HANDLE CNtlConsole::GetHandle(void)
{
	return m_hConsole;
}

void CNtlConsole::Show(bool bShow /*= true*/)
{
	if(m_hConsole == NULL) 
		return;

	// get out console's HWND and show/hide the console
	HWND hWnd = GetHWND();
	if(hWnd != NULL)
		ShowWindow(hWnd, SW_HIDE ? SW_SHOW : bShow);
}

void CNtlConsole::DisableClose(void)
{
	if(m_hConsole == NULL) 
		return;

	HWND hWnd = GetHWND();
	
	// disable the [x] button if we found our console
	if(hWnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hWnd,0);
		if(hMenu != NULL)
		{
			DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			DrawMenuBar(hWnd);
		}
	}
}

void CNtlConsole::Clear(void)
{
	if(m_hConsole == NULL)
		return;

	/***************************************/
	// This code is from one of Microsoft's
	// knowledge base articles, you can find it at 
    // http://support.microsoft.com/default.aspx?scid=KB;EN-US;q99261&
	/***************************************/

	COORD coordScreen = { 0, 0 };

	BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */ 
    DWORD dwConSize; 

	/* get the number of character cells in the current buffer */ 

    bSuccess = GetConsoleScreenBufferInfo( m_hConsole, &csbi );
	if(!bSuccess)
		return;
    
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */ 

    bSuccess = FillConsoleOutputCharacter( m_hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
    if(!bSuccess)
		return;

    /* get the current text attribute */ 

    bSuccess = GetConsoleScreenBufferInfo( m_hConsole, &csbi );
    if(!bSuccess)
		return;

    /* now set the buffer's attributes accordingly */ 

    bSuccess = FillConsoleOutputAttribute( m_hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    if(!bSuccess)
		return;

    /* put the cursor at (0, 0) */ 

    bSuccess = SetConsoleCursorPosition( m_hConsole, coordScreen );
    if(!bSuccess)
		return;
}

// close the console and delete it
void CNtlConsole::Close(void)
{
	if(m_hConsole == NULL)
		return; 

	FreeConsole();
	m_hConsole = NULL;
}