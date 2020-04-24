#include "precomp_ntlsimulation.h"
#include "NtlBehaviorCharAirMove.h"

// shared 
#include "NtlMovement.h"

// presentation
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"
#include "NtlPLHelpers.h"


// simulation
#include "NtlFSMDef.h"
#include "NtlSLApi.h"
#include "NtlSobActor.h"
#include "NtlSobCharProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"

DEFINITION_MEMORY_POOL(CNtlBehaviorCharAirMove)

CNtlBehaviorCharAirMove::CNtlBehaviorCharAirMove()
{
	SetBehaviorId(NTL_BEID_CHAR_AIR_MOVE);
	SetBehaviorName(NTL_BENAME_CHAR_AIR_MOVE);

	m_pPLAirEffect = NULL;
}

CNtlBehaviorCharAirMove::~CNtlBehaviorCharAirMove()
{
	DestroyAirEffect();
}

void CNtlBehaviorCharAirMove::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

	SetAnim(pMoveStuff->byMoveFlags);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter();
}

void CNtlBehaviorCharAirMove::Exit(void)
{
	DestroyAirEffect();

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>(pSobProxy->GetPLMainEntity());
	pPLCharacter->SetBlend(BLEND_TWEEN);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharAirMove::Update(RwReal fElapsed)
{
	// Avoid sudden splashes when the frame is low
	fElapsed = min(fElapsed, 0.2f);

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();

	RwBool bFinish = FALSE;

	if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY)
	{
		bFinish = UpdateAirFly(pMoveStuff, fElapsed);
	}
	else if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
	{
		if (pMoveStuff->byType == NTL_MOVETARGET_AIR_DASH)
			bFinish = UpdateAirStart(pMoveStuff, fElapsed);
		else
			bFinish = UpdateAirDash(pMoveStuff, fElapsed);
	}
	else if (pMoveStuff->byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
	{
		bFinish = UpdateAirAccel(pMoveStuff, fElapsed);
	}

	// effect position update
	if (m_pPLAirEffect)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwV3d vDir = m_pActor->GetDirection();
		vPos = vPos + m_vEffectOffset;
		m_pPLAirEffect->SetPosition(&vPos);
		m_pPLAirEffect->SetDirection(&vDir);

		UpdateAirEffect(vDir);
	}

	if (bFinish)
	{
		Finish();
	}
}


void CNtlBehaviorCharAirMove::UpdateAirEffect(RwV3d& vDir)
{
	RwV3d vPos = m_pActor->GetPosition();

	// Effect
	RwMatrix mat;
	RwMatrixSetIdentity(&mat);

	RwV3dAssignMacro(&mat.at, &vDir);
	RwV3dCrossProduct(&mat.right, &mat.at, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&mat.right, &mat.right);
	RwV3dCrossProduct(&mat.up, &mat.at, &mat.right);
	RwV3dNormalize(&mat.up, &mat.up);
	RwV3dAssignMacro(&mat.up, &CNtlPLGlobal::m_vYAxisV3);
	RwMatrixUpdate(&mat);

	CNtlMath::MathRwV3dAssign(&m_vEffectOffset, vDir.x*0.6f, 0.5f, vDir.z*0.5f);

	RwV3dAdd(&vPos, &vPos, &m_vEffectOffset);
	RwMatrixTranslate(&mat, &vPos, rwCOMBINEPOSTCONCAT);

	if (m_pPLAirEffect)
		m_pPLAirEffect->SetMatrix(mat);
}

RwUInt32 CNtlBehaviorCharAirMove::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharAirMove::HandleEvents");



	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharAirMove::UpdateData(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	memcpy(&m_MoveStuff, pMoveStuff, sizeof(SMoveStuff));

	SetAnim(pMoveStuff->byMoveFlags);
}


void CNtlBehaviorCharAirMove::SetAnim(RwUInt8 byMoveFlags)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	RwUInt32 uiNextAnimKey = 0;

	if (m_MoveStuff.byType == NTL_MOVETARGET_AIR_DASH)
	{
		uiNextAnimKey = FLYING_JUMP_HIGH;

		CreateAirEffect("max_fly_Up_effect_01");
		UpdateAirEffect(m_MoveStuff.vDir);
	}
	else if (m_MoveStuff.byType == NTL_MOVETARGET_DIR)
	{
		if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY)
		{
			if (byMoveFlags == NTL_MOVE_F || byMoveFlags == NTL_MOVE_F_TURN_L || byMoveFlags == NTL_MOVE_F_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_FRONT;
			}
			else if (byMoveFlags == NTL_MOVE_B || byMoveFlags == NTL_MOVE_B_TURN_L || byMoveFlags == NTL_MOVE_B_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_BACK;
			}
			else if (byMoveFlags == NTL_MOVE_L || byMoveFlags == NTL_MOVE_L_TURN_L || byMoveFlags == NTL_MOVE_L_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_LEFT;
			}
			else if (byMoveFlags == NTL_MOVE_R || byMoveFlags == NTL_MOVE_R_TURN_L || byMoveFlags == NTL_MOVE_R_TURN_R)
			{
				uiNextAnimKey = FLYING_MOVE_RIGHT;
			}
			else if (byMoveFlags == NTL_MOVE_TURN_L || byMoveFlags == NTL_MOVE_TURN_R)
			{
				uiNextAnimKey = FLYING_STAND_IDLE;
			}
			else if (byMoveFlags == NTL_MOVE_HEIGHT_DOWN)
			{
				uiNextAnimKey = FLYING_STAND_DOWN;
			}
			else if (byMoveFlags == NTL_MOVE_HEIGHT_UP)
			{
				uiNextAnimKey = FLYING_STAND_UP;
			}
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
		{
			CreateAirEffect("max_floating_foward_01");
			UpdateAirEffect(m_MoveStuff.vDir);

			uiNextAnimKey = FLYING_DASH_FRONT;
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
		{
			CreateAirEffect(NTL_VID_DASH_ACTIVE_02);
			UpdateAirEffect(m_MoveStuff.vDir);

			uiNextAnimKey = FLYING_POSE_1;
		}
	}
	else if (m_MoveStuff.byType == NTL_MOVETARGET_LOC)
	{
		if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_DASH)
		{
			uiNextAnimKey = FLYING_DASH_FRONT;
		}
		else if (m_MoveStuff.byFormFlag == NTL_MOVE_FLAG_FLY_ACCEL)
		{
			uiNextAnimKey = FLYING_POSE_1;
		}
		else
		{
			uiNextAnimKey = FLYING_MOVE_FRONT;
		}
	}

	if (uiCurrAnimKey != uiNextAnimKey)
	{
		CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>(pSobProxy->GetPLMainEntity());
		pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.20f);

		pSobProxy->SetBaseAnimation(uiNextAnimKey);
	}
}


void CNtlBehaviorCharAirMove::CreateAirEffect(const RwChar* effectName)
{
	if (!m_pActor)
		return;

	DestroyAirEffect();

	CNtlPLAttach* pPLChar = (CNtlPLAttach*)m_pActor->GetSobProxy()->GetPLMainEntity();

	m_pPLAirEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, effectName);
	pPLChar->Attach((CNtlPLAttach*)m_pPLAirEffect);
}


void CNtlBehaviorCharAirMove::DestroyAirEffect(void)
{
	if (m_pPLAirEffect)
	{
		m_pPLAirEffect->Finish();
		m_pPLAirEffect = NULL;
	}
}





RwBool CNtlBehaviorCharAirMove::UpdatePositionMove(SMoveStuff * pMoveStuff, OUT RwV3d & vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeed)
{
	if (pMoveStuff->byMoveFlags != NTL_MOVE_NONE)
	{
		RwV3d vDir = m_pActor->GetDirection();

		CNtlVector vHeading, vDest;
	//	DBO_WARNING_MESSAGE("pMoveStuff->byMoveFlags: " << (int)pMoveStuff->byMoveFlags);
		NtlGetDestination_Keyboard(vDir.x, vDir.y, vDir.z, fSpeed, vPos.x, vPos.y, vPos.z, pMoveStuff->byMoveFlags, (DWORD)(fElapsed*1000.f), 1.0f, &vHeading, &vDest);

		RwV3d vNewDir;
		CNtlMath::MathRwV3dAssign(&vNewDir, vHeading.x, vHeading.y, vHeading.z);

		m_pActor->SetDirection(&vNewDir);

		vPos.x = vDest.x;
		vPos.y = vDest.y;
		vPos.z = vDest.z;

		// If it's a player, correct Move Sync.
		//if (m_pActor->GetClassID() == SLCLASS_PLAYER)
		//{
			//UpdateMoveSync(vPos, fElapsed);

			// Apply the calculated result.
			//vPos = m_pActor->GetPosition();
		//	return FALSE; // return, we dont need collision check on player
		//}

		// Collision check.
		if (m_pActor->GetFlags() & SLFLAG_OBJECT_COLLISION)
		{
			RwReal fCurrHeight = vPos.y;
			RwBool bCollMoveImPossible;
			RwUInt8 byColliResult = Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImPossible, fElapsed, FALSE);

			vPos.y = fCurrHeight;

			if (byColliResult != NTL_CHARACTER_COLLI_NONE)
			{
				if (bCollMoveImPossible)
				{
					SetFalling();
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

RwBool CNtlBehaviorCharAirMove::UpdateAirStart(SMoveStuff * pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fMoveSpeed = Logic_GetFlyDashSpeed(m_pActor);

	RwBool bFinish = UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fMoveSpeed);

	if (!bFinish)
	{
		Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

		if (vPos.y >= m_sHStuff.fWorldHeight) //check if we on height limit
		{
		//	vPos.y = m_sHStuff.fWorldHeight;
		}
	}

	m_pActor->SetPosition(&vPos);

	return bFinish;
}

RwBool CNtlBehaviorCharAirMove::UpdateAirFly(SMoveStuff * pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFrontFlySpeed(m_pActor);

	RwBool bFinish = UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);

	if (!bFinish)
	{
		Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

		if (vPos.y <= m_sHStuff.fFinialHeight) //check if we land on ground
		{
			DBO_WARNING_MESSAGE("UF vPos.y: " << vPos.y << ", height: " << m_sHStuff.fFinialHeight << ", WATER-height: " << m_sHStuff.fWaterHeight);
			vPos.y = m_sHStuff.fFinialHeight;

		//	if (m_pActor->GetClassID() == SLCLASS_AVATAR) // if we do this check, then when avatar fly on ground, he will keep flying (for other players)
			{
				SetFalling();
				bFinish = TRUE;
			}
		}
	}

	m_pActor->SetPosition(&vPos);

	return bFinish;
}

RwBool CNtlBehaviorCharAirMove::UpdateAirDash(SMoveStuff * pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFlyDashSpeed(m_pActor);

	RwBool bFinish = UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);

	if (!bFinish)
	{
		Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

		if (vPos.y <= m_sHStuff.fFinialHeight) //check if we land on ground
		{
			vPos.y = m_sHStuff.fFinialHeight;

		//	if (m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				SetFalling();
				bFinish = TRUE;
			}
		}
	}

	m_pActor->SetPosition(&vPos);

	return bFinish;
}

RwBool CNtlBehaviorCharAirMove::UpdateAirAccel(SMoveStuff * pMoveStuff, RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = Logic_GetFlyAccelSpeed(m_pActor);

	RwBool bFinish = UpdatePositionMove(pMoveStuff, vPos, TRUE, fElapsed, fSpeed);

	if (!bFinish)
	{
		Logic_GetWorldHeight(m_pActor, &vPos, m_sHStuff);

		if (vPos.y <= m_sHStuff.fFinialHeight) //check if we land on ground
		{
			vPos.y = m_sHStuff.fFinialHeight;

		//	if (m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				SetFalling();
				bFinish = TRUE;
			}
		}
	}

	m_pActor->SetPosition(&vPos);

	return bFinish;
}

RwBool CNtlBehaviorCharAirMove::UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime)
{
	if (m_pActor->GetClassID() != SLCLASS_PLAYER)
		return FALSE;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SMoveSyncStuff* pMoveSyncStuff = pBeData->GetMoveSyncStuff();

	if (pMoveSyncStuff->m_pMoveSyncCurr == NULL)
	{
		if (pMoveSyncStuff->Next() == NULL)         // I have not received the packet yet.      
			return FALSE;
	}
	else
	{
		if (!pMoveSyncStuff->m_MoveSyncQ.empty())        // Receive and process the next packet.
		{
			pMoveSyncStuff->Next();
		}
	}

	// After calculating the position that should be in accordance with the original MoveSync, determine the force to go to that position.    
	RwV3d vSyncDir = pMoveSyncStuff->m_pMoveSyncCurr->vLoc - vPos;
	/*vSyncDir.y = 0.0f;*/
	RwReal vSyncDistance = RwV3dLength(&vSyncDir);
	RwV3dNormalize(&vSyncDir, &vSyncDir);
	vPos += vSyncDir * (vSyncDistance / MOVE_SYNC_SPEED) * fElapsedTime * 2.0f;

	// Final position
	m_pActor->SetPosition(&vPos);

	return TRUE;
}

void CNtlBehaviorCharAirMove::SetFalling()
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->byMoveResult |= NTL_MOVE_RESULT_FALLING;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sFalling.vFallingDir = m_pActor->GetDirection();

	pCtrlStuff->uExtra.sFalling.byMoveDirection = m_MoveStuff.byMoveFlags;
	pCtrlStuff->uExtra.sFalling.fSpeed = DBO_FALLING_SPEED;
	pCtrlStuff->uExtra.sFalling.fSearchHeight = m_sHStuff.fFinialHeight;
}

