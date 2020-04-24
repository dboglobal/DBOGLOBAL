//***********************************************************************************
//
//	File		:	NtlService.cpp
//
//	Begin		:	2008-06-12
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlService.h"
#include "NtlSharedDef.h"

#include <fstream>
#include <time.h>


bool Dbo_IsValidText(WCHAR* pwszText)
{
	if (NULL == pwszText)
	{
		return FALSE;
	}

	std::wstring wstrTest = pwszText;

	if (wstrTest.find_first_not_of(L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890") != std::string::npos)
		return FALSE;

	return TRUE;
}


bool Dbo_IsNameTooShort(WCHAR* pwszName)
{
	if (wcslen(pwszName) < NTL_MIN_SIZE_CHAR_NAME)
	{
		return true;
	}

	return false;
}

bool Dbo_IsNameTooLong(WCHAR* pwszName)
{
	if (wcslen(pwszName) > NTL_MAX_SIZE_CHAR_NAME)
	{
		return true;
	}

	return false;
}

DBOTIME DBO_TIMEToDBOTIME(sDBO_TIME & time)
{
	tm _tm;

	_tm.tm_year = time.year - 1900;
	_tm.tm_mon = time.month - 1;
	_tm.tm_mday = time.day;
	_tm.tm_hour = time.hour;
	_tm.tm_min = time.minute;
	_tm.tm_sec = time.second;
	_tm.tm_isdst = -1;

	return mktime(&_tm);
}

void DBOTIMEToDBO_TIME(sDBO_TIME * result, const DBOTIME time)
{
	tm _tm;

	localtime_s(&_tm, &time);
	result->year = (WORD)(_tm.tm_year + 1900);
	result->month = (BYTE)(_tm.tm_mon + 1);
	result->day = (BYTE)_tm.tm_mday;
	result->hour = (BYTE)_tm.tm_hour;
	result->minute = (BYTE)_tm.tm_min;
	result->second = 0;
}

bool ReadServerSetting( sDBO_SERVER_SETTING &sInfo )
{
	std::string path("../");
	path += SERVER_SETTING_FILE;
	std::ifstream fs( path.c_str(), std::ios::binary );
	if ( false == fs.is_open() )
	{
		return false;
	}

	fs.read((char*)&sInfo, sizeof(sDBO_SERVER_SETTING));
	fs.close();

	return true;
}