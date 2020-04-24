#include "precomp_trigger.h"
#include "DboTSEMRB.h"
#include "DboTSCoreDefine.h"
#include "DboTSRB.h"
#include "NtlTSMemIO.h"


/** 
	Event mapper rank-battle receiver
*/

struct sRB_PARAM
{
	sRB_PARAM( void ) : bFind( false ), uiTblIdx( 0xffffffff ) { return; }

	bool						bFind;
	unsigned int				uiTblIdx;
};

class CDboTSEMRB_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMRB_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_RB )
	{
		((sRB_PARAM*)pParam)->bFind = true;

		CDboTSRB* pRB = (CDboTSRB*) pEntity;
		((sRB_PARAM*)pParam)->uiTblIdx = pRB->GetRBTblIdx();
	}

	return 0;
}


/** 
	Event mapper rank-battle
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMRB, CNtlTSEvtMapper )


CDboTSEMRB::CDboTSEMRB( void )
{
}

CDboTSEMRB::~CDboTSEMRB( void )
{
}

const CNtlTSEvtMapper::vecdef_TID_LIST* CDboTSEMRB::FindTSList( unsigned int uiId, const std::string& strKey ) const
{
	if ( 0xffffffff == uiId )
	{
		hashdef_TS_LIST::const_iterator it = m_defTotalTSList.find( strKey );
		if ( it == m_defTotalTSList.end() )
		{
			return 0;
		}
		else
		{
			return &(it->second);
		}
	}
	else
	{
		return CNtlTSEvtMapper::FindTSList( uiId, strKey );
	}
}

bool CDboTSEMRB::Load( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "rb" );

	if ( NULL == pFile )
	{
		return false;
	}

	m_defTotalTSList.clear();

	//////////////////////////////////////////////////////////////////////////
	//	TS list counter
	//////////////////////////////////////////////////////////////////////////
	int nTSListCnt;
	fread( &nTSListCnt, sizeof(nTSListCnt), 1, pFile );

	for ( int nCurTSListCnt = 0; nCurTSListCnt < nTSListCnt; ++nCurTSListCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//	string
		//////////////////////////////////////////////////////////////////////////
		int nStrLen;
		fread( &nStrLen, sizeof(nStrLen), 1, pFile );

		char* pStr = new char[nStrLen+1];
		fread( pStr, nStrLen, 1, pFile );
		pStr[nStrLen] = '\0';

		vecdef_TID_LIST* pTrigList = &m_defTotalTSList.insert( hashdef_TS_LIST::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

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

bool CDboTSEMRB::Load( const char* pData, int nDataSize )
{
	CNtlTSMemInput clMemInput( pData, nDataSize );

	m_defTotalTSList.clear();

	//////////////////////////////////////////////////////////////////////////
	//	TS list counter
	//////////////////////////////////////////////////////////////////////////
	int nTSListCnt;
	if ( !clMemInput.Read( &nTSListCnt, sizeof(nTSListCnt) ) )
	{
		m_defTotalTSList.clear();
		return false;
	}

	for ( int nCurTSListCnt = 0; nCurTSListCnt < nTSListCnt; ++nCurTSListCnt )
	{
		//////////////////////////////////////////////////////////////////////////
		//	string
		//////////////////////////////////////////////////////////////////////////
		int nStrLen;
		if ( !clMemInput.Read( &nStrLen, sizeof(nStrLen) ) )
		{
			m_defTotalTSList.clear();
			return false;
		}

		char* pStr = new char[nStrLen+1];

		if ( !clMemInput.Read( pStr, nStrLen ) )
		{
			m_defTotalTSList.clear();
			return false;
		}

		pStr[nStrLen] = '\0';

		vecdef_TID_LIST* pTrigList = &m_defTotalTSList.insert( hashdef_TS_LIST::value_type( pStr, vecdef_TID_LIST() ) ).first->second;

		delete [] pStr;
		pStr = NULL;

		//////////////////////////////////////////////////////////////////////////
		//	Trigger counter
		//////////////////////////////////////////////////////////////////////////
		int nTrigCnt;
		if ( !clMemInput.Read( &nTrigCnt, sizeof(nTrigCnt) ) )
		{
			m_defTotalTSList.clear();
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
				m_defTotalTSList.clear();
				return false;
			}

			pTrigList->push_back( tID );
		}
	}

	return true;
}

bool CDboTSEMRB::Save( const std::string& strFileName )
{
	FILE* pFile = NULL;
	fopen_s( &pFile, strFileName.c_str(), "wb" );

	if ( NULL == pFile )
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//	TS list counter
	//////////////////////////////////////////////////////////////////////////
	int nTSListCnt = (int)m_defTotalTSList.size();
	fwrite( &nTSListCnt, sizeof(nTSListCnt), 1, pFile );

	hashdef_TS_LIST::iterator itTSList = m_defTotalTSList.begin();
	for ( ; itTSList != m_defTotalTSList.end(); ++itTSList )
	{
		//////////////////////////////////////////////////////////////////////////
		//	string
		//////////////////////////////////////////////////////////////////////////
		int nStrLen = (int)itTSList->first.size();
		const char* pStr = itTSList->first.c_str();
		fwrite( &nStrLen, sizeof(nStrLen), 1, pFile );
		fwrite( pStr, nStrLen, 1, pFile );

		//////////////////////////////////////////////////////////////////////////
		//	Trigger counter
		//////////////////////////////////////////////////////////////////////////
		int nTrigCnt = (int)itTSList->second.size();
		fwrite( &nTrigCnt, sizeof(nTrigCnt), 1, pFile );

		vecdef_TID_LIST::iterator itTList = itTSList->second.begin();
		for ( ; itTList != itTSList->second.end(); ++itTList )
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

bool CDboTSEMRB::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pTblList );
	UNREFERENCED_PARAMETER( pParameter );

	sRB_PARAM sParam;
	CDboTSEMRB_Recv clRecv;

	// Trigger system script들을 순회하면서 해당 rank-battle table index로 시작할 수 있는
	// trigger를 찾는다

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
			if  ( m_defMapper.find( sParam.uiTblIdx ) == m_defMapper.end() )
			{
				m_defMapper[sParam.uiTblIdx][strKey] = vecdef_TID_LIST();

				m_defMapper[sParam.uiTblIdx][strKey].push_back( pTrig->GetID() );
			}
			else
			{
				if ( m_defMapper[sParam.uiTblIdx].find( strKey ) == m_defMapper[sParam.uiTblIdx].end() )
				{
					m_defMapper[sParam.uiTblIdx][strKey] = vecdef_TID_LIST();

					m_defMapper[sParam.uiTblIdx][strKey].push_back( pTrig->GetID() );
				}
				else
				{
					m_defMapper[sParam.uiTblIdx][strKey].push_back( pTrig->GetID() );
				}
			}

			if ( m_defTotalTSList.find( strKey ) == m_defTotalTSList.end() )
			{
				m_defTotalTSList[strKey] = vecdef_TID_LIST();
				m_defTotalTSList[strKey].push_back( pTrig->GetID() );
			}
			else
			{
				m_defTotalTSList[strKey].push_back( pTrig->GetID() );
			}
		}
	}

	return true;
}

bool CDboTSEMRB::DelBuildData( const std::string& strKey )
{
	mapdef_MAPPER::iterator itMapper = m_defMapper.begin();
	for ( ; itMapper != m_defMapper.end(); ++itMapper )
	{
		hashdef_TS_LIST& TSList = itMapper->second;
		hashdef_TS_LIST::iterator itFind = TSList.find( strKey );
		if ( itFind != TSList.end() )
		{
			TSList.erase( itFind );
		}
	}

	return true;
}

