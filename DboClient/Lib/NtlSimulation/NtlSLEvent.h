/*****************************************************************************
*
* File			: NtlSLEvent.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
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

extern RWS::CEventId g_EventNotifySendPacket;			// packet을 보낼때발생하는 이벤트(PacketTrace용)
extern RWS::CEventId g_EventNotifyReceivePacket;		// packet을 받을때발생하는 이벤트(PacketTrace용)

//////////////////////////////////////////////
/**
* world change
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventWorldChange;		
extern RWS::CEventId g_EventCreateWorld;                 // 월드가 Destroy되던가 Create될때

//////////////////////////////////////////////
/**
* avatar/camera 이동 및 회전
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCameraSmoothingMove;		// camera smoothing move.
extern RWS::CEventId g_EventCameraShake;				// 진동 camera.
extern RWS::CEventId g_EventCameraDash;					// dash camera.
extern RWS::CEventId g_EventCameraNpc;					// npc click 연출 camera.
extern RWS::CEventId g_EventCameraDB;					// dragon ball 연출 camera.
extern RWS::CEventId g_EventCameraDTClear;				// camera direct node clear.
extern RWS::CEventId g_EventCameraDTSplineNode;			// camera direct spline node.
extern RWS::CEventId g_EventCameraFree;					// Free Camera
extern RWS::CEventId g_EventCameraTimeMachine;			// 타임머신 출발 연출 카메라 이벤트
extern RWS::CEventId g_EventCameraExplosion;			// 폭발 카메라 이벤트
extern RWS::CEventId g_EventCameraTimeMachineArrive;	// 타이머신 도착 카메라 이벤트
extern RWS::CEventId g_EventCameraKnockDownMatrix;		// knockdown matrix camera 연출.
extern RWS::CEventId g_EventCameraFPS;                  // 1인칭 카메라
extern RWS::CEventId g_EventCameraBus;					// 버스 카메라
extern RWS::CEventId g_EventCameraObserver;             // 천하제일 무도회 옵저버 카메라
extern RWS::CEventId g_EventCameraControlDelete;		// 카메라 삭제

//////////////////////////////////////////////
/**
* action mapping
*/

//////////////////////////////////////////////

extern RWS::CEventId g_EventActionMapHalt;				// action halt action mapping
extern RWS::CEventId g_EventActionMapJump;				// jump action mapping
extern RWS::CEventId g_EventActionMapAirJump;			// air jump action mapping
extern RWS::CEventId g_EventActionMapSitAndStand;		// sit and stand action mapping
extern RWS::CEventId g_EventActionMapSkillUse;			// skill 사용.
extern RWS::CEventId g_EventActionMapHTBUse;			// htb 사용.
extern RWS::CEventId g_EventActionMapItemUse;			// item 사용.
extern RWS::CEventId g_EventActionMapSocialUse;			// social action 사용.
extern RWS::CEventId g_EventActionMapPrivateShopOpen;	// private shop Open;
extern RWS::CEventId g_EventActionMapCharging;			// charging 사용.
extern RWS::CEventId g_EventActionMapBlocking;			// blocking 사용.
extern RWS::CEventId g_EventActionMapAutoFollow;		// auto follow 사용.
extern RWS::CEventId g_EventActionMapDice;				// 액션 주사위 사용
extern RWS::CEventId g_EventScreenShot;                 // 스크린샷을 찍은후에 발생하는 이벤트

extern RWS::CEventId g_EventSkillSelectMode;

// 액션 스킬 아이콘을 이용한 액션 (AvatarController에서 한번 걸러져서 이벤트를 보낸다)
extern RWS::CEventId g_EventActionFuncBasicAttack;      ///< 기본 공격 아이콘
extern RWS::CEventId g_EventActionFuncAssist;            ///< 어시스트 아이콘
extern RWS::CEventId g_EventActionFuncGetItem;          ///< 아이템 습득 아이콘
extern RWS::CEventId g_EventActionFuncSitAndStand;      ///< 앉기 아이콘


extern RWS::CEventId g_EventESCinNarrationMode;

//////////////////////////////////////////////
/** 
*simulation object event
*/
//////////////////////////////////////////////

//--------------------
// simulation 기본 event
//--------------------

extern RWS::CEventId g_EventSobShowNfy;

//--------------------
// input에 관한 event
//--------------------

// mouse로 terrain을 click 했을 경우 발생되는 event이다.
extern RWS::CEventId g_EventTerrainClick;		

// avatar를 keyboard 이동시킬때 발생하는 event.
extern RWS::CEventId g_EventKeyboardMove;		

// avatar를 keyboard 이동을 멈출 때 발생하는 event.
extern RWS::CEventId g_EventKeyboardMoveStop;	

// avatar를 mouse dash move 이동.
extern RWS::CEventId g_EventMouseDashMove;

// avatar를 keyboard dash move 이동.
extern RWS::CEventId g_EventKeyboardDashMove;

extern RWS::CEventId g_EventAirDashMove;
extern RWS::CEventId g_EventAirAccelMove;

// avatar를 heading을 바꿀때 발생하는 event.
extern RWS::CEventId g_EventInputChangeHeading;

// mouse로 simulation object를 click 했을 경우 발생되는 event이다.
extern RWS::CEventId g_EventSobTargetSelect;     

// mouse로 simulation object를 click 해제 event이다.
extern RWS::CEventId g_EventSobTargetSelectRelease;     

// mouse로 simulation object를 attack mark release 해제 event이다.
extern RWS::CEventId g_EventSobAttackMarkRelease;

// mouse로 simulation object를 double click 했을 경우 발생되는 event이다.
extern RWS::CEventId g_EventSobAttackSelect;     

extern RWS::CEventId g_EventSobTargetMarkSelect;                    ///< 타겟 마킹할 오브젝트를 선택할때 (메가 도동파 스킬) 

extern RWS::CEventId g_EventSobTargetMarkRelease;                   ///< 타겟 마킹할 오브젝트를 릴리즈

// mouse로 simulation object에 mouse focus가 들어왔을 경우.
extern RWS::CEventId g_EventSobGotFocus;     

// mouse로 simulation object에 mouse focus가 해제 되었을 경우.
extern RWS::CEventId g_EventSobLostFocus;     

extern RWS::CEventId g_EventCommuTargetSelect;      // 커뮤니티 Target을 선택할때의 이벤트
extern RWS::CEventId g_EventCommuTargetRelease;     // 커뮤니티 Target을 해제할때의 이벤트


/// 선공몹이 날 눈치챘을때 발생하는 이벤트
extern RWS::CEventId g_EventBotCaution_Nfy;

/// 몹이 주위 동료에게 도움을 청할때 발생하는 이벤트
extern RWS::CEventId g_EventBotHelpMe_Nfy;

/// 공유 타겟 마크를 지정하는 이벤트
extern RWS::CEventId g_EventShareTargetSelect;

// 공유 타겟 마크를 없애는 이벤트
extern RWS::CEventId g_EventShareTargetRelease;

extern RWS::CEventId g_EventTenkaichiMarking;

extern RWS::CEventId g_EventSobEventTitleEffect;

extern RWS::CEventId g_EventAvatarTargetStateChange;

extern RWS::CEventId g_EventAvatarReset;

//-----------------------------
// create/delete에 관한 함수.
//-----------------------------

// simulatin object 생성 event.
extern RWS::CEventId g_EventSobCreate;

// simulation object 소멸 event.
extern RWS::CEventId g_EventSobDelete;

// 
extern RWS::CEventId g_EventSobSummonPetSpawnSync;

//--------------------------------
// 펫 생성에 관련된 이벤트
//--------------------------------
extern RWS::CEventId g_EventSobPetSkillInfo;	// 펫 스킬에 대한 정보

//-----------------------------
// server 동기 객체의 이동에 관한 함수.
//-----------------------------
extern RWS::CEventId g_EventSobMove;				// simulation object move event
extern RWS::CEventId g_EventSobMoveSync;            // 움직임 동기화 이벤트
extern RWS::CEventId g_EventSobSecondDestMove;		// simulation object second dest move event
extern RWS::CEventId g_EventSobSplineMove;			// simulation object spline move event
extern RWS::CEventId g_EventSobAdjustMovePosition;  // simulation object adjust move position event
extern RWS::CEventId g_EventSobDash;				// simulation object dash event
extern RWS::CEventId g_EventSobMoveStop;			// simulation object move stop event
extern RWS::CEventId g_EventSobDirectionFloat;		// simulation object direction float
extern RWS::CEventId g_EventSobFollowMove;			// simulation object follow move
extern RWS::CEventId g_EventSobAdjustMove;			// simulation object adjust move event
extern RWS::CEventId g_EventSobPushingNfy;			///< 회전 공격을 받아서 슬라이딩 되는 이벤트
extern RWS::CEventId g_EventSobGuardCrush;			// simulation object guard crush를 당한다.
extern RWS::CEventId g_EventSobMovePatternSync;		// 현재 monster만 사용하고 있슴.
extern RWS::CEventId g_EventSobAirJump;
extern RWS::CEventId g_EventSobAirAccel;

//-----------------------------
// item 장착과 해제에 관한 event.
//-----------------------------

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

// ItemUseRes 가 실패로 날라왔을 때 발동되는 이벤트
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
// Dynamic Object 관련 이벤트
//////////////////////////////////////////////////////////////////////////
extern RWS::CEventId g_EventDynamicObjectUpdateState;
extern RWS::CEventId g_EventHoipoiMixRecipeRegNfy;
extern RWS::CEventId g_EventHoipoiMixItemRecipeInfo;	///< 아이템 레시피 정보
extern RWS::CEventId g_EventHoipoiMixItemMakeExpNfy;	///< 아이템 만들기 경험치 증가 알림
extern RWS::CEventId g_EventHoipoiMixInfoUpdate;			///< 호이포이믹스 인포 업데이트


// 변신 이벤트
extern RWS::CEventId g_EventTransform;
extern RWS::CEventId g_EventMobTransform;               ///< 몹이 변신할때의 이벤트
extern RWS::CEventId g_EventTransformCandy;             ///< 사탕으로 변신하는 이벤트
extern RWS::CEventId g_EventSobTransformSequela;        ///< 변신 휴유증

// 컨디션 이벤트
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
// QuickSlot에 관련된 event
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
// quest item 장착과 해제에 관한 event.
//-----------------------------

extern RWS::CEventId g_EventSobQuestItemAdd;

extern RWS::CEventId g_EventSobQuestItemDelete;

extern RWS::CEventId g_EventSobQuestItemMove;

extern RWS::CEventId g_EventSobQuestItemUpdate;


//-----------------------------
// server 동기에 대한 action들에 관한 event.
//-----------------------------

// simulation object 가 knockdown에서 stand로 변할 때 보내는 이벤트.
extern RWS::CEventId g_EventSobStanding;

// simulation object fighting mode
extern RWS::CEventId g_EventSobFightingMode;

// simulation object attack event(target에 공격을 시작시키는 이벤트)
extern RWS::CEventId g_EventSobAttack;

extern RWS::CEventId g_EventSobSpecialAttack;

extern RWS::CEventId g_EventSobBattleNotification;

// simulation object attacked event(공격시작은 반대로 공격당했을때 의미 -> g_EventSobAttack의 반대 이벤트)
extern RWS::CEventId g_EventSobAttacked;

// simulation object attack event(target에 공격을 시작시키는 이벤트)
extern RWS::CEventId g_EventSobPetBeginAttack;

// simulation object hit event(attack이 시작된후 target에게 damage를 전달해 주는 event)
extern RWS::CEventId g_EventSobHit;

// simulation object attack follow event
extern RWS::CEventId g_EventSobAttackFollow;

// simulation object attack mode event
extern RWS::CEventId g_EventSobAttackMode;

extern RWS::CEventId g_EventSobSendAttackEnd;           ///< 어택모드 종료를 서버에 알리는 이벤트

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
extern RWS::CEventId g_EventSobConvertClass;            ///< 전직 이벤트
extern RWS::CEventId g_EventSobChangeAdult;             ///< 모델을 바꾸는 이벤트 (성인 <-> 어린이)

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

// simulation object skill acted event    g_EventSobSkillActiond의 반대
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
extern RWS::CEventId g_EventSobUpdateLPStatusNfy;           ///< Sob이 빈사 상태에 빠졋을때의 이벤트

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

extern RWS::CEventId g_EventPartyShareTargetRes;            ///< 공유 타겟 선택에 대한 서버 응답
extern RWS::CEventId g_EventPartyShareTargetNfy;            ///< 공유 타겟 선택에 대한 서버 알림

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

extern RWS::CEventId g_EventHTBRPStock;						// HTB RP Stock 선택 GUI
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
extern RWS::CEventId g_EventFriendInfoNotify;           ///< UI를 업데이트 하기 위한 이벤트 (데이터->UI)


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

extern RWS::CEventId g_EventRBBoardInfo;		// 랭크배틀 게시판 갱신
extern RWS::CEventId g_EventRBJoin;				// 랭크배틀 신청관련.

//extern RWS::CEventId g_EventRBBoardUpdate;	// 랭크배틀 게시판 갱신
//extern RWS::CEventId g_EventRBRoomInfo;		// 방의 상세 정보
//extern RWS::CEventId g_EventRBRoom;			// 랭크배틀 신청 취소
//extern RWS::CEventId g_EventRBMatch;			// 랭크배틀 준비 시작 취소

extern RWS::CEventId g_EventRBBattleStateUpdate;	// 랭크배틀 경기장 상태정보
extern RWS::CEventId g_EventRBBattleTeamInfo;		// 팀 정보 알림
extern RWS::CEventId g_EventRBBattlePlayerState;	// 플레이어 상태
extern RWS::CEventId g_EventRBBattleKillScoreUpdate;// 킬 스코어 업데이트
extern RWS::CEventId g_EventRBBattleStageFinish;	// 스테이지 마침
extern RWS::CEventId g_EventRBBattleMatchFinish;	// 경기 마침
extern RWS::CEventId g_EventRBBattleEnd;			// RBBattle 월드 나가기 직전
extern RWS::CEventId g_EventRBForcedEnd;			// 서버쪽에서의 랭크배틀 강제종료

extern RWS::CEventId g_EventRBTotalScoreUpdate;		// 개인 전적 업데이트

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
extern RWS::CEventId g_EventPortalInfoRes;          ///< 포탈 정보를 받을때
extern RWS::CEventId g_EventPortalAddReq;
extern RWS::CEventId g_EventPortalAddRes;           ///< 포탈을 등록 추가했을 때의 응답
extern RWS::CEventId g_EventPortalRes;              ///< 포탈을 이동요청 했을 때의 응답

//_____________________________
// Popo Icon String Notify
//_____________________________
extern RWS::CEventId g_EventPopoNotify;

//_____________________________
// 버스 및 탈것 관련 이벤트
//_____________________________
extern RWS::CEventId g_EventSobOnBus;
extern RWS::CEventId g_EventSobOnVehicle;

extern RWS::CEventId g_EventSobVehicleStart;
extern RWS::CEventId g_EventSobRideOnOffVehicle;
extern RWS::CEventId g_EventSobVehicleEngine;
extern RWS::CEventId g_EventSobVehicleStunt;

extern RWS::CEventId g_EventBusMove;

//////////////////////////////////////////////////////////////////////////
// Client 와 TS 간의 연동 메시지들
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

extern RWS::CEventId g_EventShowTutorialNfy;		// 튜토리얼 관련 알림 이벤트

extern RWS::CEventId g_EventTSRemovingTMQQuest_Nfy;	// TMQ 퀘스트 제거 알림

extern RWS::CEventId g_EventTSPCDialog_Nfy;

extern RWS::CEventId g_EventTSMudosaTeleport_Req;	// 무도사 텔레포트

extern RWS::CEventId g_EventTSMudosaTeleport_Res;

extern RWS::CEventId g_EventQuestNarrationUserSelectDialog_Req;

extern RWS::CEventId g_EventQuestNarrationUserSelectDialog_Res;

extern RWS::CEventId g_EventTSFinished;

extern RWS::CEventId g_EventTSSkipContainer;


// Client => TS

extern RWS::CEventId g_EventTSItemUse;			// 아이템 사용
extern RWS::CEventId g_EventTSItemGet;			// 아이템 획득
extern RWS::CEventId g_EventTSItemEquip;		// 아이템 장착
extern RWS::CEventId g_EventTSScoutUse;			// 스카우터 사용
extern RWS::CEventId g_EventTSSkillUse;			// 스킬 사용
extern RWS::CEventId g_EventTSRankBattle;		// 랭크배틀
extern RWS::CEventId g_EventTSColTriggerObject;	// 트리거 오브젝트 충돌
extern RWS::CEventId g_EventTSBindStone;		// 바인드 스톤에 바인드 시
extern RWS::CEventId g_EventTSSearchQuest;		// 스카우터에서 퀘스트 찾기 시
extern RWS::CEventId g_EventTSItemUpgrade;		// 아이템 업그래이드 시
extern RWS::CEventId g_EventTSTeleport;			// 텔레포트 시
extern RWS::CEventId g_EventTSBudokai;			// 천하제일 무도회
extern RWS::CEventId g_EventTSSlotMachine;		// 슬롯 머신
extern RWS::CEventId g_EventTSHoipoiMix;		// 호이포이 믹스
extern RWS::CEventId g_EventTSPrivateShop;		// 개인상점
extern RWS::CEventId g_EventTSFreeBattle;		// 전투
extern RWS::CEventId g_EventTSItemIdentity;		// 아이템 인증
extern RWS::CEventId g_EventTSUseMail;			// 메일 사용
extern RWS::CEventId g_EventTSParty;			// 파티

extern RWS::CEventId g_EventTSClickSideIcon;	// 
extern RWS::CEventId g_EventTSLevelCheck;		// 
extern RWS::CEventId g_EventTSQuest;			// 
extern RWS::CEventId g_EventTSDialogOpen;		// 

//////////////////////////////////////////////////////////////////////////
// Server 와 TS 간의 연동 메시지들
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
extern RWS::CEventId  g_EventNightEffect;				// 용신 등장시 화면이 어두워지는 효과 
extern RWS::CEventId  g_EventDBCNarration;				// 용신 나레이션용 이벤트
extern RWS::CEventId  g_EventDBCNarrationEnd;			// 용신 나레이션 종료 이벤트
extern RWS::CEventId  g_EventDBCCheck_Res;				// Check 응답 패킷
extern RWS::CEventId  g_EventDBCReward_Res;				// 보상 응답 패킷
extern RWS::CEventId  g_EventDBCCollect_Nfy;			// 서버에서 일방적으로 끝낼때 패킷
extern RWS::CEventId  g_EventDBCShedule_Info;			// DBC의 스케쥴 정보 패킷
extern RWS::CEventId  g_EventDBCScatter;                 // 드래곤볼이 흩어지는 연출 시작을 알리는 이벤트

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
extern RWS::CEventId g_EventNPCCommuExit;              // 커뮤니티 상태를 빠져나오는 이벤트


//////////////////////////////////////////////
/**
* 정보 얻어오기.
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventSobGetState;

extern RWS::CEventId g_EventSobGetFightingMode;

extern RWS::CEventId g_EventSobGetAirMode;

//////////////////////////////////////////////
/**
* 트리거 오브젝트 스카우터 연출 관련
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventScouter;				// 스카우터 이벤트

//////////////////////////////////////////////
/**
* PopoStone World Load Ready Event
*/
//////////////////////////////////////////////

extern RWS::CEventId g_EventCharReady;								// 월드 로드가 끝난 시점에 발생

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

extern RWS::CEventId g_EventBudokaiNoticeNfy;					///< 천하제일무도회 공지
extern RWS::CEventId g_EventBudokaiStateInfoNfy;				///< 천하제일 무도회의 현재 상태를 전송
extern RWS::CEventId g_EventBudokaiUpdateStateNfy;				///< 천하제일 무도회 Main State update
extern RWS::CEventId g_EventBudokaiUpdateMatchStateNfy;			///< 천하제일 무도회 Match State Update
extern RWS::CEventId g_EventBudokaiJoinIndividualRes;			///< 개인 등록
extern RWS::CEventId g_EventBudokaiLeaveIndividualRes;			///< 개인 취소
extern RWS::CEventId g_EventBudokaiJoinTeamInfoRes;				///< 팀 포인트
extern RWS::CEventId g_EventBudokaiJoinTeamRes;					///< 팀 등록
extern RWS::CEventId g_EventBudokaiJoinTeamNfy;					///< 팀 등록 알림
extern RWS::CEventId g_EventBudokaiLeaveTeamRes;				///< 팀 등록 취소
extern RWS::CEventId g_EventBudokaiLeaveTeamNfy;				///< 팀 등록 취소 알림
extern RWS::CEventId g_EventBudokaiLeaveTeamMemberRes;			///< 팀 멤버 취소
extern RWS::CEventId g_EventBudokaiLeaveTeamMemberNfy;			///< 팀 멤버 취소 알림
extern RWS::CEventId g_EventBudokaiJoinInfoRes;					///< 팀 등록 정보
extern RWS::CEventId g_EventBudokaiMudosaInfoRes;				///< 무도사 텔레포트 정보
extern RWS::CEventId g_EventBudokaiJoinStateNfy;				///< 천하제일 무도회 참가상태
extern RWS::CEventId g_EventBudokaiJoinStateRes;				///< 참가 상태 정보 결과

extern RWS::CEventId g_EventBudokaiProgressMessageNfy;			///< 천하제일 무도회 진행 상태 메시지 출력

// Cinematic Command
extern RWS::CEventId g_EventCinematicGuiCommand;					///< 시네마틱 커맨드

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

extern RWS::CEventId g_EventChangeNickName;				///< 별명 바꾸기

///////////////////////////////////////////////////////////////////////////////////////////
// Change Class Authority Changed
extern RWS::CEventId g_EventChangeClassAuthorityChangedNfy;	///< 아바타 생성뒤에도 날라온다. 

///////////////////////////////////////////////////////////////////////////////////////////
// BattleAttribute refresh ( 타겟팅된 상대의 대한 정보 )
extern RWS::CEventId g_EventCharRefreshBattleAttributeModifier;

///////////////////////////////////////////////////////////////////////////////////////////
// RankPoint reset notify
extern RWS::CEventId g_EventCharRankPointResetNfy;

//////////////////////////////////////////////////////////////////////////
// QuickSlot 관련 Option Event
extern RWS::CEventId g_EventQuickSlotGuiMode;				///< 퀵슬롯 확장 옵션
extern RWS::CEventId g_EventQuickSlotLockMode;				///< 퀵슬롯 락 모드

//////////////////////////////////////////////////////////////////////////
// Mob 관련 Event
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
* avatar/camera 이동 및 회전
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

struct SNtlEventCameraNpc					// camera npc click 연출 event data structer.
{
	void			*pActor;
};

struct SNtlEventCameraDB
{
	void			*pActor;
};

struct SNtlEventCameraTimeMachine
{
	void            *pObject;           ///< 타임머신 오브젝트의 포인터
};

struct SNtlEventCameraExplosion
{
	void            *pObject;           ///< 폭발되는 객체의 포인터
	unsigned int	uiExplosionId;		///< 폭발 아이디 "ENtlPLExplosionEventType" 에 해당
};

struct SNtlEventCameraTimeMachineArrive
{
	RwBool			bStart;				///< TRUE : 시작, FALSE : 종료
	void            *pObject;           ///< 타이머신 오브젝트 객체의 포인터
};

struct SNtlEventCameraKnockDownMatrix	// camera knockdown matrix 연출 event data structer.
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

/// 옵저버 카메라 타입
enum EObserverCameraType
{
    E_OBSERVER_TYPE_STATIC,     ///< 정적 카메라 위치
    E_OBSERVER_TYPE_NODE,       ///< DTEditor에서 만든 데이터
};

struct SNtlEventCameraObserver
{
    EObserverCameraType eType;      ///< 타입
    RwInt32             nIndex;     ///< 인덱스
};

//////////////////////////////////////////////
/**
* action mapping
*/

//////////////////////////////////////////////
/** 
*simulation object event
*/

/// Sob에 보내는 이벤트들의 상위 구조체
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
	RwBool			bSuccess;		// change heading을 처리하였는가?
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
	RwBool          bIsClientCreate;     // 서버가 아닌 클라이언트 자체에서 생성했는지 유무 (ex.시네마틱)
};

struct SNtlEventSobMonsterCreate : public SNtlEventSobCharCreate	// simulation monster object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	sMOB_BRIEF		*pMobBrief;
	RwBool          bIsClientCreate;    // 서버가 아닌 클라이언트 자체에서 생성했는지 유무 (ex.시네마틱)
};

struct SNtlEventSobNpcCreate : public SNtlEventSobCharCreate	// simulation npc object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	sNPC_BRIEF		*pNpcBrief;
	RwBool			bCreateName;
	RwBool          bIsClientCreate;    // 서버가 아닌 클라이언트 자체에서 생성했는지 유무 (ex.시네마틱)
};

struct SNtlEventSobVehicleCreate : public SNtlEventSobCharCreate	// simulation vehicle object create
{
	RwV3d			vLoc;
	RwV3d			vDir;
	RwUInt32		uiVehicleTblIdx;
	// g_EventSobCreate 이벤트의 구조체인 경우는 Vehicle 테이블 인덱스
	// g_EventSobVehicleEngine 이벤트의 구조체인 경우는 Sob Item 핸들
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
	NESWUT_RECIEVE_DATA,		///< 서버로 부터 창고 데이터를 받았다
	NESWUT_WAREHOUSE_START,		///< 창고를 연다
	NESWUT_BUY_SLOT,			///< 창고를 구입한다
	NESWUT_ADD_ZENNY,			///< 창고에 제니를 넣는다
	NESWUT_SUB_ZENNY,			///< 창고에서 제니를 뺀다
	NESWUT_CLOSE,				///< 창고를 닫는다
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
	RwUInt8			byItemCount;	///< 유효 아이템 갯수
	void*			pData;			///< 실제 데이터
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
	RwUInt8			byFormFlag;		// 걷기, 뛰기, 수영.
	RwUInt32		uiTimeStamp;
	RwUInt8			byPathNodeNum;
	RwV3d			*pPathNode;
};

/// 이동 싱크 이벤트 구조체
struct SNtlEventSobMoveSync : public SNtlEventSob
{
    RwV3d vLoc;
    RwV3d vDir;
    DWORD dwTimeStamp;
    RwUInt8 byMoveFlag;
};

struct SNtlEventSobSecondDestMove : public SNtlEventSob
{
	RwUInt8			byFormFlag;		// 걷기, 뛰기, 수영.
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
	RwUInt8			byCount;		// Equip Index와 동일
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

/// 전직 이벤트 구조체
struct SNtlEventSobConvertClass : public SNtlEventSob
{
	RwUInt8			byClass;
};

/// 모델 체인지 이벤트 구조체
struct SNtlEventSobChangeAdult : public SNtlEventSob
{
    RwBool          bAdult;             /// True면 성인, False면 어린이로 바꾼다.
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
    RwBool bEmergency;      // TRUE : 빈사 상태, FALSE : 일반 상태
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
	EVENT_AIUT_SKILL				= 0x0010,	/** avatar가 최초 생성시 스킬이 왕창 새로이 생성된다 */
	EVENT_AIUT_SKILL_LEARN			= 0x0020,	/** 스킬 배우기 */
	EVENT_AIUT_SKILL_LEVELUP		= 0x0040,	/** 스킬 level up*/
	EVENT_AIUT_SKILL_SP				= 0x0080,	/** 스킬 SP */
	EVENT_AIUT_SKILL_COOLING_START	= 0x0100,	/** 스킬 cooling start => uiParam1 : icon serial*/
	EVENT_AIUT_SKILL_COOLING_END	= 0x0200,	/** 스킬 cooling end => uiParam1 : icon serial*/
	EVENT_AIUT_BUFF					= 0x0400,
	EVENT_AIUT_DIE					= 0x0800,
	EVENT_AIUT_ZENNY				= 0x1000,
	EVENT_AIUT_HTBSKILL_LEARN		= 0x2000,	/** HTB 배우기 **/
	EVENT_AIUT_WAREHOUSE			= 0x4000,	/** 아이템을 제외한 창고 내부에서의 변화 **/	
	EVENT_AIUT_SKILL_RPBONUS		= 0x8000,	/** RPType변화 **/
	EVENT_AIUT_SKILL_UI_INIT		= 0x00010000, /** 스킬 UI 초기화(서버이동) **/
	EVENT_AIUT_SKILL_INIT			= 0x00020000, /** 스킬 HTB 초기화(SP 반환) **/
	EVENT_AIUT_POINT_HONOR			= 0x00040000, /** 명예 포인트 */
	EVENT_AIUT_POINT_MUDOSA			= 0x00080000, /** 무도사 포인트 */
    EVENT_AIUT_NETPY                = 0x00100000, ///< PC방용 NetPy 포인트
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

enum ePartyMesssageType			///< 파티 메세지(SNtlEventPartyUpdate, SNtlEventPartyUpdateValue이 같은 것을 쓴다)
{
	PMT_PARTY_CREATE,			///< 파티 생성	
	PMT_PARTY_LEAVE,			///< 파티 탈퇴
	PMT_PARTY_MEMBER_ADD,		///< 파티원 추가
	PMT_PARTY_MEMBER_DEL,		///< 파티원 탈퇴
	PMT_PARTY_LEADER_CHANGE,	///< 파티장 변경

	PMT_ZENNY_DIVISION,			///< 제니 분배 방식이 변경 되었다
	PMT_ITEM_DIVISION,			///< 아이템 분배 방식이 변경 되었다

	PMT_LP,						///< LP 업데이트
	PMT_EP,						///< EP 업데이트
	PMT_LEVEL,					///< Level 업데이트
	PMT_CLASS,					///< 클래스 변경

	PMT_MEMBER_LOCATION,		///< 맴버 위치

	PMT_PARTY_DUNGEON_STATE,	///< 파티 던전 상태
	PMT_PARTY_DUNGEON_INITIALIZE,///< 파티 초기화
};

struct SNtlEventPartyUpdate		///< 시뮬레이션용 파티 메세지
{
	RwInt32			iMessage;		///< 파티 메세지
	SERIAL_HANDLE	hSerialId;		///< 해당 맴버의 시리얼
	RwUInt32		uiValue;		///< 메세지에 따른 수치
	RwInt32			iValue2;		///< 메세지에 따른 수치
};

struct SNtlEventPartyUpdateValue	///< 클라이언트용 파티 메세지
{
	RwInt32			iValueType;		///< PartyUpdateValue Type
	SERIAL_HANDLE	hSerial;		///< 해당 맴버의 시리얼
	RwUInt32		uiValue;		///< 메세지에 따른 수치
	RwInt32			iValue2;		///< 메세지에 따른 수치
};

struct SNtlEventPartyMemberLocation
{
	RwUInt32		uiSerial;		///< 맴버 시리얼
	RwUInt32		uiWorldID;		///< 월드 아이디
	RwUInt32		uiWorldTbl;		///< 월드 테이블 아이디
	RwV3d			vCurLoc;		///< 위치
};

struct SNtlEventPartyCreate
{
	WCHAR*		pcName;			///< 파티 이름
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
	SLGE_GUILD_INFO,				///< 길드 정보를 받는다

	SLGE_ADD_MEMBER,				///< 길드 맴버 한명의 정보를 받는다
	SLGE_DEL_MEMBER,				///< 길드 맴버 한명의 정보를 지운다

	SLGE_APPOINT_MASTER,			///< 길드장 임명
	SLGE_APPOINT_SECOND_MASTER,		///< 부길드장 임명
	SLGE_DISMISS_SECOND_MASTER,		///< 부길드장 직위 해제
	SLGE_APPOINT_MEMBER,			///< 일반 길드원으로 임명

	SLGE_BEING_DISBAND,				///< 길드 해산 대기
	SLGE_CANCLE_DISBAND,			///< 해산 취소
	SLGE_DISBAND,					///< 길드 해산

	SLGE_MEM_POS,					///< 맴버 위치 변경
	SLGE_MEM_CLASS,					///< 맴버 클래스 변경
	SLGE_MEM_LEVEL,					///< 맴버 레벨 변경
	SLGE_MEM_REPUTATION,			///< 맴버 평판 변경
	SLGE_MEM_NAME,					///< 

	SLGE_MEM_ONLINE,				///< 맴버 게임 접속
	SLGE_MEM_OFFLINE,				///< 맴버 게임 나감

	SLGE_CHANGE_GUILD_NAME,			///< 길드의 이름이 바뀌었다
	SLGE_CHANGE_GUILD_EMBLEM,		///< 길드의 문장이 바뀌었다
	SLGE_CHANGE_FUNCTION,			///< 길드 기능 변경
	SLGE_CHANGE_REPUTATION,			///< 길드 명성 변경
	SLGE_CHANGE_INVEST_ZENNY,		///< 길드를 위해 제니를 투자하였다

	SLGE_NOTICE,					///< 길드 공지 변경
	SLGE_DOGI,						///< 도복 색상 변경
	SLGE_DOGI_NFY,					///< 도복 색상 변경된 데이터
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
	DOJO_EVENT_DOJO_INFO,							///< 도장 기본 정보를 받았다
	DOJO_EVENT_CHANGE_LEVEL,						///< 도장의 레벨 변경
	DOJO_EVENT_CHANGE_TENKAI_SEED_PLAYER,			///< 도장을 소유한 유파의 천하제일 무도회 시드 출전자 변경
	DOJO_EVENT_CHANGE_DOJO_NOTICE,					///< 도장 공지
	DOJO_EVENT_NPC_GUI_INFO,						///< GUI 표시 정보
	DOJO_EVENT_LOST_DOJO,							///< 도장의 소유권을 잃었다

	DOJO_EVENT_SCRAMBLE_ACCEPT,						///< 도장 쟁탈전 수락
	DOJO_EVENT_SCRAMBLE_TELEPORT_PROPOSAL,			///< 도장 쟁탈전 텔레포트를 할 수 있다는 서버의 알림

	DOJO_EVENT_SEAL_ATTACK_STATE,					///< 도장 쟁탈전중 인장 위치, 상태 정보
	DOJO_EVENT_SEAL_ATTACK_BEGIN,					///< 도장 쟁탈전중 인장을 뺏기 위한 캐스팅을 시작한다
	DOJO_EVENT_SEAL_ATTACK_END,						///< 도장 쟁탈전중 인장을 뺏기 위한 캐스팅을 종료한다
	DOJO_EVENT_SCRAMBLE_SCORE,						///< 도장 쟁탈전 점수
	DOJO_EVENT_SCRAMBLE_CHANGE_SEAL_OWNER,			///< 도장 인장 소유 유파 변경

	DOJO_EVENT_SCRAMBLE_RESULT,						///< 도장 쟁탈전 결과
	DOJO_EVENT_SCRAMBLE_REWARD,						///< 도장 쟁탈전 보상
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
	TBT_COUNTDOWN,					///< TMQ Play time 계산 진행 여부
	TET_REWARD,						///< TMQ 보상
	TET_INFOMATION_NFY,				///< 정보 타입 eTIMEQUEST_INFORMATION_TYPE
	TET_COUPON_COUNT,				///< TMQ 쿠폰 정보
	TET_TMQ_POINT,					///< TMQ 포인트
	TET_GAME_TIME,					///< TMQ 게임 시간
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
// Rank Battle 관련 
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
* 정보 얻어오기.
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
	sTS_KEY				sTSKey;					// TS 키

	bool				bNewRegister;			// 퀘스트 등록시 New type의 등록 여부
	bool				bQuestShare;			// 퀘스트 공유
	RwUInt32			uiQuestAreaName;		// 퀘스트 지역 이름
	RwUInt32			uiQuestState;			// 퀘스트 상태
	RwUInt32			uiQuestTitle;			// 퀘스트 제목
	RwUInt32			uiQuestGoal;			// 퀘스트 목적
	eQUEST_SORT_TYPE	eQuestSortType;			// 퀘스트 종류의 타입
};

struct SNtlEventUnregQuest_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventShowQuestWindow
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventShowQuestWindow_Nfy
{
	sTS_KEY				sTSKey;					// TS 키

	RwUInt32			uiQuestAreaName;		// 퀘스트 지역 이름
	RwUInt32			uiQuestTitle;			// 퀘스트 제목
	RwUInt32			uiQuestSort;			// 퀘스트 종류
	RwUInt32			uiQuestGrade;			// 퀘스트 난이도
	eGRADE_TYPE			eGradeType;				// 퀘스트 난이도 타입
	RwUInt32			uiQuestGoal;			// 퀘스트 목표
	RwUInt32			uiQuestContents;		// 퀘스트 내용
	sREWARD_INFO		sDefaultReward[MAX_DEFAULT_REWARD];		// 퀘스트 보상 정보 - 기본 보상
	sREWARD_INFO		sSelectReward[MAX_SELECTION_REWARD];		// 퀘스트 보상 정보 - 선택 보상
	unsigned int			uiRewardExp;
	unsigned int			uiRewardZeni;
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;			// 서버 이벤트 정보 - 데이터 타입
	uSTOC_EVT_DATA		uEvtInfoData;			// 서버 이벤트 정보 - 데이터
	RwUInt32			uiTimeLimit;			// 타임 제한 정보
	RwUInt32			uiEventType;			// eEVENT_GEN_TYPE ( 현재는 eEVENT_GEN_TYPE_CLICK_NPC 만 사용함 )
	RwUInt32			uiEventID;				// uiEventType 에 따른 인덱스 eEVENT_GEN_TYPE_CLICK_NPC	: Mob table index
	RwBool				bIsEventStarter;		// Event type [ True : Starter, False : Reward ]
};

struct SNtlEventUpdateQuestState_Nfy
{
	sTS_KEY				sTSKey;					// TS 키

	RwBool				bOutStateMsg;			// 퀘스트 상태 메시지 출력 여부
	RwUInt32			uiUpdatedQuestFlag;		// 갱신된 퀘스트 상태 플래그
	RwUInt32			uiQuestState;			// 퀘스트 상태

	RwUInt32			uiQuestTitle;			// 퀘스트 제목
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;			// 서버 이벤트 정보 - 데이터 타입
	uSTOC_EVT_DATA		uEvtInfoData;			// 서버 이벤트 정보 - 데이터

	RwUInt32			uiEventType;			// eEVENT_GEN_TYPE ( 현재는 eEVENT_GEN_TYPE_CLICK_NPC 만 사용함 )
	RwUInt32			uiEventID;				// uiEventType 에 따른 인덱스 eEVENT_GEN_TYPE_CLICK_NPC	: Mob table index
	RwBool				bIsEventStarter;		// Event type [ True : Starter, False : Reward ]
};

struct SNtlEventUpdateQuestProgressInfo_Nfy
{
	sTS_KEY				sTSKey;				// TS 키

	eSTOC_EVT_DATA_TYPE	eEvtInfoType;		// 서버 이벤트 정보 - 데이터 타입
	uSTOC_EVT_DATA		uEvtInfoData;		// 서버 이벤트 정보 - 데이터
	RwUInt32			uiTimeLimit;		// 타임 제한 정보
};

struct SNtlEventGiveUpQuest
{
	sTS_KEY sTSKey;							// TS 키
};

struct SNtlEventShowIndicator
{
	sTS_KEY			sTSKey;					// TS 키	
	RwBool			bAuto;
};

struct SNtlEventHideIndicator
{
	sTS_KEY			sTSKey;					// UI에서만 사용.
};

struct SNtlEventShowIndicator_Nfy
{
	sTS_KEY				sTSKey;				// TS 키

	RwUInt32			uiQState;			// 퀘스트 상태
	RwUInt32			uiQuestTitle;		// 퀘스트 제목
	RwUInt32			uiQuestGoal;		// 퀘스트 목적
	eSTOC_EVT_DATA_TYPE	eEvtInfoType;		// 서버 이벤트 정보 - 데이터 타입
	uSTOC_EVT_DATA		uEvtInfoData;		// 서버 이벤트 정보 - 데이터
	RwUInt32			uiTimeLimit;		// 타임 제한 정보
	RwBool				bAuto;
};

struct SNtlEventDoQuest
{
	sQUEST_INFO			sQuestInfo;				// Quest 정보
	eEVENT_GEN_TYPE		eEventGenType;			// Event 종류
	RwUInt32			uiOwnerID;				// Owner ID
	RwBool				bTransCameraCtrlRight;	// 카메라 제어권을 넘기는 경우 TRUE
};

struct SNtlEventQuestProposalDialog_Req
{
	sTS_KEY			sTSKey;						// TS 키

	RwBool			bDisplaySummary;			// 퀘스트 요약 정보 미리보기 보여주기 여부
	RwUInt32		uiQuestTitle;				// 퀘스트 제목
	RwUInt32		uiQuestGoal;				// 퀘스트 목표
	RwUInt32		uiQuestGrade;				// 퀘스트 난이도
	eGRADE_TYPE		eGradeType;					// 퀘스트 난이도 타입
	RwUInt32		uiQuestSort;				// 퀘스트 종류
	RwUInt32		uiQuestContents;			// 퀘스트 내용
	sREWARD_INFO	sDefaultReward[MAX_DEFAULT_REWARD];			// 퀘스트 보상 정보 - 기본 보상
	sREWARD_INFO	sSelectReward[MAX_SELECTION_REWARD];			// 퀘스트 보상 정보 - 선택 보상
	unsigned int			uiRewardExp;
	unsigned int			uiRewardZeni;
};

struct SNtlEventQuestProposalDialog_Res
{
	bool			bResult;					// 퀘스트 수락(true) 또는 거절(false)

	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventQuestUserSelectDialog_Req
{
	sTS_KEY			sTSKey;						// TS 키

	RwUInt32		uiTargetType;				// 타겟의 종류 ( eUSER_SEL_TARGET_TYPE -> eUSER_SEL_TARGET_TYPE_NPC, eUSER_SEL_TARGET_TYPE_OBJECT, eUSER_SEL_TARGET_TYPE_INVALID )
	RwUInt32		uiTargetTblIdx;				// 타겟의 테이블 인덱스 
	RwUInt32		uiQuestTitle;				// 퀘스트 제목
	RwUInt32		uiConv;						// 대화
	RwInt32			nBranchCnt;					// 현재 사용중인 Branch의 개수
	sTS_BRANCH_INFO asBranchInfo[eBRANCH_INFO_MAX];	// Branch info
};

struct SNtlEventQuestUserSelectDialog_Res
{
	bool			bResult;					// Ok(true) 또는 Cancel(false)

	sTS_KEY			sTSKey;						// TS 키
	NTL_TS_TC_ID	tcSelID;					// 선택된 퀘스트 아이디
};

struct SNtlEventQuestNarrationUserSelectDialog_Req
{
	sTS_KEY						sTSKey;							// TS 키

	eUSER_SEL_PROGRESS_STATE	eProgState;						// Narration 진행 상태
	RwUInt32					uiConv;							// 대화
	RwInt32						nBranchCnt;						// 현재 사용중인 Branch의 개수
	sTS_BRANCH_INFO				asBranchInfo[eBRANCH_INFO_MAX];	// Branch info
};

struct SNtlEventQuestNarrationUserSelectDialog_Res
{
	bool			bResult;					// Ok(true) 또는 Cancel(false)

	sTS_KEY			sTSKey;						// TS 키
	NTL_TS_TC_ID	tcSelID;					// 선택된 퀘스트 아이디
};

struct SNtlEventTSFinished
{
	unsigned char	byTSType;					// Trigger type ( TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS )
	NTL_TS_T_ID		tID;						// 종료되는 트리거 아이디
};

struct SNtlEventTSSkipContainer
{
	NTL_TS_T_ID		tID;
	NTL_TS_TC_ID	tcID;
};

struct SNtlEventQuestRewardDialog_Req
{
	sTS_KEY			sTSKey;						// TS 키

	eREWARD_CONTAINER_TYPE eRewardType;			// 보상 타입
	RwUInt32		uiLimitTime;				// 보상 제한 시간
	RwUInt32		uiQuestTitle;				// 퀘스트 제목
	RwUInt32		uiQuestGoal;				// 퀘스트 목표
	RwUInt32		uiQuestGrade;				// 퀘스트 난이도
	eGRADE_TYPE		eGradeType;					// 퀘스트 난이도 타입
	RwUInt32		uiQuestSort;				// 퀘스트 종류
	RwUInt32		uiQuestContents;			// 퀘스트 설명
	sREWARD_INFO	sDefaultReward[MAX_DEFAULT_REWARD];	// 퀘스트 보상 정보 - 기본 보상
	sREWARD_INFO	sSelectReward[MAX_SELECTION_REWARD];// 퀘스트 보상 정보 - 선택 보상
	unsigned int		uiRewardExp;
	unsigned int		uiRewardZeni;
};

struct SNtlEventQuestRewardDialog_Res
{
	bool			bResult;					// Ok(true) 또는 Cancel(false)

	sTS_KEY			sTSKey;						// TS 키
	int				nSelRwdIdx;					// 선택 보상의 선택된 인덱스
};

struct SNtlEventQuestNarrationDialog_Req
{
	sTS_KEY						sTSKey;			// TS 키

	eNARRATION_PROGRESS_STATE	eProgState;		// Narration 진행 상태
	eNARRATION_OWNER_TYPE		eOwnerType;		// Owner의 종류
	RwUInt32					uiOwnerIdx;		// Owner의 테이블 인덱스
	eNARRATION_OWNER_STATE		eOwnerState;	// Owner의 상태
	eNARRATION_DIALOG_DIR_TYPE	eDialogDirType;	// 대화 연출 종류
	RwUInt32					uiDialog;		// 대화 내용
	eNARRATION_GUI_TYPE			eGUIType;		// GUI 종류
	RwUInt32					uiMaxLifeTime;	// 이 시간 이상 나레이션 창이 유지 되는 경우 자동으로 Next로 넘어가야 한다.
};

struct SNtlEventQuestNarrationDialog_Res
{
	bool			bResult;					// Ok(true) 또는 Cancel(false)

	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventQuestNPCDialog_Req
{
	sTS_KEY			sTSKey;						// TS 키

	eNPCCONV_TYPE	eNPCConvType;				// NPC 대화 타입
	unsigned int	uiNPCIdx;					// NPC table index
	RwUInt32		uiDesc;						// 대화
};

struct SNtlEventQuestNPCDialog_Res
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventQuestShare_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventQuestAcceptProposal_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventQuestAcceptReward_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	unsigned int	uiQTitle;					// Quest 제목
	RwBool			bOutStateMsg;				// 퀘스트 상태 메시지 출력 여부
};

struct SNtlEventQuestAcceptGiveUp_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	unsigned int	uiQTitle;					// Quest 제목
};

struct SNtlEventQuestObjDialog_Req
{
	sTS_KEY			sTSKey;						// TS 키

	eOBJCONV_TYPE	eObjConvType;				// Object 대화 타입
	RwUInt32		uiWorldIdx;					// Object 소속 월드
	RwUInt32		uiObjIdx;					// Object table index
	RwUInt32		uiDesc;						// 대화
};

struct SNtlEventQuestObjDialog_Res
{
	sTS_KEY			sTSKey;						// TS 키
};

struct SNtlEventShowTutorialHint_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	bool			bShow;						// 튜토리얼 힌트창 열고 닫기
	TBLIDX			tblidx;						// Guide hint table index
};

struct SNtlEventShowHelp_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	bool			bShow;						// 헬프창 열고 닫기
};

struct SNtlEventShowInventory_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	bool			bShow;						// 인벤토리창 열고 닫기
};

struct SNtlEventShowSkill_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	bool			bShow;						// 스킬창 열고 닫기
};

struct SNtlEventShowCharInfo_Nfy
{
	sTS_KEY			sTSKey;						// TS 키
	bool			bShow;						// 캐릭터 정보창 열고 닫기
};

enum eTutorialNfyType
{
	TUTORIAL_HINT,								// 튜토리얼 힌트
	TUTORIAL_HELP_DIALOG_SWITCH,				// 튜토리얼 헬프 다이얼로그 열고 닫기
	TUTORIAL_BAG_SWITCH,						// 가방 열고 닫기
	TUTORIAL_SKILL_DIALOG_SWITCH,				// 스킬 다이얼로그 열고 닫기
	TUTORIAL_CHARINFO_DIALOG_SWITCH,			// 캐릭터 정보 다이얼로그 열고 닫기
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
* 퀘스트 위치 마크
*/
//////////////////////////////////////////////

struct SNtlEventRegisterQuestPosMark
{
	QM_KEY				QMKey;						// Quest mark key - Key 값 중복 가능함

	eQMI_TARGET_TYPE	eTargetType;				// 타겟 마크 종류
	unsigned int		uiTargetWorldTblIdx;		// 타겟이 소속된 월드 테이블 인덱스
	unsigned int		uiTargetTableIdx;			// 타겟의 테이블 인덱스
	float				fTargetPosX;				// 타겟의 위치 ( x, y, z )
	float				fTargetPosY;
	float				fTargetPosZ;
	unsigned int		uiTooltipTblIdx;			// 툴팁에 출력할 테이블 인덱스
};

struct SNtlEventUnregisterQuestPosMark
{
	QM_KEY			QMKey;						// Quest mark key - Key 값 중복 가능하므로 해당 Key 값에 해당하는 데이터 다 지워 줘야 함
};

struct SNtlEventShowQuestPosMark
{
	sTS_KEY			sTSKey;
	RwBool			bShow;
};

//////////////////////////////////////////////
/**
* dbo 연출 시스템(UI, Camera, Flash)에 trigger event를 forward 시킨다.
*/

struct SNtlEventQuestDirect_Forward
{
	CNtlTCUnit			*pTCUnit;				// 이 변수는 보간하고 있다가 Echo 시켜주어야 한다.
	ETriggerDirectType	eTDType;			// 연출 type
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
	CNtlTCUnit			*pTCUnit;				// SNtlEventQuestDirect_Forward 구조체의 pTCUnit 값을 그대로 echo 시켜준다.
	ETriggerDirectType	eTDType;			// 연출 type
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
* 트리거 오브젝트 스카우터 연출 관련
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

// 드래곤볼 컬렉션 관련
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

// 서버로 받은 응답을 담을 구조체 (공용)
struct SNtlEventDBC_Res
{
	RwUInt32		uiAltarID;
	WORD			wResultCode;
};

// 드래곤볼 수입 스케쥴 패킷 구조체
struct SNtlEventDBCSchedule_Info
{
	bool						bIsAlive;			// 기간 유무 false: 기간이 아니다
	BYTE						byEventType;		// eSCHEDULE_EVENT_TYPE
	__int64						nStartTime;			// 최초기간 시작
	__int64						nEndTime;           // 마지막 기간 끝나는 시간
	DWORD						dwMainTerm;			// 기간 단위 (달은 시작일 나머지는 초) 
	DWORD						dwSubTerm;			// 기간 단위 (달은 만료일 나머지는 초) 
	BYTE						byTermType;			// 0: 달 1: 일 2: 시
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

// 판매자 이벤트 타입
enum ePrivateShopStateEventType
{
	PRIVATESHOP_EVENT_CREATE,
	PRIVATESHOP_EVENT_EXIT,
	PRIVATESHOP_EVENT_OPEN,
	PRIVATESHOP_EVENT_CLOSE,
};

// 구매자 이벤트 타입
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

// 개인상점
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

// Tool Tip UI를 표시한다.
struct SNtlEventToolTip
{
	RwBool          bShow;      ///< 툴팁 표시 유무
	RwInt32         nX;         ///< 표시할 X 좌표
	RwInt32         nY;         ///< 표시할 Y 좌표
	std::wstring    wstrText;    ///< 표시할 텍스트
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
* \brief 액션맵 상태 변경 ( Client 단과의 통신 )
*/
struct SNtlEventActionMapClientNotify
{
	enum eTYPE 
	{ 
		ACTIONMAP_OK = 0,				///< 액션맵이 정상적으로 지정됨
		ACTIONMAP_RELEASE,				///< 키가 해제된 액션이 있음
		ACTIONMAP_FIXEDKEY,				///< 현재 키는 유저가 임의로 사용할 수 없는 키입니다.
		ACTIONMAP_NOTCOMBINE,			///< 조합키로 사용하지 못하는 액션입니다.
		ACTIONMAP_SAMEKEY				///< 키가 중복되어 있습니다.
	};

	RwUInt8		byType;
	RwUInt16	wParam1;
	RwUInt16	wParam2;
};

struct SNtlEventPortalInfoRes
{
	BYTE           byCount;         ///< 포탈의 개수
	BYTE*          paPortalID;      ///< 포탈 인덱스의 배열
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
	BYTE           byPoint;        ///< 포탈의 포인트 (테이블의 포인트 번호)
};

struct SNtlEventOperateObjectTimeBar
{
	bool			bShow;			// true : 타임바 보여주기, false : 타임바 숨기기
	RwUInt32		uiPlayTime;		// 타임바 출력 시간
};

// 방송 대사를 어느 텍스트 테이블에서 가져올지 
enum ETelecastTextTable
{
	E_TELECAST_TEXT_NPC_SPEECH,
	E_TELECAST_TEXT_QUEST,
};

struct SNtlEventTeleCastBegNfy
{
	TBLIDX					npcTblidx;				// 방송을 하는 NPC 테이블 인덱스
	BYTE					byTelecastType;			// 방송 타입 eTELECAST_MESSAGE_TYPE
	ETelecastTextTable      eTextTable;             // 대사를 담고 있는 텍스트 테이블 종류
	TBLIDX					speechTblidx;			// 방송내용을 담은 Speech 테이블 인덱스
	std::wstring            wstrNotifyMsg;           // 서버에서 내려오는 알림 메시지 (Notify 타입일때만 유효)
	DWORD					dwDisplayTime;			// 방송시간을 나타내는 millisec ( INVALID_DWORD 이면 telecast를 계속 띄우고, 아니면 이 시간이 지나면 없앨 것 )    
};

/// 방향 지시용 오브젝트 생성 이벤트
struct SNtlEventDirectionNfy
{
	RwBool                  bCreate;                // TRUE : 표시 생성, FALSE : 표시 삭제
	RwV3d			    	vLoc;					// 지정된 위치 (오브젝트가 생성되지 않았을 경우 참조할 것)
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
	TBLIDX			skillId;			// Skill의 테이블 ID
	BYTE			bySlotIndex;		// 아바타가 가지고 있는 고유의 Skill 슬롯 Index
	BYTE			byRpBonusType;		// eDBO_RP_BONUS_TYPE
	RwBool			bRpBonusAutoMode;			
};

// Friend List
struct SNtlEventFriendAddRes
{
	WORD            wResultCode;        ///< 결과 코드    
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];    ///< 캐릭터 네임
	RwUInt32        uiTargetID;         ///< 타겟ID
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
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];    ///< 캐릭터 네임 
	// 클라이언트 구조상 필요해서, 클라이언트 구조체에만 Name을 추가한다.
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
	bool			bIsFirst;		// true : 처음 false : 나중 
};

struct SNtlEventFriendInfoChange
{
	CHARACTERID					targetID;
	BYTE						byChangeType;	// NtlFriend.h eFRIEND_CHANGE_TYPE
	DWORD						dwChangeValue;	// Online 1:On 2:Off , Class , Level , Channel, PositionMapTblidx
};

// 친구 리스트 관련 추가 메시지 (서버에서 날라오는 메시지 이외의 클라이언트에서만 필요에 따라 추가하는 메세지)
enum eFriendExtraEvent
{
	SLFE_FRIEND_LOGIN,              ///< 친구가 로그인시에 받는 메시지
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


// 커뮤니케이션 타겟을 설정하는 타입
enum ECommuTargetType
{
	COMMU_TARGET_FRIEND,
	COMMU_TARGET_GUILD,
    COMMU_TARGET_PARTY,
};

struct SNtlEventCommuTargetSelect
{
	CHARACTERID     uiCharID;               ///< 캐릭터 ID (커뮤니티 서버로부터 받은 ID)
	SERIAL_HANDLE   uiSerialID;             ///< 시리얼 ID
	WCHAR           wchName[NTL_MAX_SIZE_CHAR_NAME + 1];
	ECommuTargetType eTargetType;
};

struct SNtlEventCommuTargetRelease
{
	SERIAL_HANDLE   uiSerialID;
};

/**
* \brief 포포 아이콘 문자열 Notify
*/
struct SNtlEventPopoNotify
{
	const WCHAR* pwcTitle;			///< Notify의 제목 ( 최대 32글자 ) ( 노란색 표시 )
	const WCHAR* pwcBuffer;			///< Notify의 내용 ( 최대 64글자 )
	RwBool		 bImmediate;		///< TRUE : 즉시 표시 , FALSE : 포포아이콘의 스택에 저장
	RwReal		 fLifeTime;			///< 즉시 표시라면 얼마 동안 출력될 것인가? ( 0.0f = 자동으로 닫히지 않음 )
};

/// 변신 이벤트
struct SNtlEventTransform : public SNtlEventSob
{
    sASPECTSTATE  sApsectState;           ///< 변신 정보
};

/// 몹 변신 이벤트
struct SNtlEventMobTransform : public SNtlEventSob
{
    TBLIDX          tblTransformMobID;  ///< 변신 후의 몹 테이블 인덱스
};

/// 캔디 변신 이벤트
struct SNtlEventTransformCandy : public SNtlEventSob
{
    RwBool  bTransform;                 ///< 변신 유무
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
	int			iType;			///< Scouter에 전달되는 데이터 타입

	RwUInt32	uiUserData;
	int			iUserData2;
	int			iUserData3;
	int			iUserData4;
	int			iUserData5;
	int			iUserData6;
	int			iUserData7;
	int			iUserData8;
};

/// 파티 공유 타겟에 대한 응답
struct SNtlEventPartyShareTargetRes
{
	WORD            wResultCode;
};

/// 파티 공유 타겟에 대한 알림
struct SNtlEventPartyShareTargetNfy
{
	sSHARETARGET_INFO		sharetargetInfo[NTL_MAX_SHARETARGET_COUNT];    
};

struct SNtlEventShareTargetSelect : public SNtlEventSob
{
	RwUInt8         bySlot;             ///< 타겟이 몇번 공유 타겟인지의 슬롯
};

struct SNtlEventShareTargetRelease : public SNtlEventSob
{
};

// Passenger
struct SNtlEventSobOnBus : public SNtlEventSob
{
	RwBool				bRideOn;            ///< 버스 탑승시에는 True, 하차시에는 False
	SERIAL_HANDLE		hBusSerialId;       ///< 버스의 시리얼 ID        
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
	RwBool                  bAffect;                    ///< 적용 유무
	RwUInt32                hSubject;                   ///< 효과를 당한 대상 (아바타 or 펫)
	RwUInt32                hAttackerSerialId;          ///< 공격한 타겟의 시리얼 ID    
};

struct SNtlEventCondConfused
{
	RwBool                  bAffect;                    ///< 적용 유무
	RwUInt32                hSubject;                   ///< 효과를 당한 대상 (아바타 or 펫)
	RwUInt32                hTargetSerialId;            ///< 공격할 타겟의 시리얼 ID
};

/// 기숨기기 이벤트 구조체
struct SNtlEventCondHidingKi
{
    SERIAL_HANDLE           hSerialID;                  
    RwBool                  bAffect;                    ///< 적용 유무    
};

// 천하제일 무도회 
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
* \brief 천하제일 무도회의 현재 상태
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
* \brief 천하제일 무도회의 Main State Update
*/
struct sNtlEventBudokaiUpdateStateNfy
{
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
};

/**
* \brief 천하제일 무도회의 Match State update
*/
struct sNtlEventBudokaiUpdateMatchStateNfy
{
	RwUInt8								byMatchType;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sStateInfo;
};

/**
* \brief 천하제일 무도회 개인전 등록 결과
*/
struct SNtlEventBudokaiJoinIndividualRes
{
	RwUInt16	wResultCode;
	RwUInt16	wJoinId;
	RwBool		bDojoRecommender;
};

/**
* \brief 천하제일 무도회 개인전 취소 결과
*/
struct SNtlEventBudokaiLeaveIndividualRes
{
	RwUInt16	wResultCode;
};

/**
* \brief 천하제일 무도회 팀 정보 결과
*/
struct SNtlEventBudokaiJoinTeamInfoRes
{
	RwUInt16					wResultCode;

	RwUInt8						byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	asPointInfo[NTL_MAX_MEMBER_IN_PARTY];
};

/**
* \brief 천하제일 무도회 파티전 등록 결과
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
* \brief 천하제일 무도회 파티전 등록 알림
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
* \brief 천하제일 무도회 파티전 취소 결과
*/
struct SNtlEventBudokaiLeaveTeamRes
{
	RwUInt16	wResultCode;
};

/**
* \brief 천하제일 무도회 파티전 멤버 취소 결과
*/
struct SNtlEventBudokaiLeaveTeamMemberRes
{
	RwUInt16	wResultCode;
};

/**
* \brief 천하제일 무도회 파티전 멤버 취소 알림
*/
struct SNtlEventBudokaiLeaveTeamMemberNfy
{
	WCHAR*		pwcMemberName;
};

/**
* \brief 천하제일 무도회 등록 정보
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
* \brief 무도사 텔레포트 정보
*/
struct SNtlEventBudokaiMudosaInfoRes
{
	RwUInt16				wResultCode;
	RwUInt8					byMudosaCount;
	sBUDOKAI_MUDOSA_INFO	aMudosaInfo[BUDOKAI_MAX_MUDOSA_INFO_COUNT];
};

/// 스크린샷 찍은후의 이벤트
struct SNtlEventScreenShot
{
	std::string             strFileName;            ///< 스크린샷 찍은 파일의 이름 (확장자 제외)
};

struct SNtlEventSkillSelectMode
{
	bool			bSelect;
	SERIAL_HANDLE	hSerialId; //skill handle
};

/**
* \brief 천하제일 무도회 참가 상태
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
	RwUInt8				byJoinResult;		// 참가 결과
};

/**
* \brief 천하제일 무도회 사이드 아이콘 공지 구조체
*/
struct SNtlEventBudokaiNoticeNfy
{
	RwUInt8		byNoticeType;
	RwUInt32	tblidxNotice;
	VOID*		pData;
};

/**
* \brief 시네마틱 커맨드 
*/

struct SNtlEventCinematicGuiCommand
{
	RwChar* szCommand;
	RwBool	bShow;
};

enum eDialogMoveType
{
	TRM_DIALOG_DIALOG_MOVABLE_DEFAULT_POSITION,	///< 다이얼로그가 움직일 수 있을 때 기본 위치 세팅
	TRM_DIALOG_DIALOG_MOVABLE,			///< 다이얼로그를 움직일 수 있는 모드로 전환
	TRM_DIALOG_DIALOG_DISMOVABLE,		///< 다이얼로그를 움직일 수 없는 모드로 전환
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
	
	// 모든 옵션 적용 후 리프레쉬
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
    BYTE            byType;                 // 마킹 타입
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

/// 회전 공격을 받아서 슬라이딩 되는 이벤트
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
// Mob 관련 Event
struct SNtlEventMobDie
{
	SERIAL_HANDLE m_hSerialID;
};

/// 어택 상태 종료를 서버에 알려주는 이벤트
struct SNtlEventSobSendAttackEnd : public SNtlEventSob
{
    
};


#endif
