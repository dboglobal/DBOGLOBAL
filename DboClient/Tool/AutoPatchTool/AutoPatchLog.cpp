#include "StdAfx.h"
#include "AutoPatchLog.h"

CAutoPacthLog::CAutoPacthLog()
{
	
}

CAutoPacthLog::~CAutoPacthLog()
{
}

void CAutoPacthLog::Out(const char *pFileName, const char *Log)
{
	FILE* fp;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, pFileName, "a+") != 0)
		return;
#else
	fp = fopen(pFileName, "wt");
	if(m_pFile == NULL)
		return;
#endif

    // 현재 시간을 저장한다.
    tm time;
    __time64_t long_time;
    _time64( &long_time ); 
    _localtime64_s( &time, &long_time ); 

    fprintf(fp, "[%d/%d %d:%d] ", time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min);
	fwrite(Log, 1, strlen(Log), fp);
	fprintf(fp, "\n");

	fclose(fp);
}

