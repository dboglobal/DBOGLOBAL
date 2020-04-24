#include "precomp_ntlsimulation.h"
#include "NtlSobTriggerObject.h"

// core
#include "NtlDebug.h"

// Sound
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "ObjectTable.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobTriggerObjectProxy.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "DboTSCQAgency.h"
#include "DboTSCTAgency.h"


DEFINITION_MEMORY_POOL(CNtlSobTriggerObject)


CNtlSobTriggerObject::CNtlSobTriggerObject(VOID)
{
	m_fTime				= 0.0f;
	m_byQuestMark		= (RwUInt8)EQuestMarkNone;
	m_bActiveQuestMark	= TRUE;
	m_tID				= NTL_TS_T_ID_INVALID;

	m_byStateType = INVALID_TOBJECT_STATE_TYPE;
	m_sCurState.byMainState = TOBJECT_STATE_NONE;
	m_sCurState.bySubState = 0;

	m_sPreState = m_sCurState;

	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_TARGET_NEAR_APPROACH | SLFLAG_TARGET_SELECT_GUI_UPDATE);
	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE);
}


CNtlSobTriggerObject::~CNtlSobTriggerObject(VOID)
{
}

RwBool CNtlSobTriggerObject::Create(VOID)
{
	NTL_FUNCTION("CNtlSobTriggerObject::Create");

	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobTriggerObjectProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if(!CNtlSobActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_TRIGGER_OBJECT);

	NTL_RETURN(TRUE);
}

void CNtlSobTriggerObject::Destroy(VOID)
{
	NTL_FUNCTION("CNtlSobTriggerObject::Destroy");

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

CNtlFSMLayer* CNtlSobTriggerObject::CreateFSM(void)
{
	return NULL;
}

CNtlSobInfluence* CNtlSobTriggerObject::CreateSobInfluence(void)
{
	return NULL;
}

void CNtlSobTriggerObject::Update( RwReal fElapsed )
{
	QeustMarkCheck(fElapsed);

	CNtlSobActor::Update(fElapsed);
}

void CNtlSobTriggerObject::HandleEvents( RWS::CMsg& pMsg )
{
	NTL_FUNCTION("CNtlSobTriggerObject::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobTriggerObjectCreate *pSobCreate = reinterpret_cast<SNtlEventSobTriggerObjectCreate*>(pMsg.pData);

		// trigger object attribute events.
		
		GetSobAttr()->HandleEvents(pMsg);

		// 초기 Trigger object의 상태를 설정한다
		InitState();

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		// 좌표와 방향 세팅.
		RwV3dAssignMacro(&m_vPos, &pSobCreate->vLoc);
		RwV3d vDir = GetSobProxy()->GetDirection();
		RwV3dAssignMacro(&m_vDirection, &vDir); 

		CNtlSobTriggerObjectAttr *pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( GetSobAttr() );
		if(pTriggerObjectAttr->IsHaveOperating())
		{
			SetFlags( GetFlags() | SLFLAG_TARGET_TRIGGER_OPERATION);
		}
	}

	else if(pMsg.Id == g_EventSobDelete)
	{
	}

	else if(pMsg.Id == g_EventSobTargetSelect)
	{
		CNtlSobProxy *pSobProxy = GetSobProxy();
		if(pSobProxy == NULL)
			return;

		pSobProxy->HandleEvents(pMsg);

		if(GetFlags() & SLFLAG_TARGET_SELECT_GUI_UPDATE)
		{
			SNtlEventSobTargetSelect *pSobTargetSelect = reinterpret_cast<SNtlEventSobTargetSelect*>( pMsg.pData );
			if(GetSerialID() == pSobTargetSelect->hSerialId)
				CNtlSLEventGenerator::SobTargetUpdate(GetSerialID(), TRUE); 
		}

		PlayClickSound();
	}

	else if(pMsg.Id == g_EventSobAttackSelect)
	{
		PlayClickSound();
	}

	else if(pMsg.Id == g_EventSobServerUpdateCondition)
	{
		SNtlEventSobServerUpdateCondition *pSobServerState = reinterpret_cast<SNtlEventSobServerUpdateCondition*>( pMsg.pData );
		if(pSobServerState->dwCondition & CHARCOND_FLAG_CLIENT_UI_DISABLE)
		{
			ActiveQuestMark( FALSE );
		}
		else
		{
			ActiveQuestMark( TRUE );
		}
	}

	else if(pMsg.Id == g_EventAnimEnd)
	{
		GetSobProxy()->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}

	else if(pMsg.Id == g_EventQuestMark)
	{
		GetSobProxy()->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}

	else if(pMsg.Id == g_EventTObjectUpdateState)
	{
		SNtlEventTObjectUpdateState *pSobTObjUpdateState = reinterpret_cast<SNtlEventTObjectUpdateState*>(pMsg.pData);

		// Trigger object의 상태를 업데이트 한다
		UpdateState(pSobTObjUpdateState);

		GetSobProxy()->HandleEvents(pMsg);

		NTL_RETURNVOID();
	}

	CNtlSobActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}

void CNtlSobTriggerObject::SetDirection(const RwV3d *pDir)
{
	RwV3dAssignMacro(&m_vDirection, pDir); 
}

void CNtlSobTriggerObject::QeustMarkCheck(RwReal fElapsed)
{
	if(!m_bActiveQuestMark)
		return;

	m_fTime += fElapsed;
	if(m_fTime < 1.5f)
		return;

	RwUInt8 byQuestMark = (RwUInt8)EQuestMarkNone;

	CDboTSCQAgency *pQuestAgency = API_GetQuestAgency();
	vecdef_QUEST_INFO_LIST *pQuestInfoList = NULL;
	
	sQUEST_INFO* pQuestInfo = NULL;
	RwBool		bFirstNewQuest = TRUE;
	RwUInt16	tID = NTL_TS_T_ID_INVALID;
	
	if(pQuestAgency)
	{
		vecdef_QUEST_INFO_LIST::iterator it;
		SGET_QUEST_INFO* pGetQuestInfoList =  pQuestAgency->GetQuestInfoList(eEVENT_GEN_TYPE_CLICK_OBJECT, Logic_GetTriggerObjectId(this));
		pQuestInfoList = &pGetQuestInfoList->QuestInfoList;

		for(it = pQuestInfoList->begin(); it != pQuestInfoList->end(); ++it)
		{
			if ( (*it).bVisitOP )
			{
				byQuestMark = byQuestMark | (1 << EQuestMarkVisit );
			}

			if((*it).eProgType == eQUEST_PROGRESS_TYPE_NEW)
			{
				byQuestMark = byQuestMark | (1 << EQuestMarkNew);
				if( bFirstNewQuest )
				{
					tID = (*it).sKey.tID;
					pQuestInfo = &(*it);
					bFirstNewQuest = FALSE;
				}
			}
			else if((*it).eProgType == eQUEST_PROGRESS_TYPE_PROGRESS)
				byQuestMark = byQuestMark | (1 << EQuestMarkReward);
			else
				byQuestMark = byQuestMark | (1 << EQuestMarkReward);
		}

		if(byQuestMark & EQuestMarkFlagReward)
		{
			byQuestMark = EQuestMarkReward;
		}
		else if(byQuestMark & EQuestMarkFlagProgress)
		{
			byQuestMark = EQuestMarkProgress;
		}
		else if(byQuestMark & EQuestMarkFlagNew)
		{
			RwReal fDist = Logic_GetQuestNpcDistanceFromAvatar(this);
			if(	fDist >= NTL_QUEST_MARK_BALLOON_DIST ||
				pQuestInfo->dwQuestStartDialog == INVALID_TBLIDX )
				byQuestMark = EQuestMarkNew;
			else
				byQuestMark = EQuestMarkBalloonNew;
		}
		else if(byQuestMark & EQuestMarkFlagVisit)
		{
			byQuestMark = EQuestMarkVisit;
		}
		else
		{
			byQuestMark = EQuestMarkNone;
		}

		if( ( m_byQuestMark != byQuestMark ) ||
			( byQuestMark == EQuestMarkBalloonNew && m_tID != tID ) )
		{
			// event 발생.	
			m_byQuestMark = byQuestMark;
			m_tID = tID;
			CNtlSLEventGenerator::QuestMark(GetSerialID(), m_byQuestMark, (void*)pQuestInfo);
		}		
	}

	m_fTime = 0.0f;
}

RwBool CNtlSobTriggerObject::CanClicked(void)
{
	CNtlSobTriggerObjectAttr* pAttrTObj = dynamic_cast< CNtlSobTriggerObjectAttr* > ( GetSobAttr() );
	if ( NULL == pAttrTObj )
	{
		return FALSE;
	}

	CNtlSobTriggerObjectProxy* pProxyTObj = dynamic_cast< CNtlSobTriggerObjectProxy* > ( GetSobProxy() );
	if ( NULL == pProxyTObj )
	{
		return FALSE;
	}

	// Trigger object table의 선택 가능 플래그가 꺼저 있는 경우는 클릭 불가능.
	if ( (pAttrTObj->m_pTriggerObjTbl->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_SELECTION) != eDBO_TRIGGER_OBJECT_FUNC_SELECTION )
	{
		return FALSE;
	}

	// 상태가 존재하는 경우만 클릭 검사를 수행하고
	// 그렇지 않은 경우는 무조건 클릭 가능 상태로 처리한다
	if ( INVALID_TOBJECT_STATE_TYPE != m_byStateType && TOBJECT_STATE_NONE != m_sCurState.byMainState )
	{
		// 현재 트리거 오브젝트의 상태가 보이는 상태인지 검사
		if ( (m_sCurState.bySubState & (1<<TOBJECT_SUBSTATE_SHOW)) != (1<<TOBJECT_SUBSTATE_SHOW) )
		{
			return FALSE;
		}

		// 현재 트리거 오브젝트의 상태가 클릭 불가능 상태인지 검사
		if ( (m_sCurState.bySubState & (1<<TOBJECT_SUBSTATE_UNSELECT)) == (1<<TOBJECT_SUBSTATE_UNSELECT) )
		{
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CNtlSobTriggerObject::IsScouterObject( void )
{
	return (((CNtlSobTriggerObjectAttr*)GetSobAttr())->GetTriggerObjectTbl()->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_SCOUTER) ? TRUE : FALSE;
}

RwBool CNtlSobTriggerObject::IsTLQObject( void )
{
	return (((CNtlSobTriggerObjectAttr*)GetSobAttr())->GetTriggerObjectTbl()->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_TIME_LEAP_QUEST) ? TRUE : FALSE;
}

RwBool CNtlSobTriggerObject::IsPIDObject( void )
{
	return (((CNtlSobTriggerObjectAttr*)GetSobAttr())->GetTriggerObjectTbl()->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_PARTY_INSTANCE_DUNGEON) ? TRUE : FALSE;
}

RwBool CNtlSobTriggerObject::IsDojoSealObject( void )
{
	return (((CNtlSobTriggerObjectAttr*)GetSobAttr())->GetTriggerObjectTbl()->dwFunction & eDBO_TRIGGER_OBJECT_FUNC_DOJO_SEAL) ? TRUE : FALSE;
}

void CNtlSobTriggerObject::ActiveQuestMark(RwBool bQuestMark)
{
	if(m_bActiveQuestMark && !bQuestMark)
	{
		if(m_byQuestMark != EQuestMarkNone)
		{
			// none을 보낸다.
			m_byQuestMark = EQuestMarkNone;
			CNtlSLEventGenerator::QuestMark(GetSerialID(), m_byQuestMark, NULL);
		}
	}
	else if(!m_bActiveQuestMark && bQuestMark)
	{
	}

	m_fTime = 0.0f;
	m_bActiveQuestMark = bQuestMark;
}

RwBool CNtlSobTriggerObject::ShowTOForSCT( RwBool bShow )
{
	if ( IsScouterObject() )
	{
		((CNtlSobTriggerObjectProxy*)GetSobProxy())->EnableVisible( bShow );

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CNtlSobTriggerObject::InitState( void )
{
	CNtlSobTriggerObjectAttr* pAttr = (CNtlSobTriggerObjectAttr*)GetSobAttr();
	if ( NULL == pAttr || NULL == pAttr->GetTriggerObjectTbl() )
	{
		NTL_ASSERTFAIL( "Failed initializing the trigger object.\n" );
		NTL_RETURNVOID();
	}

	sOBJECT_TBLDAT* pTbl = pAttr->GetTriggerObjectTbl();

	m_byStateType = pTbl->byStateType;
	m_sCurState.byMainState = pTbl->byDefMainState;
	m_sCurState.bySubState = pTbl->byDefSubState;

	m_sPreState = m_sCurState;

	if ( CanClicked() )
	{
		RwUInt32 uiFlags = GetFlags();
		uiFlags &= ~SLFLAG_NOT_HIGHLIGHTED;
		uiFlags &= ~SLFLAG_NOT_TARGETED;
		SetFlags( uiFlags );
	}
	else
	{
		SetFlags( GetFlags() | SLFLAG_NOT_HIGHLIGHTED | SLFLAG_NOT_TARGETED );
	}
}


void CNtlSobTriggerObject::UpdateState( SNtlEventTObjectUpdateState *pSobTObjUpdateState )
{
	// 상태 저장
	m_sPreState = m_sCurState;

	// 새로운 상태로 업데이트
	m_sCurState.byMainState = pSobTObjUpdateState->byMainState;
	m_sCurState.bySubState = pSobTObjUpdateState->bySubState;

	if ( CanClicked() )
	{
		RwUInt32 uiFlags = GetFlags();
		uiFlags &= ~SLFLAG_NOT_HIGHLIGHTED;
		uiFlags &= ~SLFLAG_NOT_TARGETED;
		SetFlags( uiFlags );
	}
	else
	{
		SetFlags( GetFlags() | SLFLAG_NOT_HIGHLIGHTED | SLFLAG_NOT_TARGETED );
	}
}


void CNtlSobTriggerObject::PlayClickSound( void )
{
	CNtlSobTriggerObjectAttr* pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( GetSobAttr() );
	if ( NULL == pTriggerObjectAttr ) return;

	RwV3d v3Pos;
	v3Pos.x = pTriggerObjectAttr->m_pTriggerObjTbl->vLoc.x;
	v3Pos.y = pTriggerObjectAttr->m_pTriggerObjTbl->vLoc.y;
	v3Pos.z = pTriggerObjectAttr->m_pTriggerObjTbl->vLoc.z;

	// 트리거 오브젝트의 상태가 존재하지 않는 경우
	if ( INVALID_TOBJECT_STATE_TYPE != m_byStateType && TOBJECT_STATE_NONE != m_sCurState.byMainState )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_AVATAR_EFFECT_SOUND;
		tSoundParam.pcFileName		= GSD_SYSTEM_TRIGGER_OBJECT_CLICK;
		tSoundParam.fXPos			= v3Pos.x;
		tSoundParam.fYPos			= v3Pos.y;
		tSoundParam.fZPos			= v3Pos.z;

		GetSoundManager()->Play(&tSoundParam);
	}
	// 트리거 오브젝트의 상태가 존재하는 경우
	else
	{
		// 트리거 오브젝트를 클릭 가능 한 경우만 소리를 낸다
		if ( CanClicked() )
		{
			// 서버로 부터 상태가 잘못 내려온 경우
			if ( MAX_TOBJECT_STATE <= m_sCurState.byMainState )
			{
				sNtlSoundPlayParameta tSoundParam;
				tSoundParam.iChannelGroup	= CHANNEL_GROUP_AVATAR_EFFECT_SOUND;
				tSoundParam.pcFileName		= GSD_SYSTEM_TRIGGER_OBJECT_CLICK;
				tSoundParam.fXPos			= v3Pos.x;
				tSoundParam.fYPos			= v3Pos.y;
				tSoundParam.fZPos			= v3Pos.z;

				GetSoundManager()->Play(&tSoundParam);
			}
			else
			{
				std::string strSoundName = pTriggerObjectAttr->m_pTriggerObjTbl->achClickSound[m_sCurState.byMainState];

				// 상태에 따른 클릭 사운드가 존재하는 경우
				if ( !strSoundName.empty() )
				{
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_AVATAR_EFFECT_SOUND;
					tSoundParam.pcFileName		= (char*)strSoundName.c_str();
					tSoundParam.fXPos			= v3Pos.x;
					tSoundParam.fYPos			= v3Pos.y;
					tSoundParam.fZPos			= v3Pos.z;

					GetSoundManager()->Play(&tSoundParam);
				}
				// If there is no click sound for the state
				else
				{
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_AVATAR_EFFECT_SOUND;
					tSoundParam.pcFileName		= GSD_SYSTEM_TRIGGER_OBJECT_CLICK;
					tSoundParam.fXPos			= v3Pos.x;
					tSoundParam.fYPos			= v3Pos.y;
					tSoundParam.fZPos			= v3Pos.z;

					GetSoundManager()->Play(&tSoundParam);
				}
			}
		}
	}
}
