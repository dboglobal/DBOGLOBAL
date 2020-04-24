#include "stdafx.h"
#include "AttrEntityMng.h"
#include "Attr_Page.h"

#include "resource.h"

// Event
#include "Attr_EVT_ColObject.h"
#include "Attr_EVT_ClickObject.h"
#include "Attr_EVT_ClickNPC.h"
#include "Attr_EVT_ItemEquip.h"
#include "Attr_EVT_ItemGet.h"
#include "Attr_EVT_ItemUse.h"
#include "Attr_EVT_ScoutUse.h"
#include "Attr_EVT_RcvSvrEvt.h"
#include "Attr_EVT_SkillUse.h"
#include "Attr_EVT_ColRgn.h"
#include "Attr_EVT_RB.h"
#include "Attr_EVT_ClickMob.h"
#include "Attr_EVT_BindStone.h"
#include "Attr_EVT_SearchQuest.h"
#include "Attr_EVT_ItemUpgrade.h"
#include "Attr_EVT_Teleport.h"
#include "Attr_EVT_Budokai.h"
#include "Attr_EVT_SlotMachine.h"
#include "Attr_EVT_HoipoiMix.h"
#include "Attr_EVT_PrivateShop.h"
#include "Attr_EVT_FreeBattle.h"
#include "Attr_EVT_ItemIdentity.h"
#include "Attr_EVT_UseMail.h"
#include "Attr_EVT_Party.h"

// Condition
#include "Attr_COND_ClrQst.h"
#include "Attr_COND_Item.h"
#include "Attr_COND_Lvl.h"
#include "Attr_COND_PCCls.h"
#include "Attr_COND_PCRace.h"
#include "Attr_COND_QItem.h"
#include "Attr_COND_SSM.h"
#include "Attr_COND_SToCEvt.h"
#include "Attr_COND_WItem.h"
#include "Attr_COND_ObjItem.h"
#include "Attr_COND_InNPC.h"
#include "Attr_COND_ObjState.h"
#include "Attr_COND_InWorld.h"
#include "Attr_COND_OperateObject.h"
#include "Attr_COND_HasCoupon.h"
#include "Attr_COND_NPCDead.h"
#include "Attr_COND_DistNPC.h"
#include "Attr_COND_SToCDelivery.h"
#include "Attr_COND_AttachObj.h"
#include "Attr_COND_ProgQuest.h"
#include "Attr_COND_Reputation.h"
#include "Attr_COND_BudokaiState.h"
#include "Attr_COND_ProgCustomEvt.h"
#include "Attr_COND_AvatarDead.h"
#include "Attr_COND_OutRgn.h"

// Action
#include "Attr_ACT_Dir.h"
#include "Attr_ACT_ETimerS.h"
#include "Attr_ACT_ETimerE.h"
#include "Attr_ACT_InSSM.h"
#include "Attr_ACT_Item.h"
#include "Attr_ACT_NPCConv.h"
#include "Attr_ACT_OPCam.h"
#include "Attr_ACT_QItem.h"
#include "Attr_ACT_RegQInfo.h"
#include "Attr_ACT_SToCEvt.h"
#include "Attr_ACT_TWaitTS.h"
#include "Attr_ACT_TSState.h"
#include "Attr_ACT_Portal.h"
#include "Attr_ACT_ObjState.h"
#include "Attr_ACT_ConcCheck.h"
#include "Attr_ACT_ObjConv.h"
#include "Attr_ACT_Hint.h"
#include "Attr_ACT_TMQStage.h"
#include "Attr_ACT_SendSvrEvt.h"
#include "Attr_ACT_OpenWindow.h"
#include "Attr_ACT_TMQTimeBonus.h"
#include "Attr_ACT_BroadCast.h"
#include "Attr_ACT_DirIndicator.h"
#include "Attr_ACT_OperateObject.h"
#include "Attr_ACT_Drop.h"
#include "Attr_ACT_RmvCoupon.h"
#include "Attr_ACT_Escort.h"
#include "Attr_ACT_TMQInfoShow.h"
#include "Attr_ACT_WorldPlayScript.h"
#include "Attr_ACT_SWProbSF.h"
#include "Attr_ACT_PCConv.h"
#include "Attr_ACT_CustomEvt.h"
#include "Attr_ACT_TelMinorMatch.h"
#include "Attr_ACT_TelMudosa.h"
#include "Attr_ACT_PIDgn.h"
#include "Attr_ACT_TLQ.h"
#include "Attr_ACT_DoSkill.h"
#include "Attr_ACT_TObjFriendly.h"
#include "Attr_ACT_BroadMsg.h"
#include "Attr_ACT_MiniNarration.h"
#include "Attr_ACT_RegCTiming.h"
#include "Attr_ACT_ExcCGroup.h"
#include "Attr_ACT_SkipCont.h"
#include "Attr_ACT_ObjWPS.h"
#include "Attr_ACT_Dojo.h"


CAttrEntityMng* GetAttrEntityMng( void ) { static CAttrEntityMng g_clCAttrEntityMng; return &g_clCAttrEntityMng; }


CAttrEntityMng::CAttrEntityMng( void )
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Attr pages
	//
	//////////////////////////////////////////////////////////////////////////

	// Event
	m_defObjList[_T("E_ColObject")]		= RUNTIME_CLASS( CAttr_EVT_ColObject );
	m_defObjList[_T("E_ClickObject")]	= RUNTIME_CLASS( CAttr_EVT_ClickObject );
	m_defObjList[_T("E_ClickNPC")]		= RUNTIME_CLASS( CAttr_EVT_ClickNPC );
	m_defObjList[_T("E_ItemUse")]		= RUNTIME_CLASS( CAttr_EVT_ItemUse );
	m_defObjList[_T("E_ItemGet")]		= RUNTIME_CLASS( CAttr_EVT_ItemGet );
	m_defObjList[_T("E_ItemEquip")]		= RUNTIME_CLASS( CAttr_EVT_ItemEquip );
	m_defObjList[_T("E_RcvSvrEvt")]		= RUNTIME_CLASS( CAttr_EVT_RcvSvrEvt );
	m_defObjList[_T("E_ScoutUse")]		= RUNTIME_CLASS( CAttr_EVT_ScoutUse );
	m_defObjList[_T("E_SkillUse")]		= RUNTIME_CLASS( CAttr_EVT_SkillUse );
	m_defObjList[_T("E_ColRgn")]		= RUNTIME_CLASS( CAttr_EVT_ColRgn );
	m_defObjList[_T("E_RB")]			= RUNTIME_CLASS( CAttr_EVT_RB );
	m_defObjList[_T("E_ClickMob")]		= RUNTIME_CLASS( CAttr_EVT_ClickMob );
	m_defObjList[_T("E_BindStone")]		= RUNTIME_CLASS( CAttr_EVT_BindStone );
	m_defObjList[_T("E_SearchQuest")]	= RUNTIME_CLASS( CAttr_EVT_SearchQuest );
	m_defObjList[_T("E_ItemUpgrade")]	= RUNTIME_CLASS( CAttr_EVT_ItemUpgrade );
	m_defObjList[_T("E_Teleport")]		= RUNTIME_CLASS( CAttr_EVT_Teleport );
	m_defObjList[_T("E_Budokai")]		= RUNTIME_CLASS( CAttr_EVT_Budokai );
	m_defObjList[_T("E_SlotMachine")]	= RUNTIME_CLASS( CAttr_EVT_SlotMachine );
	m_defObjList[_T("E_HoipoiMix")]		= RUNTIME_CLASS( CAttr_EVT_HoipoiMix );
	m_defObjList[_T("E_PrivateShop")]	= RUNTIME_CLASS( CAttr_EVT_PrivateShop );
	m_defObjList[_T("E_FreeBattle")]	= RUNTIME_CLASS( CAttr_EVT_FreeBattle );
	m_defObjList[_T("E_ItemIdentity")]	= RUNTIME_CLASS( CAttr_EVT_ItemIdentity );
	m_defObjList[_T("E_UseMail")]		= RUNTIME_CLASS( CAttr_EVT_UseMail );
	m_defObjList[_T("E_Party")]			= RUNTIME_CLASS( CAttr_EVT_Party );

	// Condition
	m_defObjList[_T("C_ClrQst")]		= RUNTIME_CLASS( CAttr_COND_ClrQst );
	m_defObjList[_T("C_Item")]			= RUNTIME_CLASS( CAttr_COND_Item );
	m_defObjList[_T("C_Lvl")]			= RUNTIME_CLASS( CAttr_COND_Lvl );
	m_defObjList[_T("C_PCCls")]			= RUNTIME_CLASS( CAttr_COND_PCCls );
	m_defObjList[_T("C_PCRace")]		= RUNTIME_CLASS( CAttr_COND_PCRace );
	m_defObjList[_T("C_QItem")]			= RUNTIME_CLASS( CAttr_COND_QItem );
	m_defObjList[_T("C_SSM")]			= RUNTIME_CLASS( CAttr_COND_SSM );
	m_defObjList[_T("C_SToCEvt")]		= RUNTIME_CLASS( CAttr_COND_SToCEvt );
	m_defObjList[_T("C_WItem")]			= RUNTIME_CLASS( CAttr_COND_WItem );
	m_defObjList[_T("C_ObjItem")]		= RUNTIME_CLASS( CAttr_COND_ObjItem );
	m_defObjList[_T("C_InNPC")]			= RUNTIME_CLASS( CAttr_COND_InNPC );
	m_defObjList[_T("C_CheckObjState")]	= RUNTIME_CLASS( CAttr_COND_ObjState );
	m_defObjList[_T("C_CheckInWorld")]	= RUNTIME_CLASS( CAttr_COND_InWorld );
	m_defObjList[_T("C_CheckOPObject")]	= RUNTIME_CLASS( CAttr_COND_OperateObject );
	m_defObjList[_T("C_CheckHasCoupon")]	= RUNTIME_CLASS( CAttr_COND_HasCoupon );
	m_defObjList[_T("C_CheckNPCDead")]	= RUNTIME_CLASS( CAttr_COND_NPCDead );
	m_defObjList[_T("C_CheckDistNPC")]	= RUNTIME_CLASS( CAttr_COND_DistNPC );
	m_defObjList[_T("C_CheckSToCDelivery")]	= RUNTIME_CLASS( CAttr_COND_SToCDelivery );
	m_defObjList[_T("C_CheckAttachObj")]	= RUNTIME_CLASS( CAttr_COND_AttachObj );
	m_defObjList[_T("C_CheckProgQuest")]	= RUNTIME_CLASS( CAttr_COND_ProgQuest );
	m_defObjList[_T("C_CheckReputation")]	= RUNTIME_CLASS( CAttr_COND_Reputation );
	m_defObjList[_T("C_CheckBudokaiState")]	= RUNTIME_CLASS( CAttr_COND_BudokaiState );
	m_defObjList[_T("C_CheckProgCustomEvt")]	= RUNTIME_CLASS( CAttr_COND_ProgCustomEvt );
	m_defObjList[_T("C_CheckAvatarDead")]	= RUNTIME_CLASS( CAttr_COND_AvatarDead );
	m_defObjList[_T("C_CheckOutRgn")]	= RUNTIME_CLASS( CAttr_COND_OutRgn );

	// Action
	m_defObjList[_T("A_Dir")]			= RUNTIME_CLASS( CAttr_ACT_Dir );
	m_defObjList[_T("A_ETimerS")]		= RUNTIME_CLASS( CAttr_ACT_ETimerS );
	m_defObjList[_T("A_ETimerE")]		= RUNTIME_CLASS( CAttr_ACT_ETimerE );
	m_defObjList[_T("A_InSSM")]			= RUNTIME_CLASS( CAttr_ACT_InSSM );
	m_defObjList[_T("A_Item")]			= RUNTIME_CLASS( CAttr_ACT_Item );
	m_defObjList[_T("A_NPCConv")]		= RUNTIME_CLASS( CAttr_ACT_NPCConv );
	m_defObjList[_T("A_OPCam")]			= RUNTIME_CLASS( CAttr_ACT_OPCam );
	m_defObjList[_T("A_QItem")]			= RUNTIME_CLASS( CAttr_ACT_QItem );
	m_defObjList[_T("A_RegQInfo")]		= RUNTIME_CLASS( CAttr_ACT_RegQInfo );
	m_defObjList[_T("A_SToCEvt")]		= RUNTIME_CLASS( CAttr_ACT_SToCEvt );
	m_defObjList[_T("A_TWaitTS")]		= RUNTIME_CLASS( CAttr_ACT_TWaitTS );
	m_defObjList[_T("A_TSState")]		= RUNTIME_CLASS( CAttr_ACT_TSState );
	m_defObjList[_T("A_Potal")]			= RUNTIME_CLASS( CAttr_ACT_Portal );
	m_defObjList[_T("A_ObjState")]		= RUNTIME_CLASS( CAttr_ACT_ObjState );
	m_defObjList[_T("A_ConcCheck")]		= RUNTIME_CLASS( CAttr_ACT_ConcCheck );
	m_defObjList[_T("A_ObjConv")]		= RUNTIME_CLASS( CAttr_ACT_ObjConv );
	m_defObjList[_T("A_Hint")]			= RUNTIME_CLASS( CAttr_ACT_Hint );
	m_defObjList[_T("A_SendSvrEvt")]	= RUNTIME_CLASS( CAttr_ACT_SendSvrEvt );
	m_defObjList[_T("A_TMQStage")]		= RUNTIME_CLASS( CAttr_ACT_TMQStage );
	m_defObjList[_T("A_OpenWindow")]	= RUNTIME_CLASS( CAttr_ACT_OpenWindow );
	m_defObjList[_T("A_TMQTimeBonus")]	= RUNTIME_CLASS( CAttr_ACT_TMQTimeBonus );
	m_defObjList[_T("A_BroadCast")]		= RUNTIME_CLASS( CAttr_ACT_BroadCast );
	m_defObjList[_T("A_DirIndicator")]	= RUNTIME_CLASS( CAttr_ACT_DirIndicator );
	m_defObjList[_T("A_OperateObject")]	= RUNTIME_CLASS( CAttr_ACT_OperateObject );
	m_defObjList[_T("A_Drop")]			= RUNTIME_CLASS( CAttr_ACT_Drop );
	m_defObjList[_T("A_RmvCoupon")]		= RUNTIME_CLASS( CAttr_ACT_RmvCoupon );
	m_defObjList[_T("A_Escort")]		= RUNTIME_CLASS( CAttr_ACT_Escort );
	m_defObjList[_T("A_TMQInfoShow")]	= RUNTIME_CLASS( CAttr_ACT_TMQInfoShow );
	m_defObjList[_T("A_WorldPlayScript")]	= RUNTIME_CLASS( CAttr_ACT_WorldPlayScript );
	m_defObjList[_T("A_SWProbSF")]		= RUNTIME_CLASS( CAttr_ACT_SWProbSF );
	m_defObjList[_T("A_PCConv")]		= RUNTIME_CLASS( CAttr_ACT_PCConv );
	m_defObjList[_T("A_CustomEvt")]		= RUNTIME_CLASS( CAttr_ACT_CustomEvt );
	m_defObjList[_T("A_Minormatch")]	= RUNTIME_CLASS( CAttr_ACT_TelMinorMatch );
	m_defObjList[_T("A_Mudosa")]		= RUNTIME_CLASS( CAttr_ACT_TelMudosa );
	m_defObjList[_T("A_PIDgn")]			= RUNTIME_CLASS( CAttr_ACT_PIDgn );
	m_defObjList[_T("A_TLQ")]			= RUNTIME_CLASS( CAttr_ACT_TLQ );
	m_defObjList[_T("A_DoSkill")]		= RUNTIME_CLASS( CAttr_ACT_DoSkill );
	m_defObjList[_T("A_TObjFriendly")]	= RUNTIME_CLASS( CAttr_ACT_TObjFriendly );
	m_defObjList[_T("A_BroadMsg")]		= RUNTIME_CLASS( CAttr_ACT_BroadMsg );
	m_defObjList[_T("A_MiniNarration")]	= RUNTIME_CLASS( CAttr_ACT_MiniNarration );
	m_defObjList[_T("A_RegCTiming")]	= RUNTIME_CLASS( CAttr_ACT_RegCTiming );
	m_defObjList[_T("A_ExcCGroup")]		= RUNTIME_CLASS( CAttr_ACT_ExcCGroup );
	m_defObjList[_T("A_SkipCont")]		= RUNTIME_CLASS( CAttr_ACT_SkipCont );
	m_defObjList[_T("A_ObjWPS")]		= RUNTIME_CLASS( CAttr_ACT_ObjWPS );
	m_defObjList[_T("A_Dojo")]			= RUNTIME_CLASS( CAttr_ACT_Dojo );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//
	//	TS entity class name
	//
	//////////////////////////////////////////////////////////////////////////

	// Event
	m_defObjName[_T("E_ColObject")]		= _T("CDboTSColObject");
	m_defObjName[_T("E_ClickObject")]	= _T("CDboTSClickObject");
	m_defObjName[_T("E_ClickNPC")]		= _T("CDboTSClickNPC");
	m_defObjName[_T("E_ItemUse")]		= _T("CDboTSItemUse");
	m_defObjName[_T("E_ItemGet")]		= _T("CDboTSItemGet");
	m_defObjName[_T("E_ItemEquip")]		= _T("CDboTSItemEquip");
	m_defObjName[_T("E_RcvSvrEvt")]		= _T("CDboTSRcvSvrEvt");
	m_defObjName[_T("E_ScoutUse")]		= _T("CDboTSScoutUse");
	m_defObjName[_T("E_SkillUse")]		= _T("CDboTSSkillUse");
	m_defObjName[_T("E_ColRgn")]		= _T("CDboTSColRgn");
	m_defObjName[_T("E_RB")]			= _T("CDboTSRB");
	m_defObjName[_T("E_ClickMob")]		= _T("CDboTSClickMob");
	m_defObjName[_T("E_BindStone")]		= _T("CDboTSBindStone");
	m_defObjName[_T("E_SearchQuest")]	= _T("CDboTSSearchQuest");
	m_defObjName[_T("E_ItemUpgrade")]	= _T("CDboTSItemUpgrade");
	m_defObjName[_T("E_Teleport")]		= _T("CDboTSTeleport");
	m_defObjName[_T("E_Budokai")]		= _T("CDboTSBudokai");
	m_defObjName[_T("E_SlotMachine")]	= _T("CDboTSSlotMachine");
	m_defObjName[_T("E_HoipoiMix")]		= _T("CDboTSHoipoiMix");
	m_defObjName[_T("E_PrivateShop")]	= _T("CDboTSPrivateShop");
	m_defObjName[_T("E_FreeBattle")]	= _T("CDboTSFreeBattle");
	m_defObjName[_T("E_ItemIdentity")]	= _T("CDboTSItemIdentity");
	m_defObjName[_T("E_UseMail")]		= _T("CDboTSUseMail");
	m_defObjName[_T("E_Party")]			= _T("CDboTSParty");

	// Condition
	m_defObjName[_T("C_ClrQst")]		= _T("CDboTSCheckClrQst");
	m_defObjName[_T("C_Item")]			= _T("CDboTSCheckItem");
	m_defObjName[_T("C_Lvl")]			= _T("CDboTSCheckLvl");
	m_defObjName[_T("C_PCCls")]			= _T("CDboTSCheckPCCls");
	m_defObjName[_T("C_PCRace")]		= _T("CDboTSCheckPCRace");
	m_defObjName[_T("C_QItem")]			= _T("CDboTSCheckQItem");
	m_defObjName[_T("C_SSM")]			= _T("CDboTSCheckSSM");
	m_defObjName[_T("C_SToCEvt")]		= _T("CDboTSCheckSToCEvt");
	m_defObjName[_T("C_WItem")]			= _T("CDboTSCheckWItem");
	m_defObjName[_T("C_ObjItem")]		= _T("CDboTSCheckObjItem");
	m_defObjName[_T("C_InNPC")]			= _T("CDboTSCheckInNPC");
	m_defObjName[_T("C_CheckObjState")]	= _T("CDboTSCheckObjState");
	m_defObjName[_T("C_CheckInWorld")]	= _T("CDboTSCheckInWorld");
	m_defObjName[_T("C_CheckOPObject")]	= _T("CDboTSCheckOPObject");
	m_defObjName[_T("C_CheckHasCoupon")]	= _T("CDboTSCheckHasCoupon");
	m_defObjName[_T("C_CheckNPCDead")]	= _T("CDboTSCheckNPCDead");
	m_defObjName[_T("C_CheckDistNPC")]	= _T("CDboTSCheckDistWithNPC");
	m_defObjName[_T("C_CheckSToCDelivery")]	= _T("CDboTSCheckSToCDelivery");
	m_defObjName[_T("C_CheckAttachObj")]	= _T("CDboTSCheckAttachObj");
	m_defObjName[_T("C_CheckProgQuest")]	= _T("CDboTSCheckProgQuest");
	m_defObjName[_T("C_CheckReputation")]	= _T("CDboTSCheckReputation");
	m_defObjName[_T("C_CheckBudokaiState")]	= _T("CDboTSCheckBudokaiState");
	m_defObjName[_T("C_CheckProgCustomEvt")]	= _T("CDboTSCheckCustomEvent");
	m_defObjName[_T("C_CheckAvatarDead")]	= _T("CDboTSCheckAvatarDead");
	m_defObjName[_T("C_CheckOutRgn")]		= _T("CDboTSCheckOutRGN");

	// Action
	m_defObjName[_T("A_Dir")]			= _T("CDboTSActDir");
	m_defObjName[_T("A_ETimerS")]		= _T("CDboTSActETimerS");
	m_defObjName[_T("A_ETimerE")]		= _T("CDboTSActETimerE");
	m_defObjName[_T("A_InSSM")]			= _T("CDboTSActInSSM");
	m_defObjName[_T("A_Item")]			= _T("CDboTSActItem");
	m_defObjName[_T("A_NPCConv")]		= _T("CDboTSActNPCConv");
	m_defObjName[_T("A_OPCam")]			= _T("CDboTSActOPCam");
	m_defObjName[_T("A_QItem")]			= _T("CDboTSActQItem");
	m_defObjName[_T("A_RegQInfo")]		= _T("CDboTSActRegQInfo");
	m_defObjName[_T("A_SToCEvt")]		= _T("CDboTSActSToCEvt");
	m_defObjName[_T("A_TWaitTS")]		= _T("CDboTSActTWaitTS");
	m_defObjName[_T("A_TSState")]		= _T("CDboTSActTSState");
	m_defObjName[_T("A_Potal")]			= _T("CDboTSActPortal");
	m_defObjName[_T("A_ObjState")]		= _T("CDboTSActObjState");
	m_defObjName[_T("A_ConcCheck")]		= _T("CDboTSActConcCheck");
	m_defObjName[_T("A_ObjConv")]		= _T("CDboTSActObjConv");
	m_defObjName[_T("A_Hint")]			= _T("CDboTSActHint");
	m_defObjName[_T("A_SendSvrEvt")]	= _T("CDboTSActSendSvrEvt");
	m_defObjName[_T("A_TMQStage")]		= _T("CDboTSActTMQStage");
	m_defObjName[_T("A_OpenWindow")]	= _T("CDboTSActOpenWindow");
	m_defObjName[_T("A_TMQTimeBonus")]	= _T("CDboTSActTMQTimeBonus");
	m_defObjName[_T("A_BroadCast")]		= _T("CDboTSActTelecast");
	m_defObjName[_T("A_DirIndicator")]	= _T("CDboTSActDirIndicator");
	m_defObjName[_T("A_OperateObject")]	= _T("CDboTSActOPObject");
	m_defObjName[_T("A_Drop")]			= _T("CDboTSActDrop");
	m_defObjName[_T("A_RmvCoupon")]		= _T("CDboTSActRmvCoupon");
	m_defObjName[_T("A_Escort")]		= _T("CDboTSActEscort");
	m_defObjName[_T("A_TMQInfoShow")]	= _T("CDboTSActTMQInfoShow");
	m_defObjName[_T("A_WorldPlayScript")]	= _T("CDboTSActWorldPlayScript");
	m_defObjName[_T("A_SWProbSF")]		= _T("CDboTSActSWProbSF");
	m_defObjName[_T("A_PCConv")]		= _T("CDboTSActPCConv");
	m_defObjName[_T("A_CustomEvt")]		= _T("CDboTSActCustomEvt");
	m_defObjName[_T("A_Minormatch")]	= _T("CDboTSActTelMinorMatch");
	m_defObjName[_T("A_Mudosa")]		= _T("CDboTSActTelMudosa");
	m_defObjName[_T("A_PIDgn")]			= _T("CDboTSActPIDgn");
	m_defObjName[_T("A_TLQ")]			= _T("CDboTSActTLQ");
	m_defObjName[_T("A_DoSkill")]		= _T("CDboTSActDoSkill");
	m_defObjName[_T("A_TObjFriendly")]	= _T("CDboTSActTObjFriendly");
	m_defObjName[_T("A_BroadMsg")]		= _T("CDboTSActBroadMsg");
	m_defObjName[_T("A_MiniNarration")]	= _T("CDboTSActMiniNarration");
	m_defObjName[_T("A_RegCTiming")]	= _T("CDboTSActRegCTiming");
	m_defObjName[_T("A_ExcCGroup")]		= _T("CDboTSActExcCGroup");
	m_defObjName[_T("A_SkipCont")]		= _T("CDboTSActSkipCont");
	m_defObjName[_T("A_ObjWPS")]		= _T("CDboTSActObjWPS");
	m_defObjName[_T("A_Dojo")]			= _T("CDboTSActDojo");

	// Container
	m_defObjName[_T("CT_GAct")]			= _T("CDboTSContGAct");
	m_defObjName[_T("CT_GCond")]		= _T("CDboTSContGCond");
	m_defObjName[_T("CT_UsrSel")]		= _T("CDboTSContUsrSel");
	m_defObjName[_T("CT_Reward")]		= _T("CDboTSContReward");
	m_defObjName[_T("CT_Start")]		= _T("CDboTSContStart");
	m_defObjName[_T("CT_End")]			= _T("CDboTSContEnd");
	m_defObjName[_T("CT_Narration")]	= _T("CDboTSContNarration");
	m_defObjName[_T("CT_Proposal")]		= _T("CDboTSContProposal");
	m_defObjName[_T("CT_Switch")]		= _T("CDboTSContSwitch");
	m_defObjName[_T("CT_UnifiedNarration")]		= _T("CDboTSContUnifiedNarration");

	//////////////////////////////////////////////////////////////////////////
}

CAttrEntityMng::~CAttrEntityMng( void )
{
	m_defObjList.clear();
	m_defObjName.clear();
}

CAttr_Page* CAttrEntityMng::GetPage( CString strType )
{
	mapdef_OBJLIST::iterator it = m_defObjList.find( strType.GetBuffer() );
	if ( it == m_defObjList.end() ) return NULL;
	return DYNAMIC_DOWNCAST( CAttr_Page, it->second->CreateObject() );
}

CString CAttrEntityMng::GetName( CString strType )
{
	mapdef_OBJNAME::iterator it = m_defObjName.find( strType.GetBuffer() );
	if ( it == m_defObjName.end() ) return CString();
	return CString( it->second.c_str() );
}
