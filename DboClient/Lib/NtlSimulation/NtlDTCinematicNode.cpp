#include "precomp_ntlsimulation.h"
#include "NtlDTCinematicNode.h"

// shared
#include "NtlSharedType.h"
#include "NtlMovement.h"
#include "NtlDirection.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGlobal.h"
#include "NtlPLSceneManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

// simulation
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlDTCinematicManager.h"
#include "NtlSobManager.h"
#include "NtlSobActor.h"
#include "NtlFSMDef.h"
#include "NtlFSMStateBase.h"
#include "NtlSobProxy.h"
#include "NtlCameraManager.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"
#include "NtlCameraController.h"
#include "NtlSobCharProxyDecoration.h"


CNtlDTCinematicNode::CNtlDTCinematicNode()
{
	m_bFinish		= FALSE;
}

CNtlDTCinematicNode::~CNtlDTCinematicNode()
{
}


void CNtlDTCinematicNode::Finish(void)
{
	m_bFinish = TRUE;
}

void CNtlDTCinematicNode::Enter(void)
{
	
}

void CNtlDTCinematicNode::Exit(void)
{
}

void CNtlDTCinematicNode::Update(RwReal fElapsed)
{
}

void CNtlDTCinematicNode::InteractiveDirectFinish(void)
{
	Finish();
}

void CNtlDTCinematicNode::Break(void)
{

}

RwBool CNtlDTCinematicNode::IsFinish(void)
{
	return m_bFinish;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicViewNode)

CNtlDTCinematicViewNode::CNtlDTCinematicViewNode()
{
	m_bViewClose	= FALSE;
	m_bShow			= FALSE;
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;
}

CNtlDTCinematicViewNode::~CNtlDTCinematicViewNode()
{
	// <형석>
	// cinematic view를 close를 하지 않았을 경우...
	// 방어 코드를 작성했다.

	if(!m_bShow && !m_bViewClose)
	{
		CNtlPLEventGenerator::CinematicViewShow(FALSE);
	}
}

void CNtlDTCinematicViewNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		// event 발생.
		CNtlPLEventGenerator::CinematicViewShow(m_bShow);
	}
}

void CNtlDTCinematicViewNode::Exit(void)
{
}

void CNtlDTCinematicViewNode::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	m_fCurrTime += fElapsed;
	if(m_fCurrTime >= m_fDelayTime)
	{
		// event 발생.
		CNtlPLEventGenerator::CinematicViewShow(m_bShow);
		Finish();
	}
}

void CNtlDTCinematicViewNode::SetData(RwBool bShow, RwReal fDelayTime)
{
	m_bShow			= bShow;
	m_fDelayTime	= fDelayTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicBalloon)

CNtlDTCinematicBalloon::CNtlDTCinematicBalloon()
{
	m_byState		= DTCI_BALLON_DEALY;
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;
	m_byPlayType	= CTCI_BALLON_PLAY_SINGLE;
}

CNtlDTCinematicBalloon::~CNtlDTCinematicBalloon()
{
}

void CNtlDTCinematicBalloon::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		// ballon 생성 event 발생.

		RwUInt8 byBalloonPlayType = GetBallonPlayType();

		CNtlPLEventGenerator::CinematicBalloonShow(m_hSerialId, m_uiQuestTextTblId, m_byBalloonType, m_byEmotionType, byBalloonPlayType);

		if(m_byPlayType == CTCI_BALLON_PLAY_BEGIN || m_byPlayType == CTCI_BALLON_PLAY_PROGRESS)
		{
			Finish();
		}
		else
		{
			m_byState = DTCI_BALLON_IDLE;
		}
	}
}

void CNtlDTCinematicBalloon::Exit(void)
{
}

void CNtlDTCinematicBalloon::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	m_fCurrTime += fElapsed;

	if(m_byState == DTCI_BALLON_DEALY)
	{
		if(m_fCurrTime >= m_fDelayTime)
		{
			RwUInt8 byBalloonPlayType = GetBallonPlayType();
			CNtlPLEventGenerator::CinematicBalloonShow(m_hSerialId, m_uiQuestTextTblId, m_byBalloonType, m_byEmotionType, byBalloonPlayType);
			if(m_byPlayType == CTCI_BALLON_PLAY_BEGIN || m_byPlayType == CTCI_BALLON_PLAY_PROGRESS)
			{
				Finish();
				return;
			}

			m_fCurrTime = 0.0f;
			m_byState = DTCI_BALLON_IDLE;
		}
	}
	else if(m_byState == DTCI_BALLON_IDLE)
	{
		if(m_fCurrTime >= m_fLifeTime)
		{
			CNtlPLEventGenerator::CinematicBalloonHide();
			m_fCurrTime = 0.0f;
			m_byState = DTCI_BALLON_DESTROY;
		}
	}
	else
	{
		if(m_fCurrTime >= 1.0f)
		{
			// 소멸 delay
			Finish();
		}
	}
}

RwUInt8 CNtlDTCinematicBalloon::GetBallonPlayType(void)
{
	RwUInt8 byBallonPlayType = SNtlEventCinematicBalloon::PLAY_BEGIN;

	switch(m_byPlayType)
	{
	case CTCI_BALLON_PLAY_PROGRESS:
	case CTCI_BALLON_PLAY_END:
		byBallonPlayType = SNtlEventCinematicBalloon::PLAY_PROGRESS;
		break;
	default:
		byBallonPlayType = SNtlEventCinematicBalloon::PLAY_BEGIN;
		break;
	}

	return byBallonPlayType;
}

void CNtlDTCinematicBalloon::SetData(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	m_fLifeTime		= fLifeTime;
	m_fDelayTime	= fDelayTime;
	m_hSerialId		= hSerialId;
	m_uiQuestTextTblId	= uiQuestTextTblId;
	m_byBalloonType	= byBalloonType;
	m_byEmotionType	= byEmotionType;
	m_byPlayType	= CTCI_BALLON_PLAY_SINGLE;
}

void CNtlDTCinematicBalloon::SetDataBegin(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	m_fLifeTime		= 0.0f;
	m_fDelayTime	= fDelayTime;
	m_hSerialId		= hSerialId;
	m_uiQuestTextTblId	= uiQuestTextTblId;
	m_byBalloonType	= byBalloonType;
	m_byEmotionType	= byEmotionType;
	m_byPlayType	= CTCI_BALLON_PLAY_BEGIN;
}

void CNtlDTCinematicBalloon::SetDataProgress(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fDelayTime)
{
	m_fLifeTime		= 0.0f;
	m_fDelayTime	= fDelayTime;
	m_hSerialId		= hSerialId;
	m_uiQuestTextTblId	= uiQuestTextTblId;
	m_byBalloonType	= byBalloonType;
	m_byEmotionType	= byEmotionType;
	m_byPlayType	= CTCI_BALLON_PLAY_PROGRESS;
}

void CNtlDTCinematicBalloon::SetDataEnd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	m_fLifeTime		= fLifeTime;
	m_fDelayTime	= fDelayTime;
	m_hSerialId		= hSerialId;
	m_uiQuestTextTblId	= uiQuestTextTblId;
	m_byBalloonType	= byBalloonType;
	m_byEmotionType	= byEmotionType;
	m_byPlayType	= CTCI_BALLON_PLAY_END;
}

void CNtlDTCinematicBalloon::SetDataSingle(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwReal fLifeTime, RwReal fDelayTime)
{
	m_fLifeTime		= fLifeTime;
	m_fDelayTime	= fDelayTime;
	m_hSerialId		= hSerialId;
	m_uiQuestTextTblId	= uiQuestTextTblId;
	m_byBalloonType	= byBalloonType;
	m_byEmotionType	= byEmotionType;
	m_byPlayType	= CTCI_BALLON_PLAY_SINGLE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicFlash)


CNtlDTCinematicFlash::CNtlDTCinematicFlash()
{
	m_byState	= DTCI_FLASH_DEALY;
	m_fCurrTime = 0.0f;
}

CNtlDTCinematicFlash::~CNtlDTCinematicFlash()
{
}

void CNtlDTCinematicFlash::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		// flash 생성 event 발생.
		PlayFlash();
		m_byState = DTCI_FLASH_IDLE;
	}
}
	
void CNtlDTCinematicFlash::Exit(void)
{
}

void CNtlDTCinematicFlash::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	m_fCurrTime += fElapsed;

	if(m_byState == DTCI_FLASH_DEALY)
	{
		if(m_fCurrTime >= m_fDelayTime)
		{
			// flash 생성 event 발생.
			PlayFlash();

			m_fCurrTime = 0.0f;
			m_byState = DTCI_FLASH_IDLE;
		}
	}
	else if(m_byState == DTCI_FLASH_IDLE)
	{
		if(m_fCurrTime >= 30.0f)
		{
			Finish();
		}
	}
}

void CNtlDTCinematicFlash::PlayFlash(void)
{
	CNtlPLEventGenerator::CinematicFlashShow( (RwChar*)m_strFileName.c_str(), m_bLoop, m_fLifeTime, this, 0.3f, 1.0f, SNtlEventCinematicFlash::PLAY_END);
}

void CNtlDTCinematicFlash::SetData(const RwChar *pFileName, RwBool bLoop, RwReal fLifeTime, RwReal fDelayTime)
{
	m_byState		= DTCI_FLASH_DEALY;
	m_strFileName	= pFileName;
	m_fDelayTime	= fDelayTime;
	m_fLifeTime		= fLifeTime;
	m_bLoop			= bLoop;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicPauseNode)

CNtlDTCinematicPauseNode::CNtlDTCinematicPauseNode()
{
	m_fLifeTime		= 0.0f;
	m_fCurrLifeTime	= 0.0f;
}

CNtlDTCinematicPauseNode::~CNtlDTCinematicPauseNode()
{
}

void CNtlDTCinematicPauseNode::Update(RwReal fElapsed)
{
	m_fCurrLifeTime += fElapsed;
	if(m_fCurrLifeTime >= m_fLifeTime)
		Finish();
}

void CNtlDTCinematicPauseNode::SetData(RwReal fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicSobNode)

CNtlDTCinematicSobNode::CNtlDTCinematicSobNode()
{
	m_bDelayCheckStep	= TRUE;
	m_fDelayTime		= 0.0f;
	m_fCurrTime			= 0.0f;

	m_sStuff.bySobCmd	= DTCI_SOB_CRTATE;
	m_sStuff.hSerialId	= INVALID_SERIAL_ID;
	m_sStuff.uiTblId	= INVALID_TBLIDX;
	m_sStuff.fAngleY	= 0.0f;

	CNtlMath::MathRwV3dAssign(&m_sStuff.vMoveDest, 0.0f, 0.0f, 0.0f);
}

CNtlDTCinematicSobNode::~CNtlDTCinematicSobNode()
{
}

void CNtlDTCinematicSobNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		m_bDelayCheckStep = FALSE;

		if(m_sStuff.bySobCmd == DTCI_SOB_CRTATE)
		{
			if(m_sStuff.bySubCmd == DTCI_SOB_CREATE_MOB)
			{
				CreateMob();
			}
			else if(m_sStuff.bySubCmd == DTCI_SOB_CREATE_NPC)
			{
				CreateNpc();
			}
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DELETE)
		{
			GetNtlDTCinematicManager()->UnRegisterSob(m_sStuff.hSerialId);
			CNtlSLEventGenerator::SobDelete(m_sStuff.hSerialId);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_MOVE)
		{
			CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );
			RwV3d vCurrLoc = pActor->GetPosition();

			CNtlSLEventGenerator::SobDestMove(m_sStuff.hSerialId, NTL_MOVE_FLAG_RUN, vCurrLoc, m_sStuff.vMoveDest, 0);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_SPLINE_MOVE)
		{
			CNtlSLEventGenerator::SobSplineMove(m_sStuff.hSerialId, m_sStuff.bSplineGroundMove, m_sStuff.bRunMove, (RwChar*)m_sStuff.strSpline.c_str(), m_sStuff.vLoc);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_TRIGGER_OBJECT_STATE)
		{
			SERIAL_HANDLE hSerialId = GetNtlSobManager()->GetObjectTriggerSerialFromId(m_sStuff.uiTblId);
			CNtlSLEventGenerator::TObjectUpdateState(hSerialId, m_sStuff.uiTblId, m_sStuff.byTriggerObjMainState, m_sStuff.byTriggerObjSubState, 0);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_PLAY)
		{
			CNtlSLEventGenerator::SobDirectPlay(m_sStuff.hSerialId, DIRECT_PLAY_NORMAL, m_sStuff.uiTblId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY)
		{
			CNtlSLEventGenerator::SobAnimPlay(m_sStuff.hSerialId, m_sStuff.uiAnimId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY_POSITION_SETTING)
		{
			PositionSetting();
			CNtlSLEventGenerator::SobAnimPlay(m_sStuff.hSerialId, m_sStuff.uiAnimId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_POSITION_SETTING)
		{
			PositionSetting();
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_SETTING)
		{
			DirectSetting();
		}
		else if (m_sStuff.bySobCmd == DTCI_SOB_SCALE)
		{
			CNtlSLEventGenerator::SobCinematicScale(m_sStuff.hSerialId, m_sStuff.fScaling, m_sStuff.fScaleTime);
		}
	}
}

void CNtlDTCinematicSobNode::Exit(void)
{
}

void CNtlDTCinematicSobNode::Update(RwReal fElapsed)
{
//	DBO_WARNING_MESSAGE("CNtlDTCinematicSobNode::Update cmd: " << (int)m_sStuff.bySobCmd << " m_fCurrTime: " << m_fCurrTime << " m_bDelayCheckStep: " << (int)m_bDelayCheckStep);
	if(IsFinish())
		return;

	if(!m_bDelayCheckStep)
	{
		if(m_sStuff.bySobCmd == DTCI_SOB_CRTATE)
		{
			UpdateSobCreate(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DELETE)
		{
			UpdateSobDelete(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_MOVE)
		{
			UpdateSobMove(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_SPLINE_MOVE)
		{
			UpdateSobSplineMove(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_TRIGGER_OBJECT_STATE)
		{
			UpdateSobTriggetObjectState(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_PLAY)
		{
			UpdateSobDirectPlay(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY)
		{
			UpdateSobAnimPlay(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY_POSITION_SETTING)
		{
			UpdateSobAnimPlay(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_POSITION_SETTING)
		{
			UpdateSobPositionSetting(fElapsed);
		}
		else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_SETTING)
		{
			UpdateSobDirectSetting(fElapsed);
		}
		else if (m_sStuff.bySobCmd == DTCI_SOB_SCALE)
		{
			Finish();
		}
	}
	else
	{
		m_fCurrTime += fElapsed;
		if(m_fCurrTime >= m_fDelayTime)
		{
			m_bDelayCheckStep = FALSE;

			if(m_sStuff.bySobCmd == DTCI_SOB_CRTATE)
				CreateMob();
			else if(m_sStuff.bySobCmd == DTCI_SOB_DELETE)
			{
				GetNtlDTCinematicManager()->UnRegisterSob(m_sStuff.hSerialId);
				CNtlSLEventGenerator::SobDelete(m_sStuff.hSerialId);
			} 
			else if(m_sStuff.bySobCmd == DTCI_SOB_MOVE)
			{
				CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );
				RwV3d vCurrLoc = pActor->GetPosition();

				CNtlSLEventGenerator::SobDestMove(m_sStuff.hSerialId, NTL_MOVE_FLAG_RUN, vCurrLoc, m_sStuff.vMoveDest, 0);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_SPLINE_MOVE)
			{
				CNtlSLEventGenerator::SobSplineMove(m_sStuff.hSerialId, m_sStuff.bSplineGroundMove, m_sStuff.bRunMove, (RwChar*)m_sStuff.strSpline.c_str(), m_sStuff.vLoc);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_TRIGGER_OBJECT_STATE)
			{
				SERIAL_HANDLE hSerialId = GetNtlSobManager()->GetObjectTriggerSerialFromId(m_sStuff.uiTblId);
				CNtlSLEventGenerator::TObjectUpdateState(hSerialId, m_sStuff.uiTblId, m_sStuff.byTriggerObjMainState, m_sStuff.byTriggerObjSubState, 0);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_PLAY)
			{
				CNtlSLEventGenerator::SobDirectPlay(m_sStuff.hSerialId, DIRECT_PLAY_NORMAL, m_sStuff.uiTblId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY)
			{
				CNtlSLEventGenerator::SobAnimPlay(m_sStuff.hSerialId, m_sStuff.uiAnimId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_ANIM_PLAY_POSITION_SETTING)
			{
				PositionSetting();
				CNtlSLEventGenerator::SobAnimPlay(m_sStuff.hSerialId, m_sStuff.uiAnimId, m_sStuff.bTimeLoop, m_sStuff.fLoopTime);
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_POSITION_SETTING)
			{
				PositionSetting();
			}
			else if(m_sStuff.bySobCmd == DTCI_SOB_DIRECT_SETTING)
			{
				DirectSetting();
			}
			else if (m_sStuff.bySobCmd == DTCI_SOB_SCALE)
			{
				CNtlSLEventGenerator::SobCinematicScale(m_sStuff.hSerialId, m_sStuff.fScaling, m_sStuff.fScaleTime);
			}
		}
	}
}

void CNtlDTCinematicSobNode::CreateMob(void)
{
	sMOB_BRIEF sMobBrief;
	sCHARSTATE sCharState;

	Logic_MakeMobBrief(&sMobBrief, m_sStuff.uiTblId);
	Logic_MakeMobSpawnState(&sCharState, m_sStuff.vLoc, m_sStuff.vDir);

	CNtlSLEventGenerator::SobMonsterCreate(SLCLASS_MONSTER, m_sStuff.hSerialId, m_sStuff.vLoc, m_sStuff.vDir, &sMobBrief, &sCharState, TRUE);

	if(!m_sStuff.bShow)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId);
		if(pSobObj)
		{
			pSobObj->GetSobProxy()->EnableProxySystemVisible(FALSE);
		}
	}

	GetNtlDTCinematicManager()->RegisterSob(m_sStuff.hSerialId);
}

void CNtlDTCinematicSobNode::CreateNpc(void)
{
	sNPC_BRIEF sNPCBrief;
	sCHARSTATE sCharState;

	Logic_MakeNPCBrief(&sNPCBrief, m_sStuff.uiTblId);
	Logic_MakeNPCSpawnState(&sCharState, m_sStuff.vLoc, m_sStuff.vDir);

	CNtlSLEventGenerator::SobNpcCreate(SLCLASS_NPC, m_sStuff.hSerialId, m_sStuff.vLoc, m_sStuff.vDir, &sNPCBrief, &sCharState, FALSE, TRUE);

	if(!m_sStuff.bShow)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId);
		if(pSobObj)
		{
			pSobObj->GetSobProxy()->EnableProxySystemVisible(FALSE);
		}
	}

	GetNtlDTCinematicManager()->RegisterSob(m_sStuff.hSerialId);
}

void CNtlDTCinematicSobNode::PositionSetting(void)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId);
	if(pSobObj)
	{
		SWorldHeightStuff sStuff;
		RwBool bSuccess = Logic_GetWorldHeight(&m_sStuff.vLoc, sStuff);
		if(bSuccess)
		{
			m_sStuff.vLoc.y = sStuff.fWorldHeight;
			pSobObj->SetPosition(&m_sStuff.vLoc);
		}
	}
}


void CNtlDTCinematicSobNode::DirectSetting(void)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId);
	if(pSobObj)
	{
		pSobObj->SetDirection(&m_sStuff.vDir);
	}
}

void CNtlDTCinematicSobNode::UpdateSobCreate(RwReal fElapsed)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );
	
	if(pActor->CanMove())
		Finish();
}

void CNtlDTCinematicSobNode::UpdateSobDelete(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicSobNode::UpdateSobMove(RwReal fElapsed)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );

	CNtlFSMStateBase *pStateBase = pActor->GetCurrentState();
	RwUInt32 uiStateId = pStateBase->GetStateId();
	if(uiStateId == NTL_FSMSID_IDLE)
		Finish();
}

void CNtlDTCinematicSobNode::UpdateSobSplineMove(RwReal fElapsed)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );

	CNtlFSMStateBase *pStateBase = pActor->GetCurrentState();
	RwUInt32 uiStateId = pStateBase->GetStateId();
	if(uiStateId == NTL_FSMSID_IDLE)
		Finish();
}

void CNtlDTCinematicSobNode::UpdateSobTriggetObjectState(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicSobNode::UpdateSobDirectPlay(RwReal fElapsed)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );

	CNtlFSMStateBase *pStateBase = pActor->GetCurrentState();
	RwUInt32 uiStateId = pStateBase->GetStateId();
	if(uiStateId == NTL_FSMSID_IDLE)
		Finish();
}

void CNtlDTCinematicSobNode::UpdateSobAnimPlay(RwReal fElapsed)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(m_sStuff.hSerialId) );

	CNtlFSMStateBase *pStateBase = pActor->GetCurrentState();
	RwUInt32 uiStateId = pStateBase->GetStateId();
	if(uiStateId == NTL_FSMSID_IDLE)
		Finish();
}

void CNtlDTCinematicSobNode::UpdateSobPositionSetting(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicSobNode::UpdateSobDirectSetting(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicSobNode::SetDataMobCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	m_fDelayTime		= fDelayTime;
	m_sStuff.bySobCmd	= DTCI_SOB_CRTATE;
	m_sStuff.bySubCmd	= DTCI_SOB_CREATE_MOB;
	m_sStuff.hSerialId	= hSerialId;
	m_sStuff.uiTblId	= uiTblId;
	m_sStuff.fAngleY	= fAngleY;
	m_sStuff.bShow		= bShow;
	
	RwV3dAssignMacro(&m_sStuff.vLoc, &vLoc);

	RwMatrix *pMat = RwMatrixCreate();

	RwMatrixSetIdentity(pMat);
	RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_sStuff.fAngleY, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(pMat);
	
	RwV3dAssignMacro(&m_sStuff.vDir, &pMat->at);

	RwMatrixDestroy(pMat);
}

void CNtlDTCinematicSobNode::SetDataNpcCreate(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, RwReal fAngleY, RwBool bShow, RwReal fDelayTime)
{
	m_fDelayTime		= fDelayTime;
	m_sStuff.bySobCmd	= DTCI_SOB_CRTATE;
	m_sStuff.bySubCmd	= DTCI_SOB_CREATE_NPC;
	m_sStuff.hSerialId	= hSerialId;
	m_sStuff.uiTblId	= uiTblId;
	m_sStuff.fAngleY	= fAngleY;
	m_sStuff.bShow		= bShow;
	
	RwV3dAssignMacro(&m_sStuff.vLoc, &vLoc);

	RwMatrix *pMat = RwMatrixCreate();

	RwMatrixSetIdentity(pMat);
	RwMatrixRotate (pMat, &CNtlPLGlobal::m_vYAxisV3, m_sStuff.fAngleY, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(pMat);
	
	RwV3dAssignMacro(&m_sStuff.vDir, &pMat->at);

	RwMatrixDestroy(pMat);
}

void CNtlDTCinematicSobNode::SetDataSobDelete(SERIAL_HANDLE hSerialId, RwReal fDelayTime)
{
	m_fDelayTime		= fDelayTime;
	m_sStuff.bySobCmd	= DTCI_SOB_DELETE;
	m_sStuff.hSerialId	= hSerialId;
}

void CNtlDTCinematicSobNode::SetDataSobMove(SERIAL_HANDLE hSerialId, RwV3d vDest, RwReal fDelayTime)
{
	m_fDelayTime		= fDelayTime;
	m_sStuff.bySobCmd	= DTCI_SOB_MOVE;
	m_sStuff.hSerialId	= hSerialId;
	m_sStuff.vMoveDest	= vDest;
}

void CNtlDTCinematicSobNode::SetDataSobSplineMove(SERIAL_HANDLE hSerialId, RwV3d vPos, RwBool bGroundMove, RwBool bRunMove, const RwChar *pKey, RwReal fDelayTime)
{
	m_fDelayTime				= fDelayTime;
	m_sStuff.bySobCmd			= DTCI_SOB_SPLINE_MOVE;
	m_sStuff.hSerialId			= hSerialId;
	m_sStuff.vLoc				= vPos;
	m_sStuff.bSplineGroundMove	= bGroundMove;
	m_sStuff.bRunMove			= bRunMove;
	m_sStuff.strSpline			= pKey;
}

void CNtlDTCinematicSobNode::SetDataSobTriggerObjectState(RwUInt32 uiTblId, RwUInt8 byMainState, RwUInt8 bySubState, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_TRIGGER_OBJECT_STATE;
	m_sStuff.uiTblId				= uiTblId;
	m_sStuff.byTriggerObjMainState	= byMainState;
	m_sStuff.byTriggerObjSubState	= bySubState;
}

void CNtlDTCinematicSobNode::SetDataSobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_DIRECT_PLAY;
	m_sStuff.hSerialId				= hSerialId;
	m_sStuff.uiTblId				= uiTblId;
	m_sStuff.bTimeLoop				= bTimeLoop;
	m_sStuff.fLoopTime				= fLoopTime;
}


void CNtlDTCinematicSobNode::SetDataSobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_ANIM_PLAY;
	m_sStuff.hSerialId				= hSerialId;
	m_sStuff.uiAnimId				= uiAnimId;
	m_sStuff.bTimeLoop				= bTimeLoop;
	m_sStuff.fLoopTime				= fLoopTime;
}

void CNtlDTCinematicSobNode::SetDataSobAnimPlayPositionSetting(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime, RwV3d vPos, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_ANIM_PLAY_POSITION_SETTING;
	m_sStuff.hSerialId				= hSerialId;
	m_sStuff.uiAnimId				= uiAnimId;
	m_sStuff.bTimeLoop				= bTimeLoop;
	m_sStuff.vLoc					= vPos;
	m_sStuff.fLoopTime				= fLoopTime;
}

void CNtlDTCinematicSobNode::SetDataSobPositionSetting(SERIAL_HANDLE hSerialId, RwV3d vPos, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_POSITION_SETTING;
	m_sStuff.hSerialId				= hSerialId;
	RwV3dAssignMacro(&m_sStuff.vLoc, &vPos);
}

void CNtlDTCinematicSobNode::SetDataSobDirectSetting(SERIAL_HANDLE hSerialId, RwV3d vDir, RwReal fDelayTime)
{
	m_fDelayTime					= fDelayTime;
	m_sStuff.bySobCmd				= DTCI_SOB_DIRECT_SETTING;
	m_sStuff.hSerialId				= hSerialId;
	RwV3dAssignMacro(&m_sStuff.vDir, &vDir);
}

void CNtlDTCinematicSobNode::SetDataSobScale(SERIAL_HANDLE hSerialId, float fScale, float fScaleTime, float fDelayTime)
{
	m_fDelayTime = fDelayTime;
	m_sStuff.bySobCmd = DTCI_SOB_SCALE;
	m_sStuff.hSerialId = hSerialId;
	m_sStuff.fScaling = fScale;
	m_sStuff.fScaleTime = fScaleTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicPLEffectNode)

CNtlDTCinematicPLEffectNode::CNtlDTCinematicPLEffectNode()
{
	m_fDelayTime = 0.0f;
	m_fCurrTime  = 0.0f;
}

CNtlDTCinematicPLEffectNode::~CNtlDTCinematicPLEffectNode()
{
}

void CNtlDTCinematicPLEffectNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		CreatePLEffect();
	}
}

void CNtlDTCinematicPLEffectNode::Exit(void)
{
}

void CNtlDTCinematicPLEffectNode::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	if(m_fDelayTime <= 0.0f)
	{
		Finish();
		return;
	}

	m_fCurrTime += fElapsed;

	if(m_fCurrTime >= m_fDelayTime)
	{
		CreatePLEffect();
		Finish();
	}
}

void CNtlDTCinematicPLEffectNode::CreatePLEffect(void)
{
	CNtlPLEntity *pPLEntity = NULL;

	if(m_byEffectType == CN_PLEFFECT_TYPE_ACTOR_BONE)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
		if(pSobObj == NULL)
			return;

		pPLEntity = pSobObj->GetSobProxy()->CreatePLChildEffect(m_strKey.c_str(), m_strBond.c_str()); 

		if(pPLEntity)
			GetNtlDTCinematicManager()->RegisterPLEntity(m_hInstance, TRUE, m_hSerialId, pPLEntity);
	}
	else if(m_byEffectType == CN_PLEFFECT_TYPE_ACTOR_OFFSET)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
		if(pSobObj == NULL)
			return;

		pPLEntity = pSobObj->GetSobProxy()->CreatePLChildEffect(m_strKey.c_str(), m_vOffset); 

		if(pPLEntity)
			GetNtlDTCinematicManager()->RegisterPLEntity(m_hInstance, TRUE, m_hSerialId, pPLEntity);
	}
	else
	{
		pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_strKey.c_str());
		if(pPLEntity)
			pPLEntity->SetPosition(&m_vWorld);

		if(pPLEntity)
			GetNtlDTCinematicManager()->RegisterPLEntity(m_hInstance, FALSE, INVALID_SERIAL_ID, pPLEntity);
	}

}

void CNtlDTCinematicPLEffectNode::SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId, const RwChar *pBoneName, RwReal fDelayTime)
{
	m_hInstance		= hInstance;
	m_byEffectType	= CN_PLEFFECT_TYPE_ACTOR_BONE;
	m_strKey		= pKey;
	m_hSerialId		= hSerialId;
	m_strBond		= pBoneName;
	m_fDelayTime	= fDelayTime;	
}

void CNtlDTCinematicPLEffectNode::SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, SERIAL_HANDLE hSerialId, RwV3d vOffset, RwReal fDelayTime)
{
	m_hInstance		= hInstance;
	m_byEffectType	= CN_PLEFFECT_TYPE_ACTOR_OFFSET;
	m_strKey		= pKey;
	m_hSerialId		= hSerialId;
	m_vOffset		= vOffset;
	m_fDelayTime	= fDelayTime;
}

void CNtlDTCinematicPLEffectNode::SetData(SERIAL_HANDLE hInstance, const RwChar *pKey, RwV3d vWorld, RwReal fDelayTime)
{
	m_hInstance		= hInstance;
	m_byEffectType	= CN_PLEFFECT_TYPE_WORLD;
	m_strKey		= pKey;
	m_vWorld		= vWorld;
	m_fDelayTime	= fDelayTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicCameraNode)

CNtlDTCinematicCameraNode::CNtlDTCinematicCameraNode()
{
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;
	m_bCreateCamera	= FALSE;
	m_byDTType		= 0;
}
	
CNtlDTCinematicCameraNode::~CNtlDTCinematicCameraNode()
{
}

void CNtlDTCinematicCameraNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		CreateCameraDTNode();
		m_bCreateCamera = TRUE;
	}
}

void CNtlDTCinematicCameraNode::Exit(void)
{
}

void CNtlDTCinematicCameraNode::Update(RwReal fElapsed)
{
	if(m_bCreateCamera)
	{
		// Camera does not manage the life time of the cinematic node.
		if(m_bAsync)
		{
			Finish();
		}

		return;
	}

	m_fCurrTime += fElapsed;

	if(m_fCurrTime >= m_fDelayTime)
	{
		CreateCameraDTNode();
		m_bCreateCamera = TRUE;

		// Camera does not manage the life time of the cinematic node.
		if(m_bAsync)
		{
			Finish();
		}
	}
}

void CNtlDTCinematicCameraNode::CreateCameraDTNode(void)
{
	CINEMATIC_HANDLE hCinematic = NULL;
	if(!m_bAsync)
		hCinematic = (CINEMATIC_HANDLE)this;

	if(m_byDTType == CN_CAMERA_SOFT_ACTOR_DIST)
	{
		GetNtlGameCameraManager()->AddDTSoftActorDistNode(m_uCameraStuff.sSoftActorDist.hSerialId,
															m_uCameraStuff.sSoftActorDist.fHeightOffset,
															m_uCameraStuff.sSoftActorDist.fAngleX,
															m_uCameraStuff.sSoftActorDist.fAngleY,
															m_uCameraStuff.sSoftActorDist.fDistance,
															m_uCameraStuff.sSoftActorDist.fLifeTime,
															m_uCameraStuff.sSoftActorDist.vLookAt,
															hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_ACTOR_DYNAMIC_LOOKAT_DIST)
	{
		GetNtlGameCameraManager()->AddDTSoftActorDynamicLookAtDistNode(m_uCameraStuff.sSoftActorDynaLookAtDist.hSerialId,
																		m_uCameraStuff.sSoftActorDynaLookAtDist.fHeightOffset,
																		m_uCameraStuff.sSoftActorDynaLookAtDist.fAngleX,
																		m_uCameraStuff.sSoftActorDynaLookAtDist.fAngleY,
																		m_uCameraStuff.sSoftActorDynaLookAtDist.fDistance,
																		m_uCameraStuff.sSoftActorDynaLookAtDist.fLifeTime,
																		hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_CURRENT_ROTATE_X)
	{
		GetNtlGameCameraManager()->AddDTSoftCurrentRotateX(m_uCameraStuff.sSoftCurrRot.fLastAngle,
															m_uCameraStuff.sSoftCurrRot.fInterTime,
															m_uCameraStuff.sSoftCurrRot.fLifeTime,
															hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_CURRENT_ROTATE_Y)
	{
		GetNtlGameCameraManager()->AddDTSoftCurrentRotateY(m_uCameraStuff.sSoftCurrRot.fLastAngle,
															m_uCameraStuff.sSoftCurrRot.fInterTime,
															m_uCameraStuff.sSoftCurrRot.fLifeTime,
															hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_CURRENT_ROTATE_Z)
	{
		GetNtlGameCameraManager()->AddDTSoftCurrentRotateZ(m_uCameraStuff.sSoftCurrRot.fLastAngle,
															m_uCameraStuff.sSoftCurrRot.fInterTime,
															m_uCameraStuff.sSoftCurrRot.fLifeTime,
															hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_CURRENT_FOV)
	{
		GetNtlGameCameraManager()->AddDTSoftCurrentFov(m_uCameraStuff.sSoftCurrFov.fLastFov,
														m_uCameraStuff.sSoftCurrFov.fInterTime,
														hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SOFT_PAUSE)
	{
		GetNtlGameCameraManager()->AddDTSoftPauseNode(m_uCameraStuff.sSoftPause.fLifeTime,
													  m_uCameraStuff.sSoftPause.bLoop,
													  hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SPLINE_CURRENT_DYNAMIC_LOOKAT)
	{
		GetNtlGameCameraManager()->AddDTSplineCurrentDynamicLookAtNode(m_uCameraStuff.sSplineCurrDynaLookAt.hSerialId,
																		m_uCameraStuff.sSplineCurrDynaLookAt.fHeightOffset,
																		m_uCameraStuff.sSplineCurrDynaLookAt.chPropKey,
																		m_uCameraStuff.sSplineCurrDynaLookAt.fLifeTime,
																		hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SPLINE_SELF)
	{
		GetNtlGameCameraManager()->AddDTSplineSelfNode(m_uCameraStuff.sSplineSelf.vPos,
														m_uCameraStuff.sSplineSelf.fAngleX,
														m_uCameraStuff.sSplineSelf.fAngleY,
														m_uCameraStuff.sSplineSelf.fAngleZ,
														m_uCameraStuff.sSplineSelf.chPropKey,
														hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_SPLINE_SELF_ACTOR)
	{
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_uCameraStuff.sSplineSelfActor.hSerialId);
		if(!pSobObj)
		{
			Finish();
			return;
		}

		RwV3d vPos = pSobObj->GetPosition();
		RwReal fHeight = pSobObj->GetSobProxy()->GetPLEntityHeight();

		vPos.y += fHeight*m_uCameraStuff.sSplineSelfActor.fHeightRate;

		GetNtlGameCameraManager()->AddDTSplineSelfNode(vPos,
														m_uCameraStuff.sSplineSelfActor.fAngleX,
														m_uCameraStuff.sSplineSelfActor.fAngleY,
														m_uCameraStuff.sSplineSelfActor.fAngleZ,
														m_uCameraStuff.sSplineSelfActor.chPropKey,
														hCinematic);
	}
	else if(m_byDTType == CN_CAMERA_LOCK_UNLOCK)
	{
		if(m_uCameraStuff.sLockUnLock.bLock)
		{
			GetNtlGameCameraManager()->FinishLock();
		}
		else
		{
			GetNtlGameCameraManager()->FinishUnLock();
		}
	}
	else 
	{
		Finish();
	}
	
}

void CNtlDTCinematicCameraNode::SetDataSoftActorDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
													 RwReal fDistance, RwReal fLifeTime, RwV3d vLookAt, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_ACTOR_DIST;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftActorDist.hSerialId		= hSerialId;
	m_uCameraStuff.sSoftActorDist.fHeightOffset	= fHeightOffset;
	m_uCameraStuff.sSoftActorDist.fAngleX		= fAngleX;
	m_uCameraStuff.sSoftActorDist.fAngleY		= fAngleY;
	m_uCameraStuff.sSoftActorDist.fDistance		= fDistance;
	m_uCameraStuff.sSoftActorDist.fLifeTime		= fLifeTime;
	m_uCameraStuff.sSoftActorDist.vLookAt		= vLookAt;
	
}

void CNtlDTCinematicCameraNode::SetDataSoftActorDynamicLookAtDist(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, RwReal fAngleX, RwReal fAngleY, 
												    			RwReal fDistance, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_ACTOR_DYNAMIC_LOOKAT_DIST;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftActorDynaLookAtDist.hSerialId		= hSerialId;
	m_uCameraStuff.sSoftActorDynaLookAtDist.fHeightOffset	= fHeightOffset;
	m_uCameraStuff.sSoftActorDynaLookAtDist.fAngleX			= fAngleX;
	m_uCameraStuff.sSoftActorDynaLookAtDist.fAngleY			= fAngleY;
	m_uCameraStuff.sSoftActorDynaLookAtDist.fDistance		= fDistance;
	m_uCameraStuff.sSoftActorDynaLookAtDist.fLifeTime		= fLifeTime;
}

void CNtlDTCinematicCameraNode::SetDataSoftCurrentRotateX(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_CURRENT_ROTATE_X;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftCurrRot.fLastAngle		= fLastAngle;
	m_uCameraStuff.sSoftCurrRot.fInterTime		= fInterTime;
	m_uCameraStuff.sSoftCurrRot.fLifeTime		= fLifeTime;
}

void CNtlDTCinematicCameraNode::SetDataSoftCurrentRotateY(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_CURRENT_ROTATE_Y;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftCurrRot.fLastAngle		= fLastAngle;
	m_uCameraStuff.sSoftCurrRot.fInterTime		= fInterTime;
	m_uCameraStuff.sSoftCurrRot.fLifeTime		= fLifeTime;
}

void CNtlDTCinematicCameraNode::SetDataSoftCurrentRotateZ(RwReal fLastAngle, RwReal fInterTime, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_CURRENT_ROTATE_Z;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftCurrRot.fLastAngle		= fLastAngle;
	m_uCameraStuff.sSoftCurrRot.fInterTime		= fInterTime;
	m_uCameraStuff.sSoftCurrRot.fLifeTime		= fLifeTime;
}

void CNtlDTCinematicCameraNode::SetDataSoftCurrentFov(RwReal fLastFov, RwReal fInterTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_CURRENT_FOV;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftCurrFov.fLastFov		= fLastFov;
	m_uCameraStuff.sSoftCurrFov.fInterTime		= fInterTime;
}

void CNtlDTCinematicCameraNode::SetDataSoftPause(RwReal fLifeTime, RwBool bLoop, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SOFT_PAUSE;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSoftPause.fLifeTime			= fLifeTime;
	m_uCameraStuff.sSoftPause.bLoop				= bLoop;
}

void CNtlDTCinematicCameraNode::SetDataSplineCurrentDynamicLookAt(SERIAL_HANDLE hSerialId, RwReal fHeightOffset, const RwChar *pPropKey, RwReal fLifeTime, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SPLINE_CURRENT_DYNAMIC_LOOKAT;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSplineCurrDynaLookAt.hSerialId			= hSerialId;
	m_uCameraStuff.sSplineCurrDynaLookAt.fHeightOffset		= fHeightOffset;
	strcpy_s(m_uCameraStuff.sSplineCurrDynaLookAt.chPropKey, 32, pPropKey);
	m_uCameraStuff.sSplineCurrDynaLookAt.fLifeTime			= fLifeTime;
}

void CNtlDTCinematicCameraNode::SetDataSPlineSelf(RwV3d vPos, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SPLINE_SELF;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSplineSelf.vPos			= vPos;
	m_uCameraStuff.sSplineSelf.fAngleX		= fAngleX;
	m_uCameraStuff.sSplineSelf.fAngleY		= fAngleY;
	m_uCameraStuff.sSplineSelf.fAngleZ		= fAngleZ;
	strcpy_s(m_uCameraStuff.sSplineSelf.chPropKey, 32, pPropKey);
}


void CNtlDTCinematicCameraNode::SetDataSPlineSelfActor(SERIAL_HANDLE hSerialId, RwReal fHeightRate, RwReal fAngleX, RwReal fAngleY, RwReal fAngleZ, const RwChar *pPropKey, RwReal fDelayTime, RwBool bAsync)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_SPLINE_SELF_ACTOR;
	m_bAsync		= bAsync;

	m_uCameraStuff.sSplineSelfActor.hSerialId	= hSerialId;
	m_uCameraStuff.sSplineSelfActor.fHeightRate	= fHeightRate;
	m_uCameraStuff.sSplineSelfActor.fAngleX		= fAngleX;
	m_uCameraStuff.sSplineSelfActor.fAngleY		= fAngleY;
	m_uCameraStuff.sSplineSelfActor.fAngleZ		= fAngleZ;
	
	strcpy_s(m_uCameraStuff.sSplineSelfActor.chPropKey, 32, pPropKey);
}

void CNtlDTCinematicCameraNode::SetDataLockUnLock(RwBool bLock, RwReal fDelayTime)
{
	m_fDelayTime	= fDelayTime;
	m_byDTType		= CN_CAMERA_LOCK_UNLOCK;
	m_uCameraStuff.sLockUnLock.bLock	= bLock;
	m_bAsync = true; // by daneos
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicCameraShakeNode)
					   
CNtlDTCinematicCameraShakeNode::CNtlDTCinematicCameraShakeNode()
{
	m_byShakeType	= CN_CAMERA_SHAKE;
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;
}

CNtlDTCinematicCameraShakeNode::~CNtlDTCinematicCameraShakeNode()
{
}

void CNtlDTCinematicCameraShakeNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		if(m_byShakeType == CN_CAMERA_SHAKE)
		{
			CNtlCameraShakeController::SetShakeFactor(m_fWeightValue, m_fHeightValue);
			CNtlSLEventGenerator::CameraShake(0, TRUE);
		}
		else if(m_byShakeType == CN_CAMERA_SHAKE_FACTOR)
		{
			CNtlCameraShakeController::SetShakeFactor(m_fWeightValue, m_fHeightValue);
		}
		else
		{
			CNtlCameraShakeController::ResetShakeFactor();
		}
	}
}

void CNtlDTCinematicCameraShakeNode::Exit(void)
{
}

void CNtlDTCinematicCameraShakeNode::Update(RwReal fElapsed)
{
	if(m_fDelayTime <= 0.0f)
	{
		Finish();
		return;
	}

	m_fCurrTime += fElapsed;

	if(m_fCurrTime >= m_fDelayTime)
	{
		if(m_byShakeType == CN_CAMERA_SHAKE)
		{
			CNtlCameraShakeController::SetShakeFactor(m_fWeightValue, m_fHeightValue);
			CNtlSLEventGenerator::CameraShake(0, TRUE);
		}
		else if(m_byShakeType == CN_CAMERA_SHAKE_FACTOR)
		{
			CNtlCameraShakeController::SetShakeFactor(m_fWeightValue, m_fHeightValue);
		}
		else
		{
			CNtlCameraShakeController::ResetShakeFactor();
		}

		Finish();
	}
}

void CNtlDTCinematicCameraShakeNode::SetData(RwReal fWeightValue, RwReal fHeightValue, RwReal fDelayTime)
{
	m_byShakeType	= CN_CAMERA_SHAKE;
	m_fDelayTime	= fDelayTime;
	m_fWeightValue	= fWeightValue;
	m_fHeightValue	= fHeightValue;
}

void CNtlDTCinematicCameraShakeNode::SetDataShakeFactor(RwReal fWeightValue, RwReal fHeightValue)
{
	m_byShakeType	= CN_CAMERA_SHAKE_FACTOR;
	m_fDelayTime	= 0.0f;
	m_fWeightValue	= fWeightValue;
	m_fHeightValue	= fHeightValue;
}


void CNtlDTCinematicCameraShakeNode::SetDataShakeFactorReset(void)
{
	m_byShakeType	= CN_CAMERA_SHAKE_FACTOR_RESET;
	m_fDelayTime	= 0.0f;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicFadeInOutNode)

CNtlDTCinematicFadeInOutNode::CNtlDTCinematicFadeInOutNode()
{
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;
	m_fLifeTime		= 0.0f;
	m_bFadeIn		= TRUE;
	m_byRed			= 255;
	m_byGreen		= 255;
	m_byBlue		= 255;
	m_bSend			= FALSE;
}

CNtlDTCinematicFadeInOutNode::~CNtlDTCinematicFadeInOutNode()
{
}

void CNtlDTCinematicFadeInOutNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		if(m_bFadeIn)
			CNtlPLEventGenerator::CinematicFadeIn(m_byFadeType, m_fLifeTime, m_byRed, m_byGreen, m_byBlue, 0);
		else
			CNtlPLEventGenerator::CinematicFadeOut(m_byFadeType, m_fLifeTime, m_byRed, m_byGreen, m_byBlue, 255);

		m_bSend = TRUE;
	}
}

void CNtlDTCinematicFadeInOutNode::Exit(void)
{
}

void CNtlDTCinematicFadeInOutNode::Update(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;

	if(!m_bSend)
	{
		if(m_fCurrTime >= m_fDelayTime)
		{
			if(m_bFadeIn)
				CNtlPLEventGenerator::CinematicFadeIn(m_byFadeType, m_fLifeTime, m_byRed, m_byGreen, m_byBlue, 0);
			else
				CNtlPLEventGenerator::CinematicFadeOut(m_byFadeType, m_fLifeTime, m_byRed, m_byGreen, m_byBlue, 255);

			m_fCurrTime = m_fCurrTime - m_fDelayTime;
			m_bSend = TRUE;
		}
	}
	else
	{
		if(m_fCurrTime >= m_fLifeTime)
		{
			Finish();
		}
	}
}

void CNtlDTCinematicFadeInOutNode::SetData(RwUInt8 byFadeType, RwBool bFadeIn, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwReal fDelayTime)
{
	m_byFadeType	= byFadeType;
	m_fDelayTime	= fDelayTime;
	m_fLifeTime		= fLifeTime;
	m_bFadeIn		= bFadeIn;
	m_byRed			= byRed;
	m_byGreen		= byGreen;
	m_byBlue		= byBlue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicSoundNode)

CNtlDTCinematicSoundNode::CNtlDTCinematicSoundNode()
{
	m_fCurrTime		= 0.0f;
	m_fDelayTime	= 0.0f;
	m_bLoop			= FALSE;
}

CNtlDTCinematicSoundNode::~CNtlDTCinematicSoundNode()
{
}

void CNtlDTCinematicSoundNode::Enter(void)
{
}

void CNtlDTCinematicSoundNode::Exit(void)
{
}

void CNtlDTCinematicSoundNode::Update(RwReal fElapsed)
{
	m_fCurrTime += fElapsed;

	if(m_fCurrTime < m_fDelayTime)
		return;

	PlaySound();
	Finish();
}

void CNtlDTCinematicSoundNode::PlaySound(void)
{
	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= m_byChannelGroup;
	tSoundParam.pcFileName		= (char*)m_strFileName.c_str();
	tSoundParam.fXPos			= m_vPos.x;
	tSoundParam.fYPos			= m_vPos.y;
	tSoundParam.fZPos			= m_vPos.z;

	// Loop 기능은 막아둔다. Loop 기능을 쓰려면 Stop도 할 수 있어야 한다.
	//tSoundParam.bLoop			= B2b(m_bLoop);

	if( tSoundParam.iChannelGroup == CHANNEL_GROUP_BGM )
		tSoundParam.eBGM_Type	= BGM_TYPE_CINEMATIC;

	GetSoundManager()->Play(&tSoundParam);
}

void CNtlDTCinematicSoundNode::SetData(RwUInt8 byChannelGroup, const RwChar *pFileName, RwV3d *pv3Pos, RwReal fDelayTime, RwBool bLoop /* = false */ )
{
	m_byChannelGroup	= byChannelGroup;
	m_fDelayTime		= fDelayTime;
	m_strFileName		= pFileName;
	m_bLoop				= bLoop;

	if( pv3Pos )
		RwV3dAssignMacro(&m_vPos, pv3Pos);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicBGMStop)

CNtlDTCinematicBGMStop::CNtlDTCinematicBGMStop()
{

}

CNtlDTCinematicBGMStop::~CNtlDTCinematicBGMStop()
{
}

void CNtlDTCinematicBGMStop::Enter(void)
{
	CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_END_CINEMATIC_BGM);
	Finish();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicCharacterOptNode)

CNtlDTCinematicCharacterOptNode::CNtlDTCinematicCharacterOptNode()
{
	
}

CNtlDTCinematicCharacterOptNode::~CNtlDTCinematicCharacterOptNode()
{
}

void CNtlDTCinematicCharacterOptNode::Enter(void)
{
	switch(m_byOpt)
	{
	case CINEMATIC_CHAROPT_SHADOW:
		CNtlSobCharDecorationProxy::m_bShadowCreate = m_bOnOff;
		break;
	case CINEMATIC_CHAROPT_SHOW:
		{
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerialId);
			if(pSobObj)
			{
				pSobObj->GetSobProxy()->EnableProxySystemVisible(m_bOnOff);
			}
		}
		break;
	};

	Finish();
}

void CNtlDTCinematicCharacterOptNode::Exit(void)
{
}

void CNtlDTCinematicCharacterOptNode::Update(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicCharacterOptNode::SetDataShoadow(RwBool bOnOff)
{
	m_byOpt			= CINEMATIC_CHAROPT_SHADOW;
	m_bOnOff		= bOnOff;
}

void CNtlDTCinematicCharacterOptNode::SetDataShow(SERIAL_HANDLE hSerialId, RwBool bOnOff)
{
	m_byOpt			= CINEMATIC_CHAROPT_SHOW;
	m_bOnOff		= bOnOff;
	m_hSerialId		= hSerialId;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicSceneNode)

CNtlDTCinematicSceneNode::CNtlDTCinematicSceneNode()
{
	
}

CNtlDTCinematicSceneNode::~CNtlDTCinematicSceneNode()
{
}

void CNtlDTCinematicSceneNode::Enter(void)
{
	switch(m_byOpt)
	{
	case CINEMATIC_MOONOPT_SHOW:
		GetSceneManager()->SetVisiblePlanet(m_bOnOff);
		break;
	case CINEMATIC_RAIN_BLOOM_LIGHT:
		GetSceneManager()->RunableRainBloomLight(m_fFadeInTime, m_fFadeOutTime);
		break;
	case CINEMATIC_RAIN_SKY_LIGHT:
		GetSceneManager()->RunableRainSkyLight(m_fFadeInTime, m_fFadeOutTime);
		break;
	};

	Finish();
}

void CNtlDTCinematicSceneNode::Exit(void)
{
}

void CNtlDTCinematicSceneNode::Update(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicSceneNode::SetDataMoon(RwBool bOnOff)
{
	m_byOpt			= CINEMATIC_MOONOPT_SHOW;
	m_bOnOff		= bOnOff;
}

void CNtlDTCinematicSceneNode::SetRainBloomLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	m_byOpt			= CINEMATIC_RAIN_BLOOM_LIGHT;
	m_fFadeInTime	= fFadeInTime;
	m_fFadeOutTime	= fFadeOutTime;
}

void CNtlDTCinematicSceneNode::SetRainSkyLight(RwReal fFadeInTime, RwReal fFadeOutTime)
{
	m_byOpt			= CINEMATIC_RAIN_SKY_LIGHT;
	m_fFadeInTime	= fFadeInTime;
	m_fFadeOutTime	= fFadeOutTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicGuiCmdNode)

CNtlDTCinematicGuiCmdNode::CNtlDTCinematicGuiCmdNode()
{
	m_fCurrTime = 0.0f;
}

CNtlDTCinematicGuiCmdNode::~CNtlDTCinematicGuiCmdNode()
{
}

void CNtlDTCinematicGuiCmdNode::Enter(void)
{
	if(m_fDelayTime <= 0.0f)
	{
		if(m_bShow)
		{
			GetNtlDTCinematicManager()->RegisterGuiCmd(m_strCmdKey.c_str());
		}
		else
		{
			GetNtlDTCinematicManager()->UnRegisterGuiCmd(m_strCmdKey.c_str());
		}

		CNtlSLEventGenerator::CinematicGuiCommand((RwChar*)m_strCmdKey.c_str(), m_bShow);
	}
}

void CNtlDTCinematicGuiCmdNode::Exit(void)
{
}

void CNtlDTCinematicGuiCmdNode::Update(RwReal fElapsed)
{
	if(m_fDelayTime <= 0.0f)
	{
		Finish();
		return;
	}

	m_fCurrTime += fElapsed;

	if(m_fCurrTime >= m_fDelayTime)
	{
		if(m_bShow)
		{
			GetNtlDTCinematicManager()->RegisterGuiCmd(m_strCmdKey.c_str());
		}
		else
		{
			GetNtlDTCinematicManager()->UnRegisterGuiCmd(m_strCmdKey.c_str());
		}

		CNtlSLEventGenerator::CinematicGuiCommand((RwChar*)m_strCmdKey.c_str(), m_bShow);
		Finish();
	}
}

void CNtlDTCinematicGuiCmdNode::SetData(const RwChar *pCmdKey, RwBool bShow, RwReal fDelayTime)
{
	m_strCmdKey		= pCmdKey;
	m_bShow			= bShow;
	m_fDelayTime	= fDelayTime;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicServerAckNode)

CNtlDTCinematicServerAckNode::CNtlDTCinematicServerAckNode()
{
}

CNtlDTCinematicServerAckNode::~CNtlDTCinematicServerAckNode()
{
}

void CNtlDTCinematicServerAckNode::Enter(void)
{
	API_GetSLPacketGenerator()->SendCharDirectPlayAck();
	Logic_SendTutorialCondition( ETL_CONDITION_TYPE_CINEMATIC_PLAY_FINISH );

	Finish();
}

void CNtlDTCinematicServerAckNode::Exit(void)
{
}

void CNtlDTCinematicServerAckNode::Update(RwReal fElapsed)
{
	Finish();
}

void CNtlDTCinematicServerAckNode::SetData(void)
{

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlDTCinematicNodeChain)

CNtlDTCinematicNodeChain::CNtlDTCinematicNodeChain()
{
	m_pNode		= NULL;
	m_pParent	= NULL;

	m_bFinish	= FALSE;
}

CNtlDTCinematicNodeChain::~CNtlDTCinematicNodeChain()
{
	if(m_pNode)
	{
		m_pNode->Exit();
		NTL_DELETE( m_pNode );
	}
}

void CNtlDTCinematicNodeChain::Enter(void)
{
	if(m_pNode)
	{
		m_pNode->Enter();
	}
	else
	{
		CNtlDTCinematicNodeChain *pChainNode;

		if(!m_listChild.empty())
		{
			ListChild::iterator it = m_listChild.begin();
			pChainNode = (*it);

			pChainNode->Enter();
		}

		// sibling 실행.
		if(!m_listSibling.empty())
		{
			ListChild::iterator itsibling;
			for(itsibling = m_listSibling.begin(); itsibling != m_listSibling.end(); itsibling++)
			{
				pChainNode = (*itsibling);

				pChainNode->Enter();

			}
		}
	}
}

void CNtlDTCinematicNodeChain::Exit(void)
{
	if(m_pNode)
	{
		m_pNode->Exit();

		NTL_DELETE( m_pNode );
	}
}


void CNtlDTCinematicNodeChain::Update(RwReal fElapsed)
{
	if(m_pNode && !m_pNode->IsFinish())
	{
		m_pNode->Update(fElapsed);
	}

	CNtlDTCinematicNodeChain *pChainNode;

	// child 실행.
	if(!m_listChild.empty())
	{
		ListChild::iterator itchild = m_listChild.begin();
		pChainNode = (*itchild);

		pChainNode->Update(fElapsed);

		if(pChainNode->IsFinish())
		{

			pChainNode->Exit();

			itchild = m_listChild.erase(itchild);

			if(!m_listChild.empty())
			{
				pChainNode = (*itchild);
				pChainNode->Enter();
			}
		}
	}

	// sibling 실행.
	if(!m_listSibling.empty())
	{
		ListChild::iterator itsibling;
		for(itsibling = m_listSibling.begin(); itsibling != m_listSibling.end(); )
		{
			pChainNode = (*itsibling);

			pChainNode->Update(fElapsed);

			if(pChainNode->IsFinish())
			{
				pChainNode->Exit();

				itsibling = m_listSibling.erase(itsibling);
			}
			else
			{
				itsibling++;
			}
		}
	}

}

void CNtlDTCinematicNodeChain::SetNode(const CNtlDTCinematicNode *pNode)
{
	NTL_ASSERT(m_pNode == NULL, "CNtlDTCinematicNodeChain::SetNode");

	m_pNode = const_cast<CNtlDTCinematicNode*>( pNode );
}

void CNtlDTCinematicNodeChain::SetParentChain(const CNtlDTCinematicNodeChain *pChain)
{
	m_pParent = const_cast<CNtlDTCinematicNodeChain*>( pChain );
}

void CNtlDTCinematicNodeChain::AddSiblingChain(const CNtlDTCinematicNodeChain *pChain)
{
	NTL_ASSERT(pChain != NULL, "CNtlDTCinematicNodeChain::AddSiblingChain");
	m_listSibling.push_back(const_cast<CNtlDTCinematicNodeChain*>( pChain ));
}

void CNtlDTCinematicNodeChain::AddChildChain(const CNtlDTCinematicNodeChain *pChain)
{
	NTL_ASSERT(pChain != NULL, "CNtlDTCinematicNodeChain::AddChildChain");
	m_listChild.push_back(const_cast<CNtlDTCinematicNodeChain*>( pChain ));
}

RwBool CNtlDTCinematicNodeChain::IsFinish(void)
{
	if(m_pNode)
	{
		if(!m_pNode->IsFinish())
		{
			return FALSE;
		}
	}

	if(m_listSibling.empty() && m_listChild.empty())
		return TRUE;

	return FALSE;
}

CNtlDTCinematicNode* CNtlDTCinematicNodeChain::GetNode(void) const
{
	return m_pNode;
}

CNtlDTCinematicNodeChain* CNtlDTCinematicNodeChain::GetParentChain(void) const
{
	return m_pParent;
}

