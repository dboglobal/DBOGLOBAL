/*****************************************************************************
 *
 * File			: NtlSLPacketGenerator.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: simulation layer simulation object packet generator
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLPACKET_GENERATOR_H__
#define __NTL_SLPACKET_GENERATOR_H__

#include "NtlPacketGenerator.h"
#include "NtlSLDef.h"
#include "NtlQuest.h"
#include "NtlMail.h"
#include "NtlCharacter.h"

class CNtlSobActor;

class CNtlSLPacketGenerator : public CNtlPacketGenerator
{
public:

	CNtlSLPacketGenerator(CNtlNetSender *pNetSender);

	bool SendLoadingCompleteNfy(void);

	bool SendCharSpawnReady(void);

	bool SendCharReady(RwUInt8 byAvatarType);

	bool SendPetAttackTargetNfy(RwUInt8 byAvatarType);

	bool SendPetDismissReq(RwUInt8 byAvatarType);

	// 이동
	bool SendMouseMove(RwV3d vCurr, RwV3d vDest, RwUInt8 byAvatarType);

	bool SendKeyboardMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag, RwUInt8 byAvatarType);
	bool SendKeyboardAirMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag);

	bool SendMouseDashMove(RwV3d vDest);

	bool SendKeyboardDashMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag);

	bool SendAirDashMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveDirection);
	bool SendAirAccelMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveDirection);

	bool SendChangePosition(RwV3d vCurr, RwV3d vDir);

	bool SendChangeHeading(RwV3d vDir);

	bool SendCharMoveCollision(CNtlSobActor* pActor);
	bool SendCharMoveCollisionEnd(CNtlSobActor* pActor);

	bool SendCharTargetSelect(SERIAL_HANDLE hSerialId);

	bool SendCharTargetFacing(SERIAL_HANDLE hSerialId, bool bStart);

	bool SendCharFollowMove(SERIAL_HANDLE hTargetSerialId, RwReal fFollowRange, RwUInt8 byMovementReason, RwUInt8 byAvatarType);

	// jump
	bool SendJump(RwUInt8 byMoveDirection, RwV3d vLoc, RwV3d vDir);

	bool SendAirJump(RwUInt8 byMoveDirection, RwV3d vLoc, RwV3d vDir);

	bool SendChangeDirectionFloating(RwUInt8 byMoveDirection, RwV3d vDir);

	bool SendJumpEnd(void);

	// charging
	bool SendCharging(void);

	bool SendChargingEnd(void);

	bool SendBlocking(void);

	bool SendBlockingEnd(void);

	// 앉기/서기
	bool SendSitAndStand(RwBool bSitDown);

	// falling
	bool SendFalling(RwBool bFalling, RwV3d vCurrPos, RwV3d vCurrDir, RwUInt8 byMoveDirection);

	//air
	bool SendAirFalling(RwV3d vCurrPos, RwV3d vCurrDir, RwUInt8 byMoveDirection);
	bool SendAirEnd(RwV3d vCurrPos, RwV3d vCurrDir);

	// 공격
	bool SendCharToggleFighting(RwBool bFightingMode, RwUInt8 byAvatarType);
	bool SendCharAttackBegin(RwUInt8 byType, RwUInt8 byAvatarType);
	bool SendCharAttackEnd(RwUInt8 byType, RwUInt8 byAvatarType);
	bool SendCharSkillReq(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx, RwUInt8 byRpBonusType, RwUInt8 byAvatarType);
	bool SendCharSkillTargetList(RwUInt8 byAvatarType);
	bool SendCharSocialReq(RwUInt32 uiTblId);
	bool SendCharHTBReq(SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx);
	bool SendCharHTBForward(RwV3d vCurr, RwV3d vDir);
	bool SendCharItemUseReq(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerialId, RwUInt8 byPlace, RwUInt8 byPos, SERIAL_HANDLE hRefObject, RwUInt8 byKeyPlace, RwUInt8 byKeyPos);
	bool SendCharTeleportReq(void);
	bool SendCharDirectPlayCancel();
	bool SendCharDirectPlayAck(void);
    bool SendCharSkillCastingCanceledNfy(WORD uiResultCode);

    // 데미지 입었을때 이동 관련 처리 (넉다운, 슬라이딩, Push)
    bool SendCharLocAfterKnockdownNfy(const RwV3d& vLoc, const RwV3d& vDir);
    bool SendCharLocAfterSlidingNfy(const RwV3d& vLoc, const RwV3d& vDir);
    bool SendCharLocAfterPushNfy(const RwV3d& vLoc, const RwV3d& vDir);

	// 바인드
	bool SendCharBindReq( TBLIDX bindObjectTblidx );

	// 퀘스트
	bool SendTSConfirmStepReq( BYTE byTSType, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, RwUInt32* uiParam, RwUInt8 byEventType, RwUInt32 uiEventData );
	bool SendQuestGiveUpReq( NTL_TS_T_ID tId );
	bool SendTSUpdateState( NTL_TS_T_ID tId, RwUInt8 byTSType, RwUInt8 byType, RwUInt16 wTSState, RwUInt32 uiParam );
	bool SendTSExcuteTriggerObject( HOBJECT hSource, HOBJECT hTarget, RwUInt8 byEvtGenType, RwUInt32 uiParam );
	bool SendQuestShare( NTL_TS_T_ID tId );
	bool SendQuestObjectVisitReq( NTL_TS_T_ID tId, RwUInt32 uiWorldID, RwUInt8 byObjType, RwUInt32 uiObjectTblIdx );

	// PrivateShop
	bool SendPrivateShopCreateReq();

	// ActionMap
	bool SendCharKeyUpdateReq(sSHORTCUT_UPDATE_DATA* pUpdateData, BYTE byCount);

	// MailSystem
	bool SendMailLoadReq(RwUInt32 hSerialID, RwUInt8 uiCount, MAILID aMailID[NTL_MAX_MAIL_SLOT_COUNT]);

	// 공유 target
	bool SendCrossFireReq(RwUInt32 hSerialID);

    // 버스및 탈것
    bool SendRideOnBus(RwUInt32 hTargetId);                     ///< 버스 탑승을 요청한다.    
    bool SendRideOffBus();                                      ///< 버스 하차를 요청한다.    

	// vehicle
	bool SendVehicle_EngineFuelRemoveNfy();
	bool SendVehicle_EndReq();
	bool SendVehicle_FuelInsertReq(BYTE byPlace, BYTE byPos);

	bool SendVehicleDirectPlayCancelNfy();

	bool SendVehicleStuntNfy();

	// 천하제일 무도회 관련
	bool SendBudokaiMudosaTeleport(RwUInt8 byMudosaIndex);
	bool SendBudokaiJoinStateReq();
	bool SendBudokaiSocialAction(RwUInt32 uiAnimId);

	// HTB Ball
	bool SendHTBRPBallUseReq(RwUInt8 byBallCount);
};


#endif