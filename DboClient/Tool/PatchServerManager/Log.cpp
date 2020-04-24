#include "stdafx.h"
#include "Log.h"


CLog g_clLog;


void CLog::SetPath( const CString& strPath )
{
	m_strPath = strPath;
}

bool CLog::B( void )
{
	m_Stream.open( m_strPath, std::ios_base::out | std::ios_base::app );

	if ( m_Stream.is_open() )
	{
		return true;
	}

	return false;
}

std::wofstream& CLog::S( void )
{
	return m_Stream;
}

void CLog::E( void )
{
	m_Stream.flush();
	m_Stream.close();
}