#include "precomp_ntlsimulation.h"
#include "NtlSobManager.h"

// shared
#include "NtlWorld.h"
#include "TableContainer.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include "NtlFrameSkipScheduler.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlAnimEventData.h"
#include "NtlPLEntity.h"
#include "NtlAnimEventData.h"

#include "NtlPLCharacter.h"

// framework
#include "NtlTimer.h"

// simulation
#include "NtlSob.h"
#include "NtlSobGroup.h"
#include "NtlSobFactory.h"
#include "NtlSysEvent.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSobElapsedController.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobWorldItem.h"
#include "NtlSLLogic.h"
#include "ObjectTable.h"
#include "NtlSLApi.h"
#include "NtlSobProxy.h"
#include "NtlSobCharProxy.h"
#include "NtlSLGlobal.h"
#include "NtlFSMStateBase.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlCameraManager.h"
#include "NtlSobStatusAnimSyncManager.h"
#include "NtlSobCharPerfController.h"


CNtlSobManager* CNtlSobManager::m_pInstance = 0;

RwReal CNtlSobManager::m_fOptionOutRange = 150;


CNtlSobManager::CNtlSobManager()
{
	NTL_ASSERT(m_pInstance == NULL, "Simulaiton object manager instance is not NULL");
	m_pInstance = this;
	m_bActive	= TRUE;
	m_fOptiionRangeTick		= 0.0f;

	m_pAnimSyncManager = NTL_NEW CNtlSobStatusAnimSyncManager;

	m_pCharPerfCtrl = NTL_NEW CNtlSobCharPerfController;
	m_pMoveFrameSkip = NTL_NEW CNtlFrameSkipScheduler;
}

CNtlSobManager::~CNtlSobManager()
{
	NTL_DELETE(m_pMoveFrameSkip);
	NTL_DELETE(m_pCharPerfCtrl);
	NTL_DELETE(m_pAnimSyncManager);

	m_pInstance = 0; 
}

void CNtlSobManager::SetOptionOutRange(RwReal fRange)
{
	m_fOptionOutRange = fRange;
}

RwReal CNtlSobManager::GetOptionOutRange()
{
	return m_fOptionOutRange;
}


CNtlSobManager* CNtlSobManager::GetInstance(void)
{
	return m_pInstance;
}

RwBool CNtlSobManager::Create(void)
{
	NTL_FUNCTION("CNtlSobManager::Create");

	CNtlFSMStateBase::Terminate(FALSE);

	LinkMsg(g_EventUpdateTick, 0, SLUPDATE_PRIORITY_SOB);

	LinkMsg(g_EventActionMapSkillUse);

	LinkMsg(g_EventSobTargetSelect);
	LinkMsg(g_EventSobTargetSelectRelease); // required to remove the yellow effect from selected target
	LinkMsg(g_EventSobAttackMarkRelease);
    LinkMsg(g_EventSobTargetMarkSelect);
    LinkMsg(g_EventSobTargetMarkRelease);
	LinkMsg(g_EventSobGotFocus);
	LinkMsg(g_EventSobLostFocus);
	LinkMsg(g_EventSobAttackSelect);
	LinkMsg(g_EventSobCreate);
	LinkMsg(g_EventSobDelete);
	LinkMsg(g_EventSobSummonPetSpawnSync);
	LinkMsg(g_EventSobMove);
    LinkMsg(g_EventSobMoveSync);
	LinkMsg(g_EventSobSecondDestMove);
	LinkMsg(g_EventSobSplineMove);
	LinkMsg(g_EventSobAdjustMovePosition);
	LinkMsg(g_EventSobDash);
	LinkMsg(g_EventSobFollowMove);
	LinkMsg(g_EventSobMoveStop);
	LinkMsg(g_EventSobDirectionFloat);
	LinkMsg(g_EventSobAdjustMove);
	LinkMsg(g_EventSobStanding);
	LinkMsg(g_EventSobFightingMode);
	LinkMsg(g_EventSobAttack);
	LinkMsg(g_EventSobSpecialAttack);
    LinkMsg(g_EventSobSendAttackEnd);
	LinkMsg(g_EventSobAttacked);
	LinkMsg(g_EventSobPetBeginAttack);
	LinkMsg(g_EventSobHit);
	LinkMsg(g_EventSobAttackFollow);	
	LinkMsg(g_EventSobJump);
	LinkMsg(g_EventSobJumpDirection);
	LinkMsg(g_EventSobJumpEnd);
	LinkMsg(g_EventSobSit);
	LinkMsg(g_EventSobPrivateShop);
	LinkMsg(g_EventPrivateShopState);
	LinkMsg(g_EventSobConvertClass);
    LinkMsg(g_EventSobChangeAdult);
	LinkMsg(g_EventCharTitleSelectNfy);
	LinkMsg(g_EventSobDirectPlay);
	LinkMsg(g_EventSobAnimPlay);
    LinkMsg(g_EventShareTargetSelect);
    LinkMsg(g_EventShareTargetRelease);
    LinkMsg(g_EventTenkaichiMarking);
	LinkMsg(g_EventSobEventTitleEffect);
    LinkMsg(g_EventSobPushingNfy);
	LinkMsg(g_EventSobGuardCrush);
	LinkMsg(g_EventSobMovePatternSync);
	LinkMsg(g_EventSobAirJump);
	LinkMsg(g_EventSobAirAccel);
	
	LinkMsg(g_EventSobItemAdd);
	LinkMsg(g_EventSobItemDelete);
	LinkMsg(g_EventSobItemMove);
	LinkMsg(g_EventSobItemStackMove);
	LinkMsg(g_EventSobEquipChange);
	LinkMsg(g_EventSobItemUpdate);
	LinkMsg(g_EventSobItemUseAction);
	LinkMsg(g_EventSobItemCancel);
	LinkMsg(g_EventSobItemUseResFailed);
	LinkMsg(g_EventSobItemCasting);

	LinkMsg(g_EventChangeGuildName);
	LinkMsg(g_EventChangeGuildEmblem);
	
	LinkMsg(g_EventSobQuestItemAdd);
	LinkMsg(g_EventSobQuestItemDelete);
	LinkMsg(g_EventSobQuestItemMove);
	LinkMsg(g_EventSobQuestItemUpdate);

	LinkMsg(g_EventQuestMark);

	LinkMsg(g_EventSobSkillCancel);
	LinkMsg(g_EventSobSkillAffectingCancel);
	LinkMsg(g_EventSobSkillCasting);
	LinkMsg(g_EventSobSkillAction);
	LinkMsg(g_EventSobSkillActioned);
	LinkMsg(g_EventSobSkillAdd);
	LinkMsg(g_EventSobSkillUpgrade);
	LinkMsg(g_EventSobHTBSkillAdd);
	LinkMsg(g_EventSobHTBSkillAction);
	LinkMsg(g_EventSobSocialAction);
    LinkMsg(g_EventBotCaution_Nfy);
    LinkMsg(g_EventBotHelpMe_Nfy);
	
	LinkMsg(g_EventSobBuffAdd);
	LinkMsg(g_EventSobBuffDrop);
	LinkMsg(g_EventSobBuffRefreshAll);
	LinkMsg(g_EventSobBuffActivate);
	LinkMsg(g_EventSobBuffAutoRecover);

	LinkMsg(g_EventSobStateTransition);
	LinkMsg(g_EventSobServerUpdateState);
	LinkMsg(g_EventSobServerUpdateCondition);
    LinkMsg(g_EventSobServerUpdateAspect);
	LinkMsg(g_EventSobFainting);
    LinkMsg(g_EventSobUpdateLPStatusNfy);

	LinkMsg(g_EventSobGetState);
	LinkMsg(g_EventSobGetFightingMode);
	LinkMsg(g_EventSobGetAirMode);

	LinkMsg(g_EventShowPlayerName);
	LinkMsg(g_EventAnimEnd);
	LinkMsg(g_EventAnimHit);    
	LinkMsg(g_EventAnimFootStep);
	LinkMsg(g_EventWeightTime);
	LinkMsg(g_EventAnimDirect);
	LinkMsg(g_EventSubWeaponActive);
	LinkMsg(g_EventSubWeaponDeActive);
	LinkMsg(g_EventSummon);
	LinkMsg(g_EventAlpha);
	LinkMsg(g_EventTriDoodads);
    LinkMsg(g_EventAnimTMQ);
	LinkMsg(g_EventExplosion);
    LinkMsg(g_EventAnimPostEffect);
    LinkMsg(g_EventAnimColorChange);
    LinkMsg(g_EventAnimStretch);
    LinkMsg(g_EventAnimTrigger);
	LinkMsg(g_EventAnimSkillCancel);

	LinkMsg(g_EventTObjectUpdateState);
    LinkMsg(g_EventCreateEventObject);
	LinkMsg(g_EventThreadLoadingComplete);
    LinkMsg(g_EventCreateWorld);
    LinkMsg(g_EventSobCondConfused);
    LinkMsg(g_EventSobCondTerror);
	LinkMsg(g_EventGambleAniStart);
	LinkMsg(g_EventChangeNickName);
	LinkMsg(g_EventChangeCharName);
	LinkMsg(g_EventTransform);
    LinkMsg(g_EventMobTransform);
    LinkMsg(g_EventTransformCandy);
	LinkMsg(g_EventSobVehicleStart);
	LinkMsg(g_EventSobVehicleEngine);
	LinkMsg(g_EventSobVehicleStunt);
	LinkMsg(g_EventDynamicObjectUpdateState);
    LinkMsg(g_EventSobTransformSequela);    

	LinkMsg(g_EventCinematicScale);

	// event handler function link
    m_mapEventFunc[g_EventSobCreate.Get_pEventId()] = &CNtlSobManager::SobCreateEventHandler;
    m_mapEventFunc[g_EventSobDelete.Get_pEventId()] = &CNtlSobManager::SobDeleteEventHandler;
    m_mapEventFunc[g_EventUpdateTick.Get_pEventId()] = &CNtlSobManager::UpdateTickEventHandler;

    // Events to pass to the Sob object
    m_mapEventFunc[g_EventSobTargetSelect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobTargetSelectRelease.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAttackMarkRelease.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAttackSelect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobTargetMarkSelect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobTargetMarkRelease.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobGotFocus.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobLostFocus.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSummonPetSpawnSync.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobMoveSync.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSecondDestMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobDirectionFloat.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSplineMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAdjustMovePosition.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobDash.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobFollowMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobMoveStop.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAdjustMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemAdd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemDelete.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemUpdate.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemStackMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobEquipChange.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemUseAction.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobQuestItemAdd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobQuestItemDelete.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobQuestItemMove.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobQuestItemUpdate.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemCancel.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobItemUseResFailed.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobItemCasting.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobStanding.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobFightingMode.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAttack.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobSpecialAttack.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAttacked.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSendAttackEnd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobPetBeginAttack.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobHit.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAttackFollow.Get_pEventId()] = &CNtlSobManager::SobEventHandler;    
    m_mapEventFunc[g_EventSobJump.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobJumpDirection.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobJumpEnd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSit.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobPrivateShop.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobConvertClass.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobChangeAdult.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventCharTitleSelectNfy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobDirectPlay.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobAnimPlay.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventActionMapSkillUse.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillCancel.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillAffectingCancel.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillCasting.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillAction.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillActioned.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillAdd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSkillUpgrade.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobSocialAction.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobHTBSkillAdd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobHTBSkillAction.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobBuffAdd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobBuffDrop.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobBuffAutoRecover.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobBuffRefreshAll.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobBuffActivate.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobStateTransition.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobServerUpdateState.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobServerUpdateCondition.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobServerUpdateAspect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobFainting.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobUpdateLPStatusNfy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobGetState.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobGetFightingMode.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobGetAirMode.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventQuestMark.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventTObjectUpdateState.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventDynamicObjectUpdateState.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventBotCaution_Nfy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventBotHelpMe_Nfy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventShareTargetSelect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventTenkaichiMarking.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobEventTitleEffect.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventGambleAniStart.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobPushingNfy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobGuardCrush.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobMovePatternSync.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventChangeGuildName.Get_pEventId()] = &CNtlSobManager::SobEventHandler;	
    m_mapEventFunc[g_EventAnimHit.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventAnimEnd.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventWeightTime.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSubWeaponActive.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSubWeaponDeActive.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSummon.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventAnimColorChange.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventAnimStretch.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventAnimTrigger.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventAnimSkillCancel.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventTransform.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventMobTransform.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventTransformCandy.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobVehicleStart.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobVehicleEngine.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobVehicleStunt.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventDynamicObjectUpdateState.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
    m_mapEventFunc[g_EventSobTransformSequela.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobAirJump.Get_pEventId()] = &CNtlSobManager::SobEventHandler;
	m_mapEventFunc[g_EventSobAirAccel.Get_pEventId()] = &CNtlSobManager::SobEventHandler;

	m_mapEventFunc[g_EventCinematicScale.Get_pEventId()] = &CNtlSobManager::SobEventHandler;

    //////////////////////////////////////////////////////////////////////////
    //Instead of passing directly to the Sob object,

    m_mapEventFunc[g_EventAnimFootStep.Get_pEventId()] = &CNtlSobManager::AnimFootStepEventHandler;
    m_mapEventFunc[g_EventAnimDirect.Get_pEventId()] = &CNtlSobManager::AnimDirectEventHandler;
    m_mapEventFunc[g_EventAlpha.Get_pEventId()] = &CNtlSobManager::AlphaBlendEventHandler;
    m_mapEventFunc[g_EventTriDoodads.Get_pEventId()] = &CNtlSobManager::TriggerObjectEventHandler;
    m_mapEventFunc[g_EventAnimTMQ.Get_pEventId()] = &CNtlSobManager::TMQEventHandler;
    m_mapEventFunc[g_EventExplosion.Get_pEventId()] = &CNtlSobManager::ExplosionHandler;
    m_mapEventFunc[g_EventAnimPostEffect.Get_pEventId()] = &CNtlSobManager::PostEffectEventHandler;
	m_mapEventFunc[g_EventChangeGuildEmblem.Get_pEventId()] = &CNtlSobManager::SobPlayerChangeGuildEmblemHandler;		
	m_mapEventFunc[g_EventPrivateShopState.Get_pEventId()] = &CNtlSobManager::SobPrivateShopStateEventHandler;
    m_mapEventFunc[g_EventShareTargetRelease.Get_pEventId()] = &CNtlSobManager::SobShareTargetReleaseHandler;
	m_mapEventFunc[g_EventShowPlayerName.Get_pEventId()] = &CNtlSobManager::PlayerNameVisibleEventHandler;
	m_mapEventFunc[g_EventCreateEventObject.Get_pEventId()] = &CNtlSobManager::EventObjectEventHandler;
	m_mapEventFunc[g_EventThreadLoadingComplete.Get_pEventId()] = &CNtlSobManager::EventObjectThreadLoadCompleteEventHandler;
    m_mapEventFunc[g_EventCreateWorld.Get_pEventId()] = &CNtlSobManager::EventCreateWorldHandler;
    m_mapEventFunc[g_EventSobCondConfused.Get_pEventId()] = &CNtlSobManager::SobCondConfusedEventHandler;
    m_mapEventFunc[g_EventSobCondTerror.Get_pEventId()] = &CNtlSobManager::SobCondTerrorEventHandler;

	m_mapEventFunc[g_EventChangeCharName.Get_pEventId()] = &CNtlSobManager::ChangeCharName;
	m_mapEventFunc[g_EventChangeNickName.Get_pEventId()] = &CNtlSobManager::SobEventHandler;

	NTL_RETURN(TRUE);
}

void CNtlSobManager::Destroy(void)
{
	NTL_FUNCTION("CNtlSobManager::Destroy");

	CNtlFSMStateBase::Terminate(TRUE);

	m_mapEventFunc.clear();

	// event unlink
	UnLinkMsg(g_EventSobVehicleStunt);
	UnLinkMsg(g_EventSobVehicleEngine);
	UnLinkMsg(g_EventSobVehicleStart);
    UnLinkMsg(g_EventMobTransform);
	UnLinkMsg(g_EventCinematicScale);
    UnLinkMsg(g_EventTransform);
    UnLinkMsg(g_EventTransformCandy);
    UnLinkMsg(g_EventSobTransformSequela);
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventActionMapSkillUse);
	UnLinkMsg(g_EventSobTargetSelect);
	UnLinkMsg(g_EventSobTargetSelectRelease);
	UnLinkMsg(g_EventSobAttackMarkRelease);
    UnLinkMsg(g_EventSobTargetMarkSelect);
    UnLinkMsg(g_EventSobTargetMarkRelease);
	UnLinkMsg(g_EventSobGotFocus);
	UnLinkMsg(g_EventSobLostFocus);
	UnLinkMsg(g_EventSobAttackSelect);
	UnLinkMsg(g_EventSobCreate);
	UnLinkMsg(g_EventSobDelete);
	UnLinkMsg(g_EventSobSummonPetSpawnSync);
	UnLinkMsg(g_EventSobMove);
    UnLinkMsg(g_EventSobMoveSync);
	UnLinkMsg(g_EventSobSecondDestMove);
	UnLinkMsg(g_EventSobSplineMove);
	UnLinkMsg(g_EventSobAdjustMovePosition);
	UnLinkMsg(g_EventSobDash);
	UnLinkMsg(g_EventSobFollowMove);
	UnLinkMsg(g_EventSobMoveStop);
	UnLinkMsg(g_EventSobDirectionFloat);
	UnLinkMsg(g_EventSobAdjustMove);
	UnLinkMsg(g_EventSobStanding);
	UnLinkMsg(g_EventSobFightingMode);
	UnLinkMsg(g_EventSobAttack);
	UnLinkMsg(g_EventSobSpecialAttack);
    UnLinkMsg(g_EventSobSendAttackEnd);
	UnLinkMsg(g_EventSobAttacked);
	UnLinkMsg(g_EventSobPetBeginAttack);
	UnLinkMsg(g_EventSobHit);
	UnLinkMsg(g_EventSobAttackFollow);	
	UnLinkMsg(g_EventSobJump);
	UnLinkMsg(g_EventSobJumpDirection);
	UnLinkMsg(g_EventSobJumpEnd);
	UnLinkMsg(g_EventSobSit);
	UnLinkMsg(g_EventSobPrivateShop);
	UnLinkMsg(g_EventPrivateShopState);
	UnLinkMsg(g_EventSobConvertClass);
    UnLinkMsg(g_EventSobChangeAdult);
	UnLinkMsg(g_EventCharTitleSelectNfy);
	UnLinkMsg(g_EventSobDirectPlay);
	UnLinkMsg(g_EventSobAnimPlay);
    UnLinkMsg(g_EventShareTargetSelect);
    UnLinkMsg(g_EventShareTargetRelease);
    UnLinkMsg(g_EventTenkaichiMarking);

	UnLinkMsg(g_EventSobEventTitleEffect);

    UnLinkMsg(g_EventSobPushingNfy);
	UnLinkMsg(g_EventSobGuardCrush);
	UnLinkMsg(g_EventSobMovePatternSync);
	UnLinkMsg(g_EventSobAirJump);
	UnLinkMsg(g_EventSobAirAccel);
	
	UnLinkMsg(g_EventSobItemAdd);
	UnLinkMsg(g_EventSobItemDelete);
	UnLinkMsg(g_EventSobItemMove);
	UnLinkMsg(g_EventSobItemStackMove);
	UnLinkMsg(g_EventSobEquipChange);
	UnLinkMsg(g_EventSobItemUpdate);
	UnLinkMsg(g_EventSobItemUseAction);
	UnLinkMsg(g_EventSobItemCancel);
	UnLinkMsg(g_EventSobItemUseResFailed);
	UnLinkMsg(g_EventSobItemCasting);

	UnLinkMsg(g_EventChangeGuildName);	
	UnLinkMsg(g_EventChangeGuildEmblem);	

	UnLinkMsg(g_EventSobQuestItemAdd);
	UnLinkMsg(g_EventSobQuestItemDelete);
	UnLinkMsg(g_EventSobQuestItemMove);
	UnLinkMsg(g_EventSobQuestItemUpdate);

	UnLinkMsg(g_EventQuestMark);
	
	UnLinkMsg(g_EventSobSkillCancel);
	UnLinkMsg(g_EventSobSkillAffectingCancel);
	UnLinkMsg(g_EventSobSkillCasting);
	UnLinkMsg(g_EventSobSkillAction);
	UnLinkMsg(g_EventSobSkillActioned);
	UnLinkMsg(g_EventSobSkillAdd);
	UnLinkMsg(g_EventSobSkillUpgrade);
	UnLinkMsg(g_EventSobHTBSkillAdd);
	UnLinkMsg(g_EventSobHTBSkillAction);
	UnLinkMsg(g_EventSobSocialAction);
		
	UnLinkMsg(g_EventSobBuffAdd);
	UnLinkMsg(g_EventSobBuffDrop);
	UnLinkMsg(g_EventSobBuffRefreshAll);
	UnLinkMsg(g_EventSobBuffActivate);
	UnLinkMsg(g_EventSobBuffAutoRecover);

	UnLinkMsg(g_EventSobStateTransition);
	UnLinkMsg(g_EventSobServerUpdateState);
	UnLinkMsg(g_EventSobServerUpdateCondition);	
    UnLinkMsg(g_EventSobServerUpdateAspect);
	UnLinkMsg(g_EventSobFainting);
    UnLinkMsg(g_EventSobUpdateLPStatusNfy);

	UnLinkMsg(g_EventSobGetState);
	UnLinkMsg(g_EventSobGetFightingMode);
	UnLinkMsg(g_EventSobGetAirMode);

	UnLinkMsg(g_EventShowPlayerName);
	UnLinkMsg(g_EventAnimFootStep);
	UnLinkMsg(g_EventAnimHit);
	UnLinkMsg(g_EventAnimEnd);
	UnLinkMsg(g_EventWeightTime);
	UnLinkMsg(g_EventAnimDirect);
	UnLinkMsg(g_EventSubWeaponActive);
	UnLinkMsg(g_EventSubWeaponDeActive);
	UnLinkMsg(g_EventSummon);
	UnLinkMsg(g_EventAlpha);
	UnLinkMsg(g_EventTriDoodads);
    UnLinkMsg(g_EventAnimTMQ);
	UnLinkMsg(g_EventExplosion);
    UnLinkMsg(g_EventAnimPostEffect);
    UnLinkMsg(g_EventAnimColorChange);
    UnLinkMsg(g_EventAnimStretch);
    UnLinkMsg(g_EventAnimTrigger);
	UnLinkMsg(g_EventAnimSkillCancel);

	UnLinkMsg(g_EventTObjectUpdateState);
	UnLinkMsg(g_EventDynamicObjectUpdateState);

    UnLinkMsg(g_EventCreateEventObject);
	UnLinkMsg(g_EventThreadLoadingComplete);
    UnLinkMsg(g_EventCreateWorld);

    UnLinkMsg(g_EventBotCaution_Nfy);
    UnLinkMsg(g_EventBotHelpMe_Nfy);

    UnLinkMsg(g_EventSobCondConfused);
    UnLinkMsg(g_EventSobCondTerror);

	UnLinkMsg(g_EventGambleAniStart);

	UnLinkMsg(g_EventChangeNickName);
	UnLinkMsg(g_EventChangeCharName);
	UnLinkMsg(g_EventDynamicObjectUpdateState);

	RemoveAllObject();

	NTL_RETURNVOID();
}

void CNtlSobManager::Reset(void)
{
	CNtlFSMStateBase::Terminate(TRUE);

	RemoveAllObject();
	SetActive(TRUE);
}

void CNtlSobManager::SetActive(RwBool bActive)
{
	m_bActive = bActive;
}

RwBool CNtlSobManager::GetActive(void) const
{
	return m_bActive;
}

void CNtlSobManager::UpdateOptionRangeOut(RwReal fElapsed)
{
	m_fOptiionRangeTick += fElapsed;

	if(m_fOptiionRangeTick >= 0.2f)
	{
		RwReal fSobDist;
		RwV2d v2dCamPos, v2dSobPos, v2dSub;
		MapVisible::iterator out_it;

		const RwV3d *pCamPos = GetNtlGameCameraManager()->GetCameraPos();
		
		v2dCamPos.x = pCamPos->x;
		v2dCamPos.y = pCamPos->z;

		RwInt32 arrClassId[4] = { SLCLASS_PLAYER, SLCLASS_NPC, SLCLASS_MONSTER, SLCLASS_PET };

		for(RwInt32 i = 0; i < 4; i++)
		{
			RwUInt32 uiClassId = arrClassId[i];
			CNtlSobGroup* pSobGroup = GetSobGroup(uiClassId);
			if(pSobGroup)
			{
				RwBool bRangeOut;
				RwV3d vPos;
				CNtlSob *pSobObj;
				CNtlSobProxy *pSobProxy;
				CNtlSobProxySystemEffect *pSystemEffect;

				SERIAL_HANDLE hSerialId;
				
				CNtlSobGroup::MapObject::iterator it;
				MapVisible::iterator itvisible;

				CNtlSobGroup::MapObject& mapObject = pSobGroup->GetObjects();

				for(it = mapObject.begin(); it != mapObject.end(); ++it)
				{
					pSobObj = (*it).second;
					hSerialId = pSobObj->GetSerialID();
					if(CNtlSobFactory::IsClientCreateSerialId(hSerialId))
						continue;

					// camera와 현재 거리 check
					vPos = pSobObj->GetPosition();
					v2dSobPos.x = vPos.x;
					v2dSobPos.y = vPos.z;

					RwV2dSubMacro(&v2dSub, &v2dSobPos, &v2dCamPos);
					fSobDist = RwV2dLength(&v2dSub);

					bRangeOut = FALSE;
					out_it = m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT].find(hSerialId);
					if(out_it != m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT].end())
						bRangeOut = TRUE;

					if(fSobDist < m_fOptionOutRange)
					{
						if(bRangeOut)
						{
							// Range Out List에서 삭제한다.
							RemoveOptionRangeOut(hSerialId);
						}
					}
					else
					{
						if(!bRangeOut)
						{
							// Range Out List에 추가한다.
							pSobProxy = pSobObj->GetSobProxy();
							if(pSobProxy == NULL)
								continue;

							pSystemEffect = pSobProxy->AddVisualSystemEffectAlphaBlend(0.0f, 1000000.0f, TRUE);	

							m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT][hSerialId] = pSystemEffect;
						}
					}
				}
			}
		}

		m_fOptiionRangeTick = 0.0f;
	}
}

void CNtlSobManager::Update(RwReal fElapsed)
{
	if(!m_bActive)
		return;

	m_pCharPerfCtrl->Update(fElapsed);

	m_pAnimSyncManager->Update(fElapsed);

	m_pMoveFrameSkip->Update(CNtlTimer::GetFps());

	// slot item이 아닌 경우.
	RwReal fWeightElapsed;
	MapObject::iterator it;
	for(it = m_mapUpdate.begin(); it != m_mapUpdate.end(); )  
	{
		CNtlSob *pSobObj = (*it).second;

		RwUInt32 uiFlags = pSobObj->GetFlags();
		
		if(uiFlags & SLFLAG_UPDATE_PAUSE)
		{
			++it;
			continue;
		}

		fWeightElapsed = fElapsed;
		if(uiFlags & SLFLAG_WEIGHT_ELAPSED_TIME)
			fWeightElapsed = fElapsed*pSobObj->GetWeightElapsedTime();

		pSobObj->Update(fWeightElapsed); 
		
		if(pSobObj->IsFinish())
		{
			RemoveObject(pSobObj->GetClassID(), pSobObj); 
			pSobObj->Destroy(); 
			CNtlSobFactory::DeleteSobFactory(pSobObj);

			it = m_mapUpdate.erase(it);
		}
		else
			++it;
	}

//  잠시 주석 처리
//	UpdateOptionRangeOut(fElapsed);

	// elapsed control update
	GetNtlSobElapsedControlManager()->Update(fElapsed);

	// remove queue 처리.
	if(m_listRemoveQueue.size() == 0)
		return;

	ListHandle::iterator the;
	for(the = m_listRemoveQueue.begin(); the != m_listRemoveQueue.end(); the++)
	{
		m_mapUpdate.erase( (*the) );
	}
	m_listRemoveQueue.clear();
}

CNtlSob* CNtlSobManager::CreateObject(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId /*= INVALID_SLCLASS_ID*/)
{
	NTL_FUNCTION("CNtlSobManager::CreateObject");
	
	CNtlSob* pObj = CNtlSobFactory::CreateSobFactroy(uiClassId);
	pObj->SetClassID(uiClassId); 
	pObj->SetSerialID(hSerialId);

	if(hSerialId == INVALID_SERIAL_ID)
	{
		RwUInt32 uiSerialId;
		if(uiClassId != SLCLASS_TRIGGER_OBJECT)
			uiSerialId = CNtlSobFactory::AcquireSerailId();
		else
			uiSerialId = CNtlSobFactory::AcquireTriggerSerialId();

		pObj->SetSerialID(uiSerialId);
	}

	AddObject(uiClassId, pObj);

	NTL_RETURN(pObj);
}

void CNtlSobManager::DeleteObject(CNtlSob *pObj)
{
	NTL_FUNCTION("CNtlSobManager::DeleteObject");
	
	NTL_PRE(pObj);

	for(RwInt32 i = 0; i < (ESobGroupVisibleFilter)E_SOB_GROUP_VISIBLE_FILTER_END; i++)
	{
		RemoveGroupVisible((ESobGroupVisibleFilter)i, pObj->GetSerialID());
	}
	//DBO_WARNING_MESSAGE("delete obj class: " << pObj->GetClassID());
	MapObject::iterator it = m_mapUpdate.find(pObj->GetSerialID());
	if(it != m_mapUpdate.end())
		m_mapUpdate.erase(it);

	RwInt32 uiClassId = pObj->GetClassID();
	RemoveObject(uiClassId, pObj);

	pObj->Destroy(); 

	CNtlSobFactory::DeleteSobFactory(pObj);

	NTL_RETURNVOID();
}

void CNtlSobManager::DeleteObjectGroup(RwInt32 uiClassId)
{
	NTL_FUNCTION("CNtlSobManager::DeleteObjectGroup");

	CNtlSobGroup *pGroup = FindGroup(uiClassId);
	if(pGroup == NULL)
		NTL_RETURNVOID();

	CNtlSobGroup::MapObject::iterator it;
	CNtlSobGroup::MapObject& mapObject = pGroup->GetObjects();

	while(mapObject.size() > 0)
	{
		it = mapObject.begin();
		DeleteObject((*it).second);
	}

	NTL_RETURNVOID();
}

void CNtlSobManager::RemoveAllObject()
{
	// Not a slot item object.
	MapObject::iterator it;
	for(it = m_mapObject.begin(); it != m_mapObject.end(); )  
	{
		CNtlSob *pObj = (*it).second;
		CNtlSobGroup *pGroup = FindGroup(pObj->GetClassID());
		NTL_PRE(pGroup);
		pGroup->RemoveEntity(pObj); 
		pObj->Destroy(); 
		it = m_mapObject.erase(it);
		CNtlSobFactory::DeleteSobFactory(pObj); 
	}

	// object group
	MapObjectGroup::iterator the;
	for(the = m_mapGroup.begin(); the != m_mapGroup.end(); the++)
	{
		CNtlSobGroup *pGroup = (*the).second;
		pGroup->Destroy();
		NTL_DELETE(pGroup);
	}

	m_mapGroup.clear(); 
	m_mapObject.clear(); 
	m_mapUpdate.clear();

	m_listRemoveQueue.clear();

	m_mapTriggerNpc.clear();
	m_mapTriggerObject.clear();
}

void CNtlSobManager::RemoveWorld(void)
{
	CNtlSobProxy *pSobProxy;
	MapObject::iterator it;
	for(it = m_mapObject.begin(); it != m_mapObject.end(); it++)  
	{
		CNtlSob *pObj = (*it).second;
		pSobProxy = pObj->GetSobProxy();
		if(pSobProxy)
			pSobProxy->RemoveWorld();
	}
}

CNtlSob* CNtlSobManager::FindEntity(RwInt32 uiClassId, SERIAL_HANDLE hSerialId)
{
	MapObject::iterator it;
	it = m_mapObject.find(hSerialId);
	if(it == m_mapObject.end())
		return NULL;
	return (*it).second; 
}

CNtlSobGroup* CNtlSobManager::FindGroup(RwInt32 uiClassId)
{
	MapObjectGroup::iterator it;
	it = m_mapGroup.find(uiClassId); 
	if(it == m_mapGroup.end())
		return NULL;

	return (*it).second; 
}

void CNtlSobManager::AddTriggerIdMapping(RwInt32 uiClassId, CNtlSob *pObj)
{
	if(!Logic_IsTriggerActive(pObj))
		return;

	if(uiClassId == SLCLASS_NPC)
	{
		TBLIDX tblid = Logic_GetNPCTriggerId(pObj);
		if(tblid != INVALID_TBLIDX)
		{
			if( !(pObj->GetSerialID() & MARK_CLIENT_SERIAL) )
				m_mapTriggerNpc[tblid] = pObj->GetSerialID();
		}
	}

	else if(uiClassId == SLCLASS_TRIGGER_OBJECT)
	{
		TBLIDX tblid = Logic_GetTriggerObjectId(pObj);
		if(tblid != INVALID_TBLIDX)
		{
			m_mapTriggerObject[tblid] = pObj->GetSerialID();
		}
	}
}

void CNtlSobManager::RemoveTriggerIdMapping(RwInt32 uiClassId, CNtlSob *pObj)
{
	if(uiClassId == SLCLASS_NPC)
	{
		TBLIDX tblid = Logic_GetNPCTriggerId(pObj);
		if(tblid != INVALID_TBLIDX)
		{
			MapSerialTrigger::iterator it = m_mapTriggerNpc.find(tblid);
			if(it != m_mapTriggerNpc.end())
				m_mapTriggerNpc.erase(it);
		}
	}

	else if(uiClassId == SLCLASS_TRIGGER_OBJECT)
	{
		TBLIDX tblid = Logic_GetTriggerObjectId(pObj);
		if(tblid != INVALID_TBLIDX)
		{
			MapSerialTrigger::iterator it = m_mapTriggerObject.find(tblid);
			if(it != m_mapTriggerObject.end())
				m_mapTriggerObject.erase(it);
		}
	}
}

void CNtlSobManager::AddObject(RwInt32 uiClassId, CNtlSob *pObj)
{
	NTL_FUNCTION("CNtlSobManager::AddObject");

	SERIAL_HANDLE hSerialId = pObj->GetSerialID();
	
	CNtlSob *pFindSobObj = FindEntity(uiClassId, hSerialId);
	NTL_PRE(pFindSobObj == NULL);

	MapObject::iterator it = m_mapObject.find(hSerialId);
	NTL_ASSERTE(it == m_mapObject.end());

	m_mapObject[hSerialId] = pObj;
	if(pObj->GetFlags() & SLFLAG_ADD_UPDATE)
		m_mapUpdate[hSerialId] = pObj;
		

	CNtlSobGroup *pGroup = FindGroup(uiClassId);
	if(pGroup == NULL)
	{
		pGroup = NTL_NEW CNtlSobGroup;
		pGroup->Create(); 
		m_mapGroup[uiClassId] = pGroup;
	}
	
	pGroup->AddEntity(pObj); 
	
	NTL_RETURNVOID();
}

CNtlSobManager::MapObject::iterator CNtlSobManager::RemoveObject(RwInt32 uiClassId, CNtlSob *pObj)
{
	NTL_FUNCTION("CNtlSobManager::RemoveObject");
	
	RwUInt32 uiSerialId = pObj->GetSerialID();
	NTL_PRE(FindEntity(uiClassId, uiSerialId));

    // trigger id mapping remove
    RemoveTriggerIdMapping(uiClassId, pObj);

	CNtlSobGroup *pGroup = FindGroup(uiClassId);
	NTL_PRE(pGroup);
	pGroup->RemoveEntity(pObj);

	MapObject::iterator it;
	it = m_mapObject.find(uiSerialId);
	NTL_RETURN(m_mapObject.erase(it));
}


void CNtlSobManager::AddUpdate(CNtlSob *pSobObj)
{
	NTL_ASSERT(pSobObj->GetSerialID() != INVALID_SERIAL_ID, "CNtlSobManager::AddUpdate");
	m_mapUpdate[pSobObj->GetSerialID()] = pSobObj;
}

void CNtlSobManager::RemoveUpdateQueue(CNtlSob *pSobObj)
{
	m_listRemoveQueue.push_back( pSobObj->GetSerialID() );
}

void CNtlSobManager::RemoveOptionRangeOut(SERIAL_HANDLE hSerialId)
{
	MapVisible::iterator it;
	it = m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT].find( hSerialId );

	if(it != m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT].end())
	{
		CNtlSob *pSobObj = GetSobObject(hSerialId);

		if( pSobObj )
		{
			CNtlSobProxy *pSobProxy = pSobObj->GetSobProxy();
			CNtlSobProxySystemEffect *pSystemEffect = (CNtlSobProxySystemEffect*) ( (*it).second );
			pSobProxy->RemoveVisualSystemEffectAlpha(pSystemEffect);
		}

		m_mapGroupVisible[E_SOB_GROUP_VISIBLE_FILTER_RANGE_OUT].erase(it);
	}
}

void CNtlSobManager::SetFlagSobGroup(RwInt32 uiClassID, RwBool bEnable, RwUInt32 nFlag)
{
    CNtlSobGroup *pSobGroup	= GetSobGroup(uiClassID);	
    if(!pSobGroup)
        return;

    RwUInt32 nOrgFlag = 0;
    CNtlSobGroup::MapObject& mapSobGroup= pSobGroup->GetObjects();    
    for (CNtlSobGroup::MapObject::iterator it = mapSobGroup.begin(); it != mapSobGroup.end(); ++it)
    {
        nOrgFlag = (*it).second->GetFlags();
        nOrgFlag = bEnable ? nOrgFlag | nFlag : nOrgFlag & ~nFlag;
        (*it).second->SetFlags(nOrgFlag);
    }
}

void CNtlSobManager::AddGroupVisible(ESobGroupVisibleFilter eFilter, SERIAL_HANDLE hSerialId)
{
	MapVisible::iterator it;
	it = m_mapGroupVisible[eFilter].find( hSerialId );

	if ( it == m_mapGroupVisible[eFilter].end() )
	{
		CNtlSob* pSobObj = GetSobObject( hSerialId );

		if ( pSobObj )
		{
			CNtlSobProxySystemEffect* pSystemEffect = pSobObj->GetSobProxy()->AddVisualSystemEffectAlphaBlend( 0.0f, 100000.0f, TRUE );
			m_mapGroupVisible[eFilter][hSerialId] = pSystemEffect;
		}
	}
}

void CNtlSobManager::RemoveGroupVisible(ESobGroupVisibleFilter eFilter, SERIAL_HANDLE hSerialId)
{
	MapVisible::iterator it;
	it = m_mapGroupVisible[eFilter].find( hSerialId );

	if ( it != m_mapGroupVisible[eFilter].end() )
	{
		CNtlSob *pSobObj = GetSobObject(hSerialId);

		if ( pSobObj )
		{
			CNtlSobProxySystemEffect* pSystemEffect = (CNtlSobProxySystemEffect*) ( (*it).second );

			pSobObj->GetSobProxy()->RemoveVisualSystemEffectAlpha(pSystemEffect);
		}

		m_mapGroupVisible[eFilter].erase(it);
	}
}

void CNtlSobManager::VisibleAllObject(ESobGroupVisibleFilter eFilter, RwUInt32 uiClassId, RwBool bVisible)
{
	if(bVisible)
	{
		CNtlSob *pSobObj;
		CNtlSobProxy *pSobProxy;
		CNtlSobProxySystemEffect *pSystemEffect;

		MapVisible::iterator it;
		for(it = m_mapGroupVisible[eFilter].begin(); it != m_mapGroupVisible[eFilter].end(); )
		{
			pSobObj = GetSobObject((*it).first);

			if( pSobObj )
			{
				if( pSobObj->GetClassID() == uiClassId )
				{
					pSobProxy = pSobObj->GetSobProxy();
					pSystemEffect = (CNtlSobProxySystemEffect*) ( (*it).second );
					pSobProxy->RemoveVisualSystemEffectAlpha(pSystemEffect);
					it = m_mapGroupVisible[eFilter].erase(it);
				}
				else
				{
					it++;
				}
			}
			else
			{
				it = m_mapGroupVisible[eFilter].erase(it);
			}
		}
	}
	else
	{
		CNtlSobGroup* pSobGroup = GetSobGroup(uiClassId);
		if(pSobGroup)
		{
			CNtlSob *pSobObj;
			CNtlSobProxy *pSobProxy;
			CNtlSobProxySystemEffect *pSystemEffect;

			SERIAL_HANDLE hSerialId;
			
			CNtlSobGroup::MapObject::iterator it;
			MapVisible::iterator itvisible;

			CNtlSobGroup::MapObject& mapObject = pSobGroup->GetObjects();

			for(it = mapObject.begin(); it != mapObject.end(); ++it)
			{
				pSobObj = (*it).second;
				hSerialId = pSobObj->GetSerialID();
				itvisible = m_mapGroupVisible[eFilter].find(hSerialId);
				if(itvisible != m_mapGroupVisible[eFilter].end())
					continue;

				pSobProxy = pSobObj->GetSobProxy();
				if(pSobProxy == NULL)
					continue;

				pSystemEffect = pSobProxy->AddVisualSystemEffectAlphaBlend(0.0f, 100000.0f, TRUE);	

				m_mapGroupVisible[eFilter][hSerialId] = pSystemEffect;
			}
		}
	}
}

CNtlSob* CNtlSobManager::GetSobObject (SERIAL_HANDLE hSerialId)
{
	if( hSerialId == INVALID_SERIAL_ID )
		return NULL;

	MapObject::iterator it;
	it = m_mapObject.find(hSerialId); 

	if(it == m_mapObject.end())
		return NULL;
	return (*it).second; 
}

CNtlSobGroup* CNtlSobManager::GetSobGroup(RwInt32 uiClassId)
{
	return FindGroup(uiClassId) ;
}

RwUInt32 CNtlSobManager::GetSobObjectCount(void) const
{
	return (RwUInt32)m_mapObject.size();
}

RwUInt32 CNtlSobManager::GetSobObjectCount(RwInt32 uiClassId)
{
	CNtlSobGroup *pGroup = FindGroup(uiClassId) ;
	if(pGroup == NULL)
		return 0;

	return pGroup->GetCount(); 
}

RwUInt32 CNtlSobManager::GetSobObjectUpdateCount(void) const
{
	return (RwUInt32)m_mapUpdate.size();
}

/// 가장 가까운 루팅 가능한 월드 아이템을 찾아서 반환한다.
CNtlSob* CNtlSobManager::GetSobNearWorldItem(const RwV3d& vLoc)
{
	CNtlSobGroup *pGroup = FindGroup(SLCLASS_WORLD_ITEM);
	if(pGroup == NULL)
		return NULL;

	CNtlSobGroup::MapObject::iterator it;
	CNtlSobGroup::MapObject& mapObject = pGroup->GetObjects();

	CNtlSobWorldItem* pSobObj = NULL;
    CNtlSobWorldItem* pNearSobObj = NULL;
    RwReal fMinLength = NTL_WORLD_ITEM_AUTOLOOTING_RANGE, fLength = 0.0f;

	for(it = mapObject.begin(); it != mapObject.end(); ++it)
	{
		pSobObj = reinterpret_cast<CNtlSobWorldItem*>( (*it).second );
		if( !pSobObj->IsEnableLoot() )
			continue;

		if(pSobObj->IsLootSend())
			continue;

        fLength = CNtlMath::GetLength(pSobObj->GetPosition(), vLoc);
        if(fLength <= fMinLength)
        {
            fMinLength = fLength;
            pNearSobObj = pSobObj;
        }
	}

    if(pNearSobObj)
        return pNearSobObj;

	if(!mapObject.empty())
	{
		for(it = mapObject.begin(); it != mapObject.end(); ++it)
		{
			pSobObj = reinterpret_cast<CNtlSobWorldItem*>( (*it).second );
			pSobObj->SetLootSend(FALSE);
		}
	}

	return NULL;
}

SERIAL_HANDLE CNtlSobManager::GetNpcTriggerSerialFromId(RwUInt32 uiId)
{
	MapSerialTrigger::iterator it = m_mapTriggerNpc.find(uiId);
	if(it == m_mapTriggerNpc.end())
		return INVALID_SERIAL_ID;

	return (*it).second;
}

SERIAL_HANDLE CNtlSobManager::GetObjectTriggerSerialFromId(RwUInt32 uiId)
{
	MapSerialTrigger::iterator it = m_mapTriggerObject.find(uiId);
	if(it == m_mapTriggerObject.end())
		return INVALID_SERIAL_ID;

	return (*it).second;
}

void CNtlSobManager::GameOptionShadowOnOff(RwBool bShadowOnOff)
{
	MapObject::iterator it;

	CNtlSob *pSobObj		= NULL;
	CNtlSobProxy *pSobProxy	= NULL;

	CNtlSobCharDecorationProxy::m_bShadowRender = bShadowOnOff;

	for(it = m_mapObject.begin(); it != m_mapObject.end(); it++)
	{
		pSobObj = (*it).second;

		pSobProxy = pSobObj->GetSobProxy();
		if(pSobProxy)
			pSobProxy->EnableShadowRendering(bShadowOnOff);
	}
}

void CNtlSobManager::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobManager::HandleEvents");

	MapFuncEventHandler::iterator it = m_mapEventFunc.find(pMsg.Id);
	if(it != m_mapEventFunc.end())
	{
		(this->*(*it).second)(pMsg);
	}

	NTL_RETURNVOID();
}


void CNtlSobManager::UpdateTickEventHandler(RWS::CMsg &pMsg)
{
	float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
	Update(fElapsed);
}

void CNtlSobManager::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobBaseCreate *pSobCreate = reinterpret_cast<SNtlEventSobBaseCreate*>(pMsg.pData);
	//DBO_WARNING_MESSAGE("CNtlSobManager::SobCreateEventHandler: class=" << pSobCreate->uiClassId << " hSerialId=" << pSobCreate->hSerialId);
	CNtlSob *pSobObj = CreateObject(pSobCreate->uiClassId, pSobCreate->hSerialId);
	pSobObj->Create();
	pSobCreate->pSobObj = reinterpret_cast<void*>(pSobObj);
	pSobObj->HandleEvents(pMsg);
	pSobObj->PostCreate();

	// Head Name Color
	Logic_SetHeadNameColor(pSobObj);

	// Nick Name Color
	Logic_SetNickNameColor(pSobObj);

	// trigger id mapping add
	AddTriggerIdMapping(pSobObj->GetClassID(), pSobObj);

    // If included in a shared target, mark it.
    RwInt32 nSlot = Logic_isPartyShareTarget(pSobObj->GetSerialID());
    if(nSlot > -1)
    {
        CNtlSLEventGenerator::ShareTargetMarking(pSobObj->GetSerialID(), (RwUInt8)nSlot);
    }

	// Tianhe First Ball title title effect
	// The avatar is handled by the lobby packet handler
    if(pSobCreate->uiClassId == SLCLASS_PLAYER)
    {
		// by daneos
       /* SNtlEventSobPlayerCreate* pData = (SNtlEventSobPlayerCreate*)pSobCreate;
        if(pData->pPcBrief->sMarking.byCode != INVALID_MARKING_TYPE)
        {
            CNtlSLEventGenerator::TenkaichiMarking(pData->hSerialId, pData->pPcBrief->sMarking.byCode);
        }*/
    }
    else if(pSobCreate->uiClassId == SLCLASS_AVATAR)
    {
		// by daneos
        //// The First Ball Mark
        //SNtlEventSobAvatarCreate* pData = (SNtlEventSobAvatarCreate*)pSobCreate;
        //RwUInt8 byCode = GetNtlSLGlobal()->GetAvatarInfo()->sCharPf.sMarking.byCode;
        //if(byCode != INVALID_MARKING_TYPE)
        //{
        //    CNtlSLEventGenerator::TenkaichiMarking(pData->hSerialId, byCode);
        //}
    }

	m_pCharPerfCtrl->SobCreate( pSobObj );
}

void CNtlSobManager::SobDeleteEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobDelete *pSobDelete = reinterpret_cast<SNtlEventSobDelete*>(pMsg.pData);
	CNtlSob *pSobObj = GetSobObject(pSobDelete->hSerialId);
	if(pSobObj)
	{
		m_pCharPerfCtrl->SobDelete( pSobObj );

		DeleteObject(pSobObj);
	}
}

void CNtlSobManager::SobPlayerChangeGuildEmblemHandler(RWS::CMsg &pMsg)
{
	SNtlEventChangleGuildEmblem* pEvent = reinterpret_cast<SNtlEventChangleGuildEmblem*>( pMsg.pData );
	CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( GetSobObject(pEvent->hHandle) );
	if(pSobPlayer)
	{
		CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobPlayer->GetSobProxy() );

		// 게임중에 엠블렘이 변경되었다
		pSobCharProxy->SetEmblemFactor(pEvent->byTypeA, pEvent->byTypeAColor,
									   pEvent->byTypeB, pEvent->byTypeBColor,
									   pEvent->byTypeC, pEvent->byTypeCColor);
		DBO_WARNING_MESSAGE("CNtlSobManager::SobPlayerChangeGuildEmblemHandler");
		Logic_SetHeadNameColor(pSobPlayer);
	}
}

void CNtlSobManager::SobPrivateShopStateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopState *pSobPrivateShopState = reinterpret_cast<SNtlEventPrivateShopState*>(pMsg.pData);
		
	CNtlSob *pSobObj = NULL;
	if (pSobPrivateShopState->IsAvatarPrivateShop)
	{
		pSobObj = GetNtlSLGlobal()->GetSobAvatar();
	}
	else
	{
		pSobObj = GetSobObject(pSobPrivateShopState->pPrivateShopData->hOwner);
	}

	if(pSobObj)
	{
		pSobObj->HandleEvents(pMsg);
	}
}

void CNtlSobManager::PlayerNameVisibleEventHandler(RWS::CMsg &pMsg)
{
	/*
	SNtlEventShowPlayerName *pShowPlayerName = reinterpret_cast<SNtlEventShowPlayerName*>(pMsg.pData);
	CNtlSob *pSobObj = GetSobObject(pShowPlayerName->hSerialId);

	if(pSobObj == NULL)
		return;
	
	RwUInt32 uiClassID = pSobObj->GetClassID();
	if(uiClassID != SLCLASS_NPC)
		return;

	CNtlSLEventGenerator::SobProxyGUIUpdate(pShowPlayerName->hSerialId, NTL_PROXYSYSTEM_LP_VISIBLE, pShowPlayerName->bShow);
	*/
}

void CNtlSobManager::AnimFootStepEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventFootStep *pSobAnimFootStep = reinterpret_cast<SNtlEventFootStep*>(pMsg.pData);
	CNtlSob *pSobObj = GetSobObject(pSobAnimFootStep->uiSerialId);
	if(pSobObj && !Logic_IsCondition((CNtlSobActor*)pSobObj, CHARCOND_FLAG_TRANSPARENT))
	{

		pSobObj->HandleEvents(pMsg); 
	}
}

void CNtlSobManager::AnimDirectEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventDirect *pSobAnimDirect = reinterpret_cast<SNtlEventDirect*>(pMsg.pData);
	SEventDirect *pDirect = reinterpret_cast<SEventDirect*>(pSobAnimDirect->pData);
	if(pDirect->eType == DIRECT_EVENT_TYPE_CAMERA_SHAKE)
	{
		CNtlSLEventGenerator::CameraShake(pSobAnimDirect->uiSerialID, TRUE);
	}
}

void CNtlSobManager::AlphaBlendEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventAlpha *pSobAlpha = reinterpret_cast<SNtlEventAlpha*>(pMsg.pData);
	CNtlSob *pSobObj = GetSobObject(pSobAlpha->uiSerialId);
	if(pSobObj)
	{
		SEventAlpha *pAlphaData = (SEventAlpha*)pSobAlpha->pData;
		if(pAlphaData->eAlphaEventType == SEventAlpha::E_ALPHA_EVENT_CLUMP)
		{
			pSobObj->GetSobProxy()->AddVisualSystemEffectAlphaBlend((RwReal)pAlphaData->nStartAlpha/255.0f, (RwReal)pAlphaData->nDestAlpha/255.0f, 
																	pAlphaData->fFadeTime, pAlphaData->fLifeTime, FALSE);
		}
	}
}

void CNtlSobManager::TriggerObjectEventHandler(RWS::CMsg &pMsg)
{
	sNTL_EVENT_TRI_DOODADS *pTriggerObject = reinterpret_cast<sNTL_EVENT_TRI_DOODADS*>(pMsg.pData);
	
	SERIAL_HANDLE hTriggerObjOffset = Logic_GetCurrentWorldTriggerOffsetHandle();
	SERIAL_HANDLE hSerialId;

	RwV3d vPos;
	// create
	if(pTriggerObject->FlgTriDoodads == 0)
	{
		RwUInt32 NumTriObj = pTriggerObject->vecDat.size();
		for(RwUInt32 i =0; i < NumTriObj; ++i)
		{
			CNtlPLEntity* pPLEntity = static_cast<CNtlPLEntity*>(pTriggerObject->vecDat[i]);
			vPos = pPLEntity->GetPosition();

			RwUInt32 uiWorldId = Logic_GetActiveWorldTableId();
			CObjectTable *pObjectTbl = API_GetTableContainer()->GetObjectTable(uiWorldId);

			sOBJECT_TBLDAT *pObjectTblData = NULL;
			if ( pObjectTbl )
			{
				pObjectTblData = reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTbl->FindData(pPLEntity->GetSID()) );
			}
				
			if ( pObjectTblData )
			{
				hSerialId = MakeTriggerObjectHandle(hTriggerObjOffset, pObjectTblData->dwSequence);
				CNtlSLEventGenerator::SobTriggerObject(SLCLASS_TRIGGER_OBJECT, hSerialId, pPLEntity->GetSID(), vPos, pPLEntity);

#ifndef NDEBUG
				/*const RwChar *pName = pPLEntity->GetName();
				if(pName)
				{
					NtlLogFilePrintArgs("CNtlSobTriggerObjectAttr::HandleEvents => trigger object create( sid id - %d , %s) ", pPLEntity->GetSID(), pName);
				}*/
#endif
			}
			else
			{
#ifndef NDEBUG
				/*hSerialId = INVALID_SERIAL_ID;
				NtlLogFilePrintArgs("CNtlSobTriggerObjectAttr::HandleEvents => object table is null ( trigger object id - %d ) ", pPLEntity->GetSID());*/
#endif
			}
		}
	}

	// delete
	else if(pTriggerObject->FlgTriDoodads == 1)
	{
		RwUInt32 NumTriObj = pTriggerObject->vecDat.size();
		for(RwUInt32 i =0; i < NumTriObj; ++i)
		{
			CNtlPLEntity* pPLEntity = static_cast<CNtlPLEntity*>(pTriggerObject->vecDat[i]);
			if(pPLEntity)
			{
				CNtlSLEventGenerator::SobDelete(pPLEntity->GetSerialID());

#ifndef NDEBUG
			/*	const RwChar *pName = pPLEntity->GetName();
				if(pName)
				{
					NtlLogFilePrintArgs("CNtlSobTriggerObjectAttr::HandleEvents => trigger object delete( sid id - %d , %s ) ", pPLEntity->GetSID(), pName);
				}*/
#endif
			}
		}
	}	
}

void CNtlSobManager::EventObjectEventHandler( RWS::CMsg& pMsg ) 
{
    if(pMsg.Id == g_EventCreateEventObject)
    {
        SNtlPLEventEventObject* pData = reinterpret_cast<SNtlPLEventEventObject*>(pMsg.pData);
        
        if(pData->bCreate)      // Create
        {
            CNtlSLEventGenerator::SobEventObjectCreate(SLCLASS_EVENT_OBJECT, INVALID_SERIAL_ID, pData->pData);
        }
        else                   // Delete
        {
            CNtlPLEntity* pPLEntity = reinterpret_cast<CNtlPLEntity*>(pData->pData);
            if(pPLEntity)
                CNtlSLEventGenerator::SobDelete(pPLEntity->GetSerialID());
        }
    }
}

void CNtlSobManager::EventObjectThreadLoadCompleteEventHandler(RWS::CMsg& pMsg)
{
	SNtlPlThreadLoadingComplete *pThreadLoad = reinterpret_cast<SNtlPlThreadLoadingComplete*>(pMsg.pData);
	CNtlSob *pSobObj = GetSobObject(pThreadLoad->uiSerialID);

	if(pSobObj)
	{
		if(pSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
		{
			RwV3d vDir = pSobObj->GetSobProxy()->GetDirection();
			pSobObj->SetDirection(&vDir);
		}
	}
}

// 현재 캐릭터가 TMQ 상태인지 확인하고, TMQ 상태라면 카메라한테 카메라 연출 이벤트를 보낸다.
void CNtlSobManager::TMQEventHandler( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimTMQ)
    {  
        if(!Logic_IsTMQBoarding(GetNtlSLGlobal()->GetSobAvatar()))
            return;

        SNtlPLEventTMQ* pData = (SNtlPLEventTMQ*)pMsg.pData;
        if(pData->bIn)
        {
            CNtlSLEventGenerator::CameraTimeMachine(pData->uiSeiralID);
        }
        else
        {

        }
    }
}

void CNtlSobManager::ExplosionHandler( RWS::CMsg& pMsg )
{
	SNtlPlEventExplosion* pData = (SNtlPlEventExplosion*)pMsg.pData;
	SEventExplosion* pEventExplosion = (SEventExplosion*)pData->pData;

	CNtlSLEventGenerator::CameraExplosion( pData->uiSerialID, pEventExplosion->eType );
}

void CNtlSobManager::PostEffectEventHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventPostEffect* pData = (SNtlEventPostEffect*)pMsg.pData;    

    // 아래 정의된 클래스의 이벤트만 연출한다.
    // 다른 PC들의 이벤트는 연출하지 않는다.
    CNtlSob* pSob = GetSobObject(pData->uiSerialID);
    if(pSob->GetClassID() == SLCLASS_AVATAR || 
       pSob->GetClassID() == SLCLASS_NPC || 
       pSob->GetClassID() == SLCLASS_MONSTER)
    {
        pSob->HandleEvents(pMsg);        
    }
}


void CNtlSobManager::EventCreateWorldHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventCreateWorld* pData = (SNtlEventCreateWorld*)pMsg.pData;
    
    if(pData->bCreate)      // 월드가 새로 생성될때
    {
        // 아바타만 추가한다.
        GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->AddWorld();
    }
    else    // 월드가 제거 될때
    {
        RemoveWorld();
    }
}

void CNtlSobManager::SobShareTargetReleaseHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventShareTargetRelease* pData = (SNtlEventShareTargetRelease*)pMsg.pData;
    CNtlSob* pSob = GetSobObject(pData->hSerialId);
    if(pSob)
    {
        pSob->HandleEvents(pMsg);
    }

	GetNtlSLGlobal()->GetSobAvatar()->SetTargetID(INVALID_SERIAL_ID);
}

void CNtlSobManager::SobCondConfusedEventHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventCondConfused* pData = (SNtlEventCondConfused*)pMsg.pData;
    CNtlSob* pSob = GetSobObject(pData->hSubject);
    if(pSob)
    {
        // 자기 자신이거나, 자신의 펫인 경우에만 처리한다.
        if(pSob->GetClassID() == SLCLASS_AVATAR || pSob->GetOwnerID() == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            pSob->HandleEvents(pMsg);
        }        
    }
}

void CNtlSobManager::SobCondTerrorEventHandler( RWS::CMsg& pMsg ) 
{
    SNtlEventCondTerror* pData = (SNtlEventCondTerror*)pMsg.pData;
    CNtlSob* pSob = GetSobObject(pData->hSubject);
    if(pSob)
    {
        // 자기 자신이거나, 자신의 펫인 경우에만 처리한다.
        if(pSob->GetClassID() == SLCLASS_AVATAR || pSob->GetOwnerID() == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            pSob->HandleEvents(pMsg);
        }        
    }
}

void CNtlSobManager::SobEventHandler( RWS::CMsg& pMsg ) 
{
    // Pass the event to the Sob object without doing anything.
    SNtlEventSob* pData = (SNtlEventSob*)pMsg.pData;
    CNtlSob* pSob = GetSobObject(pData->hSerialId);
    if(pSob)
    {
        pSob->HandleEvents(pMsg);
    }
}

void CNtlSobManager::ChangeCharName(RWS::CMsg & pMsg)
{
	SNtlEventChangeCharName* pData = (SNtlEventChangeCharName*)pMsg.pData;

	// first we update avatar data (guild, party, friend, ..)
	CNtlSobPlayer* pSob = reinterpret_cast<CNtlSobPlayer*>(GetSobObject(Logic_GetAvatarHandle()));
	if (pSob)
	{
		pSob->HandleEvents(pMsg);

		if (pSob->GetCharID() == pData->charid)
			return; //if we changed name from ourself, we dont need to execute bottom code
	}

	// if we are not the person who executed the code..
	for (MapObject::iterator it = m_mapObject.begin(); it != m_mapObject.end(); it++)
	{
		CNtlSob* pSob = it->second;
		if(pSob->GetClassID() == SLCLASS_PLAYER)
		{
			CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(pSob);

			if (pSobPlayer->GetCharID() == pData->charid)
			{
				pSob->HandleEvents(pMsg);
				break;
			}
		}
	}
}

void CNtlSobManager::ChangeNickName(RWS::CMsg& pMsg)
{
	SNtlEventChangeNickName* pData = (SNtlEventChangeNickName*)pMsg.pData;
	CNtlSob* pSob = GetSobObject(pData->hSerialID);
	if(pSob)
	{
		pSob->HandleEvents( pMsg );

		// Nick Name Color
		Logic_SetNickNameColor(pSob);
	}
}

