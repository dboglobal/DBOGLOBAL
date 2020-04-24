#include "precomp_trigger.h"
#include "DboTSTCtrl.h"
#include "DboTSTRecv.h"
#include "DboTSTAgency.h"
#include "DboTSContStart.h"
#include "DboTSContGCond.h"
#include "DboTSContEnd.h"
#include "DboTSContGAct.h"
#include "DboTSActETimerS.h"
#include "DboTSActETimerE.h"


/**
	Quest controller
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSTCtrl, CNtlTSController )


CDboTSTCtrl::CDboTSTCtrl( void )
{
	m_pParent = 0;

	m_uiCurState = 0;
	m_pCurTSP = 0;
	memset( &m_sSSM, 0xff, sizeof(m_sSSM) );
	m_uiTWPeriod = 0;
	m_uiTWStart = 0;

	m_byEventType = 0xff;
	m_uiEventData = 0xffffffff;

	m_eEvtGenType = eEVENT_GEN_TYPE_INVALID;
	m_uiEvtGenId = 0xffffffff;
}

CDboTSTCtrl::~CDboTSTCtrl( void )
{
}

void CDboTSTCtrl::Update( void )
{
	SetError();
	CNtlTSLog::Log( "Not implemented. [%s]", TS_CODE_TRACE() );
}

bool CDboTSTCtrl::CanProgressClickObject( unsigned int uiObjId )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
			m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
			m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_OBJECT );
	SetEventGenId( uiObjId );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressCollisionObject( unsigned int uiObjId )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
			m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
			m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_COL_OBJECT );
	SetEventGenId( uiObjId );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressClickNPC( unsigned int uiNPCId )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		    m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
			m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_NPC );
	SetEventGenId( uiNPCId );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressItemUse( unsigned int uiItemIdx, vecdef_ItemTargetList* pdefItemTargetList )
{
	if ( NULL == pdefItemTargetList ) return false;
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenItemTargetInfo( pdefItemTargetList );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressItemGet( unsigned int uiItemIdx )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_GET );
	SetEventGenId( uiItemIdx );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressItemEquip( unsigned int uiItemIdx )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_EQUIP );
	SetEventGenId( uiItemIdx );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressScoutUse( unsigned int uiItemIdx, SScouterUseTargetInfo* pTargetInfo )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_SCOUT_USE );
	SetEventGenId( uiItemIdx );
	SetEventGenScouterUseTargetInfo( pTargetInfo );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressRcvSvrEvt( unsigned int uiEvtID )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_RCV_SVR_EVT );
	SetEventGenId( uiEvtID );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressSkillUse( unsigned int uiSkillIdx, SSkillUseInfo* psSkillUseInfo )
{
	if ( NULL == psSkillUseInfo ) return false;
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_SKILL_USE );
	SetEventGenId( uiSkillIdx );
	SetEventGenSkillUseInfo( psSkillUseInfo );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressColRgn( sCOL_RGN_DATA* pColRgnData )
{
	if ( NULL == pColRgnData ) return false;
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_COL_REGION );
	SetEventGenColRgnInfo( pColRgnData );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressRB( unsigned int uiRBTblIdx, SRB_DATA* psRBData )
{
	if ( NULL == psRBData ) return false;
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_RB );
	SetEventGenId( uiRBTblIdx );
	SetEventGenRBData( psRBData );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressClickMob( unsigned int uiMobGroupID, SMobTargetInfo* pMobTargetData )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_CLICK_MOB );
	SetEventGenId( uiMobGroupID );
	SetEventGenMobTargetData( pMobTargetData );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressBindStone( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_BIND_STONE );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressSearchQuest( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_SEARCH_QUEST );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressItemUpgrade( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		  m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		  m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_ITEM_UPGRADE );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressTeleport( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_TELEPORT );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressBudokai( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_BUDOKAI );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressSlotMachine( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_SLOTMACHINE );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressHoipoiMix( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_HOIPOIMIX );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressPrivateShop( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_PRIVATESHOP );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressFreeBattle( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_FREEBATTLE );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressItemIdentity( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_ITEMIDENTITY );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressUseMail( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_USEMAIL );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressParty( void )
{
	if ( IsCSComunication() ) return false;
	if ( IsClientWait() ) return false;
	if ( IsSvrWait() ) return false;
	if ( IsFailed() ) return false;
	if ( IsError() ) return false;
	if ( IsExitState() ) return false;
	if ( IsTimeWait() ) return false;
	if ( IsSvrComAfterClientWait() ) return false;
	if ( !IsContSyncQueueEmpty() ) return false;
	if ( NULL == m_pCurTSP ) return false;
	if ( !( m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		 m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD ) ) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl( this );
	sTParam.SetAgency( GetParent() );

	SetEventGenType( eEVENT_GEN_TYPE_PARTY );

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget( NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam );

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressFLink(void)
{
	if (IsCSComunication()) return false;
	if (IsClientWait()) return false;
	if (IsSvrWait()) return false;
	if (IsFailed()) return false;
	if (IsError()) return false;
	if (IsExitState()) return false;
	if (IsTimeWait()) return false;
	if (IsSvrComAfterClientWait()) return false;
	if (!IsContSyncQueueEmpty()) return false;
	if (NULL == m_pCurTSP) return false;
	if (!(m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD)) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl(this);
	sTParam.SetAgency(GetParent());

	SetEventGenType(eEVENT_GEN_TYPE_FLINK);

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget(NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam);

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressClickSideIcon(void)
{
	if (IsCSComunication()) return false;
	if (IsClientWait()) return false;
	if (IsSvrWait()) return false;
	if (IsFailed()) return false;
	if (IsError()) return false;
	if (IsExitState()) return false;
	if (IsTimeWait()) return false;
	if (IsSvrComAfterClientWait()) return false;
	if (!IsContSyncQueueEmpty()) return false;
	if (NULL == m_pCurTSP) return false;
	if (!(m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD)) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl(this);
	sTParam.SetAgency(GetParent());

	SetEventGenType(eEVENT_GEN_TYPE_CLICK_SIDEICON);

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget(NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam);

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressLevelCheck(void)
{
	if (IsCSComunication()) return false;
	if (IsClientWait()) return false;
	if (IsSvrWait()) return false;
	if (IsFailed()) return false;
	if (IsError()) return false;
	if (IsExitState()) return false;
	if (IsTimeWait()) return false;
	if (IsSvrComAfterClientWait()) return false;
	if (!IsContSyncQueueEmpty()) return false;
	if (NULL == m_pCurTSP) return false;
	if (!(m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD)) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl(this);
	sTParam.SetAgency(GetParent());

	SetEventGenType(eEVENT_GEN_TYPE_LEVELCHECK);

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget(NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam);

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressQuest(void)
{
	if (IsCSComunication()) return false;
	if (IsClientWait()) return false;
	if (IsSvrWait()) return false;
	if (IsFailed()) return false;
	if (IsError()) return false;
	if (IsExitState()) return false;
	if (IsTimeWait()) return false;
	if (IsSvrComAfterClientWait()) return false;
	if (!IsContSyncQueueEmpty()) return false;
	if (NULL == m_pCurTSP) return false;
	if (!(m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD)) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl(this);
	sTParam.SetAgency(GetParent());

	SetEventGenType(eEVENT_GEN_TYPE_QUEST);

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget(NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam);

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}

bool CDboTSTCtrl::CanProgressDialogOpen(void)
{
	if (IsCSComunication()) return false;
	if (IsClientWait()) return false;
	if (IsSvrWait()) return false;
	if (IsFailed()) return false;
	if (IsError()) return false;
	if (IsExitState()) return false;
	if (IsTimeWait()) return false;
	if (IsSvrComAfterClientWait()) return false;
	if (!IsContSyncQueueEmpty()) return false;
	if (NULL == m_pCurTSP) return false;
	if (!(m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_START ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_GCOND ||
		m_pCurTSP->GetEntityType() == DBO_CONT_TYPE_ID_CONT_REWARD)) return false;

	sTRUN_PARAM sTParam;
	sTParam.SetControl(this);
	sTParam.SetAgency(GetParent());

	SetEventGenType(eEVENT_GEN_TYPE_DIALOGOPEN);

	NTL_TSRESULT tsResult = GetTrigger()->RunTarget(NTL_TS_MAIN_GROUP_ID, m_pCurTSP->GetID(), GetParent()->GetRecv(), &sTParam);

	ClearAllEventGenInfo();

	return NTL_TSRESULT_TYPE_SUCCESS == tsResult ? true : false;
}


void CDboTSTCtrl::SetParent( CDboTSTAgency* pParent )
{
	m_pParent = pParent;
}

void CDboTSTCtrl::SetCurState( unsigned int uiState )
{
	if ( (m_uiCurState & eTS_PROG_STATE_PROGRESS_FAILED) && !(uiState & eTS_PROG_STATE_PROGRESS_FAILED) ) SetFailed( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_PROGRESS_FAILED) && (uiState & eTS_PROG_STATE_PROGRESS_FAILED) ) SetFailed( true );

	if ( (m_uiCurState & eTS_PROG_STATE_ERROR_BLOCK) && !(uiState & eTS_PROG_STATE_ERROR_BLOCK) ) SetError( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_ERROR_BLOCK) && (uiState & eTS_PROG_STATE_ERROR_BLOCK) ) SetError( true );

	if ( (m_uiCurState & eTS_PROG_STATE_CLEARED) && !(uiState & eTS_PROG_STATE_CLEARED) ) SetCleared( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_CLEARED) && (uiState & eTS_PROG_STATE_CLEARED) ) SetCleared( true );

	if ( (m_uiCurState & eTS_PROG_STATE_EXIT) && !(uiState & eTS_PROG_STATE_EXIT) ) SetExitState( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_EXIT) && (uiState & eTS_PROG_STATE_EXIT) ) SetExitState( true );

	if ( (m_uiCurState & eTS_PROG_STATE_CS_COM_WAIT) && !(uiState & eTS_PROG_STATE_CS_COM_WAIT) ) SetCSComunication( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_CS_COM_WAIT) && (uiState & eTS_PROG_STATE_CS_COM_WAIT) ) SetCSComunication( true );

	if ( (m_uiCurState & eTS_PROG_STATE_CLIENT_COM_WAIT) && !(uiState & eTS_PROG_STATE_CLIENT_COM_WAIT) ) SetClientWait( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_CLIENT_COM_WAIT) && (uiState & eTS_PROG_STATE_CLIENT_COM_WAIT) ) SetClientWait( true );

	if ( (m_uiCurState & eTS_PROG_STATE_SVR_COM_WAIT) && !(uiState & eTS_PROG_STATE_SVR_COM_WAIT) ) SetSvrWait( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_SVR_COM_WAIT) && (uiState & eTS_PROG_STATE_SVR_COM_WAIT) ) SetSvrWait( true );

	if ( (m_uiCurState & eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT) && !(uiState & eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT) ) SetSvrComAfterClientWait( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT) && (uiState & eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT) ) SetSvrComAfterClientWait( true );

	if ( (m_uiCurState & eTS_PROG_STATE_ESCORT) && !(uiState & eTS_PROG_STATE_ESCORT) ) SetEscort( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_ESCORT) && (uiState & eTS_PROG_STATE_ESCORT) ) SetEscort( true );

	if ( (m_uiCurState & eTS_PROG_STATE_SVR_EVT_WORLD_RANGE) && !(uiState & eTS_PROG_STATE_SVR_EVT_WORLD_RANGE) ) SetSvrEvtWorldRange( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_SVR_EVT_WORLD_RANGE) && (uiState & eTS_PROG_STATE_SVR_EVT_WORLD_RANGE) ) SetSvrEvtWorldRange( true );

	if ( (m_uiCurState & eTS_PROG_STATE_WPS_MODE) && !(uiState & eTS_PROG_STATE_WPS_MODE) ) SetWPSMode( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_WPS_MODE) && (uiState & eTS_PROG_STATE_WPS_MODE) ) SetWPSMode( true );

	if ( (m_uiCurState & eTS_PROG_STATE_USER_OUT_FAILED_WPS) && !(uiState & eTS_PROG_STATE_USER_OUT_FAILED_WPS) ) SetUserOutFailedWPS( false );
	else if ( !(m_uiCurState & eTS_PROG_STATE_USER_OUT_FAILED_WPS) && (uiState & eTS_PROG_STATE_USER_OUT_FAILED_WPS) ) SetUserOutFailedWPS( true );
}

void CDboTSTCtrl::AttachExceptionTimer( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, unsigned int uiCurTime, unsigned int uiRemainTime )
{
	CNtlTSCont* pCont = GetTrigger()->GetGroup( NTL_TS_MAIN_GROUP_ID )->GetChildCont( tcId );
	if ( 0 == pCont )
	{
		CNtlTSLog::Log( "Can not attach the exception timer. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), tcId, taId, TS_CODE_TRACE() );
		return;
	}
	CNtlTSEntity* pEntity = pCont->FindChildEntity( taId );
	if ( 0 == pEntity )
	{
		CNtlTSLog::Log( "Can not attach the exception timer. Info[%d, %d, %d]. [%s]", GetTrigger()->GetID(), tcId, taId, TS_CODE_TRACE() );
		return;
	}
	if ( !pEntity->IsDerivedClass( "CDboTSActETimerS" ) )
	{
		CNtlTSLog::Log( "Attach delete only the exception timer. Info[%d, %d, %d, %s]. [%s]", GetTrigger()->GetID(), tcId, taId, pEntity->GetClassName(), TS_CODE_TRACE() );
		return;
	}

	sEXCEPTION_TIMER stExceptTimer;
	stExceptTimer.tcId = tcId;
	stExceptTimer.taId = taId;
	stExceptTimer.uiCurTime = uiCurTime;
	stExceptTimer.uiRemainTime = uiRemainTime;
	stExceptTimer.pActExceptTimer = (CDboTSActETimerS*)pEntity;

	unsigned int uiKey = MAKE_EXCEPTION_TIMER_KEY( tcId, taId );
	m_defExceptTimerSlot[uiKey] = stExceptTimer;
}

void CDboTSTCtrl::DetachExceptionTimer( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	unsigned int uiKey = MAKE_EXCEPTION_TIMER_KEY( tcId, taId );

	mapdef_EXCEPT_TIMER_SLOT::iterator it = m_defExceptTimerSlot.find( uiKey );
	if ( it != m_defExceptTimerSlot.end() )
	{
		m_defExceptTimerSlot.erase( it );
	}
}

void CDboTSTCtrl::UpdateExceptionTimer( void )
{
	if ( IsError() ) return;
	if ( IsFailed() ) return;

	__int64 nRemainTime;

	mapdef_EXCEPT_TIMER_SLOT::iterator it = m_defExceptTimerSlot.begin();
	for ( ; it != m_defExceptTimerSlot.end(); ++it )
	{
		sEXCEPTION_TIMER& stTimer = it->second;

		unsigned int uiOldTime = stTimer.uiCurTime;
		stTimer.uiCurTime = GetCurTime();

		nRemainTime = stTimer.uiRemainTime;
		nRemainTime -= (stTimer.uiCurTime - uiOldTime);

		// 시간 제한은 한번만 호출되도록 한다
		if ( stTimer.pActExceptTimer->GetTimerSort() == eEXCEPT_TIMER_SORT_LIMIT_TIMER )
		{
			if ( 0xffffffff == stTimer.uiRemainTime ) continue;

			if ( nRemainTime <= 0 )
			{
				if ( IfInterestingDoExceptionGroup( stTimer.pActExceptTimer->GetTGId() ) )
				{
					AttachUpdateExceptionGroup( stTimer.pActExceptTimer->GetTGId() );
				}

				stTimer.uiRemainTime = 0xffffffff;
			}
			else
			{
				stTimer.uiRemainTime -= (stTimer.uiCurTime - uiOldTime);
			}
		}
		// 시간 제한 타이머를 제외한 타이머는 주기적으로 호출된다
		else
		{
			if ( nRemainTime <= 0 )
			{
				if ( IfInterestingDoExceptionGroup( stTimer.pActExceptTimer->GetTGId() ) )
				{
					AttachUpdateExceptionGroup( stTimer.pActExceptTimer->GetTGId() );
				}

				stTimer.uiRemainTime = stTimer.pActExceptTimer->GetTime();
			}
			else
			{
				stTimer.uiRemainTime -= (stTimer.uiCurTime - uiOldTime);
			}
		}
	}

	UpdateExceptionGroup();
}

CDboTSTCtrl::sEXCEPTION_TIMER* CDboTSTCtrl::GetTimeLimitInfo( void )
{
	mapdef_EXCEPT_TIMER_SLOT::iterator it = m_defExceptTimerSlot.begin();
	for ( ; it != m_defExceptTimerSlot.end(); ++it )
	{
		if ( it->second.pActExceptTimer->GetTimerSort() == eEXCEPT_TIMER_SORT_LIMIT_TIMER )
		{
			return &it->second;
		}
	}

	return 0;
}

void CDboTSTCtrl::SetSSM( eSSM_ID eSSMId, unsigned int uiVal )
{
	m_sSSM.auiSSM[eSSMId] = uiVal;
}

void CDboTSTCtrl::AddContSyncQueue( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	WORD wSyncKey = MAKE_EXCEPTION_TIMER_KEY( tcId, taId );
	m_sSyncQueue.defContSyncQueue[wSyncKey] = wSyncKey;
}

void CDboTSTCtrl::DelContSyncQueue( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId )
{
	if ( IsContSyncQueueEmpty() ) return;

	WORD wSyncKey = MAKE_EXCEPTION_TIMER_KEY( tcId, taId );

	hashdef_ENTITY_LIST::iterator it = m_sSyncQueue.defContSyncQueue.find( wSyncKey );
	if ( it != m_sSyncQueue.defContSyncQueue.end() ) m_sSyncQueue.defContSyncQueue.erase( it );
}

void CDboTSTCtrl::StartTimeWait( unsigned int uiTime )
{
	if ( !IsTimeWait() )
	{
		m_uiTWPeriod = uiTime;
		m_uiTWStart = GetCurTime();
	}
	else
	{
		if ( (int)(uiTime - m_uiTWPeriod) > 0 )
		{
			m_uiTWPeriod = uiTime;
		}
	}
}

unsigned char CDboTSTCtrl::GetEventType( void )
{
	return m_byEventType;
}

unsigned int CDboTSTCtrl::GetEventData( void )
{
	return m_uiEventData;
}

void CDboTSTCtrl::SetEventData( unsigned char byEventType, unsigned int uiEventData )
{
	m_byEventType = byEventType;
	m_uiEventData = uiEventData;
}

void CDboTSTCtrl::InitEventData( void )
{
	m_byEventType = 0xff;
	m_uiEventData = 0xffffffff;
}

void CDboTSTCtrl::SetEventGenType( eEVENT_GEN_TYPE eEvtGenType )
{
	m_eEvtGenType = eEvtGenType;
}

void CDboTSTCtrl::SetEventGenId( unsigned int uiEvtGenId )
{
	m_uiEvtGenId = uiEvtGenId;
}

void CDboTSTCtrl::SetEventGenScouterUseTargetInfo( SScouterUseTargetInfo* psTargetInfo )
{
	if ( psTargetInfo )
	{
		m_sScouterUseTargetInfo = *psTargetInfo;
	}
	else
	{
		m_sScouterUseTargetInfo.Clear();
	}
}

void CDboTSTCtrl::SetEventGenSkillUseInfo( SSkillUseInfo* psSkillUseInfo )
{
	if ( psSkillUseInfo )
	{
		m_sSkillUserInfo = *psSkillUseInfo;
	}
	else
	{
		m_sSkillUserInfo.byRpBonusType = 0xff;
		m_sSkillUserInfo.defSkillTargetList.clear();
	}
}

void CDboTSTCtrl::SetEventGenItemTargetInfo( vecdef_ItemTargetList* pdefItemTargetList )
{
	if ( pdefItemTargetList )
	{
		m_defItemTargetList = *pdefItemTargetList;
	}
	else
	{
		m_defItemTargetList.clear();
	}
}

void CDboTSTCtrl::SetEventGenColRgnInfo( sCOL_RGN_DATA* pColRgnData )
{
	if ( pColRgnData )
	{
		m_sColRgnData = *pColRgnData;
	}
	else
	{
		m_sColRgnData.Clear();
	}
}

void CDboTSTCtrl::SetEventGenRBData( SRB_DATA* pRBData )
{
	if ( pRBData )
	{
		m_sRBData = *pRBData;
	}
	else
	{
		m_sRBData.Clear();
	}
}

void CDboTSTCtrl::SetEventGenMobTargetData( SMobTargetInfo* pMobTargetData )
{
	if ( pMobTargetData )
	{
		m_sMobTargetData = *pMobTargetData;
	}
	else
	{
		m_sMobTargetData.Clear();
	}
}

void CDboTSTCtrl::ClearAllEventGenInfo( void )
{
	m_eEvtGenType = eEVENT_GEN_TYPE_INVALID;
	m_uiEvtGenId = 0xffffffff;
	m_sScouterUseTargetInfo.Clear();
	m_sSkillUserInfo.byRpBonusType = 0xff;
	m_sSkillUserInfo.defSkillTargetList.clear();
	m_defItemTargetList.clear();
	m_sColRgnData.Clear();
	m_sRBData.Clear();
}

unsigned int CDboTSTCtrl::GetCurTime( void )
{
	return GetTickCount();
}

void CDboTSTCtrl::AttachUpdateExceptionGroup( NTL_TS_TG_ID tgId )
{
	// 기존에 등록된 그룹이 존재하면 해당 그룹이 종료할 때 까지 기다려야 한다
	if ( m_defGroupUpdateList.end() != m_defGroupUpdateList.find( tgId ) )
	{
		return;
	}

	sGROPU_UPDATE_INFO sInfo;

	sInfo.pCurGroup = GetTrigger()->GetGroup( tgId );
	if ( 0 == sInfo.pCurGroup )
	{
		SetError();
		CNtlTSLog::Log( "Can not find TG id. Info[%d, %d], [%s]", GetTrigger()->GetID(), tgId, TS_CODE_TRACE() );
		return;
	}

	sInfo.pCurCont = sInfo.pCurGroup->GetChildCont( START_CONTAINER_ID );
	if ( 0 == sInfo.pCurCont )
	{
		SetError();
		CNtlTSLog::Log( "Can not find TC id. Info[%d, %d, %d], [%s]", GetTrigger()->GetID(), tgId, START_CONTAINER_ID, TS_CODE_TRACE() );
		return;
	}

	sInfo.nActionRef = 0;

	m_defGroupUpdateList[tgId] = sInfo;
}

void CDboTSTCtrl::UpdateTSStep( void )
{
	SetError();
	CNtlTSLog::Log( "Not implemented. [%s]", TS_CODE_TRACE() );
}

void CDboTSTCtrl::UpdateExceptionGroup( void )
{
	sGROPU_UPDATE_INFO* pUpdateInfo;

	for ( mapdef_GROUP_UPDATE_INFO::iterator it = m_defGroupUpdateList.begin(); it != m_defGroupUpdateList.end(); )
	{
		pUpdateInfo = &it->second;

		// 현재 진행중인 컨테이너가 존재하지 않는 경우
		// Update list 에서 제거한다
		if ( NULL == pUpdateInfo->pCurCont )
		{
			AutoDetachedUpdateExceptionGroup( it->second.pCurGroup );

			it = m_defGroupUpdateList.erase( it );
			continue;
		}
		else
		{
			// 현재 Exception group에 대한 기존 Action이 동작중인 경우는
			// 응답이 올때까지 대기 한다
			if ( pUpdateInfo->nActionRef > 0 )
			{
				++it;
				continue;
			}

			sTRUN_PARAM sTParam;
			sTParam.SetControl( this );
			sTParam.SetAgency( GetParent() );

			switch ( pUpdateInfo->pCurCont->GetEntityType() )
			{
			case DBO_CONT_TYPE_ID_CONT_START:
				{
					CDboTSContStart* pStart = (CDboTSContStart*)pUpdateInfo->pCurCont;
					NTL_TSRESULT tsResult = pStart->RunAll( GetParent()->GetRecv(), &sTParam );

					if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
					{
						SetError();
						CNtlTSLog::Log( "TS exception progress is failed. Info[%d,%d,%d]. [%s]", GetTrigger()->GetID(), pUpdateInfo->pCurGroup->GetID(), pStart->GetID(), TS_CODE_TRACE() );
						break;
					}

					if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
					{
						pUpdateInfo->pCurCont = pUpdateInfo->pCurGroup->GetChildCont( pStart->GetYesLinkID() );
					}
					else
					{
						pUpdateInfo->pCurCont = pUpdateInfo->pCurGroup->GetChildCont( pStart->GetNoLinkID() );
					}
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_GCOND:
				{
					CDboTSContGCond* pCond = (CDboTSContGCond*)pUpdateInfo->pCurCont;
					NTL_TSRESULT tsResult = pCond->RunAll( GetParent()->GetRecv(), &sTParam );

					if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
					{
						SetError();
						CNtlTSLog::Log( "TS exception progress is failed. Info[%d,%d,%d]. [%s]", GetTrigger()->GetID(), pUpdateInfo->pCurGroup->GetID(), pCond->GetID(), TS_CODE_TRACE() );
						break;
					}

					if ( NTL_TSRESULT_TYPE_SUCCESS == tsResult )
					{
						pUpdateInfo->pCurCont = pUpdateInfo->pCurGroup->GetChildCont( pCond->GetYesLinkID() );
					}
					else
					{
						pUpdateInfo->pCurCont = pUpdateInfo->pCurGroup->GetChildCont( pCond->GetNoLinkID() );
					}
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_END:
				{
					CDboTSContEnd* pEnd = (CDboTSContEnd*)pUpdateInfo->pCurCont;
					NTL_TSRESULT tsResult = pEnd->RunAll( GetParent()->GetRecv(), &sTParam );

					if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
					{
						SetError();
						CNtlTSLog::Log( "TS exception progress is failed. Info[%d,%d,%d]. [%s]", GetTrigger()->GetID(), pUpdateInfo->pCurGroup->GetID(), pEnd->GetID(), TS_CODE_TRACE() );
						break;
					}

					pUpdateInfo->pCurCont = 0;
				}
				break;
			case DBO_CONT_TYPE_ID_CONT_GACT:
				{
					CDboTSContGAct* pAct = (CDboTSContGAct*)pUpdateInfo->pCurCont;

					sTS_KEY sKey; sKey.Init();
					sKey.tID = GetTrigger()->GetID();
					sKey.tgID = ((CNtlTSGroup*)pAct->GetParent())->GetID();
					sKey.tcID = pAct->GetID();

					BeginExceptionAction( sKey );

					NTL_TSRESULT tsResult = pAct->RunAll( GetParent()->GetRecv(), &sTParam );

					EndExceptionAction( sKey );

					if ( tsResult & NTL_TSRESULT_TYPE_ERROR )
					{
						SetError();
						CNtlTSLog::Log( "TS exception progress is failed. Info[%d,%d,%d]. [%s]", GetTrigger()->GetID(), pUpdateInfo->pCurGroup->GetID(), pAct->GetID(), TS_CODE_TRACE() );
						break;
					}

					pUpdateInfo->pCurCont = pUpdateInfo->pCurGroup->GetChildCont( pAct->GetNextLinkID() );
				}
				break;

			default:
				{
					SetError();
					CNtlTSLog::Log( "Not supported container type. Info[%d,%d]. [%s]", GetTrigger()->GetID(), pUpdateInfo->pCurGroup->GetID(), TS_CODE_TRACE() );
					break;
				}
			}

			++it;
		}
	}
}

void CDboTSTCtrl::AutoDetachedUpdateExceptionGroup( CNtlTSGroup* pAutoDetachedGroup )
{
	UNREFERENCED_PARAMETER( pAutoDetachedGroup );
}

void CDboTSTCtrl::AddUpdateExceptionGroupActRef( NTL_TS_TG_ID tgId )
{
	mapdef_GROUP_UPDATE_INFO::iterator it = m_defGroupUpdateList.find( tgId );

	if ( it != m_defGroupUpdateList.end() )
	{
		it->second.nActionRef++;
	}
}

void CDboTSTCtrl::ReleaseUpdateExceptionGroupActRef( NTL_TS_TG_ID tgId )
{
	mapdef_GROUP_UPDATE_INFO::iterator it = m_defGroupUpdateList.find( tgId );

	if ( it != m_defGroupUpdateList.end() )
	{
		it->second.nActionRef--;
	}
}

void CDboTSTCtrl::UpdateTimeWait( void )
{
	// 타임 대기 중이 아님
	if ( 0 == m_uiTWPeriod ) return;

	// 타임 대기 종료
	if ( GetCurTime() - m_uiTWStart > m_uiTWPeriod )
	{
		m_uiTWPeriod = 0;
	}
}

void CDboTSTCtrl::MoveTSP( CNtlTSCont*, CNtlTSCont* pNextCont, bool )
{
	m_pCurTSP = pNextCont;
}
