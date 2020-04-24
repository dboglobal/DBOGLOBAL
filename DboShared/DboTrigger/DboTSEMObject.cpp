#include "precomp_trigger.h"
#include "DboTSEMObject.h"
#include "ObjectTable.h"
#include "DboTSCoreDefine.h"
#include "DboTSClickObject.h"
#include "DboTSColObject.h"


/** 
	Event mapper object receiver
*/


struct sOBJECT_PARAM
{
	sOBJECT_PARAM( void ) : uiWorldIdx( 0xffffffff ), uiId( 0xffffffff ), bFind( false ) { return; }

	unsigned int				uiWorldIdx;
	unsigned int				uiId;
	bool						bFind;
};

class CDboTSEMObject_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMObject_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_OBJECT &&
		 ((CDboTSClickObject*)pEntity)->GetWorldIdx() != 0xffffffff &&
		 ((CDboTSClickObject*)pEntity)->GetWorldIdx() == ((sOBJECT_PARAM*)pParam)->uiWorldIdx &&
		 ((CDboTSClickObject*)pEntity)->HasObjectIdx( ((sOBJECT_PARAM*)pParam)->uiId ) )
	{
		((sOBJECT_PARAM*)pParam)->bFind = true;
	}

	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_COL_OBJECT &&
		 ((CDboTSClickObject*)pEntity)->GetWorldIdx() != 0xffffffff &&
		 ((CDboTSClickObject*)pEntity)->GetWorldIdx() == ((sOBJECT_PARAM*)pParam)->uiWorldIdx &&
		 ((CDboTSColObject*)pEntity)->HasObjectIdx( ((sOBJECT_PARAM*)pParam)->uiId ) )
	{
		((sOBJECT_PARAM*)pParam)->bFind = true;
	}

	return 0;
}


/** 
	Event mapper object
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMObject, CNtlTSEvtMapper )


CDboTSEMObject::CDboTSEMObject( void )
{
}

CDboTSEMObject::~CDboTSEMObject( void )
{
}

bool CDboTSEMObject::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	CObjectTable* pObjTbl = (CObjectTable*) pTblList;

	unsigned int uiWorldIdx = pParameter ? *(unsigned int*)pParameter : 0xffffffff;

	CTable::TABLEIT itBegin = pObjTbl->Begin();
	CTable::TABLEIT itEnd = pObjTbl->End();

	vecdef_TID_LIST defTIDList;

	sOBJECT_PARAM sParam;
	CDboTSEMObject_Recv clRecv;

	// NPC table을 순회한다
	for ( ; itBegin != itEnd; ++itBegin )
	{
		defTIDList.clear();

		sOBJECT_TBLDAT* pObjData = (sOBJECT_TBLDAT*)itBegin->second;

		// Trigger system script들을 순회하면서 해당 Object id로 시작할 수 있는
		// trigger를 찾는다
		CNtlTSTrigger* pTrig;
		CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
		CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

		for ( ; citTSBegin != citTSEnd; ++citTSBegin )
		{
			pTrig = citTSBegin->second;

			sParam.uiWorldIdx = uiWorldIdx;
			sParam.uiId = pObjData->tblidx;
			sParam.bFind = false;

			pTrig->SearchTarget( NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam );

			if ( sParam.bFind )
			{
				defTIDList.push_back( pTrig->GetID() );
			}
		}

		mapdef_MAPPER::iterator itFind = m_defMapper.find( pObjData->tblidx );
		if  ( itFind == m_defMapper.end() )
		{
			m_defMapper[pObjData->tblidx][strKey] = defTIDList;
		}
		else
		{
			if ( m_defMapper[pObjData->tblidx].find( strKey ) == m_defMapper[pObjData->tblidx].end() )
			{
				m_defMapper[pObjData->tblidx][strKey] = defTIDList;
			}
			else
			{
				size_t nSize = defTIDList.size();
				for ( size_t i = 0; i < nSize; ++i )
				{
					m_defMapper[pObjData->tblidx][strKey].push_back( defTIDList[i] );
				}
			}
		}
	}

	return true;
}

bool CDboTSEMObject::DelBuildData( const std::string& strKey )
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
