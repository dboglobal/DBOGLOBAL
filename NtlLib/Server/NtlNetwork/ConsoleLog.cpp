//-- include ------------------------------------------------------------------------
#include "StdAfx.h"
#include "ConsoleLog.h"
//-----------------------------------------------------------------------------------

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CConsoleLog::CConsoleLog()
: m_pOutFile ( NULL )
, m_bIsValid ( false )
{
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose : 
//  Return  : 
//-----------------------------------------------------------------------------------
CConsoleLog::~CConsoleLog()
{
	Destroy();
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CConsoleLog::Create( char* pszFileName )
{
	m_pOutFile = new std::ofstream(pszFileName, std::ios::out);
	if( NULL == m_pOutFile )
	{
		_ASSERT( 0 );
	}

	m_bIsValid = true;

	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CConsoleLog::Destroy( void )
{
	if( NULL != m_pOutFile )
	{
		m_pOutFile->close();
	}

	delete m_pOutFile;
	m_pOutFile = NULL;
	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CConsoleLog::Write( TCHAR* ptszData, bool bIsAutoEnter )
{
	if( NULL == m_pOutFile )
	{
		return;
	}

	if( true == bIsAutoEnter )
	{
		*m_pOutFile << ptszData << "\r\n";
	}
	else
	{
		*m_pOutFile << ptszData;
	}
	return;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CConsoleLog::Flush( void )
{
	*m_pOutFile << std::flush;
}