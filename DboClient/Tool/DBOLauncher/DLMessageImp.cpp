/*****************************************************************************
*
* File			: DLMessageImp.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/

#include "stdafx.h"
#include "DLMessageImp.h"


#define REG_MSG( k, s )		m_defMsgList[k] = s


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder
//
//////////////////////////////////////////////////////////////////////////


CDLMsgFinder::CDLMsgFinder( void )
{
	m_strDefMsg = _T( "Can not find the message." );
}

CDLMsgFinder::~CDLMsgFinder( void )
{
	Delete();
}

bool CDLMsgFinder::Create( void )
{
	RegString();

	return true;
}

void CDLMsgFinder::Delete( void )
{
	return;
}

CString CDLMsgFinder::FindString( eDL_MSG eMsg )
{
	mapdef_MsgList::iterator it = m_defMsgList.find( eMsg );

	if ( it == m_defMsgList.end() )
	{
		return m_strDefMsg;
	}

	return it->second;
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_KOR
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_KOR::RegString( void )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,	_T( "현재 런쳐를 이용할 수 없습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,			_T( "파일 다운로드가 실패하였습니다.\n우선 네트웍 설정 및 인터넷 접속 여부를 살펴보시기 바랍니다.\n그래도 이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,			_T( "패치 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,	_T( "무결성 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,			_T( "드래곤볼 온라인이 이미 실행 중 입니다" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,			_T( "드래곤볼 온라인 런쳐가 이미 실행 중 입니다." ) );

	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,				_T( "현재 새로운 버전의 패치가 준비중입니다.\n잠시후에 다시 접속하여 주시기 바랍니다." ) );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_JPN
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_JPN::RegString( void )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,	_T( "현재 런쳐를 이용할 수 없습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,			_T( "파일 다운로드가 실패하였습니다.\n우선 네트웍 설정 및 인터넷 접속 여부를 살펴보시기 바랍니다.\n그래도 이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,			_T( "패치 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,	_T( "무결성 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,			_T( "드래곤볼 온라인이 이미 실행 중 입니다" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,			_T( "드래곤볼 온라인 런쳐가 이미 실행 중 입니다." ) );

	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,				_T( "현재 새로운 버전의 패치가 준비중입니다.\n잠시후에 다시 접속하여 주시기 바랍니다." ) );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLMsgFinder_ENG
//
//////////////////////////////////////////////////////////////////////////


void CDLMsgFinder_ENG::RegString( void )
{
	REG_MSG( eDL_MSG_ERROR_NOT_PREPARED_LAUNCHER,	_T( "현재 런쳐를 이용할 수 없습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_ERROR_DOWNLOAD_FAILED,			_T( "파일 다운로드가 실패하였습니다.\n우선 네트웍 설정 및 인터넷 접속 여부를 살펴보시기 바랍니다.\n그래도 이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_PATCH_FAILED,			_T( "패치 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );
	REG_MSG( eDL_MSG_ERROR_INTEGRITY_CHECK_FAILED,	_T( "무결성 진행이 실패하였습니다.\n이러한 증상이 계속될 경우 운영자에게 연락 부탁드립니다" ) );

	REG_MSG( eDL_MSG_MUTEX_CHECK_CLIENT,			_T( "드래곤볼 온라인이 이미 실행 중 입니다" ) );
	REG_MSG( eDL_MSG_MUTEX_CHECK_LAUNCHER,			_T( "드래곤볼 온라인 런쳐가 이미 실행 중 입니다." ) );

	REG_MSG( eDL_MSG_PREPARE_NEW_PATCH,				_T( "현재 새로운 버전의 패치가 준비중입니다.\n잠시후에 다시 접속하여 주시기 바랍니다." ) );
}
