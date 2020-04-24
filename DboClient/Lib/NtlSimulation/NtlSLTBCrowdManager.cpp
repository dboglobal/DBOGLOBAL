#include "precomp_ntlsimulation.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlSLTBCrowdController.h"
#include "NtlPLEvent.h"
#include "NtlSLCENode.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlMath.h"
#include "NtlSysEvent.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobNpc.h"
#include "NtlSobFactory.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlWorldConcept.h"

#define REFREE_TBL_IDX 3188101


RwBool CNtlSLTBCrowdManager::ms_bDirectionToolMode = FALSE;

CNtlSLTBCrowdManager* CNtlSLTBCrowdManager::m_pInstance = NULL;


CNtlSLTBCrowdManager::CNtlSLTBCrowdManager(void)
:m_eReservatedStep(E_STEP_NONE)
{
    m_pRefreeActor = NULL;
    Create();
}

CNtlSLTBCrowdManager::~CNtlSLTBCrowdManager(void)
{
    Destroy();
}

void CNtlSLTBCrowdManager::Create() 
{
    LinkMsg(g_EventUpdateTick, 0, SLUPDATE_PRIORITY_CROWD);
    LinkMsg(g_EventMinorMatchStateUpdate);
    LinkMsg(g_EventMajorMatchStateUpdate);
	LinkMsg(g_EventFinalMatchStateUpdate);	
    LinkMsg(g_EventSobHTBSkillAction);
    LinkMsg(g_EventKnockDownNfy);
    LinkMsg(g_EventSobHit);
    LinkMsg(g_EventSobFainting);    
    LinkMsg(g_EventWorldChange);
	LinkMsg(g_EventAnimEnd);
}

void CNtlSLTBCrowdManager::Destroy() 
{
    if(m_pRefreeActor)
    {
        CNtlSLEventGenerator::SobDelete(m_pRefreeActor->GetSerialID());
    }
    
	m_listCurController.clear();

	for each(std::pair<std::string, CNtlSLTBCrowdController*> pairController in m_mapControllerContainer)
    {
        NTL_DELETE(pairController.second);
    }
    m_mapControllerContainer.clear();

    UnLinkMsg(g_EventUpdateTick);
    UnLinkMsg(g_EventMinorMatchStateUpdate);
    UnLinkMsg(g_EventMajorMatchStateUpdate);
	UnLinkMsg(g_EventFinalMatchStateUpdate);
    UnLinkMsg(g_EventSobHTBSkillAction);
    UnLinkMsg(g_EventKnockDownNfy);
    UnLinkMsg(g_EventSobHit);
    UnLinkMsg(g_EventSobFainting);
    UnLinkMsg(g_EventWorldChange);
	UnLinkMsg(g_EventAnimEnd);
}

void CNtlSLTBCrowdManager::Update( RwReal fElapsedTime ) 
{
	RwBool bDirectingController = FALSE;
	RwBool bUpdate;

    for each(CNtlSLTBCrowdController* pController in m_listCurController)
	{
		bUpdate = pController->Update(fElapsedTime);

		if( !bDirectingController )
			bDirectingController = bUpdate;
	}

	if( !bDirectingController && m_eReservatedStep != E_STEP_NONE )
	{
		ChangeStep(m_eReservatedStep);
		m_eReservatedStep = E_STEP_NONE;
	}
}

void CNtlSLTBCrowdManager::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventUpdateTick)
    {
        float fElapsed = *reinterpret_cast<float*>(pMsg.pData);
        Update(fElapsed);
    }
    if(pMsg.Id == g_EventMinorMatchStateUpdate)
    {
		SNtlEventMinorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( pMsg.pData );
        
		if( pData->bEnterState )
			ChangeStep(ConvertServerState( pData ) );
    }
    else if(pMsg.Id == g_EventMajorMatchStateUpdate)
    {
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( pMsg.pData );

		if( pData->bEnterState )
			ChangeStep(ConvertServerState( pData ) );
    }    
    else if(pMsg.Id == g_EventFinalMatchStateUpdate)
    {
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( pMsg.pData );

		if( pData->bEnterState )
			ChangeStep(ConvertServerState( pData ) );
    }
    else if(pMsg.Id == g_EventSobHTBSkillAction)
    {
        AddStep(E_STEP_HTB);
    }
    else if(pMsg.Id == g_EventWorldChange)
    {
        for each(CNtlSLTBCrowdController* pController in m_listCurController)
        {
            pController->Destroy();            
        } 
        m_listCurController.clear();

        if(m_pRefreeActor)
        {
            m_pRefreeActor->EnableVisible(FALSE);
        }
    }
    else if(pMsg.Id == g_EventKnockDownNfy)
    {
        // When you were knocked down
        AddStep(E_STEP_KNOCKDOWN);
    }
    else if(pMsg.Id == g_EventSobHit)
    {
        // When actors other than self are knocked down
        SNtlEventSobHit* pHit = (SNtlEventSobHit*)pMsg.pData;        
        if(pHit->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            if(pHit->sHitStuff.uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
            {
                AddStep(E_STEP_KNOCKDOWN);
            }
        }
    }
    else if(pMsg.Id == g_EventSobFainting)
    {
        AddStep(E_STEP_KO);
    }
	else if(pMsg.Id == g_EventAnimEnd)
	{
		for each( CNtlSLTBCrowdController* pController in m_listCurController )
			pController->HandleEvents(pMsg);
	}
}

RwBool CNtlSLTBCrowdManager::Save( const RwChar* szFileName ) 
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);	// 기본 버퍼를 1메가로 잡는다.

    nsl<<CNtlSLCENode::m_Version.fLastestVersion;
    nsl<<m_mapControllerContainer.size();

    for each(std::pair<std::string, CNtlSLTBCrowdController*> pairController in m_mapControllerContainer)
    {
		CNtlSLTBCrowdController* pController = pairController.second;

        if(pController)
            pController->Save(nsl);
    }

    return TRUE;
}

RwBool CNtlSLTBCrowdManager::Load( const RwChar* szFileName ) 
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);	// 기본 버퍼를 1메가로 잡는다.
    if(!nsl.LoadFile((char*)szFileName))
        return FALSE;

    return Load(nsl);
}

RwBool CNtlSLTBCrowdManager::Load(CNtlFileSerializer& rSerializer)
{
	RwUInt32 nSize = 0;
	CNtlSLTBCrowdController* pController = NULL;

	rSerializer>>CNtlSLCENode::m_Version.fCurrentWorkVerstion;
	rSerializer>>nSize;
	for(RwUInt32 i = 0; i < nSize; ++i)
	{
		pController = NTL_NEW CNtlSLTBCrowdController();
		pController->Load(rSerializer);        
		m_mapControllerContainer[pController->GetName()] = (pController);
	}

	return TRUE;
}

void CNtlSLTBCrowdManager::AddStep( ETBCrowdStepType eStep ) 
{
	// 루아에서 필요한 Controller를 불러오는 코드
	char pcStepName[128];
	GetLuaFuncName(eStep, pcStepName, 128);
	LuaExec_DirectionStep(pcStepName);
}

void CNtlSLTBCrowdManager::ChangeStep( ETBCrowdStepType eStep ) 
{
	for each(CNtlSLTBCrowdController* pController in m_listCurController)
	{
		pController->Destroy();        
	} 
    m_listCurController.clear();	

	// 루아에서 필요한 Controller를 불러오는 코드
	char pcStepName[128];
	GetLuaFuncName(eStep, pcStepName, 128);
	LuaExec_DirectionStep(pcStepName);

	for each(CNtlSLTBCrowdController* pController in m_listCurController )
		pController->Create();
}

void CNtlSLTBCrowdManager::ReserveStep(ETBCrowdStepType eStep)
{
	m_eReservatedStep = eStep;
}

ETBCrowdStepType CNtlSLTBCrowdManager::ConvertServerState( SNtlEventMinorMatchStateUpdate* pData ) 
{
	if( !pData )
		return E_STEP_BASIC;

    switch(pData->byMatchState)
    {
    case BUDOKAI_MINORMATCH_STATE_NONE:			// 초기 상태
        return E_STEP_BASIC;        
    case BUDOKAI_MINORMATCH_STATE_WAIT:			// 대전자들을 기다림
        return E_STEP_BASIC;
    case BUDOKAI_MINORMATCH_STATE_DIRECTION:		// 연출
        return E_STEP_BASIC;
    case BUDOKAI_MINORMATCH_STATE_MATCH_READY:	// 경기 준비
        return E_STEP_MATCH_READY;
    case BUDOKAI_MINORMATCH_STATE_STAGE_READY:	// 스테이지 준비
        return E_STEP_STAGE_READY;
    case BUDOKAI_MINORMATCH_STATE_STAGE_RUN:		// 스테이지 진행
        return E_STEP_STAGE_RUN;
    case BUDOKAI_MINORMATCH_STATE_STAGE_FINISH:	// 스테이지 종료
        return E_STEP_STAGE_FINISH;
    case BUDOKAI_MINORMATCH_STATE_MATCH_FINISH:	// 경기 종료
        return E_STEP_MATCH_FINISH;
    case BUDOKAI_MINORMATCH_STATE_END:
        return E_STEP_BATTLE_END;
    }

    return E_STEP_BASIC;
}

ETBCrowdStepType CNtlSLTBCrowdManager::ConvertServerState( SNtlEventMajorMatchStateUpdate* pData ) 
{
	if( !pData )
		return E_STEP_BASIC;

    switch(pData->byMatchState)
    {
    case BUDOKAI_MAJORMATCH_STATE_NONE:			// 초기 상태
        return E_STEP_BASIC;
    case BUDOKAI_MAJORMATCH_STATE_WAIT:			// 대전자들을 기다림
        return E_STEP_BASIC;
    case BUDOKAI_MAJORMATCH_STATE_DIRECTION:		// 연출
        return E_STEP_BASIC;
    case BUDOKAI_MAJORMATCH_STATE_MATCH_READY:	// 경기 준비
        return E_STEP_MATCH_READY;
    case BUDOKAI_MAJORMATCH_STATE_STAGE_READY:	// 스테이지 준비
        return E_STEP_STAGE_READY;
    case BUDOKAI_MAJORMATCH_STATE_STAGE_RUN:		// 스테이지 진행
        return E_STEP_STAGE_RUN;
    case BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH:	// 스테이지 종료
        return E_STEP_STAGE_FINISH;
    case BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH:	// 경기 종료
        return E_STEP_MATCH_FINISH;
    case BUDOKAI_MAJORMATCH_STATE_END:			// 종료
        return E_STEP_BATTLE_END;
    }

    return E_STEP_BASIC;
}

ETBCrowdStepType CNtlSLTBCrowdManager::ConvertServerState( SNtlEventFinalMatchStateUpdate* pData ) 
{
	if( !pData )
		return E_STEP_BASIC;

    switch(pData->byMatchState)
    {
    case BUDOKAI_FINALMATCH_STATE_NONE:         return E_STEP_BASIC;
    case BUDOKAI_FINALMATCH_STATE_WAIT:         return E_STEP_BASIC;
    case BUDOKAI_FINALMATCH_STATE_DIRECTION:    return E_STEP_BASIC;		
    case BUDOKAI_FINALMATCH_STATE_MATCH_READY:  return E_STEP_MATCH_READY;	
    case BUDOKAI_FINALMATCH_STATE_STAGE_READY:  return E_STEP_STAGE_READY;	
    case BUDOKAI_FINALMATCH_STATE_STAGE_RUN:    return E_STEP_STAGE_RUN;		
    case BUDOKAI_FINALMATCH_STATE_STAGE_FINISH: return E_STEP_STAGE_FINISH;	
    case BUDOKAI_FINALMATCH_STATE_MATCH_FINISH: return E_STEP_MATCH_FINISH;	
    case BUDOKAI_FINALMATCH_STATE_FINALDIRECTION: return E_STEP_FINAL_DIRECTION;
    case BUDOKAI_FINALMATCH_STATE_AWARDING:     return E_STEP_AWARD;		
    case BUDOKAI_FINALMATCH_STATE_END:          return E_STEP_BATTLE_END;			        
    }

    return E_STEP_BASIC;
}

CNtlSLTBCrowdManager* CNtlSLTBCrowdManager::GetInstance() 
{
    if(!m_pInstance)
    {
        m_pInstance = NTL_NEW CNtlSLTBCrowdManager();
    }

    return m_pInstance;
}

void CNtlSLTBCrowdManager::DeleteInstance() 
{
    NTL_DELETE(m_pInstance);
}

void CNtlSLTBCrowdManager::AddController(CNtlSLTBCrowdController* pController)
{
	ITER_MAP_CONTROLLER it_MapController = m_mapControllerContainer.find(pController->GetName());
	if( it_MapController != m_mapControllerContainer.end() )
		return;

	m_mapControllerContainer[pController->GetName()] = pController;
}

void CNtlSLTBCrowdManager::RemoveController( const std::string& keyName ) 
{
	RemovePlayController(keyName);

	ITER_MAP_CONTROLLER it_mapController = m_mapControllerContainer.begin();
	for( ; it_mapController != m_mapControllerContainer.end() ; ++it_mapController )
	{
		CNtlSLTBCrowdController* pController = it_mapController->second;

		if(pController->GetName() == keyName)
		{
			NTL_DELETE(pController);
			m_mapControllerContainer.erase(it_mapController);

			// 컨트롤러가 하나도 없으면 현재 버전은 최신 버전으로....
			if( m_mapControllerContainer.size() == 0 )
				CNtlSLCENode::m_Version.fCurrentWorkVerstion = CNtlSLCENode::m_Version.fLastestVersion;

			break;
		}
	}
}

void CNtlSLTBCrowdManager::AddPlayController( CNtlSLTBCrowdController* pController )
{
	if(!pController)
		return;

    if(!ms_bDirectionToolMode && !GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_T_BUDOKAI))
        return;

	pController->Create();
	m_listCurController.push_back(pController);
}

void CNtlSLTBCrowdManager::AddPlayController( const std::string& keyName ) 
{
	ITER_MAP_CONTROLLER it_MapController = m_mapControllerContainer.find(keyName);
	if( it_MapController == m_mapControllerContainer.end() )
		return;

	AddPlayController(it_MapController->second);
}

void CNtlSLTBCrowdManager::RemovePlayController(const std::string& keyName)
{
	ITER_LIST_CONTROLLER it_listController = m_listCurController.begin();
	for( ; it_listController != m_listCurController.end() ; ++it_listController )
	{
		CNtlSLTBCrowdController* pController = *it_listController;

		if(pController->GetName() == keyName)
		{
			pController->Destroy();
			m_listCurController.erase(it_listController);
			break;
		}
	}
}

void CNtlSLTBCrowdManager::GetLuaFuncName(ETBCrowdStepType eStep, char* pcLuaFuncName, RwInt32 iBufferSize)
{
	switch(eStep)
	{
	case E_STEP_BASIC:				sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Basic_Step");				break;
	case E_STEP_MATCH_READY:		sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Match_Ready");				break;
	case E_STEP_STAGE_READY:		sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Stage_Ready");				break;
	case E_STEP_STAGE_RUN:			sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Stage_Run");				break;
	case E_STEP_STAGE_FINISH:		sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Stage_Finish");			break;
	case E_STEP_MATCH_FINISH:		sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Match_Finish");			break;
	case E_STEP_AWARD:				sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Award");			    	break;
	case E_STEP_BATTLE_END:			sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_End_Time");				break;
	case E_STEP_HTB:				sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_HTB");			        	break;
	case E_STEP_KNOCKDOWN:			sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_Knock_Down");				break;
	case E_STEP_KO:					sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_KO");			        	break;
    case E_STEP_FINAL_DIRECTION:    sprintf_s(pcLuaFuncName, iBufferSize, "%s()", "Tenkaichi_FinalDirection");          break;
	default:						NTL_ASSERTFAIL("CNtlSLTBCrowdManager::GetLuaFuncName, Invalid direction step : " << eStep);
	}
}

void CNtlSLTBCrowdManager::CreateSobRefree() 
{
    if(m_pRefreeActor)
        return;

    // 심판 객체 생성
    SERIAL_HANDLE hSerialId = CNtlSobFactory::AcquireSerailId();
    sNPC_BRIEF sNPCBrief;
    sCHARSTATE sCharState;

    Logic_MakeNPCBrief(&sNPCBrief, REFREE_TBL_IDX);
    Logic_MakeNPCSpawnState(&sCharState, ZeroAxis, ZAxis);
    sCharState.sCharStateBase.byStateID = CHARSTATE_STANDING;
    CNtlSLEventGenerator::SobNpcCreate(SLCLASS_NPC, hSerialId, ZeroAxis, ZAxis, &sNPCBrief, &sCharState, FALSE, TRUE);
    m_pRefreeActor = (CNtlSobNpc*)GetNtlSobManager()->GetSobObject(hSerialId);
    m_pRefreeActor->SetFlags(m_pRefreeActor->GetFlags() & ~SLFLAG_OBJECT_COLLISION);
}

CNtlSobNpc* CNtlSLTBCrowdManager::GetRefreeActor() 
{
    if(!m_pRefreeActor)
        CreateSobRefree();

    return m_pRefreeActor;
}