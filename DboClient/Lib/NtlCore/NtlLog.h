//***********************************************************************************
//	File		:	NtlLog.h
//	Desc		:	
//	Begin		:	2005. 8. 2
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include <string>
#include <windows.h>
#include <rwcore.h>
#include <time.h>


class CNtlLog
{
protected:
private:

	std::string		m_strFileName;

	FILE*			m_pFile;

	RwChar			m_cCalendarBuffer[MAX_PATH];
	RwChar			m_cDayBuffer[MAX_PATH];

public:
			CNtlLog();
	virtual ~CNtlLog();

	void	Create(RwChar* strFileName);
	void	Delete();

	RwChar*	GetCalendar();
	RwChar*	GetDay();

	void	fpcalendarlog(RwChar* fmt, ...);
	void	fpdaylog(RwChar* fmt, ...);
	void	fplog(RwChar* fmt, ... );

};

