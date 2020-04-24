#include "precomp_trigger.h"
#include "DboTSEMItem.h"
#include "ItemTable.h"
#include "DboTSCoreDefine.h"
#include "DboTSItemUse.h"
#include "DboTSItemGet.h"
#include "DboTSItemEquip.h"
#include "DboTSScoutUse.h"


/** 
	Event mapper item receiver
*/

struct sITEM_PARAM
{
	sITEM_PARAM( void ) : bFind( false ), uiItemIdx( 0xffffffff ) { return; }

	bool						bFind;
	unsigned int				uiItemIdx;
};

class CDboTSEMItem_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMItem_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_ITEM_USE )
	{
		((sITEM_PARAM*)pParam)->bFind = true;

		CDboTSItemUse* pItemUse = (CDboTSItemUse*) pEntity;
		((sITEM_PARAM*)pParam)->uiItemIdx = pItemUse->GetItemIdx();
	}

	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_ITEM_GET )
	{
		((sITEM_PARAM*)pParam)->bFind = true;

		CDboTSItemGet* pItemGet = (CDboTSItemGet*) pEntity;
		((sITEM_PARAM*)pParam)->uiItemIdx = pItemGet->GetItemIdx();
	}

	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_ITEM_EQUIP )
	{
		((sITEM_PARAM*)pParam)->bFind = true;

		CDboTSItemEquip* pItemEquip = (CDboTSItemEquip*) pEntity;
		((sITEM_PARAM*)pParam)->uiItemIdx = pItemEquip->GetItemIdx();
	}

	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_SCOUT_USE )
	{
		((sITEM_PARAM*)pParam)->bFind = true;

		CDboTSScoutUse* pScoutUse = (CDboTSScoutUse*) pEntity;
		((sITEM_PARAM*)pParam)->uiItemIdx = pScoutUse->GetItemIdx();
	}

	return 0;
}


/** 
	Event mapper item
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMItem, CNtlTSEvtMapper )


CDboTSEMItem::CDboTSEMItem( void )
{
}

CDboTSEMItem::~CDboTSEMItem( void )
{
}

bool CDboTSEMItem::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pTblList );
	UNREFERENCED_PARAMETER( pParameter );

	sITEM_PARAM sParam;
	CDboTSEMItem_Recv clRecv;

	// Trigger system script들을 순회하면서 해당 Item id로 시작할 수 있는
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
			if  ( m_defMapper.find( sParam.uiItemIdx ) == m_defMapper.end() )
			{
				m_defMapper[sParam.uiItemIdx][strKey] = vecdef_TID_LIST();

				m_defMapper[sParam.uiItemIdx][strKey].push_back( pTrig->GetID() );
			}
			else
			{
				if ( m_defMapper[sParam.uiItemIdx].find( strKey ) == m_defMapper[sParam.uiItemIdx].end() )
				{
					m_defMapper[sParam.uiItemIdx][strKey] = vecdef_TID_LIST();

					m_defMapper[sParam.uiItemIdx][strKey].push_back( pTrig->GetID() );
				}
				else
				{
					m_defMapper[sParam.uiItemIdx][strKey].push_back( pTrig->GetID() );
				}
			}
		}
	}

	return true;
}

bool CDboTSEMItem::DelBuildData( const std::string& strKey )
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

