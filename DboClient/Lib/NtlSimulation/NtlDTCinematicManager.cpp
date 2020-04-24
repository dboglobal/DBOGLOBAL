#include "precomp_ntlsimulation.h"
#include "NtlDTCinematicManager.h"

// core
#include "NtlDebug.h"
#include "NtlSysEvent.h"
#include "NtlMath.h"

// presentatin
#include "NtlPLEntity.h"
#include "NtlPLSceneManager.h"
#include "NtlPLDef.h"
#include "NtlPLGlobal.h"
#include "NtlPLSceneManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLResourceManager.h"
#include "NtlPLCharacter.h"

// simulation
#include "NtlSobFactory.h"
#include "NtlDTCinematicNode.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSobManager.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"
#include "NtlWorldConcept.h"
#include "NtlSobTriggerObject.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "InputActionMap.h"
#include "NtlSobAvatar.h"
#include "NtlSobCharProxyDecoration.h"


CNtlDTCinematicManager* CNtlDTCinematicManager::m_pInstance = NULL;

CNtlDTCinematicManager::CNtlDTCinematicManager()
{
	NTL_ASSERT(m_pInstance == NULL, "CNtlDTCinematicManager::CNtlDTCinematicManager");

	m_pLayerChain					= NULL;
	m_pGroupChain					= NULL;
	m_pInstance						= this;

	m_fRestoreFov					= 60.0f;
	m_bCameraAsync					= FALSE;
	m_bCameraColliCheck				= TRUE;
	m_bRestoreSkipSimpleMaterial	= CNtlPLCharacter::GetSkipSimpleMaterial();
	m_bCinematicPossible			= TRUE;
	m_bRunning						= FALSE;
	m_bIsLoadScheduling				= FALSE;
	m_bServerAck					= FALSE;

	m_bCanSkip = false;
}

CNtlDTCinematicManager::~CNtlDTCinematicManager()
{
	m_pInstance = NULL;
}

CNtlDTCinematicManager* CNtlDTCinematicManager::GetInstance(void)
{
	return m_pInstance;
}

RwBool CNtlDTCinematicManager::Create(void)
{
	LinkMsg(g_EventESCinNarrationMode);
	LinkMsg(g_EventUpdateTick, 0, SLUPDATE_PRIORITY_CINEMATIC);
	LinkMsg(g_EventCinematicEcho, 0);

	return TRUE;
}

void CNtlDTCinematicManager::Destroy(void)
{
	UnLinkMsg(g_EventESCinNarrationMode);
	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventCinematicEcho);
}

void CNtlDTCinematicManager::Update(RwReal fElapsed)
{
	if(!m_bRunning)
		return;

	CNtlDTCinematicNodeChain *pChain;

	if(!m_listUpdateChain.empty())
	{
		ListNodeChain::iterator it = m_listUpdateChain.begin();
		pChain = (*it);

		pChain->Update(fElapsed);

		if(pChain->IsFinish())
		{
			pChain->Exit();

			it = m_listUpdateChain.erase(it);
			if(!m_listUpdateChain.empty())
			{
				pChain = (*it);

				pChain->Enter();

			}
		}
	}

	if(m_listUpdateChain.empty())
	{
		m_bRunning = FALSE;

		// instance »èÁ¦.
		MapNodeChain::iterator instit;
		for(instit = m_mapInstanceChain.begin(); instit != m_mapInstanceChain.end(); instit++)
		{
			pChain = (*instit).second;

			NTL_DELETE( pChain );
		}

		m_mapInstanceChain.clear();
	}

	if(!m_bRunning)
	{
		Clear();
		
		CNtlSobCharDecorationProxy::m_bShadowCreate = TRUE;
		GetNtlGameCameraManager()->SetDTNodeAttach(FALSE);
		GetNtlGameCameraManager()->SetDTAttributeRestore(TRUE);
		GetNtlGameCameraManager()->SetRoll(0.0f);
		GetNtlGameCameraManager()->SetFov(m_fRestoreFov);
		GetNtlGameCameraManager()->EnableCollisionObj(TRUE);
		GetNtlGameCameraManager()->SetCollisionCheck(TRUE); // m_bCameraColliCheck
		GetNtlWorldConcept()->EnableAction(TRUE);
		GetNtlGameCameraManager()->FinishUnLock();

		GetNtlSLGlobal()->SetWorldUpdateAvatarPosition(TRUE);

		CNtlCameraShakeController::ResetShakeFactor();
	}
}

void CNtlDTCinematicManager::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventUpdateTick)
	{
		float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
		Update(fElapsed);
	}
	else if(pMsg.Id == g_EventCinematicEcho)
	{
		CinematicEchoEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventESCinNarrationMode)
	{
		if (IsRunning() && m_bCanSkip)
		{
			API_GetSLPacketGenerator()->SendCharDirectPlayCancel();
		}
	}
}

void CNtlDTCinematicManager::CinematicEchoEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventCinematicEcho *pEcho = reinterpret_cast<SNtlEventCinematicEcho*>(pMsg.pData);
	if(pEcho->hCinematic == INVALID_CINEMATIC_ID)
		return;

	CNtlDTCinematicNode *pEchoNode = (CNtlDTCinematicNode*)pEcho->hCinematic;

	MapNodeChain::iterator it;
	for(it = m_mapInstanceChain.begin(); it != m_mapInstanceChain.end(); it++)
	{
		if((*it).second->GetNode() == pEchoNode)
		{
			pEchoNode->InteractiveDirectFinish();
		}
	}
}

void CNtlDTCinematicManager::Clear(void)
{
	m_bCanSkip = false;

	if(IsRunning())
	{
		CNtlPLEventGenerator::CinematicBreak();

		if(m_bServerAck)
		{
			API_GetSLPacketGenerator()->SendCharDirectPlayAck();
			m_bServerAck = FALSE;
		}
	}

	MapNodeChain::iterator it;
	for(it = m_mapInstanceChain.begin(); it != m_mapInstanceChain.end(); it++)
	{
		NTL_DELETE( (*it).second );
	}

	ListLiveSob::iterator itsob;
	for(itsob = m_listLiveSob.begin(); itsob != m_listLiveSob.end(); itsob++)
	{
		CNtlSLEventGenerator::SobDelete((*itsob));
	}

	ListLivePLEntity::iterator itpl;
	for(itpl = m_listLivePLEntity.begin(); itpl != m_listLivePLEntity.end(); itpl++)
	{
		if((*itpl).bSobAttach)
		{
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject((*itpl).hSerialId);
			if(pSobObj)
				pSobObj->GetSobProxy()->DeletePLChildEffect((*itpl).pPLEntity);
		}
		else
		{
			GetSceneManager()->DeleteEntity((*itpl).pPLEntity);
		}
	}

	ListLiveTriggerObject::iterator triit;
	for(triit = m_listLiveTriggerObject.begin(); triit != m_listLiveTriggerObject.end(); triit++)
	{
		SERIAL_HANDLE hSerialId = GetNtlSobManager()->GetObjectTriggerSerialFromId((*triit).uiTblId);
		CNtlSLEventGenerator::TObjectUpdateState(hSerialId, (*triit).uiTblId, (*triit).byMainState, (*triit).bySubState, 0);
	}

	ListLiveGui::iterator guiit;
	for(guiit = m_listLiveGui.begin(); guiit != m_listLiveGui.end(); guiit++)
	{
		CNtlSLEventGenerator::CinematicGuiCommand((RwChar*)(*guiit).c_str(), FALSE);
	}

	m_listLiveTriggerObject.clear();
	m_listLiveGui.clear();

	m_mapInstanceChain.clear();
	m_listUpdateChain.clear();

	m_listLiveSob.clear();
	m_listLivePLEntity.clear();

	m_pLayerChain = NULL;
	m_pGroupChain = NULL;	

	GetNtlSLGlobal()->SetWorldUpdateAvatarPosition(TRUE);

	CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_DT);

	CNtlCameraShakeController::ResetShakeFactor();

	GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_DIRECT);

	GetNtlResourceManager()->SetLoadScheduling(m_bIsLoadScheduling);

	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_AVATAR, TRUE);
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_PLAYER, TRUE);
	GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_VEHICLE, TRUE);

	CNtlPLEventGenerator::CinematicBalloonHide();

	CNtlPLCharacter::SetSkipAnimUpdate(TRUE);
	CNtlPLCharacter::SetSkipSimpleMaterial(m_bRestoreSkipSimpleMaterial);
}

void CNtlDTCinematicManager::SetFov(RwReal fFov)
{
	m_fRestoreFov = fFov;
}


void CNtlDTCinematicManager::RegisterSob(SERIAL_HANDLE hSerialId)
{
	m_listLiveSob.push_back(hSerialId);
}

void CNtlDTCinematicManager::UnRegisterSob(SERIAL_HANDLE hSerialId)
{
	ListLiveSob::iterator it;
	for(it = m_listLiveSob.begin(); it != m_listLiveSob.end(); it++)
	{
		if((*it) == hSerialId)
		{
			m_listLiveSob.erase(it);
			return;
		}
	}
}

void CNtlDTCinematicManager::RegisterPLEntity(SERIAL_HANDLE hInstance, RwBool bSobAttach, SERIAL_HANDLE hSerialId, CNtlPLEntity *pPLEntity)
{
	if(pPLEntity == NULL)
		return;

	if(!pPLEntity->IsAutoDelete())
	{
		SLivePLEffect sPLEffect;

		sPLEffect.hInstance		= hInstance;
		sPLEffect.bSobAttach	= bSobAttach;
		sPLEffect.hSerialId		= hSerialId;
		sPLEffect.pPLEntity		= pPLEntity;

		m_listLivePLEntity.push_back(sPLEffect);
	}
}

void CNtlDTCinematicManager::RegisterGuiCmd(const RwChar *pGuiKey)
{
	m_listLiveGui.push_back(pGuiKey);
}

void CNtlDTCinematicManager::UnRegisterGuiCmd(const RwChar *pGuiKey)
{
	ListLiveGui::iterator guiit;
	for(guiit = m_listLiveGui.begin(); guiit != m_listLiveGui.end(); guiit++)
	{
		if((*guiit) == pGuiKey)
		{
			m_listLiveGui.erase(guiit);
			return;
		}
	}
}


CNtlDTCinematicNodeChain* CNtlDTCinematicManager::CreateChainNode(void)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	m_mapInstanceChain[pChain] = pChain;

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainViewShowHide(RwBool bShow, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicViewNode *pViewNode = NTL_NEW CNtlDTCinematicViewNode;

	pViewNode->SetData(bShow, fDelayTime);

	pChain->SetNode(pViewNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainBalloon(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBalloon *pBalloon = NTL_NEW CNtlDTCinematicBalloon;

	pBalloon->SetData(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	pChain->SetNode(pBalloon);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainBalloonBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBalloon *pBalloon = NTL_NEW CNtlDTCinematicBalloon;

	pBalloon->SetDataBegin(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);

	pChain->SetNode(pBalloon);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainBalloonProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBalloon *pBalloon = NTL_NEW CNtlDTCinematicBalloon;

	pBalloon->SetDataProgress(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);

	pChain->SetNode(pBalloon);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainBalloonEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBalloon *pBalloon = NTL_NEW CNtlDTCinematicBalloon;

	pBalloon->SetDataEnd(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	pChain->SetNode(pBalloon);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainBalloonSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBalloon *pBalloon = NTL_NEW CNtlDTCinematicBalloon;

	pBalloon->SetDataSingle(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);

	pChain->SetNode(pBalloon);

	return pChain;

}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainFlash(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicFlash *pFlash = NTL_NEW CNtlDTCinematicFlash;

	pFlash->SetData(pFileName, bLoop, fLifeTime, fDelayTime);

	pChain->SetNode(pFlash);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainMobCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataMobCreate(hSerialId, uiTblId, vLoc, fAngleY, bShow, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainNpcCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataNpcCreate(hSerialId, uiTblId, vLoc, fAngleY, bShow, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobDelete(hSerialId, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobMove(hSerialId, vDest, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, RwBool bGoundMove, RwBool bRunMove, const RwChar *pKey, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobSplineMove(hSerialId, vPos, bGoundMove, bRunMove, pKey, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8 bySubState, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobTriggerObjectState(uiTblId, byMainState, bySubState, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobDirectPlay(hSerialId, uiTblId, bTimeLoop, fLoopTime, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobAnimPlay(hSerialId, uiAnimId, bTimeLoop, fLoopTime, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobAnimPlayPositionSetting(hSerialId, uiAnimId, bTimeLoop, fLoopTime, vPos, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobPositionSetting(hSerialId, vPos, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobDirectSetting(hSerialId, vDir, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainPause(RwReal fLifeTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicPauseNode *pPauseNode = NTL_NEW CNtlDTCinematicPauseNode;
	pPauseNode->SetData(fLifeTime);

	pChain->SetNode(pPauseNode);

	return pChain;
}

CNtlDTCinematicNodeChain * CNtlDTCinematicManager::MakeChainSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSobNode *pSobNode = NTL_NEW CNtlDTCinematicSobNode;
	pSobNode->SetDataSobScale(hSerialId, fScale, fScaleTime, fDelayTime);

	pChain->SetNode(pSobNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeVisualEffectCreate(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId,
																		const RwChar *pBoneName, RwV3d vPos, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicPLEffectNode *pPLEffectNode = NTL_NEW CNtlDTCinematicPLEffectNode;

	if(hSerialId == INVALID_SERIAL_ID)
	{
		pPLEffectNode->SetData(hInstance, pKey, vPos, fDelayTime);
	}
	else
	{
		if(pBoneName == NULL)
		{
			pPLEffectNode->SetData(hInstance, pKey, hSerialId, vPos, fDelayTime);
		}
		else
		{
			pPLEffectNode->SetData(hInstance, pKey, hSerialId, pBoneName, fDelayTime);
		}
	}

	pChain->SetNode(pPLEffectNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainCameraShake(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicCameraShakeNode *pShakeNode = NTL_NEW CNtlDTCinematicCameraShakeNode;
	pShakeNode->SetData(fWeightValue, fHeightValue, fDelayTime);

	pChain->SetNode(pShakeNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainCameraShakeFactor(RwReal fWeightValue, RwReal fHeightValue)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicCameraShakeNode *pShakeNode = NTL_NEW CNtlDTCinematicCameraShakeNode;
	pShakeNode->SetDataShakeFactor(fWeightValue, fHeightValue);

	pChain->SetNode(pShakeNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainCameraShakeFactorReset(void)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicCameraShakeNode *pShakeNode = NTL_NEW CNtlDTCinematicCameraShakeNode;
	pShakeNode->SetDataShakeFactorReset();

	pChain->SetNode(pShakeNode);

	return pChain;
}


CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainFadeInOut(RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicFadeInOutNode *pFadeInOutNode = NTL_NEW CNtlDTCinematicFadeInOutNode;
	pFadeInOutNode->SetData(CINEMATIC_FADEINOUT_SCREEN_PANEL, bFadeIn, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	pChain->SetNode(pFadeInOutNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainFadeInOutTVH(RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicFadeInOutNode *pFadeInOutNode = NTL_NEW CNtlDTCinematicFadeInOutNode;
	pFadeInOutNode->SetData(CINEMATIC_FADEINOUT_SHUTDOWN_TV, bFadeIn, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	pChain->SetNode(pFadeInOutNode);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSound2D(const RwChar *pFileName, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicSoundNode *pSound = NTL_NEW CNtlDTCinematicSoundNode;
	pSound->SetData(CHANNEL_GROUP_JINGLE_MUSIC_WITHOUT_FADE, pFileName, NULL, fDelayTime);

	pChain->SetNode(pSound);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSound3D(const RwChar *pFileName, RwV3d vPos, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicSoundNode *pSound = NTL_NEW CNtlDTCinematicSoundNode;
	pSound->SetData(CHANNEL_GROUP_EFFECT_SOUND, pFileName, &vPos, fDelayTime);

	pChain->SetNode(pSound);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSoundBGM(const RwChar *pFileName, RwBool bLoop)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicSoundNode *pSound = NTL_NEW CNtlDTCinematicSoundNode;
	pSound->SetData(CHANNEL_GROUP_BGM, pFileName, NULL, 0.f, bLoop);

	pChain->SetNode(pSound);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainSoundBGMStop()
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicBGMStop *pSound = NTL_NEW CNtlDTCinematicBGMStop;

	pChain->SetNode(pSound);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainCharacterShadowOnOff(RwBool bOnOff)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicCharacterOptNode *pCharacterOpt = NTL_NEW CNtlDTCinematicCharacterOptNode;
	pCharacterOpt->SetDataShoadow(bOnOff);

	pChain->SetNode(pCharacterOpt);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainCharacterShowOnOff(SERIAL_HANDLE hSerialId, RwBool bOnOff)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicCharacterOptNode *pCharacterOpt = NTL_NEW CNtlDTCinematicCharacterOptNode;
	pCharacterOpt->SetDataShow(hSerialId, bOnOff);

	pChain->SetNode(pCharacterOpt);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainMoonShowOnOff(RwBool bOnOff)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicSceneNode *pScene = NTL_NEW CNtlDTCinematicSceneNode;
	pScene->SetDataMoon(bOnOff);

	pChain->SetNode(pScene);

	return pChain;
}


CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicSceneNode *pScene = NTL_NEW CNtlDTCinematicSceneNode;
	pScene->SetRainBloomLight(fFadeInTime, fFadeOutTime);

	pChain->SetNode(pScene);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicSceneNode *pScene = NTL_NEW CNtlDTCinematicSceneNode;
	pScene->SetRainSkyLight(fFadeInTime, fFadeOutTime);

	pChain->SetNode(pScene);

	return pChain;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainGuiCommand(const RwChar *pGuiCmd, RwBool bShow, RwReal fDelayTime)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicGuiCmdNode *pScene = NTL_NEW CNtlDTCinematicGuiCmdNode;
	pScene->SetData(pGuiCmd, bShow, fDelayTime);

	pChain->SetNode(pScene);

	return pChain;
}


CNtlDTCinematicNodeChain* CNtlDTCinematicManager::MakeChainServerAck(void)
{
	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;
	
	CNtlDTCinematicServerAckNode *pServerAckNode = NTL_NEW CNtlDTCinematicServerAckNode;
	pServerAckNode->SetData();

	pChain->SetNode(pServerAckNode);

	return pChain;
}

void CNtlDTCinematicManager::InsertChain(CNtlDTCinematicNodeChain *pChain)
{
	if(m_pGroupChain == NULL)
	{
		NTL_DELETE( pChain );
		return;
	}
	
	m_pGroupChain->AddChildChain(pChain);

	// insert instance map
	m_mapInstanceChain[pChain] = pChain;
}

void CNtlDTCinematicManager::CinematicPlayCheck(RwReal fStandardPosX, RwReal fStandardPosZ)
{
	m_bCinematicPossible = TRUE;

	RwV3d vPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
	RwV3d vStandardPos;
	CNtlMath::MathRwV3dAssign(&vStandardPos, fStandardPosX, vPos.y, fStandardPosZ);

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vStandardPos, &vPos);
	RwReal fLen = RwV3dLength(&vSub);

	if(fLen > 400.0f)
		m_bCinematicPossible = FALSE;

}

void CNtlDTCinematicManager::CommandBegin(RwReal fStandardPosX, RwReal fStandardPosZ)
{
	Clear();

	CinematicPlayCheck(fStandardPosX, fStandardPosZ);

	if(!IsCinematicPlayPossible())
	{
		API_GetSLPacketGenerator()->SendCharDirectPlayAck();
		return;
	}

	m_bIsLoadScheduling = GetNtlResourceManager()->IsLoadScheduling();
	GetNtlResourceManager()->SetLoadScheduling(FALSE);

	if(!GetNtlWorldConcept()->IsVisiblePCInCinematic())
	{
		GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_AVATAR, FALSE);
		GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_PLAYER, FALSE);
		GetNtlSobManager()->VisibleAllObject(CNtlSobManager::E_SOB_GROUP_VISIBLE_FILTER_CINEMATIC, SLCLASS_VEHICLE, FALSE);
	}

	CNtlSobCharDecorationProxy::m_bShadowCreate = TRUE;
	
	GetNtlGameCameraManager()->SetDTNodeAttach(FALSE);
	GetNtlGameCameraManager()->SetDTAttributeRestore(TRUE);
	GetNtlGameCameraManager()->SetRoll(0.0f);
	GetNtlGameCameraManager()->SetFov(m_fRestoreFov);
	GetNtlGameCameraManager()->EnableCollisionObj(FALSE);
	
	m_fRestoreFov = GetNtlGameCameraManager()->GetFov();
	GetNtlGameCameraManager()->SetDTAttributeRestore(FALSE);

	m_bCameraColliCheck = GetNtlGameCameraManager()->IsCollisionCheck();

	GetNtlGameCameraManager()->SetCollisionCheck(FALSE);
	GetNtlGameCameraManager()->FinishLock();

	GetNtlWorldConcept()->EnableAction(FALSE);

	GetNtlSLGlobal()->SetWorldUpdateAvatarPosition(FALSE);

	GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_DIRECT);

	CNtlPLCharacter::SetSkipAnimUpdate(FALSE);

	m_bRestoreSkipSimpleMaterial = CNtlPLCharacter::GetSkipSimpleMaterial();
	CNtlPLCharacter::SetSkipSimpleMaterial(TRUE);
}

void CNtlDTCinematicManager::CommandEnd(void)
{
	if(!IsCinematicPlayPossible())
		return;

	if(!m_listUpdateChain.empty())
	{
		ListNodeChain::iterator it = m_listUpdateChain.begin();
		(*it)->Enter();
	}

	m_bRunning = TRUE;
}

void CNtlDTCinematicManager::CommandBeginLayer(void)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChainNode = CreateChainNode();

	m_listUpdateChain.push_back(pChainNode);
	m_pLayerChain = pChainNode;

	m_mapInstanceChain[pChainNode] = pChainNode;
}

void CNtlDTCinematicManager::CommandEndLayer(void)
{
	if(!IsCinematicPlayPossible())
		return;

	m_pLayerChain = NULL;
}

void CNtlDTCinematicManager::CommnadBeginGroup(void)
{
	if(!IsCinematicPlayPossible())
		return;

	if(m_pLayerChain)
	{
		CNtlDTCinematicNodeChain *pChainNode = CreateChainNode();
		m_pGroupChain = pChainNode;

		m_pLayerChain->AddSiblingChain(pChainNode);

		m_mapInstanceChain[pChainNode] = pChainNode;
	}
}

void CNtlDTCinematicManager::CommnadEndGroup(void)
{
	if(!IsCinematicPlayPossible())
		return;

	m_pGroupChain = NULL;
}

void CNtlDTCinematicManager::CommandSiblingBegin(void)
{
	if(!IsCinematicPlayPossible())
		return;

	if(m_pLayerChain)
	{
		CNtlDTCinematicNodeChain *pChainNode = CreateChainNode();
		m_pLayerChain->AddSiblingChain(pChainNode);
		m_mapInstanceChain[pChainNode] = pChainNode;
	}
}

void CNtlDTCinematicManager::CommandSiblingEnd(void)
{
	if(!IsCinematicPlayPossible())
		return;
}

void CNtlDTCinematicManager::CommandCameraAsyncBegin(void)
{
	if(!IsCinematicPlayPossible())
		return;

	m_bCameraAsync = TRUE;
}

void CNtlDTCinematicManager::CommandCameraAsyncEnd(void)
{
	if(!IsCinematicPlayPossible())
		return;

	m_bCameraAsync = FALSE;
}

void CNtlDTCinematicManager::CommandCameraLockBegin(void)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataLockUnLock(TRUE, 0.0f);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraLockEnd(void)
{
	if (!IsCinematicPlayPossible())
	{
		return;
	}

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataLockUnLock(FALSE, 0.0f);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandViewShowHide(RwBool bShow, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainViewShowHide(bShow, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandBalloon(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainBalloon(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandBalloonBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainBalloonBegin(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandBalloonProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainBalloonProgress(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandBalloonEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainBalloonEnd(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandBalloonSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainBalloonSingle(hSerialId, uiQuestTextTblId, byBalloonType, byEmotionType, fLifeTime, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandFlash(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainFlash(pFileName, bLoop, fLifeTime, fDelayTime);
	InsertChain(pChain);
}


SERIAL_HANDLE CNtlDTCinematicManager::CommandMobCreate(RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return INVALID_SERIAL_ID;

	SERIAL_HANDLE hSerialId = CNtlSobFactory::AcquireSerailId();

	CNtlDTCinematicNodeChain *pChain = MakeChainMobCreate(hSerialId, uiTblId, vLoc, fAngleY, bShow, fDelayTime);
	InsertChain(pChain);

	return hSerialId;
}

SERIAL_HANDLE CNtlDTCinematicManager::CommandNpcCreate(RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return INVALID_SERIAL_ID;

	SERIAL_HANDLE hSerialId = CNtlSobFactory::AcquireSerailId();

	CNtlDTCinematicNodeChain *pChain = MakeChainNpcCreate(hSerialId, uiTblId, vLoc, fAngleY, bShow, fDelayTime);
	InsertChain(pChain);

	return hSerialId;
}

void CNtlDTCinematicManager::CommandSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobDelete(hSerialId, fDelayTime);
	InsertChain(pChain);
}


void CNtlDTCinematicManager::CommandSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobMove(hSerialId, vDest, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobSplineMove(hSerialId, vPos, TRUE, TRUE, pKey, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobSplineWalkMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobSplineMove(hSerialId, vPos, TRUE, FALSE, pKey, fDelayTime);
	InsertChain(pChain);
}



void CNtlDTCinematicManager::CommandSobSplineFlyMove(SERIAL_HANDLE hSerialId, RwV3d vPos, const RwChar *pKey, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobSplineMove(hSerialId, vPos, FALSE, TRUE, pKey, fDelayTime);
	InsertChain(pChain);
}


void CNtlDTCinematicManager::CommandSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8 bySubState, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobTriggerObjectState(uiTblId, byMainState, bySubState, fDelayTime);
	InsertChain(pChain);

	ListLiveTriggerObject::iterator it;
	for(it = m_listLiveTriggerObject.begin(); it != m_listLiveTriggerObject.end(); it++)
	{
		if( (*it).uiTblId == uiTblId)
			return;
	}

	SERIAL_HANDLE hSerialId = GetNtlSobManager()->GetObjectTriggerSerialFromId(uiTblId);
	CNtlSobTriggerObject *pSobTriObj = reinterpret_cast<CNtlSobTriggerObject*>( GetNtlSobManager()->GetSobObject(hSerialId) );
	if(pSobTriObj)
	{

		SNtlSobTriggerObjectState sTriObjState = pSobTriObj->GetCurrentTriggerObject();

		SLiveTriggerObject sTriObj;
		sTriObj.uiTblId		= uiTblId;
		sTriObj.byMainState	= sTriObjState.byMainState;
		sTriObj.bySubState	= sTriObjState.bySubState;

		m_listLiveTriggerObject.push_back(sTriObj);
	}
}


void CNtlDTCinematicManager::CommandSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobDirectPlay(hSerialId, uiTblId, bTimeLoop, fLoopTime, fDelayTime);
	InsertChain(pChain);
}


void CNtlDTCinematicManager::CommandSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobAnimPlay(hSerialId, uiAnimId, bTimeLoop, fLoopTime, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobAnimPlayPositionSetting(hSerialId, uiAnimId, bTimeLoop, fLoopTime, vPos, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobPositionSetting(hSerialId, vPos, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobDirectSetting(hSerialId, vDir, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobRotateY(SERIAL_HANDLE hSerialId, RwReal fAngleY, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	RwMatrix mat;
	RwMatrixSetIdentity(&mat);
	RwMatrixRotate (&mat, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);		// Yaw
	RwMatrixUpdate(&mat);
	
	CNtlDTCinematicNodeChain *pChain = MakeChainSobDirectSetting(hSerialId, mat.at, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime)
{
	if (!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainSobScale(hSerialId, fScale, fScaleTime, fDelayTime);
	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandPause(RwReal fLifeTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = MakeChainPause(fLifeTime);
	InsertChain(pChain);
}

SERIAL_HANDLE CNtlDTCinematicManager::CommandVisualEffectCreate(const RwChar *pKey, SERIAL_HANDLE hSerialId, const RwChar *pBoneName, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return INVALID_SERIAL_ID;

	SERIAL_HANDLE hInstance = CNtlSobFactory::AcquireSerailId();

	CNtlDTCinematicNodeChain *pChain;
	
	pChain = MakeVisualEffectCreate(hInstance, pKey, hSerialId, pBoneName, CNtlPLGlobal::m_vZeroV3, fDelayTime);

	InsertChain(pChain);

	return hInstance;
}

SERIAL_HANDLE CNtlDTCinematicManager::CommandVisualEffectCreate(const RwChar *pKey, SERIAL_HANDLE hSerialId, RwV3d vOffset, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return INVALID_SERIAL_ID;

	SERIAL_HANDLE hInstance = CNtlSobFactory::AcquireSerailId();

	CNtlDTCinematicNodeChain *pChain;
	
	pChain = MakeVisualEffectCreate(hInstance, pKey, hSerialId, NULL, vOffset, fDelayTime);

	InsertChain(pChain);

	return hInstance;
}

SERIAL_HANDLE CNtlDTCinematicManager::CommandVisualEffectCreate(const RwChar *pKey, RwV3d vWorld, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return INVALID_SERIAL_ID;

	SERIAL_HANDLE hInstance = CNtlSobFactory::AcquireSerailId();

	CNtlDTCinematicNodeChain *pChain;
	
	pChain = MakeVisualEffectCreate(hInstance, pKey, INVALID_SERIAL_ID, NULL, vWorld, fDelayTime);

	InsertChain(pChain);

	return hInstance;
}

void CNtlDTCinematicManager::CommandVisualEffectDelete(SERIAL_HANDLE hInstance)
{
	if(!IsCinematicPlayPossible())
		return;

	ListLivePLEntity::iterator it;
	for(it = m_listLivePLEntity.begin(); it != m_listLivePLEntity.end(); it++)
	{
		if((*it).hInstance == hInstance)
		{
			if((*it).bSobAttach)
			{
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject((*it).hSerialId);
				if(pSobObj)
					pSobObj->GetSobProxy()->DeletePLChildEffect((*it).pPLEntity);
			}
			else
			{
				GetSceneManager()->DeleteEntity((*it).pPLEntity);
			}

			m_listLivePLEntity.erase(it);

			return;
		}
	}
}

void CNtlDTCinematicManager::CommandCameraSoftActorDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
														RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftActorDist(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, vLookAt, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftActorDynamicLookAtDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
																	RwReal fDistance, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftActorDynamicLookAtDist(hSerialId, fHeightOffset, fAngleX, fAngleY, fDistance, fLifeTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftCurrentRotateX(fLastAngle, fInterTime, fLifeTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftCurrentRotateY(fLastAngle, fInterTime, fLifeTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftCurrentRotateZ(fLastAngle, fInterTime, fLifeTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSoftCurrentFov(fLastFov, fInterTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSoftPause(RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;

	pCameraNode->SetDataSoftPause(fLifeTime, FALSE, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSplineSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSPlineSelf(vPos, fAngleX, fAngleY, fAngleZ, pPropKey, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSplineSelfActor(SERIAL_HANDLE hSerialId, RwReal fHeightRate, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSPlineSelfActor(hSerialId, fHeightRate, fAngleX, fAngleY, fAngleZ, pPropKey, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraSplineCurrentDynamicLookAt(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain = NTL_NEW CNtlDTCinematicNodeChain;

	CNtlDTCinematicCameraNode *pCameraNode = NTL_NEW CNtlDTCinematicCameraNode;
	pCameraNode->SetDataSplineCurrentDynamicLookAt(hSerialId, fHeightOffset, pPropKey, fLifeTime, fDelayTime, m_bCameraAsync);

	pChain->SetNode(pCameraNode);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraShake(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainCameraShake(fWeightValue, fHeightValue, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraShakeFactor(RwReal fWeightValue, RwReal fHeightValue)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainCameraShakeFactor(fWeightValue, fHeightValue);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCameraShakeFactorReset(void)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainCameraShakeFactorReset();

	InsertChain(pChain);
}


void CNtlDTCinematicManager::CommandFadeIn(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainFadeInOut(TRUE, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandFadeOut(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainFadeInOut(FALSE, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandFadeInTVH(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainFadeInOutTVH(TRUE, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandFadeOutTVH(RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainFadeInOutTVH(FALSE, fLifeTime, byRed, byGreen, byBlue, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommnadSound2D(const RwChar *pFileName, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainSound2D(pFileName, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandSound3D(const RwChar *pFileName, RwV3d vPos, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainSound3D(pFileName, vPos, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommnadSoundBGM(const RwChar *pFileName, RwBool bLoop)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainSoundBGM(pFileName, bLoop);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommnadSoundBGMStop()
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainSoundBGMStop();

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCharacterShadowOnOff(RwBool bOnOff)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainCharacterShadowOnOff(bOnOff);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandCharacterShowOnOff(SERIAL_HANDLE hSerialId, RwBool bOnOff)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainCharacterShowOnOff(hSerialId, bOnOff);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandMoonOnOff(RwBool bOnOff)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainMoonShowOnOff(bOnOff);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainRainBloomLight(fFadeInTime, fFadeOutTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainRainSkyLight(fFadeInTime, fFadeOutTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandGuiCmd(const RwChar *pGuiCmd, RwBool bShow, RwReal fDelayTime)
{
	if(!IsCinematicPlayPossible())
		return;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainGuiCommand(pGuiCmd, bShow, fDelayTime);

	InsertChain(pChain);
}

void CNtlDTCinematicManager::CommandServerAck(void)
{
	if(!IsCinematicPlayPossible())
		return;

	m_bServerAck = TRUE;

	CNtlDTCinematicNodeChain *pChain;
	pChain = MakeChainServerAck();

	InsertChain(pChain);
}