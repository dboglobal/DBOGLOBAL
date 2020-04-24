#include "stdafx.h"
#include "ServerNaviLog.h"


CServerNaviLog::CServerNaviLog(void)
{
	Init();
}

CServerNaviLog::~CServerNaviLog(void)
{
	Destroy();
}



bool CServerNaviLog::Create()
{
	return true;
}

void CServerNaviLog::Destroy()
{
}

void CServerNaviLog::Init()
{
}

void CServerNaviLog::Log(const char * pLog, ...)
{
	FILE* pFile;
	fopen_s(&pFile, "pelog.txt", "a");
	if (pFile)
	{
		fprintf_s(pFile, "%s\n", pLog);

		fclose(pFile);
	}
}
