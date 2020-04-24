#include "precomp_ntlcore.h"
#include "NtlCoreUtil.h"
#include "NtlLog.h"

#include <ddraw.h>

float	g_fElapsedTime;

static bool	s_bEnableTrace		= true;
static bool	s_bInitTraceFile	= false;

fpFuncLastErrorHandler g_fpLastError = NULL;

void  g_SetElapsedTime(float fElapsedTime)
{
	g_fElapsedTime = fElapsedTime;
}

float g_GetElapsedTime()
{
	return g_fElapsedTime;
}



void NtlFileAttrReadOnlyRelease(const RwChar *pFileName)
{
	DWORD dwAttrs = ::GetFileAttributes(pFileName);
	if(dwAttrs == INVALID_FILE_ATTRIBUTES)
		return; 

	if( dwAttrs & FILE_ATTRIBUTE_READONLY) 
    { 
		dwAttrs &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(pFileName, dwAttrs); 
    } 
}

RwUInt32 NtlGetFileSize(const RwChar *pFileName)
{
	RwUInt32 uiFileSize = -1;

	FILE *fp = NULL;

	if(fopen_s(&fp, pFileName, "rb") != 0)
		return uiFileSize;

	fseek(fp, 0, SEEK_END);

	uiFileSize = (RwUInt32)ftell(fp);

	fclose(fp);

	return uiFileSize;
}

RwBool DeleteFolder(const RwChar* pFolderName)
{
	SHFILEOPSTRUCT	FileOp = {0};
	char			szTemp[MAX_PATH];

	strcpy_s(szTemp, MAX_PATH, pFolderName);
	szTemp[strlen(pFolderName) + 1] = NULL; 

	FileOp.hwnd						= NULL;
	FileOp.wFunc					= FO_DELETE;
	FileOp.pFrom					= NULL;
	FileOp.pTo						= NULL;
	FileOp.fFlags					= FOF_NOCONFIRMATION | FOF_NOERRORUI;
	FileOp.fAnyOperationsAborted	= false;
	FileOp.hNameMappings			= NULL;
	FileOp.lpszProgressTitle		= NULL;
	FileOp.pFrom					= szTemp;

	SHFileOperation(&FileOp);

	return TRUE;
}

RwChar g_chLogPath[MAX_PATH]	= "dbolog.txt";

// 로그 파일의 용량이 10kb를 넘어가면 삭제한다.
#define MAX_LOG_FILE_SIZE 10240
void NtlLogFileSizeCheck()
{
    HANDLE hFile = ::CreateFile(g_chLogPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFile)
    {
        DWORD dwFileSize = ::GetFileSize(hFile, NULL);

        if(dwFileSize >= MAX_LOG_FILE_SIZE)
        {
            CloseHandle(hFile);
            ::DeleteFile(g_chLogPath);
        }
        else
        {
            CloseHandle(hFile);
        }        
    }
}

void NtlLogFilePath(RwChar *pFilePath)
{
	if(pFilePath == NULL)
		return;

	strcpy_s(g_chLogPath, _countof(g_chLogPath), pFilePath);
}

void NtlLogFilePrint(RwChar *pLog)
{
	if(pLog == NULL)
		return;

	NtlFileAttrReadOnlyRelease(g_chLogPath);
    NtlLogFileSizeCheck();

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, g_chLogPath, "a+") != 0)
		return;
#else
	fp = fopen(g_chLogPath, "a+");
	if(fp == NULL)
		return;
#endif

	CNtlLog cLog;
	RwChar *pHeaderLog = cLog.GetDay();
	if(pHeaderLog)
		fprintf(fp, pHeaderLog);
		
	fprintf(fp, pLog);
	fprintf(fp, "\n");

	fclose(fp);
}


void NtlLogFilePrintArgs(RwChar *fmt, ... )
{
	NtlFileAttrReadOnlyRelease(g_chLogPath);
    NtlLogFileSizeCheck();

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, g_chLogPath, "a+") != 0)
		return;
#else
	fp = fopen(g_chLogPath, "a+");
	if(fp == NULL)
		return;
#endif

	CNtlLog cLog;
	RwChar *pHeaderLog = cLog.GetDay();
	if(pHeaderLog)
		fprintf(fp, pHeaderLog);

	va_list args;
	va_start(args, fmt);
	vfprintf_s(fp, fmt, args);
	va_end(args);
	fprintf(fp, "\n");
	
	fclose(fp);
}

// Trace는 매일 새로운 파일을 생성하여 기록한다
RwChar g_chTracePath[MAX_PATH]	= "";

void NtlTraceEnable_inRelease(bool bEnable)
{
	s_bEnableTrace = bEnable;
}

void NtlTraceInitFilePath()
{
	if( !s_bEnableTrace )
		return;

	tm curTime;
	time_t timer = time(NULL);

	localtime_s(&curTime, &timer);

	curTime.tm_year += 1900;
	curTime.tm_mon	+= 1;

	sprintf(g_chTracePath, "%d%d%dTrace.txt", curTime.tm_year, curTime.tm_mon, curTime.tm_mday);
}

void NtlTraceInitFilePath(RwChar *pFilePath)
{
	if( !s_bEnableTrace )
		return;

	if(pFilePath == NULL)
		return;

	strcpy_s(g_chTracePath, _countof(g_chTracePath), pFilePath);

	s_bInitTraceFile = true;
	NtlTraceFilePrint("=============== Start Trace ===============\n");
}

void NtlTraceFilePrint(RwChar *pLog)
{
	if( !s_bEnableTrace )
		return;

	if( !s_bInitTraceFile )
	{
		s_bInitTraceFile = true;
		NtlTraceInitFilePath();
		NtlTraceFilePrint("=============== Start Trace ===============\n");
	}

	if(pLog == NULL)
		return;

	NtlFileAttrReadOnlyRelease(g_chTracePath);
	NtlLogFileSizeCheck();

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, g_chTracePath, "a+") != 0)
		return;
#else
	fp = fopen(g_chTracePath, "a+");
	if(fp == NULL)
		return;
#endif

	CNtlLog cLog;
	RwChar *pHeaderLog = cLog.GetDay();
	if(pHeaderLog)
		fprintf(fp, pHeaderLog);

	fprintf(fp, pLog);
	fprintf(fp, "\n");

	fclose(fp);
}

void NtlTraceFilePrintArgs(RwChar *fmt, ... )
{
	if( !s_bEnableTrace )
		return;

	if( !s_bInitTraceFile )
	{
		s_bInitTraceFile = true;
		NtlTraceInitFilePath();
		NtlTraceFilePrint("=============== Start Trace ===============\n");
	}

	NtlFileAttrReadOnlyRelease(g_chTracePath);
	NtlLogFileSizeCheck();

	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, g_chTracePath, "a+") != 0)
		return;
#else
	fp = fopen(g_chTracePath, "a+");
	if(fp == NULL)
		return;
#endif

	CNtlLog cLog;
	RwChar *pHeaderLog = cLog.GetDay();
	if(pHeaderLog)
		fprintf(fp, pHeaderLog);

	va_list args;
	va_start(args, fmt);
	vfprintf_s(fp, fmt, args);
	va_end(args);
	fprintf(fp, "\n");

	fclose(fp);
}


void LinkLastErrorHandler(fpFuncLastErrorHandler fp)
{
	g_fpLastError = fp;
}

void UnLinkLastErrorHandler(void)
{
	g_fpLastError = NULL;
}

void NtlSetLastError(const RwChar *pError)
{
	if(g_fpLastError)
	{
		(*g_fpLastError)(pError);
	}
}

bool HaveMagicKeyword(const char* pcFileName, const char* pcKeyword)
{
	char acBuffer[1024] = "";
	FILE* fp;

	if( !pcFileName || !pcKeyword )
		return false;

	fopen_s(&fp, pcFileName, "r");
	if(!fp)
		return false;

	while( fgets(acBuffer, 1024, fp) )
	{
		if( strcmp(acBuffer, pcKeyword) == 0 )
		{
			fclose(fp);
			return true;
		}
	}

	fclose(fp);
	return false;
}

bool GetSystemMemorySize(unsigned __int64* pMemorySizeAvailPhys, unsigned __int64* pMemorySizeTotalPhys, unsigned __int64* pMemorySizeAvailVirtual, unsigned __int64* pMemorySizeTotalVirtual, unsigned __int64* pMemorySizeAvailPageFile, unsigned __int64* pMemorySizeTotalPageFile)
{
	MEMORYSTATUS MemStatus;
	MemStatus.dwLength = sizeof(MemStatus);
	GlobalMemoryStatus(&MemStatus);

	*pMemorySizeAvailPhys		= MemStatus.dwAvailPhys;
	*pMemorySizeAvailVirtual	= MemStatus.dwAvailVirtual;
	*pMemorySizeAvailPageFile	= MemStatus.dwAvailPageFile;

	*pMemorySizeTotalPhys		= MemStatus.dwTotalPhys;
	*pMemorySizeTotalVirtual	= MemStatus.dwTotalVirtual;
	*pMemorySizeTotalPageFile	= MemStatus.dwTotalPageFile;

	return true;
}

bool GetVideoMemorySize(DWORD* pMemorySizeAvail, DWORD* pMemorySizeTotal)
{
	LPDIRECTDRAW7 lpDD = NULL;
	DDSCAPS2      ddsCaps2; 
	HRESULT       hr; 

	if (FAILED(DirectDrawCreateEx(NULL, (VOID**)&lpDD, IID_IDirectDraw7, NULL)))
	{
		return false; 
	}

	// Initialize the structure.
	ZeroMemory(&ddsCaps2, sizeof(ddsCaps2));

	ddsCaps2.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM; 
	hr				= lpDD->GetAvailableVidMem(&ddsCaps2, pMemorySizeTotal, pMemorySizeAvail); 

	if(lpDD)
	{
		lpDD->Release();
	}
	if (FAILED(hr))
	{
		return false;
	}
	return true;		
}