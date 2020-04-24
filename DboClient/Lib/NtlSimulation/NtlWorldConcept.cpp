#include "precomp_ntlsimulation.h"
#include "NtlWorldConcept.h"

// core
#include "NtlDebug.h"

// sound
#include "NtlSoundEventGenerator.h"

// presentation
#include "NtlPLVisualManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlDNController.h"

// simulation
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSobActor.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobManager.h"
#include "NtlSobGroup.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLRankBattle.h"
#include "NtlSLLuaGlue.h"
#include "NtlCameraManager.h"
#include "NtlCamera.h"
#include "NtlSLLogic.h"
#include "NtlFSMDef.h"


// World Concept Controller
#include "NtlWorldConceptPVP.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlWorldConceptTrade.h"
#include "NtlWorldConceptDBC.h"
#include "NtlWorldConceptPrivateShop.h"
#include "NtlWorldConceptTMQ.h"
#include "NtlWorldConceptTutorial.h"
#include "NtlWorldConceptRB.h"
#include "NtlWorldConceptDirect.h"
#include "NtlWorldConceptTB.h"
#include "NtlWorldConceptPartyDungeon.h"
#include "NtlWorldConceptTLQ.h"
#include "NtlWorldConceptScramble.h"
#include "NtlWorldConceptFreePvpZone.h"
#include "NtlWorldConceptCCBD.h"



CNtlWorldConcept* CNtlWorldConcept::m_pInstance = NULL;

CNtlWorldConcept* CNtlWorldConcept::GetInstance(void)
{
	return m_pInstance;
}

CNtlWorldConcept::CNtlWorldConcept()
{
	NTL_ASSERT(m_pInstance == NULL, "CNtlWorldConcept::CNtlWorldConcept");

	m_pInstance				= this;

	m_bActionEnable			= TRUE;
    m_bIsNightOn			= FALSE;


	m_bHasTutorialWorldConcept = FALSE;
}

CNtlWorldConcept::~CNtlWorldConcept()
{
	m_pInstance = NULL;	

	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		CNtlWorldConceptController* pController = *it;
		NTL_DELETE(pController);
	}

	m_listController.clear();
}


RwBool CNtlWorldConcept::Create(void)
{
	LinkMsg(g_EventNightEffect);
	LinkMsg(g_EventTLCondition);
	LinkMsg(g_EventDirectionNfy);
	LinkMsg(g_EventTLMouseInputLockUnlock);
	LinkMsg(g_EventTLKeyboardInputLockUnlock);
	LinkMsg(g_EventTLAttackLockUnlock);
	LinkMsg(g_EventSobTargetInfoUpdate);
	LinkMsg(g_EventSobAttack);
	LinkMsg(g_EventSobSkillAction);	
	LinkMsg(g_EventTLDisableMinimap);
	LinkMsg(g_EventTLEnableSkip);
	LinkMsg(g_EventDojo);

	return TRUE;
}

void CNtlWorldConcept::Destroy(void)
{
	UnLinkMsg(g_EventNightEffect);
	UnLinkMsg(g_EventTLCondition);
	UnLinkMsg(g_EventDirectionNfy);
	UnLinkMsg(g_EventTLMouseInputLockUnlock);
	UnLinkMsg(g_EventTLKeyboardInputLockUnlock);
	UnLinkMsg(g_EventTLAttackLockUnlock);
	UnLinkMsg(g_EventSobTargetInfoUpdate);
	UnLinkMsg(g_EventSobAttack);
	UnLinkMsg(g_EventSobSkillAction);
	UnLinkMsg(g_EventTLDisableMinimap);	
	UnLinkMsg(g_EventTLEnableSkip);	
	UnLinkMsg(g_EventDojo);
}

void CNtlWorldConcept::Reset(void)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		CNtlWorldConceptController* pController = *it;
		NTL_DELETE(pController);
	}

	m_listController.clear();
}


void CNtlWorldConcept::Update(RwReal fElapsed)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); )
	{
		(*it)->Update(fElapsed);
		if((*it)->GetState() == WORLD_STATE_NONE)
		{
			CNtlWorldConceptController* pController = *it;
			NTL_DELETE(pController);
			it = m_listController.erase(it);

		}
		else
			it++;
	}	
}

void CNtlWorldConcept::AddWorldPlayConcept( EWorldPlayConcept ePlayConcept ) 
{
	CNtlWorldConceptController *pController = FindGradeExtController( ePlayConcept );		
	NTL_ASSERT(pController == NULL, "CNtlWorldConcept::AddWorldPlayConcept : same grade controller is exist !!!" << ePlayConcept );
		
	switch(ePlayConcept)
	{
		case WORLD_PLAY_TUTORIAL:
			pController = NTL_NEW CNtlWorldConceptTutorial();
			break;
		case WORLD_PLAY_TIME_MACHINE:
			pController = NTL_NEW CNtlWorldConceptTMQ();
			break;
		case WORLD_PLAY_RANK_BATTLE:
			pController = NTL_NEW CNtlWorldConceptRB();
			break;
		// 2008-06-13 by Kell
		case WORLD_PLAY_T_BUDOKAI:
			pController = NTL_NEW CNtlWorldConceptTB();
			break;
		case WORLD_PLAY_FREEPVP:
			pController = NTL_NEW CNtlWorldConceptPVP();
			break;
		case WORLD_PLAY_NPC_COMMU:
			pController = NTL_NEW CNtlWorldConceptNPCCOMMU();
			break;
		case WORLD_PLAY_TRADE:
			pController = NTL_NEW CNtlWorldConceptTrade();
			break;
		case WORLD_PLAY_DRAGONBALL_COLLECT:
			pController = NTL_NEW CNtlWorldConceptDBC();
			break;
		case WORLD_PLAY_PRIVATESHOP:
			pController = NTL_NEW CNtlWorldConceptPrivateShop();
			break;
		case WORLD_PLAY_PRIVATESHOP_VISITOR:
			pController = NTL_NEW CNtlWorldConceptPrivateShopVisitor();
			break;
		case WORLD_PLAY_DIRECT:
			pController = NTL_NEW CNtlWorldConceptDirect();
			break;
		case WORLD_PLAY_PARTY_DUNGEON:
			pController = NTL_NEW CNtlWorldConceptPartyDungeon();
			break;
		case WORLD_PLAY_TLQ:
			pController = NTL_NEW CNtlWorldConceptTLQ();
			break;
		case WORLD_PLAY_DOJO_SCRAMBLE:
			pController = NTL_NEW CNtlWorldConceptScramble();
			break;
		case WORLD_PLAY_FREEPVP_ZONE:
			pController = NTL_NEW CNtlWorldConceptFreePvpZone();
			break;
		case WORLD_PLAY_CCBD:
			pController = NTL_NEW CNtlWorldConceptCCBD();
			break;
		default:
			NTL_ASSERTFAIL("CNtlWorldConcept::AddWorldPlayConcept : find not id !!!");
			break;
	}

	m_listController.push_back(pController);

	pController->SetConceptType(ePlayConcept);
	pController->ChangeState(WORLD_STATE_ENTER);

	if( WORLD_PLAY_TUTORIAL != ePlayConcept &&
		IsGrade(WORLD_CONCEPT_FIRST_GRADE, ePlayConcept) )
		CNtlSoundEventGenerator::SendEventMusic(EVENT_MUSIC_LEAVE_MAIN_WORLD_CONCEPT);
}

void CNtlWorldConcept::RemoveWorldPlayConcept( EWorldPlayConcept ePlayConcept ) 
{
	RwBool bIncIter;

	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); )
	{
		bIncIter = TRUE;

		if((*it)->GetConceptType() == ePlayConcept)
		{
			(*it)->ChangeState(WORLD_STATE_EXIT);
			if((*it)->GetState() == WORLD_STATE_NONE)
			{
				CNtlWorldConceptController* pController = *it;
				NTL_DELETE(pController);
				it = m_listController.erase(it);
				
				bIncIter	= FALSE;
			}
		}

		if(bIncIter)
			it++;
	}
}

void CNtlWorldConcept::DeleteGradeWorldPlayConcept(RwUInt32 uiGrade)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); )
	{
		RwUInt32 uiConceptType = (RwUInt32)(*it)->GetConceptType();
		if( ( uiGrade >> 4 ) == ( uiConceptType >> 4 ) )
		{
			(*it)->ChangeState(WORLD_STATE_EXIT);
			CNtlWorldConceptController* pController = *it;
			NTL_DELETE(pController);
			it = m_listController.erase(it);
		}
		else
			it++;
	}
}


CNtlWorldConceptController* CNtlWorldConcept::FindGradeExtController( RwUInt32 uiGrade )		
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		RwUInt32 uiConceptType = (RwUInt32)(*it)->GetConceptType();
		if( ( uiGrade >> 4 ) == ( uiConceptType >> 4 ) )
			return (*it);
	}

	return NULL;
}


CNtlWorldConceptController* CNtlWorldConcept::GetWorldConceptController( EWorldPlayConcept eWorldConcept ) 
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if( (*it)->GetConceptType() == eWorldConcept )
			return (*it);
	}

	return NULL;
}

void CNtlWorldConcept::EnableAction(RwBool bEnable)
{
	m_bActionEnable = bEnable;
}

RwBool CNtlWorldConcept::IsEnableAction( EAvatarAction eAction, BOOL bUnknown/* = FALSE*/)
{
	if(!m_bActionEnable)
		return FALSE;

	// to do
	switch (eAction)
	{
		case E_ACTION_CHARGING:
		{

		}
		break;
	}

	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if( !(*it)->IsEnableAction(eAction) )
			return FALSE;
	}

	return TRUE;
}

void CNtlWorldConcept::BeforeLoadingDirect(void)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		(*it)->BeforeLoadingDirect();
	}
}

void CNtlWorldConcept::AfterLoadingDirect(void)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		(*it)->AfterLoadingDirect();
	}
}

RwBool CNtlWorldConcept::IsBeforeLoadingDirect(void)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if(!(*it)->IsBeforeLoadingDirect())
			return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldConcept::IsAfterLoadingDirect(void)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if(!(*it)->IsBeforeLoadingDirect())
			return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldConcept::IsGrade(RwUInt32 uiGrade, EWorldPlayConcept ePlayType)
{
	if( BIT_FLAG_TEST(uiGrade, ePlayType) )
		return true;

	return false;
}

RwBool CNtlWorldConcept::HasTutorialWorldConcept( void )
{
	return m_bHasTutorialWorldConcept;
}

void CNtlWorldConcept::SetHaveTutorialWorldConcept( RwBool bHave )
{
	m_bHasTutorialWorldConcept = bHave;
}

void CNtlWorldConcept::HandleEvents(RWS::CMsg &pMsg)
{
	// DBC의 Night Effect를 적용한다.
	if(pMsg.Id == g_EventNightEffect)
	{
		SNtlEventNightEffect* pNightEffect = reinterpret_cast<SNtlEventNightEffect*>(pMsg.pData);
		if(pNightEffect->bOn && !m_bIsNightOn)      // 현재 Night 상태가 아닌 경우에만 호출된다
		{
			GetSceneManager()->GetWorld()->OnDragonSkyAppearence(TRUE);
			LuaExec_DragonDNEnter();		// lua script 호출		
            m_bIsNightOn = TRUE;
		}
		else if(!pNightEffect->bOn && m_bIsNightOn) // 현재 Night 상태가 아닌 경우에는 호출되지 않는다.
		{
			GetSceneManager()->GetWorld()->OnDragonSkyAppearence(FALSE);		
			GetDnController()->AddDNNodeCurrentStart(0.0f, 3.0f, 0);		
            m_bIsNightOn = FALSE;
		}
	}

	ListController::iterator it = m_listController.begin();
	for ( ; it != m_listController.end(); ++it )
	{
		(*it)->HandleEvents( pMsg );
	}
}


RwBool CNtlWorldConcept::IsActivePlayConcept( EWorldPlayConcept ePlayConcept ) 
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if( (*it)->GetConceptType() == ePlayConcept )
			return TRUE;
	}

	return FALSE;
}

RwBool CNtlWorldConcept::IsActivePlayGrade(RwUInt32 uiGrade)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if( (RwUInt32)((*it)->GetConceptType() >> 4 ) == ( uiGrade >> 4 ) )
			return TRUE;
	}

	return FALSE;
}


RwBool CNtlWorldConcept::IsMoveableArea(CNtlSobActor *pActor, const RwV3d *pPos, const RwV3d *pDestPos)
{
	CNtlWorldConceptController *pController = FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if(pController == NULL)
		return TRUE;

	return pController->IsMoveableArea(pActor, pPos, pDestPos);
}

RwBool CNtlWorldConcept::IsVisiblePCInCinematic(VOID)
{
	CNtlWorldConceptController *pController = FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( pController )
		return pController->IsVisiblePCInCinematic();

	pController = FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
	if( pController )
		return pController->IsVisiblePCInCinematic();

	pController = FindGradeExtController(WORLD_CONCEPT_THIRD_GRADE);
	if( pController )
		return pController->IsVisiblePCInCinematic();

	// Default;
	return FALSE;
}

RwBool CNtlWorldConcept::CanUseSkill(RwUInt32 hSerialID, RwUInt32& uiResultCode)
{
	ListController::iterator it;
	for(it = m_listController.begin(); it != m_listController.end(); it++)
	{
		if( !(*it)->CanUseSkill(hSerialID, uiResultCode) )
			return FALSE;
	}

	return TRUE;
}


/**
 * 아바타의 타겟이 적인지 파악한다. (적은 Mob과 PVP 상대이다)
 * \param pActor 타겟 Actor 객체의 포인터
 * \param hTargetSerial 타겟 Actor 객체의 Serial ID
 * return 타겟이 적인면 True를 반환하고, 아니면 False를 반환한다.
 */
RwBool CNtlWorldConcept::IsEnemyTargetFromAvatarActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar == NULL)
		return FALSE;

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(hTargetSerial);
	if(pSobObj == NULL)
		return FALSE;

	return IsEnemyTargetFromAvatarActor(pActor, pSobObj);
}

RwBool CNtlWorldConcept::IsEnemyTargetFromAvatarActor(CNtlSobActor* pActor, CNtlSob* pSobObj)
{
	// to do
	//if (*(_DWORD*)(*(_DWORD*)(*(_DWORD*)(vF1F8 + 168) + 8) + 32) & 0x1000)
	//	return 1;

	RwUInt32 uiTargetClassId = pSobObj->GetClassID();

	if (uiTargetClassId == SLCLASS_MONSTER)					// 타겟이 몹이라면 무조건 TRUE
		return TRUE;
	else if (uiTargetClassId == SLCLASS_PLAYER)				// 타겟이 Player라면 PVP상태의 타겟만이 적이된다.
	{
		if (IsActivePlayConcept(WORLD_PLAY_FREEPVP_ZONE))
		{
			CNtlWorldConceptFreePvpZone* pWorldConceptPvpZone = (CNtlWorldConceptFreePvpZone*)GetWorldConceptController(WORLD_PLAY_FREEPVP_ZONE);

			if (pWorldConceptPvpZone)
			{
				if (pWorldConceptPvpZone->GetState() != WORLD_STATE_IDLE)
					return FALSE;

				CNtlSobPlayer * pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(pSobObj);
				if (pSobPlayer)
				{
					CNtlSobPlayerAttr* pSobPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pSobPlayer->GetSobAttr());

					RwUInt32 stateid = Logic_GetActorStateId(pSobPlayer);
					if (stateid != NTL_FSMSID_FAINTING && stateid != NTL_FSMSID_DIE && pSobPlayerAttr->GetIsInFreePvpZone())
						return TRUE;
				}
			}

			return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_FREEPVP))
		{
			CNtlWorldConceptPVP* pWorldConceptPVP = (CNtlWorldConceptPVP*)GetWorldConceptController(WORLD_PLAY_FREEPVP);

			if (pWorldConceptPVP)
			{
				if (pWorldConceptPVP->GetState() != WORLD_STATE_IDLE)
					return FALSE;

				if (pSobObj->GetSerialID() == pWorldConceptPVP->GetFreePvPTarget())
					return TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_RANK_BATTLE))
		{
			if (GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsAttackableEnemy(pSobObj->GetSerialID()))
				return TRUE;
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_T_BUDOKAI))
		{
			CNtlWorldConceptTB* pWorldConceptTB = reinterpret_cast<CNtlWorldConceptTB*>(GetWorldConceptController(WORLD_PLAY_T_BUDOKAI));

			if (!pWorldConceptTB || pWorldConceptTB->GetState() != WORLD_STATE_IDLE)
				return FALSE;

			if (pWorldConceptTB->IsAttackableEnemy(pSobObj->GetSerialID()))
				return TRUE;
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE))
		{
			CNtlWorldConceptScramble* pWorldConceptScramble = (CNtlWorldConceptScramble*)GetWorldConceptController(WORLD_PLAY_DOJO_SCRAMBLE);

			if (pWorldConceptScramble)
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if (!pAvatar)
					return FALSE;

				CNtlDojo* pDojo = pAvatar->GetDojo();
				sSCRAMBLE_INFO* pSCRAMBLE_INFO = pDojo->GetScramble();

				if (pSCRAMBLE_INFO->eDojoState != eDBO_DOJO_STATUS_START)
					return FALSE;

				if (pWorldConceptScramble->GetState() != WORLD_STATE_IDLE)
					return FALSE;

				if (pWorldConceptScramble->IsAttackableEnemy(pSobObj->GetSerialID()))
					return TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
	}
	else if (uiTargetClassId == SLCLASS_PET)					// 타겟이 Pet이고, PVP상태의 적이 pet의 주인인경우에만 적이 된다.
	{
		SERIAL_HANDLE hTargetOwnerSerial = pSobObj->GetOwnerID();

		if (hTargetOwnerSerial == INVALID_SERIAL_ID)
			return FALSE;

		if (IsActivePlayConcept(WORLD_PLAY_FREEPVP))
		{
			CNtlWorldConceptPVP* pWorldConceptPVP = (CNtlWorldConceptPVP*)GetWorldConceptController(WORLD_PLAY_FREEPVP);

			if (pWorldConceptPVP)
			{
				if (pWorldConceptPVP->GetState() != WORLD_STATE_IDLE)
					return FALSE;

				if (hTargetOwnerSerial == pWorldConceptPVP->GetFreePvPTarget())
					return TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_RANK_BATTLE))
		{
			if (GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->IsAttackableEnemy(hTargetOwnerSerial))
				return TRUE;
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_T_BUDOKAI))
		{
			CNtlWorldConceptTB* pWorldConceptTB = reinterpret_cast<CNtlWorldConceptTB*>(GetWorldConceptController(WORLD_PLAY_T_BUDOKAI));

			if (!pWorldConceptTB || pWorldConceptTB->GetState() != WORLD_STATE_IDLE)
				return FALSE;

			if (pWorldConceptTB->IsAttackableEnemy(hTargetOwnerSerial))
				return TRUE;
			else
				return FALSE;
		}
		else if (IsActivePlayConcept(WORLD_PLAY_DOJO_SCRAMBLE))
		{
			CNtlWorldConceptScramble* pWorldConceptScramble = (CNtlWorldConceptScramble*)GetWorldConceptController(WORLD_PLAY_DOJO_SCRAMBLE);

			if (pWorldConceptScramble)
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if (!pAvatar)
					return FALSE;

				CNtlDojo* pDojo = pAvatar->GetDojo();
				sSCRAMBLE_INFO* pSCRAMBLE_INFO = pDojo->GetScramble();

				if (pSCRAMBLE_INFO->eDojoState != eDBO_DOJO_STATUS_START)
					return FALSE;

				if (pWorldConceptScramble->GetState() != WORLD_STATE_IDLE)
					return FALSE;

				if (pWorldConceptScramble->IsAttackableEnemy(pSobObj->GetSerialID()))
					return TRUE;
				else
					return FALSE;
			}
			else
				return FALSE;
		}
	}

	return FALSE;
}


CNtlSobActor* CNtlWorldConcept::FindNearEnemyFromAvatarActor(CNtlSobActor *pActor)
{
	// pvp platform
	if (IsActivePlayConcept(WORLD_PLAY_FREEPVP_ZONE))
	{
		CNtlWorldConceptFreePvpZone* pWorldConceptPVPZone = (CNtlWorldConceptFreePvpZone*)GetWorldConceptController(WORLD_PLAY_FREEPVP_ZONE);
		if (!pWorldConceptPVPZone || pWorldConceptPVPZone->GetState() != WORLD_STATE_IDLE)
		{
			return NULL;
		}

		return pWorldConceptPVPZone->GetNearAttackableEnemy(pActor);
	}
	// freebattle
	else if ( IsActivePlayConcept( WORLD_PLAY_FREEPVP ) )
	{
		CNtlWorldConceptPVP* pWorldConceptPVP = (CNtlWorldConceptPVP*) GetWorldConceptController( WORLD_PLAY_FREEPVP );
		if ( !pWorldConceptPVP || pWorldConceptPVP->GetState() != WORLD_STATE_IDLE )
		{
			return NULL;
		}

		return reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( pWorldConceptPVP->GetFreePvPTarget() ) );
	}
	// rank battle
	else if ( IsActivePlayConcept( WORLD_PLAY_RANK_BATTLE ) ) 
	{
		return GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->GetNearAttackableEnemy( pActor );
	}
	// budokai
	else if ( IsActivePlayConcept( WORLD_PLAY_T_BUDOKAI ) )
	{
		CNtlWorldConceptTB* pWorldConceptTB = reinterpret_cast<CNtlWorldConceptTB*>( GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		if ( !pWorldConceptTB || pWorldConceptTB->GetState() != WORLD_STATE_IDLE )
		{
			return NULL;
		}

		return pWorldConceptTB->GetNearAttackableEnemy( pActor );
	}
	// dojo battle
	else if ( IsActivePlayConcept( WORLD_PLAY_DOJO_SCRAMBLE ) )
	{
		CNtlWorldConceptScramble* pWorldConcept = reinterpret_cast<CNtlWorldConceptScramble*>( GetWorldConceptController( WORLD_PLAY_DOJO_SCRAMBLE ) );
		if ( !pWorldConcept || pWorldConcept->GetState() != WORLD_STATE_IDLE )
		{
			return NULL;
		}

		return pWorldConcept->GetNearAttackableEnemy( pActor );
	}
	// Judge the enemy in the normal state
	else
	{
        return Logic_GetAutoTarget(pActor);
		
	}
}
