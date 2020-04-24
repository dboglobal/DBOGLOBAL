#include "precomp_trigger.h"
#include "DboTSEntityFactory.h"
#include "DboTSEventFactoryType.h"
#include "DboTSCondFactoryType.h"
#include "DboTSActFactoryType.h"
#include "DboTSContFactoryType.h"


/**
	Entity factory
*/


void CDboTSEntityFactory::RegisterEntityType( void )
{
	CNtlTSEntityFactory::RegisterEntityType();

	// Register event types
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSClickNPC::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSClickNPC;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSClickObject::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSClickObject;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSColObject::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSColObject;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSItemUse::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSItemUse;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSItemGet::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSItemGet;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSItemEquip::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSItemEquip;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSScoutUse::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSScoutUse;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSRcvSvrEvt::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSRcvSvrEvt;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSSkillUse::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSSkillUse;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSColRgn::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSColRgn;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSRB::OBJ_CLASS_NAME()]				= new CDboTSEntityFactoryType_CDboTSRB;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSClickMob::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSClickMob;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSSearchQuest::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSSearchQuest;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSBindStone::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSBindStone;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSItemUpgrade::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSItemUpgrade;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSTeleport::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSTeleport;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSBudokai::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSBudokai;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSSlotMachine::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSSlotMachine;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSHoipoiMix::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSHoipoiMix;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSPrivateShop::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSPrivateShop;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSFreeBattle::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSFreeBattle;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSItemIdentity::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSItemIdentity;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSUseMail::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSUseMail;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSParty::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSParty;
	//new
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSClickSideIcon::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSClickSideIcon;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSDialogOpen::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSDialogOpen;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSQuest::OBJ_CLASS_NAME()]				= new CDboTSEntityFactoryType_CDboTSQuest;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSLevelCheck::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSLevelCheck;


	// Register condition types
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckLvl::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckLvl;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckPCCls::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckPCCls;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckPCRace::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckPCRace;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckQItem::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckQItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckClrQst::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckClrQst;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckItem::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckSToCEvt::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckSToCEvt;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckSSM::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckSSM;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckWItem::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckWItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckObjItem::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckObjItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckInNPC::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSCheckInNPC;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckObjState::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckObjState;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckInWorld::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckInWorld;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckOPObject::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckOPObject;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckHasCoupon::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckHasCoupon;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckNPCDead::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckNPCDead;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckDistWithNPC::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckDistWithNPC;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckSToCDelivery::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckSToCDelivery;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckAttachObj::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckAttachObj;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckProgQuest::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckProgQuest;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckReputation::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckReputation;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckBudokaiState::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckBudokaiState;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckCustomEvent::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckCustomEvent;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckAvatarDead::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckAvatarDead;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSCheckOutRGN::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSCheckOutRGN;

	// Register action types
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActQItem::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActQItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActItem::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActItem;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActETimerS::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActETimerS;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActETimerE::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActETimerE;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActSToCEvt::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActSToCEvt;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActNPCConv::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActNPCConv;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActRegQInfo::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActRegQInfo;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActDir::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSActDir;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActOPCam::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActOPCam;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTWaitTS::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActTWaitTS;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActInSSM::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActInSSM;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTSState::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActTSState;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActPortal::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActPortal;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActObjState::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActObjState;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActConcCheck::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActConcCheck;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActObjConv::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActObjConv;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActHint::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActHint;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActSendSvrEvt::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActSendSvrEvt;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTMQStage::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTMQStage;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActOpenWindow::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActOpenWindow;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTMQTimeBonus::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTMQTimeBonus;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTelecast::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTelecast;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActDirIndicator::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActDirIndicator;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActOPObject::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActOPObject;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActDrop::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActDrop;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActRmvCoupon::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActRmvCoupon;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActEscort::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActEscort;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTMQInfoShow::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTMQInfoShow;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActWorldPlayScript::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActWorldPlayScript;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActSWProbSF::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActSWProbSF;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActPCConv::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActPCConv;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActCustomEvt::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActCustomEvt;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTelMudosa::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTelMudosa;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTelMinorMatch::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTelMinorMatch;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActPIDgn::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActPIDgn;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTLQ::OBJ_CLASS_NAME()]			= new CDboTSEntityFactoryType_CDboTSActTLQ;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActDoSkill::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActDoSkill;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActTObjFriendly::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActTObjFriendly;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActBroadMsg::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActBroadMsg;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActMiniNarration::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActMiniNarration;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActRegCTiming::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActRegCTiming;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActExcCGroup::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActExcCGroup;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActSkipCont::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActSkipCont;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActObjWPS::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActObjWPS;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActDojo::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActDojo;
	//new
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActOutMsg::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActOutMsg;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActWPSFD::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActWPSFD;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActMail::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActMail;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActAvatarDead::OBJ_CLASS_NAME()]	= new CDboTSEntityFactoryType_CDboTSActAvatarDead;
	m_defEntityTypeList[CDboTSEntityFactoryType_CDboTSActSkyDgn::OBJ_CLASS_NAME()]		= new CDboTSEntityFactoryType_CDboTSActSkyDgn;


	// Register container types
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContGCond::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContGCond;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContStart::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContStart;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContGAct::OBJ_CLASS_NAME()]			= new CDboTSContFactoryType_CDboTSContGAct;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContReward::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContReward;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContEnd::OBJ_CLASS_NAME()]			= new CDboTSContFactoryType_CDboTSContEnd;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContUsrSel::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContUsrSel;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContNarration::OBJ_CLASS_NAME()]	= new CDboTSContFactoryType_CDboTSContNarration;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContProposal::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContProposal;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContSwitch::OBJ_CLASS_NAME()]		= new CDboTSContFactoryType_CDboTSContSwitch;
	m_defEntityTypeList[CDboTSContFactoryType_CDboTSContUnifiedNarration::OBJ_CLASS_NAME()]	= new CDboTSContFactoryType_CDboTSContUnifiedNarration;
}
