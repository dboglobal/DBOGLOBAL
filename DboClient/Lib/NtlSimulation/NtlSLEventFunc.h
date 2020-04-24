/*****************************************************************************
*
* File			: NtlSLEventFunc.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 9. 01	
* Abstract		: Simulation layer event function
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SLEVENT_GENERATOR_H__
#define __NTL_SLEVENT_GENERATOR_H__

#include "NtlPacketGU.h"
#include "NtlSLEvent.h"
#include "NtlSLServerDeclear.h"
#include "NtlBehaviorData.h"
#include "DboTSCore.h"
#include "NtlMath.h"
#include "NtlSharedType.h"

class CNtlSLEventGenerator
{
public:

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* system message
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void SysMsg(SERIAL_HANDLE hSerialId, const char* chMsg, RwBool bServerMsg = TRUE);
	static void SysMsg(const WCHAR* pMsgString, RwUInt32 eType = SNtlEventSysStringMsg::TYPE_CHAT_WARN );
	static void SysMsg(WCHAR* pMsgString, RwUInt32 eType = SNtlEventSysStringMsg::TYPE_CHAT_WARN );
	static void FormatSysMsg(SERIAL_HANDLE hSerialId, const char* csMsg, RwBool bServerMsg, ... );
	static void FormatSysMsg(WCHAR* pMsgString, ...);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* network
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void NetSendError(void *pNetSender);
	static void NotifySendPacket(void *pPacket);
	static void NotifyReceivePacket(void *pPacket);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* world change
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void WorldChange(RwBool bWorldChange);
	static void CreateWorld(RwBool bCreate);            ///< 월드가 생성되거나 소멸될때 이벤트를 발생

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* avatar/camera 이동 및 회전
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void CameraSmoothingMove(RwBool bEnable);
	static void CameraShake(SERIAL_HANDLE hSerialId, RwBool bLowShakeFlag = FALSE);
	static void CameraDash(SERIAL_HANDLE hSerialId, RwReal fDashSpeed, RwV3d vDest);
	static void CameraNpc(SERIAL_HANDLE hSerialId);
	static void CameraDB(SERIAL_HANDLE hSerialId);
	static void CameraDirectNodeClear(void);
	static void CameraDirectSplineNode(RwReal fDelayTime, RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos, RwUInt8 byDirectLookAtType, const RwChar *pKey);    
	static void CameraFree();
	static void CameraTimeMachine(SERIAL_HANDLE hSerialID);
	static void CameraExplosion(SERIAL_HANDLE hSerialID, unsigned int uiExplosionId);
	static void CameraTimeMachineArrive(RwBool bStart, SERIAL_HANDLE hSerialID);
	static void CameraKnockDownMatrix(SERIAL_HANDLE hSerialID, RwReal fLifeTime);
	static void CameraFPS(RwV3d& vCurrLookAt, RwV3d& vTargetLookAt);
	static void CameraBus(CNtlSobActor* pActor);
    static void CameraObserver(EObserverCameraType eType, RwInt32 nIndex);
	static void CameraControlDelete(RwUInt8 byControllerType);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* action mapping
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static void ActionMapHalt(void);
	static void ActionMapJump(RwUInt32 uiMoveFlags);
	static void ActionMapAirJump(RwUInt32 uiMoveFlags);
	static void ActionMapCharging(RwBool bStartOrEnd);
	static void ActionMapBlocking(RwBool bStartOrEnd);
	static void ActionMapSitAndStand(void);
	static void ActionMapSkillUse(SERIAL_HANDLE hSerialId, RwBool bExistTarget, SERIAL_HANDLE hTargetSerialId, 
		SERIAL_HANDLE hSkillSerialId, RwReal fFollowRange, RwUInt8 bySkillSlotIdx, RwUInt8 byRpBonusType, TBLIDX skillTblidx);
	static void ActionMapSocialUse(RwUInt32 uiTblId);
	static void	ActionMapPrivateShopOpen(void);
	static void ActionMapHTBUse(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, SERIAL_HANDLE hHTBSerialId, 
		RwReal fFollowRange, RwUInt8 byHTBSlotIdx);
	static void ActionMapItemUse(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt8 byPlace, RwUInt8 byPos, RwUInt8 byCastingTime, 
		SERIAL_HANDLE hRefObject, RwUInt8 byKeyPlace, RwUInt8 byKeyPos);

	static void	ActionMapAutoFollow(SERIAL_HANDLE hTargetSerialId);

	static void ActionMapDice();
	static void ActionScreenShot(const std::string& strFileName);
	static void ActionSkillSelectMode(bool bSelect, SERIAL_HANDLE hSerialId);

    // 액션 아이콘을 이용한 액션 (AvatarController에서 한번 걸러져서 이벤트를 보낸다)
    static void ActionFuncSitAndStand();
    static void ActionFuncBasicAttack();
    static void ActionFuncAssist();
    static void ActionFuncGetItem();


	static void ESCinNarrationMode();


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/** 
	*simulation object event
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//---------------------------------------------------------
	// input에 관한 event

	static void SobShowNfy(SERIAL_HANDLE hSerialId, RwBool bVisible);

	static void TerrainClick(RwV3d vPickPos, RwBool bServerSend = TRUE);
	static void KeyboardMove(RwUInt8 byMoveFlags, RwBool bAvatarAngleChange, RwReal fAvatarAngleY); 
	static void KeyboardMoveStop(void); 
	static void MouseDashMove(RwV3d vPickPos, RwReal fLimitDist);
	static void KeyboardDashMove(RwUInt32 uiMoveFlags);
	static void AirDashMove(RwBool bMouse, RwV3d* pvPickPos, RwUInt32 uiMoveFlags, RwBool bUpdateMove = TRUE);
	static void AirAccelMove(BYTE byMoveDirection, RwBool bUpdateMove = TRUE);
	static void InputChagneHeading(RwBool bSmoothing, RwReal fDeltaAngleY, bool bSetAngleX, RwReal fDeltaAngleX, RwBool& bSuccess);
	static RwBool SobTargetSelect(SERIAL_HANDLE hSerialId, RwBool bFollow = FALSE);
	static void SobTargetSelectRelease(SERIAL_HANDLE hSerialId);
	static void SobAttackMarkRelease(SERIAL_HANDLE hSerialId);
	static void SobAttackSelect(SERIAL_HANDLE hSerialId);
	static void SobGotFocus(SERIAL_HANDLE hSerialId);
	static void SobLostFocus(SERIAL_HANDLE hSerialId);

	// 커뮤니티 타겟 선택/해제 이벤트
	static void CommuTargetSelect(CHARACTERID targetID, SERIAL_HANDLE hSerialId, WCHAR* wchName, ECommuTargetType type);
	static void CommuTargetRelease(SERIAL_HANDLE hSerialId);

	static void BotCaution_Nfy(SERIAL_HANDLE hSerialId);            ///< 몹이 날 발각했을때의 이벤트 발생    
	static void BotHelpMe_Nfy(SERIAL_HANDLE hSerialId, RwBool bisRequester);    ///< 몹이 도움을 요청할때 발생하는 이벤트


	/// 공유 타겟 선택/해제 이벤트
	static void ShareTargetMarking(SERIAL_HANDLE hSerialId, RwUInt8 bySlot);
	static void ShareTargetRelease(SERIAL_HANDLE hSerialId);

    /// 타겟 마크 선택/해제 이벤트
    static void SobTargetMarkSelect(SERIAL_HANDLE hSerialId);
    static void SobTargetMarkRelease(SERIAL_HANDLE hSerialId);

    static void AvatarTargetStateChange(void);

    // 천하제일 무도회 칭호 
	static void TenkaichiMarking(SERIAL_HANDLE hSerialID, BYTE byType);

	//
	static void SobEventTitleEffect(SERIAL_HANDLE hSerialID, bool bCreate, const char* pchBone, const char* pchEffect);

	//---------------------------------------------------------
	// create/delete에 관한 함수.

	static void* SobAvatarCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, sCHARSTATE *pCharState, RwBool bUIAvatarCreate = FALSE);

	static void* SobPlayerCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sPC_BRIEF *pPcBrief, sCHARSTATE *pCharState, RwBool bIsClientCreate = FALSE);

	static void* SobMonsterCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sMOB_BRIEF *pMobBrief, sCHARSTATE *pCharState, RwBool bIsClientCreate = FALSE); 

	static void* SobNpcCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sNPC_BRIEF *pNpcBrief, sCHARSTATE *pCharState, RwBool bNameCreate, RwBool bIsClientCreate = FALSE);

	static void* SobPetCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, RwBool bOwnerAvatar, UPetData uPetData, sCHARSTATE *pCharState);

	static void* SobSlotItemCreate(void *pParentSobItem, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, 
		RwUInt32 uiItemTblId, sITEM_OPTION_SET&	sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
		RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwBool bNeedToIdentify, RwBool bCreateItemModel, RwUInt8 byBattleAttribute,
		RwUInt8	byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime);

	static void* SobQuestItemCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiQuestItemTblId, RwUInt8 bySlotIdx, RwUInt8 byStackCount);

	static void* SobWorldItemCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwInt32 eObjType, void* pBrief, void* pState, void* pItemOptionSet);

	static void* SobStatusCreate(RwUInt32 uiClassId, RwUInt32 uiCurrHp, RwUInt32 uiDamage, RwUInt32 uiFillHp, RwUInt32 uiFillMp, RwUInt32 uiFillCp, RwBool bDamage);

	static void* SobTriggerObject(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, void *pPLEntity);

	static void* SobDynamicObject(SERIAL_HANDLE hOwner, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt8 byType, RwUInt32 uiTblId, RwV3d vLoc, RwUInt8 byState);

	static void* SobEventObjectCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, void* pPLEntity);

	static void* SobProjectileCreate(RwUInt32 uiClassId, SERIAL_HANDLE hOwnerSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt32 uiBehavior, RwUInt8 byShotType,
		RwReal fSpeed, RwBool bLeftHand, RwBool bCreateHitStuffEffect, const RwChar *pBoneName, const RwChar *pEffKey, RwReal fTargetHeight, void *pProjExtData, RwReal fWeightElapsedValue = 1.0f);

	static void* SobSkillCreate(RwUInt32 uiOwnerID, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiSkillTblId, RwUInt8 bySlodIdx, RwUInt8 byRpBonusType, RwBool bRPBonusAutoMode, RwInt32 iExp, RwUInt32 uiRemainTime = 0);

	static void* SobHTBSkillCreate(RwUInt32 uiOwnerID, RwUInt8 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiHTBTblId, RwUInt8 bySlotIdx, RwUInt32 uiRemainTime = 0);

	static void* SobActionSkillCreate(RwUInt8 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiActionTblId, RwUInt8 bySlotIdx );

	static void* SobBuffCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, BYTE byBuffIndex, RwUInt8 byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter, bool bActive);

	static void* SobVehicleCreate(RwUInt32 uiClassId, sCHARSTATE* pCharState, const RwV3d& vLoc, const RwV3d& vDir, RwUInt32 uiVehicleTblIdx, RwBool bForChild, RwBool bOwnerAvatar );

	static void SobDelete(SERIAL_HANDLE hSerialId);

	static void SobDeletePostMsg(SERIAL_HANDLE hSerialId);

	static void SobSummonPetSpawnSync(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hPetSerialId);

	static void SobPetSkillInfo(RwUInt8 bySkillCount, const sSKILL_INFO_PET* pSkillInfoPet);

	//---------------------------------------------------------
	// move에 관한 함수.
	static void SobMove(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlags, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDir, RwUInt32 uiTimeStamp);
    static void SobMoveSync(SERIAL_HANDLE hSerialId, RwV3d& vCurLoc, RwV3d& vCurDir, DWORD dwTimeStamp);
	static void SobSplineMove(SERIAL_HANDLE hSerialId, RwBool bGroundCheck, RwBool bRunCheck, RwChar *pSplineName, RwV3d vStartPos);
	static void SobMoveStop(SERIAL_HANDLE hSerialId, RwV3d vCurrLoc, RwV3d vDir, RwUInt32 uiTimeStamp);
	static void SobDestMove(SERIAL_HANDLE hSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDest, RwUInt32 uiTimeStamp, RwUInt8 byPathNodeNum = 0, RwV3d *pPathNode = NULL);
	static void SobSecondDestMove(SERIAL_HANDLE hSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDest, RwV3d vSecondDest, RwUInt32 uiTimeStamp);
	static void SobDirectionFloat(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlags);
	static void SobAdjustMovePosition(SERIAL_HANDLE hSerialId, RwV3d vAdjustPos);
	static void SobDash(SERIAL_HANDLE hSerialId, RwV3d vDest, RwUInt8 byMoveFlags, RwUInt32 uiTimeStamp);
	static void SobFollowMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwReal fFollowRange, RwUInt8 byReason, RwUInt32 uiTimeStamp);
	static void SobAdjustMovement(SERIAL_HANDLE hSerialId, RwV3d vDest, RwV3d vHeading);
	static void SobAttackFollow(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hDefenderSerialId);
    static void SobPushingNfy(SERIAL_HANDLE hSerialId, BYTE byAttackResult, WORD wAttackResultValue, const RwV3d& vDestPos);
	static void SobGuardCrush(SERIAL_HANDLE hSerialId);
	static void SobMovePatternSync(SERIAL_HANDLE hSerialId, RwUInt8 bySyncUnitId);
	static void SobAirJump(SERIAL_HANDLE hSerialId, RwUInt8 byMoveDirection, RwV3d& vDir);
	static void SobAirAccel(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlag, RwUInt8 byMoveDirection, RwV3d& vLoc, RwV3d& vDir);
	
	//---------------------------------------------------------
	// object item에 관한 함수.
	static void SobItemAdd(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
		sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
		RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
		RwUInt8	byRestrictType, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime);

	static void SobItemUpdate(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
		sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
		RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
		RwUInt8	byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime);

	static void SobItemDelete(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt8 byPlace, RwUInt8 bySlotIdx );

	static void ItemSocketInsertBead(SERIAL_HANDLE hSerialId, RwBool bSuccess, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byRestrictState, RwUInt8 byDurationType,
		DBOTIME nUseStartTime, DBOTIME nUseEndTime, sITEM_RANDOM_OPTION* pRandomOption, RwUInt8 byBeadPlace, RwUInt8 byBeadPos, RwUInt8 byBeadRemainStack);

	static void ItemSocketDestroyBead(SERIAL_HANDLE hSerialId, RwUInt8 byPlace, RwUInt8 byPos, RwUInt8 byRestrictState, RwUInt8 byDurationType);

	static void SobAvatarItemMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, 
		SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx);

	static void SobAvatarItemStackMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, RwUInt8 bySrcStackCount,
		SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx, RwUInt8 byDestStackCount);

	static void SobEquipModelChange(SERIAL_HANDLE hSerialId, RwUInt8 bySlotIdx, RwUInt32 uiTblId, RwUInt8 byGrade, RwUInt8 byBattleAttribute);

	static void SobEquipItemDurDown(RwUInt8* pArrDurablity);

	static void SobEquipItemDurUpdate(SERIAL_HANDLE hSerialId, RwUInt8 byArrDurablity);

	static void SobItemUseAction(SERIAL_HANDLE hSerialId, RwUInt32 uiTableIdx, RwUInt8 ucSkillResultCount, sSKILL_RESULT* pSkillResult );

	static void SobItemUpgrade(BYTE byItemGrade, BYTE byItemPlace, BYTE byItemPos, BYTE byStonePlace, BYTE byStonePos, BYTE byStoneStack, BYTE byCorePlace, BYTE byCorePos, BYTE byCoreStack, bool bItemDestroy);

	static void SobItemChangeOptionSet(void* pOptionSet, BYTE byItemPlace, BYTE byItemPos, BYTE byKitPlace, BYTE byKitPos, BYTE byKitRemainStack);

	static void SobItemChangeBattleAttribute(RwUInt8 byBattleAttribute, RwUInt8 byItemPlace, RwUInt8 byItemPos);

	static void SobEquipItemUpgradeAll(RwUInt8 byCount, RwUInt8* abyUpgradeValue);

	static void SobItemCancel(SERIAL_HANDLE hSerialId);

	static void SobItemUseResFailed(SERIAL_HANDLE hSerialId);

	static void SobItemCasting(SERIAL_HANDLE hSerialId, RwUInt32 uiTargetSerialI, RwUInt32 uiItemTblId, RwReal fCurrCastingTime, RwReal fMaxCastingTime);

	static void SobCheckItemInOut(void);

	static void SobItemRestrictUpdate(BYTE byPlace, BYTE byPos, BYTE byRestrictState);


	//---------------------------------------------------------
	// Party Select
	static void EventPartySelectStateInit(BYTE bySelectState, DWORD dwParam, bool bLastStage);
	static void EventPartySelectState(HOBJECT hTarget, BYTE bySelectState);

	//---------------------------------------------------------
	// CC Battle Dungeon
	static void EventBattleDungeonStateUpdate(BYTE byStage, TBLIDX titleTblidx, TBLIDX subTitleTblidx, DWORD dwLimitTime);
	static void EventBattleDungeonStageClear();
	static void EventBattleDungeonLimitTime(DWORD dwLimitTime, bool bStart);
	static void EventBattleDungeonFail();
	static void EventBattleDungeonRewardFinish();

	//---------------------------------------------------------
	// object item에 관한 함수.
	static void SobDeleteQuickSlotIcon(SERIAL_HANDLE hSerialId);

	//---------------------------------------------------------
	// object Warehouse item에 관한 함수.
	static void SobWarehouseUpdate(RwUInt32 uiMessageType, RwUInt32 uiSerial, RwUInt32 uiValue = 0 , RwInt32 iValue2 = 0);

	static void SobWarehouseItemCreate(RwUInt8 byItemCount, void* pData);

	static void SobWarehouseItemAdd(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
		sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
		RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
		RwUInt8	byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime);

	static void SobWarehouseItemDelete(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt8 byPlace, RwUInt8 bySlotIdx);

	static void SobWarehouseItemMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, 
		SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx);

	static void SobWarehouseItemMoveStack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, RwUInt8 bySrcStackCount,
		SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx, RwUInt8 byDestStackCount);

	//---------------------------------------------------------
	// object quest item에 관한 함수.
	static void SobQuestItemAdd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestItemTblId, RwUInt8 bySlotIdx, RwUInt8 byStackCount);

	static void SobAvatarQuestItemMove(SERIAL_HANDLE hSerialId, RwUInt8 ucSrcSlotIdx, RwUInt32 uiSrcTblIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiDestTblIdx);

	static void SobQuestItemDelete(SERIAL_HANDLE hSerialId, RwUInt8 ucDeleteSlotIdx);

	static void SobQuestItemUpdate(SERIAL_HANDLE hSerialId, RwUInt8 ucSlotIdx, RwUInt8 ucCount);
	//---------------------------------------------------------
	// 공격에 관한 함수.
	// attack animation이 발동되는 시점.
	static void SobStanding(SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir);

	static void SobFightingMode(SERIAL_HANDLE hSerialId, RwBool bFightingMode);

	static void SobAttack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSpawnId, 
							RwBool bChainAtt, RwUInt8 byAttRes, RwUInt8 byAttSeq, RwUInt8 byBlocking,
							RwUInt32 uiDamage, RwUInt32 uiAbsorbedDamage, RwUInt32 uiReflectedDamage,
		sDBO_LP_EP_RECOVERED& lpEpRecovered, RwUInt32 uiSyncEventId);

	static void SobSpecialAttack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSpawnId,
		BYTE byAttackType, BYTE bySourceType, TBLIDX sourceTblidx, BYTE byAttackResult, RwUInt32 uiDamage);

	static void SobBattleNotification(SERIAL_HANDLE hSerialId);

	static void SobAttacked(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId);

	static void SobPetBeginAttack(SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSpawnId);

	// attack animation이 진행되면서, 타겟에 damage를 보낼 때 발동되는 시점.

	static void SobHit(SERIAL_HANDLE hSerialId, const SHitStuff *pHitStuff);
	static void SobAutoAttackMode(SERIAL_HANDLE hSerialId, RwBool bAttackMode);                                             ///< 자동 공격 모드 On/Off를 알려준다.    
    static void SobSendAttackEnd(SERIAL_HANDLE hSerialId);                                                                  ///< 서버에 어택상태 종료를 알려준다.
	static void SobJump(SERIAL_HANDLE hSerialId, RwUInt8 byMoveDirection, RwV3d vDir, RwV3d vJumpDir);
	static void SobJumpDirection(SERIAL_HANDLE hSerialId, RwV3d vJumpDir, RwUInt32 uiTimeStamp);
	static void SobJumpEnd(SERIAL_HANDLE hSerialId);
	static void SobSit(SERIAL_HANDLE hSerialId, RwBool bSitDown);
	static void SobPrivateShop(SERIAL_HANDLE hSerialId, WCHAR *pName, RwBool bOwnerEmpty, RwUInt8 byShopState);
	static void SobConvertClass(SERIAL_HANDLE hSerialId, RwUInt8 byClass);
    static void SobChangeAdult(SERIAL_HANDLE hSerialId, RwBool bAdult);

	static void EventSobCharTitleSelectNfy(HOBJECT hHandle, TBLIDX uiTitle);

	static void EnableAir(bool bFlag);
	static void SobSetAp(int nAp, int nMaxAp);

	static void SobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt8 byDirectType, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime);
	static void SobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime);
	static void SobProxyGUIUpdate(SERIAL_HANDLE hSerialId, RwUInt32 uiUpdateFlags, RwBool bShow);

	//---------------------------------------------------------
	// cooling에 관한 함수.

	static void SobCooling(SERIAL_HANDLE hSerialId, RwBool bStart);

	//---------------------------------------------------------
	// skill에 관한 함수.

	static void SobSkillCancel(SERIAL_HANDLE hSerialId, RwUInt16 wReason);

	static void SobSkillAffectingCancel(SERIAL_HANDLE hSerialId);

	static void SobSkillCasting(SERIAL_HANDLE hSerialId, RwUInt32 uiTargetSerialI, RwUInt32 uiSkillTblId, RwReal fCurrCastingTime, RwReal fMaxCastingTime);

	static void SobSkillAction(SERIAL_HANDLE hSerialId, RwUInt16 wResultCode, RwUInt32 uiTargetSerialI, RwUInt32 uiSkillTblId,
		RwUInt8 byRpBonusType, RwUInt8 byResultCount, sSKILL_RESULT *pSkillAct, RwUInt32 uiSyncEventId, RwV3d& vFinalLoc);

	static void SobSkillActioned(SERIAL_HANDLE hSerialId, RwUInt32 uiAttackerSerialI);

	static void SobSkillAdd(SERIAL_HANDLE hSerialId, RwUInt8 bySkillSlotIdx, RwUInt32 uiSkillTblId);

	static void SobSkillUpgrade(SERIAL_HANDLE hSerialId, RwUInt8 bySkillSlotIdx, RwUInt32 uiUpgradeSkillTblId);

	static void SobHTBSkillAdd(SERIAL_HANDLE hSerialId, RwUInt8 byHTBSkillSlotIdx, RwUInt32 uiHTBSkillTblId);

	static void SobHTBSkillAction(SERIAL_HANDLE hSerialId, sCHARSTATE *pCharState);

	static void SobSocialAction(SERIAL_HANDLE hSerialId, RwUInt32 tblIdx);

	static void SobSocialActionCooling(RwUInt32 tblIdx);

	static void SobSkillInit(void);

	static void RpBonusSetupRes(RwUInt32 skillid, RwUInt8 bySlotIndex, RwUInt8 byRpbonusType, RwBool bRpBonusAutoMode);

	static void SobSkillSetPassiveEffect( RwUInt32 uiSkillTblidx, RwUInt8 bySkillSlotIdx, sDBO_UPDATED_SKILL_DATA* pPassiveEffectData );

	static void SobSkillInfoText(WCHAR* wchMsg, unsigned int uiColor, BYTE byMsgType);

	//---------------------------------------------------------
	// buff에 관한 함수.

	static void SobBuffAdd(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter);
	static void SobBuffDrop(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType);
	static void SobBuffRefreshAll(SERIAL_HANDLE hSerial, RwUInt8 byCount, sBUFF_INFO* pBuffInfo);
	static void SobBuffActivate(SERIAL_HANDLE hSerial, sBUFF_INFO* pBuffInfo);
	static void SobBuffAutoRecover(SERIAL_HANDLE hSerial, BYTE byBuffIndex, DWORD dwRemainTime, DWORD dwRemainValue);

	static void SobFakeBuffAdd(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter);
	static void SobFakeBuffDrop(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType);
	static void SobFakeBuffRefreshAll(SERIAL_HANDLE hSerialId, RwUInt8 byCount, sBUFF_INFO* pBuffInfo);
	static void SobFakeBuffActivate(SERIAL_HANDLE hSerial, sBUFF_INFO* pBuffInfo);

	//---------------------------------------------------------
	// simulation object state transition에 관한 함수.
	static void SobStateTransition(SERIAL_HANDLE hSerialId, RwUInt32 uiTransState);
	static void SobServerUpdateState(SERIAL_HANDLE hSerialId, sCHARSTATE *pState);
	static void SobServerUpdateStatePostMsg(SERIAL_HANDLE hSerialId, sCHARSTATE *pState);
	static void SobServerUpdateCondition(SERIAL_HANDLE hSerialId, QWORD qwCondition);
	static void SobServerUpdateAspect(SERIAL_HANDLE hSerialId, sASPECTSTATE& sAspectState);                  ///< Aspect가 서버로부터 업데이트될때 호출된다.
	static void SobFainting(SERIAL_HANDLE hSerialId);
    static void SobUpdateLPStatusNfy(SERIAL_HANDLE hSerialId, RwBool bEmergency);                            ///< 빈사 상태/ 일반 상태

	//---------------------------------------------------------
	// simulation object information update에 관한 함수.
	static void SobUpdate(SERIAL_HANDLE hSerialId, RwUInt32 uiUpdateType, RwUInt32 uiParam1);
	static void SobNpcCommunity(SERIAL_HANDLE hSerialId, RwBool bShow, RwUInt32 uiTriggerId, RwUInt8 byInputType);
	static void SobTriggerObjCommunity(SERIAL_HANDLE hSerialId, RwBool bShow, RwUInt32 uiTriggerId, RwUInt8 byInputType);
	static void SobDynamicObjCommunity(SERIAL_HANDLE hSerialId, RwBool bShow);
	static void SobPrivateShopCommunity(SERIAL_HANDLE hSerialId, RwBool bOwnerEmpty, RwUInt8 byShopState);
	static void SobTargetUpdate(SERIAL_HANDLE hSerialId, RwBool bShow);
	static void SobTargetChanged(HOBJECT hObject, HOBJECT hTarget);
	static void SobPickWorldItem(SERIAL_HANDLE hSerialId);
	static void SobCastingDirect(CNtlSobActor* pActor, RwBool bSkillOrItemCasting, RwBool bStartOrEnd);
	static void SobRevivalNotify(void);
    static void NPCCommuExit();

	//---------------------------------------------------------
	// party에 관한 함수
	static void PartyUpdate(RwInt32 iMessage, SERIAL_HANDLE hSerial = INVALID_SERIAL_ID, RwUInt32 uiValue = 0, RwInt32 iValue2 = 0);

	static void PartyCreate(WCHAR* pcName);

	static void PartyDisband();

	static void PartyMemberAdd(SERIAL_HANDLE hSerial, WCHAR* pwcName, RwUInt8 byRace = 0, RwUInt8 byClass = 0, RwUInt8 byLevel = 0,
		RwInt32 curLP = 0, RwInt32 maxLP = 0, RwUInt32 wCurEP = 0, RwUInt32 wMaxEP = 0, RwUInt32 uiWorldTableIndex = 0xffffffff, RwUInt32 uiWorldID = 0xffffffff,
		RwReal fX = 0.f, RwReal fY = 0.f, RwReal fZ = 0.f);	

	static void PartyMemberDel(SERIAL_HANDLE hSerialId);

	static void PartyLeaderChange(SERIAL_HANDLE hSerialId);

	static void PartyUpdateValue(RwInt32 iValueType, SERIAL_HANDLE hSerial = INVALID_SERIAL_ID, RwUInt32 uiValue = 0, RwInt32 iValue2 = 0);

	static void PartyMemberLocation(RwUInt32 uiSerial, RwUInt32 uiWorldID, RwUInt32 uiWorldTbl, RwV3d& vCurLoc);

	static void PartyShareTargetRes(WORD wResultCode);                      ///< 공유 타겟 요청에 대한 결과 패킷 이벤트
	static void PartyShareTargetNfy(sSHARETARGET_INFO* aShareTargetInfos);  ///< 공유 타겟에 대한 알림 패킷 이벤트

	//---------------------------------------------------------
	// guild에 관한 함수
	static void SLGuildEvent(RwInt32 iMessage, RwUInt32 uiParam = 0xffffffff, void* pData = NULL, void* pExData = NULL);
	static void NotifyGuildEvent(RwInt32 iMessage, RwUInt32 uiParam = 0xffffffff);
	static void ChangeGuildName(SERIAL_HANDLE hHandle, WCHAR* pwcGuildName);
	static void ChangeGuildEmblem(SERIAL_HANDLE hHandle, RwUInt8 byTypeA, RwUInt8 byTypeAColor,
		RwUInt8 byTypeB, RwUInt8 byTypeBColor,
		RwUInt8 byTypeC, RwUInt8 byTypeCColor);

	static void GuildWarehouseEvent(RwUInt8 byMessage, RwUInt32 uiParam = 0xffffffff, RwUInt32 uiParam2 = 0xffffffff);
	static void GuildWarehousePostEvent(RwUInt8 byMessage, RwUInt32 uiParam = 0xffffffff, RwUInt32 uiParam2 = 0xffffffff);
	static void GuildWarehouseNotify(RwUInt8 byMessage, RwUInt32 uiParam = 0xffffffff, RwUInt32 uiParam2 = 0xffffffff);

	static void GuildWarehouseItemInfo(RwUInt8 byPlace, RwUInt8 byItemCount, VOID* pData);
	static void GuildWarehouseItemMove(SERIAL_HANDLE hSrcItem, SERIAL_HANDLE hDestItem, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos);
	static void GuildWarehouseItemStackMove(SERIAL_HANDLE hSrcItem, SERIAL_HANDLE hDestItem, RwUInt8 bySrcPlace, RwUInt8 bySrcPos, RwUInt8 byDestPlace, RwUInt8 byDestPos, RwUInt8 byStackCount, RwUInt8 byStackCount2);

	//---------------------------------------------------------
	// 도장
	static void DojoEvent(RwUInt8 byDojoEvent, RwUInt32 uiParam = 0xffffffff, VOID* pExData = NULL, VOID* pExData2 = NULL, VOID* pExData3 = NULL);
	static void DojoEventNotify(RwUInt8 byDojoEvent, RwUInt32 uiParam = 0xffffffff, VOID* pExData = NULL, VOID* pExData2 = NULL, VOID* pExData3 = NULL);
	static void DojoState(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64time);
	static void DojoStateNotify(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64time);

	static void ScrambleDefaultInfo(RwUInt32 uiDojoTableIndex, RwUInt32 uiDefenceGuildID, RwUInt32 uiOffenceGuildID, WCHAR* pwcDefenceGuildName, WCHAR* pwcOffenceGuildName, VOID* pDefenceGuildEmblem, VOID* pOffenceGuildEmblem);
	static void ScrambleDefaultInfoNotify(RwUInt32 uiDojoTableIndex, RwUInt32 uiDefenceGuildID, RwUInt32 uiOffenceGuildID, WCHAR* pwcDefenceGuildName, WCHAR* pwcOffenceGuildName, VOID* pDefenceGuildEmblem, VOID* pOffenceGuildEmblem);

	//---------------------------------------------------------
	// HTB RP Result

	static void HTBRPStock(void);
	static void HTBRPResult(RwInt32 iAttackPoint, RwInt32 iDefenderPoint, RwBool bWin, RwBool bAttacker);

	//---------------------------------------------------------
	// otherparam에 관한 함수
	static void HelpHintEvent(RwUInt32 uiHelpHint);
	static void BindEvent(RwUInt8 byBindType, RwUInt32 uiWorldID, RwUInt32 uiBindObjectTblIdx);
	static void BindNotify();

	//---------------------------------------------------------
	// SideHintIcon
	static void RegSideHintIcon(RwUInt32 uiEventType, RwUInt32 uiTableIndex);

	// PrivateShop
	static void PrivateShopState(RwUInt32 uiEventType, RwUInt32 uiPrivateShopState, sPRIVATESHOP_SHOP_DATA* pPrivateShopData, RwBool IsAvatarPrivateShop);
	static void PrivateShopStateVisitor(RwUInt32 uiEventType, sPRIVATESHOP_SHOP_DATA* pPrivateShopData);
	static void PrivateShopItem(RwUInt32 uiEventType, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData);
	static void PrivateShopItemSelect(RwUInt8 uiPrivateShopPos, RwUInt8 uiItemState);	
	static void PrivateShopItemState(RwUInt8 auiStateData[NTL_MAX_PRIVATESHOP_INVENTORY], RwInt32 IStateCount);	
	static void PrivateShopItemDataInfo(SERIAL_HANDLE hOwnerSerial, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData);
	static void PrivateShopItemBuying(SERIAL_HANDLE hOwnerSerial, RwBool bIsBuyAvatar, sPRIVATESHOP_ITEM_DATA asPrivateShopItemData[NTL_MAX_BUY_SHOPPING_CART], RwUInt8 uiBuyCount);
	static void PrivateShopNameTag(SERIAL_HANDLE hOwnerSerial, RwBool bIsOwnerEmpty, RwUInt32 uiPrivateShopState, WCHAR* pwcPrivateShopName = NULL);

	//---------------------------------------------------------
	// TMQ
	static void SLTMQEnterRes();
	static void SLTMQEvent(RwUInt32 uiEventType, RwUInt32 uiTargetHandle = 0xffffffff, RwUInt32 uiValue = 0xffffffff, void* pExData = NULL);
	static void SLTMQNotify(RwUInt32 uiEventType, RwUInt32 uiValue = 0xffffffff);

	// UD
	static void SLUDEnterRes();

	static void SLDirection_Nfy(RwBool bCreate, RwV3d vLoc);                ///< Creates a direction indication system.

	//---------------------------------------------------------
	// Broad message
	static void BroadMsgNfy_TSBeg( eBROAD_MSG_UI_DIR_TYPE eUIDirType,
								   eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE eUIShowHideType,
								   eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	eUIBalloonShapeType,
								   eBROAD_MSG_UI_SPEECH_DIR_TYPE eUISpeechDirType,
								   eBROAD_MSG_OWNER_CONDITION eOwnerCondition,
								   eBROAD_MSG_OWNER_TYPE eOwnerType,
								   unsigned int uiOwnerTblIdx,
								   unsigned int uiQTblIdx,
								   float fDisplayTime );
	static void BroadMsgNfy_TSEnd( void );
	static void BroadMsgNfy_Emergency( std::wstring& wstrSpeech );
	static void BroadMsgNfy_MiniNarration( RwUInt32 uiMiniNarrationTblIdx );

	//--- DragonBall Collection 관련
	static void NightEffect(RwBool bOn);
	static void DBCNarration(const char* pszTextureName, const WCHAR* pwszText, RwInt32 eNarrationState);
	static void DBCNarrationEnd(void);	
	static void DBCCheck_Res(WORD wResultCode, RwUInt32 uiAltarID);
	static void DBCReward_Res(WORD wResultCode, RwUInt32 uiAltarID);
	static void DBCCollect_Nfy(WORD wResultCode, RwUInt32 uiAltarID);	
	static void DBCSchedule_Info(bool bIsAlive, BYTE byEventType, RwInt64 nStartTime, RwInt64 nEndTime, DWORD dwMainTerm, DWORD dwSubTerm, BYTE byTermType);	
	static void DBCScatter();

	//---------------------------------------------------------
	// Rank Battle
	//static void RBBoardUpdate(RwUInt32 tblidx, RwUInt16 wTotalPage, RwUInt16 wCurPage, RwUInt8 byRoomCount, RwUInt8 byRoomType, void* pRoomList);
	//static void RBRoomInfo(RwUInt8 byRoomMode, void* pRoomInfo);
	//static void	RBRoomEvent(RwInt32 type, RwUInt32 tblidx = 0xFFFFFFFF, RwUInt32 roomidx = 0xFFFFFFFF);
	//static void	RBMatchEvent(RwInt32 type, void* pReadyData = NULL, RwUInt8 byDataCount = 0);	
	static void RBBoardInfo( RwUInt8 byBattleMode, RwUInt32 uiRemainTime, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo );
	static void	RBJoin( RwBool bRes, RwUInt32 hBoardObject, RwUInt32 tblidx, RwUInt32 uiRemainTime, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo );
	static void RBJoinFail(void);
	static void RBLeave(void);
	static void RBCancled(void);
	static void	RBStart( RwInt16 wResultCode );

	static void	RBBattleStateUpdate(RwUInt8 byBattleState, RwUInt8 byStage);
	static void RBBattleTeamInfo(RwUInt16 wStraightWinCount, RwUInt16 wPerfectWinCount, RwUInt8 byCount, void* pMemberList, WCHAR* wcOwnerPartyName, WCHAR* wcChallengerPartyName, RwUInt32 uiOwnerPartyNameSize, RwUInt32 uiChallengerPartyNameSize );
	static void RBBattlePlayerState(RwUInt32 hSerial, RwUInt8 byState);
	static void RBBattleKillScoreUpdate(RwUInt8 byOwnerKillScore, RwUInt8 byChallengerKillScore);
	static void	RBBattleStageFinish(RwUInt8 byBattleResult, void* pStageScoreData);
	static void	RBBattleMatchFinish(void* pMatchResult);
	static void RBBattleEnd(RwUInt8 byWinner);
	static void RBTotalScoreUpdate(RwUInt8 byBattleMode, void* pTotalScoreData);
	static void RBForcedEnd(void);

	//---------------------------------------------------------
	// MailSystem
	static void	MailStart(SERIAL_HANDLE hSerialID, RwBool bIsAway, RwBool bSucceeded);
	static void MailLoadInfo(sMAIL_PROFILE* pMailProfile);
	static void MailLoadData(sMAIL_PROFILE* pMailProfile);
	static void MailSend(SERIAL_HANDLE hSerialID, WCHAR* pwcTargetName);
	static void MailRead(SERIAL_HANDLE hSerialID, MAILID mailID, DBOTIME endTime);
	static void MailDel(SERIAL_HANDLE hSerialID, MAILID mailID);
	static void MailReturn(SERIAL_HANDLE hSerialID, MAILID mailID);
	static void MailReload(SERIAL_HANDLE hSerialID, MAILID* pmailID, RwUInt8 uiMailCount, RwUInt8 uiUnreadMailCountNormal, RwUInt8 uiUnreadMailCountManager);
	static void MailReloadNfy(MAILID* pmailID, RwUInt8 uiMailCount, RwUInt8 uiUnreadMailCountNormal, RwUInt8 uiUnreadMailCountManager);
	static void MailLoad(SERIAL_HANDLE hSerialID);
	static void MailItemReceive(SERIAL_HANDLE hSerialID, MAILID mailID);
	static void MailLock(SERIAL_HANDLE hSerialID, MAILID mailID, RwBool bIsLock, DBOTIME endTime);
	static void	MailCloseNfy();
	static void	MailSystemNfy(RwUInt32 uiNotifyType, RwUInt32 uiNotifyValue);
	static void MailMultiDel(SERIAL_HANDLE hObject, RwUInt8 byCount, MAILID* pMailID);

	// auction house
	static void AuctionHouseListRes(RwUInt32 uiPage, RwUInt32 uiMaxPage, BYTE byListNum, BYTE byPage);
	static void AuctionHouseListData(void* pItemData);
	static void AuctionHouseSellRes(BYTE byPlace, BYTE byPosition, BYTE byCount);
	static void AuctionHouseSellCancelRes(ITEMID nItem);
	static void AuctionHouseBuyRes(ITEMID nItem);

	// Char
	static void CharAway(bool bIsAway);

	//---------------------------------------------------------
	// ActionMap
	static void ActionMapLoadInfo( BYTE byCount, sSHORTCUT_DATA* pasData );
	static void ActionMapUpdateRes( RwBool bSuccess );
	static void ActionMapClientNotify( RwUInt8 byType, RwUInt16 wParam1 = 0xFFFF, RwUInt16 wParam2 = 0xFFFF );

	//---------------------------------------------------------
	// 정보 얻어오기
	static void SobGetState(SERIAL_HANDLE hSerialId, RwUInt8& byServerState, RwUInt8& byClientState, char* chClientState);

	static void SobGetFightingMode(SERIAL_HANDLE hSerialId, RwBool& bFightingMode);	

	static void SobGetAirMode(SERIAL_HANDLE hSerialId, RwBool& bAirMode);

	//---------------------------------------------------------
	// trigger => client event
	static void ReloadTS( void );

	static void RegQuest_Nfy( sTS_KEY& sTSKey,
		bool bNewRegister,						// 퀘스트 등록시 New type의 등록 여부
		bool bQuestShare,						// 퀘스트 공유
		RwUInt32 uiQuestAreaName,				// 퀘스트 지역 이름
		RwUInt32 uiQuestState,					// 퀘스트 상태
		RwUInt32 uiQuestTitle,					// 퀘스트 제목
		RwUInt32 uiQuestGoal,					// 퀘스트 목적
		eQUEST_SORT_TYPE eQuestSortType );		// 퀘스트 종류의 타입

	static void UnregQuest_Nfy( sTS_KEY& sTSKey );

	static void ShowQuestWindow( sTS_KEY& sTSKey );

	static void ShowQuestWindow_Nfy( sTS_KEY& sTSKey,
		RwUInt32 uiQuestAreaName,			// 퀘스트 지역 이름
		RwUInt32 uiQuestTitle,				// 퀘스트 제목
		RwUInt32 uiQuestSort,				// 퀘스트 종류
		RwUInt32 uiQuestGrade,				// 퀘스트 난이도
		eGRADE_TYPE eGradeType,				// 퀘스트 난이도 타입
		RwUInt32 uiQuestGoal,				// 퀘스트 목표
		RwUInt32 uiQuestContents,			// 퀘스트 내용
		sREWARD_INFO* psDefaultReward,		// 퀘스트 보상 정보 - 기본 보상
		sREWARD_INFO* psSelectReward,		// 퀘스트 보상 정보 - 선택 보상
		unsigned int uiRewardExp,
		unsigned int uiRewardZeni,
		eSTOC_EVT_DATA_TYPE eEvtInfoType,	// 서버 이벤트 정보 - 데이터 타입
		const uSTOC_EVT_DATA& uEvtInfoData,	// 서버 이벤트 정보 - 데이터
		RwUInt32 uiTimeLimit,				// 타임 제한 정보
		RwUInt32 uiEventType,				// eEVENT_GEN_TYPE ( 현재는 eEVENT_GEN_TYPE_CLICK_NPC 만 사용함 )
		RwUInt32 uiEventID,					// uiEventType 에 따른 인덱스, eEVENT_GEN_TYPE_CLICK_NPC : Mob table index
		RwBool bIsEventStarter );			// Event type [ True : Starter, False : Reward ]

	static void UpdateQuestState_Nfy( sTS_KEY& sTSKey,
		RwBool bOutStateMsg,					// 퀘스트 상태 메시지 출력 여부
		RwUInt32 uiUpdatedQuestFlag,			// 갱신된 퀘스트 상태 플래그
		RwUInt32 uiQuestState,					// 퀘스트 상태
		RwUInt32 uiQuestTitle,					// 퀘스트 제목
		eSTOC_EVT_DATA_TYPE eEvtInfoType,		// 서버 이벤트 정보 - 데이터 타입
		const uSTOC_EVT_DATA& uEvtInfoData,		// 서버 이벤트 정보 - 데이터
		RwUInt32 uiEventType,					// eEVENT_GEN_TYPE ( 현재는 eEVENT_GEN_TYPE_CLICK_NPC 만 사용함 )
		RwUInt32 uiEventID,						// uiEventType 에 따른 인덱스, eEVENT_GEN_TYPE_CLICK_NPC : Mob table index
		RwBool bIsEventStarter );				// Event type [ True : Starter, False : Reward ]


	static void UpdateQuestProgressInfo_Nfy( sTS_KEY& sTSKey,
		eSTOC_EVT_DATA_TYPE eEvtInfoType,	// 서버 이벤트 정보 - 데이터 타입
		uSTOC_EVT_DATA& uEvtInfoData,	// 서버 이벤트 정보 - 데이터
		RwUInt32 uiTimeLimit );			// 타임 제한 정보

	static void GiveUpQuest( sTS_KEY& sTSKey );

	static void ShowIndicator( sTS_KEY& sTSKey, RwBool bAuto );

	static void HideIndicator( sTS_KEY& sTSKey );

	static void ShowIndicator_Nfy( sTS_KEY& sTSKey,
		RwUInt32 uiQState,						// 퀘스트 상태
		RwUInt32 uiQuestTitle,					// 퀘스트 제목
		RwUInt32 uiQuestGoal,					// 퀘스트 목적
		eSTOC_EVT_DATA_TYPE eEvtInfoType,		// 서버 이벤트 정보 - 데이터 타입
		const uSTOC_EVT_DATA& uEvtInfoData,		// 서버 이벤트 정보 - 데이터
		RwUInt32 uiTimeLimit,					// 타임 제한 정보
		RwBool bAuto );

	static void DoQuest( sQUEST_INFO& sQuestInfo,							// Quest 정보
						 eEVENT_GEN_TYPE eEventGenType,						// Event 종류
						 RwUInt32 uiOwnerID,								// Owner ID
						 RwBool bTransCameraCtrlRight);						// 카메라 제어권을 넘기는 경우 TRUE

	static void QuestProposalDialog_Req( sTS_KEY& sTSKey,
		RwBool bDisplaySummary,				// 퀘스트 요약 정보 미리보기 보여주기 여부
		RwUInt32 uiQuestTitle,					// 퀘스트 제목
		RwUInt32 uiQuestGoal,					// 퀘스트 목표
		RwUInt32 uiQuestSort,					// 퀘스트 종류
		RwUInt32 uiQuestGrade,					// 퀘스트 난이도
		eGRADE_TYPE eGradeType,				// 퀘스트 난이도 타입
		RwUInt32 uiQuestContents,				// 퀘스트 내용
		const sREWARD_INFO* psDefaultReward,	// 퀘스트 보상 정보 - 기본 보상
		const sREWARD_INFO* psSelectReward,
		unsigned int uiRewardExp,
		unsigned int uiRewardZeni);	// 퀘스트 보상 정보 - 선택 보상

	static void QuestProposalDialog_Res( bool bResult,							// 퀘스트 수락(true) 또는 거절(false)
		sTS_KEY& sTSKey );

	static void QuestUserSelectDialog_Req( sTS_KEY& sTSKey,
		RwUInt32 uiTargetType,				// 타겟의 종류 ( eUSER_SEL_TARGET_TYPE -> eUSER_SEL_TARGET_TYPE_NPC, eUSER_SEL_TARGET_TYPE_OBJECT, eUSER_SEL_TARGET_TYPE_INVALID )
		RwUInt32 uiTargetTblIdx,				// 타겟의 테이블 인덱스 
		RwUInt32 uiQuestTitle,				// 퀘스트 제목
		RwUInt32 uiConv,						// 대화
		RwInt32 nBranchCnt,					// 현재 사용중인 Branch의 개수
		sTS_BRANCH_INFO* pBranchInfo );		// Branch info

	static void QuestUserSelectDialog_Res( bool bResult,						// Ok(true) 또는 Cancel(false)
		sTS_KEY& sTSKey,
		NTL_TS_TC_ID tcSelID );				// 선택된 퀘스트 아이디

	static void QuestNarrationUserSelectDialog_Req( sTS_KEY& sTSKey,
		eUSER_SEL_PROGRESS_STATE eProgState,	// 나래이션 진행 상태
		RwUInt32 uiConv,						// 대화
		RwInt32 nBranchCnt,						// 현재 사용중인 Branch의 개수
		sTS_BRANCH_INFO* pBranchInfo );			// Branch info

	static void QuestNarrationUserSelectDialog_Res( bool bResult,							// Ok(true) 또는 Cancel(false)
		sTS_KEY& sTSKey,
		NTL_TS_TC_ID tcSelID );					// 선택된 퀘스트 아이디

	static void TSFinished( unsigned char byTSType,								// Trigger type ( TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS )
		NTL_TS_T_ID tID );									// 종료되는 트리거 아이디

	static void TSSkipContainer( NTL_TS_T_ID tID,
		NTL_TS_TC_ID tcID );

	static void QuestRewardDialog_Req( sTS_KEY& sTSKey,
		eREWARD_CONTAINER_TYPE eRewardType,		// 보상 타입
		RwUInt32 uiLimitTime,					// 보상 제한시간
		RwUInt32 uiQuestTitle,					// 퀘스트 제목
		RwUInt32 uiQuestGoal,					// 퀘스트 목표
		RwUInt32 uiQuestSort,					// 퀘스트 종류
		RwUInt32 uiQuestGrade,					// 퀘스트 난이도
		eGRADE_TYPE eGradeType,					// 퀘스트 난이도 타입
		RwUInt32 uiQuestContents,				// 퀘스트 설명
		const sREWARD_INFO* psDefaultReward,		// 퀘스트 보상 정보 - 기본 보상
		const sREWARD_INFO* psSelectReward,
		unsigned int uiRewardExp,
		unsigned int uiRewardZeni);	// 퀘스트 보상 정보 - 선택 보상

	static void QuestRewardDialog_Res( bool bResult,							// Ok(true) 또는 Cancel(false)
		sTS_KEY& sTSKey,
		int nSelRwdIdx );						// 선택 보상의 선택된 인덱스


	static void QuestNarrationDialog_Req( sTS_KEY& sTSKey,
		eNARRATION_PROGRESS_STATE eProgState,			// Narration 진행 상태
		eNARRATION_OWNER_TYPE eOwnerType,				// Owner의 종류
		RwUInt32 uiOwnerIdx,							// Owner의 테이블 인덱스
		eNARRATION_OWNER_STATE eOwnerState,			// Owner의 상태
		eNARRATION_DIALOG_DIR_TYPE eDialogDirType,	// 대화 연출 종류
		RwUInt32 uiDialog,							// 대화 내용
		eNARRATION_GUI_TYPE eGUIType,					// GUI 종류
		RwUInt32 uiMaxLifeTime );						// 이 시간 이상 나레이션 창이 유지 되는 경우 자동으로 Next로 넘어가야 한다.

	static void QuestNarrationDialog_Res( bool bResult,							// Ok(true) 또는 Cancel(false)
		sTS_KEY& sTSKey );

	static void QuestNPCDialog_Req( sTS_KEY& sTSKey,
		eNPCCONV_TYPE eNPCConvType,					// NPC 대화 타입
		unsigned int uiNPCIdx,						// NPC 테이블 인덱스
		RwUInt32 uiDesc );							// NPC 대화

	static void QuestNPCDialog_Res( sTS_KEY& sTSKey );

	static void QuestShare_Nfy( sTS_KEY& sTSKey );

	static void QuestAcceptProposal_Nfy( sTS_KEY& sTSKey );

	static void QuestAcceptReward_Nfy( sTS_KEY& sTSKey, RwUInt32 uiQTitle, RwBool bOutStateMsg );

	static void QuestAcceptGiveUp_Nfy( sTS_KEY& sTSKey, RwUInt32 uiQTitle );

	static void RegisterQuestPosMark( QM_KEY QMKey,
		eQMI_TARGET_TYPE eTargetType,
		unsigned int uiTargetWorldTblIdx,
		unsigned int uiTargetTableIdx,
		float fTargetPosX,
		float fTargetPosY,
		float fTargetPosZ,
		unsigned int uiTooltipTblIdx );

	static void UnregisterQuestPosMark( QM_KEY QMKey );

	static void ShowQuestPosMark( sTS_KEY& sTSKey, RwBool bShow );

	static void QuestObjDialog_Req( sTS_KEY& sTSKey,
		eOBJCONV_TYPE eObjConvType,				// Object 대화 타입
		RwUInt32 uiWorldIdx,					// Object 소속 world index
		RwUInt32 uiObjIdx,						// Object 테이블 인덱스
		RwUInt32 uiDesc );						// Object 대화

	static void QuestObjDialog_Res( sTS_KEY& sTSKey );

	static void TutorialNfy(RwUInt32 uiMessage, RwBool bShow = true, RwUInt32 uiParam = 0xffffffff);

	static void RemovingTMQQuest_Nfy( NTL_TS_T_ID tRmvTSId );

	static void TSPCDialog_Nfy( RwUInt32 uiMessage );

	static void TSMudosaTeleport_Req( sTS_KEY& sTSKey );

	static void TSMudosaTeleport_Res( sTS_KEY& sTSKey, RwUInt8 byMudosaIndex, RwBool bCancel );

	static void TSItemUse( sGU_CHAR_ACTION_ITEM* pItemAction );

	static void TSItemGet( RwUInt32 uiItemIdx );

	static void TSItemEquip( RwUInt32 uiItemIdx );

	static void TSScoutUse( RwUInt32 uiItemIdx, RwUInt32 uiTargetClassID, RwUInt32 uiTargetIdx );

	static void TSSkillUse_Charging( SERIAL_HANDLE hSource );

	static void TSSkillUse_Guard( SERIAL_HANDLE hSource );

	static void TSSkillUse_Dash( SERIAL_HANDLE hSource );

	static void TSSkillUse_Skill( SERIAL_HANDLE hSource, SSkillStuff* pSkillStuff, CNtlAttackData* pAttackData );

	static void TSSkillUse_HTB( SERIAL_HANDLE hSource, SERIAL_HANDLE hTarget, RwUInt32 uiSkillTblId );

	static void TSRankBattle( RwUInt32 uiRBTblIdx, eRB_COND_FLAG eRBCondFlag );

	static void TSColTriggerObject( SERIAL_HANDLE hTriggerObject );

	static void TSBindStone( void );

	static void TSSearchQuest( void );

	static void TSItemUpgrade( void );

	static void TSTeleport( void );

	static void TSBudokai( void );

	static void TSSlotMachine( void );

	static void TSHoipoiMix( void );

	static void TSPrivateShop( void );

	static void TSFreeBattle( void );

	static void TSItemIdentity( void );

	static void TSUseMail( void );

	static void TSParty( void );

	static void TSClickSideIcon();
	static void TSLevelCheck();
	static void TSQuest();
	static void TSDialogOpen();

	//---------------------------------------------------------
	// trigger direct event
	static void	QuestTriggerDirectForward(SNtlEventQuestDirect_Forward *pDirect);

	static void QuestTriggerDirectEcho(SNtlEventQuestDirect_Echo *pDirect);

	static void QuestMark(SERIAL_HANDLE hSerialId, RwUInt8 byMarkType, void *pQuestInfo);

	//---------------------------------------------------------
	// trigger object
	static void TObjectUpdateState( SERIAL_HANDLE hSerialId, TBLIDX tblIdx, RwUInt8 byMainState, RwUInt8 bySubState, RwUInt32 uiStateTime );

	//---------------------------------------------------------
	// 트리거 오브젝트 스카우터 연출 관련
	static void ScouterEvent(int iType, RwUInt32 uiSerailID = 0, int iUserData2 = 0, int iUserData3 = 0, 
		int iUserData4 = 0, int iUserData5 = 0, int iUserData6 = 0, int iUserData7 = 0, int iUserData8 = 0);

	//---------------------------------------------------------
	// Char Ready
	static void CharReady(VOID);

	//---------------------------------------------------------
	// World Concept
	static void ChangeWorldConceptState(EWorldPlayConcept eWorldConcept, RwUInt32 uiState);

	static void TMQCleintState(EWorldPlayConcept eWorldConcept, RwUInt32 uiState);

	// UI
	static void ShowToolTip(RwBool bShow, RwInt32 nX, RwInt32 nY, const std::wstring& wstrText);    //< UI의 툴팁을 표시한다. 좌표를 0으로 넣으면 마우스 커서 위치에 표시한다.
	static void UIConfigReload();

	// Portal System
	static void PortalInfoRes(BYTE byCount, PORTALID* paPortalID);
	static void PortalAddReq(RwUInt32 hNPCSerialID);
	static void PortalAddRes(PORTALID portalID);
	static void PortalRes(PORTALID byPoint);

	// Operate object
	static void OperateObjectTimeBar(bool bShow, RwUInt32 uiPlayTime);

	// Friend System
	static void FriendAddRes(WORD wResultCode, WCHAR* wchName, RwUInt32 uiTargetID);
	static void FriendDelRes(WORD wResultCode, RwUInt32 uiTargetID);
	static void FriendMoveRes(WORD wReslutCode, RwUInt32 uiTargetID);
	static void FriendListInfo(BYTE byCount, sFRIEND_FULL_INFO* paInfo);
	static void BlackListAddRes(WORD wResultCode, WCHAR* wchName, RwUInt32 uiTargetID);
	static void BlackListDelRes(WORD wResultCode, RwUInt32 uiTargetID);
	static void FriendInfo(sFRIEND_INFO* pInfo, bool bIsFirst);
	static void FriendInfoChange(RwUInt32 uiTargetID, BYTE byChangeType, DWORD dwChangeValue);
	static void FriendInfoNotify(RwInt32 nNotifyType, RwUInt32 uiTargetID);             ///< UI 갱신을 위한 이벤트 (데이터->UI)

	// KnockDownNotify -> Client Wakeup
	static void KnockDownNfy();
	static void KnockDownWakeUpNfy();
	static void BlockingNfy(RwBool bStartOrEnd, DWORD dwCoolingTime);
	static void ChargingNfy(RwBool bStartOrEnd);

	// Popo Icon Notify
	static void PopoNotify( const WCHAR* pwcTitle, const WCHAR* pwcBuffer, RwBool bImmediate = FALSE, RwReal fLifeTime = 0.0f );

	// 버스 및 탈것 관련 이벤트
	static void SobOnBus(SERIAL_HANDLE hSerialId, RwBool bRideOn, SERIAL_HANDLE hBusSerialId);
	static void SobOnVehicle( SERIAL_HANDLE hSerialId, RwBool bRideOn, SERIAL_HANDLE hDriverId, RwUInt32 uiVehicleTblIdx );

	static void BusMove(RwUInt8 byMessage, SERIAL_HANDLE hSerialId, TBLIDX idxBusTable, RwV3d* pV3Pos, RwV3d* pV3Dir);

	static void SobVehicleStart( SERIAL_HANDLE hDriver, SERIAL_HANDLE hVehicleItem, TBLIDX tblVehicleItem );
	static void SobRideOnOffVehicle( SERIAL_HANDLE hSerialId, RwBool bRideOn, RwUInt32 uiVehicleTblIdx );
	static void SobVehicleEngine(SERIAL_HANDLE hSerialId, RwUInt8 byMessage, RwUInt32 uiParam = 0xffffffff);
	static void SobVehicleStunt(SERIAL_HANDLE hSerialId);

	// Charm point
	static void CharmPointUpdate(RwUInt32 uiCharmPoint);
	static void CharmPointNotify(RwUInt32 uiCharmPoint);

	// 변신 관련 이벤트
	static void SobTransform(SERIAL_HANDLE hSerialId, const sASPECTSTATE& aspectState);                                 ///< PC의 변신 이벤트
    static void SobMobTransform(SERIAL_HANDLE hSerialId, TBLIDX tblTransformMobId);                                     ///< Mob의 변신 이벤트
    static void SobTransformCandy(SERIAL_HANDLE hSerialId, RwBool bTransform);                                          ///< 사탕 변신 이벤트
    static void SobTransformSequela(SERIAL_HANDLE hSerialId);                                                           ///< 초사이야인 변신 휴유증 이벤트 (아바타 전용)

	static void SobCinematicScale(SERIAL_HANDLE hSerialId, float fScale, float fTime);

	// 컨디션 관련 이벤트
	static void SobAfterEffectRemainTimeNfy(DWORD dwTimeRemaining);                                                     ///< 변신휴유증 남은 시간
	static void SobCondConfused(RwBool bAffect, SERIAL_HANDLE hSubject, SERIAL_HANDLE hTargetSerialId);                 ///< 혼란 상태
	static void SobCondTerror(RwBool bAffect, SERIAL_HANDLE hSubject, SERIAL_HANDLE hAttackerSerialId);                 ///< 공포 상태
    static void SobCondHidingKi(RwBool bAffect, SERIAL_HANDLE hSubject);                                                ///< 기숨기기

	// Tutorial
	static void TLDefaultSkin( const char* pFontName, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwInt32 nFontSize, RwUInt32 uiFontAllign, RwV2d& vFontOffset, RwV2d& vMsgLayerOffset, RwV2d& vOkOffset, RwV2d& vIMGOffset );
	static void TLGuideMsg_ShowHide( RwUInt32 uiTextTblIdx, RwReal fFadeInOutTime, RwBool bShow );
	static void TLGuideMsg_ChangeColor( RwUInt32 uiTextTblIdx, RwReal fFadeInOutTime, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor );
	static void TLGuideIMG_ShowHide( RwChar* pIMGName, RwReal fFadeInOutTime, RwBool bShow, RwReal fSizeRate = 1.f );
	static void TLGuideIMGOk_ShowHide( RwReal fFadeInOutTime, RwBool bShow );
	static void TLUIIndicator_ShowHide( RwUInt32 uiTLIndicatorType, RwChar* pcFlashName, RwV2d& vFocusOffPos, RwBool bShow );
	static void TLMouseInputLockUnlock( RwUInt32 uiMouseInputType, RwBool bLock );
	static void TLKeyboardInputLockUnlock( RwUInt32 uiKeyboardInputType, RwBool bLock );
	static void TLActionMapBackup( void );
	static void TLActionMapRestore( void );
	static void TLExcuteQuest( RwUInt16 tQuestID );
	static void TLAttackLockUnlock( RwUInt32 uiAttackType, RwBool bLock );
	static void TLPlayCamera_TBLIDX( RwUInt32 uiCameraType, RwUInt32 uiTblIdx );
	static void TLMinimap( RwBool bShow );
	static void TLNotifyLockUnlock( RwUInt8 byLockType, RwUInt8 byIndex, RwBool bLock );
	static void TLPlayCamera_POS( RwUInt32 uiCameraType, RwV3d& vTargetPos );
	static void TLCondition( RwUInt32 uiCondType, RwUInt32 uiParam1 = 0xffffffff );
	static void TLDisableMinimap( void );
	static void TLDisableDialogMoveOption(RwBool bDiableDialogMoveOption);
	static void TLEnableSkip(RwBool bEnableSkip);

	// 천하제일 무도회
	static void MinorMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState );
	static void MinorMatchTeamInfo( RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData );
	static void MinorMatchPlayerState( RwUInt32 hPlayer, RwUInt8 byPcState );
	static void MinorMatchUpdateScore( RwUInt16 wTeamType, RwUInt8 byScore, RwUInt32 hSlayer, RwUInt32 hFainter );
	static void MinorMatchTeamScore( RwUInt8 byCount, sMATCH_MINORMATCH_TEAM_SCORE* pTeamScore );
	static void MinorMatchSelection( RwUInt16 wWinnerTeam, RwUInt8 byTeamCount, sMINORMATCH_SELECTION_DATA* pSelection );
	static void MinorMatchStageFinish( RwUInt8 byStageResult, RwUInt16 wStageWinner );
	static void MinorMatchMatchFinish( RwUInt8 byMatchResult, RwUInt16 wMatchWinner, RwUInt8 byKillCount );
	static void MajorMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState );
	static void MajorMatchTeamInfo( RwUInt8 byMatchDepth, RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData );
	static void MajorMatchPlayerState( RwUInt32 hPlayer, RwUInt8 byPcState );
	static void MajorMatchUpdateScore( sMATCH_SCORE* pStageScore );
	static void MajorMatchStageFinish( sMATCH_SCORE* pStageScore, RwUInt16 wStageWinner, RwUInt8 byMatchResult, bool bIsObserver );
	static void MajorMatchMatchFinish( sMATCH_SCORE* pStageScore, RwUInt16 wMatchWinner, RwUInt8 byMatchResult );
	static void FinalMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState );
	static void FinalMatchTeamInfo( RwUInt8 byMatchDepth, RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData );
	static void FinalMatchPlayerState( RwUInt32 hPlayer, RwUInt8 byPcState );
	static void FinalMatchUpdateScore( sMATCH_SCORE* pStageScore );
	static void FinalMatchStageFinish( sMATCH_SCORE* pStageScore, RwUInt16 wStageWinner, RwUInt8 byMatchResult, bool bIsObserver );
	static void FinalMatchMatchFinish( sMATCH_SCORE* pStageScore, RwUInt16 wMatchWinner, RwUInt8 byMatchResult );
	static void AwardInfo( RwUInt16* paTeamNameIndex, sVARIABLE_DATA* pData );

	static void BudokaiStateInfoNfy(RwUInt16 wSeasonCount, BUDOKAITIME tmDefaultOpen, 
		sBUDOKAI_UPDATE_STATE_INFO* pStateInfo, 
		sBUDOKAI_UPDATE_MATCH_STATE_INFO* pMatchStateInfo,
		sBUDOKAI_JOIN_INFO*	pJoinInfo );
	static void BudokaiUpdateStateNfy( sBUDOKAI_UPDATE_STATE_INFO* pStateInfo );
	static void BudokaiUpdateMatchStateNfy( RwUInt8 byMatchType, sBUDOKAI_UPDATE_MATCH_STATE_INFO* pStateInfo );
	static void BudokaiJoinIndividualRes( RwUInt16 wResultCode, RwUInt16 wJoinId, RwBool bDojoRecommender );
	static void BudokaiLeaveIndividualRes( RwUInt16 wResultCode );
	static void BudokaiJoinTeamInfoRes( RwUInt16 wResultCode, RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo );
	static void BudokaiJoinTeamRes( RwUInt16 wResultCode, RwUInt32 hTroublePC, RwReal fTotalRankPoint, RwUInt16 wJoinId, WCHAR* pTeamName
		, RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo );
	static void BudokaiJoinTeamNfy( RwReal fTotalRankPoint, RwUInt16 wJoinId, WCHAR* pTeamName, RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo );
	static void BudokaiLeaveTeamRes( RwUInt16 wResultCode );
	static void BudokaiLeaveTeamNfy( void );
	static void BudokaiLeaveTeamMemberRes( RwUInt16 wResultCode );
	static void BudokaiLeaveTeamMemberNfy( WCHAR* pwcMemberName );
	static void BudokaiJoinInfoRes( RwUInt8 wResultCode, RwUInt8 byMatchType,
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO* pIndividualInfo,
		sBUDOKAI_REGISTER_TEAM_INFO* pTeamInfo );
	static void BudokaiMudosaInfoRes( RwUInt16 wResultCode, RwUInt8 byMudosaCount, sBUDOKAI_MUDOSA_INFO* paMudosaInfo);
	static void BudokaiJoinStateNfy( RwUInt8 byMatchType, RwUInt8 byJoinState, RwUInt8 byJoinResult );
	static void BudokaiJoinStateRes( RwUInt16 wResultCode, RwUInt8 byMatchType, RwUInt8 byJoinState, RwUInt8 byJoinResult );
	static void BudokaiNoticeNfy(RwUInt8 byNoticeType, RwUInt32 tblidxNotice, void* pData );
	static void BudokaiProgressMessageNfy( RwUInt8 byMsgId );

	static void CinematicGuiCommand(RwChar* szCommand, RwBool bShow);
	
	static void DialogEvent( RwInt32 nType );

	static void GambleAniStart( SERIAL_HANDLE hGambleNpc );
	static void GambleAniEnd( SERIAL_HANDLE hGambleNpc );

	static void GameServerChangeOut(void);
	static void GameServerChangeIn(void);
	static void GameServerChannelChanged(void);

	// Game Chat option
	static void	GameChatOption( RwUInt8 byData1, RwUInt8 byData2, RwUInt8 byData3 = 0);

	// Naming
	static void ChangeCharName( CHARACTERID charid, WCHAR* wchCharName, WCHAR* wchOldCharName);
	static void ChangeNickName( SERIAL_HANDLE hSerialID, TBLIDX nicknameTblidx );

	// ChangeClassAuthorityChanged
	static void ChangeClassAuthorityChanged( RwBool bCanChangeClass );

	// Hoipoi Item Machine
	static void HoipoiMixRecipeReg( TBLIDX idxRecipeTbl );
	static void HoipoiMixItemRecipeInfo(RwUInt16 byCount, void* pRecipeData );
	static void HoipoiMixItemMakeExp( RwUInt32 dwCurExp, RwUInt8 byCurLevel, DWORD dwExpGained);
	static void HoipoiMixInfoUpdate( RwUInt8 byType, RwUInt32 dwData );

	// DynamicObject
	static void DynamicObjectUpdateState( SERIAL_HANDLE hSerial, sDYNAMIC_OBJECT_BRIEF* pBrief, sDYNAMIC_OBJECT_STATE* pState );

	// Character battle attribute refresh ( on target )
	static void CharRefreshBattleAttributeModifier( SERIAL_HANDLE hSerial,
		RwReal fHonestOffenceModifier, RwReal fHonestDefenceModifier, RwReal fStrangeOffenceModifier, RwReal fStrangeDefenceModifier,
		RwReal fWildOffenceModifier, RwReal fWildDefenceModifier, RwReal fEleganceOffenceModifier, RwReal fEleganceDefenceModifier,
		RwReal fFunnyOffenceModifier, RwReal fFunnyDefenceModifier );

	static void CharRankPointResetNfy( void );

	static void QuickSlotGuiMode( RwBool bExpand1, RwBool bExpand2 );
	static void QuickSlotLockMode( RwBool bLock );

	static void MobDie( RwUInt32 uiSerialID );
};

#endif
