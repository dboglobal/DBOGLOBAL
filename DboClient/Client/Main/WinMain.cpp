#include "precomp_dboclient.h"
#include "DboApplication.h"
#include "NtlDebug.h"
#include "DboCompileOpt.h"
#include "DboDef.h"


BOOL IsMultiOpen(const char *pExeName)
{
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, pExeName);
	if(NULL == hMutex)
		return FALSE;

	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE;
	return TRUE;
}

BOOL IsLimitDirectory(void)
{
	RwChar chCurrDir[1024];
	memset(chCurrDir, 0, 1024);

	GetCurrentDirectory(1024, chCurrDir);

	RwInt32 iLen = (RwInt32)strlen(chCurrDir);
	if(iLen >= 200)
		return FALSE;

	return TRUE;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{

#ifdef MUTEX_CHECK
	if ( NULL == lpCmdLine && strcmp( lpCmdLine, "nomutex" ) > 0 )
	{
		if(!IsMultiOpen("DragonBall"))
		{
			::MessageBox(NULL, "DragonBallOnline Global is already opened !!!", "DragonBallOnline Global", MB_OK);
			return 1;
		}
	}
#endif

	NtlClientNetSetPrintFlag( 0 );

	if(!IsLimitDirectory())
	{
		::MessageBox(NULL, "A directory name length must not exceed 200 letters", "DragonBallOnline Global", MB_OK);
		return 1;
	}

	CDboApplication app;
	
	app.Init();

#ifdef NDEBUG
	
	if(!app.Create(hInstance, 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, FALSE))
	{
		NTL_ASSERTE("Application create failed!!!");
		return 1;
	}

#else

	if(!app.Create(hInstance, 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, NTL_DEBUG_WINDOW_MODE))
	{
		NTL_ASSERTE("Application create failed!!!");
		return 1;
	}

#endif


	app.MessagePump();
	app.Destroy();

	return 1;
}


