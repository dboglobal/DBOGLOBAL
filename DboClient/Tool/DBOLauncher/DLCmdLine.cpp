/*****************************************************************************
*
* File			: DLCmdLine.h
* Author		: 
* Copyright		: (주)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "StdAfx.h"
#include "DLCmdLine.h"
#include "NtlClientService.h"
#include "DBOLauncherDef.h"
#include "DLUtility.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLCmdLine
//
//////////////////////////////////////////////////////////////////////////


CDLCmdLine g_clDLCmdLine;


CDLCmdLine::CDLCmdLine( void )
{
}

CDLCmdLine::~CDLCmdLine( void )
{
}

bool CDLCmdLine::LoadCmdLine( const CString& strCmdLine )
{
	m_strCmdLine = strCmdLine;

	if ( !ParseCmdLine() )
	{
		CString strDebug; strDebug.Format( _T( "Loading the command line is failed. %s, %s, %d" ), m_strCmdLine, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	CString strServiceCmd = GetServiceCmd();

	char szServiceCmd[4096];

	if ( !strServiceCmd.IsEmpty() )
	{
		if ( 0 == ::WideCharToMultiByte( GetACP(), CP_ACP, strServiceCmd, -1, szServiceCmd, 4096, NULL, NULL ) )
		{
			CString strDebug; strDebug.Format( _T( "Loading the command line is failed. %s, %s, %d" ), strServiceCmd, __FILEW__, __LINE__ );
			DLSendMessage_ForDebug( strDebug );

			return false;
		}
	}
	else
	{
		memset( szServiceCmd, 0, sizeof( szServiceCmd ) );
	}

	if ( !GetServiceInterface()->Begin( szServiceCmd ) )
	{
		CString strDebug; strDebug.Format( _T( "Loading the command line is failed. %s, %s, %d" ), strServiceCmd, __FILEW__, __LINE__ );
		DLSendMessage_ForDebug( strDebug );

		return false;
	}

	return true;
}

void CDLCmdLine::UnloadCmdLine( void )
{
	GetServiceInterface()->End();
}

int CDLCmdLine::GetCmdTypeCnt( CDLCmdLine::eDL_CMD_TYPE eCmdType )
{
	int nCmdTypeCnt = 0;

	for ( vecdef_CMD_FIELD_DATA_LIST::iterator it = m_defFieldDataList.begin();
		  it != m_defFieldDataList.end();
		  ++it )
	{
		sCMD_FILED_DATA& sData = *it;

		if ( sData.eCmdType == eCmdType )
		{
			nCmdTypeCnt++;
		}
	}

	return nCmdTypeCnt;
}

CString CDLCmdLine::GetServiceCmd( void )
{
	CString strServiceCmd;

	for ( vecdef_CMD_FIELD_DATA_LIST::iterator it = m_defFieldDataList.begin();
		  it != m_defFieldDataList.end();
		  ++it )
	{
		sCMD_FILED_DATA& sData = *it;

		if ( eDL_CMD_TYPE_SERVICE == sData.eCmdType )
		{
			strServiceCmd += sData.strCmd;
			strServiceCmd += _T( " " );
		}
	}

	strServiceCmd.Trim();

	return strServiceCmd;
}

unsigned int CDLCmdLine::GetPublisherID( void )
{
	return GetServiceInterface()->GetPublisherID();
}

bool CDLCmdLine::ParseCmdLine( void )
{
	bool bRet = true;

	int nPos = 0;
	CString strToken;

	strToken = m_strCmdLine.Tokenize( _T( " " ), nPos );
	while ( strToken != _T( "" ) )
	{
		sCMD_FILED_DATA sCmdFieldData;

		sCmdFieldData.eCmdType	= eDL_CMD_TYPE_INVALID;
		sCmdFieldData.strCmd	= strToken;

		// 1. Launcher command
		if ( DL_CMD_LAUNCHER_PATCH == strToken )
		{
			sCmdFieldData.eCmdType = eDL_CMD_TYPE_LAUNCHER_PATCH;
		}

		// 2. Service command
		else
		{
			unsigned int uiPublisherID = GetServiceInterface()->GetPublisherID();

			if ( eDBO_SERVICE_PUBLISHER_NTL == uiPublisherID )
			{
				// 개발용인 경우는 Launcher command 이외는 있을 수 없다
				sCmdFieldData.eCmdType	= eDL_CMD_TYPE_INVALID;

				bRet &= false;
			}
			else if ( eDBO_SERVICE_PUBLISHER_CJI_KOR == uiPublisherID )
			{
				// CJI Kor인 경우는 단 하나만의 Service command 만 존재한다
				if ( 0 == GetCmdTypeCnt( eDL_CMD_TYPE_SERVICE ) )
				{
					sCmdFieldData.eCmdType = eDL_CMD_TYPE_SERVICE;
				}
				else
				{
					sCmdFieldData.eCmdType	= eDL_CMD_TYPE_INVALID;

					bRet &= false;
				}
			}
			else
			{
				sCmdFieldData.eCmdType	= eDL_CMD_TYPE_INVALID;

				bRet &= false;
			}
		}

		m_defFieldDataList.push_back( sCmdFieldData );

		strToken = m_strCmdLine.Tokenize( _T( " " ), nPos );
	}

	return bRet;
}
