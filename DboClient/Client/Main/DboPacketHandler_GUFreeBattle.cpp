/*****************************************************************************
* File			: DboPackethandler_GUFreeBattle.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: Free Battle 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "TextAllTable.h"

// core
#include "NtlMath.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptPVP.h"
#include "NtlWorldConceptFreePvpZone.h"

// client
#include "MsgBoxManager.h"
#include "CautionNotifyGui.h"


void PacketHandler_GSFreeBattleChallengeRes(void *pPacket)
{
	sGU_FREEBATTLE_CHALLENGE_RES *pResult = (sGU_FREEBATTLE_CHALLENGE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_FREEBATTLE_CHALLENGE_RES);

	if(pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_FREEBATTLE_CHALLENGE_RES"), TRUE);
	}
}

void PacketHandler_GSFreeBattleAcceptReq(void *pPacket)
{
	sGU_FREEBATTLE_ACCEPT_REQ *pAcceptReq = (sGU_FREEBATTLE_ACCEPT_REQ*)pPacket;

	GetDboGlobal()->SetAskedPCSerial(pAcceptReq->hChallenger);

	if( Logic_CanAcceptOthersAsk() == false )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_CANCEL);
		GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
		return;
	}	
    else if(Logic_IsBlackList(pAcceptReq->hChallenger)) // 블랙리스트에 등록되어 있는 경우 거절  
    {
        GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_DENY);        
        GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
        return;
    }

	GetAlarmManager()->FormattedAlarmMessage( "DST_FREEPVP_CHALLENGE", FALSE, NULL, Logic_GetName( GetNtlSobManager()->GetSobObject( pAcceptReq->hChallenger ) ) );


	//WCHAR wchBuffer[MAX_MSG_BOX_STRING_LEN];
	//const WCHAR *pString = GetDisplayStringManager()->GetString("DST_FREEPVP_CHALLENGE");

	//if(pString == NULL)
	//	return;

	//CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pAcceptReq->hChallenger);
	//if(pSobObj == NULL)
	//	return;

	//const WCHAR *pName = Logic_GetName(pSobObj);
	//if(pName == NULL)
	//	return;

	//swprintf_s(wchBuffer, MAX_MSG_BOX_STRING_LEN, pString, pName);

	//CDboEventGenerator::MsgBoxShow( wchBuffer, MBW_FREEPVP_CHALLENGE, MBT_OKCANCEL );
}

void PacketHandler_GSFreeBattleCancelNfy(void *pPacket)
{
	sGU_FREEBATTLE_CANCEL_NFY *pResult = (sGU_FREEBATTLE_CANCEL_NFY*)pPacket;

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	if(pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_FREEBATTLE_CHALLENGE_RES"), TRUE);
		//GetMsgBoxManager()->DeleteMsgBox( MBW_FREEPVP_CHALLENGE );			// TO DO
	}
}

void PacketHandler_GSFreeBattleStartNfy(void *pPacket)
{		
	sGU_FREEBATTLE_START_NFY *pStartNfy = (sGU_FREEBATTLE_START_NFY*)pPacket;

	RwV3d vPos;
	CNtlMath::MathRwV3dAssign(&vPos, pStartNfy->vRefreeLoc.x, pStartNfy->vRefreeLoc.y, pStartNfy->vRefreeLoc.z);
	
	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_FREEPVP);
	CNtlWorldConceptPVP* pWorldConceptPVP = (CNtlWorldConceptPVP*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_FREEPVP);
	pWorldConceptPVP->SetFreePvPReferPos(vPos);
	pWorldConceptPVP->SetFreePvPTarget(pStartNfy->hTarget);

	pWorldConceptPVP->SetPvpName(pStartNfy->hTarget);
}

void PacketHandler_GSFreeBattleOutSideNfy(void *pPacket)
{
	GetAlarmManager()->FormattedAlarmMessage( "DST_FREEPVP_OUTSIDE_STAGE", FALSE, NULL, (RwInt32)dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME );
}

void PacketHandler_GSFreeBattleInSideNfy(void *pPacket)
{
	CDboEventGenerator::TimeFixedNotifyMessage(FALSE, "DST_FREEPVP_OUTSIDE_STAGE", NULL);
}

void PacketHandler_GSFreeBattleEndNfy(void *pPacket)
{
	sGU_FREEBATTLE_END_NFY *pEndNfy = (sGU_FREEBATTLE_END_NFY*)pPacket;
	
	if(pEndNfy->byFreeBattleResult == FREEBATTLE_RESULT_WIN)
		GetAlarmManager()->FormattedAlarmMessage( "DST_FREEPVP_WIN", FALSE, NULL, Logic_GetName( GetNtlSLGlobal()->GetSobAvatar() ) );		
	else if(pEndNfy->byFreeBattleResult == FREEBATTLE_RESULT_LOSE)
		GetAlarmManager()->FormattedAlarmMessage( "DST_FREEPVP_LOSE", FALSE, NULL, Logic_GetName( GetNtlSLGlobal()->GetSobAvatar() ) );		
	else
		GetAlarmManager()->AlarmMessage("DST_FREEPVP_DRAW");

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_FREEPVP);

	CNtlSLEventGenerator::TSFreeBattle();
}

void PacketHandler_GSWorldFreePvpZoneEnteredNfy(void * pPacket)
{
	sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY *pNfy = (sGU_WORLD_FREE_PVP_ZONE_ENTERED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar)
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pNfy->hSubject);
		if (pSob)
		{
			CNtlSobAttr* pSobAttr = pSob->GetSobAttr();

			CNtlSobPlayerAttr* pAvatarPlayer = reinterpret_cast<CNtlSobPlayerAttr*>(pAvatar->GetSobAttr());

			if (pSob->GetSerialID() == pAvatar->GetSerialID())
			{
				if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_FREEPVP_ZONE) == FALSE)
				{
					GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_FREEPVP_ZONE);
				}

				CNtlWorldConceptFreePvpZone* pWorldConcept = (CNtlWorldConceptFreePvpZone*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_FREEPVP_ZONE);
				if (pWorldConcept)
				{
					CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
					pSobPlayerAttr->SetIsInFreePvpZone(true);

					pWorldConcept->ChangeState(WORLD_STATE_IDLE);
					pWorldConcept->UpdateNames();

					GetAlarmManager()->AlarmMessage("DST_PVP_ZONE_ENTER");
				}
			}
			else if (pSob->GetClassID() == SLCLASS_PLAYER)
			{
				CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
				if (pSobPlayerAttr)
				{
					pSobPlayerAttr->SetIsInFreePvpZone(true);
				}

				Logic_SetHeadNameColor(pSob);
			}
			else if (pSob->GetClassID() == SLCLASS_PET)
			{
				CNtlSob* pSobOwner = GetNtlSobManager()->GetSobObject(pSob->GetOwnerID());
				if (pSobOwner)
				{
					CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
					if (pSobPlayerAttr)
						pSobPlayerAttr->SetIsInFreePvpZone(true);

					Logic_SetHeadNameColor(pSobOwner);

					//if ( *(_DWORD *)(v3 + 48) != v13[12] )
						Logic_SetHeadNameColor(pSob);
				}
			}
		}
	}
}

void PacketHandler_GSWorldFreePvpZoneLeftNfy(void * pPacket)
{
	sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY *pNfy = (sGU_WORLD_FREE_PVP_ZONE_LEFT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar)
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pNfy->hSubject);
		if (pSob)
		{
			CNtlSobAttr* pSobAttr = pSob->GetSobAttr();

			CNtlSobPlayerAttr* pAvatarPlayer = reinterpret_cast<CNtlSobPlayerAttr*>(pAvatar->GetSobAttr());

			if (pSob->GetSerialID() == pAvatar->GetSerialID())
			{
				if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_FREEPVP_ZONE))
				{
					CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
					pSobPlayerAttr->SetIsInFreePvpZone(false);

					CNtlWorldConceptFreePvpZone* pWorldConcept = (CNtlWorldConceptFreePvpZone*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_FREEPVP_ZONE);
					if (pWorldConcept)
					{
						pWorldConcept->UpdateNames();
					}

					GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_FREEPVP_ZONE);
					GetAlarmManager()->AlarmMessage("DST_PVP_ZONE_LEFT");
				}
			}
			else if (pSob->GetClassID() == SLCLASS_PLAYER)
			{
				CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
				if(pSobPlayerAttr)
					pSobPlayerAttr->SetIsInFreePvpZone(false);
				
				Logic_SetHeadNameColor(pSob);
			}
			else if (pSob->GetClassID() == SLCLASS_PET)
			{
				CNtlSob* pSobOwner = GetNtlSobManager()->GetSobObject(pSob->GetOwnerID());
				if (pSobOwner)
				{
					CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobAttr);
					if (pSobPlayerAttr)
						pSobPlayerAttr->SetIsInFreePvpZone(false);

					Logic_SetHeadNameColor(pSobOwner);
					Logic_SetHeadNameColor(pSob);
				}
			}
		}
	}
}
