#include "precomp_trigger.h"
#include "DboTSEMSvrEvt.h"
#include "DboTSCoreDefine.h"
#include "DboTSRcvSvrEvt.h"


/** 
	Event mapper server event
*/

struct sEVT_PARAM
{
	sEVT_PARAM( void ) : bFind( false ), uiEvtID( 0xffffffff ) { return; }

	bool						bFind;
	unsigned int				uiEvtID;
};

class CDboTSEMSvrEvt_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search( CNtlTSEntity* pEntity, void* pParam );
	NTL_TSRESULT				Run( CNtlTSEntity*, void* ) { return 0; }
};

NTL_TSRESULT CDboTSEMSvrEvt_Recv::Search( CNtlTSEntity* pEntity, void* pParam )
{
	if ( pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_RCV_SVR_EVT )
	{
		((sEVT_PARAM*)pParam)->bFind = true;

		CDboTSRcvSvrEvt* pSvrEvt = (CDboTSRcvSvrEvt*) pEntity;
		((sEVT_PARAM*)pParam)->uiEvtID = pSvrEvt->GetEvtID();
	}

	return 0;
}


/** 
	Event mapper server event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSEMSvrEvt, CNtlTSEvtMapper )


CDboTSEMSvrEvt::CDboTSEMSvrEvt( void )
{
}

CDboTSEMSvrEvt::~CDboTSEMSvrEvt( void )
{
}

bool CDboTSEMSvrEvt::AddBuildData( const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter )
{
	UNREFERENCED_PARAMETER( pTblList );
	UNREFERENCED_PARAMETER( pParameter );

	sEVT_PARAM sParam;
	CDboTSEMSvrEvt_Recv clRecv;

	// Trigger that can be started with the corresponding server event id while traversing the system scripts
	// Find the trigger

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
			if  ( m_defMapper.find( sParam.uiEvtID ) == m_defMapper.end() )
			{
				m_defMapper[sParam.uiEvtID][strKey] = vecdef_TID_LIST();

				m_defMapper[sParam.uiEvtID][strKey].push_back( pTrig->GetID() );
			}
			else
			{
				if ( m_defMapper[sParam.uiEvtID].find( strKey ) == m_defMapper[sParam.uiEvtID].end() )
				{
					m_defMapper[sParam.uiEvtID][strKey] = vecdef_TID_LIST();

					m_defMapper[sParam.uiEvtID][strKey].push_back( pTrig->GetID() );
				}
				else
				{
					m_defMapper[sParam.uiEvtID][strKey].push_back( pTrig->GetID() );
				}
			}
		}
	}

	return true;
}

bool CDboTSEMSvrEvt::DelBuildData( const std::string& strKey )
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

