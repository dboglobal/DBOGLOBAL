//***********************************************************************************
//	File		:	NtlLog.cpp
//	Desc		:	
//	Begin		:	2005. 8. 2
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlcore.h"
#include ".\ntllog.h"

#include "NtlDebug.h"


CNtlLog::CNtlLog()
{
	m_pFile = NULL;
}

CNtlLog::~CNtlLog()
{
	if (m_pFile != NULL)
	{
		Delete();
	}
}


//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlLog::Create(RwChar* strFileName)
{
	NTL_ASSERTE(m_pFile == NULL);

	m_strFileName = strFileName;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&m_pFile, m_strFileName.c_str(), "wt") != 0)
		return;
#else
	m_pFile = fopen(m_strFileName.c_str(), "wt");
	if(m_pFile == NULL)
		return;
#endif
	
	fprintf(m_pFile, "\n");
	fprintf(m_pFile, "======================================================= \n");
	fprintf(m_pFile, GetCalendar() );
	fprintf(m_pFile, "START LOG FILE\n" );
	fprintf(m_pFile, "======================================================= \n");
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlLog::Delete()
{
	NTL_ASSERTE(m_pFile != NULL);

	fprintf(m_pFile, "======================================================= \n");
	fprintf(m_pFile, GetCalendar() );
	fprintf(m_pFile, "END LOG FILE\n" );
	fprintf(m_pFile, "======================================================= \n");
	fprintf(m_pFile, "\n");

	fclose(m_pFile);
	m_pFile = NULL;
}


//------------------------------------------------------------------
//	FuncName	: GetCalendar
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwChar* CNtlLog::GetCalendar()
{
	tm tmData;
	time_t the_time;

	time(&the_time);	
    localtime_s(&tmData, &the_time);

	strftime(m_cCalendarBuffer, 256, "[%Y/%m/%d %a %H:%M:%S %p]\t", &tmData);

	return m_cCalendarBuffer;
}

//------------------------------------------------------------------
//	FuncName	: GetDay
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwChar* CNtlLog::GetDay()
{
	tm tmData;
	time_t the_time;

	time(&the_time);	
    localtime_s(&tmData, &the_time);

	strftime(m_cDayBuffer, 256, "[%H:%M:%S %p]\t", &tmData);

	return m_cDayBuffer;
}
 
//------------------------------------------------------------------
//	FuncName	: fpcalendarlog
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlLog::fpcalendarlog(RwChar * fmt, ...)
{
	NTL_ASSERTE(m_pFile != NULL);

	va_list args;
	va_start(args, fmt);
	fprintf(m_pFile, GetCalendar());
	vfprintf(m_pFile, fmt, args);
	va_end( args );
	fprintf(m_pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: fpdaylog
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlLog::fpdaylog(RwChar * fmt, ...)
{
	NTL_ASSERTE(m_pFile != NULL);
	va_list args;
	va_start(args, fmt);
	fprintf(m_pFile, GetDay());
	vfprintf(m_pFile, fmt, args);
	va_end(args);
	fprintf(m_pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: fplog
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlLog::fplog(RwChar * fmt, ...)
{
	NTL_ASSERTE(m_pFile != NULL);
	va_list args;
	va_start(args, fmt);
	vfprintf(m_pFile, fmt, args);
	va_end(args);
	fprintf(m_pFile, "\n");
}

