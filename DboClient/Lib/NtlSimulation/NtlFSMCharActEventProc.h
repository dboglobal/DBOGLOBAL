/*****************************************************************************
 *
 * File			: NtlFSMCharActEventProc.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: fsm character action event proc
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_FSM_CAHRACT_EVENTPROC_H__
#define __NTL_FSM_CAHRACT_EVENTPROC_H__

#include "NtlSLEvent.h"

class CNtlSobActor;
class CNtlFSMStateBase;



void FillAttackStuff(SAttackStuff *pAttStuff, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSerialId, RwUInt8 byAttResult,
					 RwBool bChainAtt, RwUInt8 byAttSeq, RwUInt8 byBlocking, SAttackDamageStuff sDamage, RwV3d vPushDir, RwUInt8 byLogicType, 
					 RwBool bAnimSpeedApply, RwUInt32 uiLogicTblId, RwUInt32 uiSyncEventId, RwV3d vFinalLoc);

RwUInt8 JumpKeyMapping(RwUInt8 byCurrKey, RwUInt8 byNextKey);
RwUInt8 JumpKeyStopMapping(RwUInt8 byCurrKey);

// device input event
void FSMEvent_CharActActionMapJump(CNtlSobActor *pActor, SNtlEventActionMapJump *pJump, CNtlFSMStateBase *pStateBase);
void FSMEvent_CharActActionMapAirJump(CNtlSobActor *pActor, SNtlEventActionMapAirJump *pJump, CNtlFSMStateBase *pStateBase);
void FSMEvent_CharActActionMapSkillUse(CNtlSobActor *pActor, SNtlEventActionMapSkillUse *pSkillUse);
void FSMEvent_CharActActionMapSocialUse(CNtlSobActor *pActor, SNtlEventActionMapSocialUse *pSocialUse);
void FSMEvent_CharActActionMapHTBUse(CNtlSobActor *pActor, SNtlEventActionMapHTBUse *pHTBUse);
void FSMEvent_CharActActionMapAutoFollow(CNtlSobActor *pActor, SNtlEventActionMapAutoFollow *pAutoFollow);

void FSMEvent_CharActTerrainClick(CNtlSobActor *pActor, SNtlEventTerrainClick *pTerrainClick);
void FSMEvent_CharActKeyboardMove(CNtlSobActor *pActor, SNtlEventKeyboardMove *pKeyboardMove);
void FSMEvent_CharActKeyboardMoveStop(CNtlSobActor *pActor);
void FSMEvent_CharActMouseDashMove(CNtlSobActor *pActor, SNtlEventMouseDashMove *pMouseDashMove);
void FSMEvent_CharActKeyboardDashMove(CNtlSobActor *pActor, SNtlEventKeyboardDashMove *pKeyboardDashMove);
RwBool FSMEvent_CharActAirDashMove(CNtlSobActor *pActor, SNtlEventAirDashMove *pAirDashMove);
RwBool FSMEvent_CharActAirAccelMove(CNtlSobActor* pActor, SNtlEventAirAccelMove* pAirMove);
void FSMEvent_CharActInputChangeHeading(CNtlSobActor *pActor, SNtlEventInputChangeHading *pChangeHeading);

void FSMEvent_CharActSobMove(CNtlSobActor *pActor, SNtlEventSobMove *pMove);
void FSMEvent_CharActSobSecondDestMove(CNtlSobActor *pActor, SNtlEventSobSecondDestMove *pMove);
void FSMEvent_CharActSobSplineMove(CNtlSobActor *pActor, SNtlEventSobSplineMove *pSplineMove);
void FSMEvent_CharActSobDash(CNtlSobActor *pActor, SNtlEventSobDash *pDash);
void FSMEvent_CharActSobFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove);
void FSMEvent_CharActSobFightingFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove);
void FSMEvent_CharActSobSkillFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove);
void FSMEvent_CharActSobMoveStop(CNtlSobActor *pActor, SNtlEventSobMoveStop *pMoveStop);
void FSMEvent_CharActSobDirectionFloat(CNtlSobActor *pActor, SNtlEventSobDirectionFloat *pFloat);
void FSMEvent_CharActSobTargetSelect(CNtlSobActor *pActor, SNtlEventSobTargetSelect *pSobTargetSelect);
void FSMEvent_CharActSobAttackSelect(CNtlSobActor *pActor, SNtlEventSobAttackSelect *pSobAttackSelect);
void FSMEvent_CharActAttack(CNtlSobActor *pActor, SNtlEventSobAttack *pAttack);
void FSMEvent_CharActHit(CNtlSobActor *pActor, SNtlEventSobHit *pHit);
void FSMEvent_CharActHit(CNtlSobActor* pActor, BYTE byAttackResult, WORD wAttackResultValue);       ///< 회전 공격의 Push에서 공격 결과와 데미지만 계산하기 위한 함수
void FSMEvent_CharActStanding(CNtlSobActor *pActor, SNtlEventSobStanding *pStanding);
void FSMEvent_CharActJump(CNtlSobActor *pActor, SNtlEventSobJump *pJump);
void FSMEvent_CharActAirJump(CNtlSobActor *pActor, SNtlEventSobAirJump *pJump);
RwBool FSMEvent_CharActAirAccel(CNtlSobActor *pActor, SNtlEventSobAirAccel *pAccel);
void FSMEvent_CharActSit(CNtlSobActor *pActor, SNtlEventSobSit *pSit);
void FSMEvent_CharActDirectPlay(CNtlSobActor *pActor, SNtlEventSobDirectPlay *pDirectPlay);
void FSMEvent_CharActAnimPlay(CNtlSobActor *pActor, SNtlEventSobAnimPlay *pAnimPlay);
void FSMEvent_CharActPrivateShop(CNtlSobActor *pActor, SNtlEventSobPrivateShop *pPrivateShop);
void FSMEvent_CharActPrivateShopState(CNtlSobActor *pActor, SNtlEventPrivateShopState *pPrivateShopState);
void FSMEvent_CharActSobPushing(CNtlSobActor* pActor, SNtlEventSobPushingNfy* pPushingNfy);

void FSMEvent_CharActSkillCasting(CNtlSobActor *pActor, SNtlEventSobSkillCasting *pSkillCasting);
void FSMEvent_CharActSkillAction(CNtlSobActor *pActor, SNtlEventSobSkillAction *pSkillAction);
void FSMEvent_CharActHTBSkillAction(CNtlSobActor *pActor, SNtlEventSobHTBSkillAction *pHTBSkillAction);
void FSMEvent_CharActSocialAction(CNtlSobActor *pActor, SNtlEventSobSocialAction *pSocialAction);

void FSMEvent_CharActItemCasting(CNtlSobActor *pActor, SNtlEventSobItemCasting *pItemCasting);

void FSMEvent_CharActFainting(CNtlSobActor *pActor, SNtlEventSobFainting *pFainting);

#endif
