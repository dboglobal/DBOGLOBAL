#include "precomp_ntlsimulation.h"
#include "NtlFSMCharController.h"

//shared
#include "NtlMovement.h"
#include "NtlResultCode.h"

//core
#include "NtlDebug.h"
#include "NtlMath.h"

// simulation
#include "NtlFSMDef.h"
#include "NtlFSMLayer.h"
#include "NtlFSMStateBase.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMEventQueuingHelper.h"
#include "NtlPacketGenerator.h"
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlBehaviorData.h"
#include "NtlSobActor.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlClock.h"
#include "NtlSobAvatar.h"

#define NTL_FSM_ATTACKBEGIN_SEARCH_TIME		0.03f
#define NTL_FSM_ATTACKEND_SEARCH_TIME		1.0f

CNtlFSMSyncMsgAttackBegin::CNtlFSMSyncMsgAttackBegin()
{
	m_bAttackBegin		= FALSE;
	m_fBeginAndEndTime	= 0.0f;
	m_hTargetSerialId	= INVALID_SERIAL_ID;
	m_byAvatarControlId	= 0;
}

CNtlFSMSyncMsgAttackBegin::~CNtlFSMSyncMsgAttackBegin()
{
}

void CNtlFSMSyncMsgAttackBegin::SetTargetSerialId( SERIAL_HANDLE hTargetSerialId )
{
    if(m_hTargetSerialId != hTargetSerialId)
    {
        m_bAttackBegin = FALSE;
		API_GetSLPacketGenerator()->SendCharAttackEnd(m_byAvatarControlId, GetNtlSLGlobal()->GetSobAvatar()->GetServerSyncAvatarType());
    }

    m_hTargetSerialId = hTargetSerialId;
}

////////////////////////////////////////////////////////////////////////////
//  des : attack range를 검사하여 attack begin or end packet을 보낸다.
//  ------------------------------------------------------------------------
//  paramater : 
//  \param1 : delta time.
////////////////////////////////////////////////////////////////////////////

void CNtlFSMSyncMsgAttackBegin::Update(CNtlSobActor *pActor, RwReal fElapsed)
{
	// 서버로 attack end protocol을 보낸다.
	if(m_hTargetSerialId == INVALID_SERIAL_ID)
	{
		if(m_bAttackBegin)
		{
			m_bAttackBegin = FALSE;
			API_GetSLPacketGenerator()->SendCharAttackEnd(m_byAvatarControlId, pActor->GetServerSyncAvatarType());
		}

		return;
	}
	
	m_fBeginAndEndTime += fElapsed;
	if(m_bAttackBegin)
	{
		if(m_fBeginAndEndTime < NTL_FSM_ATTACKEND_SEARCH_TIME)
			return;
	}
	else
	{
		if(m_fBeginAndEndTime < NTL_FSM_ATTACKBEGIN_SEARCH_TIME)
			return;
	}

	if( !GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor(pActor, m_hTargetSerialId) )
	{
		if(m_bAttackBegin)
		{
			API_GetSLPacketGenerator()->SendCharAttackEnd(m_byAvatarControlId, pActor->GetServerSyncAvatarType());
			m_bAttackBegin = FALSE;
		}

		return;
	}

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hTargetSerialId);
	if(pSobObj == NULL)
	{
		API_GetSLPacketGenerator()->SendCharAttackEnd(m_byAvatarControlId, pActor->GetServerSyncAvatarType());
		m_hTargetSerialId = INVALID_SERIAL_ID;
		m_bAttackBegin = FALSE;
	}
	else
	{
		RwV3d vSub = pSobObj->GetPosition() - pActor->GetPosition();				
		vSub.y = 0.0f;
		RwReal fSubDist = RwV3dLength(&vSub);

		if(m_bAttackBegin)
		{
			if(fSubDist > Logic_GetAttackRange(pActor, m_hTargetSerialId) * 1.3f || !pActor->IsFightingMode())
			{
				m_bAttackBegin = FALSE;
				API_GetSLPacketGenerator()->SendCharAttackEnd(m_byAvatarControlId, pActor->GetServerSyncAvatarType());
			}
		}
		else
		{
			CNtlSobActor *pTarActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
			if(pActor->IsFightingMode() && (fSubDist - 0.1f) <= Logic_GetAttackRange(pActor, m_hTargetSerialId) && !pTarActor->IsDie())
			{
				m_bAttackBegin = TRUE;
				API_GetSLPacketGenerator()->SendCharAttackBegin(m_byAvatarControlId, pActor->GetServerSyncAvatarType());
			}
		}
	}

	m_fBeginAndEndTime = 0.0f;
}

