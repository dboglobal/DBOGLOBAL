#include "precomp_trigger.h"
#include "DboTSEMNPC.h"
#include "NPCTable.h"
#include "DboTSCoreDefine.h"
#include "DboTSClickNPC.h"


/** 
	Event mapper npc receiver
*/

struct sNPC_PARAM
{
	sNPC_PARAM( void ) : uiId( 0xffffffff ), bFind( false ) { return; }

	unsigned int				uiId;
	bool						bFind;
};

class CDboTSEMNPC_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMNPC_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_NPC &&
		 ((sNPC_PARAM*)pParam)->uiId == ((CDboTSClickNPC*)pEntity)->GetNPCIdx() )
	{
		((sNPC_PARAM*)pParam)->bFind = true;
	}

	return 0;
}


/** 
	Event mapper npc
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMNPC, CNtlTSEvtMapper )


CDboTSEMNPC::CDboTSEMNPC( void )
{
}

CDboTSEMNPC::~CDboTSEMNPC( void )
{
}

bool CDboTSEMNPC::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pParameter );

	CNPCTable* pNPCTbl = (CNPCTable*) pTblList;

	CTable::TABLEIT itBegin = pNPCTbl->Begin();
	CTable::TABLEIT itEnd = pNPCTbl->End();

	vecdef_TID_LIST defTIDList;

	sNPC_PARAM sParam;
	CDboTSEMNPC_Recv clRecv;

	// NPC table을 순회한다
	for ( ; itBegin != itEnd; ++itBegin )
	{
		sNPC_TBLDAT* pNPCData = (sNPC_TBLDAT*)itBegin->second;

		// Trigger flag가 켜져 있는 것만을 대상으로 동작한다
		if ( pNPCData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_QUEST_GRANTER )
		{
			defTIDList.clear();

			// Trigger system script들을 순회하면서 해당 NPC id로 시작할 수 있는
			// trigger를 찾는다
			CNtlTSTrigger* pTrig;
			CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
			CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

			for ( ; citTSBegin != citTSEnd; ++citTSBegin )
			{
				pTrig = citTSBegin->second;

				sParam.uiId = pNPCData->tblidx;
				sParam.bFind = false;

				pTrig->SearchTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam );

				if ( sParam.bFind )
				{
					defTIDList.push_back( pTrig->GetID() );
				}
			}

			mapdef_MAPPER::iterator itFind = m_defMapper.find( pNPCData->tblidx );
			if  ( itFind == m_defMapper.end() )
			{
				m_defMapper[pNPCData->tblidx][strKey] = defTIDList;
			}
			else
			{
				if ( m_defMapper[pNPCData->tblidx].find( strKey ) == m_defMapper[pNPCData->tblidx].end() )
				{
					m_defMapper[pNPCData->tblidx][strKey] = defTIDList;
				}
				else
				{
					size_t nSize = defTIDList.size();
					for ( size_t i = 0; i < nSize; ++i )
					{
						m_defMapper[pNPCData->tblidx][strKey].push_back( defTIDList[i] );
					}
				}
			}
		}
	}

	return true;
}

bool CDboTSEMNPC::DelBuildData( const std::string& strKey )
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

