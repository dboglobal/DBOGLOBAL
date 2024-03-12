/*****************************************************************************
 *
 * File			: NtlSLLogic.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
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

extern RwReal   g_fTestVal;             /// �پ��� �׽�Ʈ�� ���Ǵ� ���� ���� 
extern RwReal   g_fTestVal2;
extern RwReal   g_fTestVal3;

extern RwInt32	g_nTestVal;

extern RwInt32	g_nBroadCastGuiPosX;

extern RwUInt32 g_uiTargetMarkingId;    ///< ���� Ÿ�� ������ ��ü�� ID

extern RwBool	g_bActiveAnimSync;

extern RwUInt32	g_uiLimitMemory;

extern RwUInt32	g_uiCreateHitEffectRate;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �Լ� ������ ���
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
// ���� : �ӵ��� table data�� ���� �� ������ ����� �ӵ��� �ɰ��̴�.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor �Ӽ� logic

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
// player ���� 
RwUInt8		Logic_GetPlayerRace(CNtlSobActor *pActor);

RwUInt8		Logic_GetPlayerClass(CNtlSobActor *pActor);

RwUInt8     Logic_GetPlayerGender(CNtlSobActor *pActor);

void		Logic_SetReputation(CNtlSobActor *pActor, RwUInt32 uiReputation);

RwUInt32	Logic_GetReputation(CNtlSobActor *pActor);

RwUInt8		Logic_ConverClasstoRace(RwUInt8 byClass);

RwBool		Logic_IsChild(CNtlSobActor *pActor);

RwBool		Logic_IsFirstClass(CNtlSobPlayer* pPlayer);	// 1�����̳�.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attack , npc community , skill use range

RwReal		Logic_GetRadius(CNtlSobActor *pActor);
RwReal		Logic_GetAttackRange(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);
RwReal		Logic_GetCommunityRange(CNtlSobActor *pActor);
RwReal		Logic_GetCommunityOutRange(CNtlSobActor *pActor);
void		Logic_BeginActorCommunity(CNtlSobActor *pActor, CNtlSobActor *pTarActor); 
RwReal		Logic_GetSkillUseRange(sSKILL_TBLDAT *pSkillTblData);
RwReal      Logic_GetSkillApplyRange(CNtlSobActor* pActor, RwUInt32 uiSkillTblidx);         /// ���� ��ų�� ������ ��ȯ�Ѵ�.
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
// actor�� ���� �� mode

RwBool		Logic_IsCommunityActor(CNtlSobActor *pActor);           // actor�� community actor�ΰ�?(npc, trigger object, world item)
RwBool		Logic_IsGambleActor(CNtlSobActor *pActor);
RwBool		Logic_IsOperationTriggerActor(CNtlSobActor *pActor);    // operation trigger actor �ΰ�?
RwUInt32	Logic_GetDirectIdTriggerActor(CNtlSobActor *pActor);
RwUInt32	Logic_GetActorStateId(CNtlSobActor *pActor);            // actor�� state id�� �Ѱ��ش�.
RwUInt32    Logic_GetActorStateFlags(CNtlSobActor* pActor);          // Actor�� ���� State Flag�� �Ѱ��ش�.
SERIAL_HANDLE Logic_GetActorTargetSerialId(CNtlSobActor *pActor);   // actor�� target �ϰ� �ִ� serial id
RwBool		Logic_IsActorBattleMode(CNtlSobActor *pActor);          // actor ���� �������ΰ�?
RwBool		Logic_CanSkillUse(CNtlSobActor *pActor);                // actor�� skill ����� �����Ѱ�?
RwBool		Logic_IsNotFollowMoveSkill(CNtlSobActor *pActor, RwUInt8 bySlotIdx);
void		Logic_GetAvatarSkillApplyArea(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, std::list<SERIAL_HANDLE>& listApplyActor);      // area skill
void		Logic_GetAvatarSkillApplyArea(SERIAL_HANDLE hTargetSerial, RwUInt32 uiSkillId, std::list<SERIAL_HANDLE>& listApplyActor);       // area skill 
RwBool		Logic_IsEnemyTargetFromActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);        // target�� actor�� ���Ͽ� ���ΰ�?
RwBool		Logic_IsEnemyTargetFromAvatarActor(SERIAL_HANDLE hTargetSerial);        // target�� avatar�� ���Ͽ� ���ΰ�?
RwBool		Logic_IsEnemyTagetFromPetActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial);      // target�� pet actor�� ���Ͽ� ���ΰ�?
RwBool      Logic_isEnemyTargetOcclusion(SERIAL_HANDLE hTargetSerial);  ///< Ÿ���� ���󹰿� ������ �ִ°�?
CNtlSobActor* Logic_FindNearEnemyFromAvatarActor(void);             // avatar�� ���Ͽ� ���� ����� ���� ã�´�.
CNtlSobActor* Logic_GetAutoTarget(CNtlSobActor* pActor);            ///< �ڵ� Ÿ���� ��븦 ã�´�.
RwBool      Logic_IsBlackList(RwUInt32 uiSerialID);                 // ��� ĳ���Ͱ� ���� ���ܸ���Ʈ�� �ִ��� Ȯ���Ѵ�.
RwBool      Logic_IsDirectMode(CNtlSobActor* pActor);               ///< Direct Mode(���� �Ҵ�)���� Ȯ���Ѵ�.
RwBool      Logic_CanTranslateIdleState(CNtlSobActor* pActor);      ///< Idle ���·� ���̰� �����Ѱ�? 
RwBool      Logic_CanTargetted(CNtlSobActor* pActor);               ///< Ÿ������ �����Ѱ�?
RwBool      Logic_CanAttacked(CNtlSobActor* pActor);		//< Check if target can be attacked
void        Logic_SetTargetMarkingID(RwUInt32 uiSerialId);          ///< Ÿ�� ��ŷ�� ID üũ
RwUInt32    Logic_GetTargetMarkingID();
void		Logic_GetSkillFinalLocation(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, RwV3d* vFinalLoc, RwV3d* vFinalSubjectLoc);
void		Logic_GetSkillFinalLocation2(SERIAL_HANDLE hTargetSerial, sSKILL_TBLDAT *pSkillTblData, RwV3d * vFinalLoc, RwV3d * vFinalSubjectLoc);

void        Logic_SetActorAnimation(RwUInt32 uiSerialID, RwUInt32 uiAnimID, RwBool bLoop);             ///<  �ִϸ��̼��� �����Ѵ�.

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

///< NPC Serial ��ȣ�� ���̺� �����͸� ��ȯ�Ѵ�.
sNPC_TBLDAT* Logic_GetNPCDataFromSob(SERIAL_HANDLE hSerial);

TBLIDX		Logic_GetNPCTriggerId(CNtlSob *pSobObj);

// ���� quest npc�� ������ 
RwReal		Logic_GetQuestNpcDistanceFromAvatar(CNtlSob *pSobNpc);

RwBool		Logic_IsDragonBallNPC(TBLIDX tblidx);

RwBool		Logic_IsDragonBallNPCActor(SERIAL_HANDLE hSerialId);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trigger object

// quest �� trigger agency

RwBool		Logic_IsTriggerActive(CNtlSob *pSobObj);

TBLIDX		Logic_GetTriggerObjectId(CNtlSob *pSobObj);

RwBool		Logic_IsNormalQuest( RwUInt16 tID );

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Object

TBLIDX		Logic_GetDynamicObjTriggerId(CNtlSob* pSobObj);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item

void			Logic_SetItemStack(CNtlSobItem* pSobItem, RwUInt8 ucStackNum);

// weapon equip slot�� ������ item table�� ���Ѵ�.
sITEM_TBLDAT*	Logic_GetEquipedWeaponItemTableData(CNtlSob *pSobObj);

// �ش��ϴ� equip slot�� ������ item serial�� ���Ѵ�.
SERIAL_HANDLE	Logic_GetEquipedItemSerial(CNtlSob *pSobObj, RwUInt8 bySlotIdx);

// item table�� staff weapon�ΰ�?
RwBool			Logic_IsEquipedStaffWeapon(sITEM_TBLDAT *pItemTblData);

RwUInt32        Logic_GetIdleAnimationID(CNtlSob* pSobObj);     ///< ���� ���� ���¿� �´� IDLE �ִϸ��̼��� ��ȯ�Ѵ�.

// sub weapon equip slot�� ������ item serial�� ���Ѵ�.
SERIAL_HANDLE	Logic_GetEquipedSubWeaponItemSerial(CNtlSob *pSobObj);

// ������ �� �ִ� sub weapon �̳�?
RwBool			Logic_IsMoveableSubWeapon(sITEM_TBLDAT *pItemTblData); 

// ������ �� �ִ� sub weapon �̳�?
RwBool			Logic_IsMoveableSubWeapon(TBLIDX tblidx);

// sub weapon�� active ���¿��� character�� attach ���� �ʾƾ� �ϴ°�?
RwBool			Logic_IsActiveNotAttachSubWeapon(TBLIDX tblidx);

// main weapon�� change�ؾ� �ϴ� sub weapon�ΰ�?
RwBool			Logic_IsWeaponChangeSubWeapon(TBLIDX tblidx);

//Getting item grade -Makoto
RwUInt8         Logic_GetBootsGrade(CNtlSob* pSobObj);

// costume item �ΰ�?
RwBool			Logic_IsCostumeItem(sITEM_TBLDAT *pItemTblData);

// ������ �������� hide �Ǿ�� �ϴ°�?
RwBool			Logic_IsHideCostumeItem(RwUInt16 wHideBigFlags, void *pData);

// With items that can be sold
RwBool			Logic_IsCanSellItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSellItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSellItem( sITEM_PROFILE* pItemProfile );

// Check if item can be discarded
RwBool			Logic_IsCanDiscardItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanDiscardItem(CNtlSobItemAttr* pSobItemAttr);
RwBool			Logic_IsCanDiscardItem(sITEM_PROFILE* pItemProfile);

// ������ �ŷ��� �� �ִ� �������ΰ�
RwBool			Logic_IsCanUserTradeItem(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanUserTradeItem( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanUserTradeItem( sITEM_PROFILE* pItemProfile );

RwBool			Logic_IsCanUserAuctionItem(CNtlSobItemAttr* pSobItemAttr);

// â�� ������ �� �ִ� �������ΰ�
RwBool			Logic_IsCanSaveWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveWarehouse( sITEM_PROFILE* pItemProfile );

// ����â�� ������ �� �ִ� �������ΰ�
RwBool			Logic_IsCanSaveCommonWarehouse(sITEM_TBLDAT* pITEM_TBLDAT);
RwBool			Logic_IsCanSaveCommonWarehouse( CNtlSobItemAttr* pSobItemAttr );
RwBool			Logic_IsCanSaveCommonWarehouse( sITEM_PROFILE* pItemProfile );

// ���Ⱓ �������ΰ�
RwBool			Logic_IsTimeLimitItem( CNtlSobItemAttr* pItemAttr );
RwBool			Logic_IsTimeLimitItem( sITEM_PROFILE* pItemProfile );
RwBool			Logic_IsTimeLimitItem( sITEM_TBLDAT* pITEM_TBLDAT );



// ��� ������ ����ִ� ������ ������ ��ȯ�Ѵ�
RwInt32			Logic_GetCountEmptySlotofBag();

// ù ��° ������� ����ִ� ù ��° ������ ã�Ƽ� ��ȯ�Ѵ�
void			Logic_GetFirstEmptySlot_of_Bag(RwUInt32& uiBagIndex, SERIAL_HANDLE& hBagHandle, RwUInt32& uiSlot_of_bag);

// ù ��° â����� ����ִ� ù ��° ������ ã�Ƽ� ��ȯ�Ѵ�
void			Logic_GetFirstEmptySlot_of_Warehouse(RwUInt8& byWarehouseIndex, SERIAL_HANDLE& hWarehouseHandle,
													 RwUInt8& bySlot_of_warehouse);

// ���ڷ� ���� �ڵ��� �������� â�� �ִٸ� �ε����� ���� ��ȣ�� ��ȯ�Ѵ�
RwBool			Logic_FindWarehouseItem(SERIAL_HANDLE hItem, RwUInt8& byWarehouseIndex, RwUInt8& bySlot_of_warehouse);

// ù ��° ��� â����� ����ִ� ù ��° ������ ã�Ƽ� ��ȯ�Ѵ�
void			Logic_GetFirstEmptySlot_of_GuildWarehouse(RwUInt8& byGuildWarehouseIndex, RwUInt8& bySlot_of_GuildWarehouse);

// ����ִ� ��ī������ ù ��° ����
RwUInt8			Logic_GetFirstEmptyScouterSlot();

// ���ڷ� ���� ������ �ε����� ���� ������� ù��° �������� �ڵ��� ��ȯ�Ѵ�
SERIAL_HANDLE	Logic_FirstItem_in_Bag(TBLIDX index);


///< Charm Serial ��ȣ�� ���̺� �����͸� ��ȯ�Ѵ�.
sCHARM_TBLDAT*	Logic_GetCharmDataFromTable(TBLIDX index);

///< MERCHANT Serial ��ȣ�� ���̺� �����͸� ��ȯ�Ѵ�.
sMERCHANT_TBLDAT* Logic_GetMerchantDataFromTable(TBLIDX index);

///< Item Serial Ȥ�� Sob ��ȣ�� ���̺� �����͸� ��ȯ�Ѵ�.
sITEM_TBLDAT*	Logic_GetItemDataFromTable(TBLIDX index);
sITEM_TBLDAT*	Logic_GetItemDataFromSob(SERIAL_HANDLE hSerial);

///< Item Serial�� SobItemAttr�� ��ȯ�Ѵ�.
CNtlSobItemAttr* Logic_GetItemSobAttrFromSobSerial( SERIAL_HANDLE hSerial );

// �ش��ϴ� ��ī���� ����Ÿ���� �����ߴ��� �˻��Ѵ�.
bool			Logic_ExistScouterEffect(int nEffectCode);

bool			Logic_IsValidScouterEquipped(bool bShowByDefault);


RwReal			Logic_GetItemSystemEffectValue(TBLIDX idxItem, RwUInt32 uiSystemEffect);
RwReal			Logic_GetItemSystemEffectValue(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiSystemEffect);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Skill

sSKILL_TBLDAT*	Logic_GetSkillDataFromTable(TBLIDX index);
RwBool          Logic_IsSkillSystemEffectCode(TBLIDX index, RwUInt32 uiSystemEffect);               ///< ��ų�� �ý��� ����Ʈ �ڵ尡 �ִ��� Ȯ���Ѵ�.
RwReal          Logic_GetSkillSystemEffectValue(TBLIDX index, RwUInt32 uiSystemEffect);             ///< ��ų�� �ý��� ����Ʈ ���� ��ȯ�Ѵ�.
void			Logic_SetSkillExp(RwUInt32 uiSkillSlotIdx, RwUInt16 wExp);
void			Logic_SetActiveRpSkillBonus(RwUInt8 byRpBonusType);
RwUInt8			Logic_GetActiveRpSkillBonus(void);
RwBool          Logic_IsCastingTargetSkill(TBLIDX tblidx);                      ///< ĳ���� Ÿ���� ǥ���ϴ� ��ų�ΰ� (�ް�������)
RwBool			Logic_IsStrongCastSkill(TBLIDX tblidx);							///< ��Ʈ�� ĳ���� ��ų�ΰ�
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

// target actor���� heading�� ���ϴ� �Լ�.
void			Logic_CharHeading(CNtlSobActor *pActor, SERIAL_HANDLE uiTargetSeriaIId);

// target�� ���� �׾� �ִ°�?
RwBool			Logic_CharIsDie(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarTargetStateChange(SERIAL_HANDLE uiTargetSeriaIId);

void			Logic_AvatarStateChange(void);

RwBool          Logic_IsBehind(CNtlSob* pSob, RwV3d& vTargetPos);   /// �ڿ� �ִ°��� �Ǻ��Ѵ�.


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
RwBool			Logic_IsMoveWorldArea(CNtlSobActor *pActor, RwV3d vCurrPos, RwV3d vDestPos);                    ///< �����Ӽ��� �����ؼ� �̵� ���� �������� �Ǵ��Ѵ�
RwBool          Logic_IsCollisionLine(RwV3d& vStartPos, RwV3d& vEndPos, OUT CNtlPLEntity* pColEntity = NULL);                                        ///< �� ���� ���̿� ������Ʈ �浹 ���θ� üũ�Ѵ�.

RwUInt8			Logic_VehicleCollisionEx(CNtlSobVehicle *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump = FALSE);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor

CNtlSobPlayer*	Logic_GetPlayer_from_CharID(CHARACTERID charID);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Commuity

// �ڽ��� ��Ƽ�� �������� ����
RwBool			Logic_I_am_PartyLeader();

// �ڽŰ� ���� ��Ƽ������ ����
RwBool			Logic_IsMyPartyMember(SERIAL_HANDLE hSerial);

RwBool			Logic_CanPartyLeave();

RwBool			Logic_CanPartyInvite();

// �ڽ��� ��������� ����
RwBool			Logic_I_am_GuildLeader();

// �ڽ��� �α�������� ����
RwBool			Logic_I_am_SecondGuildLeader();

// ��Ƽ ���� Ÿ�ٿ� ���ԵǾ� �ִ��� Ȯ���Ѵ�. ���ԵǾ� �ִٸ� slot ��ȣ��, ���ԵǾ� �����ʴٸ� -1�� ��ȯ�Ѵ�.
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

// Entity ���¿� ���� ����� Entity ���� �̸� ���� ����
void			Logic_SetHeadNameColor(CNtlSob* pSob);

void			Logic_SetNickNameColor(CNtlSob* pSob);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// privateshop
// ������ ���� �ִ°�?
RwBool			Logic_IsCanCreatePrivateShop();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TMQ
RwBool			Logic_IsTMQBoarding(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
RwBool			Logic_IsCoolTimeUpdate(CNtlSobActor *pActor);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� ó�� �Լ�.

RwBool			Logic_ProcKnockdownMatrixDirection(CNtlSob *pSobObj, RwUInt32 uiMatrixSerialId, const SHitStuff *pHitStuff);
RwBool          Logic_IsCinematic();            ///< ���� �ó׸�ƽ ���������� ������ ��ȯ�Ѵ�.


// Ŭ�� ���� ���� ����
void            Logic_SetClickDisable(CNtlSobActor* pActor, RwBool bDisable);
RwBool          Logic_IsClickDisable();

// Auto Run
void			Logic_SetAutoRun(RwBool bAutoRun);

//////////////////////////////////////////////////////////////////////////
// ������ Ż�� ���� ����
RwBool          Logic_IsBus(CNtlSobActor* pSobObj);             /// �ش� ������Ʈ�� �������� Ȯ���Ѵ�.
RwBool			Logic_IsBus(SERIAL_HANDLE hHandle);

RwBool          Logic_IsVehicleDriver(CNtlSobActor* pSobObj);   /// �ش� ���Ͱ� Ż���� ���������� Ȯ���Ѵ�.
RwBool			Logic_IsVehicleDriver(SERIAL_HANDLE hHandle);

// TRUE		: ž�� ���� �Ǵ� �ش� �������� Vehicle �������� �ƴ�
// FALSE	: ž�� �Ұ�
RwBool			Logic_CanRideOnVehicle(SERIAL_HANDLE hHandle, CNtlSobItem* pSobItem);

TBLIDX			Logic_GetRegisterFuelItemTableIndex();

bool			Logic_IsAirMode(CNtlSobActor* pSobObj); //is flying

//////////////////////////////////////////////////////////////////////////
// �÷���
void			Logic_SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
void			Logic_UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool			Logic_IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

//////////////////////////////////////////////////////////////////////////
// ���� ���� ����
RwBool          Logic_IsTransform(CNtlSobActor* pSobActor);         ///< ������ �ߴ��� Ȯ���Ѵ�.
RwUInt32        Logic_IsTransformSkill(RwUInt32 uiSkillTblId);      ///< ��ų�� ���� ��ų���� Ȯ���Ѵ�.
RwBool          Logic_IsTransformGreatNamek(CNtlSobActor* pSobActor);    ///< �Ŵ� �������� Ȯ���Ѵ�.         (����� ��û�� ���� ���� ���� by agebreak)
RwBool          Logic_IsTransformSpinAttack(CNtlSobActor* pSobActor);   ///< ȸ�� ���� ������ Ȯ���Ѵ�.      (ȸ�� ������ �������� ���Ż���)
RwBool          Logic_IsTransformRollingAttack(CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// Ʃ�丮��
RwBool			Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE byType);
RwBool			Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE byType);
RwBool			Logic_CanAttack_in_Tutorial(ETL_ATTACK_TYPE byType);

void			Logic_SendTutorialCondition(ETL_CONDITION_TYPE byType, RwUInt32 uiParam1 = 0xffffffff );

//
BYTE			 Logic_GetServerState(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// ����� ó�� ����
RwBool      Logic_IsCondition(CNtlSobActor* pActor, RwUInt64 uiConditon);         ///< �ش� ������� �ִ��� Ȯ���Ѵ�.
RwBool      Logic_RandomMove(CNtlSobActor* pActor, RwReal fDistance);             ///< �������� ���͸� �̵���Ų��.


void		Logic_ShowHTBRPSelectGui(CNtlSobActor* pActor, SERIAL_HANDLE hTargetSerial);
void		Logic_ShowSandBagRPSelectGUI(CNtlSobActor* pActor);

//////////////////////////////////////////////////////////////////////////
// Apply �Լ�
bool		Logic_ApplyNtlStorageUnit( CNtlStorageUnit* pUnit, unsigned int uiFlags );


//////////////////////////////////////////////////////////////////////////
// PC�� ���� �Լ�
void        Logic_SetNetPy(DWORD dwNetPy);
DWORD       Logic_GetNetPy();

//////////////////////////////////////////////////////////////////////////
// Sob ��ü ���־� ����

// ĳ������ ���� �������� ������ ������ �ٲٰų� ������ ���´�.
// SobPlayer ��ü�� ���� ������ SobPlayer�� �Լ��� ���� �ٲپ�� �Ѵ�
RwUInt8		Logic_GetSobPlayerDogiColor(RwUInt32 hSobPlayerHandle);
RwUInt8		Logic_GetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwBool bUICharacter = FALSE);
RwBool		Logic_SetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwUInt8 byColorIndex, RwBool bUICharacter = FALSE, RwBool bForce = FALSE);

RwBool		Logic_GetEmblemFactor(sEmblemFactor* pOutEmblemFactor, CNtlSobActor* pSobActor);

//////////////////////////////////////////////////////////////////////////
// ���� / �ε��� ����

// ������ �� �� �ִ� �ε������� �˻�
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);

// Invalid���� �����Ͽ� ������ �� �� �ִ� �ε������� �˻�
RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

// ������ ������ �ϱ����� �� �� �ִ� �ε������� �˻�
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex);
RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex);

//////////////////////////////////////////////////////////////////////////
// Camera controller
RwBool Logic_IsNPCCameraMode(void);

void	Logic_CancelNpcFacing();


//////////////////////////////////////////////////////////////////////////
// ���� ����

SOUND_HANDLE	Logic_PlayGUISound(char* pcFileName);

// �Ϲ� ���Կ��� �������� ���ų� ������ ���� �Ҹ��� ����
void			Logic_PlayItemPickUpSound(sITEM_TBLDAT* pITEM_TBLDAT);
void			Logic_PlayItemPutDownSound(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt8 byDestPlace = 0);

//
bool			Logic_CanRequestTrade();
bool			Logic_CanRequestFreeBattle();


#endif
