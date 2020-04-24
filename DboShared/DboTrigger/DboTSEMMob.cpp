#include "precomp_trigger.h"
#include "DboTSEMMob.h"
#include "MobTable.h"
#include "DboTSCoreDefine.h"
#include "DboTSClickMob.h"


/** 
	Event mapper mob receiver
*/

struct sMOB_PARAM
{
	sMOB_PARAM( void ) : uiId( 0xffffffff ), bFind( false ) { return; }

	unsigned int				uiId;
	bool						bFind;
};

class CDboTSEMMob_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMMob_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_MOB &&
		 ((sMOB_PARAM*)pParam)->uiId == ((CDboTSClickMob*)pEntity)->GetMobGroupIdx() )
	{
		((sMOB_PARAM*)pParam)->bFind = true;
	}

	return 0;
}


/** 
	Event mapper mob
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMMob, CNtlTSEvtMapper )


CDboTSEMMob::CDboTSEMMob( void )
{
}

CDboTSEMMob::~CDboTSEMMob( void )
{
}

bool CDboTSEMMob::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pParameter );

	std::map< unsigned int, unsigned int > mapMobGroupList;

	CMobTable* pMobTbl = (CMobTable*) pTblList;

	CTable::TABLEIT itBegin = pMobTbl->Begin();
	CTable::TABLEIT itEnd = pMobTbl->End();

	// Mob table을 순회하여 Mob group을 추출한다
	for ( ; itBegin != itEnd; ++itBegin )
	{
		sMOB_TBLDAT* pMobData = (sMOB_TBLDAT*)itBegin->second;

		mapMobGroupList[pMobData->dwMobGroup] = pMobData->dwMobGroup;
	}

	vecdef_TID_LIST defTIDList;

	sMOB_PARAM sParam;
	CDboTSEMMob_Recv clRecv;

	unsigned int uiMobGroupID;
	std::map< unsigned int, unsigned int >::iterator itMobGroupID = mapMobGroupList.begin();
	for ( ; itMobGroupID != mapMobGroupList.end(); ++itMobGroupID )
	{
		uiMobGroupID = itMobGroupID->first;

		defTIDList.clear();

		// Trigger system script들을 순회하면서 해당 Mob group id로 시작할 수 있는
		// trigger를 찾는다
		CNtlTSTrigger* pTrig;
		CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
		CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

		for ( ; citTSBegin != citTSEnd; ++citTSBegin )
		{
			pTrig = citTSBegin->second;

			sParam.uiId = uiMobGroupID;
			sParam.bFind = false;

			pTrig->SearchTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam );

			if ( sParam.bFind )
			{
				defTIDList.push_back( pTrig->GetID() );
			}
		}

		mapdef_MAPPER::iterator itFind = m_defMapper.find( uiMobGroupID );
		if  ( itFind == m_defMapper.end() )
		{
			m_defMapper[uiMobGroupID][strKey] = defTIDList;
		}
		else
		{
			if ( m_defMapper[uiMobGroupID].find( strKey ) == m_defMapper[uiMobGroupID].end() )
			{
				m_defMapper[uiMobGroupID][strKey] = defTIDList;
			}
			else
			{
				size_t nSize = defTIDList.size();
				for ( size_t i = 0; i < nSize; ++i )
				{
					m_defMapper[uiMobGroupID][strKey].push_back( defTIDList[i] );
				}
			}
		}
	}

	return true;
}

bool CDboTSEMMob::DelBuildData( const std::string& strKey )
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

