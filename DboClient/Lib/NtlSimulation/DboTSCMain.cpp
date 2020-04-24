#include "precomp_ntlsimulation.h"
#include "DboTSCMain.h"
#include "DboTSCQAgency.h"
#include "DboTSCQRecv.h"
#include "DboTSCTAgency.h"
#include "DboTSCTRecv.h"
#include "DboTSCObjAgency.h"
#include "DboTSFactory.h"
#include "NtlDebug.h"
#include "TableContainer.h"
#include "NtlSLApi.h"
#include "WorldTable.h"
#include "NPCTable.h"
#include "SpawnTable.h"
#include "DboTSEntityFastFinder.h"
#include "NtlUnzip.h"
#include "NtlCipher.h"


class CClientTSLog : public CNtlTSLog
{
protected:
	virtual void						OnLogEvent( const char* pLog )
	{
		DBO_WARNING_MESSAGE(pLog );
	}
};

/** 
	Client TS main
*/


CDboTSCMain* CDboTSCMain::s_pInstance = NULL;


NTL_TS_IMPLEMENT_RTTI( CDboTSCMain, CDboTSMain )


CDboTSCMain::CDboTSCMain( void )
{
	NTL_ASSERT( NULL == s_pInstance, "CDboTSCMain::CDboTSCMain" );

	s_pInstance = this;

	m_bCreated = false;

	m_pEntityFastFinder = 0;

	m_pQAgency = 0;
	m_pTAgency = 0;
	m_pObjAgency = 0;

	m_bSchedulingLoad = false;

	m_pclQuestZip = NULL;
	m_pclTriggerZip = NULL;
	m_pclObjectZip = NULL;

	m_pQRecv = 0;
	m_pTRecv = 0;
}

CDboTSCMain::~CDboTSCMain( void )
{
	Delete();

	NTL_ASSERT( s_pInstance, "CDboTSCMain::~CDboTSCMain" );
	s_pInstance = NULL;
}

bool CDboTSCMain::IsCreated( void )
{
	return m_bCreated;
}

bool CDboTSCMain::Create( void )
{
	m_strQuestPath = ".\\ts\\quest.e";
	m_strTriggerPath = ".\\ts\\pctrigger.e";
	m_strObjectPath = ".\\ts\\objtrigger.e";

	if ( !HasEventMapperFromFile() )
	{
		m_bSchedulingLoad = false;
	}
	else
	{
		m_bSchedulingLoad = true;
	}

	m_pclQuestZip = NTL_NEW CNtlUnzip;
	m_pclTriggerZip = NTL_NEW CNtlUnzip;
	m_pclObjectZip = NTL_NEW CNtlUnzip;

	m_pEntityFastFinder = NTL_NEW CDboTSEntityFastFinder;

	if ( !m_pEntityFastFinder->Create() )
	{
		CNtlTSLog::Log( "Can not create CDboTSEntityFastFinder instance. [%s]", TS_CODE_TRACE() );

		NTL_DELETE( m_pEntityFastFinder );
	}

	RwV3d v3Pos;
	CDboTSWorldMng* pTSWorldMng = NULL;
	CWorldTable* pWorldTbl = API_GetTableContainer()->GetWorldTable();
	for ( CTable::TABLEIT itWorld = pWorldTbl->Begin(); itWorld != pWorldTbl->End(); ++itWorld )
	{
		sWORLD_TBLDAT* pWorldTblData = (sWORLD_TBLDAT*) itWorld->second;

		if ( m_pEntityFastFinder )
		{
			pTSWorldMng = m_pEntityFastFinder->CreateWorldMng( pWorldTblData );
		}

		CSpawnTable* pNPCSpawnTbl = API_GetTableContainer()->GetNpcSpawnTable( pWorldTblData->tblidx );
		for ( CTable::TABLEIT itNPCSpawn = pNPCSpawnTbl->Begin(); itNPCSpawn != pNPCSpawnTbl->End(); ++itNPCSpawn )
		{
			sSPAWN_TBLDAT* pNPCSpwnTblData = (sSPAWN_TBLDAT*) itNPCSpawn->second;

			sNPC_TBLDAT* pNPCTblData = (sNPC_TBLDAT*)API_GetTableContainer()->GetNpcTable()->FindData( pNPCSpwnTblData->mob_Tblidx );

			if ( pNPCTblData && pNPCTblData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_QUEST_GRANTER )
			{
				v3Pos.x = pNPCSpwnTblData->vSpawn_Loc.x;
				v3Pos.y = pNPCSpwnTblData->vSpawn_Loc.y;
				v3Pos.z = pNPCSpwnTblData->vSpawn_Loc.z;

				RegisterNPCPosition( itWorld->first, pNPCSpwnTblData->mob_Tblidx, v3Pos );

				if ( m_pEntityFastFinder && pTSWorldMng )
				{
					pTSWorldMng->AddNPC( v3Pos.x, v3Pos.z, pNPCSpwnTblData->mob_Tblidx );
				}
			}
		}

		if ( m_pEntityFastFinder && pTSWorldMng && pTSWorldMng->IsEmptyEntity() )
		{
			m_pEntityFastFinder->DeleteWorldMng( pTSWorldMng );
			pTSWorldMng = NULL;
		}
	}

	bool bRet = CDboTSMain::Create();

	if ( bRet )
	{
		m_bCreated = true;
	}

	return bRet;
}

void CDboTSCMain::Delete( void )
{
	ClearAllNPCPosition();

	if ( m_pEntityFastFinder )
	{
		m_pEntityFastFinder->Delete();

		NTL_DELETE( m_pEntityFastFinder );
	}

	DeleteAgency_Quest();
	DeleteAgency_Trigger();
	DeleteAgency_Object();

	CDboTSMain::Delete();

	NTL_DELETE( m_pclQuestZip );
	NTL_DELETE( m_pclTriggerZip );
	NTL_DELETE( m_pclObjectZip );

	m_bCreated = false;
}

CDboTSCQAgency* CDboTSCMain::CreateAgency_Quest( void )
{
	DeleteAgency_Quest();

	CNtlTSUIObject* pObj = m_pUIFactory->CreateObj( "CDboTSCQAgency" );
	if ( 0 == pObj || !pObj->IsDerivedClass( "CDboTSCQAgency" ) )
	{
		CNtlTSLog::Log( "Can not type cast from CNtlTSUIObject to CDboTSCQAgency. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		if ( 0 != pObj ) m_pUIFactory->DeleteObj( pObj );
		return 0;
	}

	m_pQAgency = (CDboTSCQAgency*)pObj;
	m_pQAgency->SetParent( this );
	m_pQAgency->SetRecv( GetQRecv() );

	return m_pQAgency;
}

void CDboTSCMain::DeleteAgency_Quest( void )
{
	if ( m_pQAgency )
	{
		DeleteAgency( (CNtlTSAgency*&)m_pQAgency );
	}
}

CDboTSCTAgency* CDboTSCMain::CreateAgency_Trigger( void )
{
	DeleteAgency_Trigger();

	CNtlTSUIObject* pObj = m_pUIFactory->CreateObj( "CDboTSCTAgency" );
	if ( 0 == pObj || !pObj->IsDerivedClass( "CDboTSCTAgency" ) )
	{
		CNtlTSLog::Log( "Can not type cast from CNtlTSUIObject to CDboTSCTAgency. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		if ( 0 != pObj ) m_pUIFactory->DeleteObj( pObj );
		return 0;
	}

	m_pTAgency = (CDboTSCTAgency*)pObj;
	m_pTAgency->SetParent( this );
	m_pTAgency->SetRecv( GetTRecv() );

	return m_pTAgency;
}

void CDboTSCMain::DeleteAgency_Trigger( void )
{
	if ( m_pTAgency )
	{
		DeleteAgency( (CNtlTSAgency*&)m_pTAgency );
	}
}

CDboTSCObjAgency* CDboTSCMain::CreateAgency_Object( void )
{
	DeleteAgency_Object();

	CNtlTSUIObject* pObj = m_pUIFactory->CreateObj( "CDboTSCObjAgency" );
	if ( 0 == pObj || !pObj->IsDerivedClass( "CDboTSCObjAgency" ) )
	{
		CNtlTSLog::Log( "Can not type cast from CNtlTSUIObject to CDboTSCObjAgency. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		if ( 0 != pObj ) m_pUIFactory->DeleteObj( pObj );
		return 0;
	}

	m_pObjAgency = (CDboTSCObjAgency*)pObj;
	m_pObjAgency->SetParent( this );

	return m_pObjAgency;
}

void CDboTSCMain::DeleteAgency_Object( void )
{
	if ( m_pObjAgency )
	{
		DeleteAgency( (CNtlTSAgency*&)m_pObjAgency );
	}
}

CNtlTSTrigger* CDboTSCMain::FindQuestFromTS( NTL_TS_T_ID tId )
{
	mapdef_TLIST::iterator it = m_defQuest.find( tId );
	if ( it == m_defQuest.end() )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTriggerObjectFromUnZip( tId, m_pclQuestZip, m_defQuest ) )
			{
				return 0;
			}
			else
			{
				it = m_defQuest.find( tId );

				if ( it == m_defQuest.end() )
				{
					return 0;
				}

				return it->second;
			}
		}

		return 0;
	}

	return it->second;
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::BeginQuestFromTS( void ) const
{
	return m_defQuest.begin();
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::EndQuestFromTS( void ) const
{
	return m_defQuest.end();
}

CNtlTSTrigger* CDboTSCMain::FindTriggerFromTS( NTL_TS_T_ID tId )
{
	mapdef_TLIST::iterator it = m_defTrigger.find( tId );
	if ( it == m_defTrigger.end() )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTriggerObjectFromUnZip( tId, m_pclTriggerZip, m_defTrigger ) )
			{
				return 0;
			}
			else
			{
				it = m_defTrigger.find( tId );

				if ( it == m_defTrigger.end() )
				{
					return 0;
				}

				return it->second;
			}
		}

		return 0;
	}

	return it->second;
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::BeginTriggerFromTS( void ) const
{
	return m_defTrigger.begin();
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::EndTriggerFromTS( void ) const
{
	return m_defTrigger.end();
}

CNtlTSTrigger* CDboTSCMain::FindObjectFromTS( NTL_TS_T_ID tId )
{
	mapdef_TLIST::iterator it = m_defObject.find( tId );
	if ( it == m_defObject.end() )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTriggerObjectFromUnZip( tId, m_pclObjectZip, m_defObject ) )
			{
				return 0;
			}
			else
			{
				it = m_defObject.find( tId );

				if ( it == m_defObject.end() )
				{
					return 0;
				}

				return it->second;
			}
		}

		return 0;
	}

	return it->second;
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::BeginObjectFromTS( void ) const
{
	return m_defObject.begin();
}

CNtlTSMain::mapdef_TLISTC_CIT CDboTSCMain::EndObjectFromTS( void ) const
{
	return m_defObject.end();
}

RwV3d* CDboTSCMain::FindNPCPosition( TBLIDX WorldIdx, TBLIDX NPCIdx )
{
	NPC_POS_KEY Key = (((NPC_POS_KEY)WorldIdx << 32) | (NPC_POS_KEY)NPCIdx);

	mapdef_NPC_POS_LIST::iterator it = m_defNPCPosList.find( Key );
	if ( it == m_defNPCPosList.end() ) return NULL;

	return &it->second.Pos;
}

bool CDboTSCMain::LoadLog( void )
{
	UnloadLog();

	m_pLog = NTL_NEW CClientTSLog;
	CNtlTSLog::Register( m_pLog );

	return true;
}

void CDboTSCMain::UnloadLog( void )
{
	if ( m_pLog )
	{
		CNtlTSLog::Unregister();

		NTL_DELETE( m_pLog );
	}
}

bool CDboTSCMain::LoadFactories( void )
{
	UnloadFactories();

	// Entity factory 생성
	m_pEntityFactory = NTL_NEW CDboTSEntityFactory;
	m_pEntityFactory->RegisterEntityType();

	// Control factory 생성
	m_pCtrlFactory = NTL_NEW CDboTSCCtrlFactory;
	m_pCtrlFactory->RegisterCtrlType();

	// UI factory 생성
	m_pUIFactory = NTL_NEW CDboTSCUIFactory;
	m_pUIFactory->RegisterUIType();

	return true;
}

void CDboTSCMain::UnloadFactories( void )
{
	NTL_DELETE( m_pUIFactory );
	NTL_DELETE( m_pCtrlFactory );
	NTL_DELETE( m_pEntityFactory );
}

bool CDboTSCMain::LoadScripts( void )
{
	UnloadAllScripts();

	if ( 'e' == m_strQuestPath[m_strQuestPath.size()-1] )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTSCryptoData_UnZip( m_strQuestPath, m_pclQuestZip ) )
			{
				return false;
			}
		}
		else
		{
			if ( !LoadTSCryptoData( m_strQuestPath, m_defQuest ) )
			{
				return false;
			}
		}
	}
	else
	{
		if ( !LoadTSZip( m_strQuestPath, m_defQuest ) )
		{
			char szPath[1024], szName[128];
			_splitpath_s( m_strQuestPath.c_str(), 0, 0, szPath, 1024, szName, 128, 0, 0 );
			std::string strPath = szPath;
			strPath += szName;

			if ( !LoadTSPath( strPath.c_str(), m_defQuest ) )
			{
				return false;
			}
		}
	}

	if ( 'e' == m_strTriggerPath[m_strTriggerPath.size()-1] )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTSCryptoData_UnZip( m_strTriggerPath, m_pclTriggerZip ) )
			{
				return false;
			}
		}
		else
		{
			if ( !LoadTSCryptoData( m_strTriggerPath, m_defTrigger ) )
			{
				return false;
			}
		}
	}
	else
	{
		if ( !LoadTSZip( m_strTriggerPath, m_defTrigger ) )
		{
			char szPath[1024], szName[128];
			_splitpath_s( m_strTriggerPath.c_str(), 0, 0, szPath, 1024, szName, 128, 0, 0 );
			std::string strPath = szPath;
			strPath += szName;

			if ( !LoadTSPath( strPath.c_str(), m_defTrigger ) )
			{
				return false;
			}
		}
	}

	if ( 'e' == m_strObjectPath[m_strObjectPath.size()-1] )
	{
		if ( m_bSchedulingLoad )
		{
			if ( !LoadTSCryptoData_UnZip( m_strObjectPath, m_pclObjectZip ) )
			{
				return false;
			}
		}
		else
		{
			if ( !LoadTSCryptoData( m_strObjectPath, m_defObject ) )
			{
				return false;
			}
		}
	}
	else
	{
		if ( !LoadTSZip( m_strObjectPath, m_defObject ) )
		{
			char szPath[1024], szName[128];
			_splitpath_s( m_strObjectPath.c_str(), 0, 0, szPath, 1024, szName, 128, 0, 0 );
			std::string strPath = szPath;
			strPath += szName;

			if ( !LoadTSPath( strPath.c_str(), m_defObject ) )
			{
				return false;
			}
		}
	}

	return true;
}

void CDboTSCMain::UnloadAllScripts( void )
{
	mapdef_TLIST::iterator it;

	it = m_defQuest.begin();
	for ( ; it != m_defQuest.end(); ++it )
	{
		CNtlTSTrigger* pTrig = it->second;
		m_pEntityFactory->DeleteObj( (CNtlTSScrObject*&)pTrig );
	}
	m_defQuest.clear();

	it = m_defTrigger.begin();
	for ( ; it != m_defTrigger.end(); ++it )
	{
		CNtlTSTrigger* pTrig = it->second;
		m_pEntityFactory->DeleteObj( (CNtlTSScrObject*&)pTrig );
	}
	m_defTrigger.clear();

	it = m_defObject.begin();
	for ( ; it != m_defObject.end(); ++it )
	{
		CNtlTSTrigger* pTrig = it->second;
		m_pEntityFactory->DeleteObj( (CNtlTSScrObject*&)pTrig );
	}
	m_defObject.clear();
}

bool CDboTSCMain::LoadEventMappers( void )
{
	if ( !LoadEventMapperFromFile() )
	{
		if ( !LoadEventMapperFromRunTime() )
		{
			return false;
		}
	}

	return true;
}

void CDboTSCMain::UnloadEventMappers( void )
{
	hashdef_EVT_MAPPER_LIST::iterator it = m_defEvtMapper.begin();
	for ( ; it != m_defEvtMapper.end(); ++it )
	{
		m_pCtrlFactory->DeleteObj( (CNtlTSControlObject *&)it->second );
	}
	m_defEvtMapper.clear();
}

bool CDboTSCMain::LoadRecv( void )
{
	UnloadRecv();

	CNtlTSUIObject* pObj;

	pObj = m_pUIFactory->CreateObj( "CDboTSCQRecv" );
	if ( 0 == pObj || !pObj->IsDerivedClass( "CDboTSCQRecv" ) )
	{
		CNtlTSLog::Log( "Can not type cast from CNtlTSUIObject to CDboTSCQRecv. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}
	m_pQRecv = (CDboTSCQRecv*) pObj;

	pObj = m_pUIFactory->CreateObj( "CDboTSCTRecv" );
	if ( 0 == pObj || !pObj->IsDerivedClass( "CDboTSCTRecv" ) )
	{
		CNtlTSLog::Log( "Can not type cast from CNtlTSUIObject to CDboTSCTRecv. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}
	m_pTRecv = (CDboTSCTRecv*) pObj;

	return true;
}

void CDboTSCMain::UnloadRecv( void )
{
	if ( m_pTRecv )
	{
		m_pUIFactory->DeleteObj( (CNtlTSUIObject*&)m_pTRecv );
	}

	if ( m_pQRecv )
	{
		m_pUIFactory->DeleteObj( (CNtlTSUIObject*&)m_pQRecv );
	}
}

void CDboTSCMain::RegisterNPCPosition( TBLIDX WorldIdx, TBLIDX NPCIdx, RwV3d& v3Pos )
{
	NPC_POS_KEY Key = (((NPC_POS_KEY)WorldIdx << 32) | (NPC_POS_KEY)NPCIdx);

	sNPC_POS_INFO sPosInfo;
	sPosInfo.Pos = v3Pos;

	m_defNPCPosList[Key] = sPosInfo;
}

void CDboTSCMain::ClearAllNPCPosition( void )
{
	m_defNPCPosList.clear();
}

void CDboTSCMain::LoadEventMapper( sUZ_FILEINFO* pFileInfo, char* pBuf )
{
	std::string strName = pFileInfo->szFileName;

	std::string::size_type pos = strName.find( ".evt" );
	if ( std::string::npos != pos )
	{
		strName = strName.substr( 0, pos );
	}

	std::string strTypeName = strName;

	pos = strName.find( "_" );
	if ( std::string::npos != pos )
	{
		strTypeName = strName.substr( 0, pos );
	}

	CNtlTSEvtMapper* pEvtMapper = (CNtlTSEvtMapper*)m_pCtrlFactory->CreateObj( strTypeName );
	if (pEvtMapper)
	{
		if (pEvtMapper->Load(pBuf, pFileInfo->dwUncompressedSize))
		{
			m_defEvtMapper[strName.c_str()] = pEvtMapper;
		}
		else
		{
			CNtlTSLog::Log("Loading event mapper failed. Info[%s]. [%s]", strName.c_str(), TS_CODE_TRACE());

			m_pCtrlFactory->DeleteObj((CNtlTSControlObject*&)pEvtMapper);
		}
	}
}

bool CDboTSCMain::HasEventMapperFromFile( void )
{
	std::string strEvtPath = ".\\ts\\evt.e";

	FILE* pFile;
	fopen_s( &pFile, strEvtPath.c_str(), "rb" );
	if ( NULL == pFile )
	{
		return false;
	}

	fclose( pFile );

	return true;
}

bool CDboTSCMain::LoadEventMapperFromFile( void )
{
	std::string strEvtPath = ".\\ts\\evt.e";

	char* pReadBuf = NULL;
	char* pDecryptBuf = NULL;
	char* pBuf = NULL;

	// 파일 로딩
	FILE* pFile;
	fopen_s( &pFile, strEvtPath.c_str(), "rb" );
	if ( NULL == pFile )
	{
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	int nReadSize = ftell( pFile ) - 4;
	fseek( pFile, 0, SEEK_SET );
	pReadBuf = NTL_NEW char[nReadSize];
	int nOriginSize;
	fread( &nOriginSize, sizeof(int), 1, pFile );
	fread( pReadBuf, 1, nReadSize, pFile );
	fclose( pFile );

	// 암호화를 푼다
	std::string strKey = "dnfldbofmftkfkdgowntpdy";
	CNtlCipher Cipher;
	Cipher.SetKey( DES_CIPHER, strKey.c_str(), (int)strKey.size() );
	pDecryptBuf = NTL_NEW char[nReadSize+512];
	ZeroMemory( pDecryptBuf, nReadSize+512 );
	Cipher.Decrypt( pReadBuf, nReadSize, pDecryptBuf, nReadSize+512 );

	CNtlUnzip clZip;
	if ( !clZip.OpenZip( strEvtPath.c_str(), pDecryptBuf, nOriginSize ) )
	{
		CNtlTSLog::Log( "Zip file open failed. Info[%s]. [%s]", strEvtPath.c_str(), TS_CODE_TRACE() );

		NTL_ARRAY_DELETE( pReadBuf );
		NTL_ARRAY_DELETE( pDecryptBuf );

		return false;
	}

	if ( !clZip.GotoFirstFile() )
	{
		CNtlTSLog::Log( "Zip GotoFirstFile failed. Info[%s]. [%s]", strEvtPath.c_str(), TS_CODE_TRACE() );

		NTL_ARRAY_DELETE( pReadBuf );
		NTL_ARRAY_DELETE( pDecryptBuf );

		return false;
	}

	unsigned int MAX_BUFFER_SIZE = 1024000;
	pBuf = NTL_NEW char[MAX_BUFFER_SIZE];

	sUZ_FILEINFO sInfo;

	do 
	{
		clZip.GetFileInfo( sInfo );

		if ( sInfo.bFolder )
		{
			continue;
		}

		int nNameLength = (int)strlen( sInfo.szFileName );

		if ( nNameLength <= 2 )
		{
			continue;
		}

		if ( '.' != sInfo.szFileName[nNameLength-4] ||
			 'e' != sInfo.szFileName[nNameLength-3] ||
			 'v' != sInfo.szFileName[nNameLength-2] ||
			 't' != sInfo.szFileName[nNameLength-1] )
		{
			continue;
		}

		if ( MAX_BUFFER_SIZE < sInfo.dwUncompressedSize )
		{
			MAX_BUFFER_SIZE = sInfo.dwUncompressedSize + 1;

			NTL_ARRAY_DELETE( pBuf );

			pBuf = NTL_NEW char[MAX_BUFFER_SIZE];
		}

		if ( !clZip.ReadFileData( pBuf, sInfo.dwUncompressedSize ) )
		{
			CNtlTSLog::Log( "Load TS compressed file. Info[%s]. [%s]", sInfo.szFileName, TS_CODE_TRACE() );
		}
		else
		{
			LoadEventMapper( &sInfo, pBuf );
		}
	}
	while ( clZip.GotoNextFile() );

	NTL_ARRAY_DELETE( pBuf );
	NTL_ARRAY_DELETE( pReadBuf );
	NTL_ARRAY_DELETE( pDecryptBuf );

	return true;
}

bool CDboTSCMain::LoadEventMapperFromRunTime( void )
{
	CNtlTSControlObject* pObj;

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMNPC" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMNPC" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "quest", m_defQuest, API_GetTableContainer()->GetNpcTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the npc in the npc event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "pctrigger", m_defTrigger, API_GetTableContainer()->GetNpcTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the npc in the npc event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMNPC. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	CTableContainer::OBJTABLEIT it = API_GetTableContainer()->BeginObjectTable();
	CTableContainer::OBJTABLEIT itEnd = API_GetTableContainer()->EndObjectTable();

	for ( ; it != itEnd; ++it )
	{
		pObj = m_pCtrlFactory->CreateObj( "CDboTSEMObject" );
		if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMObject" ) )
		{
			unsigned int uiWorldTblIdx = it->first;

			sprintf_s( g_NtlTSString, "CDboTSEMObject_%d", uiWorldTblIdx );

			m_defEvtMapper[g_NtlTSString] = (CNtlTSEvtMapper*)pObj;

			if ( !((CDboTSEMObject*)pObj)->AddBuildData( "quest", m_defQuest, it->second, &uiWorldTblIdx ) )
			{
				CNtlTSLog::Log( "Building the npc in the object event mapper is failed. [%s].", TS_CODE_TRACE() );
				return false;
			}

			if ( !((CDboTSEMObject*)pObj)->AddBuildData( "pctrigger", m_defTrigger, it->second, &uiWorldTblIdx ) )
			{
				CNtlTSLog::Log( "Building the npc in the object event mapper is failed. [%s].", TS_CODE_TRACE() );
				return false;
			}
		}
		else
		{
			CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMObject. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
			return false;
		}
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItem" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItem" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItem*)pObj)->AddBuildData( "quest", m_defQuest, API_GetTableContainer()->GetItemTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the item in the item event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItem*)pObj)->AddBuildData( "pctrigger", m_defTrigger, API_GetTableContainer()->GetItemTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the item in the item event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItem. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSvrEvt" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSvrEvt" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSvrEvt*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building server event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSvrEvt*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building server event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSvrEvt. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSkill" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSkill" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSkill*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building skill event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSkill*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building skill event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSkill. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMColRgn" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMColRgn" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMColRgn*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building collision region event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMColRgn*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building collision region mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMColRgn. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMRB" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMRB" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMRB*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building rank-battle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMRB*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building rank-battle mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMRB. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMMob" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMMob" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "quest", m_defQuest, API_GetTableContainer()->GetMobTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the mob in the mob event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMNPC*)pObj)->AddBuildData( "pctrigger", m_defTrigger, API_GetTableContainer()->GetMobTable(), NULL ) )
		{
			CNtlTSLog::Log( "Building the mob in the mob event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMMob. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMBindStone" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMBindStone" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMBindStone*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building bind stone event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMBindStone*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building bind stone event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMBindStone. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSearchQuest" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSearchQuest" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSearchQuest*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building quest search event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSearchQuest*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building quest search event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSearchQuest. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItemUpgrade" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItemUpgrade" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItemUpgrade*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item upgrade event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItemUpgrade*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item upgrade event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItemUpgrade. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMTeleport" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMTeleport" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMTeleport*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building teleport event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMTeleport*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building teleport event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMTeleport. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMBudokai" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMBudokai" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMBudokai*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building budokai mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMBudokai*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building budokai event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMBudokai. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMSlotMachine" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMSlotMachine" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMSlotMachine*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building SlotMachine event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMSlotMachine*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building SlotMachine event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMSlotMachine. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMHoipoiMix" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMHoipoiMix" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMHoipoiMix*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building HoipoiMix event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMHoipoiMix*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building HoipoiMix event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMHoipoiMix. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMPrivateShop" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMPrivateShop" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMPrivateShop*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building PrivateShop event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMPrivateShop*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building PrivateShop event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMPrivateShop. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMFreeBattle" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMFreeBattle" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMFreeBattle*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building FreeBattle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMFreeBattle*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building FreeBattle event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMFreeBattle. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMItemIdentity" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMItemIdentity" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMItemIdentity*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item identity event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMItemIdentity*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building item identity event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMItemIdentity. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMUseMail" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMUseMail" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMUseMail*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building use mail event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMUseMail*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building use mail event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMUseMail. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj( "CDboTSEMParty" );
	if ( pObj && pObj->IsDerivedClass( "CNtlTSEvtMapper" ) && pObj->IsDerivedClass( "CDboTSEMParty" ) )
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if ( !((CDboTSEMParty*)pObj)->AddBuildData( "quest", m_defQuest, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building party event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}

		if ( !((CDboTSEMParty*)pObj)->AddBuildData( "pctrigger", m_defTrigger, NULL, NULL ) )
		{
			CNtlTSLog::Log( "Building party event mapper is failed. [%s].", TS_CODE_TRACE() );
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log( "Can not do type cast from CNtlTSControlObject to CDboTSEMParty. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE() );
		return false;
	}

	/// new

	pObj = m_pCtrlFactory->CreateObj("CDboTSEMFLink");
	if (pObj && pObj->IsDerivedClass("CNtlTSEvtMapper") && pObj->IsDerivedClass("CDboTSEMFLink"))
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if (!((CDboTSEMFLink*)pObj)->AddBuildData("quest", m_defQuest, NULL, NULL))
		{
			CNtlTSLog::Log("Building CDboTSEMFLink mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log("Can not do type cast from CNtlTSControlObject to CDboTSEMFLink. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE());
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj("CDboTSEMSideIcon");
	if (pObj && pObj->IsDerivedClass("CNtlTSEvtMapper") && pObj->IsDerivedClass("CDboTSEMSideIcon"))
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if (!((CDboTSEMSideIcon*)pObj)->AddBuildData("quest", m_defQuest, NULL, NULL))
		{
			CNtlTSLog::Log("Building party event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}

		if (!((CDboTSEMSideIcon*)pObj)->AddBuildData("pctrigger", m_defTrigger, NULL, NULL))
		{
			CNtlTSLog::Log("Building flink event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log("Can not do type cast from CNtlTSControlObject to CDboTSEMSideIcon. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE());
		return false;
	}


	pObj = m_pCtrlFactory->CreateObj("CDboTSEMLevelCheck");
	if (pObj && pObj->IsDerivedClass("CNtlTSEvtMapper") && pObj->IsDerivedClass("CDboTSEMLevelCheck"))
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if (!((CDboTSEMLevelCheck*)pObj)->AddBuildData("quest", m_defQuest, NULL, NULL))
		{
			CNtlTSLog::Log("Building party event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}

		if (!((CDboTSEMLevelCheck*)pObj)->AddBuildData("pctrigger", m_defTrigger, NULL, NULL))
		{
			CNtlTSLog::Log("Building flink event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log("Can not do type cast from CNtlTSControlObject to CDboTSEMLevelCheck. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE());
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj("CDboTSEMQuest");
	if (pObj && pObj->IsDerivedClass("CNtlTSEvtMapper") && pObj->IsDerivedClass("CDboTSEMQuest"))
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if (!((CDboTSEMQuest*)pObj)->AddBuildData("quest", m_defQuest, NULL, NULL))
		{
			CNtlTSLog::Log("Building party event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}

		if (!((CDboTSEMQuest*)pObj)->AddBuildData("pctrigger", m_defTrigger, NULL, NULL))
		{
			CNtlTSLog::Log("Building flink event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log("Can not do type cast from CNtlTSControlObject to CDboTSEMQuest. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE());
		return false;
	}

	pObj = m_pCtrlFactory->CreateObj("CDboTSEMDialogOpen");
	if (pObj && pObj->IsDerivedClass("CNtlTSEvtMapper") && pObj->IsDerivedClass("CDboTSEMDialogOpen"))
	{
		m_defEvtMapper[pObj->GetClassName()] = (CNtlTSEvtMapper*)pObj;

		if (!((CDboTSEMDialogOpen*)pObj)->AddBuildData("quest", m_defQuest, NULL, NULL))
		{
			CNtlTSLog::Log("Building party event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}

		if (!((CDboTSEMDialogOpen*)pObj)->AddBuildData("pctrigger", m_defTrigger, NULL, NULL))
		{
			CNtlTSLog::Log("Building flink event mapper is failed. [%s].", TS_CODE_TRACE());
			return false;
		}
	}
	else
	{
		CNtlTSLog::Log("Can not do type cast from CNtlTSControlObject to CDboTSEMDialogOpen. Info[%s]. [%s]", pObj ? pObj->GetClassName() : "NULL", TS_CODE_TRACE());
		return false;
	}

	return true;
}
