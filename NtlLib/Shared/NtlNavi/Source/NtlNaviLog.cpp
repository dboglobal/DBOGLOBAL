#include "precomp_navi.h"
#include "NtlNaviLog.h"


CNtlNaviLog* CNtlNaviLog::GetInstance( void )
{
	static CNtlNaviLog g_clNaviLog;
	return &g_clNaviLog;
}


char CNtlNaviLog::s_szStrBuf[LOG_MAX_BUFFER];


CNtlNaviLog::CNtlNaviLog( void )
{
	m_pLog = NULL;
}

CNtlNaviLog::~CNtlNaviLog( void )
{
	m_pLog = NULL;
}

void CNtlNaviLog::SetLog( INtlNaviLog* pLog )
{
	CNtlNaviAutoCS clAutoLock( &m_clLogCS );

	m_pLog = pLog;
}

void CNtlNaviLog::Log( const char* pFormat, ... )
{
	CNtlNaviAutoCS clAutoLock( &m_clLogCS );

	if ( m_pLog )
	{
		va_list vaList;

		va_start( vaList, pFormat );
		vsprintf_s( s_szStrBuf, LOG_MAX_BUFFER, pFormat, vaList );
		va_end( vaList );

		m_pLog->Log( s_szStrBuf );
	}
}
