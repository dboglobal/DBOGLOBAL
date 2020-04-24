/*****************************************************************************
* File			: DboPackethandler.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 패킷 핸들러 모음
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// core
#include "NtlMath.h"

// shared
#include "NtlMovement.h"
#include "SystemEffectTable.h"
#include "ItemTable.h"
#include "PCTable.h"
#include "UseItemTable.h"
#include "QuestItemTable.h"
#include "ObjectTable.h"
#include "NPCTable.h"
#include "SkillTable.h"
#include "NtlCharacterState.h"
#include "NtlPacketUtil.h"
#include "NtlAdmin.h"
#include "NtlTeleport.h"
#include "NtlDirection.h"
#include "DirectionLinkTable.h"
#include "NtlLuaState.h"
#include "NtlItem.h"
#include "NtlFSMDef.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLResourceManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLEventGenerator.h"

// simulation
#include "NtlSLDef.h"
#include "NtlClientNet.h"
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobActor.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobIcon.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobWorldItem.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlWorldconceptTrade.h"
#include "NtlWorldConceptDBC.h"
#include "NtlSobManager.h"
#include "NtlSobStatusAnimSyncManager.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"
#include "InputActionMap.h"
#include "NtlDTCinematicManager.h"
#include "NtlSobProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlBehaviorData.h"
#include "NtlSLLuaGlue.h"
#include "NtlSLLogic.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSLGlobal.h"
#include "NtlSkillContainer.h"
#include "NtlSobSkillIcon.h"
#include "NtlSoundMoodManager.h"


// dbo
#include "IconMoveManager.h"
#include "PacketProc.h"
#include "InfoWndManager.h"
#include "MsgBoxManager.h"
#include "DboTSCTAgency.h"
#include "DboTSCQAgency.h"
#include "DialogManager.h"
#include "ChatBalloonManager.h"
#include "AlarmManager.h"
#include "DiceManager.h"
#include "DisplayStringManager.h"

#include "GMPopupGui.h"
#include "GMQuickSlotGui.h"

// discord
#ifdef USE_DISCORD
#include "Discord.h"
#endif


void PacketHandler_GSObjectCreate(void *pPacket)
{
	sGU_OBJECT_CREATE *pObjCreate = (sGU_OBJECT_CREATE*)pPacket;

	RwV3d vLoc, vDir;

	if(pObjCreate->sObjectInfo.objType == OBJTYPE_PC)
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurLoc.x,
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurLoc.y,
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir,
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurDir.x,
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurDir.y,
			pObjCreate->sObjectInfo.pcState.sCharStateBase.vCurDir.z);

		sPC_BRIEF *pPcBrief = &(pObjCreate->sObjectInfo.pcBrief);
		sCHARSTATE *pCharState = &(pObjCreate->sObjectInfo.pcState);

		CNtlSLEventGenerator::SobPlayerCreate(SLCLASS_PLAYER, pObjCreate->handle, vLoc, vDir, pPcBrief, pCharState);
	}
	else if(pObjCreate->sObjectInfo.objType == OBJTYPE_MOB)
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurLoc.x,
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurLoc.y,
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir, 
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurDir.x,
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurDir.y,
			pObjCreate->sObjectInfo.mobState.sCharStateBase.vCurDir.z);

		sMOB_BRIEF *pMobBrief = &(pObjCreate->sObjectInfo.mobBrief);
		sCHARSTATE *pCharState = &(pObjCreate->sObjectInfo.mobState);

		CNtlSLEventGenerator::SobMonsterCreate(SLCLASS_MONSTER, pObjCreate->handle, vLoc, vDir, pMobBrief, pCharState);
	}
	else if(pObjCreate->sObjectInfo.objType == OBJTYPE_NPC)
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurLoc.x,
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurLoc.y,
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir, 
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurDir.x,
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurDir.y,
			pObjCreate->sObjectInfo.npcState.sCharStateBase.vCurDir.z);

		sNPC_BRIEF *pNpcBrief = &(pObjCreate->sObjectInfo.npcBrief);
		sCHARSTATE *pCharState = &(pObjCreate->sObjectInfo.npcState);

		// dragon ball camera 연출.
		if(GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT) && 
			Logic_IsDragonBallNPC(pNpcBrief->tblidx))
		{
			RwBool bScheduling = GetNtlResourceManager()->IsLoadScheduling();

			GetNtlResourceManager()->SetLoadScheduling(FALSE);
			CNtlSLEventGenerator::SobNpcCreate(SLCLASS_NPC, pObjCreate->handle, vLoc, vDir, pNpcBrief, pCharState, FALSE);
			GetNtlResourceManager()->SetLoadScheduling(bScheduling);

            // 자신이 소환한 용신일때만 카메라 연출을 한다. (2007.9.10 by agebreak)
            CNtlWorldConceptDBC* pDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
            if(pDBC->IsMyDragon())
            {
			    // 여기다가... dragon ball camera를 연출한다.
			    CNtlSLEventGenerator::CameraDB(pObjCreate->handle);
            }
		}
		else
		{
			CNtlSLEventGenerator::SobNpcCreate(SLCLASS_NPC, pObjCreate->handle, vLoc, vDir, pNpcBrief, pCharState, TRUE);

			sNPC_TBLDAT* pNpc = (sNPC_TBLDAT*)API_GetTableContainer()->GetNpcTable()->FindData(pNpcBrief->tblidx);
			if (pNpc)
			{
				if (pNpc->dwFunc_Bit_Flag & NPC_FUNC_FLAG_PORTAL)
				{
					CNtlSLEventGenerator::PortalAddReq(pObjCreate->handle);
				}
			}
		}
		
	}
	else if(pObjCreate->sObjectInfo.objType == OBJTYPE_SUMMON_PET)
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurLoc.x,
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurLoc.y,
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurLoc.z);
		CNtlMath::MathRwV3dAssign(&vDir, 
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurDir.x,
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurDir.y,
			pObjCreate->sObjectInfo.summonPetState.sCharStateBase.vCurDir.z);

		sCHARSTATE *pCharState = &(pObjCreate->sObjectInfo.summonPetState);
		
		// 이 부분 잠시 검토
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if(pSobAvatar->GetSerialID() == pObjCreate->sObjectInfo.summonPetBrief.hOwner)
		{
			SSummonPetInfo *pSummonPetInfo = GetNtlSLGlobal()->GetAvatarSummonPetInfo();

			UPetData uPetBrief;
			uPetBrief.pPetProfile = &(pSummonPetInfo->sCharPf);

			CNtlSLEventGenerator::SobPetCreate(SLCLASS_PET, pSummonPetInfo->uiSerialId, vLoc, vDir, TRUE, uPetBrief, pCharState);
			CNtlSLEventGenerator::SobSummonPetSpawnSync(pSummonPetInfo->sCharPf.hOwner, pSummonPetInfo->uiSerialId);

			// 자신의 Pet인경우에만 Pet UI를 뛰운다.
			CDboEventGenerator::SummonPet(TRUE, pSummonPetInfo->uiSerialId);
		}
		else
		{
			UPetData uPetBrief;
			uPetBrief.pPetBrief = &(pObjCreate->sObjectInfo.summonPetBrief);

			CNtlSLEventGenerator::SobPetCreate(SLCLASS_PET, pObjCreate->handle, vLoc, vDir, FALSE, uPetBrief, pCharState);
			CNtlSLEventGenerator::SobSummonPetSpawnSync(pObjCreate->sObjectInfo.summonPetBrief.hOwner, pObjCreate->handle);
		}
	}
	else if( pObjCreate->sObjectInfo.objType == OBJTYPE_DROPITEM )
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.itemState.vCurLoc.x,
			pObjCreate->sObjectInfo.itemState.vCurLoc.y,
			pObjCreate->sObjectInfo.itemState.vCurLoc.z);

		void* pBrief = reinterpret_cast<void*>( &pObjCreate->sObjectInfo.itemBrief );
		void* pState = reinterpret_cast<void*>( &pObjCreate->sObjectInfo.itemState );
		void* pOptionSet = reinterpret_cast<void*>(&pObjCreate->sObjectInfo.itemOptionSet);

		CNtlSLEventGenerator::SobWorldItemCreate( SLCLASS_WORLD_ITEM, pObjCreate->handle, vLoc, OBJTYPE_DROPITEM, pBrief, pState, pOptionSet);
	}
	else if( pObjCreate->sObjectInfo.objType == OBJTYPE_DROPMONEY )
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.moneyState.vCurLoc.x,
			pObjCreate->sObjectInfo.moneyState.vCurLoc.y,
			pObjCreate->sObjectInfo.moneyState.vCurLoc.z);

		void* pBrief = reinterpret_cast<void*>( &pObjCreate->sObjectInfo.moneyBrief );
		void* pState = reinterpret_cast<void*>( &pObjCreate->sObjectInfo.moneyState );
		void* pOptionSet = NULL;

		CNtlSLEventGenerator::SobWorldItemCreate( SLCLASS_WORLD_ITEM, pObjCreate->handle, vLoc, OBJTYPE_DROPMONEY, pBrief, pState, pOptionSet);
	}
	// Dynamic Object
	else if(pObjCreate->sObjectInfo.objType == OBJTYPE_DYNAMIC)
	{
		CNtlMath::MathRwV3dAssign(&vLoc, 
			pObjCreate->sObjectInfo.dynamicObjState.vCurLoc.x,
			pObjCreate->sObjectInfo.dynamicObjState.vCurLoc.y,
			pObjCreate->sObjectInfo.dynamicObjState.vCurLoc.z);

		// Create
		CNtlSLEventGenerator::SobDynamicObject( pObjCreate->sObjectInfo.dynamicObjBrief.hOwner,
			SLCLASS_DYNAMIC_OBJECT,
			pObjCreate->handle, 
			pObjCreate->sObjectInfo.dynamicObjBrief.byObjectType,
			pObjCreate->sObjectInfo.dynamicObjBrief.idObjectTblidx,
			vLoc, 
			pObjCreate->sObjectInfo.dynamicObjState.byState ); 
	}
}

void PacketHandler_GSObjectDestroy(void *pPacket)
{
	sGU_OBJECT_DESTROY *pObjDestroy = (sGU_OBJECT_DESTROY*)pPacket;
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pObjDestroy->handle);

	if(Logic_GetAvatarTargetHandle() == pObjDestroy->handle)
	{
		Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
	}

	if(pSobAvatar->GetSerialID() != pObjDestroy->handle)
	{
		// 자기 Pet인 경우에만 UI 이벤트를 보낸다.
		if(pSobObj && pSobObj->GetClassID() == SLCLASS_PET && pSobObj->GetOwnerID() == pSobAvatar->GetSerialID())
		{
			CDboEventGenerator::SummonPet(FALSE, pObjDestroy->handle);
		}

		CDboEventGenerator::CharObjDelete(pObjDestroy->handle);
		CNtlSLEventGenerator::SobDelete(pObjDestroy->handle);
	}
}


/// Process result packet when assisting
void PacketHandler_GSCrossFireRes(void *pPacket)
{
	sGU_CROSSFIRE_RES *pPartyCrossfireRes = (sGU_CROSSFIRE_RES*)pPacket;

    CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pPartyCrossfireRes->hTarget);
    if(!pTarSobObj)
        return;

    // The server does not judge the target. The CLA should determine if it is an attackable target.
    if(Logic_IsEnemyTargetFromAvatarActor(pPartyCrossfireRes->hTarget) && pTarSobObj->CanHurted() && 
       !Logic_isEnemyTargetOcclusion(pPartyCrossfireRes->hTarget))
    {
		Logic_SobTarget(pPartyCrossfireRes->hTarget, INVALID_BYTE);
	//	CNtlSLEventGenerator::SobAttackSelect(pPartyCrossfireRes->hTarget); // by daneos: I dont think this is required ?
    }	
}

void PacketHandler_GSCharTargetLostNfy(void * pPacket)
{
	sGU_CHAR_TARGET_LOST_NFY *pRes = (sGU_CHAR_TARGET_LOST_NFY*)pPacket;

	CNtlSLEventGenerator::SobTargetUpdate(pRes->hSubject, FALSE);
}

void PacketHandler_GSCharTargetChanged(void * pPacket)
{
	sGU_CHAR_TARGET_CHANGED *pRes = (sGU_CHAR_TARGET_CHANGED*)pPacket;

	CNtlSLEventGenerator::SobTargetChanged(pRes->hObject, pRes->hTarget);
}

void PacketHandler_GSPetCharInfo(void *pPacket)
{
	sGU_PET_CHAR_INFO *pPetCharInfo = (sGU_PET_CHAR_INFO*)pPacket;

	SSummonPetInfo *pSummonPetInfo = GetNtlSLGlobal()->GetAvatarSummonPetInfo();
	
	pSummonPetInfo->uiSerialId = pPetCharInfo->handle;
	memcpy(&pSummonPetInfo->sCharPf, &pPetCharInfo->sSummonPetProfile, sizeof(sSUMMON_PET_PROFILE));

	memcpy(&pSummonPetInfo->sCharState.sCharStateBase, &pPetCharInfo->sCharState.sCharStateBase, sizeof(sCHARSTATE_BASE)); 

	RwInt32 iDataLen = pPetCharInfo->wCharStateSize - sizeof(sCHARSTATE_BASE);
	if(iDataLen > 0)
		memcpy(&pSummonPetInfo->sCharState.sCharStateDetail, &pPetCharInfo->sCharState.sCharStateDetail, iDataLen); 
}

void PacketHandler_GSPetCharSkillInfo( void *pPacket ) 
{
	sGU_PET_SKILL_INFO *pPetSkillInfo = (sGU_PET_SKILL_INFO*)pPacket;

	SSummonPetSkillInfo *pSummonPetSkillInfo = GetNtlSLGlobal()->GetAvatarSummonPetSkillInfo();
	// by daneos.. temporary
	pSummonPetSkillInfo->bySkillCount = 0;// pPetSkillInfo->bySkillCount;
	//memcpy(&pSummonPetSkillInfo->aSkillInfo, &pPetSkillInfo->aSkillInfo, sizeof(sSKILL_INFO_PET) * pSummonPetSkillInfo->bySkillCount );
}

void PacketHandler_GSPetCharInfoEnd(void *pPacket)
{
	/*
	RwV3d vLoc, vDir;

	SSummonPetInfo *pSummonPetInfo = GetNtlSLGlobal()->GetAvatarSummonPetInfo();

	sCHARSTATE *pCharState = &(pSummonPetInfo->sCharState);

	CNtlMath::MathRwV3dAssign(&vLoc, 
								pCharState->sCharStateBase.vCurLoc.x,
								pCharState->sCharStateBase.vCurLoc.y,
								pCharState->sCharStateBase.vCurLoc.z);
	CNtlMath::MathRwV3dAssign(&vDir, 
								pCharState->sCharStateBase.vCurDir.x,
								pCharState->sCharStateBase.vCurDir.y,
								pCharState->sCharStateBase.vCurDir.z);

	UPetData uPetBrief;
	uPetBrief.pPetProfile = &(pSummonPetInfo->sCharPf);

	CNtlSLEventGenerator::SobPetCreate(SLCLASS_PET, pSummonPetInfo->uiSerialId, vLoc, vDir, TRUE, uPetBrief, pCharState);
	CNtlSLEventGenerator::SobSummonPetSpawnSync(pSummonPetInfo->sCharPf.hOwner, pSummonPetInfo->uiSerialId);
	*/
}

void PacketHandler_GSPetCharDesapwnedNfy(void *pPacket)
{
	SSummonPetInfo *pSummonPetInfo = GetNtlSLGlobal()->GetAvatarSummonPetInfo();
	pSummonPetInfo->uiSerialId = INVALID_SERIAL_ID;
}

void PacketHandler_GSCharTeleportRes(void *pPacket)
{
	sGU_CHAR_TELEPORT_RES *pTeleport = (sGU_CHAR_TELEPORT_RES*)pPacket;
	if(pTeleport->wResultCode == GAME_SUCCESS)
	{
		eDialogType edType = (eDialogType)GetDialogManager()->GetOpendNPCDIalog();
		if( edType != DIALOG_UNKNOWN )
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_NPC_BYEBYE, DIALOG_UNKNOWN, edType);
		}

		// teleport 전에 정보를 clear 한다.
		// 정보 clear
		Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
		//CBalloonManager::GetInstance()->RemoveAllBalloonData();
		GetNtlGameCameraManager()->EnableUpdateData(FALSE);
		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
		GetNtlWorldConcept()->DeleteGradeWorldPlayConcept(WORLD_CONCEPT_SECOND_GRADE);
		GetInputActionMap()->Reset();
		if(GetNtlDTCinematicManager()->IsRunning())
			GetNtlDTCinematicManager()->Clear();
		CNtlSLEventGenerator::SobRevivalNotify();

		// 새로운 서버로 옮겨야 하는가?
		if(pTeleport->bIsToMoveAnotherServer)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendServerChangeReq();
		}
		else
		{
			// 새로운 정보 setting.

			SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo(); 

			memcpy(&pAvatarInfo->sCharState.sCharStateBase.vCurLoc, &pTeleport->vNewLoc, sizeof(sVECTOR3));
			memcpy(&pAvatarInfo->sCharState.sCharStateBase.vCurDir, &pTeleport->vNewDir, sizeof(sVECTOR3));

			GetDboGlobal()->GetGamePacketGenerator()->SendEnterWorld();
			
			if(pAvatarInfo->sWorldInfo.worldID == pTeleport->sWorldInfo.worldID)
			{
				RwV3d vNewPos;
				vNewPos.x = pTeleport->vNewLoc.x;
				vNewPos.y = pTeleport->vNewLoc.y;
				vNewPos.z = pTeleport->vNewLoc.z;

				RwV3d vPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
				RwV3d vSub;
				RwV3dSubMacro(&vSub, &vNewPos, &vPos);
				vSub.y = 0.0f;

				RwReal fTeleportLen = RwV3dLength(&vSub);
				if(fTeleportLen < 100.0f)
				{
					SWorldHeightStuff sHStuff;
					Logic_GetWorldHeight(&vNewPos, sHStuff);

					vNewPos.y = sHStuff.fFinialHeight;
					GetNtlSLGlobal()->GetSobAvatar()->SetPosition(&vNewPos);

					API_GetSLPacketGenerator()->SendLoadingCompleteNfy();
					API_GetSLPacketGenerator()->SendCharSpawnReady();
				}
				else
				{
					CNtlSLEventGenerator::WorldChange(FALSE);				
				}
			}
			else
			{
				GetSceneManager()->SetVisiblePlanet(TRUE);
			}
		}
	}	
}

void PacketHandler_GSCharRevivalRes(void *pPacket)
{
	sGU_CHAR_REVIVAL_RES *pRescue = (sGU_CHAR_REVIVAL_RES*)pPacket;
	if(pRescue->wResultCode == GAME_SUCCESS)
	{
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_MAINWORLD_WITH_POPO");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_MAINWORLD" );
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_TLQ" );	
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_TMQ" );
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_PARTY_DUNGEON" );
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_DOJO" );
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_CCBD");
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRescue->wResultCode, "GU_CHAR_REVIVAL_RES"), TRUE);
	}
}

void PacketHandler_GSCharCrescentPopoRevivalRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CHAR_CRESCENT_POPO_REVIVAL_RES);

	sGU_CHAR_CRESCENT_POPO_REVIVAL_RES *pRescue = (sGU_CHAR_CRESCENT_POPO_REVIVAL_RES*)pPacket;

	if (pRescue->wResultCode == GAME_SUCCESS)
	{
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_MAINWORLD_WITH_POPO");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_MAINWORLD");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_TLQ");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_TMQ");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_PARTY_DUNGEON");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_DOJO");
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_CCBD");
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRescue->wResultCode, "GU_CHAR_CRESCENT_POPO_REVIVAL_RES"), TRUE);
	}
}


void PacketHandler_GSItemCreate(void *pPacket)
{
	sGU_ITEM_CREATE *pItemCreate = (sGU_ITEM_CREATE*)pPacket;
	sITEM_DATA *pItemData = &pItemCreate->sItemData;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobItemAdd(pSobAvatar->GetSerialID(), pItemCreate->handle, pItemData->itemNo, &pItemData->sOptionSet, pItemData->byPlace, pItemData->byPosition,
		pItemData->byStackcount, pItemData->byRank, pItemData->byGrade, pItemData->byCurrentDurability, pItemData->byCurrentDurability, pItemData->bNeedToIdentify, pItemData->byBattleAttribute,
		pItemData->byRestrictState, pItemData->awchMaker, pItemData->byDurationType, pItemData->nUseStartTime, pItemData->nUseEndTime);
	
	if( pItemCreate->bIsNew )
	{
		if( pItemData->byPlace >= CONTAINER_TYPE_BAG_FIRST && pItemData->byPlace <= CONTAINER_TYPE_BAG_LAST )
			CDboEventGenerator::ItemCreatedEffect( pItemCreate->handle, (BYTE)(pItemData->byPlace - CONTAINER_TYPE_BAG1), pItemData->byPosition );

		if( pItemData->bNeedToIdentify )
			GetAlarmManager()->AlarmMessage( "DST_NOTIFY_GET_UNIDENTIFIED_ITEM" );
		else
		{
			GetAlarmManager()->FormattedAlarmMessage("DST_NOTIFY_GET_ITEM", FALSE, NULL, pItemData->byStackcount, Logic_GetItemName(pItemData->itemNo));
		}
	}
}

void PacketHandler_GSItemReplace(void *pPacket)
{
	sGU_ITEM_REPLACE* pItemReplace = (sGU_ITEM_REPLACE*)pPacket;
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	// 삭제해주기 전에 Table을 가지고 와서 확인해야 한다.
	sITEM_TBLDAT* pItemTblDat = Logic_GetItemDataFromSob( pItemReplace->hDeleteItem );
	
	// Gamble
	if( pItemTblDat->byItem_Type == ITEM_TYPE_GAMBLE )
	{
		// 연출 플래쉬의 이름
		CHAR acBuffer[DBO_MAX_LENGTH_ITEM_ICON_NAME + 1];
		sprintf_s( acBuffer, DBO_MAX_LENGTH_ITEM_ICON_NAME + 1, "%s", pItemTblDat->szIcon_Name );
		CHAR* pcNewExtension = ".swf";
		CHAR* pExtensionPos = strstr( acBuffer, ".png" );
		if( pExtensionPos == NULL )
		{
			NTL_ASSERTFAIL( "PacketHandler_GSItemReplace - invalid icon extension name" << pItemTblDat->szIcon_Name );
			return;
		}

		// 확장자 .png -> .swf 변경
		memcpy( pExtensionPos, pcNewExtension, strlen(pcNewExtension) * sizeof(CHAR) );

		// 파일 이름 앞에 인식자 TB_Boom_
		CHAR acBuffer2[DBO_MAX_LENGTH_ITEM_ICON_NAME +1 ];
		sprintf_s( acBuffer2, DBO_MAX_LENGTH_ITEM_ICON_NAME+1, "TB_Boom_%s", acBuffer );

		// Delete item
		if( pItemReplace->byDeleteItemPlace >= CONTAINER_TYPE_BANK1 && pItemReplace->byDeleteItemPlace <= CONTAINER_TYPE_BANK4 )
		{
			// 창고의 아이템을 지운다
			CNtlSLEventGenerator::SobWarehouseItemDelete(pSobAvatar->GetSerialID(), pItemReplace->hDeleteItem, pItemReplace->byDeleteItemPlace, pItemReplace->byDeleteItemPos);
		}
		else
		{		
			CNtlSLEventGenerator::SobItemDelete(pSobAvatar->GetSerialID(), pItemReplace->hDeleteItem, pItemReplace->byDeleteItemPlace, pItemReplace->byDeleteItemPos);
		}

		// Create item
		sITEM_DATA* pITEM_DATA = &pItemReplace->sCreateItemData;
		CNtlSLEventGenerator::SobItemAdd(pSobAvatar->GetSerialID(), pItemReplace->hCreateItem, pITEM_DATA->itemNo,
			&pITEM_DATA->sOptionSet, pItemReplace->byDeleteItemPlace,
			pITEM_DATA->byPosition,	pITEM_DATA->byStackcount, pITEM_DATA->byRank,
			pITEM_DATA->byGrade, pITEM_DATA->byCurrentDurability, pITEM_DATA->byCurrentDurability,
			pITEM_DATA->bNeedToIdentify, pITEM_DATA->byBattleAttribute,
			pITEM_DATA->byRestrictState, pITEM_DATA->awchMaker, pITEM_DATA->byDurationType, pITEM_DATA->nUseStartTime, pITEM_DATA->nUseEndTime);

		// Gamble effect
		CDboEventGenerator::GambleUseEffect( (RwUInt8)(pItemReplace->byDeleteItemPlace - CONTAINER_TYPE_BAG1), (RwUInt8)(pItemReplace->byDeleteItemPos),
			pItemReplace->hCreateItem, acBuffer2 );
	}
	// others
	else
	{
		// Delete item
		if( pItemReplace->byDeleteItemPlace >= CONTAINER_TYPE_BANK1 && pItemReplace->byDeleteItemPlace <= CONTAINER_TYPE_BANK4 )
		{
			// 창고의 아이템을 지운다
			CNtlSLEventGenerator::SobWarehouseItemDelete(pSobAvatar->GetSerialID(), pItemReplace->hDeleteItem, pItemReplace->byDeleteItemPlace, pItemReplace->byDeleteItemPos);
		}
		else
		{		
			CNtlSLEventGenerator::SobItemDelete(pSobAvatar->GetSerialID(), pItemReplace->hDeleteItem, pItemReplace->byDeleteItemPlace, pItemReplace->byDeleteItemPos);
		}

		// Create item
		sITEM_DATA* pITEM_DATA = &pItemReplace->sCreateItemData;
		CNtlSLEventGenerator::SobItemAdd(pSobAvatar->GetSerialID(), pItemReplace->hCreateItem, pITEM_DATA->itemNo,
			&pITEM_DATA->sOptionSet, pItemReplace->byDeleteItemPlace,
			pITEM_DATA->byPosition,	pITEM_DATA->byStackcount, pITEM_DATA->byRank,
			pITEM_DATA->byGrade, pITEM_DATA->byCurrentDurability, pITEM_DATA->byCurrentDurability,
			pITEM_DATA->bNeedToIdentify, pITEM_DATA->byBattleAttribute, 
			pITEM_DATA->byRestrictState, pITEM_DATA->awchMaker, pITEM_DATA->byDurationType, pITEM_DATA->nUseStartTime, pITEM_DATA->nUseEndTime );

		if( pItemReplace->bIsNew )
		{
			if( pItemReplace->byDeleteItemPlace >= CONTAINER_TYPE_BAG_FIRST && pItemReplace->byDeleteItemPlace <= CONTAINER_TYPE_BAG_LAST )
				CDboEventGenerator::ItemCreatedEffect( pItemReplace->hCreateItem, (RwUInt8)(pItemReplace->byDeleteItemPlace - CONTAINER_TYPE_BAG1), pItemReplace->byDeleteItemPos );

			if( pITEM_DATA->bNeedToIdentify )
				GetAlarmManager()->AlarmMessage( "DST_NOTIFY_GET_UNIDENTIFIED_ITEM" );
			else
			{
				GetAlarmManager()->FormattedAlarmMessage("DST_NOTIFY_GET_ITEM", FALSE, NULL, pITEM_DATA->byStackcount, Logic_GetItemName(pItemReplace->hCreateItem));
			}
		}
	}
}

void PacketHandler_GSItemDelete(void *pPacket)
{
	sGU_ITEM_DELETE *pItemDelRes = (sGU_ITEM_DELETE*)pPacket;	

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if( pItemDelRes->bySrcPlace >= CONTAINER_TYPE_BANK1 && pItemDelRes->bySrcPlace <= CONTAINER_TYPE_BANK4 )
	{
		// 창고의 아이템을 지운다
		CNtlSLEventGenerator::SobWarehouseItemDelete(pSobAvatar->GetSerialID(), pItemDelRes->hSrcItem, pItemDelRes->bySrcPlace, pItemDelRes->bySrcPos);
	}
	else
	{		
		CNtlSLEventGenerator::SobItemDelete(pSobAvatar->GetSerialID(), pItemDelRes->hSrcItem, pItemDelRes->bySrcPlace, pItemDelRes->bySrcPos);
	}
}

void PacketHandler_GSItemDeleteRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_DELETE_RES );

	sGU_ITEM_DELETE_RES *pItemDelRes = (sGU_ITEM_DELETE_RES*)pPacket;

	if(pItemDelRes->wResultCode != GAME_SUCCESS)
	{
		Logic_EnableIcon( TRUE, pItemDelRes->byPlace, pItemDelRes->byPos );	
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pItemDelRes->wResultCode, "").c_str());
		return;
	}
}

void PacketHandler_GSItemMoveRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_MOVE_RES );

	sGU_ITEM_MOVE_RES *pItemMoveRes = (sGU_ITEM_MOVE_RES*)pPacket;

	if(pItemMoveRes->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pItemMoveRes->wResultCode, "").c_str());
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMoveRes->hSrcItem ) );
	if( pItem )
	{
		CNtlSobItemAttr*	pItemAttr		= reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		sITEM_TBLDAT*		pITEM_TBLDAT	= pItemAttr->GetItemTbl();

		if( pItemMoveRes->byDestPlace == CONTAINER_TYPE_EQUIP )
			CNtlSLEventGenerator::TSItemEquip( pITEM_TBLDAT->tblidx );

		Logic_PlayItemPutDownSound(pITEM_TBLDAT,pItemMoveRes->byDestPlace);
	}
	else
	{
		DBO_FAIL("Not exist sob item of handle : " << pItemMoveRes->hSrcItem);
	}
	

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	CNtlSLEventGenerator::SobAvatarItemMove(pSobAvatar->GetSerialID(), pItemMoveRes->hSrcItem, pItemMoveRes->bySrcPlace, pItemMoveRes->bySrcPos,
		pItemMoveRes->hDestItem, pItemMoveRes->byDestPlace, pItemMoveRes->byDestPos);   

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void PacketHandler_GSItemStackMoveRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_MOVE_STACK_RES );

	sGU_ITEM_MOVE_STACK_RES *pItemStackMoveRes = (sGU_ITEM_MOVE_STACK_RES*)pPacket;

	if(pItemStackMoveRes->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pItemStackMoveRes->wResultCode, "").c_str());
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMoveRes->hSrcItem ) );
	if( pItem )
	{
		CNtlSobItemAttr*	pItemAttr		= reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		sITEM_TBLDAT*		pITEM_TBLDAT	= pItemAttr->GetItemTbl();

		Logic_PlayItemPutDownSound(pITEM_TBLDAT, pItemStackMoveRes->byDestPlace);
	}
	else
	{
		DBO_FAIL("Not exist sob item of handle : " << pItemStackMoveRes->hSrcItem);
	}

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobAvatarItemStackMove(pSobAvatar->GetSerialID(), pItemStackMoveRes->hSrcItem, pItemStackMoveRes->bySrcPlace, pItemStackMoveRes->bySrcPos, pItemStackMoveRes->byStackCount1,
		pItemStackMoveRes->hDestItem, pItemStackMoveRes->byDestPlace, pItemStackMoveRes->byDestPos, pItemStackMoveRes->byStackCount2);

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void PacketHandler_GSItemUpdate(void *pPacket)
{
	sGU_ITEM_UPDATE *pItemUpdateRes = (sGU_ITEM_UPDATE*)pPacket;
	sITEM_DATA* pItemData = &pItemUpdateRes->sItemData;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobItemUpdate(pSobAvatar->GetSerialID(), pItemUpdateRes->handle, pItemData->itemNo, &pItemData->sOptionSet, pItemData->byPlace, pItemData->byPosition,
		pItemData->byStackcount, pItemData->byRank, pItemData->byGrade, pItemData->byCurrentDurability, pItemData->byCurrentDurability, pItemData->bNeedToIdentify, pItemData->byBattleAttribute,
		pItemData->byRestrictState, pItemData->awchMaker, pItemData->byDurationType, pItemData->nUseStartTime, pItemData->nUseEndTime );
}

void PacketHandler_GSItemRepair(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_REPAIR_RES);

	sGU_ITEM_REPAIR_RES* pResult = (sGU_ITEM_REPAIR_RES*)pPacket;
	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_REPAIR_RES"), TRUE );
		return;
	}

	Logic_PlayGUISound(GSD_SYSTEM_ITEM_REPAIR);

	sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(pResult->hItem);
	if( pITEM_TBLDAT )
	{
		CTextTable*		pItemTextTable	= API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
		std::wstring&	strItemName		= pItemTextTable->GetText(pITEM_TBLDAT->Name);		

		GetAlarmManager()->FormattedAlarmMessage("DST_REPAIR_ITEM", FALSE, NULL, Logic_FormatZeni(pResult->dwSpendedZenny), strItemName.c_str());
	}
	else
	{
		DBO_FAIL("Not exist item table of item handle : " << pResult->hItem);
	}

	CDboEventGenerator::DialogEvent(DIALOGEVENT_REPAIR, DIALOG_UNKNOWN, DIALOG_NPCSHOP);
}

void PacketHandler_GSItemRepairAll(void *pPacket)
{
	// 서버로부터 응답을 받았다
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_EQUIP_REPAIR_RES);

	sGU_ITEM_EQUIP_REPAIR_RES* pResult = (sGU_ITEM_EQUIP_REPAIR_RES*)pPacket;
	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_EQUIP_REPAIR_RES"), TRUE );
		return;
	}

	// %d 제니를 지불하고 모든 아이템을 수리하였습니다
	GetAlarmManager()->FormattedAlarmMessage("DST_REPAIR_ALL_ITEM", FALSE, NULL, Logic_FormatZeni(pResult->dwSpendedZenny));

	CDboEventGenerator::DialogEvent(DIALOGEVENT_REPAIR_ALL, DIALOG_UNKNOWN, DIALOG_NPCSHOP);
}

void PacketHandler_GSItemUseRes(void *pPacket)
{
	sGU_ITEM_USE_RES *pItemUseRes = (sGU_ITEM_USE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_ITEM_USE_RES);

	if(pItemUseRes->wResultCode != GAME_SUCCESS)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
		CNtlSLEventGenerator::SysMsg(pSobAvatar->GetSerialID(), Logic_GetResultCodeString(pItemUseRes->wResultCode, "GU_ITEM_USE_RES").c_str());

		// It informs that the use of the item has failed.
		CNtlSLEventGenerator::SobItemUseResFailed(pSobAvatar->GetSerialID());

		return;
	}

	// If you succeeded in using items
	sITEM_TBLDAT* pItemData = reinterpret_cast<sITEM_TBLDAT*>(API_GetTableContainer()->GetItemTable()->FindData(pItemUseRes->tblidxItem));
	if( pItemData )
	{
		if( pItemData->byItem_Type == ITEM_TYPE_CAPSULE )
			CDboEventGenerator::CapsuleLockItem( TRUE, PLACE_BAG, pItemUseRes->byPos, Logic_ConvertContainerTypeToBagIdx(pItemUseRes->byPlace));
	}

	Logic_PlayGUISound(GSD_SYSTEM_ITEM_SPEND);
}


void PacketHandler_GSItemCancel(void *pPacket)
{
	sGU_ITEM_CASTING_CANCELED *pItemCancel = (sGU_ITEM_CASTING_CANCELED*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pItemCancel->handle == pSobAvatar->GetSerialID())
		CNtlSLEventGenerator::SysMsg(pSobAvatar->GetSerialID(), Logic_GetResultCodeString(pItemCancel->wResultCode, "").c_str());

	CNtlSLEventGenerator::SobItemCancel(pItemCancel->handle);
}

void PacketHandler_GSItemCastingDelayed(void *pPacket)
{
	sGU_ITEM_CASTING_DELAYED_NFY *pItemCastingDelayed = (sGU_ITEM_CASTING_DELAYED_NFY*)pPacket;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pItemCastingDelayed->handle) );

	CNtlBeCharData *pBeData			= reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff		= pBeData->GetSkillStuff();
	pSkillStuff->fCurrCastingTime	= (RwReal)(pItemCastingDelayed->dwCastingTime - pItemCastingDelayed->dwCastingTimeRemaining)/1000.f;
	pSkillStuff->fMaxCastingTime	= (RwReal)pItemCastingDelayed->dwCastingTime/1000.0f;
}

void PacketHandler_GSItemAction(void *pPacket)
{
	sGU_CHAR_ACTION_ITEM *pItemAction = (sGU_CHAR_ACTION_ITEM*)pPacket;
	if(pItemAction->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(pItemAction->handle, Logic_GetResultCodeString(pItemAction->wResultCode, "").c_str());
		return;
	}

	CItemTable *pItemTable = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData(pItemAction->itemTblidx) );

	CUseItemTable *pUseItemTbl = API_GetTableContainer()->GetUseItemTable();
	sUSE_ITEM_TBLDAT *pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTbl->FindData(pItemTblData->Use_Item_Tblidx) );
	NTL_ASSERT(pUseItemTblData, "PacketHandler_GSItemAction => use item table data not found" << "(" << pItemTblData->Use_Item_Tblidx << ")");

	CNtlSLEventGenerator::SobItemUseAction( pItemAction->handle, pItemTblData->Use_Item_Tblidx, pItemAction->bySkillResultCount, pItemAction->aSkillResult );

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pItemAction->handle);
	if(pSobObj == NULL)
		return;

	if(pUseItemTblData->wAction_Animation_Index == INVALID_ANIMATIONID)
	{
		if(strlen(pUseItemTblData->szAction_Effect) > 0)
		{
			RwV3d vOffset;
			RwReal fHeight = (RwReal)pUseItemTblData->byActionEffectPosition/(RwReal)100;
			CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, fHeight, 0.0f);
			pSobObj->GetSobProxy()->CreatePLChildEffect(pUseItemTblData->szAction_Effect, vOffset);
		}
		DBO_WARNING_MESSAGE("pUseItemTblData->wAction_Animation_Index.. Idx: " << pItemTblData->tblidx);
		// I will process it later.
		/*
		CSystemEffectTable *pSystemEffTbl = pGameTbl->GetSystemEffectTbl();

		sSYSTEM_EFFECT_TBLDAT *pSystemEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pUseItemTblData->->FindData(pEffAffected->effectTblidx) );
		NTL_ASSERT( pSystemEffTblData, "PacketHandler_GSEffectAffected => find not system effect table id ( " << pEffAffected->effectTblidx << ")" );

		RwBool bHealing = FALSE;
		RwUInt8 byEffectType = pSystemEffTblData->byActive_Effect_Type;
		if(byEffectType == SKILL_ACTIVE_TYPE_DH || byEffectType == SKILL_ACTIVE_TYPE_HOT)
		{
		bHealing = TRUE;
		}

		CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
		NTL_ASSERT(pSobProxy, "PacketHandler_GSEffectAffected => sob proxy instance is null" << "sob class id( " << pSobObj->GetClassID() << " )" );
		RwV3d vPos = pSobObj->GetPosition();
		pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pEffAffected->fActualArgument, bHealing);

		if(pSystemEffTblData->strKeep_Effect_Name.size() > 0)
		{
		vPos.y += pSobProxy->GetPLEntityHeight() * (RwReal)pSystemEffTblData->byTarget_Effect_Position / 100.0f;
		pSobProxy->CreatePLChildEffect(pSystemEffTblData->strKeep_Effect_Name.c_str(), vPos);
		}
		*/
	}
	else
	{
		if(pItemAction->bySkillResultCount > 0)
		{
			CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pItemAction->aSkillResult[0].hTarget);
			if(pTarSobObj)
			{
				RwV3d vPos	= pSobObj->GetPosition();
				RwV3d vTarPos = pTarSobObj->GetPosition();

				RwV3d vDir;
				RwV3dSubMacro(&vDir, &vTarPos, &vPos);
				RwV3dNormalize(&vDir, &vDir);

				pSobObj->SetDirection(&vDir);
			}
		}
		
		pSobObj->GetSobProxy()->SetBaseAnimation(pUseItemTblData->wAction_Animation_Index, FALSE);
	}

	// Quest Item Use Event occurrence
	CNtlSLEventGenerator::TSItemUse( pItemAction );
}

void PacketHandler_GSItemDurDown(void *pPacket)
{
	sGU_ITEM_DUR_DOWN *pItemDurDown = (sGU_ITEM_DUR_DOWN*)pPacket;

	CNtlSLEventGenerator::SobEquipItemDurDown( pItemDurDown->byDur );
}

void PacketHandler_GSItemDurUpdate(void *pPacket)
{
	sGU_ITEM_DUR_UPDATE* pResult = (sGU_ITEM_DUR_UPDATE*)pPacket;

	CNtlSLEventGenerator::SobEquipItemDurUpdate( pResult->hItemHandle, pResult->byDur );	
}

void PacketHandler_GSItemStackUpdate(void *pPacket)
{
	sGU_ITEM_STACK_UPDATE* pResult = (sGU_ITEM_STACK_UPDATE*)pPacket;

	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pResult->hItemHandle ) );
	NTL_ASSERT( pSobItem, "PacketHanler_GSItemUseRes : Item must be present" );

	Logic_SetItemStack( pSobItem, pResult->byStack );

	if( pResult->bIsNew )
	{
		RwUInt8 byBagIndex = (RwUInt8)pSobItem->GetParentItemSlotIdx();
		if( byBagIndex >= 0 && byBagIndex < NTL_MAX_BAGSLOT_COUNT )
			CDboEventGenerator::ItemCreatedEffect( pResult->hItemHandle, byBagIndex, (RwUInt8)pSobItem->GetItemSlotIdx() );
	}
}

void PacketHandler_GSItemUpgradeRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_UPGRADE_RES );

	sGU_ITEM_UPGRADE_RES* pUpgrade = (sGU_ITEM_UPGRADE_RES*)pPacket;

	if( pUpgrade->wResultCode == GAME_SUCCESS)
	{
		bool bDestroyItem = false;

		if( pUpgrade->wResultMessageCode == ITEM_UPGRADE_RESULT_DESTROY)
		{
			bDestroyItem = true;
		}

		CDboEventGenerator::ItemUpgradeResult(pUpgrade->wResultCode, pUpgrade->byItemGrade, pUpgrade->wResultMessageCode, pUpgrade->byStoneStack, pUpgrade->byCoreStack);
		CNtlSLEventGenerator::SobItemUpgrade( pUpgrade->byItemGrade, pUpgrade->byItemPlace, pUpgrade->byItemPos, pUpgrade->byStonePlace,
											  pUpgrade->byStonePos, pUpgrade->byStoneStack, pUpgrade->byCorePlace, pUpgrade->byCorePos, pUpgrade->byCoreStack, bDestroyItem );

		// When Item upgrade succeeds, it sends event to TS
		if (pUpgrade->wResultCode == ITEM_UPGRADE_RESULT_SUCCESS)
		{
			CNtlSLEventGenerator::TSItemUpgrade();
		}
	}
	else
	{
		CDboEventGenerator::ItemUpgradeResult(pUpgrade->wResultCode, pUpgrade->byItemGrade, pUpgrade->wResultMessageCode, pUpgrade->byStoneStack, pUpgrade->byCoreStack);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pUpgrade->wResultCode, "GU_ITEM_UPGRADE_RES"), TRUE );
	}
}

void Packethandler_GSItemUpgradeAllNfy(void *pPacket)
{
	sGU_ITEM_UPGRADE_ALL_NFY* pUpgrade = (sGU_ITEM_UPGRADE_ALL_NFY*)pPacket;

	CNtlSLEventGenerator::SobEquipItemUpgradeAll( NTL_ITEM_UPGRADE_EQUIP_COUNT, pUpgrade->abyGrade );
}

void PacketHandler_GSItemChangeOptionRes(void * pPacket)
{
	sGU_ITEM_CHANGE_OPTION_RES* pRes = (sGU_ITEM_CHANGE_OPTION_RES*)pPacket;

	if (pRes->wResultCode == GAME_SUCCESS)
	{
		CDboEventGenerator::ItemChangeOptionResult((void*)&pRes->sItemOptionSet, pRes->byKitRemainStack);

		CNtlSLEventGenerator::SobItemChangeOptionSet((void*)&pRes->sItemOptionSet, pRes->byItemPlace, pRes->byItemPos, pRes->byKitPlace, pRes->byKitPos, pRes->byKitRemainStack);
	}
	else
	{
		CDboEventGenerator::ItemChangeOptionResult(NULL, pRes->byKitRemainStack);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_ITEM_CHANGE_OPTION_RES"), TRUE);
	}
}

void Packethandler_GSItemChangeBattleAttributeRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES);

	sGU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES* pUpgrade = (sGU_ITEM_CHANGE_BATTLE_ATTRIBUTE_RES*)pPacket;

	if (pUpgrade->wResultCode == GAME_SUCCESS)
	{
		CDboEventGenerator::ItemChangeBattleAttributeResult(pUpgrade->wResultCode, pUpgrade->byBattleAttribute);

		CNtlSLEventGenerator::SobItemChangeBattleAttribute(pUpgrade->byBattleAttribute, pUpgrade->byItemPlace, pUpgrade->byItemPos);
	}
	else
	{
		CDboEventGenerator::ItemChangeBattleAttributeResult(pUpgrade->wResultCode);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pUpgrade->wResultCode, "GU_ITEM_UPGRADE_RES"), TRUE);
	}
}

void Packethandler_QuickTeleportLoadRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_QUICK_TELEPORT_LOAD_RES);

	sGU_QUICK_TELEPORT_LOAD_RES* pRes = (sGU_QUICK_TELEPORT_LOAD_RES*)pPacket;

	if (pRes->wResultCode == GAME_SUCCESS)
	{
		// event
		CDboEventGenerator::EventQuickTeleportLoad(pRes->byCount, pRes->asData);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_QUICK_TELEPORT_LOAD_RES"), TRUE);
	}
}

void Packethandler_QuickTeleportUpdateRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_QUICK_TELEPORT_UPDATE_RES);

	sGU_QUICK_TELEPORT_UPDATE_RES* pRes = (sGU_QUICK_TELEPORT_UPDATE_RES*)pPacket;

	if (pRes->wResultCode == GAME_SUCCESS)
	{
		// event
		CDboEventGenerator::EventQuickTeleportUpdate(&pRes->aData);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_QUICK_TELEPORT_UPDATE_RES"), TRUE);
	}
}

void Packethandler_QuickTeleportDelRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_QUICK_TELEPORT_DEL_RES);

	sGU_QUICK_TELEPORT_DEL_RES* pRes = (sGU_QUICK_TELEPORT_DEL_RES*)pPacket;

	if (pRes->wResultCode == GAME_SUCCESS)
	{
		// event
		CDboEventGenerator::EventQuickTeleportDelete(pRes->bySlot);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_QUICK_TELEPORT_DEL_RES"), TRUE);
	}
}

void Packethandler_QuickTeleportUseRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_QUICK_TELEPORT_USE_RES);

	sGU_QUICK_TELEPORT_USE_RES* pRes = (sGU_QUICK_TELEPORT_USE_RES*)pPacket;

	if (pRes->wResultCode == GAME_SUCCESS)
	{
		// event
		CDboEventGenerator::EventQuickTeleportMove(pRes->bySlot);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_QUICK_TELEPORT_USE_RES"), TRUE);
	}
}

void PacketHandler_GSItemLuckyDrop(void *pPacket)
{
	sGU_MOB_LUCKY_DROP_NFY* pLuckyDrop = (sGU_MOB_LUCKY_DROP_NFY*)pPacket;
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pLuckyDrop->hMobhandle);
	if(pSobObj)
	{
		RwV3d vPos = pSobObj->GetPosition();
		CNtlPLEntity *pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, NTL_VID_LUCKY_DROP);
		if(pPLEntity)
			pPLEntity->SetPosition(&vPos);

		Logic_PlayGUISound(GSD_SYSTEM_DROP_LUCKY);
	}
}

void PacketHandler_GSItemSealRes(void * pPacket)
{
	sGU_ITEM_SEAL_RES* pResult = (sGU_ITEM_SEAL_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_ITEM_SEAL_RES);

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_SEAL_RES"), TRUE);
		return;
	}

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pInventory->GetBagItem(pResult->bySealPlace - CONTAINER_TYPE_BAG1))); // convert item place to bag slot
	NTL_ASSERT(pBagItem, "PacketHandler_GSItemSealRes Bag is must present");

	CNtlSobItem* pItem = pBagItem->GetChildItem(pResult->bySealPos);
	NTL_ASSERT(pItem, "PacketHandler_GSItemSealRes Item must present");

	Logic_SetItemStack(pItem, pResult->bySealStack);
	
	CNtlSLEventGenerator::SobItemRestrictUpdate(pResult->byItemPlace, pResult->byItemPos, pResult->byRestrictState);
}

void PacketHandler_GSItemSealExtractRes(void * pPacket)
{
	sGU_ITEM_SEAL_EXTRACT_RES* pResult = (sGU_ITEM_SEAL_EXTRACT_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_ITEM_SEAL_EXTRACT_RES);

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_SEAL_EXTRACT_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobItemRestrictUpdate(pResult->byItemPlace, pResult->byItemPos, pResult->byRestrictState);
}

void PacketHandler_GSPartySelectStateNfy(void * pPacket)
{
	sGU_PARTY_SELECT_STATE_NFY* pResult = (sGU_PARTY_SELECT_STATE_NFY*)pPacket;

	// event
	CNtlSLEventGenerator::EventPartySelectState(pResult->hTarget, pResult->bySelectState);
}

void PacketHandler_GSPartySelectStateRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_PARTY_SELECT_STATE_RES);

	sGU_PARTY_SELECT_STATE_RES* pResult = (sGU_PARTY_SELECT_STATE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PARTY_SELECT_STATE_RES"));
		return;
	}

	// event
	CNtlSLEventGenerator::EventPartySelectState(Logic_GetAvatarHandle(), pResult->bySelectState);
}

void PacketHandler_GSPartySelectStateInitNfy(void * pPacket)
{
	sGU_PARTY_SELECT_STATE_INIT_NFY* pResult = (sGU_PARTY_SELECT_STATE_INIT_NFY*)pPacket;

	// event
	CNtlSLEventGenerator::EventPartySelectStateInit(pResult->bySelectState, pResult->dwParam, pResult->bLastStage);
}


void PacketHandler_GSUltimateDungeonEnterRes(void * pPacket)
{
	sGU_ULTIMATE_DUNGEON_ENTER_RES* pResult = (sGU_ULTIMATE_DUNGEON_ENTER_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_ULTIMATE_DUNGEON_ENTER_RES);

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ULTIMATE_DUNGEON_ENTER_RES"), TRUE);
		return;
	}

	// Event
	CNtlSLEventGenerator::SLUDEnterRes();
}

void PacketHandler_GSBattleDungeonEnterRes(void * pPacket)
{
	sGU_BATTLE_DUNGEON_ENTER_RES* pResult = (sGU_BATTLE_DUNGEON_ENTER_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_BATTLE_DUNGEON_ENTER_RES);

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BATTLE_DUNGEON_ENTER_RES"), TRUE);
		return;
	}
}

void PacketHandler_GSBattleDungeonLeaveRes(void * pPacket)
{
	sGU_BATTLE_DUNGEON_LEAVE_RES* pResult = (sGU_BATTLE_DUNGEON_LEAVE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_BATTLE_DUNGEON_LEAVE_RES);

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BATTLE_DUNGEON_LEAVE_RES"), TRUE);
		return;
	}
}

void PacketHandler_GSBattleDungeonStateUpdateNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_STATE_UPATE_NFY* pResult = (sGU_BATTLE_DUNGEON_STATE_UPATE_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonStateUpdate(pResult->byStage, pResult->titleTblidx, pResult->subTitleTblidx, pResult->dwLimitTime);
}

void PacketHandler_GSBattleDungeonStageClearNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_STAGE_CLEAR_NFY* pResult = (sGU_BATTLE_DUNGEON_STAGE_CLEAR_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonStageClear();
}

void PacketHandler_GSBattleDungeonLimitTimeStartNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY* pResult = (sGU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonLimitTime(pResult->dwLimitTime, true);
}

void PacketHandler_GSBattleDungeonLimitTimeEndNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY* pResult = (sGU_BATTLE_DUNGEON_LIMIT_TIME_END_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonLimitTime(0, false);
}

void PacketHandler_GSBattleDungeonFailNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_FAIL_NFY* pResult = (sGU_BATTLE_DUNGEON_FAIL_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonFail();
}

void PacketHandler_GSBattleDungeonRewardFinishNfy(void * pPacket)
{
	sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY* pResult = (sGU_BATTLE_DUNGEON_REWARD_FINISH_NFY*)pPacket;

	// Event
	CNtlSLEventGenerator::EventBattleDungeonRewardFinish();
}

void PacketHandler_GSQuestItemCreate(void *pPacket)
{
	sGU_QUEST_ITEM_CREATE_NFY* pCreate = (sGU_QUEST_ITEM_CREATE_NFY*)pPacket;

	CNtlSLEventGenerator::SobQuestItemAdd( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pCreate->qItemTblidx, pCreate->byPos, pCreate->byCurCount );

	// 메세지 출력
	std::wstring wstrName;	
	CTextTable* pQuestItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();
	sQUESTITEM_TBLDAT* pQuestItemData = reinterpret_cast<sQUESTITEM_TBLDAT*>( API_GetTableContainer()->GetQuestItemTable()->FindData( pCreate->qItemTblidx ) );
	pQuestItemTextTable->GetText( pQuestItemData->ItemName, &wstrName );
	GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_QUEST_ITEM", FALSE, NULL, wstrName.c_str(), pCreate->byCurCount);

	Logic_PlayGUISound(GSD_QUEST_ITEM_GET);
}

void PacketHandler_GSQuestItemUpdateNfy(void *pPacket)
{
	sGU_QUEST_ITEM_UPDATE_NFY* pUpdate = (sGU_QUEST_ITEM_UPDATE_NFY*)pPacket;

	// 메세지 출력
	std::wstring wstrName;
	CTextTable* pQuestItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetQuestItemTbl();
	CNtlSobQuestItemAttr* pQuestItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetQuestInventory()->GetQuestItemFromIdx( pUpdate->byPos )->GetSobAttr() );
	if( pQuestItemAttr->GetStackNum() < pUpdate->byCurCount )
	{
		pQuestItemTextTable->GetText( pQuestItemAttr->GetQuestItemTbl()->ItemName, &wstrName );
		GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_QUEST_ITEM", FALSE, NULL, wstrName.c_str(), pUpdate->byCurCount, pQuestItemAttr->GetStackNum());
	}	

	CNtlSLEventGenerator::SobQuestItemUpdate( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pUpdate->byPos, pUpdate->byCurCount );	
}

void PacketHandler_GSQuestItemMoveRes(void *pPacket)
{
	sGU_QUEST_ITEM_MOVE_RES* pMove = (sGU_QUEST_ITEM_MOVE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_QUEST_ITEM_MOVE_RES );

	if( pMove->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), Logic_GetResultCodeString(pMove->wResultCode, "").c_str());
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CNtlSobQuestItem *pSobQuestItem = reinterpret_cast<CNtlSobQuestItem*>(pMove);

	if( pSobQuestItem)
	{
		CNtlSobQuestItemAttr *pSobQuestItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>(pSobQuestItem->GetSobAttr());
		sQUESTITEM_TBLDAT* pQuestItemTbl = pSobQuestItemAttr->GetQuestItemTbl();		
	}
	else
	{
		DBO_FAIL("Not exist sob Quest item of handle : " << pMove->dwSrcTblidx );
	}
	
	CNtlSLEventGenerator::SobAvatarQuestItemMove( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pMove->bySrcPos, pMove->dwSrcTblidx, pMove->byDestPos, pMove->dwDestTblidx );

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

void PacketHandler_GSQuestItemDeleteRes(void *pPacket)
{
	sGU_QUEST_ITEM_DELETE_RES* pDelete = (sGU_QUEST_ITEM_DELETE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_QUEST_ITEM_DELETE_RES );

	if( pDelete->wResultCode != GAME_SUCCESS )
	{
		CDboEventGenerator::EnableItemIcon( TRUE, PLACE_QUESTBAG, pDelete->byDeletePos );		
		CNtlSLEventGenerator::SysMsg( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), Logic_GetResultCodeString(pDelete->wResultCode, "").c_str());
	}
}

void PacketHandler_GSQuestItemDeleteNfy(void *pPacket)
{
	sGU_QUEST_ITEM_DELETE_NFY* pDelete = (sGU_QUEST_ITEM_DELETE_NFY*)pPacket;

	CNtlSLEventGenerator::SobQuestItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pDelete->byDeletePos );
}


void PacketHandler_GSWorldItemPickRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_PICK_RES );

	sGU_ITEM_PICK_RES *pWorldItemPickRes = (sGU_ITEM_PICK_RES*)pPacket;

	if(pWorldItemPickRes->wResultCode == GAME_SUCCESS)
	{
		if( pWorldItemPickRes->bByPartyHunting )
			return;
	
		Logic_PlayGUISound(GSD_SYSTEM_ITEM_GET);

		CNtlSLEventGenerator::TSItemGet( pWorldItemPickRes->itemTblidx );
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_LOOT_ITEM);
	}
	else
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pWorldItemPickRes->wResultCode, "").c_str());
	}
}

void PacketHandler_GSWorldZennyPickRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ZENNY_PICK_RES );

	sGU_ZENNY_PICK_RES *pWorldZennyPickRes = (sGU_ZENNY_PICK_RES*)pPacket;

	if(pWorldZennyPickRes->wResultCode == GAME_SUCCESS)
	{
		if( pWorldZennyPickRes->bSharedInParty )
		{
			// Gets Jenny by party distribution (%S out of %s)
			// This message comes from the party, not GAIN, when you get it at the party.

            if(pWorldZennyPickRes->dwBonusZenny > 0)
            {
                GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY_AND_BONUS", FALSE, NULL, Logic_FormatZeni(pWorldZennyPickRes->dwOriginalZenny), Logic_FormatZeni(pWorldZennyPickRes->dwAcquisitionZenny), Logic_FormatZeni(pWorldZennyPickRes->dwBonusZenny));
            }
            else
            {
                GetAlarmManager()->FormattedAlarmMessage("DST_PARTY_GET_ZENNY", FALSE, NULL, Logic_FormatZeni(pWorldZennyPickRes->dwOriginalZenny), Logic_FormatZeni(pWorldZennyPickRes->dwZenny));
            }			
		}
        else
        {
            if(pWorldZennyPickRes->dwBonusZenny > 0)        // PC bonus Zeni acquisition
            {
                GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_ZENNY_AND_BONUS", FALSE, NULL, Logic_FormatZeni(pWorldZennyPickRes->dwAcquisitionZenny), Logic_FormatZeni(pWorldZennyPickRes->dwBonusZenny) );
            }
            else    // Generic Zeni acquisition
            {
                GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_ZENNY", FALSE, NULL, Logic_FormatZeni(pWorldZennyPickRes->dwZenny) );
            }
        }
	}
	else
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pWorldZennyPickRes->wResultCode, "").c_str());
	}
}

void PacketHandler_GSCharDestMove(void *pPacket)
{
	sGU_CHAR_DEST_MOVE *pDestMove = (sGU_CHAR_DEST_MOVE*)pPacket;

	//// Dest location counter는 반드시 1 이상이어야 한다
	//if ( 0 == pDestMove->byDestLocCount )
	//{
	//	DBO_FAIL( "A dest location counter must be one more." );
	//	return;
	//}

	//// Dest location counter는 반드시 DBO_MAX_NEXT_DEST_LOC_COUNT 보다 작거나 같아야 한다
	//if ( pDestMove->byDestLocCount > DBO_MAX_NEXT_DEST_LOC_COUNT )
	//{
	//	DBO_FAIL( "A dest location counter must be less equal than DBO_MAX_NEXT_DEST_LOC_COUNT." );
	//	return;
	//}

	//if ( !Logic_IsBus( pDestMove->handle ) )
	//{
	//	RwV3d vCurrLoc;
	//	NtlLocationDecompress(&pDestMove->vCurLoc, &vCurrLoc.x, &vCurrLoc.y, &vCurrLoc.z);

	//	RwV3d* pNodeList = new RwV3d[ pDestMove->byDestLocCount ];

	//	if ( NULL == pNodeList )
	//	{
	//		DBO_FAIL( "Creating dest location nodes is failed." );
	//		return;
	//	}

	//	for ( int i = 0; i < (int)pDestMove->byDestLocCount; ++i )
	//	{
	//		NtlLocationDecompress(&pDestMove->avDestLoc[i], &pNodeList[i].x, &pNodeList[i].y, &pNodeList[i].z);
	//	}

	//	CNtlSLEventGenerator::SobDestMove( pDestMove->handle, pDestMove->byMoveFlag, vCurrLoc, pNodeList[0], 0, pDestMove->byDestLocCount, pNodeList );

	//	if ( pNodeList )
	//	{
	//		delete [] pNodeList;
	//	}
	//}
	//else
	//{
	//	RwV3d vCurrLoc, vDest;

	//	NtlLocationDecompress(&pDestMove->vCurLoc, &vCurrLoc.x, &vCurrLoc.y, &vCurrLoc.z);
	//	NtlLocationDecompress(&pDestMove->avDestLoc[0], &vDest.x, &vDest.y, &vDest.z);

	//	if ( pDestMove->bHaveSecondDestLoc )
	//	{
	//		RwV3d vSecondDest;

	//		NtlLocationDecompress(&pDestMove->vSecondDestLoc, &vSecondDest.x, &vSecondDest.y, &vSecondDest.z);

	//		CNtlSLEventGenerator::SobSecondDestMove( pDestMove->handle, pDestMove->byMoveFlag, vCurrLoc, vDest, vSecondDest, 0 );
	//	}
	//	else
	//	{
	//		CNtlSLEventGenerator::SobSecondDestMove( pDestMove->handle, pDestMove->byMoveFlag, vCurrLoc, vDest, vDest, 0 );
	//	}
	//}
}

void PacketHandler_GSCharMove(void *pPacket)
{
	/*sGU_CHAR_MOVE *pMove = (sGU_CHAR_MOVE*)pPacket;

	RwV3d vLoc, vDir;

	NtlLocationDecompress(&pMove->vCurLoc, &vLoc.x, &vLoc.y, &vLoc.z);
	NtlDirectionDecompress(&pMove->vCurDir, &vDir.x, &vDir.y, &vDir.z);

	if(pMove->byMoveDirection == NTL_MOVE_NONE)
		CNtlSLEventGenerator::SobMoveStop(pMove->handle, vLoc, vDir, 0);
	else
		CNtlSLEventGenerator::SobMove(pMove->handle, pMove->byMoveDirection, pMove->byMoveFlag, vLoc, vDir, 0);*/
}

void PacketHandler_GSCharMoveSync( void* pPacket ) 
{
   /* sGU_CHAR_MOVE_SYNC* pMoveSync = (sGU_CHAR_MOVE_SYNC*)pPacket;
    RwV3d vLoc, vDir;    
    SVector2RwV3d(vLoc, pMoveSync->vCurLoc);
    SVector2RwV3d(vDir, pMoveSync->vCurDir);

    CNtlSLEventGenerator::SobMoveSync(pMoveSync->handle, vLoc, vDir, pMoveSync->dwTimeStamp);*/
}

void PacketHandler_GSCharFollowMove(void *pPacket)
{
	sGU_CHAR_FOLLOW_MOVE *pFollowMove = (sGU_CHAR_FOLLOW_MOVE*)pPacket;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pFollowMove->handle);
	if(pSobObj == NULL)
		return;

	if(pSobObj->GetFlags() & SLFLAG_SERVER_SENDER)
		return;

	
	RwV3d vLoc;
	CNtlMath::MathRwV3dAssign(&vLoc, 0.0f, 0.0f, 0.0f);
	
	CNtlSLEventGenerator::SobFollowMove(pFollowMove->handle, pFollowMove->hTarget, pFollowMove->byMoveFlag, vLoc, pFollowMove->fDistance,
										pFollowMove->byMovementReason, pFollowMove->dwTimeStamp);
}

void PacketHandler_GSCharChangeHeading(void *pPacket)
{
	sGU_CHAR_CHANGE_HEADING *pChangeHeading = (sGU_CHAR_CHANGE_HEADING*)pPacket;
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pChangeHeading->handle);
	NTL_ASSERT(pSobObj, "CGamePacketHandler::CharChangeHeading => handle( " << pChangeHeading->handle << " ) is invalid" );
    if(!pSobObj)
        return;

	RwV3d vDir;

	NtlDirectionDecompress(&pChangeHeading->vNewHeading, &vDir.x, &vDir.y, &vDir.z);
	
	pSobObj->SetDirection(&vDir);

	//CNtlSLEventGenerator::SobAdjustMovePosition(pChangeHeading->handle, vPos);  // by daneos
}

void PacketHandler_GSCharDirectionFloat(void *pPacket)
{
	sGU_CHAR_CHANGE_DIRECTION_ON_FLOATING *pFloat = (sGU_CHAR_CHANGE_DIRECTION_ON_FLOATING*)pPacket;

	CNtlSLEventGenerator::SobDirectionFloat(pFloat->hSubject, pFloat->byMoveDirection);
}

void PacketHandler_GSCharNonBattleMovePatternSync(void *pPacket)
{
	sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY *pUnitIdxNfy = (sGU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY*)pPacket;

	Logic_GetMovePatternSync(pUnitIdxNfy->hBotHandle, pUnitIdxNfy->unitIdx);

	CNtlSLEventGenerator::SobMovePatternSync(pUnitIdxNfy->hBotHandle, pUnitIdxNfy->unitIdx);
}

void PacketHandler_GSCharFightingMode(void *pPacket)
{
	sGU_CHAR_FIGHTMODE *pFightingMode = (sGU_CHAR_FIGHTMODE*)pPacket;
	CNtlSLEventGenerator::SobFightingMode(pFightingMode->handle, pFightingMode->bFightMode);
}

void PacketHandler_GSCharActionAttack(void *pPacket)
{
	sGU_CHAR_ACTION_ATTACK *pActAttack = (sGU_CHAR_ACTION_ATTACK*)pPacket;

	CNtlSLEventGenerator::SobAttack(pActAttack->hSubject, pActAttack->hSubject, pActAttack->hTarget, pActAttack->bChainAttack, 
									pActAttack->byAttackResult, pActAttack->byAttackSequence, pActAttack->byBlockedAction,
									(RwUInt32)pActAttack->attackResultValue, 0, (RwUInt32)pActAttack->fReflectedDamage, pActAttack->lpEpRecovered,
									pActAttack->dwLpEpEventId);

	CNtlSLEventGenerator::SobAttacked(pActAttack->hTarget, pActAttack->hSubject);
}

void PacketHandler_GSCharSpecialAttack(void* pPacket)
{
	sGU_CHAR_SPECIAL_ATTACK_NFY* pAttack = (sGU_CHAR_SPECIAL_ATTACK_NFY*)pPacket;

	CNtlSLEventGenerator::SobSpecialAttack(pAttack->hAttacker, pAttack->hAttacker, pAttack->hTarget, pAttack->byAttackType,
		pAttack->bySourceType, pAttack->sourceTblidx, pAttack->byAttackResult,
		pAttack->attackResultValue);

	CNtlSLEventGenerator::SobAttacked(pAttack->hTarget, pAttack->hAttacker);
}

void PacketHandler_GSCharBlockingCoolTimeNfy(void *pPacket)
{
	sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY *pBlockingNft = (sGU_CHAR_BLOCK_MODE_COOL_TIME_NFY*)pPacket;

	CNtlSLEventGenerator::BlockingNfy(FALSE, pBlockingNft->dwCoolTimeRemaining);
}

void PacketHandler_GSCharGuardCrush(void *pPacket)
{
	sGU_CHAR_GUARD_CRASHED_NFY *pGuardCrush = (sGU_CHAR_GUARD_CRASHED_NFY*)pPacket;
	CNtlSLEventGenerator::SobGuardCrush(pGuardCrush->hSubject);
}


void PacketHandler_GSCharJump(void *pPacket)
{
	sGU_CHAR_JUMP *pJump = (sGU_CHAR_JUMP*)pPacket;

	RwV3d vDir,vJumpDir;

	NtlDirectionDecompress(&pJump->vCurrentHeading, &vDir.x, &vDir.y, &vDir.z);
	NtlDirectionDecompress(&pJump->vJumpDir, &vJumpDir.x, &vJumpDir.y, &vJumpDir.z);

	CNtlSLEventGenerator::SobJump(pJump->handle, pJump->byMoveDirection, vDir, vJumpDir);
}

void PacketHandler_GSCharJumpDirection(void *pPacket)
{
	sGU_CHAR_CHANGE_JUMP_DIRECTION *pJumpDirection = (sGU_CHAR_CHANGE_JUMP_DIRECTION*)pPacket;

	RwV3d vJumpDir;

	NtlDirectionDecompress(&pJumpDirection->vJumpDir, &vJumpDir.x, &vJumpDir.y, &vJumpDir.z);

	CNtlSLEventGenerator::SobJumpDirection(pJumpDirection->handle, vJumpDir, 0);
}

void PacketHandler_GSCharJumpEnd(void *pPacket)
{
	sGU_CHAR_JUMP_END *pJumpEnd = (sGU_CHAR_JUMP_END*)pPacket;

	CNtlSLEventGenerator::SobJumpEnd(pJumpEnd->handle);
}

void PacketHandler_GSCharSitDown(void *pPacket)
{
	/*sGU_CHAR_SITDOWN *pSitDown = (sGU_CHAR_SITDOWN*)pPacket;
	CNtlSLEventGenerator::SobSit(pSitDown->handle, TRUE);*/
}

void PacketHandler_GSCharStandUp(void *pPacket)
{
	/*sGU_CHAR_STANDUP *pStandUp = (sGU_CHAR_STANDUP*)pPacket;
	CNtlSLEventGenerator::SobSit(pStandUp->handle, FALSE);*/
}

void PacketHandler_GSCharSkillRes(void *pPacket)
{
	Logic_SetSkillReqFlag(FALSE);

	sGU_CHAR_SKILL_RES *pSkillRes = (sGU_CHAR_SKILL_RES*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	if(pSkillRes->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(pSobAvatar->GetSerialID(), Logic_GetResultCodeString(pSkillRes->wResultCode, "").c_str());
		CNtlSLEventGenerator::SobSkillCancel(pSobAvatar->GetSerialID(), pSkillRes->wResultCode);
	}
}

void PacketHandler_GSCharSkillCancel(void *pPacket)
{
	sGU_SKILL_CASTING_CANCELED_NFY *pSkillCancel = (sGU_SKILL_CASTING_CANCELED_NFY*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSkillCancel->handle == pSobAvatar->GetSerialID())
	{
//		CNtlSLEventGenerator::SysMsg(pSobAvatar->GetSerialID(),pSkillCancel->wResultCode);
	}

	CNtlSLEventGenerator::SobSkillCancel(pSkillCancel->handle, pSkillCancel->wResultCode);
}

void PacketHandler_GSCharSkillAffectingCancel(void *pPacket)
{
	sGU_SKILL_AFFECTING_CANCELED_NFY *pSkillAffectingCancel = (sGU_SKILL_AFFECTING_CANCELED_NFY*)pPacket;
	
	CNtlSLEventGenerator::SobSkillAffectingCancel(pSkillAffectingCancel->hSubject);
}

void PacketHandler_GSCharSkillCastingDelayed(void *pPacket)
{
	sGU_SKILL_CASTING_DELAYED_NFY *pSkillCastingDelayed = (sGU_SKILL_CASTING_DELAYED_NFY*)pPacket;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pSkillCastingDelayed->handle) );

	if(pActor == NULL)
		return;

	CNtlBeCharData *pBeData			= reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff		= pBeData->GetSkillStuff();
	pSkillStuff->fCurrCastingTime	= (RwReal)(pSkillCastingDelayed->dwCastingTime - pSkillCastingDelayed->dwCastingTimeRemaining)/1000.f;
	pSkillStuff->fMaxCastingTime	= (RwReal)pSkillCastingDelayed->dwCastingTime/1000.0f;
}

void PacketHandler_GSCharSkillTargetListReq(void *pPacket)
{
	sGU_SKILL_TARGET_LIST_REQ *pSkillTargetListReq = (sGU_SKILL_TARGET_LIST_REQ*)pPacket;
	
	API_GetSLPacketGenerator()->SendCharSkillTargetList(pSkillTargetListReq->byAvatarType);
}

void PacketHandler_GSCharSkillAction(void *pPacket)
{
	sGU_CHAR_ACTION_SKILL *pSkillAction = (sGU_CHAR_ACTION_SKILL*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	NTL_ASSERT(pSobAvatar, "Not exist avatar instance");

	// failure
	if(pSkillAction->wResultCode != GAME_SUCCESS)
	{
		if( pSobAvatar->GetSerialID() == pSkillAction->handle )
			CNtlSLEventGenerator::SysMsg(pSobAvatar->GetSerialID(), Logic_GetResultCodeString(pSkillAction->wResultCode, "GU_CHAR_ACTION_SKILL").c_str());

		CNtlSLEventGenerator::SobSkillCancel(pSkillAction->handle, pSkillAction->wResultCode);
	}
	else // success
	{
		RwUInt32 stateId = Logic_GetActorStateId(pSobAvatar);

		// not sure why this faint/die part is required
		if (stateId == NTL_FSMSID_HTB_SANDBAG)
		{
			CNtlSLEventGenerator::SobSkillCancel(pSkillAction->handle, pSkillAction->wResultCode);
			return;
		}

		RwV3d vFinalLoc;
		NtlLocationDecompress(&pSkillAction->vFinalLoc, &vFinalLoc.x, &vFinalLoc.y, &vFinalLoc.z);

		CNtlSLEventGenerator::SobSkillAction(pSkillAction->handle, pSkillAction->wResultCode, pSkillAction->hAppointedTarget, pSkillAction->skillId,
			pSkillAction->byRpBonusType, pSkillAction->bySkillResultCount, pSkillAction->aSkillResult, pSkillAction->dwLpEpEventId, vFinalLoc);

		CNtlSLEventGenerator::SobSkillActioned(pSkillAction->hAppointedTarget, pSkillAction->handle);

		//CNtlSLEventGenerator::SobSkillResult(); // to do

		CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>(pSkillTbl->FindData(pSkillAction->skillId));

		CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
		std::wstring wstrSkillName = pSkillText->GetText(pSkillTblData->Skill_Name);

		if (Logic_GetAvatarHandle() == pSkillAction->handle)
		{
			WCHAR buf[256] = { 0, };
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_ACTION_ATTACKER"), wstrSkillName.c_str());

			CNtlSLEventGenerator::SobSkillInfoText(buf, INFOCOLOR_1, CHAT_TYPE_SYSTEM);
		}
		else if (Logic_GetAvatarTargetHandle() == pSkillAction->handle)
		{
			CNtlSob* pTarget = GetNtlSobManager()->GetSobObject(pSkillAction->handle);
			if (!pTarget)
				return;

			WCHAR buf[256] = { 0, };
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_ACTION_DEFENDER"), Logic_GetName(pTarget), wstrSkillName.c_str());

			CNtlSLEventGenerator::SobSkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
		}
	}
}

void PacketHandler_GSCharSkillNotifyExp(void *pPacket)
{
	sGU_SKILL_NOTIFY_EXP *pSkillExp = (sGU_SKILL_NOTIFY_EXP*)pPacket;
	Logic_SetSkillExp(pSkillExp->bySlotIndex, pSkillExp->wSkillExp);
}

void Packethandler_GSCharSkillLearnedNfy(void *pPacket)
{
	sGU_SKILL_LEARNED_NFY *pSkillLearnedNfy = (sGU_SKILL_LEARNED_NFY*)pPacket;

	Logic_PlayGUISound(GSD_SYSTEM_SKILL_LEARN);

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobSkillAdd(pSobAvatar->GetSerialID(), pSkillLearnedNfy->skillIndex, pSkillLearnedNfy->skillId);

	GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_LEARN_SKILL", FALSE, NULL, Logic_GetSkillName( pSkillLearnedNfy->skillId ) );
}

void PacketHandler_GSCharSkillLearnRes(void *pPacket)
{
	sGU_SKILL_LEARN_RES *pSkillLearnRes = (sGU_SKILL_LEARN_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_SKILL_LEARN_RES );

	if(pSkillLearnRes->wResultCode != GAME_SUCCESS)
	{			
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pSkillLearnRes->wResultCode, "GU_SKILL_LEARN_RES"));
		return;
	}
}

void PacketHandler_GSCharSkillLearnByItemRes(void * pPacket)
{
	sGU_SKILL_LEARN_BY_ITEM_RES *pSkillLearnRes = (sGU_SKILL_LEARN_BY_ITEM_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_SKILL_LEARN_BY_ITEM_RES);

	if (pSkillLearnRes->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pSkillLearnRes->wResultCode, "GU_SKILL_LEARN_BY_ITEM_RES"));
		return;
	}
}

void PacketHandler_GSCharSkillUpgradeRes(void *pPacket)
{
	sGU_SKILL_UPGRADE_RES *pSkillUpgradeRes = (sGU_SKILL_UPGRADE_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_SKILL_UPGRADE_RES );

	if(pSkillUpgradeRes->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pSkillUpgradeRes->wResultCode, "GU_SKILL_UPGRADE_RES"));
		return;
	}

	Logic_PlayGUISound(GSD_SYSTEM_SKILL_LEARN);

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobSkillUpgrade(pSobAvatar->GetSerialID(), pSkillUpgradeRes->skillIndex, pSkillUpgradeRes->skillId);

	GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_LEARN_SKILL", FALSE, NULL, Logic_GetSkillName( pSkillUpgradeRes->skillId ) );
}

void PacketHandler_GSCharSkillCoolTimeStartNfy(void* pPacket)
{
	sGU_SKILL_COOL_TIME_STARTED_NFY* pSkillCoolTimeStartNfy = (sGU_SKILL_COOL_TIME_STARTED_NFY*)pPacket;

	CNtlSkillContainer* pSkillContainer = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer();
	CNtlSobSkill* pSobSkill = pSkillContainer->GetSkillFromTableId( pSkillCoolTimeStartNfy->skillTblidx );
	NTL_ASSERT( pSobSkill, "PacketHandler_GSCharSkillCoolTimeStartNfy : Invalid Skill table id" );
	if( pSobSkill )
	{
		CNtlSobSkillIcon* pSkillIcon = reinterpret_cast<CNtlSobSkillIcon*>( pSobSkill->GetIcon() );
		pSkillIcon->SetSkillIconState( CNtlSobSkillIcon::ICON_STATE_COOLING, pSkillCoolTimeStartNfy->byRpBonus);
	}
}

void PacketHandler_GSCharSkillSetPassiveEffect(void* pPacket)
{
	sGU_SKILL_PASSIVE_EFFECT_APPLIED_NFY* pPassiveEffect = (sGU_SKILL_PASSIVE_EFFECT_APPLIED_NFY*)pPacket;

	CNtlSLEventGenerator::SobSkillSetPassiveEffect( pPassiveEffect->skillTblidx, pPassiveEffect->skillIndex, &pPassiveEffect->sUpdatedSkillData );
}

void PacketHandler_GSCharUpdateSP(void* pPacket)
{
	sGU_UPDATE_CHAR_SP* pUpdateSP = (sGU_UPDATE_CHAR_SP*)pPacket;

	Logic_SetSp( pUpdateSP->dwSpPoint );
}

void PacketHandler_GSSkillInitRes(void* pPacket)
{
	sGU_SKILL_INIT_RES* pSkillInitRes = (sGU_SKILL_INIT_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_SKILL_INIT_RES );

	if( pSkillInitRes->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pSkillInitRes->wResultCode, "GU_SKILL_INIT_RES"), TRUE );
		return;
	}	
}

void PacketHandler_GSSkillInitNfy(void* pPacket)
{
	sGU_SKILL_INIT_NFY* pSkillInitRes = (sGU_SKILL_INIT_NFY*)pPacket;

	CNtlSLEventGenerator::SobSkillInit();
}

void PacketHandler_GSSkillResetPlusRes(void * pPacket)
{
	sGU_SKILL_RESET_PLUS_RES* pRes = (sGU_SKILL_RESET_PLUS_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_SKILL_RESET_PLUS_RES);

	if (pRes->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_SKILL_RESET_PLUS_RES"), TRUE);
		return;
	}
}

void PacketHandler_GSCharRenameRes(void * pPacket)
{
	sGU_CHARACTER_RENAME_RES* pRes = (sGU_CHARACTER_RENAME_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock(GU_CHARACTER_RENAME_RES);

	if (pRes->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pRes->wResultCode, "GU_CHARACTER_RENAME_RES"), TRUE);
		return;
	}
}

void PacketHandler_GUSkillRpBonusSettingRes( void *pPacket ) 
{
	sGU_SKILL_RP_BONUS_SETTING_RES* pResult = (sGU_SKILL_RP_BONUS_SETTING_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_SKILL_RP_BONUS_SETTING_RES );

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SKILL_RP_BONUS_SETTING_RES"), TRUE );
		return;
	}

	// 설정이 적용되었다.
	CNtlSLEventGenerator::RpBonusSetupRes( pResult->skillId, pResult->skillIndex, pResult->byRpBonusType, pResult->bIsRpBonusAuto );
	GetAlarmManager()->AlarmMessage( "DST_SKILL_ABILITY_ACCEPT" );
}

void PacketHandler_GSCharHTBLearnRes(void* pPacket)
{
	sGU_HTB_LEARN_RES* pHTBLearnRes = (sGU_HTB_LEARN_RES*)pPacket;

	if( pHTBLearnRes->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pHTBLearnRes->wResultCode, "GU_HTB_LEARN_RES"), TRUE );
		return;
	}

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobHTBSkillAdd( pSobAvatar->GetSerialID(), pHTBLearnRes->bySkillSlot, pHTBLearnRes->skillId );
	GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_LEARN_SKILL", FALSE, NULL, Logic_GetHTBName( pHTBLearnRes->skillId ) );
}

void PacketHandler_GSCharHTBStartRes(void* pPacket)
{
	sGU_HTB_START_RES *pHTBStart = (sGU_HTB_START_RES*)pPacket;

	if( pHTBStart->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), Logic_GetResultCodeString(pHTBStart->wResultCode, "").c_str());
	}
}

void PacketHandler_GSCharSocialAction(void* pPacket)
{
	sGU_SOCIAL_ACTION* pSocialAction = (sGU_SOCIAL_ACTION*)pPacket;

	CNtlSLEventGenerator::SobSocialAction( pSocialAction->hSubject, pSocialAction->socialActionId );
}

void PacketHandler_GSBuffRegisted(void *pPacket)
{
	sGU_BUFF_REGISTERED *pBuffReg = (sGU_BUFF_REGISTERED*)pPacket;

	CNtlSLEventGenerator::SobBuffAdd(pBuffReg->hHandle, pBuffReg->buffInfo.buffIndex, pBuffReg->buffInfo.bySourceType, pBuffReg->buffInfo.sourceTblidx, 
		pBuffReg->buffInfo.dwTimeRemaining, pBuffReg->buffInfo.dwInitialDuration, pBuffReg->buffInfo.aBuffParameter);

	// Fake Buff의 처리	
	if (pBuffReg->hHandle != Logic_GetAvatarHandle())
	{
		CNtlSLEventGenerator::SobFakeBuffAdd(pBuffReg->hHandle, pBuffReg->buffInfo.buffIndex, pBuffReg->buffInfo.bySourceType, pBuffReg->buffInfo.sourceTblidx, pBuffReg->buffInfo.dwTimeRemaining, pBuffReg->buffInfo.dwInitialDuration, pBuffReg->buffInfo.aBuffParameter);
	}
	else if (pBuffReg->bNeedToDisplayMessage)
	{
		if( pBuffReg->buffInfo.bySourceType == DBO_OBJECT_SOURCE_SKILL )
			GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_BUFF", FALSE, NULL, Logic_GetSkillName( pBuffReg->buffInfo.sourceTblidx) );
		else if( pBuffReg->buffInfo.bySourceType == DBO_OBJECT_SOURCE_ITEM )
			GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_BUFF", FALSE, NULL, Logic_GetItemName( pBuffReg->buffInfo.sourceTblidx) );
	}
}


void PacketHandler_GSBuffDropped(void *pPacket)
{
	sGU_BUFF_DROPPED *pBuffDrop = (sGU_BUFF_DROPPED*)pPacket;

	CNtlSLEventGenerator::SobBuffDrop(pBuffDrop->hHandle, pBuffDrop->buffIndex, pBuffDrop->bySourceType);

	// Fake Buff의 처리	
	if( pBuffDrop->hHandle != Logic_GetAvatarHandle() )
		CNtlSLEventGenerator::SobFakeBuffDrop(pBuffDrop->hHandle, pBuffDrop->buffIndex, pBuffDrop->bySourceType);
}

void PacketHandler_GSBuffRefreshAll(void *pPacket)
{
	sGU_BUFF_REFRESH_ALL *pBuffRefreshAll = (sGU_BUFF_REFRESH_ALL*)pPacket;

	CNtlSLEventGenerator::SobBuffRefreshAll( pBuffRefreshAll->hHandle, pBuffRefreshAll->byCount, pBuffRefreshAll->aBuffInfo );

	// Fake Buff의 처리	
	if( pBuffRefreshAll->hHandle != Logic_GetAvatarHandle() )
		CNtlSLEventGenerator::SobFakeBuffRefreshAll(pBuffRefreshAll->hHandle, pBuffRefreshAll->byCount, pBuffRefreshAll->aBuffInfo);
}

void PacketHandler_GSBuffDropRes(void *pPacket)
{
	sGU_BUFF_DROP_RES *pBuffDropRes = (sGU_BUFF_DROP_RES*)pPacket;

	if( pBuffDropRes->wResultCode != GAME_SUCCESS )
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pBuffDropRes->wResultCode, "GU_BUFF_DROP_RES"), TRUE );
}

void PacketHandler_GSBuffActivate(void * pPacket)
{
	sGU_BUFF_ACTIVATE *pRes = (sGU_BUFF_ACTIVATE*)pPacket;

	CNtlSLEventGenerator::SobBuffActivate(pRes->handle, &pRes->sBuffInfo);

	if (pRes->handle != Logic_GetAvatarHandle())
		CNtlSLEventGenerator::SobFakeBuffActivate(pRes->handle, &pRes->sBuffInfo);
}

void PacketHandler_GSCharUpdateLp(void *pPacket)
{
	sGU_UPDATE_CHAR_LP *pUpdateLp = (sGU_UPDATE_CHAR_LP*)pPacket;

	CNtlParty* pParty = reinterpret_cast<CNtlParty*>(GetNtlSLGlobal()->GetSobAvatar()->GetParty());
	if( pParty->IsMember(pUpdateLp->handle) )
	{
		// 나의 파티원이라면 업데이트
		CNtlSLEventGenerator::PartyUpdate(PMT_LP, pUpdateLp->handle, pUpdateLp->curLp, pUpdateLp->maxLp);
	}

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateLp->handle) );
	if(pActor == NULL)
		return;

	// LP Sync
	CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();
	
	if(pAnimSyncManager->IsExistTarget(pActor->GetSerialID()))
	{
		pAnimSyncManager->SetFinialServerSyncUnit(pActor->GetSerialID(), pUpdateLp->dwLpEpEventId, pUpdateLp->curLp, pUpdateLp->maxLp);
	}
	else
	{
		Logic_SetLp(pActor, pUpdateLp->curLp, pUpdateLp->maxLp);
	}

//	Logic_SetLp(pActor, pUpdateLp->wCurLP, pUpdateLp->wMaxLP);
}

void PacketHandler_GSCharUpdateEp(void *pPacket)
{
	sGU_UPDATE_CHAR_EP *pUpdateEp = (sGU_UPDATE_CHAR_EP*)pPacket;

	CNtlParty* pParty = reinterpret_cast<CNtlParty*>(GetNtlSLGlobal()->GetSobAvatar()->GetParty());
	if( pParty->IsMember(pUpdateEp->handle) )
	{
		// 나의 파티원이라면 업데이트
		CNtlSLEventGenerator::PartyUpdate(PMT_EP, pUpdateEp->handle, pUpdateEp->wCurEP, pUpdateEp->wMaxEP);
	}

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateEp->handle) );

	if(pActor == NULL)
	{
		// avooo : 멀리 떨어져 있는 파티의 EP 업데이트도 오기에 주석처리 했습니다.
		//NTL_ASSERTFAIL( "PacketHandler_GSCharUpdateRp => invalid handle (" << pUpdateEp->handle << ")" );
		return;
	}
	Logic_SetEp(pActor, pUpdateEp->wCurEP, pUpdateEp->wMaxEP);
}

void PacketHandler_GSCharUpdateLpEp(void* pPacket)
{
	sGU_UPDATE_CHAR_LP_EP *pUpdateLpEp = (sGU_UPDATE_CHAR_LP_EP*)pPacket;

	CNtlParty* pParty = reinterpret_cast<CNtlParty*>(GetNtlSLGlobal()->GetSobAvatar()->GetParty());
	if( pParty->IsMember(pUpdateLpEp->handle) )
	{
		// 나의 파티원이라면 업데이트
		CNtlSLEventGenerator::PartyUpdate(PMT_LP, pUpdateLpEp->handle, pUpdateLpEp->curLp, pUpdateLpEp->maxLp);		
		CNtlSLEventGenerator::PartyUpdate(PMT_EP, pUpdateLpEp->handle, pUpdateLpEp->wCurEP, pUpdateLpEp->wMaxEP);
	}

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateLpEp->handle) );
	if(pActor == NULL)
		return;

	// LP Sync
	CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();
	
	if(pAnimSyncManager->IsExistTarget(pActor->GetSerialID()))
	{
		pAnimSyncManager->SetFinialServerSyncUnit(pActor->GetSerialID(), pUpdateLpEp->dwLpEpEventId, pUpdateLpEp->curLp, pUpdateLpEp->maxLp);
	}
	else
	{
		Logic_SetLp(pActor, pUpdateLpEp->curLp, pUpdateLpEp->maxLp);
	}
	
//	Logic_SetLp(pActor, pUpdateLpEp->wCurLP, pUpdateLpEp->wMaxLP); //why is this out?? by daneos question
	Logic_SetEp(pActor, pUpdateLpEp->wCurEP, pUpdateLpEp->wMaxEP);
}

void PacketHandler_GSCharRefreshLpEp(void* pPacket)
{
	//sGU_CHAR_REFRESH_LP_EP *pRefreshLpEp = (sGU_CHAR_REFRESH_LP_EP*)pPacket;

	//CNtlParty* pParty = reinterpret_cast<CNtlParty*>(GetNtlSLGlobal()->GetSobAvatar()->GetParty());
	//if( pParty->IsMember(pRefreshLpEp->hSubject) )
	//{
	//	// 나의 파티원이라면 업데이트
	//	CNtlSLEventGenerator::PartyUpdate(PMT_LP, pRefreshLpEp->hSubject, pRefreshLpEp->wCurLp, pRefreshLpEp->wMaxLp);		
	//	CNtlSLEventGenerator::PartyUpdate(PMT_EP, pRefreshLpEp->hSubject, pRefreshLpEp->wCurEp, pRefreshLpEp->wMaxEp);
	//}

	//CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pRefreshLpEp->hSubject) );
	//if(pActor == NULL)
	//	return;
	//
	//CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();

	//if(pAnimSyncManager->IsExistTarget(pActor->GetSerialID()))
	//{
	//	pAnimSyncManager->SetFinialServerSyncUnit(pActor->GetSerialID(), INVALID_SYNCEVENT_ID, pRefreshLpEp->wCurLp, pRefreshLpEp->wMaxLp);
	//}
	//else
	//{
	//	Logic_SetLp(pActor, pRefreshLpEp->wCurLp, pRefreshLpEp->wMaxLp);
	//}

	//Logic_SetEp(pActor, pRefreshLpEp->wCurEp, pRefreshLpEp->wMaxEp);
}

void PacketHandler_GSCharUpdateRp(void *pPacket)
{
	sGU_UPDATE_CHAR_RP *pUpdateRp = (sGU_UPDATE_CHAR_RP*)pPacket;

	CDboEventGenerator::UpdateRP( Logic_GetAvatarHandle(), (RwInt32)pUpdateRp->wCurRP, (RwInt32)pUpdateRp->wMaxRP );
	Logic_SetRp(Logic_GetAvatarActor(), pUpdateRp->wCurRP, pUpdateRp->wMaxRP);
}


void PacketHandler_GSCharUpdateRpBall( void* pPacket ) 
{
    sGU_UPDATE_CHAR_RP_BALL* pData = (sGU_UPDATE_CHAR_RP_BALL*)pPacket;
  
    CDboEventGenerator::UpdateRPStock(Logic_GetAvatarHandle(), pData->byCurRPBall, (RwInt32)GetNtlSLGlobal()->GetAvatarInfo()->byMaxRpStock, pData->bDropByTime);
	Logic_SetRpStock(Logic_GetAvatarActor(), pData->byCurRPBall);
}

void PacketHandler_GSCharUpdateRpBallMax( void* pPacket ) 
{
    sGU_UPDATE_CHAR_RP_BALL_MAX* pData = (sGU_UPDATE_CHAR_RP_BALL_MAX*)pPacket;

	RwInt32 uiRpStock = (RwInt32)GetNtlSLGlobal()->GetAvatarInfo()->sCharPf.byCurRPBall;
    CDboEventGenerator::UpdateRPStock(Logic_GetAvatarHandle(), uiRpStock, pData->byMaxRPBall);

	Logic_SetRpStockMax(Logic_GetAvatarActor(), pData->byMaxRPBall);
}

void PacketHandler_GSCharUpdateExp(void *pPacket)
{
	sGU_UPDATE_CHAR_EXP *pUpdateExp = (sGU_UPDATE_CHAR_EXP*)pPacket;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateExp->handle) );
	if(pActor == NULL)
	{
		NTL_ASSERTFAIL( "PacketHandler_GSCharUpdateExp => invalid handle (" << pUpdateExp->handle << ")" );
		return;
	}

	Logic_SetExp(pActor, pUpdateExp->dwCurExp);
	CDboEventGenerator::UpdateExp( pUpdateExp->dwIncreasedExp, pUpdateExp->dwAcquisitionExp, pUpdateExp->dwBonusExp );
}

void PacketHandler_GSCharUpdateLevel(void *pPacket)
{
	sGU_UPDATE_CHAR_LEVEL *pUpdateLevel = (sGU_UPDATE_CHAR_LEVEL*)pPacket;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateLevel->handle) );

	if(pActor == NULL)
	{
		NTL_ASSERTFAIL( "PacketHandler_GSCharUpdateLevel => invalid handle (" << pUpdateLevel->handle << ")" );
		return;
	}
	
	if(pUpdateLevel->byPrevLevel != pUpdateLevel->byCurLevel)
	{
		// Notify
		if( pActor->GetSerialID() == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_LEVEL_UP", FALSE, NULL, pUpdateLevel->byPrevLevel, pUpdateLevel->byCurLevel );

			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
			tSoundParam.pcFileName		= GSD_JINGLE_LEVEL_UP;
			GetSoundManager()->Play(&tSoundParam);
		}

		CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
		RwV3d vOffset;
		CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);
		pSobProxy->CreatePLChildEffect(NTL_VID_LEVEL_UP, vOffset);		

		// discord
#ifdef USE_DISCORD
		if(Logic_GetAvatarHandle() == pUpdateLevel->handle)
			GetDiscordManager()->UpdateLevel(pUpdateLevel->byCurLevel);
#endif
	}

	Logic_SetLevel(pActor, pUpdateLevel->byCurLevel);
	Logic_SetMaxExp(pActor, pUpdateLevel->dwMaxExpInThisLevel);
}

void PacketHandler_GSCharUpdateSpeed(void *pPacket)
{
	sGU_UPDATE_CHAR_SPEED *pUpdateSpeed = (sGU_UPDATE_CHAR_SPEED*)pPacket;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateSpeed->handle) );
	if( pActor == NULL)
		return;

	Logic_SetSpeed(pActor, pUpdateSpeed->fLastWalkingSpeed, pUpdateSpeed->fLastRunningSpeed);
}

void PacketHandler_GSCharUpdateAnimSpeed(void *pPacket)
{
	sGU_UPDATE_CHAR_ATTACK_SPEEDRATE *pUpdateAnimSpeed = (sGU_UPDATE_CHAR_ATTACK_SPEEDRATE*)pPacket;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateAnimSpeed->handle) );
	if( pActor == NULL)
		return;

	if(pUpdateAnimSpeed->wAttackSpeedRate > 0)
		Logic_SetAttackAnimSpeed(pActor, (RwReal)1000/(RwReal)pUpdateAnimSpeed->wAttackSpeedRate);
}

void PacketHandler_GSCharUpdateItemEquip(void *pPacket)
{
	sGU_UPDATE_ITEM_EQUIP *pUpdateItemEquip = (sGU_UPDATE_ITEM_EQUIP*)pPacket;

	CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetNtlSobManager()->GetSobObject(pUpdateItemEquip->handle) );
	CNtlInventoryBase *pInventory = pSobPlayer->GetEquipInventory();

	pInventory->SetEquipTableId(pUpdateItemEquip->byPos, pUpdateItemEquip->sItemBrief.tblidx);

	CNtlSLEventGenerator::SobEquipModelChange(pUpdateItemEquip->handle, pUpdateItemEquip->byPos, 
		pUpdateItemEquip->sItemBrief.tblidx, pUpdateItemEquip->sItemBrief.byGrade, pUpdateItemEquip->sItemBrief.byBattleAttribute);
}

void PacketHandler_GSCharUpdateTargetInfo(void *pPacket)
{
	// by daneos
	/*sGU_UPDATE_TARGET_INFO *pUpdateTargteInfo = (sGU_UPDATE_TARGET_INFO*)pPacket;
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateTargteInfo->handle) );
	if(pActor == NULL)
	{
		NTL_ASSERTFAIL( "PacketHandler_GSCharUpdateTargetInfo => invalid handle (" << pUpdateTargteInfo->handle << ")" );
		return;
	}*/

	///*

	//CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();

	//if(pAnimSyncManager->IsExistTarget(pActor->GetSerialID()))
	//{
	//	pAnimSyncManager->SetFinialServerSyncUnit(pActor->GetSerialID(), INVALID_SYNCEVENT_ID, pRefreshLpEp->wCurLp, pRefreshLpEp->wMaxLp);
	//}
	//else
	//{
	//	Logic_SetLp(pActor, pUpdateTargteInfo->wCurLP, pUpdateTargteInfo->wMaxLP);
	//}
	//*/

	//Logic_SetLp(pActor, pUpdateTargteInfo->wCurLP, pUpdateTargteInfo->wMaxLP);
}

void PacketHandler_GSCharUpdateState(void *pPacket)
{
	sGU_UPDATE_CHAR_STATE *pUpdateState = (sGU_UPDATE_CHAR_STATE*)pPacket;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(pUpdateState->handle));
	if (pActor)
	{
		RwV3d v3DirTemp;
		v3DirTemp.x = pUpdateState->sCharState.sCharStateBase.vCurDir.x;
		v3DirTemp.y = pUpdateState->sCharState.sCharStateBase.vCurDir.y;
		v3DirTemp.z = pUpdateState->sCharState.sCharStateBase.vCurDir.z;
		DBO_ASSERT(-INVALID_FLOAT < v3DirTemp.x, "min dirX : " << v3DirTemp.x << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(-INVALID_FLOAT < v3DirTemp.y, "min dirX : " << v3DirTemp.y << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(-INVALID_FLOAT < v3DirTemp.z, "min dirX : " << v3DirTemp.z << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);

		DBO_ASSERT(INVALID_FLOAT > v3DirTemp.x, "max dirX : " << v3DirTemp.x << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(INVALID_FLOAT > v3DirTemp.y, "max dirX : " << v3DirTemp.y << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(INVALID_FLOAT > v3DirTemp.z, "max dirX : " << v3DirTemp.z << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);


		RwV3d v3PosTemp;
		v3PosTemp.x = pUpdateState->sCharState.sCharStateBase.vCurLoc.x;
		v3PosTemp.y = pUpdateState->sCharState.sCharStateBase.vCurLoc.y;
		v3PosTemp.z = pUpdateState->sCharState.sCharStateBase.vCurLoc.z;
		DBO_ASSERT(-INVALID_FLOAT < v3PosTemp.x, "min posX : " << v3PosTemp.x << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(-INVALID_FLOAT < v3PosTemp.y, "min posX : " << v3PosTemp.y << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(-INVALID_FLOAT < v3PosTemp.z, "min posX : " << v3PosTemp.z << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);

		DBO_ASSERT(INVALID_FLOAT > v3PosTemp.x, "max posX : " << v3PosTemp.x << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(INVALID_FLOAT > v3PosTemp.y, "max posX : " << v3PosTemp.y << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);
		DBO_ASSERT(INVALID_FLOAT > v3PosTemp.z, "max posX : " << v3PosTemp.z << "State : " << pUpdateState->sCharState.sCharStateBase.byStateID);

		if (GetSceneManager()->GetAvailablePos(v3PosTemp) == FALSE)
		{
			DBO_FAIL("unavailable avatar position. x : " << v3PosTemp.x << " y : " << v3PosTemp.y << " z : " << v3PosTemp.z);

			return;
		}

		if (pUpdateState->sCharState.sCharStateBase.byStateID == CHARSTATE_FAINTING)
		{
			RwUInt32 uiClassId = pActor->GetClassID();
			if (!(uiClassId == SLCLASS_PLAYER || uiClassId == SLCLASS_AVATAR))
			{
				if (Logic_GetAvatarTargetHandle() == pUpdateState->handle)
				{
					Logic_SobTarget(INVALID_SERIAL_ID, INVALID_BYTE);
				}
			}
		}

		RwUInt32 classid = pActor->GetClassID();
	//	if (classid == SLCLASS_PLAYER || classid == SLCLASS_AVATAR)
	//		DBO_WARNING_MESSAGE("Class: " << classid << " receive state update to: " << (int)pUpdateState->sCharState.sCharStateBase.byStateID);

		CNtlSLEventGenerator::SobServerUpdateState(pUpdateState->handle, &pUpdateState->sCharState);
	}
}

void PacketHandler_GSCharUpdateCondition(void *pPacket)
{
	sGU_UPDATE_CHAR_CONDITION *pUpdateCondition = (sGU_UPDATE_CHAR_CONDITION*)pPacket;

	CNtlSLEventGenerator::SobServerUpdateCondition(pUpdateCondition->handle, pUpdateCondition->qwConditionFlag);
}

void PacketHandler_GSCharUpdateAspect( void* pPacket ) 
{   
    sGU_UPDATE_CHAR_ASPECT_STATE* pUpdateAspect = (sGU_UPDATE_CHAR_ASPECT_STATE*)pPacket;

    CNtlSLEventGenerator::SobServerUpdateAspect(pUpdateAspect->handle, pUpdateAspect->aspectState);
}

void PacketHandler_GSCharUpdateZenny(void *pPacket)
{
	sGU_UPDATE_CHAR_ZENNY *pUpdateState = (sGU_UPDATE_CHAR_ZENNY*)pPacket;

	if( pUpdateState->bIsNew )
	{
		RwUInt32 uiZenny = pUpdateState->dwZenny - Logic_GetZenny();
		CDboEventGenerator::ZennyLootingEffect( uiZenny );

        if(pUpdateState->byChangeType != ZENNY_CHANGE_TYPE_PICK &&    // PICK은 PICK RES에서 처리한다.
		   pUpdateState->byChangeType != ZENNY_CHANGE_TYPE_PARTY_PICK ) // ZENNY_CHANGE_TYPE_PARTY_PICK : GU_ZENNY_PICK_RES 에서 처리
        {
		    GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GET_ZENNY", FALSE, NULL, Logic_FormatZeni(uiZenny) );
        }
	}

	switch( pUpdateState->byChangeType )
	{
		case ZENNY_CHANGE_TYPE_MAIL_SEND:
			GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, 0 );
			break;
		case ZENNY_CHANGE_TYPE_PARTY_ITEM_INVEST:
			GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::PARTY_INVEN, 0 );
			break;
		case ZENNY_CHANGE_TYPE_TRADE:
			GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::USER_TRADE, 0 );
			break;
		case ZENNY_CHANGE_TYPE_PARTY_VICTIM_ADD:
			GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::PARTY_CHARM, 0 );
			break;
	}

	Logic_SetZenny(pUpdateState->dwZenny);
}

void PacketHandler_GSCharUpdateMudosaPoint( void* pPacket )
{
	sGU_UPDATE_CHAR_MUDOSA_POINT *pUpdateState = (sGU_UPDATE_CHAR_MUDOSA_POINT*)pPacket;

	Logic_SetMudosaPoint( pUpdateState->dwMudosaPoint );
}

void PacketHandler_GSAvatarAttrUpdate(void *pPacket)
{
	if(GetNtlSobManager() == NULL)
		return;

	sGU_AVATAR_ATTRIBUTE_UPDATE *pAttrUpdate = (sGU_AVATAR_ATTRIBUTE_UPDATE*)pPacket;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pAttrUpdate->hHandle);
	if(pSobObj)
		Logic_SetStatus(pSobObj, pAttrUpdate->byAttributeTotalCount, pAttrUpdate->abyFlexibleField);
}

void PacketHandler_GUCharUpdateHonor(void *pPacket)
{
	sGU_UPDATE_CHAR_REPUTATION* pUpdateHonor = (sGU_UPDATE_CHAR_REPUTATION*)pPacket;

	CNtlSobActor *pActor = GetNtlSLGlobal()->GetSobAvatar();

	Logic_SetReputation(pActor, pUpdateHonor->dwReputation);
}


void PacketHandler_GSItemIdentificationRes(void *pPacket)
{	
	// 미확인 아이템 감정 결과
	sGU_ITEM_IDENTIFY_RES* pResult = (sGU_ITEM_IDENTIFY_RES*)pPacket;

	API_GetSLPacketLockManager()->Unlock( GU_ITEM_IDENTIFY_RES );

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_IDENTIFY_RES"), TRUE );
		return;
	}

	// 아이템 Update와 함께 Effect
	sITEM_DATA* pItemData = &pResult->sItemData;
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSLEventGenerator::SobItemUpdate(pSobAvatar->GetSerialID(), pResult->hItemHandle, pItemData->itemNo, &pItemData->sOptionSet, pItemData->byPlace, pItemData->byPosition,
		pItemData->byStackcount, pItemData->byRank, pItemData->byGrade, pItemData->byCurrentDurability, pItemData->byCurrentDurability, pItemData->bNeedToIdentify, pItemData->byBattleAttribute,
		pItemData->byRestrictState, pItemData->awchMaker, pItemData->byDurationType, pItemData->nUseStartTime, pItemData->nUseEndTime);
	CDboEventGenerator::ItemIdentifyEffect( TRUE, pResult->hItemHandle );

	// 아이템 감정 퀘스트
	CNtlSLEventGenerator::TSItemIdentity();
}

void PacketHandler_GSScouterEquipCheckRes(void *pPacket)
{
	// PC 장비 보기
	sGU_SCOUTER_EQUIP_CHECK_RES* pResult = (sGU_SCOUTER_EQUIP_CHECK_RES*)pPacket;

	if( pResult && pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SCOUTER_EQUIP_CHECK_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_PC_INFO_VIEW, pResult->hTarget, pResult->byItemCount, (RwUInt32)pResult->aItemProfile);
}


void PacketHandler_GSConvertClass(void *pPacket)
{
	sGU_CHAR_CONVERT_CLASS *pConvertClass = (sGU_CHAR_CONVERT_CLASS*)pPacket;

	if( pConvertClass->hTarget == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
	{
		CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
		GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_CHANGE_CLASS", FALSE, NULL, Logic_GetClassName( pSobAvatarAttr->GetClass() ), Logic_GetClassName( pConvertClass->byClass ) );
	}
	
	CNtlSLEventGenerator::SobConvertClass(pConvertClass->hTarget, pConvertClass->byClass);
}

void PacketHandler_GSCharBindRes(void *pPacket)
{
	sGU_CHAR_BIND_RES* pResult = (sGU_CHAR_BIND_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CHAR_BIND_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage( "DST_BIND_SUCCESS" );

	CNtlSLEventGenerator::BindEvent(pResult->byBindType, pResult->bindWorldId, pResult->bindObjectTblidx);

	CNtlSLEventGenerator::TSBindStone();
}

void PacketHandler_GSCharStateWrong(void *pPacket)
{

#ifdef NDEBUG

	if(g_bDispServerMsg)
	{
		sGU_CHAR_STATE_WRONG *pStateWrong = (sGU_CHAR_STATE_WRONG*)pPacket;

		RwChar chBuffer[1024];
		sprintf_s(chBuffer, 1024, "opcode(%s - 현 상태에서 처리할 수 없는 event), handle(%d) => send opcode(%s), server current state(%s)",
			NtlGetPacketName(pStateWrong->wOpCode), pStateWrong->handle, NtlGetPacketName(pStateWrong->wPrevOPCode), NtlGetCharStateString(pStateWrong->byStateID) );

		WCHAR wMsg[1024];
		memset((char*)wMsg, 0, sizeof(WCHAR) * (1024));
		::MultiByteToWideChar(GetACP(), 0, chBuffer, -1, wMsg, 1024);
		RwInt32 nLen = (int)wcslen(wMsg);
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", nLen, wMsg );	
	}

#else

	sGU_CHAR_STATE_WRONG *pStateWrong = (sGU_CHAR_STATE_WRONG*)pPacket;

	RwChar chBuffer[1024];
	sprintf_s(chBuffer, 1024, "opcode(%s - 현 상태에서 처리할 수 없는 event), handle(%d) => send opcode(%s), server current state(%s)",
		NtlGetPacketName(pStateWrong->wOpCode), pStateWrong->handle, NtlGetPacketName(pStateWrong->wPrevOPCode), NtlGetCharStateString(pStateWrong->byStateID) );

	WCHAR wMsg[1024];
	memset((char*)wMsg, 0, sizeof(WCHAR) * (1024));
	::MultiByteToWideChar(GetACP(), 0, chBuffer, -1, wMsg, 1024);
	RwUInt16 nLen = (RwUInt16)wcslen(wMsg);
	CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", nLen, wMsg );	

#endif
}

void PacketHandler_GSEffectAffected(void *pPacket)
{
	sGU_EFFECT_AFFECTED *pEffAffected = (sGU_EFFECT_AFFECTED*)pPacket;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pEffAffected->handle);
	if(pSobObj == NULL)
		return;

	CSystemEffectTable *pSystemEffTbl = API_GetTableContainer()->GetSystemEffectTable();
	sSYSTEM_EFFECT_TBLDAT *pSystemEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffTbl->FindData(pEffAffected->effectTblidx) );
	NTL_ASSERT( pSystemEffTblData, "PacketHandler_GSEffectAffected => find not system effect table id ( " << pEffAffected->effectTblidx << ")" );

	RwBool bHealing = FALSE;
	RwUInt8 byEffectType = pSystemEffTblData->byActive_Effect_Type;
	if(byEffectType == SKILL_ACTIVE_TYPE_DH || byEffectType == SKILL_ACTIVE_TYPE_HOT)
	{
		bHealing = TRUE;
	}

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	NTL_ASSERT(pSobProxy, "PacketHandler_GSEffectAffected => sob proxy instance is null" << "sob class id( " << pSobObj->GetClassID() << " )" );
	RwV3d vPos = pSobObj->GetPosition();
	pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pEffAffected->fActualArgument1, bHealing);


	if (!bHealing)
	{
		CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>(pSkillTbl->FindData(pEffAffected->sourceTblidx));
		NTL_ASSERT(pSkillTblData, "PacketHandler_GSEffectAffected => find not skill table id ( " << pEffAffected->effectTblidx << ")");

		CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
		std::wstring wstrSkillName = pSkillText->GetText(pSkillTblData->Skill_Name);

		/*sub_6209D0(*(_DWORD *)(v37 + 8));
		sub_404DA0((int)&v177, v36, v48);
		v190 = 0;
		v49 = (_DWORD *)sub_46C1B0();
		v50 = sub_470EC0(v49, *(_DWORD *)(a1 + 6));
		if (!v50)
		{
			v190 = -1;
			sub_404E10(&v177);
			return;
		}*/

		if (pSkillTblData->byBuff_Group != 7) // TO DO : not sure if buff_group..  *(_BYTE *)(v37 + 103)
		{
			if (Logic_GetAvatarHandle() == pEffAffected->handle)
			{

			}
			else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
			{

			}
		}

		if (pSystemEffTblData->effectCode == ACTIVE_LP_STEAL_OVER_TIME)
		{
			WCHAR buf[256] = { 0, };

			if (Logic_GetAvatarHandle() == pEffAffected->handle)
			{
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_LP_ABSORBED_ATTACKER"), Logic_GetAvatarName(), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
				Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_COMBAT);
			}
			else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
			{
				CNtlSob* pTarget = GetNtlSobManager()->GetSobObject(pEffAffected->handle);
				if (pTarget)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_LP_ABSORBED_DEFENDER"), Logic_GetName(pTarget), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_COMBAT);
				}
			}
		}
		else if (pSystemEffTblData->effectCode == ACTIVE_EP_STEAL_OVER_TIME)
		{
			WCHAR buf[256] = { 0, };

			if (Logic_GetAvatarHandle() == pEffAffected->handle)
			{
				swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_EP_ABSORBED_ATTACKER"), Logic_GetAvatarName(), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
				Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_COMBAT);
			}
			else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
			{
				CNtlSob* pTarget = GetNtlSobManager()->GetSobObject(pEffAffected->handle);
				if (pTarget)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_EP_ABSORBED_DEFENDER"), Logic_GetName(pTarget), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_COMBAT);
				}
			}
		}
	}
	else
	{
		if (pEffAffected->bySourceType == DBO_OBJECT_SOURCE_SKILL)
		{
			WCHAR buf[256] = { 0, };

			CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>(pSkillTbl->FindData(pEffAffected->sourceTblidx));
			NTL_ASSERT(pSkillTblData, "PacketHandler_GSEffectAffected => find not skill table id ( " << pEffAffected->effectTblidx << ")");

			CTextTable* pSkillText = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();
			std::wstring wstrSkillName = pSkillText->GetText(pSkillTblData->Skill_Name);

			if (pSystemEffTblData->effectCode == ACTIVE_HEAL_OVER_TIME)
			{
				if (Logic_GetAvatarHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_LP"), Logic_GetAvatarName(), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
				}
				else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
				{
					CNtlSob* pTarget = GetNtlSobManager()->GetSobObject(pEffAffected->handle);
					if (pTarget)
					{
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_LP"), Logic_GetName(pTarget), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
						Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
					}
				}
			}
			else if (pSystemEffTblData->effectCode == ACTIVE_EP_OVER_TIME)
			{
				if (Logic_GetAvatarHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_EP"), Logic_GetAvatarName(), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
				}
				else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
				{
					CNtlSob* pTarget = GetNtlSobManager()->GetSobObject(pEffAffected->handle);
					if (pTarget)
					{
						swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_SKILL_EP"), Logic_GetName(pTarget), wstrSkillName.c_str(), (int)pEffAffected->fActualArgument1);
						Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
					}
				}
			}


		}
		else if (pEffAffected->bySourceType == DBO_OBJECT_SOURCE_ITEM)
		{
			WCHAR buf[256] = { 0, };

			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>(pItemTbl->FindData(pEffAffected->sourceTblidx));
			NTL_ASSERT(pItemTblData, "PacketHandler_GSEffectAffected => find not item table id ( " << pEffAffected->effectTblidx << ")");

			/*if (!*(_DWORD *)(pItemTblData + 268))
				goto LABEL_138;*/

			CTextTable* pItemText = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
			std::wstring wstrItemName = pItemText->GetText(pItemTblData->Name);

			if (pSystemEffTblData->effectCode == ACTIVE_HEAL_OVER_TIME)
			{
				if (Logic_GetAvatarHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_LP"), wstrItemName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
				}
				else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_LP"), wstrItemName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
				}
			}
			else if (pSystemEffTblData->effectCode == ACTIVE_EP_OVER_TIME)
			{
				if (Logic_GetAvatarHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_EP"), wstrItemName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
				}
				else if (Logic_GetAvatarTargetHandle() == pEffAffected->handle)
				{
					swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_EP"), wstrItemName.c_str(), (int)pEffAffected->fActualArgument1);
					Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
				}
			}
		}
	}


	CTextTable* pSystemEffectText = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl();
	if(pSystemEffTblData->Keep_Effect_Name != INVALID_TBLIDX)
	{
		std::wstring wstrKeepEffectName = pSystemEffectText->GetText( pSystemEffTblData->Keep_Effect_Name );

		if( wstrKeepEffectName.size() > 0)
		{
			vPos.y += pSobProxy->GetPLEntityHeight() * (RwReal)pSystemEffTblData->byTarget_Effect_Position * 0.01f;

            char chBuffer[1024] = {0,};
			::WideCharToMultiByte(GetACP(), 0, wstrKeepEffectName.c_str(), -1, chBuffer, 1024, NULL, NULL);

			pSobProxy->CreatePLChildEffect(chBuffer, vPos);
		}
	}

    // 원기 흡수를 위한 처리
    if(pSystemEffTblData->effectCode == ACTIVE_LP_STEAL_OVER_TIME || 
       pSystemEffTblData->effectCode == ACTIVE_EP_STEAL_OVER_TIME)
    {
        CNtlSob* pSobCaster = GetNtlSobManager()->GetSobObject(pEffAffected->hCaster);
        if(pSobCaster)
        {
			CNtlSobActor *pSobCasterActor = reinterpret_cast<CNtlSobActor*>(pSobCaster);
			if (pSobCasterActor)
			{
				RwUInt32 stateId = Logic_GetActorStateId(pSobCasterActor);

				// not sure why this faint/die part is required
				if (stateId == NTL_FSMSID_FAINTING || stateId == NTL_FSMSID_DIE)
				{
					if (pEffAffected->fActualArgument2 > 0.0f)
					{
						pSobCasterActor->GetSobProxy()->CreatePLDamageBox(pSobCasterActor->GetPosition(), (RwUInt32)pEffAffected->fActualArgument2, TRUE);
					}
				}
				else
				{
					pSobCaster->GetSobProxy()->CreatePLDamageBox(pSobCaster->GetPosition(), (RwUInt32)pEffAffected->fActualArgument2, TRUE);
				}
			}
        }
    }
}

void PacketHandler_GSAutoRecoverNfy(void * pPacket)
{
	sGU_AUTO_RECOVER_NFY *pRes = (sGU_AUTO_RECOVER_NFY*)pPacket;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pRes->hHandle);
	if (pSobObj == NULL)
		return;

	CItemTable *pItemTable = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT* pItemTbl = reinterpret_cast<sITEM_TBLDAT*>(pItemTable->FindData(pRes->tblidx));
	NTL_ASSERT(pItemTbl, "PacketHandler_GSAutoRecoverNfy => item table is null (" << pRes->tblidx << ")");

	sUSE_ITEM_TBLDAT* pUseItemTable = reinterpret_cast<sUSE_ITEM_TBLDAT*>(API_GetTableContainer()->GetUseItemTable()->FindData(pItemTbl->Use_Item_Tblidx));

	CSystemEffectTable *pSystemEffTbl = API_GetTableContainer()->GetSystemEffectTable();
	sSYSTEM_EFFECT_TBLDAT *pSystemEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>(pSystemEffTbl->FindData(pUseItemTable->aSystem_Effect[0]));
	NTL_ASSERT(pSystemEffTblData, "PacketHandler_GSEffectAffected => find not system effect table id ( " << pUseItemTable->aSystem_Effect[0] << ")");


	CTextTable* pItemText = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	std::wstring wstrItemName = pItemText->GetText(pItemTbl->Name);

	CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
	NTL_ASSERT(pSobProxy, "PacketHandler_GSAutoRecoverNfy => sob proxy instance is null" << "sob class id( " << pSobObj->GetClassID() << " )");

	RwV3d vPos = pSobObj->GetPosition();

	WCHAR buf[256] = { 0, };

	if (pSystemEffTblData->effectCode == ACTIVE_LP_AUTO_RECOVER)
	{
		pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pRes->recoverValue, TRUE);

		if (Logic_GetAvatarHandle() == pRes->hHandle)
		{
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_LP"), wstrItemName.c_str(), pRes->recoverValue);
			Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
		}
		else if (Logic_GetAvatarTargetHandle() == pRes->hHandle)
		{
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_LP"), wstrItemName.c_str(), pRes->recoverValue);
			Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
		}
	}
	else if (pSystemEffTblData->effectCode == ACTIVE_EP_AUTO_RECOVER)
	{
		pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pRes->recoverValue, TRUE);

		if (Logic_GetAvatarHandle() == pRes->hHandle)
		{
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_EP"), wstrItemName.c_str(), pRes->recoverValue);
			Logic_SkillInfoText(buf, INFOCOLOR_3, CHAT_TYPE_SYSTEM);
		}
		else if (Logic_GetAvatarTargetHandle() == pRes->hHandle)
		{
			swprintf_s(buf, GetDisplayStringManager()->GetString("DST_SYSTEMMSG_ITME_EP"), wstrItemName.c_str(), pRes->recoverValue);
			Logic_SkillInfoText(buf, INFOCOLOR_5, CHAT_TYPE_SYSTEM);
		}
	}

	CTextTable* pSystemEffectText = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl();
	if (pSystemEffTblData->Keep_Effect_Name != INVALID_TBLIDX)
	{
		std::wstring wstrKeepEffectName = pSystemEffectText->GetText(pSystemEffTblData->Keep_Effect_Name);

		if (wstrKeepEffectName.size() > 0)
		{
			vPos.y += pSobProxy->GetPLEntityHeight() * (RwReal)pSystemEffTblData->byTarget_Effect_Position * 0.01f;

			char chBuffer[1024] = { 0, };
			::WideCharToMultiByte(GetACP(), 0, wstrKeepEffectName.c_str(), -1, chBuffer, 1024, NULL, NULL);

			pSobProxy->CreatePLChildEffect(chBuffer, vPos);
		}
	}

	// only do stuff if avatar (start cool-down etc)
	if (Logic_GetAvatarHandle() == pRes->hHandle)
	{
		CNtlSLEventGenerator::SobBuffAutoRecover(pRes->hHandle, pRes->buffIndex, pRes->dwRemainTime, pRes->dwRemainValue);
	}
}

void PacketHandler_DisplayNotice(void *pPacket)
{
	sGU_SYSTEM_DISPLAY_TEXT *pDisplayText = (sGU_SYSTEM_DISPLAY_TEXT*)pPacket;

	WCHAR pBuffer[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
	memset(pBuffer, 0, sizeof(pBuffer));
	memcpy(pBuffer, pDisplayText->awchMessage, sizeof(WCHAR)*pDisplayText->wMessageLengthInUnicode); 
		
	if( pDisplayText->byDisplayType == SERVER_TEXT_SYSTEM )
	{
		std::wstring wstrMessage = pBuffer;
		/*gui::RemoveLineFeed( wstrMessage );*/
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", (RwUInt16)wstrMessage.size(), wstrMessage.c_str() );	
	}
	else if( pDisplayText->byDisplayType == SERVER_TEXT_NOTICE )
	{
		CDboEventGenerator::NotifyMessage( SDboEventNotify::PUBLIC, pBuffer );
	}
	else if( pDisplayText->byDisplayType == SERVER_TEXT_SYSNOTICE )
	{
		std::wstring wstrMessage = pBuffer;
		CDboEventGenerator::NotifyMessage( SDboEventNotify::PUBLIC, pBuffer );

		gui::RemoveLineFeed( wstrMessage );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", (RwUInt16)wstrMessage.size(), wstrMessage.c_str() );	
	}
	else if( pDisplayText->byDisplayType == SERVER_TEXT_EMERGENCY )
	{
		std::wstring wstrMessage = pBuffer;
		CNtlSLEventGenerator::BroadMsgNfy_Emergency( wstrMessage );
	}

	else if (pDisplayText->byDisplayType == SERVER_TEXT_CAUTION)
	{
		CDboEventGenerator::NotifyMessage(SDboEventNotify::ACTION, pBuffer);
	}
	else if (pDisplayText->byDisplayType == SERVER_TEXT_TERRITORY)
	{
		CDboEventGenerator::NotifyMessage(SDboEventNotify::TERRITORY, pBuffer);
	}
	else if (pDisplayText->byDisplayType == SERVER_TEXT_RESULTCODE)
	{
		CDboEventGenerator::NotifyMessage(SDboEventNotify::RESULTCODE, pBuffer);
	}

	else if (pDisplayText->byDisplayType != SERVER_TEXT_UNKNOWN)
	{
		DBO_WARNING_MESSAGE( "DisplayNotice Type is Wrong " << pDisplayText->byDisplayType);
	}
}

void PacketHandler_GameCommonMsgNfy(void *pPacket)
{
	sGU_GAME_COMMON_MSG_NFY* pResult = (sGU_GAME_COMMON_MSG_NFY*)pPacket;
	GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_GAME_COMMON_MSG_NFY"), TRUE );
}

void PacketHandler_GSNPCShopStartRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_SHOP_START_RES );

	sGU_SHOP_START_RES* pResult = (sGU_SHOP_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_NPCSHOP);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_START_RES"), TRUE );
		return;
	}

	CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
	pWorldConceptController->ChangeState(WORLD_NPC_SHOP);

	CDboEventGenerator::ShopEvent(pResult->byType, pResult->handle);
}

void PacketHandler_GSNPCShopBuyRes(void *pPacket)
{
	// 서버로부터 응답을 받았다
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_BUY_RES);

	sGU_SHOP_BUY_RES* pResult = (sGU_SHOP_BUY_RES*)pPacket;
	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_BUY_RES"), TRUE );
		return;
	}

	CDboEventGenerator::ShopEvent(TRM_BUY_SUCCESS, pResult->handle);
}

void PacketHandler_GSNPCShopSellRes(void *pPacket)
{
	// 서버로부터 응답을 받았다
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_SELL_RES);

	sGU_SHOP_SELL_RES* pResult = (sGU_SHOP_SELL_RES*)pPacket;
	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_SELL_RES"), TRUE );
		return;
	}

	CDboEventGenerator::ShopEvent(TRM_SELL_SUCCESS, 0);
}

void PacketHandler_GSNPCShopEndRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_SHOP_END_RES );

	sGU_SHOP_END_RES* pResult = (sGU_SHOP_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_END_RES"), TRUE );
		return;
	}

	// 상점을 닫는다
	CDboEventGenerator::ShopEvent(TRM_CLOSE, 0);
}

void PacketHandler_GSNPCEventShopStartRes(void *pPacket)
{
	// 이벤트 상점 시작
	API_GetSLPacketLockManager()->Unlock( GU_SHOP_EVENTITEM_START_RES );

	sGU_SHOP_EVENTITEM_START_RES* pResult = (sGU_SHOP_EVENTITEM_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_NPCSHOP);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_EVENTITEM_START_RES"), TRUE );
		return;
	}

	CNtlWorldConceptNPCCOMMU* pWorldConceptController = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_NPC_COMMU) );
	pWorldConceptController->ChangeState(WORLD_NPC_SHOP);

	CDboEventGenerator::ShopEvent(TRM_NPCSHOP_OPEN, pResult->handle);
}

void PacketHandler_GSNPCEventShopBuyRes(void *pPacket)
{
	// 이벤트 상점에서 구입을 했다
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_EVENTITEM_BUY_RES);

	sGU_SHOP_EVENTITEM_BUY_RES* pResult = (sGU_SHOP_EVENTITEM_BUY_RES*)pPacket;
	if( pResult && pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_EVENTITEM_BUY_RES"), TRUE );
		return;
	}

	CDboEventGenerator::ShopEvent(TRM_BUY_SUCCESS, pResult->handle);
}

void PacketHandler_GSNPCEventShopEndRes(void *pPacket)
{
	// 이벤트 상점 종료
	API_GetSLPacketLockManager()->Unlock( GU_SHOP_EVENTITEM_END_RES );

	sGU_SHOP_EVENTITEM_END_RES* pResult = (sGU_SHOP_EVENTITEM_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_EVENTITEM_END_RES"), TRUE );
		return;
	}

	// 상점을 닫는다
	CDboEventGenerator::ShopEvent(TRM_CLOSE, 0);
}

void PacketHandler_GSNPCShopItemIdentifyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_SHOP_ITEM_IDENTIFY_RES );

	sGU_SHOP_ITEM_IDENTIFY_RES* pResult = (sGU_SHOP_ITEM_IDENTIFY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_ITEM_IDENTIFY_RES"), TRUE );
		return;
	}

	// Pos 슬롯 위치

	sITEM_DATA* pItemData = &pResult->sItemData;
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	SERIAL_HANDLE hItem = Logic_GetItemHandleFromContainerPos( pResult->byPlace, pResult->byPos );

	CNtlSLEventGenerator::SobItemUpdate(pSobAvatar->GetSerialID(), hItem, pItemData->itemNo, &pItemData->sOptionSet, pItemData->byPlace, pItemData->byPosition,
		pItemData->byStackcount, pItemData->byRank, pItemData->byGrade, pItemData->byCurrentDurability, pItemData->byCurrentDurability, pItemData->bNeedToIdentify, pItemData->byBattleAttribute, 
		pItemData->byRestrictState, pItemData->awchMaker, pItemData->byDurationType, pItemData->nUseStartTime, pItemData->nUseEndTime);
	CDboEventGenerator::ItemIdentifyEffect( TRUE, hItem );

	// 아이템 감정 퀘스트
	CNtlSLEventGenerator::TSItemIdentity();
}

void PacketHandler_GSItemDisassembleRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_DISASSEMBLE_RES);

	sGU_ITEM_DISASSEMBLE_RES* pResult = (sGU_ITEM_DISASSEMBLE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_DISASSEMBLE_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSLEventGenerator::SobItemDelete(pSobAvatar->GetSerialID(), pResult->hDeleteItem, pResult->byDeleteItemPlace, pResult->byDeleteItemPos);
}

void PacketHandler_GSItemSocketInsertBeadRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_SOCKET_INSERT_BEAD_RES);

	sGU_ITEM_SOCKET_INSERT_BEAD_RES* pResult = (sGU_ITEM_SOCKET_INSERT_BEAD_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS && pResult->wResultCode == GAME_ITEM_INSERT_BEAD_FAIL_AND_DEL)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_SOCKET_INSERT_BEAD_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSLEventGenerator::ItemSocketInsertBead(pSobAvatar->GetSerialID(), pResult->wResultCode == GAME_SUCCESS, pResult->byItemPlace, pResult->byItemPos, pResult->byRestrictState, pResult->byDurationType,
		pResult->nUseStartTime, pResult->nUseEndTime, pResult->sItemRandomOption, pResult->byBeadPlace, pResult->byBeadPos, pResult->byBeadRemainStack);
}

void PacketHandler_GSItemSocketDestroyBeadRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_SOCKET_DESTROY_BEAD_RES);

	sGU_ITEM_SOCKET_DESTROY_BEAD_RES* pResult = (sGU_ITEM_SOCKET_DESTROY_BEAD_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_SOCKET_DESTROY_BEAD_RES"), TRUE);
		return;
	}

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSLEventGenerator::ItemSocketDestroyBead(pSobAvatar->GetSerialID(), pResult->byItemPlace, pResult->byItemPos, pResult->byRestrictState, pResult->byDurationType);

	GetAlarmManager()->AlarmMessage("DST_ITEM_BEAD_DESTROY_SUCCESS");
}

void PacketHandler_GSItemExchangeRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_ITEM_EXCHANGE_RES);

	sGU_ITEM_EXCHANGE_RES* pResult = (sGU_ITEM_EXCHANGE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_ITEM_EXCHANGE_RES"), TRUE);
		return;
	}
}

void PacketHandler_GSScouterIndicatorRes(void *pPacket)
{	
	sGU_SCOUTER_INDICATOR_RES* pResult = (sGU_SCOUTER_INDICATOR_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_MEASURE_ERROR, pResult->hTarget, pResult->dwRetValue);
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SCOUTER_INDICATOR_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_MEASURE_MEASURE_RESULT, pResult->hTarget, pResult->dwRetValue, 
		pResult->wPhysicalOffence, pResult->wPhysicalDefence, pResult->wEnergyOffence, pResult->wEnergyDefence, pResult->wAttackRate, pResult->wDodgeRate);
}

void PacketHandler_GUTSConfirmStepRes(void *pPacket)
{
	sGU_TS_CONFIRM_STEP_RES* pResult = (sGU_TS_CONFIRM_STEP_RES*)pPacket;

	if ( TS_TYPE_QUEST_CS == pResult->byTsType )
	{
		GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Avatar_TS_Confirm_Step( pResult->wResultCode, pResult->tId, pResult->tcCurId, pResult->tcNextId );
	}
	else if ( TS_TYPE_PC_TRIGGER_CS == pResult->byTsType )
	{
		GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency()->GU_Avatar_TS_Confirm_Step( pResult->wResultCode, pResult->tId, pResult->tcCurId, pResult->tcNextId );
	}
	else
	{
		NTL_ASSERT( false, "The trigger system type miss match" );
	}
}

void PacketHandler_GUTSUpdateSyncNfy(void *pPacket)
{
}

void PacketHandler_GUTSSToCEvtStartNfy(void *pPacket)
{
	sGU_QUEST_SVREVT_START_NFY* pResult = (sGU_QUEST_SVREVT_START_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Avatar_TS_SToC_Event_Start_NFY( pResult->tId, pResult->tcId, pResult->taId );
}

void PacketHandler_GUTSSToCEvtEndNfy(void *pPacket)
{
	sGU_QUEST_SVREVT_END_NFY* pResult = (sGU_QUEST_SVREVT_END_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Avatar_TS_SToC_Event_End_NFY( pResult->tId, pResult->tcId, pResult->taId );
}

void PacketHandler_GUTSSToCEvtUpdateNfy(void *pPacket)
{
	sGU_QUEST_SVREVT_UPDATE_NFY* pResult = (sGU_QUEST_SVREVT_UPDATE_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Avatar_TS_SToC_Event_Update_NFY( pResult->tId, pResult->tcId, pResult->taId, pResult->bySvrEvtType, pResult->bySlot, pResult->uEvtData );
}

void PacketHandler_GUQuestGiveUpRes(void *pPacket)
{
	sGU_QUEST_GIVEUP_RES* pResult = (sGU_QUEST_GIVEUP_RES*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Avatar_TS_GiveUp_Quest( pResult->wResultCode, pResult->tId );
}

void PacketHandler_GUTSUpdateState(void *pPacket)
{
	sGU_TS_UPDATE_STATE* pResult = (sGU_TS_UPDATE_STATE*)pPacket;

	switch ( pResult->byTsType )
	{
	case TS_TYPE_QUEST_CS:
		GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_TS_Update_State( pResult->tId, pResult->byType, pResult->wTSState, pResult->dwParam );
		break;
	case TS_TYPE_PC_TRIGGER_CS:
		GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency()->GU_TS_Update_State( pResult->tId, pResult->byType, pResult->wTSState, pResult->dwParam );
		break;
	default:
		CNtlTSLog::Log( "Not supported trigger system type. Info[%d,%d]. [%s]", pResult->tId, pResult->byTsType, TS_CODE_TRACE() );
		break;
	}
}

void PacketHandler_GUQuestShareNfy(void *pPacket)
{
	sGU_QUEST_SHARE_NFY* pResult = (sGU_QUEST_SHARE_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Quest_Share_Nfy( pResult->wResultCode, pResult->hActor, pResult->tId );
}

void PacketHandler_GUTSUpdateEventNfy(void* pPacket)
{
	sGU_TS_UPDATE_EVENT_NFY* pResult = (sGU_TS_UPDATE_EVENT_NFY*)pPacket;

	switch ( pResult->byTsType )
	{
	case TS_TYPE_QUEST_CS:
		GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_TS_Update_Event_Nfy( pResult->teid );
		break;
	case TS_TYPE_PC_TRIGGER_CS:
		GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency()->GU_TS_Update_Event_Nfy( pResult->teid );
		break;
	default:
		CNtlTSLog::Log( "Not supported trigger system type at TS Update Event. Info[%d]. [%s]", pResult->byTsType, TS_CODE_TRACE() );
		break;
	}
}

void PacketHandler_GUQuestResetNfy(void *pPacket)
{
	sGU_QUEST_RESET_NFY* pResult = (sGU_QUEST_RESET_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_TS_RemoveTMQQuest( pResult->startResetQID, pResult->endResetQID );
}

void PacketHandler_GUTSExcuteTrigger(void *pPacket)
{
	sGU_TS_EXCUTE_TRIGGER* pResult = (sGU_TS_EXCUTE_TRIGGER*)pPacket;

	switch ( pResult->byTsType )
	{
	case TS_TYPE_QUEST_CS:
		GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_TS_Excute_Trigger( pResult->tId );
		break;
	case TS_TYPE_PC_TRIGGER_CS:
	default:
		CNtlTSLog::Log( "Not supported trigger system type at TS Update Event. Info[%d]. [%s]", pResult->byTsType, TS_CODE_TRACE() );
		break;
	}
}

void PacketHandler_GUTSExcuteTriggerObjectRes(void *pPacket)
{
	sGU_TS_EXCUTE_TRIGGER_OBJECT_RES* pResult = (sGU_TS_EXCUTE_TRIGGER_OBJECT_RES*)pPacket;

	if ( RESULT_SUCCESS == pResult->wResultCode )
	{
		CNtlSobActor* pSobTObj = dynamic_cast< CNtlSobActor* > ( GetNtlSobManager()->GetSobObject( pResult->hTriggerObject ) );
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if ( pSobTObj && pSobAvatar && Logic_IsOperationTriggerActor( pSobTObj ) )
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( pSobAvatar->GetBehaviorData() );
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 

			pCtrlStuff->hTargetSerialId					= pSobTObj->GetSerialID();
			pCtrlStuff->uExtra.sOperate.hTargetSerialId	= pSobTObj->GetSerialID();
			pCtrlStuff->uExtra.sOperate.uiOperateTime	= 0;
			pCtrlStuff->uExtra.sOperate.uiTblId			= Logic_GetDirectIdTriggerActor( pSobTObj );

			CNtlSLEventGenerator::SobStateTransition( pSobAvatar->GetSerialID(), NTL_FSMSID_TRIGGER_OPERATE );
		}
	}
}

void PacketHandler_GUTSTriggerObjectDialogNfy(void *pPacket)
{
	sGU_TS_TRIGGER_OBJECT_DIALOG_NFY * pResult = (sGU_TS_TRIGGER_OBJECT_DIALOG_NFY*)pPacket;

	CNtlSobTriggerObject* pSobTObj = dynamic_cast< CNtlSobTriggerObject* > (GetNtlSobManager()->GetSobObject( pResult->hTriggerObject ) );
	CNtlSobTriggerObjectAttr* pSobTObjAttr = dynamic_cast< CNtlSobTriggerObjectAttr* > (pSobTObj->GetSobAttr() );

	if ( pSobTObj && pSobTObjAttr )
	{
		eOBJCONV_TYPE eObjConvType;
		if ( 0xff == pResult->byDialogType )
		{
			eObjConvType = eOBJCONV_TYPE_INVALID;
		}
		else
		{
			eObjConvType = (eOBJCONV_TYPE)pResult->byDialogType;
		}

		sTS_KEY sKey; sKey.Init();
		sKey.byTSType = TS_TYPE_OBJECT_TRIGGER_S;

		CNtlSLEventGenerator::QuestObjDialog_Req( sKey, eObjConvType, Logic_GetActiveWorldTableId(), pSobTObjAttr->GetTriggerObjectTbl()->tblidx, pResult->textTblidx );
	}
}

void PacketHandler_GUTSPCDialogNfy(void* pPacket)
{
	sGU_TS_PC_DIALOG_NFY* pResult = (sGU_TS_PC_DIALOG_NFY*)pPacket;

	CNtlSLEventGenerator::TSPCDialog_Nfy( pResult->textTblidx );
}

void PacketHandler_GUQuestObjectVisitRes(void* pPacket)
{
	sGU_QUEST_OBJECT_VISIT_RES* pResult = (sGU_QUEST_OBJECT_VISIT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
}

void PacketHandler_GUSkipContainer(void* pPacket)
{
	sGU_TS_SKIP_CONTAINER* pResult = (sGU_TS_SKIP_CONTAINER*)pPacket;

	CNtlSLEventGenerator::TSSkipContainer(pResult->tId, pResult->tcId);
}

void PacketHandler_GUQuestForceCompletion(void* pPacket)
{
	sGU_QUEST_FORCED_COMPLETION_NFY* pResult = (sGU_QUEST_FORCED_COMPLETION_NFY*)pPacket;

	GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GU_Qeust_Force_Completion( pResult->questId );
}

void PacketHandler_GSTradeStartNfy(void *pPacket)
{
	// 거래 요청을 받은 사람에게 거래 시작을 알린다
	sGU_TRADE_START_NFY* pResult = (sGU_TRADE_START_NFY*)pPacket;	

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetMsgBoxManager()->DeleteMsgBox( "DST_TRADE_ACCEPT_TRADE" );
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_START_NFY"), TRUE );
		return;
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_TRADE);

	CNtlWorldConceptTrade* pTradeConcept = reinterpret_cast<CNtlWorldConceptTrade*>( GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE) );
	NTL_ASSERT(pTradeConcept, "PacketHandler_GSTradeStartNfy, Not exist Trade world concept");
	pTradeConcept->SetOtherPCHandle(pResult->hTarget);

	CDboEventGenerator::UserTrade(USERTRADE_START, pResult->hTarget, pResult->handle, pResult->byEmptyInven);
}

void PacketHandler_GSTradeStartRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_START_RES );

	// 거래 요청을 한 사람에게 시작을 알린다
	sGU_TRADE_START_RES* pResult = (sGU_TRADE_START_RES*)pPacket;	

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_START_RES"), TRUE );
		return;
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_TRADE);

	CNtlWorldConceptTrade* pTradeConcept = reinterpret_cast<CNtlWorldConceptTrade*>( GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE) );
	NTL_ASSERT(pTradeConcept, "PacketHandler_GSTradeStartNfy, Not exist Trade world concept");
	pTradeConcept->SetOtherPCHandle(pResult->hTarget);

	CDboEventGenerator::UserTrade(USERTRADE_START, pResult->hTarget, pResult->handle, pResult->byEmptyInven);
}

void PacketHandler_GSTradeOKReq(void *pPacket)
{	
	// 다른 사람으로부터 거래를 신청받는다
	sGU_TRADE_OK_REQ* pResult = (sGU_TRADE_OK_REQ*)pPacket;

	CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(GetNtlSobManager()->GetSobObject(pResult->handle));
	CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobPlayer->GetSobAttr());

	if( Logic_CanAcceptOthersAsk() == FALSE )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(pResult->handle, ACCEPT_RES_TYPE_CANCEL);
		GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
		return;
	}
	else if(Logic_IsBlackList(pResult->handle)) // 블랙리스트에 등록되어 있는 경우 거절  
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(pResult->handle, ACCEPT_RES_TYPE_DENY);        
		GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);
		return;
	}

	// 질문을 한 PC의 핸들 저장
	GetDboGlobal()->SetAskedPCSerial(pResult->handle);

	//memset((char*)awcPacketMessageBuffer, 0, sizeof(WCHAR) * dPACKET_MEESAGE_LENGTH);
	//swprintf_s(awcPacketMessageBuffer, dPACKET_MEESAGE_LENGTH, GetDisplayStringManager()->GetString("DST_TRADE_ACCEPT_TRADE"), pSobPlayerAttr->GetName() );	

	// %s님의 거래 요청에 응하시겠습니까?
	//CDboEventGenerator::MsgBoxShow(awcPacketMessageBuffer, MBW_USER_TRADE_REQ, MBTF_OK | MBTF_CANCEL);
	GetAlarmManager()->FormattedAlarmMessage( "DST_TRADE_ACCEPT_TRADE", FALSE, NULL, pSobPlayerAttr->GetName() );
}

void PacketHandler_GSTradeAddNfy(void *pPacket)
{
	// 다른 사람이 아이템을 등록한다
	sGU_TRADE_ADD_NFY* pResult = (sGU_TRADE_ADD_NFY*)pPacket;

	CDboEventGenerator::UserTrade(USERTRADE_ADD_ITEM_NOTIFY, INVALID_SERIAL_ID, pResult->hItem, pResult->byCount, (void*)&pResult->sItem);
}

void PacketHandler_GSTradeAddRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_ADD_RES );

	// 자신이 아이템을 등록한다
	sGU_TRADE_ADD_RES* pResult = (sGU_TRADE_ADD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_ADD_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CDboEventGenerator::UserTrade(USERTRADE_ADD_ITEM, INVALID_SERIAL_ID, pResult->hItem, pResult->byCount);
}

void PacketHandler_GSTradeDelNfy(void *pPacket)
{
	// 다른 사람이 아이템을 등록해제한다
	sGU_TRADE_DEL_NFY* pResult = (sGU_TRADE_DEL_NFY*)pPacket;

	CDboEventGenerator::UserTrade(USERTRADE_DEL_ITEM_NOTIFY, INVALID_SERIAL_ID, pResult->hItem);
}

void PacketHandler_GSTradeDelRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_DEL_RES );

	// 내가 아이템을 등록해제한다
	sGU_TRADE_DEL_RES* pResult = (sGU_TRADE_DEL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_DEL_RES"), TRUE );
		return;
	}

	CDboEventGenerator::UserTrade(USERTRADE_DEL_ITEM, INVALID_SERIAL_ID, pResult->hItem);
}

void PacketHandler_GSTradeModifyNfy(void *pPacket)
{
	// 다른 사람이 아이템 갯수를 변경하였다
	sGU_TRADE_MODIFY_NFY* pResult = (sGU_TRADE_MODIFY_NFY*)pPacket;

	CDboEventGenerator::UserTrade(USERTRADE_UPDATE_ITEM_NOTIFY, INVALID_SERIAL_ID, pResult->hItem, pResult->byCount);
}

void PacketHandler_GSTradeModifyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_MODIFY_RES );

	// 내가 아이템을 아이템 갯수를 변경하였다
	sGU_TRADE_MODIFY_RES* pResult = (sGU_TRADE_MODIFY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_MODIFY_RES"), TRUE );
		return;
	}

	CDboEventGenerator::UserTrade(USERTRADE_UPDATE_ITEM, INVALID_SERIAL_ID, pResult->hItem, pResult->byCount);
}

void PacketHandler_GSTradeZennyUpdateNfy(void *pPacket)
{
	// 다른 사람이 제니를 변경하였다
	sGU_TRADE_ZENNY_UPDATE_NFY* pResult = (sGU_TRADE_ZENNY_UPDATE_NFY*)pPacket;

	CDboEventGenerator::UserTrade(USERTRADE_UPDATE_ZENNY_NOTIFY, INVALID_SERIAL_ID, pResult->dwZenny);
}

void PacketHandler_GSTradeZennyUpdateRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_ZENNY_UPDATE_RES );

	// 내가 제니를 변경하였다
	sGU_TRADE_ZENNY_UPDATE_RES* pResult = (sGU_TRADE_ZENNY_UPDATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_ZENNY_UPDATE_RES"), TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	CDboEventGenerator::UserTrade(USERTRADE_UPDATE_ZENNY, INVALID_SERIAL_ID, pResult->dwZenny);
}

void PacketHandler_GSTradeEndNfy(void *pPacket)
{
	// 상대방이 거래 준비를 끝났다
	sGU_TRADE_END_NFY* pResult = (sGU_TRADE_END_NFY*)pPacket;	

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_END_NFY"), TRUE );
		return;
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	CDboEventGenerator::UserTrade(USERTRADE_REDAY_NOTIFY, INVALID_SERIAL_ID, INVALID_SERIAL_ID, 0, (VOID*)&pResult->bIsSet);
}

void PacketHandler_GSTradeEndRes(void *pPacket)
{
	// 내가 거래 준비가 끝났다
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_END_RES );
	
	sGU_TRADE_END_RES* pResult = (sGU_TRADE_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_END_RES"), TRUE );
		return;
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	CDboEventGenerator::UserTrade(USERTRADE_REDAY, INVALID_SERIAL_ID, INVALID_SERIAL_ID, 0, (VOID*)&pResult->bIsSet);
}

void PacketHandler_GSTradeCancelNfy(void *pPacket)
{
	// 서버에서 다양한 이유로 GU_TRADE_START_RES의 응답 대신 GU_TRADE_CANCEL_NFY로 종료될 수 있다
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_START_RES );

	// 상대방이 거래를 종료하였다
	sGU_TRADE_CANCEL_NFY* pResult = (sGU_TRADE_CANCEL_NFY*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		// 종료되는 이유를 설명하고 남은 로직을 계속 진행한다
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_START_RES"), TRUE );
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	// 상대방이 거래를 종료하였습니다
	GetAlarmManager()->AlarmMessage("DST_TRADE_OTHER_CANCLE_TRADE");

	if( GetDialogManager()->IsOpenDialog(DIALOG_TRADECART) )
		CDboEventGenerator::UserTrade(USERTRADE_END);
}

void PacketHandler_GSTradeCancelRes(void *pPacket)
{
	// 내가 거래를 종료하였다

	// 서버에서 다양한 이유로 GU_TRADE_START_RES의 응답 대신 GU_TRADE_CANCEL_RES로 종료될 수 있다
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_START_RES );
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_CANCEL_RES );
	
	sGU_TRADE_CANCEL_RES* pResult = (sGU_TRADE_CANCEL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		// 종료되는 이유를 설명하고 남은 로직을 계속 진행한다
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_CANCEL_RES"), TRUE );
	}

	GetDboGlobal()->SetAskedPCSerial(INVALID_SERIAL_ID);

	if( GetDialogManager()->IsOpenDialog(DIALOG_TRADECART) )
		CDboEventGenerator::UserTrade(USERTRADE_END);
}

void PacketHandler_GSTradeDeclineRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TRADE_DENY_RES );

	// 내가 거래 신청을 받지 않도록 요청하였다
	sGU_TRADE_DENY_RES* pResult = (sGU_TRADE_DENY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TRADE_DENY_RES"), TRUE );
		return;
	}

	if( pResult->bIsDeny )
	{
		// 거래 신청을 받지 않는다
		CDboEventGenerator::UserTrade(USERTRADE_DECLINE, INVALID_SERIAL_ID);
	}
	else
	{
		// 거래 신청을 받는다
		CDboEventGenerator::UserTrade(USERTRADE_ACCEPT, INVALID_SERIAL_ID);
	}
}

void PacketHandler_GUTutorial_Hint_Update_Res(void *pPacket)
{
	sGU_TUTORIAL_HINT_UPDATE_RES *pResult = (sGU_TUTORIAL_HINT_UPDATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_TUTORIAL_HINT_UPDATE_RES"), TRUE );
		return;
	}

	CNtlSLEventGenerator::HelpHintEvent(pResult->dwTutorialHint);
}

void PacketHandler_GUTeleCast_Message_Beg_Nfy( void* pPacket ) 
{
    //sGU_TELECAST_MESSAGE_BEG_NFY* pResult = (sGU_TELECAST_MESSAGE_BEG_NFY*)pPacket;
 /*   CNtlSLEventGenerator::SLTeleCastMessageBeg_Nfy(pResult->npcTblidx,
                                                   pResult->byTelecastType,
                                                   pResult->speechTblidx,
                                                   pResult->dwDisplayTime);*/
}

void PacketHandler_GUTeleCast_Message_End_Nfy( void* pPacket ) 
{
 /*   sGU_TELECAST_MESSAGE_END_NFY* pResult = (sGU_TELECAST_MESSAGE_END_NFY*)pPacket;*/
    /*CNtlSLEventGenerator::SLTeleCastMessageEnd_Nfy();*/
}

void PacketHandler_GUBroad_Message_Beg_Nfy(void* pPacket)
{
	sGU_BROAD_MESSAGE_BEG_NFY* pResult = (sGU_BROAD_MESSAGE_BEG_NFY*)pPacket;

	CNtlSLEventGenerator::BroadMsgNfy_TSBeg( (eBROAD_MSG_UI_DIR_TYPE)pResult->byUIDirType,
											 (eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE)pResult->byUIShowHideTypel,
											 (eBROAD_MSG_UI_BALLOON_SHAPE_TYPE)pResult->byUIBalloonShapeType,
											 (eBROAD_MSG_UI_SPEECH_DIR_TYPE)pResult->byUISpeechDirType,
											 (eBROAD_MSG_OWNER_CONDITION)pResult->byOwnerCondition,
											 (eBROAD_MSG_OWNER_TYPE)pResult->byOwnerType,
											 pResult->uiOwnerTblIdx,
											 pResult->uiQuestTextTblidx,
											 pResult->fDisplayTime );

}

void PacketHandler_GUBroad_Message_End_Nfy(void* pPacket)
{
	CNtlSLEventGenerator::BroadMsgNfy_TSEnd();
}

void PacketHandler_GUMini_Narration(void* pPacket)
{
	sGU_MINI_NARRATION_NFY* pResult = (sGU_MINI_NARRATION_NFY*)pPacket;

	CNtlSLEventGenerator::BroadMsgNfy_MiniNarration( pResult->uiNarrtionTblidx );
}

void PacketHandler_GU_Progess_message_Nfy( void* pPacket ) 
{
	sGU_PROGRESS_MESSAGE_NFY* pResult = (sGU_PROGRESS_MESSAGE_NFY*)pPacket;

	// byProgressType, byMessageValue, byMessageType 순서로 받은 인자를
	// 2번째 3번째 값을 바꾸어서 적용한다
	CDboEventGenerator::FlashNotify(pResult->byProgressType, pResult->byMessageValue, pResult->byMessageType);
}

void PacketHandler_GU_Direction_Indicate_Nfy( void* pPacket ) 
{
	sGU_DIRECTION_INDICATE_NFY* pResult = (sGU_DIRECTION_INDICATE_NFY*)pPacket;

	RwV3d vLoc;

	if ( pResult->bIndicate )
	{
		vLoc.x = pResult->vLoc.x;
		vLoc.y = pResult->vLoc.y;
		vLoc.z = pResult->vLoc.z;    
	}
	else
	{
		vLoc.x = 0.f;
		vLoc.y = 0.f;
		vLoc.z = 0.f;
	}

	CNtlSLEventGenerator::SLDirection_Nfy(pResult->bIndicate, vLoc);
}

void PacketHandler_GSTObjectUpdateState(void* pPacket)
{
	sGU_TOBJECT_UPDATE_STATE* pResult = (sGU_TOBJECT_UPDATE_STATE*)pPacket;

	CNtlSLEventGenerator::TObjectUpdateState( pResult->handle,
											  pResult->tobjectBrief.objectID,
											  pResult->tobjectState.byState,
											  pResult->tobjectState.bySubStateFlag,
											  pResult->tobjectState.dwStateTime );
}

void PacketHandler_GSCharDialog(void* pPacket)
{
    sGU_CHAR_DIALOG* pResult = (sGU_CHAR_DIALOG*)pPacket;

	CDboEventGenerator::ServerCharDialog( pResult->hSubject, pResult->byDialogType, pResult->textTblidx, pResult->bIsRefSpeechTable, pResult->wTextLen, pResult->awchText );
}

void PacketHandler_GSCharDirectPlay(void* pPacket)
{
	sGU_CHAR_DIRECT_PLAY *pDirectPlay = (sGU_CHAR_DIRECT_PLAY*)pPacket;

	if(pDirectPlay->byPlayMode == DIRECTION_CINEMATIC)
	{
		CDirectionLinkTable *pLinkTbl = API_GetTableContainer()->GetDirectionLinkTable();
		if(pLinkTbl == NULL)
			return;

		sDIRECTION_LINK_TBLDAT *pLinkTblData = reinterpret_cast<sDIRECTION_LINK_TBLDAT*>( pLinkTbl->FindData(pDirectPlay->directTblidx) );
		if(pLinkTblData == NULL)
			return;

		std::string strFunc = pLinkTblData->szFunctionName;
		strFunc += "()";
		DBO_WARNING_MESSAGE("Start directplay: " << strFunc.c_str());
		CNtlLuaState *pLuaState = GetLuaState();
		pLuaState->RunString(strFunc.c_str());

		if (pDirectPlay->bCanSkip)
			GetNtlDTCinematicManager()->SetCanSkip(true);
	}
}

void PacketHandler_GSCharDirectPlayCancelNfy(void * pPacket)
{
	sGU_CHAR_DIRECT_PLAY_CANCEL_NFY *pNfy = (sGU_CHAR_DIRECT_PLAY_CANCEL_NFY*)pPacket;

	// Event
	CNtlPLEventGenerator::EventDirectPlayCancel(pNfy->hPc);
}

void PacketHandler_GSCharDirectPlayEnd(void* pPacket)
{
	GetNtlDTCinematicManager()->Clear();
}

void PacketHandler_GUDBCCheckRes( void* pPacket ) 
{
	API_GetSLPacketLockManager()->Unlock(GU_DRAGONBALL_CHECK_RES);

	sGU_DRAGONBALL_CHECK_RES* pResult = (sGU_DRAGONBALL_CHECK_RES*)pPacket;

	CNtlSLEventGenerator::DBCCheck_Res(pResult->wResultCode, pResult->hObject);
}

void PacketHandler_GUDBCRewardRes( void* pPacket ) 
{	
	API_GetSLPacketLockManager()->Unlock(GU_DRAGONBALL_REWARD_RES);

	sGU_DRAGONBALL_REWARD_RES* pResult = (sGU_DRAGONBALL_REWARD_RES*)pPacket;

	CNtlSLEventGenerator::DBCReward_Res(pResult->wResultCode, pResult->hObject);
}

void PacketHandler_GUDBCCollectNfy( void* pPacket ) 
{
	sGU_DRAGONBALL_COLLECT_NFY* pResult = (sGU_DRAGONBALL_COLLECT_NFY*)pPacket;

	CNtlSLEventGenerator::DBCCollect_Nfy(pResult->wResultCode, pResult->hObject);
}

void PacketHandler_GUDBCSchedule( void* pPacket ) 
{
	sGU_DRAGONBALL_SCHEDULE_INFO* pResult = (sGU_DRAGONBALL_SCHEDULE_INFO*)pPacket;
	
	SNtlEventDBCSchedule_Info sDBCScheduleInfo;
	sDBCScheduleInfo.bIsAlive = pResult->bIsAlive;
	sDBCScheduleInfo.byEventType = pResult->byEventType;
	sDBCScheduleInfo.nStartTime = pResult->nStartTime;
	sDBCScheduleInfo.nEndTime = pResult->nEndTime;
	sDBCScheduleInfo.dwMainTerm = pResult->dwMainTerm;
	sDBCScheduleInfo.dwSubTerm = pResult->dwSubTerm;
	sDBCScheduleInfo.byTermType = pResult->byTermType;

	GetDboGlobal()->SetDBCScheduleInfo(sDBCScheduleInfo);

	CNtlSLEventGenerator::DBCSchedule_Info(pResult->bIsAlive, pResult->byEventType, pResult->nStartTime,
										   pResult->nEndTime, pResult->dwMainTerm, pResult->dwSubTerm,
										   pResult->byTermType);
}

void PacketHandler_TimeQuestEnterRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ENTER_RES);

	sGU_TIMEQUEST_ENTER_RES* pResult = (sGU_TIMEQUEST_ENTER_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "GU_TIMEQUEST_ENTER_RES").c_str());
		return;
	}

	// Event

}

void PacketHandler_GUAuctionHouseListRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TENKAICHIDAISIJYOU_LIST_RES);

	sGU_TENKAICHIDAISIJYOU_LIST_RES* pResult = (sGU_TENKAICHIDAISIJYOU_LIST_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "GU_TENKAICHIDAISIJYOU_LIST_RES").c_str());
		return;
	}

	CNtlSLEventGenerator::AuctionHouseListRes(pResult->uiPage, pResult->uiMaxPage, pResult->byListNum, pResult->byPage);
}

void PacketHandler_GUAuctionHouseData(void * pPacket)
{
	sGU_TENKAICHIDAISIJYOU_LIST_DATA* pResult = (sGU_TENKAICHIDAISIJYOU_LIST_DATA*)pPacket;

	CNtlSLEventGenerator::AuctionHouseListData(&pResult->sData);
}

void PacketHandler_GUAuctionHouseSellRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TENKAICHIDAISIJYOU_SELL_RES);

	sGU_TENKAICHIDAISIJYOU_SELL_RES* pResult = (sGU_TENKAICHIDAISIJYOU_SELL_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "GU_TENKAICHIDAISIJYOU_SELL_RES").c_str());
		return;
	}

	CNtlSLEventGenerator::AuctionHouseSellRes(pResult->byPlace, pResult->byPosition, pResult->byCount);
}

void PacketHandler_GUAuctionHouseSellCancelRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES);

	sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES* pResult = (sGU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "GU_TENKAICHIDAISIJYOU_SELL_CANCEL_RES").c_str());
		return;
	}

	CNtlSLEventGenerator::AuctionHouseSellCancelRes(pResult->nItem);
}

void PacketHandler_GUAuctionHouseBuyRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TENKAICHIDAISIJYOU_BUY_RES);

	sGU_TENKAICHIDAISIJYOU_BUY_RES* pResult = (sGU_TENKAICHIDAISIJYOU_BUY_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "GU_TENKAICHIDAISIJYOU_BUY_RES").c_str());
		return;
	}

	// set zeni
	Logic_SetZenny(pResult->dwMoney);

	// Event
	CNtlSLEventGenerator::AuctionHouseBuyRes(pResult->nItem);
}

void PacketHandler_GUCharAwayRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CHAR_AWAY_RES);

	sGU_CHAR_AWAY_RES* pResult = (sGU_CHAR_AWAY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		return;
	}

	CNtlSLEventGenerator::CharAway(pResult->bIsAway);
}

void PacketHandler_GUCharMailInfo(void* pPacket)
{
	sGU_CHAR_MAIL_INFO* pResult = (sGU_CHAR_MAIL_INFO*)pPacket;

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	pAvatarInfo->byMailCount				= pResult->byMailCount;
	pAvatarInfo->byUnreadMailCountNormal	= pResult->byNormalCount;
	pAvatarInfo->byUnreadMailCountManager	= pResult->byManagerCount;
}

// ActionMap
void PacketHandler_GUCharKeyInfo(void *pPacket)
{
	sGU_CHAR_KEY_INFO* pData = (sGU_CHAR_KEY_INFO*)pPacket;

	CNtlSLEventGenerator::ActionMapLoadInfo( pData->byCount, pData->asData );
}

void PacketHandler_GUCharKeyUpdateRes( void* pPacket ) 
{
	sGU_CHAR_KEY_UPDATE_RES* pResult = (sGU_CHAR_KEY_UPDATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
		CNtlSLEventGenerator::ActionMapUpdateRes( FALSE );
	}

	CNtlSLEventGenerator::ActionMapUpdateRes( TRUE );
}


void PacketHandler_GUPortalInfoRes( void* pPacket ) 
{
	// receive portal info
	sGU_PORTAL_INFO* pResult = (sGU_PORTAL_INFO*)pPacket;

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	memset(pAvatarInfo->aPortalID, INVALID_BYTE, sizeof(pAvatarInfo->aPortalID));

	for (BYTE i = 0; i < pResult->byCount; i++)
	{
		pAvatarInfo->aPortalID[i] = pResult->aPortalID[i];
	}

	//CNtlSLEventGenerator::PortalInfoRes(pResult->byCount, pResult->aPortalID)
}

void PacketHandler_GUPortalAddRes( void* pPacket ) 
{
	API_GetSLPacketLockManager()->Unlock(GU_PORTAL_ADD_RES);

	sGU_PORTAL_ADD_RES* pResult = (sGU_PORTAL_ADD_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PORTAL_ADD_RES"), TRUE);
		return;
	}

	// Event
	CNtlSLEventGenerator::PortalAddRes(pResult->PortalID);
}

void PacketHandler_GUPortalRes( void* pPacket ) 
{
	API_GetSLPacketLockManager()->Unlock(GU_PORTAL_RES);

	sGU_PORTAL_RES* pResult = (sGU_PORTAL_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_PORTAL_RES"), TRUE);
		return;
	}

	// Event
	CNtlSLEventGenerator::PortalRes(pResult->byPoint);
}

void PacketHandler_GUBotCautionNfy( void* pPacket ) 
{
    sGU_BOT_BOTCAUTION_NFY* pData = (sGU_BOT_BOTCAUTION_NFY*)pPacket;
    
    CNtlSLEventGenerator::BotCaution_Nfy(pData->hBot);
}

void PacketHandler_GUBotHelpMeNfy( void* pPacket ) 
{
    sGU_BOT_BOTCAUTION_HELPME_NFY* pData = (sGU_BOT_BOTCAUTION_HELPME_NFY*)pPacket;

    CNtlSLEventGenerator::BotHelpMe_Nfy(pData->hBot, pData->bIsRequester);
}

void PacketHandler_GSTutorial_PlayQuitRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_TUTORIAL_PLAY_QUIT_RES );

	sGU_TUTORIAL_PLAY_QUIT_RES* pTutorialPlayQuitRes = (sGU_TUTORIAL_PLAY_QUIT_RES*)pPacket;

	if(pTutorialPlayQuitRes->wResultCode != GAME_SUCCESS)
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pTutorialPlayQuitRes->wResultCode, "GU_TUTORIAL_PLAY_QUIT_RES"), TRUE );
}

void PacketHandler_GURideOnBusRes( void* pPacket ) 
{
    API_GetSLPacketLockManager()->Unlock(GU_RIDE_ON_BUS_RES);

    sGU_RIDE_ON_BUS_RES* pResult = (sGU_RIDE_ON_BUS_RES*)pPacket;

    if( pResult->wResultCode != GAME_SUCCESS )
    {
        CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
        return;
    }

	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(pResult->hTarget);
	if( pNPC_TBLDAT )
	{
		GetAlarmManager()->FormattedAlarmMessage("DST_BUS_SUCCESS_GET_ON", FALSE, NULL, pNPC_TBLDAT->amerchant_Tblidx[0] );
	}

    // NOTE: 성공후에는 스테이트 변환이 날라오기 때문에, 실제 처리는 스테이트에서 한다.
}

void PacketHandler_GURideOffBusRes( void* pPacket ) 
{
    API_GetSLPacketLockManager()->Unlock(GU_RIDE_OFF_BUS_RES);

    sGU_RIDE_OFF_BUS_RES* pResult = (sGU_RIDE_OFF_BUS_RES*)pPacket;

    if( pResult->wResultCode != GAME_SUCCESS )
    {
        CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
        return;
    }

    // NOTE: 성공후에는 스테이트 변환이 날라오기 때문에, 실제 처리는 스테이트에서 한다.
}

void PacketHandler_GUAfterEffectRemainTimeNfy( void* pPacket ) 
{
    sGU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY* pData = (sGU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY*)pPacket;
    CNtlSLEventGenerator::SobAfterEffectRemainTimeNfy(pData->dwTimeRemaining);
}

void PacketHandler_GUTransformCancelRes( void* pPacket ) 
{
    sGU_TRANSFORM_CANCEL_RES* pResult = (sGU_TRANSFORM_CANCEL_RES*)pPacket;

    if(pResult->wResultCode != GAME_SUCCESS)
    {
        CNtlSLEventGenerator::SysMsg(INVALID_SERIAL_ID, Logic_GetResultCodeString(pResult->wResultCode, "").c_str());
        return;
    }
    else
    {
        sASPECTSTATE state;
        state.sAspectStateBase.byAspectStateId = ASPECTSTATE_INVALID;
        CNtlSLEventGenerator::SobTransform(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), state);
    }
}

void PacketHandler_GUSkillConfuseTragetNfy( void* pPacket ) 
{
    sGU_SKILL_CONFUSE_TARGET_NFY* pData = (sGU_SKILL_CONFUSE_TARGET_NFY*)pPacket;
    CNtlSLEventGenerator::SobCondConfused(TRUE, pData->hSubject, pData->hTarget);
}

void PacketHandler_GUSkillTerrorCourseChangeNfy( void* pPacket ) 
{
    sGU_SKILL_TERROR_COURSE_CHANGE_NFY* pData = (sGU_SKILL_TERROR_COURSE_CHANGE_NFY*)pPacket;
    CNtlSLEventGenerator::SobCondTerror(TRUE, pData->hSubject, pData->hAttacker);
}

void PacketHandler_GUCharRankBattleScore( void* pPacket )
{
	sGU_CHAR_RANKBATTLE_SCORE* pData = (sGU_CHAR_RANKBATTLE_SCORE*)pPacket;

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	pAvatarInfo->sRankBattleScoreInfo.wMaxStraightWin = pData->sScoreInfo.wMaxStraightWin;
	pAvatarInfo->sRankBattleScoreInfo.wStraightWin	= pData->sScoreInfo.wStraightWin;
	pAvatarInfo->sRankBattleScoreInfo.wMaxStraightKOWin	= pData->sScoreInfo.wMaxStraightKOWin;
	pAvatarInfo->sRankBattleScoreInfo.wStraightKOWin	= pData->sScoreInfo.wStraightKOWin;
	pAvatarInfo->sRankBattleScoreInfo.dwWin	= pData->sScoreInfo.dwWin;
	pAvatarInfo->sRankBattleScoreInfo.dwDraw = pData->sScoreInfo.dwDraw;
	pAvatarInfo->sRankBattleScoreInfo.dwLose = pData->sScoreInfo.dwLose;
	pAvatarInfo->sRankBattleScoreInfo.fPoint = pData->sScoreInfo.fPoint;
}


void PacketHandler_GUDisconnectNfy( void* pPacket ) 
{
	sGU_DISCONNECTED_NFY* pResult = (sGU_DISCONNECTED_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DISCONNECTED_NFY"), TRUE );

	SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
	pConnectData->sGameCon.bBanishment = true;
}
 
void PacketHandler_GUDiceRollRes( void* pPacket ) 
{
	API_GetSLPacketLockManager()->Unlock(GU_DICE_ROLL_RES);

	sGU_DICE_ROLL_RES* pResult = (sGU_DICE_ROLL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DICE_ROLL_RES"), TRUE );
		return;
	}

	CDboEventGenerator::DiceResult(Logic_GetAvatarHandle(), DICE_WORKD_ID_ACTION, (RwUInt8)pResult->wDiceResult);
}

void PacketHandler_GUDiceRolledNfy( void* pPacket ) 
{
	sGU_DICE_ROLLED_NFY* pResult = (sGU_DICE_ROLLED_NFY*)pPacket;
	CDboEventGenerator::DiceResult(pResult->hSubject, DICE_WORKD_ID_ACTION, (RwUInt8)pResult->wDiceResult);
}

void PacketHandler_GUShopGambleBuyRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_GAMBLE_BUY_RES );

	sGU_SHOP_GAMBLE_BUY_RES* pResult = (sGU_SHOP_GAMBLE_BUY_RES*)pPacket;

	CDboEventGenerator::ShopGambleBuyRes( pResult->wResultCode, pResult->handle, pResult->hItem );

	CNtlSLEventGenerator::TSSlotMachine();
}

void PacketHandler_GUTeleportProposalNfy( void* pPacket )
{
	sGU_TELEPORT_PROPOSAL_NFY* pNotify = (sGU_TELEPORT_PROPOSAL_NFY*)pPacket;

	// 구조체를 한번에 날린다.
	SDboEventTeleportProposalNfy data;
	data.byTeleportType = pNotify->byTeleportType;
	data.byInfoIndex = pNotify->byInfoIndex;
	data.wWaitTime = pNotify->wWaitTime;
	data.worldTblidx = pNotify->worldTblidx;
	
	data.dwReserve = pNotify->dwReserve;
	data.byBudokaiMatchDepth = pNotify->byBudokaiMatchDepth;

	NTL_SAFE_WCSCPY(data.wszSummonnerName, pNotify->wszSummonnerName);

	CDboEventGenerator::TeleportProposalNfy( &data );
}

void PacketHandler_GUTeleportConfirmRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_TELEPORT_CONFIRM_RES);

	sGU_TELEPORT_CONFIRM_RES* pResult = (sGU_TELEPORT_CONFIRM_RES*)pPacket;

	SDboEventTeleportConfirmRes data;
	data.wResultCode = pResult->wResultCode;
	data.byTeleportIndex = pResult->byTeleportIndex;
	data.bTeleport = pResult->bTeleport;
	data.bClearInterface = pResult->bClearInterface;

	// ResultCode 검사 등 모든 것을 TeleportProposalManager에서 받아서 처리한다.

	CDboEventGenerator::TeleportConfirmRes( &data );
}

void PacketHandler_NickNameChangedNfy(void* pPacket)
{
	/*sGU_CHAR_NICKNAME_CHANGED_NFY* pResult = (sGU_CHAR_NICKNAME_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::ChangeNickName( pResult->hSubject, pResult->nicknameTblidx );*/
}

void PacketHandler_SystemDisplayResultCode(void* pPacket)
{
	sGU_SYSTEM_DISPLAY_RESULTCODE* pResult = (sGU_SYSTEM_DISPLAY_RESULTCODE*)pPacket;

	const WCHAR* szText = GetDisplayStringManager()->GetString(Logic_GetResultCodeString(pResult->wResultCode, "GU_SYSTEM_DISPLAY_RESULTCODE"), CDisplayStringManager::SERVER_SYNC_RESULT_MSG );
	if ( NULL == szText ) return;

	std::wstring wstrMessage = szText;

	int nMsgLen = wstrMessage.size();
    nMsgLen = (nMsgLen < NTL_MAX_LENGTH_OF_CHAT_MESSAGE ? nMsgLen : NTL_MAX_LENGTH_OF_CHAT_MESSAGE);

	WCHAR pBuffer[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
	memset( pBuffer, 0, sizeof( pBuffer ) );
	memcpy( pBuffer, szText, sizeof( WCHAR ) * nMsgLen ); 

	if ( pResult->byDisplayType == SERVER_TEXT_SYSTEM )
	{
		gui::RemoveLineFeed( wstrMessage );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", (RwUInt16)wstrMessage.size(), wstrMessage.c_str() );	
	}
	else if ( pResult->byDisplayType == SERVER_TEXT_NOTICE )
	{
		CDboEventGenerator::NotifyMessage( SDboEventNotify::PUBLIC, pBuffer );
	}
	else if ( pResult->byDisplayType == SERVER_TEXT_SYSNOTICE )
	{
		gui::RemoveLineFeed( wstrMessage );
		CDboEventGenerator::SayMessage( CHAT_TYPE_SYSTEM, L"", (RwUInt16)wstrMessage.size(), wstrMessage.c_str() );	

		CDboEventGenerator::NotifyMessage( SDboEventNotify::PUBLIC, pBuffer );
	}
	else if( pResult->byDisplayType == SERVER_TEXT_EMERGENCY )
	{
		CNtlSLEventGenerator::BroadMsgNfy_Emergency( wstrMessage );
    }
	else if (pResult->byDisplayType == SERVER_TEXT_UNKNOWN)
	{
		//CNtlSLEventGenerator::BroadMsgNfy_Emergency(wstrMessage);// TODO
	}
	else
	{
		NTL_ASSERTFAIL( "DisplayNotice Type is Wrong" );
	}
}

void PacketHandler_GSCharPushedNfy( void* pPacket ) 
{
    /*sGU_CHAR_PUSHED_NFY* pData = (sGU_CHAR_PUSHED_NFY*)pPacket;
    RwV3d vDestLoc;    
    vDestLoc.x = pData->vDestLoc.x;
    vDestLoc.y = pData->vDestLoc.y;
    vDestLoc.z = pData->vDestLoc.z;
    
    CNtlSLEventGenerator::SobPushingNfy(pData->hSubject, pData->byAttackResult, pData->wAttackResultValue, vDestLoc);*/
}

void PacketHandler_GUHTBLastStepChangedNfy(void *pPacket)
{
	sGU_HTB_LAST_STEP_CHANGED_NFY* pHTBRPBall = (sGU_HTB_LAST_STEP_CHANGED_NFY*)pPacket;

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pHTBRPBall->hSubject) );

	if(pActor == NULL)
		return;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();

	pHTBStuff->byStepCount = pHTBRPBall->byLastStep;
}

void PacketHandler_GSHTBRPBallResultDecidedNfy( void* pPacket ) 
{
	sGU_HTB_RP_BALL_RESULT_DECIDED_NFY* pHTBRPBall = (sGU_HTB_RP_BALL_RESULT_DECIDED_NFY*)pPacket;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	SERIAL_HANDLE hAvatarSerialId = pSobAvatar->GetSerialID();

	RwBool bWin				= FALSE;
	RwBool bAttacker		= FALSE;
	RwInt32 iAttackPoint	= pHTBRPBall->bySubjectRpBallUsed;
	RwInt32 iDefenderPoint	= pHTBRPBall->byTargetRpBallUsed;
	
	// 공격자 인가?
	if(hAvatarSerialId == pHTBRPBall->hAttacker)
	{
		bAttacker = TRUE;
	}

	if(hAvatarSerialId == pHTBRPBall->hWinner)
	{
		bWin = TRUE;
	}

	if(!pHTBRPBall->bHasSubjectRpBallUsed)
	{
		iAttackPoint = -1;
	}

	if(!pHTBRPBall->bHasTargetRpBallUsed)
	{
		iDefenderPoint = -1;
	}

	CNtlSLEventGenerator::HTBRPResult(iAttackPoint, iDefenderPoint, bWin, bAttacker);
}

void PacketHandler_GURPIncreaseStartNfy( void* pPacket )
{
	CDboEventGenerator::RPCharge( TRUE, FALSE );
}

void PacketHandler_GURPDecreaseStartNfy( void* pPacket )
{
	CDboEventGenerator::RPCharge( FALSE, FALSE );
}

void PacketHandler_GURPIncreaseStopNfy( void* pPacket )
{
	CDboEventGenerator::RPCharge( TRUE, TRUE );
}

void PacketHandler_GUCharIsBattleCombating(void * pPacket)
{
	sGU_CHAR_IS_BATTLECOMBATING* pRes = (sGU_CHAR_IS_BATTLECOMBATING*)pPacket;

	CDboEventGenerator::BattleCombatMode(pRes->bIsBattle);
}

void PacketHandler_GUDynamicObjectUpdateState( void* pPacket )
{
	sGU_DYNAMIC_OBJECT_UPDATE_STATE* pState = (sGU_DYNAMIC_OBJECT_UPDATE_STATE*)pPacket;

	CNtlSLEventGenerator::DynamicObjectUpdateState( pState->handle, 
		&pState->dynamicObjBrief,
		&pState->dynamicObjState );
}

void PacketHandler_GUGMBudokaiServerEnterRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES);

	sGU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES* pResult = (sGU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES"), TRUE );
		return;
	}
}

void PacketHandler_GUGMBudokaiServerLeaveRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES);

	sGU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES* pResult = (sGU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES"), TRUE );
		return;
	}
}

void PacketHandler_GUGMBudokaiMatchProgressInfoReq( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES);
	CDboEventGenerator::ObserverMenu(OBSERVER_TENKAICHI_MATCH_DATA_RES);

	sGU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES* pResult = (sGU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES"), TRUE );
		return;
	}

	CGMQuickSlotGui* pGMQuickSlot = (CGMQuickSlotGui*)GetDialogManager()->GetDialog(DIALOG_GM_QUICKSLOT);
	if( pGMQuickSlot )
	{
		CGMPopupGui* pGMPopupGui = pGMQuickSlot->GetPopupGui();
		if( pGMPopupGui )
		{
			pGMPopupGui->SetBudokaiInfo(pResult->byMatchType, pResult->byMatchWorldType, pResult->byMatchDepth,
										pResult->byStateCount, pResult->aStateList);
		}
	}
	
	CDboEventGenerator::ObserverMenu(OBSERVER_TENKAICHI_MATCH_DATA, INVALID_SERIAL_ID, pResult->byMatchType);
}

void PacketHandler_GUGMBudokaiMatchArenaEnterReq( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES);

	sGU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES* pResult = (sGU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES"), TRUE );
		return;
	}
}

void PacketHandler_GUGMBudokaiMatchArenaLeaveReq( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES);

	sGU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES* pResult = (sGU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES"), TRUE );
		return;
	}
}

void PacketHandler_GUMonsterTransformedNfy( void* pPacket ) 
{
    sGU_MONSTER_TRANSFORMED_NFY* pData = (sGU_MONSTER_TRANSFORMED_NFY*)pPacket;

    CNtlSLEventGenerator::SobMobTransform(pData->hSubject, pData->newTblidx);
}

void PacketHandler_GSChildAdultSwitchedNfy( void* pPacket ) 
{
    sGU_CHILD_ADULT_SWITCHED_NFY* pData = (sGU_CHILD_ADULT_SWITCHED_NFY*)pPacket;

    CNtlSLEventGenerator::SobChangeAdult(pData->hSubject, pData->bIsAdult);

	if( pData->hSubject == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
	{
		if(pData->bIsAdult)
			GetAlarmManager()->AlarmMessage( "DST_CHAR_GROWN_UP" );
		else
			GetAlarmManager()->AlarmMessage("DST_CHAR_GROWN_DOWN");
	}
}

void PacketHandler_GSChangeClassAuthorityChangedNfy(void* pPacket)
{
	sGU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY* pData = (sGU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY*)pPacket;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
	if( pAvatarAttr )
		pAvatarAttr->m_bCanChangeClass = pData->bCanChangeClass;

	CNtlSLEventGenerator::ChangeClassAuthorityChanged( pData->bCanChangeClass );
}

void PacketHandler_GUSoundPlayBGM(void* pPacket)
{
	sGU_PLAY_BGM* pResult = (sGU_PLAY_BGM*)pPacket;

	GetNtlSoundMoodManager()->Condition_AskPlay_from_Server(pResult);
}

void PacketHandler_GUCharUpdateNetPy( void* pPacket ) 
{
    sGU_UPDATE_CHAR_NETP* pData = (sGU_UPDATE_CHAR_NETP*)pPacket;        
    
    CDboEventGenerator::UpdateNetPy(pData->netP, pData->dwAccumulationNetP, pData->timeNextGainTime);

    // 기존값과 비교를 위해서, 세팅은 뒤에 한다.
    Logic_SetNetPy(pData->netP);
}

void PacketHandler_GUBusLocationNfy( void* pPacket ) 
{
	// 아바타가 위치하고 있는 존의 버스 위치, 방향 정보를 받았다
	sGU_BUS_LOCATION_NFY* pResult = (sGU_BUS_LOCATION_NFY*)pPacket;

	RwV3d v3Pos = {pResult->vCurLoc.x, 0.f, pResult->vCurLoc.z};
	RwV3d v3Dir = {pResult->vCurDir.x, 0.f, pResult->vCurDir.z};
	
	CNtlSLEventGenerator::BusMove(BUS_MOVE_POS_DIR, pResult->hSubject, pResult->busTblidx, &v3Pos, &v3Dir);
}

void PacketHandler_GUBusLocationErasedNfy( void* pPacket ) 
{
	// 아바타가 위치하고 있는 존의 버스가 사라졌다(혹은 디스폰)
	sGU_BUS_LOCATION_ERASED_NFY* pResult = (sGU_BUS_LOCATION_ERASED_NFY*)pPacket;

	CNtlSLEventGenerator::BusMove(BUS_MOVE_DELETE_BUS, pResult->hSubject, INVALID_TBLIDX, NULL, NULL);
}

void PacketHandler_GUBusLocationResetAllNfy( void* pPacket ) 
{
	// 기존에 서버로 부터 받았던 버스의 위치, 방향 정보를 모두 지운다
	//sGU_BUS_LOCATION_RESET_ALL_NFY* pResult = (sGU_BUS_LOCATION_RESET_ALL_NFY*)pPacket;

	CNtlSLEventGenerator::BusMove(BUS_MOVE_RESET, INVALID_SERIAL_ID, INVALID_TBLIDX, NULL, NULL);
}

void PacketHandler_GUItemExpiredNfy(void* pPacket)
{
	sGU_ITEM_EXPIRED_NFY* pResult = (sGU_ITEM_EXPIRED_NFY*)pPacket;

	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pResult->hItem ) );	

	if( pSobItem )
	{
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		pSobItemAttr->Expired();

		sMsgBoxData data;
		data.sItemInfo.hSerial = pResult->hItem;
		data.sItemInfo.ePlace = PLACE_BAG;
		data.sItemInfo.nPlaceIdx = pSobItem->GetParentItemSlotIdx();
		data.sItemInfo.nSlotIdx = pSobItem->GetItemSlotIdx();

		CNtlSLEventGenerator::SobUpdate( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), EVENT_AIUT_ITEM, 0 );
		GetAlarmManager()->FormattedAlarmMessage( "DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG", FALSE, &data, API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText( pSobItemAttr->GetItemTbl()->Name ).c_str() );
	}	
	else
	{
		DBO_ASSERTE( "PacketHandler_GUItempExpredNfy : Item Handle is invalid" );
		// Place, Pos로 다시 한번 검사
	}
}

void PacketHandler_GUCharRefreshBattleAttributeModifier( void* pPacket )
{
	sGU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER* pNotify = (sGU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER*)pPacket;

	CNtlSLEventGenerator::CharRefreshBattleAttributeModifier(
		pNotify->hSubject,
		pNotify->fHonestOffenceModifier, pNotify->fHonestDefenceModifier,
		pNotify->fStrangeOffenceModifier, pNotify->fStrangeDefenceModifier,
		pNotify->fWildOffenceModifier, pNotify->fWildDefenceModifier,
		pNotify->fEleganceOffenceModifier, pNotify->fEleganceDefenceModifier,
		pNotify->fFunnyOffenceModifier, pNotify->fFunnyDefenceModifier );
}

void PacketHandler_GUCharRankPointResetNfy( void* pPacket )
{
	//sGU_CHAR_RANKPOINT_RESET_NFY* pNotify = (sGU_CHAR_RANKPOINT_RESET_NFY*)pPacket;

	CNtlSLEventGenerator::CharRankPointResetNfy();
}

void PacketHandler_GUNetMarbleMemberShipNfy( void* pPacket ) 
{
    //sGU_NETMARBLEMEMBERIP_NFY* pMsg = (sGU_NETMARBLEMEMBERIP_NFY*)pPacket;

    CDboEventGenerator::NetMarbleMemberShipNfy();
}

void PacketHandler_GUInventoryCapsuleUnlockNfy( void* pPacket )
{
	//sGU_INVENTORY_CAPSULE_UNLOCK_NFY* pNotify = (sGU_INVENTORY_CAPSULE_UNLOCK_NFY*)pPacket;

	CDboEventGenerator::CapsuleLockItem( FALSE, PLACE_BAG, 0 );
}

void PacketHandler_GUShopNetPyItemStartRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_NETPYITEM_START_RES);

	sGU_SHOP_NETPYITEM_START_RES* pResult = (sGU_SHOP_NETPYITEM_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_NETPYITEM_START_RES"), TRUE );
		return;
	}

	// Event 
	CDboEventGenerator::NetPyShopEvent( eNETPYSHOP_EVENT_START, pResult->byType );
}

void PacketHandler_GUShopNetPyItemBuyRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_NETPYITEM_BUY_RES);

	sGU_SHOP_NETPYITEM_BUY_RES* pResult = (sGU_SHOP_NETPYITEM_BUY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_NETPYITEM_BUY_RES"), TRUE );
		return;
	}

	// Event
	CDboEventGenerator::NetPyShopEvent( eNETPYSHOP_EVENT_BUY_SUCCESS, INVALID_SERIAL_ID );
}

void PacketHandler_GUShopNetPyItemEndRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_SHOP_NETPYITEM_END_RES);

	sGU_SHOP_NETPYITEM_END_RES* pResult = (sGU_SHOP_NETPYITEM_END_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_SHOP_NETPYITEM_END_RES"), TRUE );
		return;
	}

	// Event
	CDboEventGenerator::NetPyShopEvent( eNETPYSHOP_EVENT_END, INVALID_SERIAL_ID );
}

void PacketHandler_GUDurationItemBuyRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_DURATION_ITEM_BUY_RES);

	sGU_DURATION_ITEM_BUY_RES* pResult = (sGU_DURATION_ITEM_BUY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DURATION_ITEM_BUY_RES"), TRUE );
		return;
	}
	/// TBLIDX merchantTblidx, BYTE byPos;
}

void PacketHandler_GUDurationRenewRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_DURATION_RENEW_RES);

	sGU_DURATION_RENEW_RES* pResult = (sGU_DURATION_RENEW_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DURATION_RENEW_RES"), TRUE );
		return;
	}
}

void PacketHandler_GUCashItemHLShopStartRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_HLSHOP_START_RES);

	sGU_CASHITEM_HLSHOP_START_RES* pResult = (sGU_CASHITEM_HLSHOP_START_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_HLSHOP_START_RES"), TRUE);
		return;
	}

	// Event
	CDboEventGenerator::HLShopEvent(eHLSHOP_EVENT_START, pResult->dwRemainAmount);
}

void PacketHandler_GUCashItemHLShopEndRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_HLSHOP_END_RES);

	sGU_CASHITEM_HLSHOP_END_RES* pResult = (sGU_CASHITEM_HLSHOP_END_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_HLSHOP_END_RES"), TRUE);
		return;
	}

	// Event
	CDboEventGenerator::HLShopEvent(eHLSHOP_EVENT_END, 0);
}

void PacketHandler_GUCashItemHLShopRefreshRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_HLSHOP_REFRESH_RES);

	sGU_CASHITEM_HLSHOP_REFRESH_RES* pResult = (sGU_CASHITEM_HLSHOP_REFRESH_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_HLSHOP_REFRESH_RES"), TRUE);
		return;
	}

	// Event
	CDboEventGenerator::HLShopEvent(eHLSHOP_EVENT_REFRESH, pResult->dwRemainAmount);
}

void PacketHandler_GUCashItemInfoRes(void * pPacket)
{
	sGU_CASHITEM_INFO_RES* pResult = (sGU_CASHITEM_INFO_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_INFO_RES"), TRUE);
		return;
	}

	// Event
	CDboEventGenerator::HLShopEventItemInfoRes();
}

void PacketHandler_GUCashItemInfo(void * pPacket)
{
	sGU_CASHITEM_INFO* pResult = (sGU_CASHITEM_INFO*)pPacket;

	// Event
	CDboEventGenerator::HLShopEventItemInfo(pResult->byCount, pResult->aInfo);
}

void PacketHandler_GUCashItemMoveRes( void* pPacket )
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_MOVE_RES);

	sGU_CASHITEM_MOVE_RES* pResult = (sGU_CASHITEM_MOVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_MOVE_RES"), TRUE );
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_YARDRAT_MOVE_SUCCESS");

	// Event
	CDboEventGenerator::HLShopEventItemMoveRes(pResult->qwProductId);
}

void PacketHandler_GUCashItemUseRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_USE_RES);

	sGU_CASHITEM_USE_RES* pResult = (sGU_CASHITEM_USE_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_USE_RES"), TRUE);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_YARDRAT_USE_SUCCESS");

	// Event
	CDboEventGenerator::HLShopEventItemUseRes(pResult->qwProductId);
}

void PacketHandler_GUCashItemAddNfy(void * pPacket)
{
	sGU_CASHITEM_ADD_NFY* pResult = (sGU_CASHITEM_ADD_NFY*)pPacket;

	// Event
	CDboEventGenerator::HLShopEventItemAddNfy(&pResult->sInfo);
}

void PacketHandler_GUCashItemDelNfy(void * pPacket)
{
	sGU_CASHITEM_DEL_NFY* pResult = (sGU_CASHITEM_DEL_NFY*)pPacket;

	// Event
	CDboEventGenerator::HLShopEventItemDelNfy(pResult->qwProductId);
}

void PacketHandler_GUCashItemBuyRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_BUY_RES);

	sGU_CASHITEM_BUY_RES* pResult = (sGU_CASHITEM_BUY_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_BUY_RES"), TRUE);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_HLS_ITEM_BUY_SUCCESS");

	// Event
	CDboEventGenerator::HLShopEventItemBuyRes(pResult->dwRemainAmount);
}

void PacketHandler_GUCashItemSendGiftRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CASHITEM_SEND_GIFT_RES);

	sGU_CASHITEM_SEND_GIFT_RES* pResult = (sGU_CASHITEM_SEND_GIFT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CASHITEM_SEND_GIFT_RES"), TRUE);
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_HLS_ITEM_GIFT_SUCCESS");

	// Event
	CDboEventGenerator::HLShopEventItemGiftRes(pResult->dwRemainAmount);
}

void PacketHandler_GUCashItemRecvGiftNfy(void * pPacket)
{
	sGU_CASHITEM_RECV_GIFT_NFY* pResult = (sGU_CASHITEM_RECV_GIFT_NFY*)pPacket;

	// Event
	CDboEventGenerator::HLShopEventItemAddNfy(&pResult->sInfo);

	// Event
	CNtlSLEventGenerator::PopoNotify(GetDisplayStringManager()->GetString("DST_SIDEICON_HINT_GIFT_TITLE"),
		GetDisplayStringManager()->GetString("DST_SIDEICON_HINT_GIFT_TEXT"), TRUE, 5.0f); // Shows for 5 seconds
}

void Packethandler_GUQuickSlotDelNfy( void* pPacket )
{
	sGU_QUICK_SLOT_DEL_NFY* pNotify = (sGU_QUICK_SLOT_DEL_NFY*)pPacket;

	// peessi: With regard to the skill deletion in the tutorial, the packet will be flushed before the GUI is loaded, before the avatar is created.
	// Here we add the logic to change the value of AvatarInfo in case.

	if( !GetNtlSLGlobal()->GetSobAvatar() )
	{
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

		if( !pAvatarInfo->bySetQuickSlotInfo )
			return;			

		for( RwInt32 i = 0 ; i < pAvatarInfo->byQuickSlotInfoCount ; ++i )
		{
			if( pAvatarInfo->sQuickSlotInfo[i].bySlot == pNotify->bySlotID )
				pAvatarInfo->sQuickSlotInfo[i].bySlot = INVALID_BYTE;
		}
	}
	else
	{
		CDboEventGenerator::QuickSlotDelNfy(pNotify->bySlotID);
	}
}

void PacketHandler_GUCharUpdateLPStatusNfy( void* pPacket ) 
{
    sGU_UPDATE_CHAR_LP_STATUS_NFY* pNfy = (sGU_UPDATE_CHAR_LP_STATUS_NFY*)pPacket;

    CNtlSLEventGenerator::SobUpdateLPStatusNfy(pNfy->handle, pNfy->bEmergency);
}

void PacketHandler_GUCharUpdateMaxLP( void* pPacket ) 
{/*
	sGU_UPDATE_CHAR_MAX_LP* pNfy = (sGU_UPDATE_CHAR_MAX_LP*)pPacket;

	CNtlParty* pParty = reinterpret_cast<CNtlParty*>(GetNtlSLGlobal()->GetSobAvatar()->GetParty());
	if( pParty->IsMember(pUpdateLp->handle) )
	{
		// 나의 파티원이라면 업데이트
		CNtlSLEventGenerator::PartyUpdate(PMT_LP, pNfy->hSubject, pUpdateLp->wCurLP, pUpdateLp->wMaxLP);
	}

	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pUpdateLp->handle) );
	if(pActor == NULL)
		return;

	// LP Sync
	CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();

	if(pAnimSyncManager->IsExistTarget(pActor->GetSerialID()))
	{
		pAnimSyncManager->SetFinialServerSyncUnit(pActor->GetSerialID(), pUpdateLp->dwLpEpEventId, pUpdateLp->wCurLP, pUpdateLp->wMaxLP);
	}
	else
	{
		Logic_SetLp(pActor, pUpdateLp->wCurLP, pUpdateLp->wMaxLP);
	}*/
}

void PacketHandler_GUCharUpdateMaxEP( void* pPacket ) 
{
	//sGU_UPDATE_CHAR_MAX_EP* pNfy = (sGU_UPDATE_CHAR_MAX_EP*)pPacket;
}

void PacketHandler_GUCharUpdateMaxRP( void* pPacket ) 
{/*
	sGU_UPDATE_CHAR_MAX_RP* pNfy = (sGU_UPDATE_CHAR_MAX_RP*)pPacket;

	CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pNfy->hSubject) );
	if( !pActor )
		return;

	CNtlSobLifeAttr* pSobListAttr = reinterpret_cast<CNtlSobLifeAttr*>( pActor->GetSobAttr() );

	CDboEventGenerator::UpdateRP(pNfy->hSubject, pSobListAttr->GetRp(), pNfy->wMaxRp);
	Logic_SetRp(pActor, pSobListAttr->GetRp(), pNfy->wMaxRp);*/
}

void PacketHandler_GUCharUpdateAP(void * pPacket)
{
	sGU_UPDATE_CHAR_AP *pRes = (sGU_UPDATE_CHAR_AP*)pPacket;

	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	Logic_SetAp(pSobAvatar, pRes->curAP, pRes->maxAP);
}

void PacketHandler_GSAvatarCharTitleInfo(void * pPacket)
{
	// Receive chartitle information when loading avatar
	sGU_CHARTITLE_LIST_INFO* pResult = (sGU_CHARTITLE_LIST_INFO*)pPacket;

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	char* pTitleIndexFlag = pAvatarInfo->abyTitleIndexFlag;

	memcpy(pTitleIndexFlag, pResult->TitleIndexFlag, NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG);
}

void PacketHandler_GSAvatarCharTitleSelectRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_CHARTITLE_SELECT_RES);

	sGU_CHARTITLE_SELECT_RES* pResult = (sGU_CHARTITLE_SELECT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_CHARTITLE_SELECT_RES"));
		return;
	}

	CDboEventGenerator::CharTitleSelectRes(pResult->tblIndex);
}

void PacketHandler_GSAvatarCharTitleAdd(void * pPacket)
{
	sGU_CHARTITLE_ADD* pResult = (sGU_CHARTITLE_ADD*)pPacket;

	CDboEventGenerator::CharTitleUpdate(pResult->tblIndex, false);
}

void PacketHandler_GSAvatarCharTitleDel(void * pPacket)
{
	sGU_CHARTITLE_DELETE* pResult = (sGU_CHARTITLE_DELETE*)pPacket;

	CDboEventGenerator::CharTitleUpdate(pResult->tblIndex, true);
}

void PacketHandler_GSAvatarCharTitleSelectNfy(void * pPacket)
{
	sGU_CHARTITLE_SELECT_NFY* pResult = (sGU_CHARTITLE_SELECT_NFY*)pPacket;

	CNtlSLEventGenerator::EventSobCharTitleSelectNfy(pResult->handle, pResult->charTitle);
}
