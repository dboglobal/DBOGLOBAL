#include "precomp_trigger.h"
#include "DboTSEMFreeBattle.h"
#include "DboTSCoreDefine.h"
#include "NtlTSMemIO.h"


struct sFREEBATTLE_PARAM
{
	sFREEBATTLE_PARAM( void ) : bFind( false ) { return; }

	bool						bFind;
};

class CDboTSEMFreeBattle_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMFreeBattle_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_FREEBATTLE )
	{
		((sFREEBATTLE_PARAM*)pParam)->bFind = true;
	}

	return 0;
}


/** 
	Event mapper free battle
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMFreeBattle, CNtlTSEvtMapper )


CDboTSEMFreeBattle::CDboTSEMFreeBattle( void )
{
}

CDboTSEMFreeBattle::~CDboTSEMFreeBattle( void )
{
}

const CNtlTSEvtMapper::vecdef_TID_LIST* CDboTSEMFreeBattle::FindTSList( unsigned int uiId, const std::string& strKey ) const
{
	UNREFERENCED_PARAMETER( uiId );

	mapdef_FREEBATTLE_MAPPER::const_iterator it = m_defFreeBattleMapper.find( strKey );
	if ( it == m_defFreeBattleMapper.end() ) return NULL;

	return &(it->second);
}

void CDboTSEMFreeBattle::Clear( void )
{
	m_defFreeBattleMapper.clear();
}

bool CDboTSEMFreeBattle::Load( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "rb" );

	if ( NULL == pFile )
	{
		return false;
	}

	m_defFreeBattleMapper.clear();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	fread( &nMapperCnt, sizeof(nMapperCnt), 1, pFile );

	for ( int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	String
		//
		//////////////////////////////////////////////////////////////////////////
		int nStrLen;
		fread( &nStrLen, sizeof(nStrLen), 1, pFile );

		char* pStr = new char[nStrLen+1];
		fread( pStr, nStrLen, 1, pFile );
		pStr[nStrLen] = '\0';

		vecdef_TID_LIST* pTrigList = &m_defFreeBattleMapper.insert( mapdef_FREEBATTLE_MAPPER::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

		delete [] pStr;
		pStr = NULL;

		//////////////////////////////////////////////////////////////////////////
		//	Trigger counter
		//////////////////////////////////////////////////////////////////////////
		int nTrigCnt;
		fread( &nTrigCnt, sizeof(nTrigCnt), 1, pFile );

		for ( int nCurTrigCnt = 0; nCurTrigCnt < nTrigCnt; ++nCurTrigCnt )
		{
			//////////////////////////////////////////////////////////////////////////
			//	Trigger
			//////////////////////////////////////////////////////////////////////////
			NTL_TS_T_ID tID;
			fread( &tID, sizeof(tID), 1, pFile );

			pTrigList->push_back( tID );
		}
	}

	fclose( pFile );

	return true;
}

bool CDboTSEMFreeBattle::Load( const char* pData, int nDataSize )
{
	CNtlTSMemInput clMemInput( pData, nDataSize );

	m_defFreeBattleMapper.clear();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	if ( !clMemInput.Read( &nMapperCnt, sizeof(nMapperCnt) ) )
	{
		m_defFreeBattleMapper.clear();
		return false;
	}

	for ( int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	String
		//
		//////////////////////////////////////////////////////////////////////////
		int nStrLen;
		if ( !clMemInput.Read( &nStrLen, sizeof(nStrLen) ) )
		{
			m_defFreeBattleMapper.clear();
			return false;
		}

		char* pStr = new char[nStrLen+1];

		if ( !clMemInput.Read( pStr, nStrLen ) )
		{
			m_defFreeBattleMapper.clear();
			return false;
		}

		pStr[nStrLen] = '\0';

		vecdef_TID_LIST* pTrigList = &m_defFreeBattleMapper.insert( mapdef_FREEBATTLE_MAPPER::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

		delete [] pStr;
		pStr = NULL;

		//////////////////////////////////////////////////////////////////////////
		//	Trigger counter
		//////////////////////////////////////////////////////////////////////////
		int nTrigCnt;
		if ( !clMemInput.Read( &nTrigCnt, sizeof(nTrigCnt) ) )
		{
			m_defFreeBattleMapper.clear();
			return false;
		}

		for ( int nCurTrigCnt = 0; nCurTrigCnt < nTrigCnt; ++nCurTrigCnt )
		{
			//////////////////////////////////////////////////////////////////////////
			//	Trigger
			//////////////////////////////////////////////////////////////////////////
			NTL_TS_T_ID tID;
			if ( !clMemInput.Read( &tID, sizeof(tID) ) )
			{
				m_defFreeBattleMapper.clear();
				return false;
			}

			pTrigList->push_back( tID );
		}
	}

	return true;
}

bool CDboTSEMFreeBattle::Save( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "wb" );

	if ( NULL == pFile )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt = (int)m_defFreeBattleMapper.size();
	fwrite( &nMapperCnt, sizeof(nMapperCnt), 1, pFile );

	mapdef_FREEBATTLE_MAPPER::iterator itMapper = m_defFreeBattleMapper.begin();
	for ( ; itMapper != m_defFreeBattleMapper.end(); ++itMapper )
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	String
		//
		//////////////////////////////////////////////////////////////////////////
		int nStrLen = (int)itMapper->first.size();
		const char* pStr = itMapper->first.c_str();
		fwrite( &nStrLen, sizeof(nStrLen), 1, pFile );
		fwrite( pStr, nStrLen, 1, pFile );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Trigger list size
		//
		//////////////////////////////////////////////////////////////////////////
		int nTrigListSize = (int)itMapper->second.size();
		fwrite( &nTrigListSize, sizeof(nTrigListSize), 1, pFile );

		vecdef_TID_LIST::iterator itTList = itMapper->second.begin();
		for ( ; itTList != itMapper->second.end(); ++itTList )
		{
			//////////////////////////////////////////////////////////////////////////
			//	Trigger
			//////////////////////////////////////////////////////////////////////////
			NTL_TS_T_ID tID = *itTList;
			fwrite( &tID, sizeof(tID), 1, pFile );
		}
	}

	fclose( pFile );

	return true;
}

bool CDboTSEMFreeBattle::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pTblList );
	UNREFERENCED_PARAMETER( pParameter );

	if ( m_defFreeBattleMapper.find( strKey ) == m_defFreeBattleMapper.end() )
	{
		m_defFreeBattleMapper[strKey] = vecdef_TID_LIST();
	}

	vecdef_TID_LIST& defTIDList = m_defFreeBattleMapper[strKey];

	// Trigger system script들을 순회하면서 해당 collision region으로 시작할 수 있는
	// trigger를 찾는다

	sFREEBATTLE_PARAM sParam;
	CDboTSEMFreeBattle_Recv clRecv;

	CNtlTSTrigger* pTrig;
	CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
	CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

	for ( ; citTSBegin != citTSEnd; ++citTSBegin )
	{
		pTrig = citTSBegin->second;

		sParam.bFind = false;

		pTrig->SearchTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam );

		if ( sParam.bFind )
		{
			defTIDList.push_back( citTSBegin->first );
		}
	}

	return true;
}

bool CDboTSEMFreeBattle::DelBuildData( const std::string& strKey )
{
	mapdef_FREEBATTLE_MAPPER::iterator it = m_defFreeBattleMapper.find( strKey );
	if ( it != m_defFreeBattleMapper.end() )
	{
		m_defFreeBattleMapper.erase( it );
	}

	return true;
}

