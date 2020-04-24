#include "precomp_navi.h"
#include "NtlNaviPEDataImportMng.h"
#include "NtlNaviLog.h"
#include "NtlNaviUtility.h"
#include "NtlNaviPEWorld.h"
#include "NtlNaviLoadingQueue.h"


CNtlNaviPEDataImportMng::CNtlNaviPEDataImportMng( void )
{
	m_bLoadingFailed = false;

	m_uiLoadBeginTime = 0;
	m_uiLoadEndTime = 0;
}

CNtlNaviPEDataImportMng::~CNtlNaviPEDataImportMng( void )
{
	Delete();
}

bool CNtlNaviPEDataImportMng::Create( void )
{
	return CNtlNaviLoadFinishCheckThread::Create();
}

void CNtlNaviPEDataImportMng::Delete( void )
{
	ClearWorld();

	CNtlNaviLoadFinishCheckThread::Delete();
}

bool CNtlNaviPEDataImportMng::WaitUntilLoadingFinish( unsigned int uiWaitTime, unsigned int& uiResult )
{
	HANDLE hEvent = GetLoadingFinishEvent();

	if ( hEvent )
	{
		uiResult = WaitForSingleObjectEx( hEvent, uiWaitTime, FALSE );
	}
	else
	{
		uiResult = 0xffffffff;
	}

	return !IsLoadingFailed();
}

unsigned int CNtlNaviPEDataImportMng::GetLoadingTime( void )
{
	return m_uiLoadEndTime - m_uiLoadBeginTime;
}

NAVI_INST_HANDLE CNtlNaviPEDataImportMng::CreateInstanceHandler( unsigned int uiWorldID )
{
	CNtlNaviPEWorld* pPEWorld = FindNaviWorld( uiWorldID );

	if ( NULL == pPEWorld )
	{
		return NULL;
	}

	return pPEWorld->CreateInstanceHandler( uiWorldID );
}

void CNtlNaviPEDataImportMng::DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )
{
	if ( NULL == hHandle )
	{
		return;
	}

	sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;

	CNtlNaviPEWorld* pPEWorld = FindNaviWorld( pHandler->uiWorldID );

	if ( NULL == pPEWorld )
	{
		return;
	}

	return pPEWorld->DeleteInstanceHandler( hHandle );
}

CNtlNaviPEWorld* CNtlNaviPEDataImportMng::FindNaviWorld( NAVI_INST_HANDLE hHandle )
{
	sNAVI_INST_HANDLER* pHandler = (sNAVI_INST_HANDLER*) hHandle;

	if ( NULL == pHandler )
	{
		return NULL;
	}

	return FindNaviWorld( pHandler->uiWorldID );
}

CNtlNaviPEWorld* CNtlNaviPEDataImportMng::FindNaviWorld( unsigned int uiWorldID )
{
	mapdef_PE_WORLD_LIST::iterator it = m_defPEWorldList.find( uiWorldID );
	if ( it == m_defPEWorldList.end() ) return NULL;
	return it->second;
}

bool CNtlNaviPEDataImportMng::ImportWorlds( const char* pRootPath, vecdef_WorldIDList& defWorldIDList, unsigned char byLoadFlags )
{
	m_uiLoadBeginTime = GetTickCount();

	std::string strRootPath = pRootPath;
	AttachBackSlash( strRootPath );

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// 파일을 찾지 못했을 때,
	hFind = FindFirstFile( (strRootPath + "*.*").c_str(), &FindFileData );
	if ( hFind == INVALID_HANDLE_VALUE )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import world all. [%s]", pRootPath );

		return false;
	}

	do
	{
		if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 (FindFileData.cFileName[0] != '.') )
		{
			std::string strWorldPath = strRootPath;
			strWorldPath += FindFileData.cFileName;

			unsigned int uiWorldID = atoi( FindFileData.cFileName );

			if ( defWorldIDList.end() != std::find( defWorldIDList.begin(), defWorldIDList.end(), uiWorldID ) )
			{
				continue;
			}

			if ( !ImportWorld( pRootPath, uiWorldID, byLoadFlags ) )
			{
				FindClose( hFind );

				return false;
			}
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	m_uiLoadEndTime = GetTickCount();

	if ( CNtlLoadingQueue::GetInstance() )
	{
		if ( !Begin() )
		{
			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import the worlds. Begin(). [%s, %d, %d]", pRootPath, defWorldIDList.size(), byLoadFlags );

			return false;
		}
	}

	return true;
}

bool CNtlNaviPEDataImportMng::IsLoadingFailed( void )
{
	m_csLoadingFailed.Lock();
	bool bLoadingFailed = m_bLoadingFailed;
	m_csLoadingFailed.Unlock();

	return bLoadingFailed;
}

void CNtlNaviPEDataImportMng::SetLoadingFailed( bool bLoadingFailed )
{
	m_csLoadingFailed.Lock();
	m_bLoadingFailed = bLoadingFailed;
	m_csLoadingFailed.Unlock();
}

bool CNtlNaviPEDataImportMng::ImportWorld( const char* pRootPath, unsigned int uiWorldID, unsigned char byLoadFlags )
{
	std::string strRootPath = pRootPath;
	AttachBackSlash( strRootPath );

	char szWorldID[128];
	sprintf_s( szWorldID, 128, "%d", uiWorldID );

	std::string strWorldPath = strRootPath + szWorldID;

	CNtlNaviPEWorld* pPEWorld = new CNtlNaviPEWorld;

	if ( !pPEWorld->ImportPathData( strWorldPath.c_str(), byLoadFlags ) )
	{
		CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import the world. [%s, %s, %d]", pRootPath, strWorldPath.c_str(), byLoadFlags );

		delete pPEWorld;
		return false;
	}

	AttachWorld( uiWorldID, pPEWorld );

	return true;
}

void CNtlNaviPEDataImportMng::AttachWorld( unsigned int uiWorldID, CNtlNaviPEWorld* pPEWorld )
{
	m_csPEWorldList.Lock();

	m_defPEWorldList[uiWorldID] = pPEWorld;

	m_csPEWorldList.Unlock();
}

void CNtlNaviPEDataImportMng::ClearWorld( void )
{
	m_csPEWorldList.Lock();

	mapdef_PE_WORLD_LIST::iterator it = m_defPEWorldList.begin();
	for ( ; it != m_defPEWorldList.end(); ++it )
	{
		delete it->second;
	}
	m_defPEWorldList.clear();

	m_csPEWorldList.Unlock();
}

unsigned int CNtlNaviPEDataImportMng::GetAttachedWorldCnt( void )
{
	CNtlNaviAutoCS clAuto( &m_csPEWorldList );

	return (unsigned int)m_defPEWorldList.size();
}

CNtlNaviPEWorld* CNtlNaviPEDataImportMng::GetAttachedWorld( int nIndex )
{
	CNtlNaviAutoCS clAuto( &m_csPEWorldList );

	return m_defPEWorldList[nIndex];
}

unsigned int CNtlNaviPEDataImportMng::ThreadCallBackFunc( void )
{
	unsigned int uiCompleteCnt = 0;

	unsigned int uiWorldCnt = GetAttachedWorldCnt();

	for ( unsigned int i = 0; i < uiWorldCnt; ++i )
	{
		CNtlNaviPEWorld* pPEWorld = GetAttachedWorld( i );

		if ( !pPEWorld->CheckLoadComplete() )
		{
			SetLoadingFailed( true );

			End();

			CNtlNaviLog::GetInstance()->Log( "[IMPORT] Can not import the world. [%s, %d]", pPEWorld->GetImportPath(), pPEWorld->GetLoadedFlags() );

			return 0;
		}

		if ( pPEWorld->GetCurState() == CNtlNaviPEWorld::eNAVI_PE_STATE_COMPLETE )
		{
			uiCompleteCnt++;
		}
	}

	if ( uiCompleteCnt == uiWorldCnt )
	{
		m_uiLoadEndTime = GetTickCount();

		End();
	}

	return 0;
}
