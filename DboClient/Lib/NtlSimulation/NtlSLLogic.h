/*****************************************************************************
 *
 * File			: NtlSLLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 10	
 * Abstract		: simulation logic.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLLOGIC_H__
#define __NTL_SLLOGIC_H__

#include "NtlSharedType.h"
#include "NtlSLLogicDef.h"
#include "NtlSLEvent.h"
#include "NtlSLServerDeclear.h"
#include "NtlAnimEventData.h"

struct sWORLD_INFO;
struct sDOJO_INFO;
struct sEmblemFactor;

class CNtlSob;
class CNtlSobActor;
class CNtlSobPlayer;
class CNtlSobNpc;
class CNtlSobElapsedController;
class CNtlSobItem;
class CNtlSobItemAttr;
class CNtlSobVehicle;

class CNtlStorageUnit;


extern RwReal	g_fCharColliAngle;
extern RwReal	g_fCharSlippingAngle;
extern RwUInt32	g_uiFlashBalloonWidth;
extern RwUInt32 g_uiFlashBalloonHeight;
extern RwReal	g_fMatrixTime;
extern RwReal	g_fMatrixValue;
extern RwReal	g_fMatrixDelayTime;
extern RwReal	g_fMatrixCamera;
extern RwBool	g_bDispPacketLockMsg;
extern RwBool	g_bDispServerMsg;

extern RwUInt8	g_byShadowRed;
extern RwUInt8	g_byShadowGreen;
extern RwUInt8	g_byShadowBlue;

extern RwReal   g_fTestVal;             /// 다양한 테스트에 사용되는 전역 변수 
extern RwReal   g_fTestVal2;
extern RwReal   g_fTestVal3;

extern RwInt32	g_nTestVal;

extern RwInt32	g_nBroadCastGuiPosX;

extern RwUInt32 g_uiTargetMarkingId;    ///< 현재 타겟 마케팅 객체의 ID

extern RwBool	g_bActiveAnimSync;

extern RwUInt32	g_uiLimitMemory;

extern RwUInt32	g_uiCreateHitEffectRate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 함수 포인터 등록
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef const WCHAR* (*Callback_GetNPCJobName)(RwUInt8 byJob);
extern Callback_GetNPCJobName g_fnGetNPCJobName;

void LinkCallback_GetNPCJobName(Callback_GetNPCJobName fn);
const WCHAR* Logic_GetSLNPCJobName(RwUInt8 byJob);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_GetCurMemory(void);

void   Logic_CalcLimitMemory(SIZE_T TotalPhysMemory);

RwBool Logic_IsCreateHitEffect(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// game master key definition
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_IsDevUser(void);

RwBool Logic_IsUIDevInfoVisible(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// monster/player logic
// 주의 : 속도는 table data와 상태 및 버프가 고려된 속도가 될것이다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor 속성 logic

// name
const WCHAR* Logic_GetName(CNtlSob *pSobObj);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LP
void		Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp);

void		Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp, RwUInt32 uiMaxLp);

void		Logic_SetLpPlusMinus(CNtlSob *pSobObj, RwInt32 iDeltaLp);

RwUInt32	Logic_GetLp(CNtlSob *pSobObj);

RwUInt32	Logic_GetMaxLp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EP
void		Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp);

void		Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp, RwUInt32 uiMaxEp);

RwUInt32	Logic_GetEp(CNtlSob *pSobObj);

RwUInt32	Logic_GetMaxEp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RP
void		Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp);
void		Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp, RwUInt32 uiMaxRp);
void        Logic_SetRpStock(CNtlSob* pSobObj, RwUInt32 uiRpStock);                 
void        Logic_SetRpStockMax(CNtlSob* pSobObj, RwUInt32 uiRpStockMax);
RwUInt32	Logic_GetRp(CNtlSob *pSobObj);
RwUInt32	Logic_GetMaxRp(CNtlSob *pSobObj);
RwUInt32    Logic_GetRpStock(CNtlSob* pSobObj);
RwUInt32    Logic_GetMaxRpStock(CNtlSob* pSobObj);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AP

void		Logic_SetAp(CNtlSob *pSobObj, int nAp, int nMaxAp);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Level
void		Logic_SetLevel(CNtlSob *pSobObj, RwUInt8 byLevel);

RwUInt8		Logic_GetLevel(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exp
void		Logic_SetExp(CNtlSob *pSobObj, RwUInt32 uiExp);

RwUInt32	Logic_GetExp(CNtlSob *pSobObj);

void		Logic_SetMaxExp(CNtlSob *pSobObj, RwUInt32 uiMaxExp);

RwUInt32	Logic_GetMaxExp(CNtlSob *pSobObj);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Zenny
void		Logic_SetZenny(RwUInt32 uiZenny);
RwUInt32	Logic_GetZenny(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HLS Cash
void		Logic_SetHlsCash(DWORD dwCash);
DWORD		Logic_GetHlsCash(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sp
void		Logic_SetSp( RwUInt32 uiSp );

RwUInt32	Logic_GetSp(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Honor Point
void		Logic_SetHonorPoint( RwUInt32 uiHonorPoint );
RwUInt32	Logic_GetHonorPoint(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mudosa Point
void		Logic_SetMudosaPoint( RwUInt32 uiMudosaPoint );
RwUInt32	Logic_GetMudosaPoint(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// avatar status value
void		Logic_SetStatus(CNtlSob *pSobObj, RwUInt8 byAttributeTotalCount, RwUInt8 *pData);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Speed
void		Logic_SetSpeed(CNtlSobActor *pActor, RwReal fWalkSpeed, RwReal fRunSpeed);
RwReal		Logic_GetDefaultFrontWalkSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFrontWalkSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDefaultFrontRunSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFrontRunSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDefaultBackSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetBackSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetTurningSpeed(CNtlSobActor *pActor);
RwBool		Logic_IsActorWalkMove(CNtlSobActor *pActor, RwUInt8 byMoveForm);
void		Logic_SetAttackAnimSpeed(CNtlSobActor *pActor, RwReal fAnimSpeed);
RwReal		Logic_GetAttackAnimSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetSkillAnimSpeed(CNtlSobActor *pActor, sSKILL_TBLDAT *pSkilTblData);
RwReal      Logic_GetRunAnimSpeed(CNtlSobActor* pActor);
RwReal		Logic_GetFrontFlySpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFlyDashSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetFlyAccelSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDashSpeed(CNtlSobActor *pActor);
RwReal		Logic_GetDashAnimSpeed(CNtlSobActor* pActor);

float		Logic_GetMaxDashDistanceForward(CNtlSobActor *pActor);
float		Logic_GetMaxDashDistanceBackLeftRight(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// player 전직 
RwUInt8		Logic_GetPlayerRace(CNtlSobActor *pActor);

RwUInt8		Logic_GetPlayerClass(CNtlSobActor *pActor);

RwUInt8     Logic_GetPlayerGender(CNtlSobActor *pActor);

void		Logic_SetReputation(CNtlSobActor *pActor, RwUInt32 uiReputation);

RwUInt32	Logic_GetReputation(CNtlSobActor *pActor);

RwUInt8		Logic_ConverClasstoRace(RwUInt8 byClass);

RwBool		Logic_IsChild(CNtlSobActor *pActor);

RwBool		Logic_IsFirstClass(CNtlSobPlayer* pPlayer);	// 1차직이냐.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attack , npc community , skill use range

RwReal		Logic_GetRadius(CNtlSobActor *pActor);
RwReal		Logic_GetAttackRange(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);
RwReal		Logic_GetCommunityRange(CNtlSobActor *pActor);
RwReal		Logic_GetCommunityOutRange(CNtlSobActor *pActor);
void		Logic_BeginActorCommunity(CNtlSobActor *pActor, CNtlSobActor *pTarActor); 
RwReal		Logic_GetSkillUseRange(sSKILL_TBLDAT *pSkillTblData);
RwReal      Logic_GetSkillApplyRange(CNtlSobActor* pActor, RwUInt32 uiSkillTblidx);         /// 범위 스킬의 범위를 반환한다.
RwBool		Logic_IsMasterSkill(SERIAL_HANDLE hSkillSerial);
RwReal		Logic_GetCurrentCastingTime(CNtlSobActor *pActor);
RwReal		Logic_GetMaxCastingTime(CNtlSobActor *pActor);
void		Logic_SetSkillReqFlag(RwBool bFlag);
void		Logic_UpdateSkillResTimeOut(RwReal fElapsed);

float		Logic_GetRange(RwV3d& vPos, RwV3d& vTargetPos);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Active, HTB Skill Time Notifier

RwReal		Logic_GetCastingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetCoolingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetKeepingTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetDOTValueFactor(CNtlSobActor* pActor);
RwReal		Logic_GetDOTTimeFactor(CNtlSobActor* pActor);
RwReal		Logic_GetRequiredEPFactor(CNtlSobActor* pActor);
//RwReal		Logic_GetCastingTimeModifier(CNtlSobActor* pActor);
//RwReal		Logic_GetCoolingTimeModifier(CNtlSobActor* pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor의 상태 및 mode

RwBool		Logic_IsCommunityActor(CNtlSobActor *pActor);           // actor가 community actor인가?(npc, trigger object, world item)
RwBool		Logic_IsGambleActor(CNtlSobActor *pActor);
RwBool		Logic_IsOperationTriggerActor(CNtlSobActor *pActor);    // operation trigger actor 인가?
RwUInt32	Logic_GetDirectIdTriggerActor(CNtlSobActor *pActor);
RwUInt32	Logic_GetActorStateId(CNtlSobActor *pActor);            // actor의 state id를 넘겨준다.
RwUInt32    Logic_GetActorStateFlags(CNtlSobActor* pActor);          // Actor의 현재 State Flag를 넘겨준다.
SERIAL_HANDLE Logic_GetActorTargetSerialId(CNtlSobActor *pActor);   // actor가 target 하고 있는 serial id
RwBool		Logic_IsActorBattleMode(CNtlSobActor *pActor);          // actor 현재 전투중인가?
RwBool		Logic_CanSkillUse(CNtlSobActor *pActor);                // actor가 skill 사용이 가능한가?
RwBool		Logic_IsNotFollowMoveSkill(CNtlSobActor *pActor, RwUInt8 bySlotIdx);
void		Logic_GetAvatarSkillApplyArea(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, std::list<SERIAL_HANDLE>& listApplyActor);      // area skill
void		Logic_GetAvatarSkillApplyArea(SERIAL_HANDLE hTargetSerial, RwUInt32 uiSkillId, std::list<SERIAL_HANDLE>& listApplyActor);       // area skill 
RwBool		Logic_IsEnemyTargetFromActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);        // target이 actor에 대하여 적인가?
RwBool		Logic_IsEnemyTargetFromAvatarActor(SERIAL_HANDLE hTargetSerial);        // target이 avatar에 대하여 적인가?
RwBool		Logic_IsEnemyTagetFromPetActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);      // target이 pet actor에 대하여 적인가?
RwBool      Logic_isEnemyTargetOcclusion(SERIAL_HANDLE hTargetSerial);  ///< 타겟이 차폐물에 가려져 있는가?
CNtlSobActor* Logic_FindNearEnemyFromAvatarActor(void);             // avatar에 대하여 가장 가까운 적을 찾는다.
CNtlSobActor* Logic_GetAutoTarget(CNtlSobActor* pActor);            ///< 자동 타겟팅 상대를 찾는다.
RwBool      Logic_IsBlackList(RwUInt32 uiSerialID);                 // 상대 캐릭터가 나의 차단리스트에 있는지 확인한다.
RwBool      Logic_IsDirectMode(CNtlSobActor* pActor);               ///< Direct Mode(제어 불능)인지 확인한다.
RwBool      Logic_CanTranslateIdleState(CNtlSobActor* pActor);      ///< Idle 상태로 전이가 가능한가? 
RwBool      Logic_CanTargetted(CNtlSobActor* pActor);               ///< 타겟팅이 가능한가?
RwBool      Logic_CanAttacked(CNtlSobActor* pActor);		//< Check if target can be attacked
void        Logic_SetTargetMarkingID(RwUInt32 uiSerialId);          ///< 타겟 마킹된 ID 체크
RwUInt32    Logic_GetTargetMarkingID();
void		Logic_GetSkillFinalLocation(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, RwV3d* vFinalLoc, RwV3d* vFinalSubjectLoc);
void		Logic_GetSkillFinalLocation2(SERIAL_HANDLE hTargetSerial, sSKILL_TBLDAT *pSkillTblData, RwV3d * vFinalLoc, RwV3d * vFinalSubjectLoc);

void        Logic_SetActorAnimation(RwUInt32 uiSerialID, RwUInt32 uiAnimID, RwBool bLoop);             ///<  애니메이션을 세팅한다.

RwBool		Logic_IsMovePatternApply(CNtlSobActor* pActor);
void		Logic_GetMovePatternSync(RwUInt32 uiSerialID, RwUInt8 byPatternId);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ground fly height
RwBool		Logic_IsGroundFlyActor(CNtlSobActor *pActor);

RwReal		Logic_GetGroundFlyHeight(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hit Mark

const RwChar* Logic_GetVisualHitMark(CNtlSobActor *pActor, EAttackType eAttackType, RwBool bCritical, RwBool bPowerEffect);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NPC

void		Logic_MakeMobBrief(sMOB_BRIEF *pBrief, TBLIDX tblidx);

void		Logic_MakeNPCBrief(sNPC_BRIEF *pBrief, TBLIDX tblidx);

void		Logic_MakeMobSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir);

void		Logic_MakeNPCSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir);

///< NPC Serial 번호로 테이블 데이터를 반환한다.
sNPC_TBLDAT* Logic_GetNPCDataFromSob(SERIAL_HANDLE hSerial);

TBLIDX		Logic_GetNPCTriggerId(CNtlSob *pSobObj);

// 현재 quest npc가 떨어져 
RwReal		Logic_GetQuestNpcDistanceFromAvatar(CNtlSob *pSobNpc);

RwBool		Logic_IsDragonBallNPC(TBLIDX tblidx);

RwBool		Logic_IsDragonBallNPCActor(SERIAL_HANDLE hSerialId);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trigger object

// quest 및 trigger agency

RwBool		Logic_IsTriggerActive(CNtlSob *pSobObj);

TBLIDX		Logic_GetTriggerObjectId(CNtlSob *pSobObj);

RwBool		Logic_IsNormalQuest( RwUInt16 tID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Object

TBLIDX		Logic_GetDynamicObjTriggerId(CNtlSob* pSobObj);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item

void			Logic_SetItemStack(CNtlSobItem* pSobItem, RwUInt8 ucStackNum);

// weapon equip slot에 장착된 item table을 구한다.
sITEM_TBLDAT*	Logic_GetEquipedWeaponItemTableData(CNtlSob *pSobObj);

// 해당하는 equip slot에 장착된 item serial을 구한다.
SERIAL_HANDLE	Logic_GetEquipedItemSerial(CNtlSob *pSobObj, RwUInt8 bySlotIdx);

// item table이 staff weapon인가?
RwBool			Logic_IsEquipedStaffWeapon(sITEM_TBLDAT *pItemTblData);

RwUInt32        Logic_GetIdleAnimationID(CNtlSob* pSobObj);     ///< 무기 장착 상태에 맞는 IDLE 애니메이션을 반환한다.

// sub weapon equip slot에 장착된 item serial을 구한다.
SERIAL_HANDLE	Logic_GetEquipedSubWeaponItemSerial(CNtlSob *pSobObj);

// 움직일 수 있는 sub weapon 이냐?
RwBool			Logic_IsMoveableSubWeapon(sITEM_TBLDAT *pItemTblData); 

// 움직일 수 있는 sub weapon 이냐?
RwBool			Logic_IsMoveableSubWeapon(TBLIDX tblidx);

// sub weapon이 active 상태에서 character에 attach 되지 않아야 하는가?
RwBool			Logic_IsActiveNotAttachSubWeapon(TBLIDX tblidx);

// main weapon과 change해야 하는 sub weapon인가?
RwBool			Logic_IsWeaponChangeSubWeapon(TBLIDX tblidx);

//Getting item grade -Makoto
RwUInt8         Logic_GetBootsGrade(CNtlSob* pSobObj);

// costume item 인가?
RwBool			Logic_IsCostumeItem(sITEM_TBLDAT *pItemTblData);

// 현재의 아이템이 hide 되어야 하는가?
RwBool			Logic_IsHideCostumeItem(RwUInt16 wHideBigFlags, void *pData);

// With items that can be sold
RwBool			Logic_IsCanSellItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSellItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSellItem( sITEM_PROFILE* pItemProfile );

// Check if item can be discarded
RwBool			Logic_IsCanDiscardItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanDiscardItem(CNtlSobItemAttr* pSobItemAttr);
RwBool			Logic_IsCanDiscardItem(sITEM_PROFILE* pItemProfile);

// 유저간 거래할 수 있는 아이템인가
RwBool			Logic_IsCanUserTradeItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanUserTradeItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanUserTradeItem( sITEM_PROFILE* pItemProfile );

RwBool			Logic_IsCanUserAuctionItem(CNtlSobItemAttr* pSobItemAttr);

// 창고에 저장할 수 있는 아이템인가
RwBool			Logic_IsCanSaveWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveWarehouse( sITEM_PROFILE* pItemProfile );

// 공유창고에 저장할 수 있는 아이템인가
RwBool			Logic_IsCanSaveCommonWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveCommonWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveCommonWarehouse( sITEM_PROFILE* pItemProfile );

// 사용기간 아이템인가
RwBool			Logic_IsTimeLimitItem( CNtlSobItemAttr* pItemAttr );
RwBool			Logic_IsTimeLimitItem( sITEM_PROFILE* pItemProfile );
RwBool			Logic_IsTimeLimitItem( sITEM_TBLDAT* pITEM_TBLDAT );



// 모든 가방의 비어있는 슬롯의 갯수를 반환한다
RwInt32			Logic_GetCountEmptySlotofBag();

// 첫 번째 가방부터 비어있는 첫 번째 슬롯을 찾아서 반환한다
void			Logic_GetFirstEmptySlot_of_Bag(RwUInt32& uiBagIndex, SERIAL_HANDLE& hBagHandle, RwUInt32& uiSlot_of_bag);

// 첫 번째 창고부터 비어있는 첫 번째 슬롯을 찾아서 반환한다
void			Logic_GetFirstEmptySlot_of_Warehouse(RwUInt8& byWarehouseIndex, SERIAL_HANDLE& hWarehouseHandle,
													 RwUInt8& bySlot_of_warehouse);

// 인자로 받은 핸들의 아이템이 창고에 있다면 인덱스와 슬롯 번호를 반환한다
RwBool			Logic_FindWarehouseItem(SERIAL_HANDLE hItem, RwUInt8& byWarehouseIndex, RwUInt8& bySlot_of_warehouse);

// 첫 번째 길드 창고부터 비어있는 첫 번째 슬롯을 찾아서 반환한다
void			Logic_GetFirstEmptySlot_of_GuildWarehouse(RwUInt8& byGuildWarehouseIndex, RwUInt8& bySlot_of_GuildWarehouse);

// 비어있는 스카우터의 첫 번째 슬롯
RwUInt8			Logic_GetFirstEmptyScouterSlot();

// 인자로 받은 아이템 인덱스와 같은 가방안의 첫번째 아이템의 핸들을 반환한다
SERIAL_HANDLE	Logic_FirstItem_in_Bag(TBLIDX index);


///< Charm Serial 번호로 테이블 데이터를 반환한다.
sCHARM_TBLDAT*	Logic_GetCharmDataFromTable(TBLIDX index);

///< MERCHANT Serial 번호로 테이블 데이터를 반환한다.
sMERCHANT_TBLDAT* Logic_GetMerchantDataFromTable(TBLIDX index);

///< Item Serial 혹은 Sob 번호로 테이블 데이터를 반환한다.
sITEM_TBLDAT*	Logic_GetItemDataFromTable(TBLIDX index);
sITEM_TBLDAT*	Logic_GetItemDataFromSob(SERIAL_HANDLE hSerial);

///< Item Serial로 SobItemAttr을 반환한다.
CNtlSobItemAttr* Logic_GetItemSobAttrFromSobSerial( SERIAL_HANDLE hSerial );

// 해당하는 스카우터 파츠타입을 장착했는지 검사한다.
bool			Logic_ExistScouterEffect(int nEffectCode);

bool			Logic_IsValidScouterEquipped(bool bShowByDefault);


RwReal			Logic_GetItemSystemEffectValue(TBLIDX idxItem, RwUInt32 uiSystemEffect);
RwReal			Logic_GetItemSystemEffectValue(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiSystemEffect);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Skill

sSKILL_TBLDAT*	Logic_GetSkillDataFromTable(TBLIDX index);
RwBool          Logic_IsSkillSystemEffectCode(TBLIDX index, RwUInt32 uiSystemEffect);               ///< 스킬의 시스템 이펙트 코드가 있는지 확인한다.
RwReal          Logic_GetSkillSystemEffectValue(TBLIDX index, RwUInt32 uiSystemEffect);             ///< 스킬의 시스템 이펙트 값을 반환한다.
void			Logic_SetSkillExp(RwUInt32 uiSkillSlotIdx, RwUInt16 wExp);
void			Logic_SetActiveRpSkillBonus(RwUInt8 byRpBonusType);
RwUInt8			Logic_GetActiveRpSkillBonus(void);
RwBool          Logic_IsCastingTargetSkill(TBLIDX tblidx);                      ///< 캐스팅 타겟을 표시하는 스킬인가 (메가도동파)
RwBool			Logic_IsStrongCastSkill(TBLIDX tblidx);							///< 스트롱 캐스팅 스킬인가
RwBool			Logic_IsSkillUseAfterAttack(SERIAL_HANDLE hSkillSerial);

void			Logic_SkillInfoText(WCHAR* wchMsg, unsigned int uiColor, BYTE byMsgType);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Target

// Avatar serial

CNtlSobActor*	Logic_GetAvatarActor(void);

SERIAL_HANDLE	Logic_GetAvatarHandle(void);

SERIAL_HANDLE	Logic_GetAvatarTargetHandle(void);

bool			Logic_SobTarget(RwUInt32 hTargetSerial, BYTE byFollow);

const WCHAR*	Logic_GetAvatarName(void);

// target actor와의 heading을 구하는 함수.
void			Logic_CharHeading(CNtlSobActor *pActor, SERIAL_HANDLE uiTargetSeriaIId);

// target이 현재 죽어 있는가?
RwBool			Logic_CharIsDie(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarTargetStateChange(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarStateChange(void);

RwBool          Logic_IsBehind(CNtlSob* pSob, RwV3d& vTargetPos);   /// 뒤에 있는가를 판별한다.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor math logic

RwBool			Logic_InFollowRange(CNtlSobActor *pActor, CNtlSobActor *pTarActor, RwReal fFollowRange);

RwBool			Logic_InFollowRange(CNtlSobActor *pActor, const RwV3d& v3TargetPos, RwReal fFollowRange);

RwBool			Logic_CanActorCommunity(CNtlSobActor* pActor, CNtlSobActor* pTarActor, RwReal fFollowRange);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor world presenation coordinate

RwBool			Logic_GetWorldHeight(const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal = NULL);

RwBool			Logic_GetWorldHeight(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal = NULL);

void			Logic_GetWorldSlop(CNtlSobActor *pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold = 30.f, RwReal fDownThreshold = 30.f);

void			Logic_GetWorldSlopForVehicle(CNtlSobVehicle* pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold = 30.f, RwReal fDownThreshold = 30.f);

RwReal			Logic_GetSwimmingOffset(CNtlSobActor *pActor);

RwReal			Logic_GetSwimmingHeight(CNtlSobActor *pActor, const RwV3d *pPos);

RwBool			Logic_IsSwimmingActor(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff);

RwBool			Logic_IsSwimming(CNtlSobActor *pActor, const RwV3d *pPos, RwReal fWorldHeight, RwReal fWaterHeight);

const RwV3d*	Logic_GetCameraPos(const RwCamera *pCamera);

const RwV3d*	Logic_GetCameraAt(const RwCamera *pCamera);

const RwV3d*	Logic_GetCameraUp(const RwCamera *pCamera);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// character collision 

void			Logic_EnableCharacterCollisionSlant(RwBool bSlantCheck);
void			Logic_EnableCharacterSlipping(RwBool bSlippingCheck);
RwUInt8			Logic_CharacterCollisionEx(CNtlSobActor *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump = FALSE);
RwBool			Logic_CharacterSlipping(CNtlSobActor *pActor, RwV3d& vNewPos, RwReal fElapsed);
RwBool			Logic_IsMoveWorldArea(CNtlSobActor *pActor, RwV3d vCurrPos, RwV3d vDestPos);                    ///< 지형속성을 조사해서 이동 가능 영역인지 판단한다
RwBool          Logic_IsCollisionLine(RwV3d& vStartPos, RwV3d& vEndPos, OUT CNtlPLEntity* pColEntity = NULL);                                        ///< 두 지점 사이에 오브젝트 충돌 여부를 체크한다.

RwUInt8			Logic_VehicleCollisionEx(CNtlSobVehicle *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump = FALSE);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor

CNtlSobPlayer*	Logic_GetPlayer_from_CharID(CHARACTERID charID);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commuity

// 자신이 파티의 리더인지 여부
RwBool			Logic_I_am_PartyLeader();

// 자신과 같은 파티원인지 여부
RwBool			Logic_IsMyPartyMember(SERIAL_HANDLE hSerial);

RwBool			Logic_CanPartyLeave();

RwBool			Logic_CanPartyInvite();

// 자신이 길드장인지 여부
RwBool			Logic_I_am_GuildLeader();

// 자신이 부길드장인지 여부
RwBool			Logic_I_am_SecondGuildLeader();

// 파티 공유 타겟에 포함되어 있는지 확인한다. 포함되어 있다면 slot 번호를, 포함되어 있지않다면 -1을 반환한다.
RwInt32         Logic_isPartyShareTarget(SERIAL_HANDLE hSerial);
RwUInt32        Logic_GetShareTargetSerial(RwInt32 nSlot);
RwInt32         Logic_GetShareTargetBlankSlot();

sDOJO_INFO*		Logic_GetDojoInfo(SERIAL_HANDLE hNPC);

eDBO_TEAM		Logic_GetScrambleTeam(eTOBJECT_STATE_TYPE_C eTObjectState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world information

RwUInt8			Logic_GetActiveWorldRuleType(void);

bool			Logic_GetActiveWorldAllowFlight();

sWORLD_INFO*	Logic_GetActiveWorldInfo(void);

HOBJECT			Logic_GetCurrentWorldTriggerOffsetHandle(void);

RwUInt32		Logic_GetActiveWorldId(void);

RwUInt32		Logic_GetActiveWorldTableId(void);

const RwChar*	Logic_GetActiveWorldName(void);

// Entity 상태에 따라 월드상 Entity 위의 이름 색상 지정
void			Logic_SetHeadNameColor(CNtlSob* pSob);

void			Logic_SetNickNameColor(CNtlSob* pSob);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// privateshop
// 상점을 열수 있는가?
RwBool			Logic_IsCanCreatePrivateShop();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TMQ
RwBool			Logic_IsTMQBoarding(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
RwBool			Logic_IsCoolTimeUpdate(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 연출 처리 함수.

RwBool			Logic_ProcKnockdownMatrixDirection(CNtlSob *pSobObj, RwUInt32 uiMatrixSerialId, const SHitStuff *pHitStuff);
RwBool          Logic_IsCinematic();            ///< 현재 시네마틱 연출중인지 유무를 반환한다.


// 클릭 가능 유무 설정
void            Logic_SetClickDisable(CNtlSobActor* pActor, RwBool bDisable);
RwBool          Logic_IsClickDisable();

// Auto Run
void			Logic_SetAutoRun(RwBool bAutoRun);

//////////////////////////////////////////////////////////////////////////
// 버스및 탈것 관련 로직
RwBool          Logic_IsBus(CNtlSobActor* pSobObj);             /// 해당 오브젝트가 버스인지 확인한다.
RwBool			Logic_IsBus(SERIAL_HANDLE hHandle);

RwBool          Logic_IsVehicleDriver(CNtlSobActor* pSobObj);   /// 해당 액터가 탈것의 운전자인지 확인한다.
RwBool			Logic_IsVehicleDriver(SERIAL_HANDLE hHandle);

// TRUE		: 탑승 가능 또는 해당 아이템이 Vehicle 아이템이 아님
// FALSE	: 탑승 불가
RwBool			Logic_CanRideOnVehicle(SERIAL_HANDLE hHandle, CNtlSobItem* pSobItem);

TBLIDX			Logic_GetRegisterFuelItemTableIndex();

bool			Logic_IsAirMode(CNtlSobActor* pSobObj); //is flying

//////////////////////////////////////////////////////////////////////////
// 플래그
void			Logic_SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
void			Logic_UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool			Logic_IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

//////////////////////////////////////////////////////////////////////////
// 변신 관련 로직
RwBool          Logic_IsTransform(CNtlSobActor* pSobActor);         ///< 변신을 했는지 확인한다.
RwUInt32        Logic_IsTransformSkill(RwUInt32 uiSkillTblId);      ///< 스킬이 변신 스킬인지 확인한다.
RwBool          Logic_IsTransformGreatNamek(CNtlSobActor* pSobActor);    ///< 거대 나멕인지 확인한다.         (팀장님 요청에 따라 따로 만듬 by agebreak)
RwBool          Logic_IsTransformSpinAttack(CNtlSobActor* pSobActor);   ///< 회전 공격 중인지 확인한다.      (회전 공격은 서버에서 변신상태)
RwBool          Logic_IsTransformRollingAttack(CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// 튜토리얼
RwBool			Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE byType);
RwBool			Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE byType);
RwBool			Logic_CanAttack_in_Tutorial(ETL_ATTACK_TYPE byType);

void			Logic_SendTutorialCondition(ETL_CONDITION_TYPE byType, RwUInt32 uiParam1 = 0xffffffff );

//
BYTE			 Logic_GetServerState(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// 컨디션 처리 관련
RwBool      Logic_IsCondition(CNtlSobActor* pActor, RwUInt64 uiConditon);         ///< 해당 컨디션이 있는지 확인한다.
RwBool      Logic_RandomMove(CNtlSobActor* pActor, RwReal fDistance);             ///< 랜덤으로 액터를 이동시킨다.


void		Logic_ShowHTBRPSelectGui(CNtlSobActor* pActor, SERIAL_HANDLE hTargetSerial);
void		Logic_ShowSandBagRPSelectGUI(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// Apply 함수
bool		Logic_ApplyNtlStorageUnit( CNtlStorageUnit* pUnit, unsigned int uiFlags );


//////////////////////////////////////////////////////////////////////////
// PC방 관련 함수
void        Logic_SetNetPy(DWORD dwNetPy);
DWORD       Logic_GetNetPy();

//////////////////////////////////////////////////////////////////////////
// Sob 객체 비주얼 관련

// 캐릭터의 실제 보여지는 도복의 색상을 바꾸거나 색상을 얻어온다.
// SobPlayer 객체의 도복 색상은 SobPlayer의 함수로 직접 바꾸어야 한다
RwUInt8		Logic_GetSobPlayerDogiColor(RwUInt32 hSobPlayerHandle);
RwUInt8		Logic_GetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwBool bUICharacter = FALSE);
RwBool		Logic_SetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwUInt8 byColorIndex, RwBool bUICharacter = FALSE, RwBool bForce = FALSE);

RwBool		Logic_GetEmblemFactor(sEmblemFactor* pOutEmblemFactor, CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// 수량 / 인덱스 관련

// 저장을 할 수 있는 인덱스인지 검사
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

// Invalid값을 포함하여 저장을 할 수 있는 인덱스인지 검사
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

// 실제로 참조를 하기위해 쓸 수 있는 인덱스인지 검사
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

//////////////////////////////////////////////////////////////////////////
// Camera controller
RwBool Logic_IsNPCCameraMode(void);

void	Logic_CancelNpcFacing();


//////////////////////////////////////////////////////////////////////////
// 사운드 관련

SOUND_HANDLE	Logic_PlayGUISound(char* pcFileName);

// 일반 슬롯에서 아이템을 집거나 놓았을 때의 소리를 낸다
void			Logic_PlayItemPickUpSound(sITEM_TBLDAT* pITEM_TBLDAT);
void			Logic_PlayItemPutDownSound(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt8 byDestPlace = 0);

//
bool			Logic_CanRequestTrade();
bool			Logic_CanRequestFreeBattle();


#endif
