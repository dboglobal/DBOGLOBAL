/*****************************************************************************
*
* File			: NtlSLEvent.h
* Author		: HyungSuk, Jang
* Copyright	: (��)NTL
* Date			: 2005. 9. 01	
* Abstract		: Simulation layer event type
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_SLEVENT_H__
#define __NTL_SLEVENT_H__

#pragma warning(disable:4819) // add by peessi


#include "ceventhandler.h"
#include "NtlSLServerDeclear.h"
#include "NtlBehaviorData.h"
#include "NtlTriggerDirectSync.h"
#include "NtlQuest.h"
#include "NtlItem.h"
#include "NtlSkill.h"
#include "NtlMail.h"
#include "NtlFriend.h"
#include "NtlCharacter.h"
#include "DboTSCoreDefine.h"
#include "NtlWorldConcept.h"
#include "DboTLDefine.h"
#include "NtlDirection.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"
#include "NtlObject.h"
#include "NtlRankBattle.h"

//////////////////////////////////////////////
/**	
* game system message
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventSysMsg;
extern RWS::CEventId g_EventSysStringMsg;
extern RWS::CEventId g_EventFormatSysMsg;
extern RWS::CEventId g_EventFormatSysStringMsg;

//////////////////////////////////////////////
/**
* network
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventNetSendError;		// network

extern RWS::CEventId g_EventNotifySendPacket;			// packet�� �������߻��ϴ� �̺�Ʈ(PacketTrace��)
extern RWS::CEventId g_EventNotifyReceivePacket;		// packet�� �������߻��ϴ� �̺�Ʈ(PacketTrace��)

//////////////////////////////////////////////
/**
* world change
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventWorldChange;		
extern RWS::CEventId g_EventCreateWorld;                 // ���尡 Destroy�Ǵ��� Create�ɶ�

//////////////////////////////////////////////
/**
* avatar/camera �̵� �� ȸ��
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCameraSmoothingMove;		// camera smoothing move.
extern RWS::CEventId g_EventCameraShake;				// ���� camera.
extern RWS::CEventId g_EventCameraDash;					// dash camera.
extern RWS::CEventId g_EventCameraNpc;					// npc click ���� camera.
extern RWS::CEventId g_EventCameraDB;					// dragon ball ���� camera.
extern RWS::CEventId g_EventCameraDTClear;				// camera direct node clear.
extern RWS::CEventId g_EventCameraDTSplineNode;			// camera direct spline node.
extern RWS::CEventId g_EventCameraFree;					// Free Camera
extern RWS::CEventId g_EventCameraTimeMachine;			// Ÿ�Ӹӽ� ��� ���� ī�޶� �̺�Ʈ
extern RWS::CEventId g_EventCameraExplosion;			// ���� ī�޶� �̺�Ʈ
extern RWS::CEventId g_EventCameraTimeMachineArrive;	// Ÿ�̸ӽ� ���� ī�޶� �̺�Ʈ
extern RWS::CEventId g_EventCameraKnockDownMatrix;		// knockdown matrix camera ����.
extern RWS::CEventId g_EventCameraFPS;                  // 1��Ī ī�޶�
extern RWS::CEventId g_EventCameraBus;					// ���� ī�޶�
extern RWS::CEventId g_EventCameraObserver;             // õ������ ����ȸ ������ ī�޶�
extern RWS::CEventId g_EventCameraControlDelete;		// ī�޶� ����

//////////////////////////////////////////////
/**
* action mapping
*/

//////////////////////////////////////////////

extern RWS::CEventId g_EventActionMapHalt;				// action halt action mapping
extern RWS::CEventId g_EventActionMapJump;				// jump action mapping
extern RWS::CEventId g_EventActionMapAirJump;			// air jump action mapping
extern RWS::CEventId g_EventActionMapSitAndStand;		// sit and stand action mapping
extern RWS::CEventId g_EventActionMapSkillUse;			// skill ���.
extern RWS::CEventId g_EventActionMapHTBUse;			// htb ���.
extern RWS::CEventId g_EventActionMapItemUse;			// item ���.
extern RWS::CEventId g_EventActionMapSocialUse;			// social action ���.
extern RWS::CEventId g_EventActionMapPrivateShopOpen;	// private shop Open;
extern RWS::CEventId g_EventActionMapCharging;			// charging ���.
extern RWS::CEventId g_EventActionMapBlocking;			// blocking ���.
extern RWS::CEventId g_EventActionMapAutoFollow;		// auto follow ���.
extern RWS::CEventId g_EventActionMapDice;				// �׼� �ֻ��� ���
extern RWS::CEventId g_EventScreenShot;                 // ��ũ������ �����Ŀ� �߻��ϴ� �̺�Ʈ

extern RWS::CEventId g_EventSkillSelectMode;

// �׼� ��ų �������� �̿��� �׼� (AvatarController���� �ѹ� �ɷ����� �̺�Ʈ�� ������)
extern RWS::CEventId g_EventActionFuncBasicAttack;      ///< �⺻ ���� ������
extern RWS::CEventId g_EventActionFuncAssist;            ///< ��ý�Ʈ ������
extern RWS::CEventId g_EventActionFuncGetItem;          ///< ������ ���� ������
extern RWS::CEventId g_EventActionFuncSitAndStand;      ///< �ɱ� ������


extern RWS::CEventId g_EventESCinNarrationMode;

//////////////////////////////////////////////
/** 
*simulation object event
*/
//////////////////////////////////////////////

//--------------------
// simulation �⺻ event
//--------------------

extern RWS::CEventId g_EventSobShowNfy;

//--------------------
// input�� ���� event
//--------------------

// mouse�� terrain�� click ���� ��� �߻��Ǵ� event�̴�.
extern RWS::CEventId g_EventTerrainClick;		

// avatar�� keyboard �̵���ų�� �߻��ϴ� event.
extern RWS::CEventId g_EventKeyboardMove;		

// avatar�� keyboard �̵��� ���� �� �߻��ϴ� event.
extern RWS::CEventId g_EventKeyboardMoveStop;	

// avatar�� mouse dash move �̵�.
extern RWS::CEventId g_EventMouseDashMove;

// avatar�� keyboard dash move �̵�.
extern RWS::CEventId g_EventKeyboardDashMove;

extern RWS::CEventId g_EventAirDashMove;
extern RWS::CEventId g_EventAirAccelMove;

// avatar�� heading�� �ٲܶ� �߻��ϴ� event.
extern RWS::CEventId g_EventInputChangeHeading;

// mouse�� simulation object�� click ���� ��� �߻��Ǵ� event�̴�.
extern RWS::CEventId g_EventSobTargetSelect;     

// mouse�� simulation object�� click ���� event�̴�.
extern RWS::CEventId g_EventSobTargetSelectRelease;     

// mouse�� simulation object�� attack mark release ���� event�̴�.
extern RWS::CEventId g_EventSobAttackMarkRelease;

// mouse�� simulation object�� double click ���� ��� �߻��Ǵ� event�̴�.
extern RWS::CEventId g_EventSobAttackSelect;     

extern RWS::CEventId g_EventSobTargetMarkSelect;                    ///< Ÿ�� ��ŷ�� ������Ʈ�� �����Ҷ� (�ް� ������ ��ų) 

extern RWS::CEventId g_EventSobTargetMarkRelease;                   ///< Ÿ�� ��ŷ�� ������Ʈ�� ������

// mouse�� simulation object�� mouse focus�� ������ ���.
extern RWS::CEventId g_EventSobGotFocus;     

// mouse�� simulation object�� mouse focus�� ���� �Ǿ��� ���.
extern RWS::CEventId g_EventSobLostFocus;     

extern RWS::CEventId g_EventCommuTargetSelect;      // Ŀ�´�Ƽ Target�� �����Ҷ��� �̺�Ʈ
extern RWS::CEventId g_EventCommuTargetRelease;     // Ŀ�´�Ƽ Target�� �����Ҷ��� �̺�Ʈ


/// �������� �� ��ġë���� �߻��ϴ� �̺�Ʈ
extern RWS::CEventId g_EventBotCaution_Nfy;

/// ���� ���� ���ῡ�� ������ û�Ҷ� �߻��ϴ� �̺�Ʈ
extern RWS::CEventId g_EventBotHelpMe_Nfy;

/// ���� Ÿ�� ��ũ�� �����ϴ� �̺�Ʈ
extern RWS::CEventId g_EventShareTargetSelect;

// ���� Ÿ�� ��ũ�� ���ִ� �̺�Ʈ
extern RWS::CEventId g_EventShareTargetRelease;

extern RWS::CEventId g_EventTenkaichiMarking;

extern RWS::CEventId g_EventSobEventTitleEffect;

extern RWS::CEventId g_EventAvatarTargetStateChange;

extern RWS::CEventId g_EventAvatarReset;

//-----------------------------
// create/delete�� ���� �Լ�.
//-----------------------------

// simulatin object ���� event.
extern RWS::CEventId g_EventSobCreate;

// simulation object �Ҹ� event.
extern RWS::CEventId g_EventSobDelete;

// 
extern RWS::CEventId g_EventSobSummonPetSpawnSync;

//--------------------------------
// �� ������ ���õ� �̺�Ʈ
//--------------------------------
extern RWS::CEventId g_EventSobPetSkillInfo;	// �� ��ų�� ���� ����

//-----------------------------
// server ���� ��ü�� �̵��� ���� �Լ�.
//-----------------------------
extern RWS::CEventId g_EventSobMove;				// simulation object move event
extern RWS::CEventId g_EventSobMoveSync;            // ������ ����ȭ �̺�Ʈ
extern RWS::CEventId g_EventSobSecondDestMove;		// simulation object second dest move event
extern RWS::CEventId g_EventSobSplineMove;			// simulation object spline move event
extern RWS::CEventId g_EventSobAdjustMovePosition;  // simulation object adjust move position event
extern RWS::CEventId g_EventSobDash;				// simulation object dash event
extern RWS::CEventId g_EventSobMoveStop;			// simulation object move stop event
extern RWS::CEventId g_EventSobDirectionFloat;		// simulation object direction float
extern RWS::CEventId g_EventSobFollowMove;			// simulation object follow move
extern RWS::CEventId g_EventSobAdjustMove;			// simulation object adjust move event
extern RWS::CEventId g_EventSobPushingNfy;			///< ȸ�� ������ �޾Ƽ� �����̵� �Ǵ� �̺�Ʈ
extern RWS::CEventId g_EventSobGuardCrush;			// simulation object guard crush�� ���Ѵ�.
extern RWS::CEventId g_EventSobMovePatternSync;		// ���� monster�� ����ϰ� �ֽ�.
extern RWS::CEventId g_EventSobAirJump;
extern RWS::CEventId g_EventSobAirAccel;

//-----------------------------
// item ������ ������ ���� event.
//-----------------------------
extern RWS::CEventId g_EventSobCreateItem;

// simulation item add event
extern RWS::CEventId g_EventSobItemAdd;

// simulation item delete event
extern RWS::CEventId g_EventSobItemDelete;

// simulation avater item move event
extern RWS::CEventId g_EventSobItemMove;

// simulation avatar item update event
extern RWS::CEventId g_EventSobItemUpdate;

// simulation avater item stack move event
extern RWS::CEventId g_EventSobItemStackMove;

// simulation avatar item durability update
extern RWS::CEventId g_EventSobItemDurUpdate;

// simulation player equipment model change
extern RWS::CEventId g_EventSobEquipChange;

// simulation avatar equip item durability down
extern RWS::CEventId g_EventSobEquipItemDurDown;

// simulation avatar item use action event
extern RWS::CEventId g_EventSobItemUseAction;

// simulation avatar item Upgrade action event
extern RWS::CEventId g_EventSobItemUpgrade;

//
extern RWS::CEventId g_EventSobItemChangeOptionSet;

// 
extern RWS::CEventId g_EventSobItemChangeBattleAttribute;

// simulation avatar item Upgrade All ( by cheat )
extern RWS::CEventId g_EventSobEquippedItemUpgradeAll;

// simulation avatar item cancel event
extern RWS::CEventId g_EventSobItemCancel;

// ItemUseRes �� ���з� ������� �� �ߵ��Ǵ� �̺�Ʈ
extern RWS::CEventId g_EventSobItemUseResFailed;

// simulation avatar item casting event
extern RWS::CEventId g_EventSobItemCasting;

// simulation avatar item in out check time event. ex) avatar create, item add, item delete
extern RWS::CEventId g_EventSobCheckItemInOut;

// update item restriction
extern RWS::CEventId g_EventSobItemRestrictionUpdate;

extern RWS::CEventId g_EventItemSocketInsertBead;
extern RWS::CEventId g_EventItemSocketDestroyBead;

//////////////////////////////////////////////////////////////////////////
// Dynamic Object ���� �̺�Ʈ
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventDynamicObjectUpdateState;
extern RWS::CEventId g_EventHoipoiMixRecipeRegNfy;
extern RWS::CEventId g_EventHoipoiMixItemRecipeInfo;	///< ������ ������ ����
extern RWS::CEventId g_EventHoipoiMixItemMakeExpNfy;	///< ������ ����� ����ġ ���� �˸�
extern RWS::CEventId g_EventHoipoiMixInfoUpdate;			///< ȣ�����̹ͽ� ���� ������Ʈ


// ���� �̺�Ʈ
extern RWS::CEventId g_EventTransform;
extern RWS::CEventId g_EventMobTransform;               ///< ���� �����Ҷ��� �̺�Ʈ
extern RWS::CEventId g_EventTransformCandy;             ///< �������� �����ϴ� �̺�Ʈ
extern RWS::CEventId g_EventSobTransformSequela;        ///< ���� ������

// ����� �̺�Ʈ
extern RWS::CEventId g_EventAfterEffectRemainTimeNfy;
extern RWS::CEventId g_EventSobCondTerror;
extern RWS::CEventId g_EventSobCondConfused;
extern RWS::CEventId g_EventSobCondHindingKi;

extern RWS::CEventId g_EventCinematicScale;

//-----------------------------
// Party Select
//-----------------------------
extern RWS::CEventId g_EventPartySelectStateInit;
extern RWS::CEventId g_EventPartySelectState;
//-----------------------------
// CC Battle Dungeon
//-----------------------------
extern RWS::CEventId g_EventBattleDungeonStateUpdate;
extern RWS::CEventId g_EventBattleDungeonStageClear;
extern RWS::CEventId g_EventBattleDungeonLimitTime;
extern RWS::CEventId g_EventBattleDungeonFail;
extern RWS::CEventId g_EventBattleDungeonRewardFinish;



//-----------------------------
// QuickSlot�� ���õ� event
//-----------------------------
extern RWS::CEventId g_EventSobDeleteQuickSlotIcon;

//-----------------------------
// Warehouse
//-----------------------------

extern RWS::CEventId g_EventSobWarehouseUpdate;

extern RWS::CEventId g_EventSobWarehouseItemCreate;

extern RWS::CEventId g_EventSobWarehouseItemAdd;

extern RWS::CEventId g_EventSobWarehouseItemDelete;

extern RWS::CEventId g_EventSobWarehouseItemMove;

extern RWS::CEventId g_EventSobWarehouseItemStackMove;

//-----------------------------
// quest item ������ ������ ���� event.
//-----------------------------

extern RWS::CEventId g_EventSobQuestItemAdd;

extern RWS::CEventId g_EventSobQuestItemDelete;

extern RWS::CEventId g_EventSobQuestItemMove;

extern RWS::CEventId g_EventSobQuestItemUpdate;


//-----------------------------
// server ���⿡ ���� action�鿡 ���� event.
//-----------------------------

// simulation object �� knockdown���� stand�� ���� �� ������ �̺�Ʈ.
extern RWS::CEventId g_EventSobStanding;

// simulation object fighting mode
extern RWS::CEventId g_EventSobFightingMode;

// simulation object attack event(target�� ������ ���۽�Ű�� �̺�Ʈ)
extern RWS::CEventId g_EventSobAttack;

extern RWS::CEventId g_EventSobSpecialAttack;

extern RWS::CEventId g_EventSobBattleNotification;

// simulation object attacked event(���ݽ����� �ݴ�� ���ݴ������� �ǹ� -> g_EventSobAttack�� �ݴ� �̺�Ʈ)
extern RWS::CEventId g_EventSobAttacked;

// simulation object attack event(target�� ������ ���۽�Ű�� �̺�Ʈ)
extern RWS::CEventId g_EventSobPetBeginAttack;

// simulation object hit event(attack�� ���۵��� target���� damage�� ������ �ִ� event)
extern RWS::CEventId g_EventSobHit;

// simulation object attack follow event
extern RWS::CEventId g_EventSobAttackFollow;

// simulation object attack mode event
extern RWS::CEventId g_EventSobAttackMode;

extern RWS::CEventId g_EventSobSendAttackEnd;           ///< ���ø�� ���Ḧ ������ �˸��� �̺�Ʈ

// simulation object jump event
extern RWS::CEventId g_EventSobJump;

// simulation object jump event
extern RWS::CEventId g_EventSobJumpDirection;

// simulation object jump end event
extern RWS::CEventId g_EventSobJumpEnd;

// simulation object sit event
extern RWS::CEventId g_EventSobSit;

// simulation object private shop event
extern RWS::CEventId g_EventSobPrivateShop;

// simulation player convert class
extern RWS::CEventId g_EventSobConvertClass;            ///< ���� �̺�Ʈ
extern RWS::CEventId g_EventSobChangeAdult;             ///< ���� �ٲٴ� �̺�Ʈ (���� <-> ���)

extern RWS::CEventId g_EventCharTitleSelectNfy;

extern RWS::CEventId g_EventEnableAir;
extern RWS::CEventId g_EventSetAp;

// simulation object direct play
extern RWS::CEventId g_EventSobDirectPlay;

extern RWS::CEventId g_EventSobAnimPlay;

extern RWS::CEventId g_EventSobProxyGUIUpdate;


//-----------------------------
// cooling event.
//-----------------------------

extern RWS::CEventId g_EventSobCooling;

//-----------------------------
// skill event.
//-----------------------------

// simulaion object skill cancel event
extern RWS::CEventId g_EventSobSkillCancel;

// simulaion object skill affectinn cancel event
extern RWS::CEventId g_EventSobSkillAffectingCancel;

// simulation object skill casting event
extern RWS::CEventId g_EventSobSkillCasting;

// simulation object skill act event
extern RWS::CEventId g_EventSobSkillAction;

// simulation object skill acted event    g_EventSobSkillActiond�� �ݴ�
extern RWS::CEventId g_EventSobSkillActioned;

// simulation object skill add
extern RWS::CEventId g_EventSobSkillAdd;

// simulation object skill update
extern RWS::CEventId g_EventSobSkillUpgrade;

// simulation object HTBSkill Add
extern RWS::CEventId g_EventSobHTBSkillAdd;

// simulation object HTBSkill Action
extern RWS::CEventId g_EventSobHTBSkillAction;

// simulation object Social Action
extern RWS::CEventId g_EventSobSocialAction;

// simulation object Social Action Cooling
extern RWS::CEventId g_EventSobSocialActionCooling;

// simulation object skill rpbonus setup result
extern RWS::CEventId g_EventRpBonusSetupRes;

// skill, htb init
extern RWS::CEventId g_EventSkillInit;

// passive effect applying
extern RWS::CEventId g_EventSetSkillPassiveEffect;

extern RWS::CEventId g_EventSkillInfoText;

//-----------------------------
// buff event.
//-----------------------------

// simulation object buff add
extern RWS::CEventId g_EventSobBuffAdd;

// simulation object buff drop
extern RWS::CEventId g_EventSobBuffDrop;

// simulation object buff refresh all
extern RWS::CEventId g_EventSobBuffRefreshAll;

// simulation object buff activation
extern RWS::CEventId g_EventSobBuffActivate;

extern RWS::CEventId g_EventSobBuffAutoRecover;

extern RWS::CEventId g_EventSobFakeBuffAdd;

extern RWS::CEventId g_EventSobFakeBuffDrop;

extern RWS::CEventId g_EventSobFakeBuffRefreshAll;

extern RWS::CEventId g_EventSobFakeBuffActivate;

//-----------------------------
// state event
//_____________________________

// simulation state transition event
extern RWS::CEventId g_EventSobStateTransition;
extern RWS::CEventId g_EventSobServerUpdateState;
extern RWS::CEventId g_EventSobServerUpdateCondition;
extern RWS::CEventId g_EventSobServerUpdateAspect;
extern RWS::CEventId g_EventSobFainting;
extern RWS::CEventId g_EventSobUpdateLPStatusNfy;           ///< Sob�� ��� ���¿� ���������� �̺�Ʈ

//-----------------------------
// party event
//_____________________________

extern RWS::CEventId g_EventPartyUpdate;

extern RWS::CEventId g_EventPartyCreate;

extern RWS::CEventId g_EventPartyLeave;

extern RWS::CEventId g_EventPartyMemberAdd;

extern RWS::CEventId g_EventPartyMemberDel;

extern RWS::CEventId g_EventPartyLeaderChange;

extern RWS::CEventId g_EventPartyUpdateValue;

extern RWS::CEventId g_EventPartyMemberLocation;

extern RWS::CEventId g_EventPartyShareTargetRes;            ///< ���� Ÿ�� ���ÿ� ���� ���� ����
extern RWS::CEventId g_EventPartyShareTargetNfy;            ///< ���� Ÿ�� ���ÿ� ���� ���� �˸�

//-----------------------------
// Guild event
//_____________________________

extern RWS::CEventId g_EventSLGuild;
extern RWS::CEventId g_EventNotifyGuild;
extern RWS::CEventId g_EventChangeGuildName;
extern RWS::CEventId g_EventChangeGuildEmblem;

extern RWS::CEventId g_EventGuildWarehouse;
extern RWS::CEventId g_EventGuildWarehouseNotify;

extern RWS::CEventId g_EventGuildWarehouseItemInfo;
extern RWS::CEventId g_EventGuildWarehouseItemMove;
extern RWS::CEventId g_EventGuildWarehouseItemStackMove;

//-----------------------------
// Dojo event
//_____________________________

extern RWS::CEventId g_EventDojo;
extern RWS::CEventId g_EventDojoNotify;
extern RWS::CEventId g_EventDojoState;
extern RWS::CEventId g_EventDojoStateNotify;

extern RWS::CEventId g_EventScrambleDefaultInfo;
extern RWS::CEventId g_EventScrambleDefaultInfoNotify;

//-----------------------------
// HTB RP
//-----------------------------

extern RWS::CEventId g_EventHTBRPStock;						// HTB RP Stock ���� GUI
extern RWS::CEventId g_EventHTBRPResult;	


//-----------------------------
// Friend List Event
//_____________________________
extern RWS::CEventId g_EventFriendAdd_Res;
extern RWS::CEventId g_EventFriendDel_Res;
extern RWS::CEventId g_EventFriendMove_Res;
extern RWS::CEventId g_EventFriendList_Info;
extern RWS::CEventId g_EventBlackListAdd_Res;
extern RWS::CEventId g_EventBlackListDel_Res;
extern RWS::CEventId g_EventFriendInfo;
extern RWS::CEventId g_EventFriendInfoChange;
extern RWS::CEventId g_EventFriendInfoNotify;           ///< UI�� ������Ʈ �ϱ� ���� �̺�Ʈ (������->UI)


//-----------------------------
// OtherParam Event
//_____________________________

extern RWS::CEventId g_EventHelpHint;
extern RWS::CEventId g_EventRegisterQuestPosMark;
extern RWS::CEventId g_EventUnregisterQuestPosMark;
extern RWS::CEventId g_EventShowQuestPosMark;
extern RWS::CEventId g_EventBind;
extern RWS::CEventId g_EventBindNotify;

//-----------------------------
// PrivateShop Event
//_____________________________

extern RWS::CEventId g_EventPrivateShopState;
extern RWS::CEventId g_EventPrivateShopStateVisitor;
extern RWS::CEventId g_EventPrivateShopItem;
extern RWS::CEventId g_EventPrivateShopItemDataInfo;
extern RWS::CEventId g_EventPrivateShopItemSelect;
extern RWS::CEventId g_EventPrivateShopItemBuying;
extern RWS::CEventId g_EventPrivateShopItemState;
extern RWS::CEventId g_EventPrivateShopNameTag;

//-----------------------------
// TMQ
//_____________________________
extern RWS::CEventId g_EventTMQEnterRes;
extern RWS::CEventId g_EventTMQ;
extern RWS::CEventId g_EventTMQNotify;
extern RWS::CEventId g_EventDirectionNfy;

//-----------------------------
// UD
//_____________________________
extern RWS::CEventId g_EventUDEnterRes;

//-----------------------------
// Broad message
//_____________________________

extern RWS::CEventId g_EventBroadMsgBegNfy;
extern RWS::CEventId g_EventBroadMsgEndNfy;

//-----------------------------
// RankBattle
//_____________________________

extern RWS::CEventId g_EventRBBoardInfo;		// ��ũ��Ʋ �Խ��� ����
extern RWS::CEventId g_EventRBJoin;				// ��ũ��Ʋ ��û����.

//extern RWS::CEventId g_EventRBBoardUpdate;	// ��ũ��Ʋ �Խ��� ����
//extern RWS::CEventId g_EventRBRoomInfo;		// ���� �� ����
//extern RWS::CEventId g_EventRBRoom;			// ��ũ��Ʋ ��û ���
//extern RWS::CEventId g_EventRBMatch;			// ��ũ��Ʋ �غ� ���� ���

extern RWS::CEventId g_EventRBBattleStateUpdate;	// ��ũ��Ʋ ����� ��������
extern RWS::CEventId g_EventRBBattleTeamInfo;		// �� ���� �˸�
extern RWS::CEventId g_EventRBBattlePlayerState;	// �÷��̾� ����
extern RWS::CEventId g_EventRBBattleKillScoreUpdate;// ų ���ھ� ������Ʈ
extern RWS::CEventId g_EventRBBattleStageFinish;	// �������� ��ħ
extern RWS::CEventId g_EventRBBattleMatchFinish;	// ��� ��ħ
extern RWS::CEventId g_EventRBBattleEnd;			// RBBattle ���� ������ ����
extern RWS::CEventId g_EventRBForcedEnd;			// �����ʿ����� ��ũ��Ʋ ��������

extern RWS::CEventId g_EventRBTotalScoreUpdate;		// ���� ���� ������Ʈ

//-----------------------------
// MailSystem
//_____________________________
extern RWS::CEventId g_EventMailStart;
extern RWS::CEventId g_EventMailLoadInfo;
extern RWS::CEventId g_EventMailLoadData;
extern RWS::CEventId g_EventMailSend;
extern RWS::CEventId g_EventMailRead;
extern RWS::CEventId g_EventMailDel;
extern RWS::CEventId g_EventMailReturn;
extern RWS::CEventId g_EventMailItemReceive;
extern RWS::CEventId g_EventMailReload;
extern RWS::CEventId g_EventMailReloadNfy;
extern RWS::CEventId g_EventMailLoad;
extern RWS::CEventId g_EventMailLock; 
extern RWS::CEventId g_EventMailCloseNfy;
extern RWS::CEventId g_EventMailSystemNfy;
extern RWS::CEventId g_EventMailMultiDelRes;

//-----------------------------
// Char
//_____________________________
extern RWS::CEventId g_EventCharAway;


//-----------------------------
// Auction House
//_____________________________
extern RWS::CEventId g_EventAuctionHouseListRes;
extern RWS::CEventId g_EventAuctionHouseListData;
extern RWS::CEventId g_EventAuctionHouseSellRes;
extern RWS::CEventId g_EventAuctionHouseSellCancelRes;
extern RWS::CEventId g_EventAuctionHouseBuyRes;
//-----------------------------
// ActionMap
//_____________________________
extern RWS::CEventId g_EventActionMapLoadInfo;
extern RWS::CEventId g_EventActionMapUpdateRes;
extern RWS::CEventId g_EventActionMapClientNotify;

//_____________________________
// Portal System
//_____________________________
extern RWS::CEventId g_EventPortalInfoRes;          ///< ��Ż ������ ������
extern RWS::CEventId g_EventPortalAddReq;
extern RWS::CEventId g_EventPortalAddRes;           ///< ��Ż�� ��� �߰����� ���� ����
extern RWS::CEventId g_EventPortalRes;              ///< ��Ż�� �̵���û ���� ���� ����

//_____________________________
// Popo Icon String Notify
//_____________________________
extern RWS::CEventId g_EventPopoNotify;

//_____________________________
// ���� �� Ż�� ���� �̺�Ʈ
//_____________________________
extern RWS::CEventId g_EventSobOnBus;
extern RWS::CEventId g_EventSobOnVehicle;

extern RWS::CEventId g_EventSobVehicleStart;
extern RWS::CEventId g_EventSobRideOnOffVehicle;
extern RWS::CEventId g_EventSobVehicleEngine;
extern RWS::CEventId g_EventSobVehicleStunt;

extern RWS::CEventId g_EventBusMove;

//////////////////////////////////////////////////////////////////////////
// Client �� TS ���� ���� �޽�����
//////////////////////////////////////////////////////////////////////////

// TS => Client

extern RWS::CEventId g_EventReloadTS;

extern RWS::CEventId g_EventRegQuest_Nfy;

extern RWS::CEventId g_EventUnregQuest_Nfy;

extern RWS::CEventId g_EventShowQuestWindow;

extern RWS::CEventId g_EventShowQuestWindow_Nfy;

extern RWS::CEventId g_EventUpdateQuestState_Nfy;

extern RWS::CEventId g_EventUpdateQuestProgressInfo_Nfy;

extern RWS::CEventId g_EventGiveUpQuest;

extern RWS::CEventId g_EventShowIndicator;

extern RWS::CEventId g_EventHideIndicator;

extern RWS::CEventId g_EventShowIndicator_Nfy;

extern RWS::CEventId g_EventDoQuest;

extern RWS::CEventId g_EventQuestProposalDialog_Req;
extern RWS::CEventId g_EventQuestProposalDialog_Res;

extern RWS::CEventId g_EventQuestUserSelectDialog_Req;
extern RWS::CEventId g_EventQuestUserSelectDialog_Res;

extern RWS::CEventId g_EventQuestRewardDialog_Req;
extern RWS::CEventId g_EventQuestRewardDialog_Res;

extern RWS::CEventId g_EventQuestNarrationDialog_Req;
extern RWS::CEventId g_EventQuestNarrationDialog_Res;

extern RWS::CEventId g_EventQuestNPCDialog_Req;
extern RWS::CEventId g_EventQuestNPCDialog_Res;

extern RWS::CEventId g_EventQuestShare_Nfy;

extern RWS::CEventId g_EventQuestAcceptProposal_Nfy;
extern RWS::CEventId g_EventQuestAcceptReward_Nfy;
extern RWS::CEventId g_EventQuestAcceptGiveUp_Nfy;

extern RWS::CEventId g_EventQuestObjDialog_Req;
extern RWS::CEventId g_EventQuestObjDialog_Res;

extern RWS::CEventId g_EventShowTutorialNfy;		// Ʃ�丮�� ���� �˸� �̺�Ʈ

extern RWS::CEventId g_EventTSRemovingTMQQuest_Nfy;	// TMQ ����Ʈ ���� �˸�

extern RWS::CEventId g_EventTSPCDialog_Nfy;

extern RWS::CEventId g_EventTSMudosaTeleport_Req;	// ������ �ڷ���Ʈ

extern RWS::CEventId g_EventTSMudosaTeleport_Res;

extern RWS::CEventId g_EventQuestNarrationUserSelectDialog_Req;

extern RWS::CEventId g_EventQuestNarrationUserSelectDialog_Res;

extern RWS::CEventId g_EventTSFinished;

extern RWS::CEventId g_EventTSSkipContainer;


// Client => TS

extern RWS::CEventId g_EventTSItemUse;			// ������ ���
extern RWS::CEventId g_EventTSItemGet;			// ������ ȹ��
extern RWS::CEventId g_EventTSItemEquip;		// ������ ����
extern RWS::CEventId g_EventTSScoutUse;			// ��ī���� ���
extern RWS::CEventId g_EventTSSkillUse;			// ��ų ���
extern RWS::CEventId g_EventTSRankBattle;		// ��ũ��Ʋ
extern RWS::CEventId g_EventTSColTriggerObject;	// Ʈ���� ������Ʈ �浹
extern RWS::CEventId g_EventTSBindStone;		// ���ε� ���濡 ���ε� ��
extern RWS::CEventId g_EventTSSearchQuest;		// ��ī���Ϳ��� ����Ʈ ã�� ��
extern RWS::CEventId g_EventTSItemUpgrade;		// ������ ���׷��̵� ��
extern RWS::CEventId g_EventTSTeleport;			// �ڷ���Ʈ ��
extern RWS::CEventId g_EventTSBudokai;			// õ������ ����ȸ
extern RWS::CEventId g_EventTSSlotMachine;		// ���� �ӽ�
extern RWS::CEventId g_EventTSHoipoiMix;		// ȣ������ �ͽ�
extern RWS::CEventId g_EventTSPrivateShop;		// ���λ���
extern RWS::CEventId g_EventTSFreeBattle;		// ����
extern RWS::CEventId g_EventTSItemIdentity;		// ������ ����
extern RWS::CEventId g_EventTSUseMail;			// ���� ���
extern RWS::CEventId g_EventTSParty;			// ��Ƽ

extern RWS::CEventId g_EventTSClickSideIcon;	// 
extern RWS::CEventId g_EventTSLevelCheck;		// 
extern RWS::CEventId g_EventTSQuest;			// 
extern RWS::CEventId g_EventTSDialogOpen;		// 

//////////////////////////////////////////////////////////////////////////
// Server �� TS ���� ���� �޽�����
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
/**
* trigger direct system message
*/
//////////////////////////////////////////////////////////////////////////

extern RWS::CEventId  g_EventQuestDirectForward;
extern RWS::CEventId  g_EventQuestDirectEcho;
extern RWS::CEventId  g_EventQuestMark;


//////////////////////////////////////////////////////////////////////////
/**
* trigger object
*/
//////////////////////////////////////////////////////////////////////////

extern RWS::CEventId  g_EventTObjectUpdateState;

//////////////////////////////////////////////////////////////////////////
/**
* DragonBall Collection
*/
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId  g_EventNightEffect;				// ��� ����� ȭ���� ��ο����� ȿ�� 
extern RWS::CEventId  g_EventDBCNarration;				// ��� �����̼ǿ� �̺�Ʈ
extern RWS::CEventId  g_EventDBCNarrationEnd;			// ��� �����̼� ���� �̺�Ʈ
extern RWS::CEventId  g_EventDBCCheck_Res;				// Check ���� ��Ŷ
extern RWS::CEventId  g_EventDBCReward_Res;				// ���� ���� ��Ŷ
extern RWS::CEventId  g_EventDBCCollect_Nfy;			// �������� �Ϲ������� ������ ��Ŷ
extern RWS::CEventId  g_EventDBCShedule_Info;			// DBC�� ������ ���� ��Ŷ
extern RWS::CEventId  g_EventDBCScatter;                 // �巡�ﺼ�� ������� ���� ������ �˸��� �̺�Ʈ

//////////////////////////////////////////////
/**
* simulation information update
*/
//////////////////////////////////////////////

//-----------------------------
// simulation object attack mode event
//-----------------------------

extern RWS::CEventId g_EventSobInfoUpdate;
extern RWS::CEventId g_EventSobNpcCommunity;
extern RWS::CEventId g_EventSobTriggerObjCommunity;
extern RWS::CEventId g_EventSobPrivateShopCommunity;
extern RWS::CEventId g_EventSobDynamicObjCommunity;
extern RWS::CEventId g_EventSobTargetInfoUpdate;
extern RWS::CEventId g_EventSobTargetChanged;
extern RWS::CEventId g_EventSobPickWorldItem;
extern RWS::CEventId g_EventSobCastingDirect;
extern RWS::CEventId g_EventSobRevivalNotify;
extern RWS::CEventId g_EventNPCCommuExit;              // Ŀ�´�Ƽ ���¸� ���������� �̺�Ʈ


//////////////////////////////////////////////
/**
* ���� ������.
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventSobGetState;

extern RWS::CEventId g_EventSobGetFightingMode;

extern RWS::CEventId g_EventSobGetAirMode;

//////////////////////////////////////////////
/**
* Ʈ���� ������Ʈ ��ī���� ���� ����
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventScouter;				// ��ī���� �̺�Ʈ

//////////////////////////////////////////////
/**
* PopoStone World Load Ready Event
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCharReady;								// ���� �ε尡 ���� ������ �߻�

//////////////////////////////////////////////
/**
* SideIcon : HintIcon Register
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventRegSideHintIcon;

//////////////////////////////////////////////
/**
* World concept
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventChangeWorldConceptState;

extern RWS::CEventId g_EventTMQCleintState;

//////////////////////////////////////////////////////////////////////////
// UI
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventToolTip; 
extern RWS::CEventId g_EventUIConfigReload;

//////////////////////////////////////////////////////////////////////////
// Operate object
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventOperateObjectTimeBar; 

//////////////////////////////////////////////////////////////////////////
// KnockDown Notify -> client wakeup gui
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventKnockDownNfy;
extern RWS::CEventId g_EventKnockDownWakeUpNfy;

extern RWS::CEventId g_EventBlockingNfy;
extern RWS::CEventId g_EventChargingNfy;

//////////////////////////////////////////////////////////////////////////
// Tutorial
//////////////////////////////////////////////////////////////////////////

// Actions
extern RWS::CEventId g_EventTLDefaultSkin;

extern RWS::CEventId g_EventTLGuideMsg_ShowHide;
extern RWS::CEventId g_EventTLGuideMsg_ChangeColor;

extern RWS::CEventId g_EventTLGuideIMG_ShowHide;

extern RWS::CEventId g_EventTLGuideIMGOk_ShowHide;

extern RWS::CEventId g_EventTLUIIndicator_ShowHide;

extern RWS::CEventId g_EventTLMouseInputLockUnlock;

extern RWS::CEventId g_EventTLKeyboardInputLockUnlock;

extern RWS::CEventId g_EventTLActionMapBackup;

extern RWS::CEventId g_EventTLActionMapRestore;

extern RWS::CEventId g_EventTLExcuteQuest;

extern RWS::CEventId g_EventTLAttackLockUnlock;

extern RWS::CEventId g_EventTLPlayCamera;

extern RWS::CEventId g_EventTLMinimap;

extern RWS::CEventId g_EventTLNotifyLockUnlock;

// Condition
extern RWS::CEventId g_EventTLCondition;
extern RWS::CEventId g_EventTLDisableMinimap;
extern RWS::CEventId g_EventTLDisableDialogMoveOption;
extern RWS::CEventId g_EventTLEnableSkip;

///////////////////////////////////////////////////////////////////////////////////////////
// Tenkaichi Budokai
extern RWS::CEventId g_EventMinorMatchStateUpdate;
extern RWS::CEventId g_EventMinorMatchTeamInfo;
extern RWS::CEventId g_EventMinorMatchPlayerState;
extern RWS::CEventId g_EventMinorMatchUpdateScore;
extern RWS::CEventId g_EventMinorMatchTeamScore;
extern RWS::CEventId g_EventMinorMatchSelection;
extern RWS::CEventId g_EventMinorMatchStageFinish;
extern RWS::CEventId g_EventMinorMatchMatchFinish;
extern RWS::CEventId g_EventMajorMatchStateUpdate;
extern RWS::CEventId g_EventMajorMatchTeamInfo;
extern RWS::CEventId g_EventMajorMatchPlayerState;
extern RWS::CEventId g_EventMajorMatchUpdateScore;
extern RWS::CEventId g_EventMajorMatchStageFinish;
extern RWS::CEventId g_EventMajorMatchMatchFinish;
extern RWS::CEventId g_EventFinalMatchStateUpdate;
extern RWS::CEventId g_EventFinalMatchTeamInfo;
extern RWS::CEventId g_EventFinalMatchPlayerState;
extern RWS::CEventId g_EventFinalMatchUpdateScore;
extern RWS::CEventId g_EventFinalMatchStageFinish;
extern RWS::CEventId g_EventFinalMatchMatchFinish;
extern RWS::CEventId g_EventAwardInfo;

extern RWS::CEventId g_EventBudokaiNoticeNfy;					///< õ�����Ϲ���ȸ ����
extern RWS::CEventId g_EventBudokaiStateInfoNfy;				///< õ������ ����ȸ�� ���� ���¸� ����
extern RWS::CEventId g_EventBudokaiUpdateStateNfy;				///< õ������ ����ȸ Main State update
extern RWS::CEventId g_EventBudokaiUpdateMatchStateNfy;			///< õ������ ����ȸ Match State Update
extern RWS::CEventId g_EventBudokaiJoinIndividualRes;			///< ���� ���
extern RWS::CEventId g_EventBudokaiLeaveIndividualRes;			///< ���� ���
extern RWS::CEventId g_EventBudokaiJoinTeamInfoRes;				///< �� ����Ʈ
extern RWS::CEventId g_EventBudokaiJoinTeamRes;					///< �� ���
extern RWS::CEventId g_EventBudokaiJoinTeamNfy;					///< �� ��� �˸�
extern RWS::CEventId g_EventBudokaiLeaveTeamRes;				///< �� ��� ���
extern RWS::CEventId g_EventBudokaiLeaveTeamNfy;				///< �� ��� ��� �˸�
extern RWS::CEventId g_EventBudokaiLeaveTeamMemberRes;			///< �� ��� ���
extern RWS::CEventId g_EventBudokaiLeaveTeamMemberNfy;			///< �� ��� ��� �˸�
extern RWS::CEventId g_EventBudokaiJoinInfoRes;					///< �� ��� ����
extern RWS::CEventId g_EventBudokaiMudosaInfoRes;				///< ������ �ڷ���Ʈ ����
extern RWS::CEventId g_EventBudokaiJoinStateNfy;				///< õ������ ����ȸ ��������
extern RWS::CEventId g_EventBudokaiJoinStateRes;				///< ���� ���� ���� ���

extern RWS::CEventId g_EventBudokaiProgressMessageNfy;			///< õ������ ����ȸ ���� ���� �޽��� ���

// Cinematic Command
extern RWS::CEventId g_EventCinematicGuiCommand;					///< �ó׸�ƽ Ŀ�ǵ�

// Dialog Moveable Option
extern RWS::CEventId g_EventDialogMove;

///////////////////////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventGambleAniStart;
extern RWS::CEventId g_EventGambleAniEnd;


///////////////////////////////////////////////////////////////////////////////////////////
// game server change

extern RWS::CEventId g_EventGameServerChangeOut;
extern RWS::CEventId g_EventGameServerChangeIn;
extern RWS::CEventId g_EventGameServerChannelChanged;

///////////////////////////////////////////////////////////////////////////////////////////
// game chat option
extern RWS::CEventId g_EventGameChatOption;

///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
// Naming
extern RWS::CEventId g_EventChangeCharName;

extern RWS::CEventId g_EventChangeNickName;				///< ���� �ٲٱ�

///////////////////////////////////////////////////////////////////////////////////////////
// Change Class Authority Changed
extern RWS::CEventId g_EventChangeClassAuthorityChangedNfy;	///< �ƹ�Ÿ �����ڿ��� ����´�. 

///////////////////////////////////////////////////////////////////////////////////////////
// BattleAttribute refresh ( Ÿ���õ� ����� ���� ���� )
extern RWS::CEventId g_EventCharRefreshBattleAttributeModifier;

///////////////////////////////////////////////////////////////////////////////////////////
// RankPoint reset notify
extern RWS::CEventId g_EventCharRankPointResetNfy;

//////////////////////////////////////////////////////////////////////////
// QuickSlot ���� Option Event
extern RWS::CEventId g_EventQuickSlotGuiMode;				///< ������ Ȯ�� �ɼ�
extern RWS::CEventId g_EventQuickSlotLockMode;				///< ������ �� ���

//////////////////////////////////////////////////////////////////////////
// Mob ���� Event
extern RWS::CEventId g_EventMobDie;				///< Mob Die

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
*  Simulation layer Events Data Structer
*/

//////////////////////////////////////////////
/**
* system message
*/


struct SNtlEventSysMsg						// system message event data structer.
{
	SERIAL_HANDLE	hSerialId;				// target serial id
	std::string		strMsg;
	RwBool			bServerMsg;				
};

struct SNtlEventSysStringMsg
{
	enum TYPE { TYPE_MSGBOX			= 0x01, 
		TYPE_LOBBY_MSGBOX	= 0x02,
		TYPE_CHAT_WARN		= 0x04,
		TIMEFIXED_NOTIFY	= 0x08,
		ACTION_NOTIFY		= 0x10,
		TYPE_CHAT_NOTICE	= 0x20,
		TYPE_LOG			= 0x40,
		TYPE_CONSOLE		= 0x80 }; // must be same as alarm message type

	RwUInt32 eType;
	WCHAR*	 szMessage;
};

struct SNtlEventFormatSysMsg
{
	SERIAL_HANDLE	hSerialID;
	std::string		strMsg;
	RwBool			bServerMsg;				
	va_list			args;
};

struct SNtlEventFormatSysStringMsg
{
	WCHAR*			szMessage;
	va_list			args;
};

struct SNtlEventNetSendError
{
	void *pNetSender;
};

//////////////////////////////////////////////
/**
* world change
*/

struct SNtlEventWorldChange
{
	RwBool bWorldChange;
};

struct SNtlEventCreateWorld
{
	RwBool bCreate;                     ///< CreateWorld = TURE, DestroyWorld = FALSE
};

//////////////////////////////////////////////
/**
* avatar/camera �̵� �� ȸ��
*/

struct SNtlEventCameraSmoothingMove
{
	RwBool			bEnable;
};

struct SNtlEventCameraShake					// camera shake event data structer.
{
	void			*pActor;
	RwBool			bLowShakeFlag;
};

struct SNtlEventCameraDash					// camera dash event data structer.
{
	void			*pActor;
	RwReal			fDashSpeed;	// dash speed
	RwV3d			vDest;		// dash dest
};

struct SNtlEventCameraNpc					// camera npc click ���� event data structer.
{
	void			*pActor;
};

struct SNtlEventCameraDB
{
	void			*pActor;
};

struct SNtlEventCameraTimeMachine
{
	void            *pObject;           ///< Ÿ�Ӹӽ� ������Ʈ�� ������
};

struct SNtlEventCameraExplosion
{
	void            *pObject;           ///< ���ߵǴ� ��ü�� ������
	unsigned int	uiExplosionId;		///< ���� ���̵� "ENtlPLExplosionEventType" �� �ش�
};

struct SNtlEventCameraTimeMachineArrive
{
	RwBool			bStart;				///< TRUE : ����, FALSE : ����
	void            *pObject;           ///< Ÿ�̸ӽ� ������Ʈ ��ü�� ������
};

struct SNtlEventCameraKnockDownMatrix	// camera knockdown matrix ���� event data structer.
{
	void			*pActor;
	RwReal			fLifeTime;
};


struct SNtlEventCameraDTSplineNode
{
	RwReal			fDelayTime;
	RwV3d			vStartPos;
	RwV3d			vEndPos;
	RwV3d			vLookAtPos;
	RwUInt8			byDirectLookAtType;
	RwChar			chKey[NTL_MAX_DIRECT_NAME];
};

struct SNtlEventCameraControlDelete
{
	RwUInt8			byControllerType;
};

struct SNtlEventCameraControlBus
{
	CNtlSobActor*	pActor;
};

/// ������ ī�޶� Ÿ��
enum EObserverCameraType
{
    E_OBSERVER_TYPE_STATIC,     ///< ���� ī�޶� ��ġ
    E_OBSERVER_TYPE_NODE,       ///< DTEditor���� ���� ������
};

struct SNtlEventCameraObserver
{
    EObserverCameraType eType;      ///< Ÿ��
    RwInt32             nIndex;     ///< �ε���
};

//////////////////////////////////////////////
/**
* action mapping
*/

//////////////////////////////////////////////
/** 
*simulation object event
*/

/// Sob�� ������ �̺�Ʈ���� ���� ����ü
struct SNtlEventSob
{
    SERIAL_HANDLE hSerialId;
};

struct SNtlEventActionMapJump
{
	RwUInt32		uiMoveFlags;
};

struct SNtlEventActionMapAirJump
{
	RwUInt32		uiMoveFlags;
};

struct SNtlEventSobShowNfy
{
	SERIAL_HANDLE	hSerialId;
	RwBool			bShow;
};

struct SNtlEventTerrainClick				// terrain click event data structer.
{
	RwV3d			vPickPos;
	RwBool			bServerSend;
};

struct SNtlEventKeyboardMove				// avater keyboard move event data structer.
{
	RwUInt8			byMoveFlags;
	RwBool			bAvatarAngleChange;
	RwReal			fAvatarAngleY;
};

struct SNtlEventKeyboardDashMove
{
	RwUInt32		uiMoveFlags;
};

struct SNtlEventMouseDashMove
{
	RwReal			fLimitDist;
	RwV3d			vPickPos;
};

struct SNtlEventAirDashMove
{
	RwBool			bMouse;
	RwV3d			vPickPos;
	RwUInt32		uiMoveFlags;
	RwBool			bUpdateMove;
};

struct SNtlEventAirAccelMove
{
	BYTE			byMoveDirection;
	RwBool			bUpdateMove;
};

struct SNtlEventInputChangeHading			// avater heading hading
{
	RwBool			bSuccess;		// change heading�� ó���Ͽ��°�?
	RwBool			bSoothing;
    RwReal			fDeltaAngleY;		// left/right

	bool			bSetAngleX;
	RwReal			fDeltaAngleX;		// up/down
};

struct SNtlEventSobTargetSelect : public SNtlEventSob
{
	RwBool			bSuccess;	
	RwBool			bFollow;
};

struct SNtlEventSobTargetSelectRelease : public SNtlEventSob
{
	
};
    
struct SNtlEventSobAttackMarkRelease : public SNtlEventSob
{
	
};

struct SNtlEventSobAttackSelect : public SNtlEventSob
{
	void			*pSobActor;
};

struct SNtlEventSobTargetMarkSelect : public SNtlEventSob
{
    
};

struct SNtlEventSobTargetMarkRelease : public SNtlEventSob
{
    
};


struct SNtlEventSobGotFocus : public SNtlEventSob
{
	
};

struct SNtlEventSobLostFocus : public SNtlEventSob
{
};


struct SNtlEventSobBaseCreate : public SNtlEventSob
{
	RwUInt32		uiClassId;	
	void			*pSobObj;
};

struct SNtlEventSobCharCreate : public SNtlEventSobBaseCreate
{
	sCHARSTATE		*pCharState;
};

struct SNtlEventSobAvatarCreate : public SNtlEventSobCharCreate
{
	RwBool bUIAvatarCreate;
};

struct SNtlEventSobPlayerCreate : public SNtlEventSobCharCreate	// simulation player object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	sPC_BRIEF		*pPcBrief;
	RwBool          bIsClientCreate;     // ������ �ƴ� Ŭ���̾�Ʈ ��ü���� �����ߴ��� ���� (ex.�ó׸�ƽ)
};

struct SNtlEventSobMonsterCreate : public SNtlEventSobCharCreate	// simulation monster object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	sMOB_BRIEF		*pMobBrief;
	RwBool          bIsClientCreate;    // ������ �ƴ� Ŭ���̾�Ʈ ��ü���� �����ߴ��� ���� (ex.�ó׸�ƽ)
};

struct SNtlEventSobNpcCreate : public SNtlEventSobCharCreate	// simulation npc object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	sNPC_BRIEF		*pNpcBrief;
	RwBool			bCreateName;
	RwBool          bIsClientCreate;    // ������ �ƴ� Ŭ���̾�Ʈ ��ü���� �����ߴ��� ���� (ex.�ó׸�ƽ)
};

struct SNtlEventSobVehicleCreate : public SNtlEventSobCharCreate	// simulation vehicle object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	RwUInt32		uiVehicleTblIdx;
	// g_EventSobCreate �̺�Ʈ�� ����ü�� ���� Vehicle ���̺� �ε���
	// g_EventSobVehicleEngine �̺�Ʈ�� ����ü�� ���� Sob Item �ڵ�
	RwBool			bForChild;
	RwBool			bOwnerAvatar;
};

union UPetData
{
	void				*pPetData;
	sSUMMON_PET_BRIEF	*pPetBrief;
	sSUMMON_PET_PROFILE *pPetProfile;
};

struct SNtlEventSobPetCreate : public SNtlEventSobCharCreate	// simulation pet object create
{
	RwV3d			vLoc;
	RwV3d			vDir;

	RwBool			bOwnerAvatar;
	UPetData		uPetBrief;
};

struct SNtlEventSobPetSkillInfo		// Pet Skill info
{
	BYTE				bySkillCount;
	sSKILL_INFO_PET		aSkillInfo[NTL_MAX_NPC_HAVE_SKILL];
};

struct SNtlEventSobWorldItemCreate : public SNtlEventSobBaseCreate
{
	RwV3d			vLoc;
	RwInt32			eObjType;
	void*			pBrief;
	void*			pState;
	void*			pItemOptionSet;
};

struct SNtlEventSobItemCreate : public SNtlEventSobBaseCreate
{
	void			*pParentSobObj;
	RwUInt32		uiItemTblId;
	sITEM_OPTION_SET sOptionSet;
	RwUInt8			byPlace;
	RwUInt8			bySlotIdx;
	RwUInt8			byStackCount;
	RwUInt8			byRank;
	RwUInt8			byGrade;
	RwUInt8			byDur;
	RwBool			bNeedToIdentify;
	RwBool			bCreateItemModel;
	RwBool			byBattleAttribute;
	RwUInt8			byRestrictState;
	const WCHAR*	wszMaker;
	RwUInt8			byDurationType;
	DBOTIME			StartTime;
	DBOTIME			EndTime;
};

struct SNtlEventSobQuestItemCreate : public SNtlEventSobBaseCreate
{
	RwUInt32		uiQuestItemTblId;
	RwUInt8			bySlotIdx;
	RwUInt8			byStackCount;
};

enum eNtlEventSobWarehouseUpdateType
{
	NESWUT_RECIEVE_DATA,		///< ������ ���� â�� �����͸� �޾Ҵ�
	NESWUT_WAREHOUSE_START,		///< â���� ����
	NESWUT_BUY_SLOT,			///< â���� �����Ѵ�
	NESWUT_ADD_ZENNY,			///< â���� ���ϸ� �ִ´�
	NESWUT_SUB_ZENNY,			///< â������ ���ϸ� ����
	NESWUT_CLOSE,				///< â���� �ݴ´�
};

struct SNtlEventSobWarehouseUpdate
{
	RwUInt32		uiMessageType;
	RwUInt32		uiSerial;
	RwUInt32		uiValue;
	RwInt32			iValue2;
};

struct SNtlEventSobWarehouseItemCreate
{
	RwUInt8			byItemCount;	///< ��ȿ ������ ����
	void*			pData;			///< ���� ������
};

struct SNtlEventSobStatusCreate : public SNtlEventSobBaseCreate
{
	RwBool			bDamage;
	RwUInt32		uiCurrHp;
	RwUInt32		uiDamage;
	RwUInt32		uiFillHp;
	RwUInt32		uiFillMp;
	RwUInt32		uiFillCp;
};

struct SNtlEventSobTriggerObjectCreate : public SNtlEventSobBaseCreate
{
	RwUInt32		uiTriggerObjectTblid;
	RwV3d			vLoc;
	void			*pPLEntity;
};

struct SNtlEventSobDynamicObjectCreate : public SNtlEventSobBaseCreate
{
	RwUInt8			byType;
	RwUInt32		uiTableIndexForType;
	RwV3d			vLoc;
	RwUInt8			byState;
	SERIAL_HANDLE	hOwner;
};

struct SNtlEventSobEventObjectCreate : public SNtlEventSobBaseCreate
{
	void*           pPlEntity;
};

struct SNtlEventSobProjectileCreate : public SNtlEventSobBaseCreate
{
	SERIAL_HANDLE	hOwnerSerialId;
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt32		uiBehavior;
	RwUInt8			byShotType;
	RwReal			fSpeed;
	RwBool			bLeftHand;
	RwBool			bCreateHitStuffEffect;
	RwChar			chBoneName[NTL_MAX_BONE_NAME];
	RwChar			chEffName[NTL_MAX_EFFECT_NAME];
	RwReal			fTargetHeight;
	void			*pProjExtData;
	RwReal			fWeightElapsedValue;
};

struct CNtlEventSobSkillCreate : public SNtlEventSobBaseCreate
{
	RwUInt32		uiOwnerID;
	RwUInt32		uiSkillTblId;
	RwUInt8			bySlodIdx;
	RwUInt8			byRPBonusType;
	RwBool			bRPBonusAutoMode;
	RwInt32			iExp;
	RwUInt32		uiRemainTime;		
};

struct SNtlEventSobHTBSkillCreate : public SNtlEventSobBaseCreate
{
	RwUInt32		uiOwnerID;
	RwUInt32		uiHTBSkillTblId;
	RwUInt8			bySlotIdx;
	RwUInt32		uiRemainTime;
};

struct SNtlEventSobActionSkillCreate : public SNtlEventSobBaseCreate
{
	RwUInt32		uiActionSkillTblId;
	RwUInt8			bySlotIdx;
};

struct CNtlEventSobBuffCreate : public SNtlEventSobBaseCreate
{
	BYTE					byBuffIndex;
	BYTE					byBuffType;
	RwUInt32				uiTblId;
	RwUInt32				uiRemainTime;
	RwUInt32				uiKeepTime;
	sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
	bool					bActive;
};


// simulation object delete
struct SNtlEventSobDelete
{
	SERIAL_HANDLE	hSerialId;
};

struct SNtlEventSobSummonPetSpawnSync : public SNtlEventSob
{	
	SERIAL_HANDLE	hPetSerialId;
};

struct SNtlEventSobMove : public SNtlEventSob
{	
	RwBool			bMouseMove;		// mouse move or keyboard move
	RwV3d			vCurrLoc;
	RwV3d			vLoc;
	RwV3d			vDir;
	RwUInt8			byMoveFlags;
	RwUInt8			byFormFlag;		// �ȱ�, �ٱ�, ����.
	RwUInt32		uiTimeStamp;
	RwUInt8			byPathNodeNum;
	RwV3d			*pPathNode;
};

/// �̵� ��ũ �̺�Ʈ ����ü
struct SNtlEventSobMoveSync : public SNtlEventSob
{
    RwV3d vLoc;
    RwV3d vDir;
    DWORD dwTimeStamp;
    RwUInt8 byMoveFlag;
};

struct SNtlEventSobSecondDestMove : public SNtlEventSob
{
	RwUInt8			byFormFlag;		// �ȱ�, �ٱ�, ����.
	RwV3d			vCurrLoc;
	RwV3d			vLoc;
	RwV3d			vSecondDest;
	RwUInt32		uiTimeStamp;
};

struct SNtlEventSobDirectionFloat  : public SNtlEventSob
{
	RwUInt8			byMoveFlags;
};

struct SNtlEventSobSplineMove : public SNtlEventSob
{
	RwBool			bGroundCheck;
	RwBool			bRunMove;
	RwChar			*pSplineName;
	RwV3d			vStartPos;
};

struct SNtlEventSobAdjustMovePosition : public SNtlEventSob
{
	RwV3d			vAdjustPos;
};

struct SNtlEventSobDash : public SNtlEventSob
{
	RwV3d			vDest;
	RwUInt8			byMoveFlags;
	RwUInt32		uiTimeStamp;
};


struct SNtlEventSobFollowMove : public SNtlEventSob
{
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt8			byFormFlag;	
	RwV3d			vCurrLoc;
	RwReal			fFollowRange;
	RwUInt8			byReason;
	RwUInt32		uiTimeStamp;
};

struct SNtlEventSobMoveStop : public SNtlEventSob
{
	RwV3d			vLoc;
	RwV3d			vDir;
};

struct SNtlEventSobAdjustMove : public SNtlEventSob
{
	RwV3d			vDest;
	RwV3d			vHeading;
};


struct SNtlEventSobItemAdd : public SNtlEventSob
{
	SERIAL_HANDLE	hItemSerialId;
	RwUInt32		uiItemTblId; 
	sITEM_OPTION_SET	sOptionSet;
	RwUInt8			byPlace;
	RwUInt8			bySlotIdx;
	RwUInt8			byStackCount;
	RwUInt8			byRank;
	RwUInt8			byGrade;
	RwUInt8			byDur;
	RwUInt8			byMaxDur;
	RwBool			bNeedToIdentify;
	RwUInt8			byBattleAttribute;
	RwUInt8			byRestrictState;
	const WCHAR*	wszMaker;
	RwUInt8			byDurationType;
	DBOTIME			StartTime;
	DBOTIME			EndTime;
};

struct SNtlEventSobItemDelete : public SNtlEventSob
{
	SERIAL_HANDLE	hItemSerialId;
	RwUInt8			byPlace;
	RwUInt8			bySlotIdx;
};

struct SNtlEventItemSocketInsertBead : public SNtlEventSob
{
	RwBool					bSuccess;
	RwUInt8					byItemPlace;
	RwUInt8					byItemPos;
	RwUInt8					byRestrictState;
	RwUInt8					byDurationType;
	DBOTIME					nUseStartTime;
	DBOTIME					nUseEndTime;
	sITEM_RANDOM_OPTION*	pRandomOption;
	RwUInt8					byBeadPlace;
	RwUInt8					byBeadPos;
	RwUInt8					byBeadRemainStack;
};

struct SNtlEventItemSocketDestroyBead : public SNtlEventSob
{
	RwUInt8			byPlace;
	RwUInt8			byPos;
	RwUInt8			byRestrictState;
	RwUInt8			byDurationType;
};

struct SNtlEventSobItemUpdate : public SNtlEventSob
{
	SERIAL_HANDLE	hItemSerialId;
	RwUInt32		uiItemTblId; 
	sITEM_OPTION_SET sOptionSet;
	RwUInt8			byPlace;
	RwUInt8			bySlotIdx;
	RwUInt8			byStackCount;
	RwUInt8			byRank;
	RwUInt8			byGrade;
	RwUInt8			byDur;
	RwUInt8			byMaxDur;
	RwBool			bNeedToIdentify;
	RwUInt8			byBattleAttribute;
	RwUInt8			byRestrictState;
	const WCHAR*	wszMaker;
	RwUInt8			byDurationType;
	DBOTIME			StartTime;
	DBOTIME			EndTime;
};

struct SNtlEventSobItemDurUpdate : public SNtlEventSob
{
	RwUInt8			byDur;
};

struct SNtlEventSobItemMove : public SNtlEventSob
{
	SERIAL_HANDLE	hSrcSerial;
	RwUInt8			bySrcPlace;
	RwUInt8			bySrcSlotIdx;
	SERIAL_HANDLE	hDestSerial;
	RwUInt8			byDestPlace;
	RwUInt8			byDestSlotIdx;
	RwUInt8			byBattleAttribute;
};
    
struct SNtlEventSobItemStackMove : public SNtlEventSob
{
	SERIAL_HANDLE	hSrcSerial;
	RwUInt8			bySrcPlace;
	RwUInt8			bySrcSlotIdx;
	RwUInt8			bySrcStackCount;
	SERIAL_HANDLE	hDestSerial;
	RwUInt8			byDestPlace;
	RwUInt8			byDestSlotIdx;
	RwUInt8			byDestStackCount;
	RwUInt8			byBattleAttribute;
};

struct SNtlEventSobEquipChange : public SNtlEventSob
{
	RwUInt8			bySlotIdx;
	RwUInt32		uiTblId;
	RwUInt8			byGrade;
	RwUInt8			byBattleAttribute;
};

struct SNtlEventSobItemUseAction : public SNtlEventSob
{
	RwUInt32		uiItemTableIdx;
	RwUInt8			ucSkillResultCount;
	sSKILL_RESULT*	pSkillResult;
};

struct SNtlEventSobItemUpgrade
{
	BYTE			byItemGrade;
	BYTE			byItemPlace;
	BYTE			byItemPos;
	BYTE			byStonePlace;
	BYTE			byStonePos;
	BYTE			byStoneStack;
	BYTE			byCorePlace;
	BYTE			byCorePos;
	BYTE			byCoreStack;
	bool			bItemDestroy;
};

struct SNtlEventSobItemChangeOptionSet
{
	void*		pOptionSet;
	BYTE		byItemPlace;
	BYTE		byItemPos;
	BYTE		byKitPlace;
	BYTE		byKitPos;
	BYTE		byKitRemainStack;
};

struct SNtlEventSobItemChangeBattleAttribute
{
	BYTE			byBattleAttribute;
	BYTE			byItemPlace;
	BYTE			byItemPos;
};

struct SNtlEventSobEquippedItemUpgradeAll
{
	RwUInt8			byCount;		// Equip Index�� ����
	RwUInt8*		abyUpgradeValue;
};

struct SNtlEventSobQuestItemAdd : public SNtlEventSob
{
	RwUInt32		uiQuestItemTblId; 
	RwUInt8			bySlotIdx;
	RwUInt8			byStackCount;
};

struct SNtlEventSobQuestItemDelete : public SNtlEventSob
{
	RwUInt8			bySlotIdx;
};

struct SNtlEventSobQuestItemMove : public SNtlEventSob
{
	RwUInt8			bySrcSlotIdx;
	RwUInt32		uiSrcTblIdx;
	RwUInt8			byDestSlotIdx;
	RwUInt32		uiDestTblIdx;
};

struct SNtlEventSobQuestItemUpdate : public SNtlEventSob
{
	RwUInt8			bySlotIdx;
	RwUInt8			byCount;
};

struct SNtlEventSobItemCancel : public SNtlEventSob
{
	
};

struct SNtlEventSobItemCasting : public SNtlEventSob
{
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt32		uiItemTblId;
	RwReal			fCurrCastingTime;
	RwReal			fMaxCastingTime;
};

struct SNtlEventSobDeleteQuickSlotIcon : public SNtlEventSob
{
};


struct SNtlEventPartySelectStateInit
{
	BYTE		bySelectState;
	DWORD		dwParam;
	bool		bLastStage;
};

struct SNtlEventPartySelectState
{
	HOBJECT		hTarget;
	BYTE		bySelectState;
};

struct SNtlEventBattleDungeonStateUpdate
{
	BYTE		byStage;
	TBLIDX		titleTblidx;
	TBLIDX		subTitleTblidx;
	DWORD		dwLimitTime;
};

struct SNtlEventBattleDungeonLimitTime
{
	DWORD		dwLimitTime;
	bool		bStart;
};


struct SNtlEventSobItemRestrictionUpdate : public SNtlEventSob
{
	BYTE	byPlace;
	BYTE	byPos;
	BYTE	byRestrictState;
};

struct SNtlEventSobStanding : public SNtlEventSob
{
	RwV3d			vLoc;
	RwV3d			vDir;
};


struct SNtlEventSobFightingMode : public SNtlEventSob
{
	RwBool			bFightingMode;
};

struct SNtlEventSobAttack : public SNtlEventSob
{
	SERIAL_HANDLE			hAttackerSerialId;
	SERIAL_HANDLE			hDefenderSerialId;
	RwUInt8					byAttRes;
	RwBool					bChainAtt;
	RwUInt8					byAttSeq;
	RwUInt8					byBlocking;
	RwUInt32				uiDamage;
	RwUInt32				uiAbsorbedDamage;
	RwUInt32				uiReflectedDamage;
	sDBO_LP_EP_RECOVERED	lpEpRecovered;
	RwV3d					vPushDir;
	RwUInt32				uiSyncEventId;
};

struct SNtlEventSobSpecialAttack : public SNtlEventSob
{
	SERIAL_HANDLE			hAttackerSerialId;
	SERIAL_HANDLE			hDefenderSerialId;
	BYTE					byAttackType;
	BYTE					bySourceType;
	TBLIDX					sourceTblidx;
	BYTE					byAttackResult;
	RwUInt32				uiDamage;
};

struct SNtlEventSobAttacked : public SNtlEventSob
{
	SERIAL_HANDLE	hAttackerSerialId;
};

struct SNtlEventSobPetBeginAttack : public SNtlEventSob
{
	SERIAL_HANDLE	hDefenderSerialId;
};

struct SNtlEventSobHit : public SNtlEventSob
{
	SHitStuff		sHitStuff;
};

struct SNtlEventSobAttackFollow : public SNtlEventSob
{
	SERIAL_HANDLE	hDefenderSerialId;
};

struct SNtlEventSobAttackMode : public SNtlEventSob
{
	RwBool			bAttackMode;
};

struct SNtlEventSobJump : public SNtlEventSob
{
	RwUInt8			byMoveDirection;
	RwV3d			vDir;
	RwV3d			vJumpDir;
};

struct SNtlEventSobJumpDirection : public SNtlEventSob
{
	RwV3d			vJumpDir;
	RwUInt32		uiTimeStamp;
};


struct SNtlEventSobJumpEnd : public SNtlEventSob
{
};

struct SNtlEventSobAirJump : public SNtlEventSob
{
	RwUInt8			byMoveDirection;
	RwV3d			vDir;
};

struct SNtlEventSobAirAccel : public SNtlEventSob
{
	RwUInt8			byMoveFlag;
	RwUInt8			byMoveDirection;
	RwV3d			vLoc;
	RwV3d			vDir;
};

struct SNtlEventSobSit : public SNtlEventSob
{
	RwBool			bSitDown;
};

struct SNtlEventSobPrivateShop : public SNtlEventSob
{
	WCHAR			wcShopName[NTL_MAX_PRIVATESHOP_NAME];
	RwBool			bIsOwnerEmpty;
	RwUInt8			byShopState;
};

/// ���� �̺�Ʈ ����ü
struct SNtlEventSobConvertClass : public SNtlEventSob
{
	RwUInt8			byClass;
};

/// �� ü���� �̺�Ʈ ����ü
struct SNtlEventSobChangeAdult : public SNtlEventSob
{
    RwBool          bAdult;             /// True�� ����, False�� ��̷� �ٲ۴�.
};

struct SDboEventCharTitleSelectNfy : public SNtlEventSob
{
	TBLIDX	uiTitle;
};

struct SNtlEventEnableAir
{
	bool			bFlag;
};

struct SNtlEventSetAp
{
	int			nAp;
	int			nMaxAp;
};

struct SNtlEventSobDirectPlay : public SNtlEventSob
{
	RwUInt8			byDirectType;
	RwUInt32		uiTblId;
	RwBool			bTimeLoop;
	RwReal			fLoopTime;
};

struct SNtlEventSobAnimPlay : public SNtlEventSob
{
	RwUInt32		uiAnimId;
	RwBool			bTimeLoop;
	RwReal			fLoopTime;
};


struct SNtlEventSobProxyGUIUpdate : public SNtlEventSob
{
	RwUInt32		uiUpdateFlags;
	RwBool			bShow;
};


struct SNtlEventSobCooling : public SNtlEventSob
{
	RwBool			bStartOrEnd;			/** TRUE : start , FALSE : end */
};

struct SNtlEventActionMapSkillUse : public SNtlEventSob
{
	RwBool			bExistTarget;
	SERIAL_HANDLE	hTargetSerialId;
	SERIAL_HANDLE	hSkillSerialId;
	RwReal			fFollowRange;
	RwUInt8			bySkillSlotIdx;
	RwUInt8			byRpBonusType;
	TBLIDX			skillTblidx;
};

struct SNtlEventActionMapHTBUse : public SNtlEventSob
{
	SERIAL_HANDLE	hTargetSerialId;
	SERIAL_HANDLE	hSkillSerialId;
	RwReal			fFollowRange;
	RwUInt8			bySkillSlotIdx;
};

struct SNtlEventActionMapItemUse : public SNtlEventSob
{
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt8			byPlace;
	RwUInt8			byPos;
	SERIAL_HANDLE	hRefObject;
	RwUInt8			byKeyPlace;
	RwUInt8			byKeyPos;
	RwUInt8			byCastingTime;
};

struct SNtlEventActionMapSocialUse
{
	RwUInt32		uiTblId;
};

struct SNtlEventActionMapCharging
{
	RwBool			bStartOrEnd;		// TRUE : start , FALSE : end
};

struct SNtlEventActionMapBlocking
{
	RwBool			bStartOrEnd;		// TRUE : start , FALSE : end
};


struct SNtlEventActionMapAutoFollow
{
	SERIAL_HANDLE	hTargetSerialId;
};

struct SNtlEventSobSkillCancel : public SNtlEventSob
{
	RwUInt16		wReason;
};

struct SNtlEventSobSkillAffectingCancel : public SNtlEventSob
{
};

struct SNtlEventSobSkillCasting : public SNtlEventSob
{
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt32		uiSkillTblId;
	RwReal			fCurrCastingTime;
	RwReal			fMaxCastingTime;
};

struct SNtlEventSobSkillAction : public SNtlEventSob
{
	RwUInt16		wResultCode;
	SERIAL_HANDLE	hTargetSerialId;
	RwUInt32		uiSkillTblId;
	RwUInt8			byRpBonusType;
	RwUInt8			byResultCount;		/** skill result count */
	sSKILL_RESULT	*pSkillAct;
	RwUInt32		uiSyncEventId;
	RwV3d			vFinalLoc;
};

struct SNtlEventSobSkillActioned : public SNtlEventSob
{
	SERIAL_HANDLE	hAttackerSerialId;
};

struct SNtlEventSobSkillAdd : public SNtlEventSob
{
	RwUInt8			bySkillSlotIdx;
	RwUInt32		uiSkillTblId;
};


struct SNtlEventSobSkillUpgrade : public SNtlEventSob
{
	RwUInt8			bySkillSlotIdx;
	RwUInt32		uiUpgradeSkillTblId;
};

struct SNtlEventSobSocialAction : public SNtlEventSob
{
	RwUInt32		uiTblIdx;
};

struct SNtlEventSobSocialActionCooling
{
	RwUInt32		uiTblIdx;
};

struct SNtlEventSetSkillPassiveEffect
{
	RwUInt32		uiSkillTblidx;
	RwUInt8			bySkillSlotIdx;
	sDBO_UPDATED_SKILL_DATA* pPassiveEffectData;
};

struct SNtlEventSobSkillInfoText
{
	BYTE			byMsgType;
	unsigned int	uiColor;
	WCHAR*			wchMsg;
};

struct SNtlEventSobHTBSkillAdd : public SNtlEventSob
{
	RwUInt8			byHTBSkillSlotIdx;
	RwUInt32		uiHTBSkillTblId;
};

struct SNtlEventSobHTBSkillAction : public SNtlEventSob
{
	sCHARSTATE		*pCharState;
};

struct SNtlEventSobBuffAdd : public SNtlEventSob
{
	BYTE					byBuffIndex;
	BYTE					byBuffType;
	RwUInt32				uiTblId;
	RwUInt32				uiRemainTime;
	RwUInt32				uiKeepTime;
	sDBO_BUFF_PARAMETER		aBuffParameter[NTL_MAX_EFFECT_IN_SKILL];
	bool					bActive;
};

struct SNtlEventSobBuffDrop : public SNtlEventSob
{
	BYTE			byBuffIndex;
	RwUInt8			byBuffType;
};

struct SNtlEventSobBuffRefreshAll : public SNtlEventSob
{
	RwUInt8			byCount;
	sBUFF_INFO*		pBuffInfo;	
};

struct SNtlEventSobBuffActivate : public SNtlEventSob
{
	sBUFF_INFO*		pBuffInfo;
};

struct SNtlEventSobBuffAutoRecover : public SNtlEventSob
{
	BYTE byBuffIndex;
	DWORD dwRemainTime;
	DWORD dwRemainValue;
};

typedef SNtlEventSobBuffAdd			SNtlEventFakeBuffAdd;
typedef SNtlEventSobBuffDrop		SNtlEventFakeBuffDrop;
typedef SNtlEventSobBuffRefreshAll	SNtlEventFakeBuffRefreshAll;


struct SNtlEventSobStateTransition : public SNtlEventSob
{
	RwUInt32		uiTransState;
};

struct SNtlEventSobServerUpdateState : public SNtlEventSob
{
	sCHARSTATE		*pState;
};

struct SNtlEventSobServerUpdateCondition : public SNtlEventSob
{
	QWORD  		    dwCondition;
};

struct SNtlEventSobServerUpdateAspect : public SNtlEventSob
{
	sASPECTSTATE    sApectState;
};

struct SNtlEventSobFainting : public SNtlEventSob
{
};

struct SNtlEventSobUpdateLPStatusNfy : public SNtlEventSob
{
    RwBool bEmergency;      // TRUE : ��� ����, FALSE : �Ϲ� ����
};

struct SNtlEventNightEffect
{
	RwBool			bOn;
};

//////////////////////////////////////////////
/**
* simulation information update
*/


// AIUT => AvatarInfoUpdateType

enum EEventAvatarInfoUpdateType
{
	EVENT_AIUT_ATTR					= 0x0001,
	EVENT_AIUT_ATTR_LEVELUP			= 0x0002,
	EVENT_AIUT_ITEM					= 0x0004,
	EVENT_AIUT_QUESTITEM			= 0x0008,
	EVENT_AIUT_SKILL				= 0x0010,	/** avatar�� ���� ������ ��ų�� ��â ������ �����ȴ� */
	EVENT_AIUT_SKILL_LEARN			= 0x0020,	/** ��ų ���� */
	EVENT_AIUT_SKILL_LEVELUP		= 0x0040,	/** ��ų level up*/
	EVENT_AIUT_SKILL_SP				= 0x0080,	/** ��ų SP */
	EVENT_AIUT_SKILL_COOLING_START	= 0x0100,	/** ��ų cooling start => uiParam1 : icon serial*/
	EVENT_AIUT_SKILL_COOLING_END	= 0x0200,	/** ��ų cooling end => uiParam1 : icon serial*/
	EVENT_AIUT_BUFF					= 0x0400,
	EVENT_AIUT_DIE					= 0x0800,
	EVENT_AIUT_ZENNY				= 0x1000,
	EVENT_AIUT_HTBSKILL_LEARN		= 0x2000,	/** HTB ���� **/
	EVENT_AIUT_WAREHOUSE			= 0x4000,	/** �������� ������ â�� ���ο����� ��ȭ **/	
	EVENT_AIUT_SKILL_RPBONUS		= 0x8000,	/** RPType��ȭ **/
	EVENT_AIUT_SKILL_UI_INIT		= 0x00010000, /** ��ų UI �ʱ�ȭ(�����̵�) **/
	EVENT_AIUT_SKILL_INIT			= 0x00020000, /** ��ų HTB �ʱ�ȭ(SP ��ȯ) **/
	EVENT_AIUT_POINT_HONOR			= 0x00040000, /** ���� ����Ʈ */
	EVENT_AIUT_POINT_MUDOSA			= 0x00080000, /** ������ ����Ʈ */
    EVENT_AIUT_NETPY                = 0x00100000, ///< PC��� NetPy ����Ʈ
};

struct SNtlEventSobInfoUpdate : public SNtlEventSob
{
	RwUInt32		uiUpdateType;
	RwUInt32		uiParam1;			
};


struct SNtlEventSobNpcCommunity : public SNtlEventSob
{
	RwBool			bShow;
	RwUInt32		uiTriggerId;
	RwUInt8			byInputType;
};

struct SNtlEventSobTriggerObjCommunity : public SNtlEventSob
{
	RwBool			bShow;
	RwUInt32		uiTriggerId;
	RwUInt8			byInputType;
};

struct SNtlEventSobPrivateShopCommunity : public SNtlEventSob
{
	RwBool			bOwnerEmpty;
	RwUInt8			byShopState;
};

struct SNtlEventSobDynamicObjCommunity : public SNtlEventSob
{
	RwBool			bShow;
};

struct SNtlEventSobTargetInfoUpdate : public SNtlEventSob
{
	RwBool			bShow;
};

struct SNtlEventSobTargetChanged : public SNtlEventSob
{
	RwBool			hTarget;
};

struct SNtlEventSobPickWorldItem : public SNtlEventSob
{
};

struct SNtlEventSobCastingDirect
{
	CNtlSobActor*	pActor;
	RwBool bSkillOrItemCasting;	// TRUE : skill, FALSE : item
	RwBool bStartOrEnd;			// TRUE : start, FALSE : end
};

enum ePartyMesssageType			///< ��Ƽ �޼���(SNtlEventPartyUpdate, SNtlEventPartyUpdateValue�� ���� ���� ����)
{
	PMT_PARTY_CREATE,			///< ��Ƽ ����	
	PMT_PARTY_LEAVE,			///< ��Ƽ Ż��
	PMT_PARTY_MEMBER_ADD,		///< ��Ƽ�� �߰�
	PMT_PARTY_MEMBER_DEL,		///< ��Ƽ�� Ż��
	PMT_PARTY_LEADER_CHANGE,	///< ��Ƽ�� ����

	PMT_ZENNY_DIVISION,			///< ���� �й� ����� ���� �Ǿ���
	PMT_ITEM_DIVISION,			///< ������ �й� ����� ���� �Ǿ���

	PMT_LP,						///< LP ������Ʈ
	PMT_EP,						///< EP ������Ʈ
	PMT_LEVEL,					///< Level ������Ʈ
	PMT_CLASS,					///< Ŭ���� ����

	PMT_MEMBER_LOCATION,		///< �ɹ� ��ġ

	PMT_PARTY_DUNGEON_STATE,	///< ��Ƽ ���� ����
	PMT_PARTY_DUNGEON_INITIALIZE,///< ��Ƽ �ʱ�ȭ
};

struct SNtlEventPartyUpdate		///< �ùķ��̼ǿ� ��Ƽ �޼���
{
	RwInt32			iMessage;		///< ��Ƽ �޼���
	SERIAL_HANDLE	hSerialId;		///< �ش� �ɹ��� �ø���
	RwUInt32		uiValue;		///< �޼����� ���� ��ġ
	RwInt32			iValue2;		///< �޼����� ���� ��ġ
};

struct SNtlEventPartyUpdateValue	///< Ŭ���̾�Ʈ�� ��Ƽ �޼���
{
	RwInt32			iValueType;		///< PartyUpdateValue Type
	SERIAL_HANDLE	hSerial;		///< �ش� �ɹ��� �ø���
	RwUInt32		uiValue;		///< �޼����� ���� ��ġ
	RwInt32			iValue2;		///< �޼����� ���� ��ġ
};

struct SNtlEventPartyMemberLocation
{
	RwUInt32		uiSerial;		///< �ɹ� �ø���
	RwUInt32		uiWorldID;		///< ���� ���̵�
	RwUInt32		uiWorldTbl;		///< ���� ���̺� ���̵�
	RwV3d			vCurLoc;		///< ��ġ
};

struct SNtlEventPartyCreate
{
	WCHAR*		pcName;			///< ��Ƽ �̸�
};

struct SNtlEventPartyAdd
{
	SERIAL_HANDLE	hSerial;
	WCHAR*			pcName;
	RwUInt8			byRace;
	RwUInt8			byClass;
	RwUInt8			byLevel;
	RwInt32			curLP;
	RwInt32			maxLP;
	RwUInt16		wCurEP;
	RwUInt16		wMaxEP;
	RwUInt32		uiWorldTableIndex;
	RwUInt32		uiWorldID;
	RwV3d			v3Pos;
};

enum eSLGuildEvent
{
	SLGE_GUILD_INFO,				///< ��� ������ �޴´�

	SLGE_ADD_MEMBER,				///< ��� �ɹ� �Ѹ��� ������ �޴´�
	SLGE_DEL_MEMBER,				///< ��� �ɹ� �Ѹ��� ������ �����

	SLGE_APPOINT_MASTER,			///< ����� �Ӹ�
	SLGE_APPOINT_SECOND_MASTER,		///< �α���� �Ӹ�
	SLGE_DISMISS_SECOND_MASTER,		///< �α���� ���� ����
	SLGE_APPOINT_MEMBER,			///< �Ϲ� �������� �Ӹ�

	SLGE_BEING_DISBAND,				///< ��� �ػ� ���
	SLGE_CANCLE_DISBAND,			///< �ػ� ���
	SLGE_DISBAND,					///< ��� �ػ�

	SLGE_MEM_POS,					///< �ɹ� ��ġ ����
	SLGE_MEM_CLASS,					///< �ɹ� Ŭ���� ����
	SLGE_MEM_LEVEL,					///< �ɹ� ���� ����
	SLGE_MEM_REPUTATION,			///< �ɹ� ���� ����
	SLGE_MEM_NAME,					///< 

	SLGE_MEM_ONLINE,				///< �ɹ� ���� ����
	SLGE_MEM_OFFLINE,				///< �ɹ� ���� ����

	SLGE_CHANGE_GUILD_NAME,			///< ����� �̸��� �ٲ����
	SLGE_CHANGE_GUILD_EMBLEM,		///< ����� ������ �ٲ����
	SLGE_CHANGE_FUNCTION,			///< ��� ��� ����
	SLGE_CHANGE_REPUTATION,			///< ��� ���� ����
	SLGE_CHANGE_INVEST_ZENNY,		///< ��带 ���� ���ϸ� �����Ͽ���

	SLGE_NOTICE,					///< ��� ���� ����
	SLGE_DOGI,						///< ���� ���� ����
	SLGE_DOGI_NFY,					///< ���� ���� ����� ������
};

struct SNtlEventGuild
{
	RwInt32			iMessage;
	RwUInt32		uiParam;
	void*			pData;
	void*			pExData;
};

struct SNtlEventNotifyGuild
{
	RwInt32			iMessage;
	RwUInt32		uiParam;
};

struct SNtlEventChangleGuildName : public SNtlEventSob
{
	WCHAR*			pwcGuildName;
};

struct SNtlEventChangleGuildEmblem
{
	SERIAL_HANDLE	hHandle;
	RwUInt8			byTypeA;
	RwUInt8			byTypeAColor;
	RwUInt8			byTypeB;
	RwUInt8			byTypeBColor;
	RwUInt8			byTypeC;
	RwUInt8			byTypeCColor;
};

enum eDojoEventType
{
	DOJO_EVENT_DOJO_INFO,							///< ���� �⺻ ������ �޾Ҵ�
	DOJO_EVENT_CHANGE_LEVEL,						///< ������ ���� ����
	DOJO_EVENT_CHANGE_TENKAI_SEED_PLAYER,			///< ������ ������ ������ õ������ ����ȸ �õ� ������ ����
	DOJO_EVENT_CHANGE_DOJO_NOTICE,					///< ���� ����
	DOJO_EVENT_NPC_GUI_INFO,						///< GUI ǥ�� ����
	DOJO_EVENT_LOST_DOJO,							///< ������ �������� �Ҿ���

	DOJO_EVENT_SCRAMBLE_ACCEPT,						///< ���� ��Ż�� ����
	DOJO_EVENT_SCRAMBLE_TELEPORT_PROPOSAL,			///< ���� ��Ż�� �ڷ���Ʈ�� �� �� �ִٴ� ������ �˸�

	DOJO_EVENT_SEAL_ATTACK_STATE,					///< ���� ��Ż���� ���� ��ġ, ���� ����
	DOJO_EVENT_SEAL_ATTACK_BEGIN,					///< ���� ��Ż���� ������ ���� ���� ĳ������ �����Ѵ�
	DOJO_EVENT_SEAL_ATTACK_END,						///< ���� ��Ż���� ������ ���� ���� ĳ������ �����Ѵ�
	DOJO_EVENT_SCRAMBLE_SCORE,						///< ���� ��Ż�� ����
	DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER,			///< ���� ���� ���� ���� ����

	DOJO_EVENT_SCRAMBLE_RESULT,						///< ���� ��Ż�� ���
	DOJO_EVENT_SCRAMBLE_REWARD,						///< ���� ��Ż�� ����
};

struct SNtlEventDojo
{
	RwUInt8			byDojoEvent;
	RwUInt32		uiParam;
	VOID*			pExData;
	VOID*			pExData2;
	VOID*			pExData3;
};

struct SNtlEventDojoMessage
{
	RwUInt32		uiDojoTableIndex;
	RwUInt8			byDojoState;
	RwUInt64		ui64time;
};

struct SNtlEventScrambleDefaultInfo
{
	RwUInt32		uiDojoTableIndex;
	RwUInt32		uiDefenceGuildID;
	RwUInt32		uiOffenceGuildID;
	WCHAR*			pwcDefenceGuildName;
	WCHAR*			pwcOffenceGuildName;
	VOID*			pDefenceGuildEmblem;
	VOID*			pOffenceGuildEmblem;
};

enum eGuildWarehouseEventType
{
	GUILD_WAREHOUSE_EVENT_START,
	GUILD_WAREHOUSE_EVENT_SET_ZENNY,
	GUILD_WAREHOUSE_EVENT_ADD_ZENNY,
	GUILD_WAREHOUSE_EVENT_SUB_ZENNY,
	GUILD_WAREHOUSE_EVENT_SET_ITEM,
	GUILD_WAREHOUSE_EVENT_END,
};

struct SNtlGuildWarehouse
{
	RwUInt8			byMessage;
	RwUInt32		uiParam;
	RwUInt32		uiParam2;
};

typedef SNtlGuildWarehouse SNtlGuildWarehouseNotify;

struct SNtlGuildWarehouseItemInfo
{
	RwUInt8			byPlace;
	RwUInt8			byItemCount;
	VOID*			pData;
};

struct SNtlGuildWarehouseItemMove
{
	SERIAL_HANDLE	hSrcItem;
	SERIAL_HANDLE	hDestItem;
	RwUInt8			bySrcPlace;
	RwUInt8			bySrcPos;
	RwUInt8			byDestPlace;
	RwUInt8			byDestPos;
};

struct SNtlGuildWarehouseItemStackMove
{
	SERIAL_HANDLE	hSrcItem;
	SERIAL_HANDLE	hDestItem;
	RwUInt8			bySrcPlace;
	RwUInt8			bySrcPos;
	RwUInt8			byDestPlace;
	RwUInt8			byDestPos;
	RwUInt8			bySrcStackCount;
	RwUInt8			byDestStackCount;
};

struct SNtlGuildWarehouseZennuInfo
{
	RwUInt32		uiZenny;
};

//-----------------------------
// OtherParam Evect
//_____________________________

struct SNtlEventHelpHint
{
	RwUInt32		uiHelpHint;
};

enum eTMQEventType
{
	TBT_COUNTDOWN,					///< TMQ Play time ��� ���� ����
	TET_REWARD,						///< TMQ ����
	TET_INFOMATION_NFY,				///< ���� Ÿ�� eTIMEQUEST_INFORMATION_TYPE
	TET_COUPON_COUNT,				///< TMQ ���� ����
	TET_TMQ_POINT,					///< TMQ ����Ʈ
	TET_GAME_TIME,					///< TMQ ���� �ð�
	TET_CLEAR_TIME,
	TET_BONUS_TIME,
	TET_DAY_RECORD,
	TET_IS_BEST_RECORD,
	TET_TIME_UNTIL_TELEPORT_TO_MAINWORLD,
};

struct SNtlEventTMQ
{
	RwUInt32		uiEventType;
	SERIAL_HANDLE	hTargetHandle;
	RwUInt32		uiValue;
	void*			pExData;
};

struct SNtlEventTMQNotify
{
	RwUInt32		uiEventType;
	RwUInt32		uiValue;
};

//////////////////////////////////////////////
// Rank Battle ���� 
//struct SNtlEventRBBoardUpdate
//{
//	RwUInt32		tblidx;
//	RwUInt16		wTotalPage;
//	RwUInt16		wCurPage;
//	RwUInt8			byRoomCount;
//	RwUInt8			byRoomType;
//	void*			pRoomList;
//};
//
//struct SNtlEventRBRoomInfo
//{
//	RwUInt8			byRoomMode;
//	void*			pRoomInfo;
//};
//
//struct SNtlEventRBRoom
//{
//	enum TYPE { ROOM_CREATE, ROOM_LEAVE, ROOM_CHALLENGE, ROOM_JOIN_CHALLENGE, ROOM_JOIN_OWNER };
//
//	RwInt32			type;
//	RwUInt32		tblidx;
//	RwUInt32		roomidx;
//};
//
//struct SNtlEventRBMatch
//{
//	enum TYPE { MATCH_READY, MATCH_START, MATCH_CANCEL, MATCH_CANCELED };
//
//	RwInt32			type;
//	RwUInt8			byDataCount;
//	void*			pReadyData;
//};

struct SNtlEventRBBoardInfo
{
	RwUInt32				uiRemainTime;		// second
	RwUInt8					byBattleMode;
	RwUInt8					byArenaCount;	
	sRANKBATTLE_ARENA_INFO*	pArenaInfo;
};

struct SNtlEventRBJoin
{
	enum TYPE { STATE_JOIN, STATE_JOINFAIL, STATE_LEAVE, STATE_CANCELED, STATE_START };

	RwInt32	eType;

	union
	{
		// Join
		struct  
		{
			RwBool					bRes;			// TRUE : RES, FALSE : NFY
			RwUInt32				hBoardObject;
			RwUInt32				tblidx;
			RwUInt32				uiRemainTime;	// second
			RwUInt8					byArenaCount;
			sRANKBATTLE_ARENA_INFO*	pArenaInfo;
		};

		// Start
		struct 
		{
			RwUInt16				wResultCode;
		};		
	};	
};

struct SNtlEventRBBattleStateUpdate
{
	RwUInt8			byBattleState;
	RwUInt8			byStage;
};

struct SNtlEventRBBattleTeamInfo
{
	RwUInt16		wStraightWinCount;
	RwUInt16		wPerfectWinCount;
	RwUInt8			byCount;
	void*			pMemberList;
	WCHAR			wcOwnerPartyName[NTL_MAX_SIZE_PARTY_NAME + 1];
	WCHAR			wcChallengerPartyName[NTL_MAX_SIZE_PARTY_NAME + 1];
};

struct SNtlEventRBBattlePlayerState
{
	RwUInt32		hSerial;
	RwUInt8			byState;
};

struct SNtlEventRBBattleKillScoreUpdate
{
	RwUInt8			byOwnerKillScore;
	RwUInt8			byChallengerKillScore;
};

struct SNtlEventRBBattleStageFinish
{
	RwUInt8			byBattleResult;
	void*			pStageScoreData;
};

struct SNtlEventRBBattleMatchFinish
{
	void*			pMatchResult;
};

struct SNtlEventRBBattleEnd
{
	RwUInt8			byWinner;
};

struct SNtlEventRBTotalScoreUpdate
{
	RwUInt8			byBattleMode;
	void*			pTotalScoreData;
};

//////////////////////////////////////////////
/**
* ���� ������.
*/

struct SNtlEventSobGetState : public SNtlEventSob
{
	RwUInt8			byServerState;
	RwUInt8			byClientState;
	RwChar*			chClientState;
};

struct SNtlEventSobGetFightingMode : public SNtlEventSob
{
	RwUInt8			bFightingMode;
};

struct SNtlEventSobGetAirMode : public SNtlEventSob
{
	RwUInt8			bAirMode;
};

//////////////////////////////////////////////
/**
* trigger event
*/

struct sNtlEventReloadTS
{
};

struct SNtlEventRegQuest_Nfy
{
	sTS_KEY				sTSKey;					// TS Ű

	bool				bNewRegister;			// ����Ʈ ��Ͻ� New type�� ��� ����
	bool				bQuestShare;			// ����Ʈ ����
	RwUInt32			uiQuestAreaName;		// ����Ʈ ���� �̸�
	RwUInt32			uiQuestState;			// ����Ʈ ����
	RwUInt32			uiQuestTitle;			// ����Ʈ ����
	RwUInt32			uiQuestGoal;			// ����Ʈ ����
	eQUEST_SORT_TYPE	eQuestSortType;			// ����Ʈ ������ Ÿ��
};

struct SNtlEventUnregQuest_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventShowQuestWindow
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventShowQuestWindow_Nfy
{
	sTS_KEY				sTSKey;					// TS Ű

	RwUInt32			uiQuestAreaName;		// ����Ʈ ���� �̸�
	RwUInt32			uiQuestTitle;			// ����Ʈ ����
	RwUInt32			uiQuestSort;			// ����Ʈ ����
	RwUInt32			uiQuestGrade;			// ����Ʈ ���̵�
	eGRADE_TYPE			eGradeType;				// ����Ʈ ���̵� Ÿ��
	RwUInt32			uiQuestGoal;			// ����Ʈ ��ǥ
	RwUInt32			uiQuestContents;		// ����Ʈ ����
	sREWARD_INFO		sDefaultReward[MAX_DEFAULT_REWARD];		// ����Ʈ ���� ���� - �⺻ ����
	sREWARD_INFO		sSelectReward[MAX_SELECTION_REWARD];		// ����Ʈ ���� ���� - ���� ����
	unsigned int			uiRewardExp;
	unsigned int			uiRewardZeni;
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;			// ���� �̺�Ʈ ���� - ������ Ÿ��
	uSTOC_EVT_DATA		uEvtInfoData;			// ���� �̺�Ʈ ���� - ������
	RwUInt32			uiTimeLimit;			// Ÿ�� ���� ����
	RwUInt32			uiEventType;			// eEVENT_GEN_TYPE ( ����� eEVENT_GEN_TYPE_CLICK_NPC �� ����� )
	RwUInt32			uiEventID;				// uiEventType �� ���� �ε��� eEVENT_GEN_TYPE_CLICK_NPC	: Mob table index
	RwBool				bIsEventStarter;		// Event type [ True : Starter, False : Reward ]
};

struct SNtlEventUpdateQuestState_Nfy
{
	sTS_KEY				sTSKey;					// TS Ű

	RwBool				bOutStateMsg;			// ����Ʈ ���� �޽��� ��� ����
	RwUInt32			uiUpdatedQuestFlag;		// ���ŵ� ����Ʈ ���� �÷���
	RwUInt32			uiQuestState;			// ����Ʈ ����

	RwUInt32			uiQuestTitle;			// ����Ʈ ����
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;			// ���� �̺�Ʈ ���� - ������ Ÿ��
	uSTOC_EVT_DATA		uEvtInfoData;			// ���� �̺�Ʈ ���� - ������

	RwUInt32			uiEventType;			// eEVENT_GEN_TYPE ( ����� eEVENT_GEN_TYPE_CLICK_NPC �� ����� )
	RwUInt32			uiEventID;				// uiEventType �� ���� �ε��� eEVENT_GEN_TYPE_CLICK_NPC	: Mob table index
	RwBool				bIsEventStarter;		// Event type [ True : Starter, False : Reward ]
};

struct SNtlEventUpdateQuestProgressInfo_Nfy
{
	sTS_KEY				sTSKey;				// TS Ű

	eSTOC_EVT_DATA_TYPE	eEvtInfoType;		// ���� �̺�Ʈ ���� - ������ Ÿ��
	uSTOC_EVT_DATA		uEvtInfoData;		// ���� �̺�Ʈ ���� - ������
	RwUInt32			uiTimeLimit;		// Ÿ�� ���� ����
};

struct SNtlEventGiveUpQuest
{
	sTS_KEY sTSKey;							// TS Ű
};

struct SNtlEventShowIndicator
{
	sTS_KEY			sTSKey;					// TS Ű	
	RwBool			bAuto;
};

struct SNtlEventHideIndicator
{
	sTS_KEY			sTSKey;					// UI������ ���.
};

struct SNtlEventShowIndicator_Nfy
{
	sTS_KEY				sTSKey;				// TS Ű

	RwUInt32			uiQState;			// ����Ʈ ����
	RwUInt32			uiQuestTitle;		// ����Ʈ ����
	RwUInt32			uiQuestGoal;		// ����Ʈ ����
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;		// ���� �̺�Ʈ ���� - ������ Ÿ��
	uSTOC_EVT_DATA		uEvtInfoData;		// ���� �̺�Ʈ ���� - ������
	RwUInt32			uiTimeLimit;		// Ÿ�� ���� ����
	RwBool				bAuto;
};

struct SNtlEventDoQuest
{
	sQUEST_INFO			sQuestInfo;				// Quest ����
	eEVENT_GEN_TYPE		eEventGenType;			// Event ����
	RwUInt32			uiOwnerID;				// Owner ID
	RwBool				bTransCameraCtrlRight;	// ī�޶� ������� �ѱ�� ��� TRUE
};

struct SNtlEventQuestProposalDialog_Req
{
	sTS_KEY			sTSKey;						// TS Ű

	RwBool			bDisplaySummary;			// ����Ʈ ��� ���� �̸����� �����ֱ� ����
	RwUInt32		uiQuestTitle;				// ����Ʈ ����
	RwUInt32		uiQuestGoal;				// ����Ʈ ��ǥ
	RwUInt32		uiQuestGrade;				// ����Ʈ ���̵�
	eGRADE_TYPE		eGradeType;					// ����Ʈ ���̵� Ÿ��
	RwUInt32		uiQuestSort;				// ����Ʈ ����
	RwUInt32		uiQuestContents;			// ����Ʈ ����
	sREWARD_INFO	sDefaultReward[MAX_DEFAULT_REWARD];			// ����Ʈ ���� ���� - �⺻ ����
	sREWARD_INFO	sSelectReward[MAX_SELECTION_REWARD];			// ����Ʈ ���� ���� - ���� ����
	unsigned int			uiRewardExp;
	unsigned int			uiRewardZeni;
};

struct SNtlEventQuestProposalDialog_Res
{
	bool			bResult;					// ����Ʈ ����(true) �Ǵ� ����(false)

	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventQuestUserSelectDialog_Req
{
	sTS_KEY			sTSKey;						// TS Ű

	RwUInt32		uiTargetType;				// Ÿ���� ���� ( eUSER_SEL_TARGET_TYPE -> eUSER_SEL_TARGET_TYPE_NPC, eUSER_SEL_TARGET_TYPE_OBJECT, eUSER_SEL_TARGET_TYPE_INVALID )
	RwUInt32		uiTargetTblIdx;				// Ÿ���� ���̺� �ε��� 
	RwUInt32		uiQuestTitle;				// ����Ʈ ����
	RwUInt32		uiConv;						// ��ȭ
	RwInt32			nBranchCnt;					// ���� ������� Branch�� ����
	sTS_BRANCH_INFO asBranchInfo[eBRANCH_INFO_MAX];	// Branch info
};

struct SNtlEventQuestUserSelectDialog_Res
{
	bool			bResult;					// Ok(true) �Ǵ� Cancel(false)

	sTS_KEY			sTSKey;						// TS Ű
	NTL_TS_TC_ID	tcSelID;					// ���õ� ����Ʈ ���̵�
};

struct SNtlEventQuestNarrationUserSelectDialog_Req
{
	sTS_KEY						sTSKey;							// TS Ű

	eUSER_SEL_PROGRESS_STATE	eProgState;						// Narration ���� ����
	RwUInt32					uiConv;							// ��ȭ
	RwInt32						nBranchCnt;						// ���� ������� Branch�� ����
	sTS_BRANCH_INFO				asBranchInfo[eBRANCH_INFO_MAX];	// Branch info
};

struct SNtlEventQuestNarrationUserSelectDialog_Res
{
	bool			bResult;					// Ok(true) �Ǵ� Cancel(false)

	sTS_KEY			sTSKey;						// TS Ű
	NTL_TS_TC_ID	tcSelID;					// ���õ� ����Ʈ ���̵�
};

struct SNtlEventTSFinished
{
	unsigned char	byTSType;					// Trigger type ( TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS )
	NTL_TS_T_ID		tID;						// ����Ǵ� Ʈ���� ���̵�
};

struct SNtlEventTSSkipContainer
{
	NTL_TS_T_ID		tID;
	NTL_TS_TC_ID	tcID;
};

struct SNtlEventQuestRewardDialog_Req
{
	sTS_KEY			sTSKey;						// TS Ű

	eREWARD_CONTAINER_TYPE eRewardType;			// ���� Ÿ��
	RwUInt32		uiLimitTime;				// ���� ���� �ð�
	RwUInt32		uiQuestTitle;				// ����Ʈ ����
	RwUInt32		uiQuestGoal;				// ����Ʈ ��ǥ
	RwUInt32		uiQuestGrade;				// ����Ʈ ���̵�
	eGRADE_TYPE		eGradeType;					// ����Ʈ ���̵� Ÿ��
	RwUInt32		uiQuestSort;				// ����Ʈ ����
	RwUInt32		uiQuestContents;			// ����Ʈ ����
	sREWARD_INFO	sDefaultReward[MAX_DEFAULT_REWARD];	// ����Ʈ ���� ���� - �⺻ ����
	sREWARD_INFO	sSelectReward[MAX_SELECTION_REWARD];// ����Ʈ ���� ���� - ���� ����
	unsigned int		uiRewardExp;
	unsigned int		uiRewardZeni;
};

struct SNtlEventQuestRewardDialog_Res
{
	bool			bResult;					// Ok(true) �Ǵ� Cancel(false)

	sTS_KEY			sTSKey;						// TS Ű
	int				nSelRwdIdx;					// ���� ������ ���õ� �ε���
};

struct SNtlEventQuestNarrationDialog_Req
{
	sTS_KEY						sTSKey;			// TS Ű

	eNARRATION_PROGRESS_STATE	eProgState;		// Narration ���� ����
	eNARRATION_OWNER_TYPE		eOwnerType;		// Owner�� ����
	RwUInt32					uiOwnerIdx;		// Owner�� ���̺� �ε���
	eNARRATION_OWNER_STATE		eOwnerState;	// Owner�� ����
	eNARRATION_DIALOG_DIR_TYPE	eDialogDirType;	// ��ȭ ���� ����
	RwUInt32					uiDialog;		// ��ȭ ����
	eNARRATION_GUI_TYPE			eGUIType;		// GUI ����
	RwUInt32					uiMaxLifeTime;	// �� �ð� �̻� �����̼� â�� ���� �Ǵ� ��� �ڵ����� Next�� �Ѿ�� �Ѵ�.
};

struct SNtlEventQuestNarrationDialog_Res
{
	bool			bResult;					// Ok(true) �Ǵ� Cancel(false)

	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventQuestNPCDialog_Req
{
	sTS_KEY			sTSKey;						// TS Ű

	eNPCCONV_TYPE	eNPCConvType;				// NPC ��ȭ Ÿ��
	unsigned int	uiNPCIdx;					// NPC table index
	RwUInt32		uiDesc;						// ��ȭ
};

struct SNtlEventQuestNPCDialog_Res
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventQuestShare_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventQuestAcceptProposal_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventQuestAcceptReward_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	unsigned int	uiQTitle;					// Quest ����
	RwBool			bOutStateMsg;				// ����Ʈ ���� �޽��� ��� ����
};

struct SNtlEventQuestAcceptGiveUp_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	unsigned int	uiQTitle;					// Quest ����
};

struct SNtlEventQuestObjDialog_Req
{
	sTS_KEY			sTSKey;						// TS Ű

	eOBJCONV_TYPE	eObjConvType;				// Object ��ȭ Ÿ��
	RwUInt32		uiWorldIdx;					// Object �Ҽ� ����
	RwUInt32		uiObjIdx;					// Object table index
	RwUInt32		uiDesc;						// ��ȭ
};

struct SNtlEventQuestObjDialog_Res
{
	sTS_KEY			sTSKey;						// TS Ű
};

struct SNtlEventShowTutorialHint_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	bool			bShow;						// Ʃ�丮�� ��Ʈâ ���� �ݱ�
	TBLIDX			tblidx;						// Guide hint table index
};

struct SNtlEventShowHelp_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	bool			bShow;						// ����â ���� �ݱ�
};

struct SNtlEventShowInventory_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	bool			bShow;						// �κ��丮â ���� �ݱ�
};

struct SNtlEventShowSkill_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	bool			bShow;						// ��ųâ ���� �ݱ�
};

struct SNtlEventShowCharInfo_Nfy
{
	sTS_KEY			sTSKey;						// TS Ű
	bool			bShow;						// ĳ���� ����â ���� �ݱ�
};

enum eTutorialNfyType
{
	TUTORIAL_HINT,								// Ʃ�丮�� ��Ʈ
	TUTORIAL_HELP_DIALOG_SWITCH,				// Ʃ�丮�� ���� ���̾�α� ���� �ݱ�
	TUTORIAL_BAG_SWITCH,						// ���� ���� �ݱ�
	TUTORIAL_SKILL_DIALOG_SWITCH,				// ��ų ���̾�α� ���� �ݱ�
	TUTORIAL_CHARINFO_DIALOG_SWITCH,			// ĳ���� ���� ���̾�α� ���� �ݱ�
};

struct SNtlEventTutorialNfy
{
	RwUInt32		uiMessage;
	RwBool			bShow;
	RwUInt32		uiParam;
};


struct SNtlEventTSRemovingTMQQuest_Nfy
{
	NTL_TS_T_ID		tRmvTSId;
};

struct SNtlEventTSPCDialog_Nfy
{
	RwUInt32		uiMessage;
};


struct SNtlEventTSMudosaTeleport_Req
{
	sTS_KEY			sTSKey;
};


struct SNtlEventTSMudosaTeleport_Res
{
	sTS_KEY			sTSKey;
	RwUInt8			byMudosaIndex;
	RwBool			bCancel;
};


// Client => TS


struct SNtlEventTSItemUse
{
	void*			pItemAction;
};

struct SNtlEventTSItemGet
{
	RwUInt32		uiItemIdx;
};

struct SNtlEventTSItemEquip
{
	RwUInt32		uiItemIdx;
};

struct SNtlEventTSScoutUse
{
	RwUInt32		uiItemIdx;
	RwUInt32		uiTargetClassID;
	RwUInt32		uiTargetIdx;
};

struct SNtlEventTSSkillUse
{
	RwUInt32		uiSkillType;
	RwUInt32		uiSkillIdx;
	RwUInt8			byRpBonusType;
	SERIAL_HANDLE	arhTargetHandle[NTL_MAX_NUMBER_OF_SKILL_TARGET];
};

struct SNtlEventTSRankBattle
{
	RwUInt32		uiRBTblIdx;
	eRB_COND_FLAG	eRBCondFlag;
};

struct SNtlEventTSColTriggerObject
{
	SERIAL_HANDLE	hTriggerObject;
};

struct SNtlEventTSBindStone
{
};

struct SNtlEventTSSearchQuest
{
};

struct SNtlEventTSItemUpgrade
{
};

struct SNtlEventTSTeleport
{
};

struct SNtlEventTSBudokai
{
};

struct SNtlEventTSSlotMachine
{
};

struct SNtlEventTSHoipoiMix
{
};

struct SNtlEventTSPrivateShop
{
};

struct SNtlEventTSFreeBattle
{
};

struct SNtlEventTSItemIdentity
{
};

struct SNtlEventTSUseMail
{
};

struct SNtlEventTSParty
{
};

struct SNtlEventTSClickSideIcon
{
};

struct SNtlEventTSLevelCheck
{
};

struct SNtlEventTSQuest
{
};

struct SNtlEventTSDialogOpen
{
};

//////////////////////////////////////////////
/**
* ����Ʈ ��ġ ��ũ
*/
//////////////////////////////////////////////

struct SNtlEventRegisterQuestPosMark
{
	QM_KEY				QMKey;						// Quest mark key - Key �� �ߺ� ������

	eQMI_TARGET_TYPE	eTargetType;				// Ÿ�� ��ũ ����
	unsigned int		uiTargetWorldTblIdx;		// Ÿ���� �Ҽӵ� ���� ���̺� �ε���
	unsigned int		uiTargetTableIdx;			// Ÿ���� ���̺� �ε���
	float				fTargetPosX;				// Ÿ���� ��ġ ( x, y, z )
	float				fTargetPosY;
	float				fTargetPosZ;
	unsigned int		uiTooltipTblIdx;			// ������ ����� ���̺� �ε���
};

struct SNtlEventUnregisterQuestPosMark
{
	QM_KEY			QMKey;						// Quest mark key - Key �� �ߺ� �����ϹǷ� �ش� Key ���� �ش��ϴ� ������ �� ���� ��� ��
};

struct SNtlEventShowQuestPosMark
{
	sTS_KEY			sTSKey;
	RwBool			bShow;
};

//////////////////////////////////////////////
/**
* dbo ���� �ý���(UI, Camera, Flash)�� trigger event�� forward ��Ų��.
*/

struct SNtlEventQuestDirect_Forward
{
	CNtlTCUnit			*pTCUnit;				// �� ������ �����ϰ� �ִٰ� Echo �����־�� �Ѵ�.
	ETriggerDirectType	eTDType;			// ���� type
	union
	{
		char chData[512];
		SNtlEventQuestProposalDialog_Req			sProposal;
		SNtlEventQuestUserSelectDialog_Req			sUserSelect;
		SNtlEventQuestRewardDialog_Req				sReward;
		SNtlEventQuestNarrationDialog_Req			sNarration;
		SNtlEventQuestNarrationUserSelectDialog_Req sNarrationUserSelect;
	};
};

struct SNtlEventQuestDirect_Echo
{
	CNtlTCUnit			*pTCUnit;				// SNtlEventQuestDirect_Forward ����ü�� pTCUnit ���� �״�� echo �����ش�.
	ETriggerDirectType	eTDType;			// ���� type
	union
	{
		char chData[512];
		SNtlEventQuestProposalDialog_Res			sProposal;
		SNtlEventQuestUserSelectDialog_Res			sUserSelect;
		SNtlEventQuestRewardDialog_Res				sReward;
		SNtlEventQuestNarrationDialog_Res			sNarration;
		SNtlEventQuestNarrationUserSelectDialog_Res	sNarrationUserSelect;
	};
};

struct SNtlEventQuestMark : public SNtlEventSob
{
	RwUInt8			byMarkType;
	void*			pQuestInfo;	
};


//////////////////////////////////////////////
/**
* Trigger object
*/

struct SNtlEventTObjectUpdateState : public SNtlEventSob
{
	TBLIDX			tblIdx;
	RwUInt8			byMainState;
	RwUInt8			bySubState;
	RwUInt32		uiStateTime;
};


//////////////////////////////////////////////
/**
* Ʈ���� ������Ʈ ��ī���� ���� ����
*/
//////////////////////////////////////////////

struct SNtlEventTrigObjScouterDir_WearScouterItem
{
	SERIAL_HANDLE	hSerial;
};

struct SNtlEventTrigObjScouterDir_TakeOffScouterItem
{
	SERIAL_HANDLE	hSerial;
};

// �巡�ﺼ �÷��� ����
struct SNtlEventDBCNarration
{
	enum State
	{
		DBC_NARRATION_START,
		DBC_NARRATION_PROGRESS,
		DBC_NARRATION_END,
		DBC_NARRATION_END_ATONCE,
		DBC_NARRATION_INVALID = -1
	};

	const char*		pszTextureName;
	const WCHAR*	pwszText;
	RwInt32			eNarrationState;
};

// ������ ���� ������ ���� ����ü (����)
struct SNtlEventDBC_Res
{
	RwUInt32		uiAltarID;
	WORD			wResultCode;
};

// �巡�ﺼ ���� ������ ��Ŷ ����ü
struct SNtlEventDBCSchedule_Info
{
	bool						bIsAlive;			// �Ⱓ ���� false: �Ⱓ�� �ƴϴ�
	BYTE						byEventType;		// eSCHEDULE_EVENT_TYPE
	__int64						nStartTime;			// ���ʱⰣ ����
	__int64						nEndTime;           // ������ �Ⱓ ������ �ð�
	DWORD						dwMainTerm;			// �Ⱓ ���� (���� ������ �������� ��) 
	DWORD						dwSubTerm;			// �Ⱓ ���� (���� ������ �������� ��) 
	BYTE						byTermType;			// 0: �� 1: �� 2: ��
};

// SideIcon : HintIcon
enum eSideHintIconEventType
{
	HINTICON_HELP,
	HINTICON_GUIDE,
};

struct SNtlEventRegSideHintIcon
{
	RwUInt32		uiEventType;
	RwUInt32		uiTableIndex;
};

//////////////////////////////////////////////////////////////////////////////////
// Private Shop
//////////////////////////////////////////////////////////////////////////////////

// �Ǹ��� �̺�Ʈ Ÿ��
enum ePrivateShopStateEventType
{
	PRIVATESHOP_EVENT_CREATE,
	PRIVATESHOP_EVENT_EXIT,
	PRIVATESHOP_EVENT_OPEN,
	PRIVATESHOP_EVENT_CLOSE,
};

// ������ �̺�Ʈ Ÿ��
enum ePrivateShopStateVisitorEventType
{
	PRIVATESHOP_EVENT_ENTER,
	PRIVATESHOP_EVENT_LEAVE,
};

enum ePrivateShopItemEventType
{
	PRIVATESHOP_EVENT_ITEM_INSERT,
	PRIVATESHOP_EVENT_ITEM_UPDATE,
	PRIVATESHOP_EVENT_ITEM_DELETE,
};

// ���λ���
struct SNtlEventPrivateShopState
{
	RwUInt32				uiEventType;		// ePrivateShopStateEventType
	RwUInt32				uiPrivateShopState;	// ePRIVATESHOP_STATE
	RwBool					IsAvatarPrivateShop;
	sPRIVATESHOP_SHOP_DATA*	pPrivateShopData;
};

struct SNtlEventPrivateShopStateVisitor
{
	RwUInt32				uiEventType;		// ePrivateShopStateVisitorEventType 
	sPRIVATESHOP_SHOP_DATA*	pPrivateShopData;
};


struct SNtlEventPrivateShopItem
{
	RwUInt32				uiEventType;		// ePrivateShopItemEventType
	sPRIVATESHOP_ITEM_DATA	sPrivateShopItemData;
};

struct SNtlEventPrivateShopItemDataInfo
{
	SERIAL_HANDLE hOwnerSerial;
	sPRIVATESHOP_ITEM_DATA	sPrivateShopItemData;
};

struct SNtlEventPrivateShopItemSelect
{
	RwUInt8			uiPrivateShopPos;
	RwUInt8			uiItemState;
};

struct SNtlEventPrivateShopItemState
{
	RwUInt8			auiItemState[NTL_MAX_PRIVATESHOP_INVENTORY];
	RwInt32			iStateCount;
};

struct SNtlEventPrivateShopItemBuying
{
	SERIAL_HANDLE				hOwnerSerial;
	RwBool						bIsBuyAvatar;
	RwUInt8						uiBuyCount;
	sPRIVATESHOP_ITEM_DATA		asPrivateShopItemData[NTL_MAX_BUY_SHOPPING_CART];
};

struct SNtlEventWorldConceptState
{
	EWorldPlayConcept	eWorldConcept;
	RwUInt32	uiState;
};

struct SNtlEventTMQServerState
{
	EWorldPlayConcept	eWorldConcept;
	RwUInt32	uiState;
};

struct SNtlEventMailStart
{
	SERIAL_HANDLE	hSerial;
	RwBool			bIsAway;
	RwBool			bSucceeded;
};

struct SNtlEventMailLoadInfo
{
	sMAIL_PROFILE	sMailProfile;
};

struct SNtlEventMailLoadData
{
	sMAIL_PROFILE	sMailProfile;
};

struct SNtlEventMailSend
{
	SERIAL_HANDLE	hSerialID;
	WCHAR*			pwcTargetName;
};

struct SNtlEventMailRead
{
	SERIAL_HANDLE	hSerialID;
	MAILID			mailID;
	RwUInt8			uiExpiredDay;
	DBOTIME			endTime;
};

struct SNtlEventMailDel
{
	SERIAL_HANDLE	hSerialID;
	MAILID			mailID;
};

struct SNtlEventMailReturn
{
	SERIAL_HANDLE	hSerialID;
	MAILID			mailID;
};

struct SNtlEventMailReload
{
	SERIAL_HANDLE	hSerialID;
	MAILID*			pmailID;
	RwUInt8			uiMailCount;
	RwUInt8			uiUnreadMailCountNormal;
	RwUInt8			uiUnreadMailCountManager;
};

struct SNtlEventMailReloadNfy
{
	MAILID*			pmailID;
	RwUInt8			uiMailCount;
	RwUInt8			uiUnreadMailCountNormal;
	RwUInt8			uiUnreadMailCountManager;
};

struct SNtlEventMailLoad
{
	SERIAL_HANDLE	hSerialID;
};

struct SNtlEventMailItemReceive
{
	SERIAL_HANDLE	hSerialID;
	MAILID			mailID;
};

struct SNtlEventMailLock
{
	SERIAL_HANDLE	hSerialID;
	MAILID			mailID;
	RwBool			bIsLock;
	RwUInt8			uiExpiredDay;
	DBOTIME			endTime;
};

struct SNtlEventMailSystemNfy
{
	RwUInt32		uiNotifyType;
	RwUInt32		uiNotifyValue;	
};

struct SNtlEventMailMultiDelRes
{
	SERIAL_HANDLE	hObject;
	RwUInt8			byCount;
	MAILID			aMailID[NTL_MAX_COUNT_OF_MULTI_DEL];
};

struct SNtlEventAuctionHouseListRes
{
	RwUInt32 uiPage;
	RwUInt32 uiMaxPage;
	BYTE byListNum;
	BYTE byPage;
};

struct SNtlEventAuctionHouseSellRes
{
	BYTE byPlace;
	BYTE byPosition;
	BYTE byCount;
};

struct SNtlEventAuctionHouseSellCancelRes
{
	ITEMID nItem;
};

struct SNtlEventAuctionHouseBuyRes
{
	ITEMID nItem;
};

struct SNtlEventAuctionHouseListData
{
	void*			pItemData;
};

struct SNtlEventCharAway
{
	RwBool			bIsAway;
};

struct SNtlEventPrivateShopNameTag
{
	SERIAL_HANDLE	hOwnerSerial;
	RwBool			bIsOwnerEmpty;
	RwUInt32		uiPrivateShopState;
	WCHAR*			pwcPrivateShopName;
	//	WCHAR			awcPrivateShopName[NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1];
};

// Tool Tip UI�� ǥ���Ѵ�.
struct SNtlEventToolTip
{
	RwBool          bShow;      ///< ���� ǥ�� ����
	RwInt32         nX;         ///< ǥ���� X ��ǥ
	RwInt32         nY;         ///< ǥ���� Y ��ǥ
	std::wstring    wstrText;    ///< ǥ���� �ؽ�Ʈ
};

struct SNtlEventBind
{
	RwUInt8			byBindType;
	WORLDID			BindedWorldID;
	RwUInt32		uiBindObjectTblIdx;
};

// ActionMap
struct SNtlEventActionMapLoadInfo
{
	BYTE			byCount;
	sSHORTCUT_DATA	asData[NTL_SHORTCUT_MAX_COUNT];
};

struct SNtlEventActionMapUpdateRes
{
	RwBool bSuccess;
};

/**
* \brief �׼Ǹ� ���� ���� ( Client �ܰ��� ��� )
*/
struct SNtlEventActionMapClientNotify
{
	enum eTYPE 
	{ 
		ACTIONMAP_OK = 0,				///< �׼Ǹ��� ���������� ������
		ACTIONMAP_RELEASE,				///< Ű�� ������ �׼��� ����
		ACTIONMAP_FIXEDKEY,				///< ���� Ű�� ������ ���Ƿ� ����� �� ���� Ű�Դϴ�.
		ACTIONMAP_NOTCOMBINE,			///< ����Ű�� ������� ���ϴ� �׼��Դϴ�.
		ACTIONMAP_SAMEKEY				///< Ű�� �ߺ��Ǿ� �ֽ��ϴ�.
	};

	RwUInt8		byType;
	RwUInt16	wParam1;
	RwUInt16	wParam2;
};

struct SNtlEventPortalInfoRes
{
	BYTE           byCount;         ///< ��Ż�� ����
	BYTE*          paPortalID;      ///< ��Ż �ε����� �迭
};

struct SNtlEventPortalAddReq
{
	RwUInt32	hNPCSerailID;
};

struct SNtlEventPortalAddRes
{
	BYTE           portalID;
};

struct SNtlEventPortalRes
{
	BYTE           byPoint;        ///< ��Ż�� ����Ʈ (���̺��� ����Ʈ ��ȣ)
};

struct SNtlEventOperateObjectTimeBar
{
	bool			bShow;			// true : Ÿ�ӹ� �����ֱ�, false : Ÿ�ӹ� �����
	RwUInt32		uiPlayTime;		// Ÿ�ӹ� ��� �ð�
};

// ��� ��縦 ��� �ؽ�Ʈ ���̺����� �������� 
enum ETelecastTextTable
{
	E_TELECAST_TEXT_NPC_SPEECH,
	E_TELECAST_TEXT_QUEST,
};

struct SNtlEventTeleCastBegNfy
{
	TBLIDX					npcTblidx;				// ����� �ϴ� NPC ���̺� �ε���
	BYTE					byTelecastType;			// ��� Ÿ�� eTELECAST_MESSAGE_TYPE
	ETelecastTextTable      eTextTable;             // ��縦 ��� �ִ� �ؽ�Ʈ ���̺� ����
	TBLIDX					speechTblidx;			// ��۳����� ���� Speech ���̺� �ε���
	std::wstring            wstrNotifyMsg;           // �������� �������� �˸� �޽��� (Notify Ÿ���϶��� ��ȿ)
	DWORD					dwDisplayTime;			// ��۽ð��� ��Ÿ���� millisec ( INVALID_DWORD �̸� telecast�� ��� ����, �ƴϸ� �� �ð��� ������ ���� �� )    
};

/// ���� ���ÿ� ������Ʈ ���� �̺�Ʈ
struct SNtlEventDirectionNfy
{
	RwBool                  bCreate;                // TRUE : ǥ�� ����, FALSE : ǥ�� ����
	RwV3d			    	vLoc;					// ������ ��ġ (������Ʈ�� �������� �ʾ��� ��� ������ ��)
};

struct SNtlEventBroadMsgBegNfy
{
	RwUInt8								byMsgType;			// eBROAD_MSG_TYPE
	vecdef_BroadMsgDataList				defMsgDataList;		// Broad msg data list
};

struct SNtlEventBroadMsgEndNfy
{
	RwUInt8								byMsgType;			// eBROAD_MSG_TYPE
};

// Skill RpBonus Setup Result
struct SNtlEventRpBonusSetupRes
{
	TBLIDX			skillId;			// Skill�� ���̺� ID
	BYTE			bySlotIndex;		// �ƹ�Ÿ�� ������ �ִ� ������ Skill ���� Index
	BYTE			byRpBonusType;		// eDBO_RP_BONUS_TYPE
	RwBool			bRpBonusAutoMode;			
};

// Friend List
struct SNtlEventFriendAddRes
{
	WORD            wResultCode;        ///< ��� �ڵ�    
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];    ///< ĳ���� ����
	RwUInt32        uiTargetID;         ///< Ÿ��ID
};

struct SNtlEventFriendDelRes
{
	WORD            wResultCode;    
	RwUInt32        uiTargetID;
};

struct SNtlEventFriendMoveRes
{
	WORD            wResultCode;    
	RwUInt32        uiTargetID;
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];    ///< ĳ���� ���� 
	// Ŭ���̾�Ʈ ������ �ʿ��ؼ�, Ŭ���̾�Ʈ ����ü���� Name�� �߰��Ѵ�.
};

struct SNtlEventFriendListInfo
{
	BYTE            byCount;
	sFRIEND_FULL_INFO			asInfo[NTL_MAX_COUNT_FRIEND + NTL_MAX_COUNT_FRIEND];
};

struct SNtlEventBlackListAddRes
{
	WORD            wResultCode;
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	RwUInt32        uiTargetID;
};

struct SNtlEventBlackListDelRes
{
	WORD            wResultCode;
	RwUInt32        uiTargetID;
};

struct SNtlEventFriendInfo
{
	sFRIEND_INFO	sInfo;  
	bool			bIsFirst;		// true : ó�� false : ���� 
};

struct SNtlEventFriendInfoChange
{
	CHARACTERID					targetID;
	BYTE						byChangeType;	// NtlFriend.h eFRIEND_CHANGE_TYPE
	DWORD						dwChangeValue;	// Online 1:On 2:Off , Class , Level , Channel, PositionMapTblidx
};

// ģ�� ����Ʈ ���� �߰� �޽��� (�������� ������� �޽��� �̿��� Ŭ���̾�Ʈ������ �ʿ信 ���� �߰��ϴ� �޼���)
enum eFriendExtraEvent
{
	SLFE_FRIEND_LOGIN,              ///< ģ���� �α��νÿ� �޴� �޽���
};

struct SNtlEventFriendInfoNotify
{
	RwInt32         nNotifyMsg;
	CHARACTERID     targetID;
};

struct SNtlEventBlockingNfy
{
	RwBool			bStartOrEnd;
	DWORD			dwCoolingTime;
};

struct SNtlEventChargingNfy
{
	RwBool			bStartOrEnd;
};


// Ŀ�´����̼� Ÿ���� �����ϴ� Ÿ��
enum ECommuTargetType
{
	COMMU_TARGET_FRIEND,
	COMMU_TARGET_GUILD,
    COMMU_TARGET_PARTY,
};

struct SNtlEventCommuTargetSelect
{
	CHARACTERID     uiCharID;               ///< ĳ���� ID (Ŀ�´�Ƽ �����κ��� ���� ID)
	SERIAL_HANDLE   uiSerialID;             ///< �ø��� ID
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	ECommuTargetType eTargetType;
};

struct SNtlEventCommuTargetRelease
{
	SERIAL_HANDLE   uiSerialID;
};

/**
* \brief ���� ������ ���ڿ� Notify
*/
struct SNtlEventPopoNotify
{
	const WCHAR* pwcTitle;			///< Notify�� ���� ( �ִ� 32���� ) ( ����� ǥ�� )
	const WCHAR* pwcBuffer;			///< Notify�� ���� ( �ִ� 64���� )
	RwBool		 bImmediate;		///< TRUE : ��� ǥ�� , FALSE : ������������ ���ÿ� ����
	RwReal		 fLifeTime;			///< ��� ǥ�ö�� �� ���� ��µ� ���ΰ�? ( 0.0f = �ڵ����� ������ ���� )
};

/// ���� �̺�Ʈ
struct SNtlEventTransform : public SNtlEventSob
{
    sASPECTSTATE  sApsectState;           ///< ���� ����
};

/// �� ���� �̺�Ʈ
struct SNtlEventMobTransform : public SNtlEventSob
{
    TBLIDX          tblTransformMobID;  ///< ���� ���� �� ���̺� �ε���
};

/// ĵ�� ���� �̺�Ʈ
struct SNtlEventTransformCandy : public SNtlEventSob
{
    RwBool  bTransform;                 ///< ���� ����
};

struct SNtlEventBotCaution_Nfy : public SNtlEventSob
{
};

struct SNtlEventBotHelpMe_Nfy : public SNtlEventSob
{
	RwBool        bIsRequester;          ///< 1: Mobs for help 2: Mobs for help
};

#define FPS_INTERPOLATION_TIME  0.2f                   ///< General camera -> FPS camera interpolation time
struct SNtlEventCameraFPS
{
	RwV3d   vCurrLookAt;                    ///< Direction you are looking at
	RwV3d   vTargetLookAt;                  ///< Direction to look when changed to first person camera
};

enum eScouterEventType
{
	SCOUTER_EVENT_STATUS,					///< Event set scouter status on/off

	SCOUTER_EVENT_MEASURE_POWER_START,		///< Start measuring attack power
	SCOUTER_EVENT_MEASURE_MEASURE_RESULT,	///< I have received a measure of attack power.
	SCOUTER_EVENT_MEASURE_ERROR,			///< Force measurement error

	SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER,	///< Scout failure
	SCOUTER_EVENT_EXIT,					    ///< When leaving Scout (End of Use)

	SCOUTER_EVENT_PC_INFO_VIEW,				///< View PC information
};

struct SDboEventScouter
{
	int			iType;			///< Scouter�� ���޵Ǵ� ������ Ÿ��

	RwUInt32	uiUserData;
	int			iUserData2;
	int			iUserData3;
	int			iUserData4;
	int			iUserData5;
	int			iUserData6;
	int			iUserData7;
	int			iUserData8;
};

/// ��Ƽ ���� Ÿ�ٿ� ���� ����
struct SNtlEventPartyShareTargetRes
{
	WORD            wResultCode;
};

/// ��Ƽ ���� Ÿ�ٿ� ���� �˸�
struct SNtlEventPartyShareTargetNfy
{
	sSHARETARGET_INFO		sharetargetInfo[NTL_MAX_SHARETARGET_COUNT];    
};

struct SNtlEventShareTargetSelect : public SNtlEventSob
{
	RwUInt8         bySlot;             ///< Ÿ���� ��� ���� Ÿ�������� ����
};

struct SNtlEventShareTargetRelease : public SNtlEventSob
{
};

// Passenger
struct SNtlEventSobOnBus : public SNtlEventSob
{
	RwBool				bRideOn;            ///< ���� ž�½ÿ��� True, �����ÿ��� False
	SERIAL_HANDLE		hBusSerialId;       ///< ������ �ø��� ID        
};

struct SNtlEventSobOnVehicle : public SNtlEventSob
{
	RwBool				bRideOn;			// TRUE : ride on, FALSE : ride off
	SERIAL_HANDLE		hDriverId;			// Driver handle
	RwUInt32			uiVehicleTblIdx;	// Vehicle table index
};

enum eBUS_MOVE_TYPE
{
	BUS_MOVE_POS_DIR,
	BUS_MOVE_DELETE_BUS,
	BUS_MOVE_RESET
};

struct SNtlEventBusMove
{
	RwUInt8				byMessage;
	SERIAL_HANDLE		hBus;
	TBLIDX				idxBusTable;
	RwV3d*				pV3Pos;
	RwV3d*				pV3Dir;
};

// Driver
struct SNtlEventSobVehicleStart : public SNtlEventSob
{
	SERIAL_HANDLE		hVehicleItem;
	TBLIDX				tblVehicleItem;
	TBLIDX				tblFuelItemTblIdx;
};

struct SNtlEventSobRideOnOffVehicle : public SNtlEventSob
{
	RwBool				bRideOn;			// TRUE : ride on, FALSE : ride off
	RwUInt32			uiVehicleTblIdx;	// Vehicle table index
};

struct SNtlEventSobVehicleEngine : public SNtlEventSob
{
	RwUInt8				byMessage;			// eVehicleEngineMessgeType
	RwUInt32			uiParam;
};

struct SNtlEventSobVehicleStunt : public SNtlEventSob
{
};

struct SNtlEventAfterEffectRemainTimeNfy
{
	DWORD dwTimeRemaining;

	SNtlEventAfterEffectRemainTimeNfy()
	{
		dwTimeRemaining = 0;
	}
};

struct SNtlEventTLDefaultSkin
{
	const char*		pFontName;
	RwRGBA			rgbFontBackColor;
	RwRGBA			rgbFontColor;
	RwInt32			nFontSize;
	RwUInt32		uiFontAllign;
	RwV2d			vFontOffset;

	RwV2d			vMsgLayerOffset;

	RwV2d			vOkOffset;

	RwV2d			vIMGOffset;
};

struct SNtlEventTLGuideMsg_ShowHide
{
	RwUInt32		uiTextTblIdx;
	RwReal			fFadeInOutTime;
	RwBool			bShow;
};

struct SNtlEventTLGuideMsg_ChangeColor
{
	RwUInt32		uiTextTblIdx;
	RwReal			fFadeInOutTime;
	RwRGBA			rgbFontBackColor;
	RwRGBA			rgbFontColor;
};

struct SNtlEventTLGuideIMG_ShowHide
{
	RwChar*			pIMGName;
	RwReal			fFadeInOutTime;
	RwBool			bShow;
	RwReal			fSizeRate;
};

struct SNtlEventTLGuideIMGOk_ShowHide
{
	RwReal			fFadeInOutTime;
	RwBool			bShow;
};

struct SNtlEventTLUIIndicator_ShowHide
{
	RwUInt32		uiTLIndicatorType;
	RwChar*			pcFlashName;
	RwV2d			vFocusOffPos;
	RwBool			bShow;
};

struct SNtlEventTLMouseInputLockUnlock
{
	RwUInt32				uiMouseInputType;
	RwBool					bLock;
};

struct SNtlEventTLKeyboardInputLockUnlock
{
	RwUInt32				uiKeyboardInputType;
	RwBool					bLock;
};

struct SNtlEventTLActionMapBackup
{
};

struct SNtlEventTLActionMapRestore
{
};

struct SNtlEventTLExcuteQuest
{
	RwUInt16				tQuestID;
};

struct SNtlEventTLAttackLockUnlock
{
	RwUInt32				uiAttackType;
	RwBool					bLock;
};

struct SNtlEventTLPlayCamera
{
	RwUInt32				uiCameraType;
	RwUInt32				uiTblIdx;
	RwV3d					vTargetPos;
};

struct SNtlEventTLMinimap
{
	RwBool					bShow;
};

enum eNtlTLLockType
{
	TLLOCK_MOUSE,
	TLLOCK_KEYBOARD,
	TLLOCK_ATTACK
};

struct SNtlEventTLLockUnlcok
{
	RwUInt8					byLockType;
	RwUInt8					byIndex;
	RwBool					bLock;
};

// Condition
struct SNtlEventTLCondition
{
	RwUInt32				uiCondType;
	RwUInt32				uiParam1;
};

struct SNtlEventTLDisableDialogMoveOption
{
	RwBool					bDiableDialogMoveOption;
};

struct SNtlEventTLEnableSkip
{
	RwBool					bEnableSkip;
};

struct SNtlEventCondTerror
{
	RwBool                  bAffect;                    ///< ���� ����
	RwUInt32                hSubject;                   ///< ȿ���� ���� ��� (�ƹ�Ÿ or ��)
	RwUInt32                hAttackerSerialId;          ///< ������ Ÿ���� �ø��� ID    
};

struct SNtlEventCondConfused
{
	RwBool                  bAffect;                    ///< ���� ����
	RwUInt32                hSubject;                   ///< ȿ���� ���� ��� (�ƹ�Ÿ or ��)
	RwUInt32                hTargetSerialId;            ///< ������ Ÿ���� �ø��� ID
};

/// ������ �̺�Ʈ ����ü
struct SNtlEventCondHidingKi
{
    SERIAL_HANDLE           hSerialID;                  
    RwBool                  bAffect;                    ///< ���� ����    
};

// õ������ ����ȸ 
struct SNtlEventMinorMatchStateUpdate
{
	RwUInt8		byMatchState;
	RwUInt8		byStage;
	RwUInt32	dwRemainTime;
	RwBool		bEnterState;
};

struct SNtlEventMinorMatchTeamInfo
{
	RwUInt8			byMatchIndex;
	RwUInt8			byTeamCount;
	RwUInt16		wTeamInfo_var;
	sVARIABLE_DATA*	pData;	
};

struct SNtlEventMinorMatchPlayerState
{
	RwUInt32	hPlayer;
	RwUInt8		byPcState;
};

struct SNtlEventMinorMatchUpdateScore
{
	RwUInt16	wTeamType;
	RwUInt8		byScore;
	RwUInt32	hSlayer;
	RwUInt32	hFainter;
};

struct SNtlEventMinorMatchTeamScore
{
	RwUInt8 						byCount;
	sMATCH_MINORMATCH_TEAM_SCORE	aTeamScore[BUDOKAI_MAX_MINORMATCH_TEAM_COUNT];
};

struct SNtlEventMinorMatchSelection
{
	RwUInt16	wWinnerTeam;
	RwUInt8		byTeamCount;
	sMINORMATCH_SELECTION_DATA	aSelection[BUDOKAI_MAX_MINORMATCH_TEAM_COUNT];
};

struct SNtlEventMinorMatchStageFinish
{
	RwUInt8		byStageResult;
	RwUInt16	wStageWinner;
};

struct SNtlEventMinorMatchMatchFinish
{
	RwUInt8		byMatchResult;
	RwUInt16	wMatchWinner;
	RwUInt8		byKillCount;
};

struct SNtlEventMajorMatchStateUpdate
{
	RwUInt8		byMatchState;
	RwUInt8		byStage;
	RwUInt32	dwRemainTime;
	RwBool		bEnterState;
};

struct SNtlEventMajorMatchTeamInfo
{
	RwUInt8			byMatchDepth;
	RwUInt8			byMatchIndex;
	RwUInt8			byTeamCount;
	RwUInt16		wTeamInfo_var;
	sVARIABLE_DATA*	pData;	
};

struct SNtlEventMajorMatchPlayerState
{
	RwUInt32	hPlayer;
	RwUInt8		byPcState;
};

struct SNtlEventMajorMatchUpdateScore
{
	sMATCH_SCORE*	pStageScore;
};

struct SNtlEventMajorMatchStageFinish
{
	sMATCH_SCORE*	pStageScore;
	RwUInt16		wStageWinner;
	RwUInt8			byStageResult;
	bool			bIsObserver;
};

struct SNtlEventMajorMatchMatchFinish
{
	sMATCH_SCORE*	pStageScore;
	RwUInt16		wMatchWinner;
	RwUInt8			byMatchResult;
};

struct SNtlEventFinalMatchStateUpdate
{
	RwUInt8		byMatchState;
	RwUInt8		byStage;
	RwUInt32	dwRemainTime;
	RwBool		bEnterState;
};

struct SNtlEventFinalMatchTeamInfo
{
	RwUInt8			byMatchDepth;
	RwUInt8			byMatchIndex;
	RwUInt8			byTeamCount;
	RwUInt16		wTeamInfo_var;
	sVARIABLE_DATA*	pData;	
};

struct SNtlEventFinalMatchPlayerState
{
	RwUInt32	hPlayer;
	RwUInt8		byPcState;
};

struct SNtlEventFinalMatchUpdateScore
{
	sMATCH_SCORE*	pStageScore;
};

struct SNtlEventFinalMatchStageFinish
{
	sMATCH_SCORE*	pStageScore;
	RwUInt16		wStageWinner;
	RwUInt8			byStageResult;
	bool			bIsObserver;
};

struct SNtlEventFinalMatchMatchFinish
{
	sMATCH_SCORE*	pStageScore;
	RwUInt16		wMatchWinner;
	RwUInt8			byMatchResult;
};

struct SNtlEventAwardInfo
{
	RwUInt16*		paTeamNameIndex;
	sVARIABLE_DATA*	pData;
};

/**
* \brief õ������ ����ȸ�� ���� ����
*/
struct sNtlEventBudokaiStateInfoNfy
{
	RwUInt16	wSeasonCount;
	BUDOKAITIME	tmDefaultOpenTime;

	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	aMatchStateInfo[MAX_BUDOKAI_MATCH_TYPE];

	sBUDOKAI_JOIN_INFO					sJoinInfo;
};

/**
* \brief õ������ ����ȸ�� Main State Update
*/
struct sNtlEventBudokaiUpdateStateNfy
{
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
};

/**
* \brief õ������ ����ȸ�� Match State update
*/
struct sNtlEventBudokaiUpdateMatchStateNfy
{
	RwUInt8								byMatchType;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sStateInfo;
};

/**
* \brief õ������ ����ȸ ������ ��� ���
*/
struct SNtlEventBudokaiJoinIndividualRes
{
	RwUInt16	wResultCode;
	RwUInt16	wJoinId;
	RwBool		bDojoRecommender;
};

/**
* \brief õ������ ����ȸ ������ ��� ���
*/
struct SNtlEventBudokaiLeaveIndividualRes
{
	RwUInt16	wResultCode;
};

/**
* \brief õ������ ����ȸ �� ���� ���
*/
struct SNtlEventBudokaiJoinTeamInfoRes
{
	RwUInt16					wResultCode;

	RwUInt8						byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	asPointInfo[NTL_MAX_MEMBER_IN_PARTY];
};

/**
* \brief õ������ ����ȸ ��Ƽ�� ��� ���
*/
struct SNtlEventBudokaiJoinTeamRes
{
	RwUInt16	wResultCode;
	RwUInt32	hTroublePc;
	RwReal		fTotalRankPoint;
	RwUInt16	wJoinId;
	WCHAR		wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1];

	BYTE		byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	aTeamInfo[NTL_MAX_MEMBER_IN_PARTY];
};

/**
* \brief õ������ ����ȸ ��Ƽ�� ��� �˸�
*/
struct SNtlEventBudokaiJoinTeamNfy
{
	RwReal		fTotalRankPoint;
	RwUInt16	wJoinId;
	WCHAR		wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1];

	BYTE		byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	aTeamInfo[NTL_MAX_MEMBER_IN_PARTY];
};

/**
* \brief õ������ ����ȸ ��Ƽ�� ��� ���
*/
struct SNtlEventBudokaiLeaveTeamRes
{
	RwUInt16	wResultCode;
};

/**
* \brief õ������ ����ȸ ��Ƽ�� ��� ��� ���
*/
struct SNtlEventBudokaiLeaveTeamMemberRes
{
	RwUInt16	wResultCode;
};

/**
* \brief õ������ ����ȸ ��Ƽ�� ��� ��� �˸�
*/
struct SNtlEventBudokaiLeaveTeamMemberNfy
{
	WCHAR*		pwcMemberName;
};

/**
* \brief õ������ ����ȸ ��� ����
*/
struct SNtlEventBudokaiJoinInfoRes
{
	RwUInt16	wResultCode;

	RwUInt8		byMatchType;

	union
	{
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO	sIndividualInfo;
		sBUDOKAI_REGISTER_TEAM_INFO			sTeamInfo;
	};
};

/**
* \brief ������ �ڷ���Ʈ ����
*/
struct SNtlEventBudokaiMudosaInfoRes
{
	RwUInt16				wResultCode;
	RwUInt8					byMudosaCount;
	sBUDOKAI_MUDOSA_INFO	aMudosaInfo[BUDOKAI_MAX_MUDOSA_INFO_COUNT];
};

/// ��ũ���� �������� �̺�Ʈ
struct SNtlEventScreenShot
{
	std::string             strFileName;            ///< ��ũ���� ���� ������ �̸� (Ȯ���� ����)
};

struct SNtlEventSkillSelectMode
{
	bool			bSelect;
	SERIAL_HANDLE	hSerialId; //skill handle
};

/**
* \brief õ������ ����ȸ ���� ����
*/
struct SNtlEventBudokaiJoinStateNfy
{
	RwUInt8			byMatchType;		// eBUDOKAI_MATCH_TYPE
	RwUInt8			byJoinState;		// eBUDOKAI_JOIN_STATE
	RwUInt8			byJoinResult;		// eBUDOKAI_JOIN_RESULT
};

struct SNtlEventBudokaiJoinStateRes
{
	RwUInt16			wResultCode;
	RwUInt8				byMatchType;		// eBUDOKAI_MATCH_TYPE
	RwUInt8				byJoinState;		// eBUDOKAI_JOIN_STATE
	RwUInt8				byJoinResult;		// ���� ���
};

/**
* \brief õ������ ����ȸ ���̵� ������ ���� ����ü
*/
struct SNtlEventBudokaiNoticeNfy
{
	RwUInt8		byNoticeType;
	RwUInt32	tblidxNotice;
	VOID*		pData;
};

/**
* \brief �ó׸�ƽ Ŀ�ǵ� 
*/

struct SNtlEventCinematicGuiCommand
{
	RwChar* szCommand;
	RwBool	bShow;
};

enum eDialogMoveType
{
	TRM_DIALOG_DIALOG_MOVABLE_DEFAULT_POSITION,	///< ���̾�αװ� ������ �� ���� �� �⺻ ��ġ ����
	TRM_DIALOG_DIALOG_MOVABLE,			///< ���̾�α׸� ������ �� �ִ� ���� ��ȯ
	TRM_DIALOG_DIALOG_DISMOVABLE,		///< ���̾�α׸� ������ �� ���� ���� ��ȯ
};

struct SNtlEventDialogMove
{
	RwInt32		nType;
};

struct SNtlEventGambleAniStart : public SNtlEventSob
{
};

struct SNtlEventGambleAniEnd
{
	RwInt32 hGambleNpc;
};

enum eEVENT_GAME_OPTION_TYPE
{
	OPTION_CHAT_INPUTMODE = 0,
	OPTION_CHAT_WINDOWLOCK,
	OPTION_CHAT_WINDOWMSCROLL,
	OPTION_CHAT_ADDEDWINDOW,
	OPTION_CHAT_WINDOWOPACITY,

	OPTION_CHAT_NORMAL,
	OPTION_CHAT_WISPHER,
	OPTION_CHAT_PARTY,
	OPTION_CHAT_GUILD,
	OPTION_CHAT_TRADE,
	OPTION_CHAT_SHOUT,
	OPTION_CHAT_FIND_PARTY,
//	OPTION_CHAT_RAID,
	OPTION_CHAT_SYSTEM,
	
	// ��� �ɼ� ���� �� ��������
	OPTION_CHAT_REFRESH,

	GAME_OPTION_MOVIE_VOLUME,
};

struct SNtlEventGameChatOption
{
	RwUInt8 byData1;
	RwUInt8 byData2;
	RwUInt8 byData3;
};

struct SNtlEventTenkaichiMarking : public SNtlEventSob
{
    BYTE            byType;                 // ��ŷ Ÿ��
};

struct SNtlEventTitleEffect : public SNtlEventSob
{
	bool		bCreate;
	const char* pchBone;
	const char* pchEffect;
};

struct SNtlEventBudokaiProgressMessageNfy
{
	RwUInt8			byMsgId;
};

/// ȸ�� ������ �޾Ƽ� �����̵� �Ǵ� �̺�Ʈ
struct SNtlEventSobPushingNfy : public SNtlEventSob
{
    BYTE                byAttackResult;
    WORD                wAttackResultValue;
    RwV3d               vDestPos;
};

struct SNtlEventChangeClassAuthorityChanged
{
	RwBool				bCanChangeClass;
};

struct SNtlEventSobGuardCrush : public SNtlEventSob
{
	
};

struct SNtlEventSobMovePatternSync : public SNtlEventSob
{
	RwUInt8 bySyncUnitId;
};


struct SNtlEventSobWarMode : public SNtlEventSob
{
	
};

struct SNtlEventSobTransfromSequela : public SNtlEventSob
{

};

struct SNtlEventSobCinematicScale : public SNtlEventSob
{
	float fScale;
	float fTime;
};

struct SNtlEventChangeCharName
{
	CHARACTERID		charid;
	WCHAR			wchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR			wchOldCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
};

struct SNtlEventChangeNickName
{
	SERIAL_HANDLE		hSerialID;
	TBLIDX				nicknameTblidx;
};

struct SNtlEventHTPRpResult
{
	RwInt32				iAttackPoint;
	RwInt32				iDefenderPoint;
	RwBool				bWin;
	RwBool				bAttacker;
};

struct SNtlEventHoipoiMixItemMachineMake
{
	RwUInt8				byPlace;
	RwUInt8				byPos;
};

struct SNtlEventDynamicObjectUpdateState
{
	SERIAL_HANDLE			uiHandle;
	sDYNAMIC_OBJECT_BRIEF*	pBrief;
	sDYNAMIC_OBJECT_STATE*	pState;
};

struct SNtlEventHoipoiMixItemRecipeInfo
{
	RwUInt16		wCount;
	void*			pRecipeData;
};

struct SNtlEventHoipoiMixRecipeRegNfy
{
	TBLIDX			idxRecipeTbl;
};

struct SNtlEventHoipoiMixItemMakeExpNfy
{
	RwUInt32		dwCurExp;
	RwUInt8			byCurLevel;
	DWORD			dwExpGained;
};

enum eHOIPOIMIX_INFO_UPDATE_TYPE
{
	eHOIPOIMIX_INFO_LEVELUP,
	eHOIPOIMIX_INFO_ADD_EXP
};

struct SNtlEventHoipoiMixInfoUpdate
{
	RwUInt8			byType;
	RwUInt32		dwData;
};


struct SNtlEventCharRefreshBattleAttributeModifier
{
	SERIAL_HANDLE	hSerialID;
	RwReal			fHonestOffenceModifier;
	RwReal			fHonestDefenceModifier;
	RwReal			fStrangeOffenceModifier;
	RwReal			fStrangeDefenceModifier;
	RwReal			fWildOffenceModifier;
	RwReal			fWildDefenceModifier;
	RwReal			fEleganceOffenceModifier;
	RwReal			fEleganceDefenceModifier;
	RwReal			fFunnyOffenceModifier;
	RwReal			fFunnyDefenceModifier;
};

//////////////////////////////////////////////////////////////////////////
// QuickSlot Gui Mode
//////////////////////////////////////////////////////////////////////////

struct SNtlEventQuickSlotGuiMode
{
	RwBool	bExpand1;
	RwBool	bExpand2;
};

struct SNtlEventQuickSlotLockMode
{
	RwBool	bLock;
};

//////////////////////////////////////////////////////////////////////////
// Mob ���� Event
struct SNtlEventMobDie
{
	SERIAL_HANDLE m_hSerialID;
};

/// ���� ���� ���Ḧ ������ �˷��ִ� �̺�Ʈ
struct SNtlEventSobSendAttackEnd : public SNtlEventSob
{
    
};


#endif
