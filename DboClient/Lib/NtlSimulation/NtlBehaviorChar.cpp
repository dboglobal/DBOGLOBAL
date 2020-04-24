#include "precomp_ntlsimulation.h"
#include "NtlBehaviorChar.h"

// shared 
#include "NtlMovement.h"
#include "NtlBattle.h"
#include "SkillTable.h"
#include "NtlCharacter.h"
#include "DboNavi.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"


// core
#include "NtlMath.h"

// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"
#include "NtlAnimEventData.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLHelpers.h"
#include "NtlPLEntityBlend.h"


// simulation
#include "NtlSLGlobal.h"
#include "NtlSobActor.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobStatusAnimSyncManager.h"
#include "NtlFSMDef.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLuaGlue.h"
#include "NtlSobProjectile.h"
#include "NtlSobCharProxy.h"
#include "TableContainer.h"
#include "NtlFSMCharActEventProc.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSobSyncStatus.h"
#include "NtlSLPacketGenerator.h"
#include "NtlCameraController.h"
#include "NtlSLApi.h"
#include "NtlSobElapsedController.h"
#include "NtlFSMStateBase.h"
#include "NtlSobPlayer.h"
#include "NtlFSMCharActAgent.h"
#include "NtlFSMLayer.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// character behavior macros

#define SPAWN_DELAY_TIME				2.0f
#define SPAWN_DELAY_SPECIAL_TIME		0.2f

// push data 속성.
#define MAX_PUSH_TIME					0.3f

// sliding data 속성.
#define MAX_SLIDING_TIME				0.667f
#define MAX_SLIDING_DIST				4.0f

// hit shock data 속성.
#define MAX_SHOCK_TIME					0.1f
#define MAX_SHOCK_DIST					0.2f

// knock donw data 속성.
#define MAX_KNOCKDOWN_SLIDING_TIME		0.7f
#define MAX_KNOCKDOWN_FALLDOWN_TIME		1.1f

// smooth change heading
#define SMOOTH_TURN_LIMIT_ANGLE			60


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlBehaviorCharSpawn)


CNtlBehaviorCharSpawn::CNtlBehaviorCharSpawn()
{
	SetBehaviorId(NTL_BEID_CHAR_SPAWN);
	SetBehaviorName(NTL_BENAME_CHAR_SPAWN);

	m_byType			= SPAWN_NORMAL;
	m_bSpawnAnimExist	= FALSE;
	m_fTime				= 0.0f;
	m_fSpawnDelayTime	= SPAWN_DELAY_TIME;
	m_bEnterShow		= FALSE;

	m_pVSEAlpha			= NULL;
}

CNtlBehaviorCharSpawn::~CNtlBehaviorCharSpawn()
{
}


void CNtlBehaviorCharSpawn::EnterNormalSpawn(void)
{
	SetNormalSpawnAnim(SPAWN_DELAY_TIME);
	SetNormalSpawnVisualEffect();
}

void CNtlBehaviorCharSpawn::EnterSpecialSpawn(void)
{
	SetNormalSpawnAnim(SPAWN_DELAY_SPECIAL_TIME);
	SetSpecialSpawnVisualEffect();
}

void CNtlBehaviorCharSpawn::EnterTMQInSpawn(void)
{
	m_pActor->EnableVisible(FALSE);
}

void CNtlBehaviorCharSpawn::Enter(void)
{
	SetSpawnType();

	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

	if (m_pActor->IsAirMode() == FALSE)
	{
		vPos.y = sHStuff.fFinialHeight;
	}

	m_pActor->SetPosition(&vPos);

	if(m_byType == SPAWN_NORMAL)
	{
		EnterNormalSpawn();
	}
	else if(m_byType == SPAWN_TMQ_IN)
	{
		EnterTMQInSpawn();
	}
	else
	{
		EnterSpecialSpawn();
	}
	
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharSpawn::ExitNormalSpawn(void)
{
	m_pActor->EnableVisible(TRUE);

	CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
	pSobCharProxy->SetAlpha(255);
	pSobCharProxy->SetInkThickness(DEFAULT_INK_THICKNESS);

	if(m_pActor->GetClassID() == SLCLASS_PET)
		pSobCharProxy->SetScale(1.0f);
}

void CNtlBehaviorCharSpawn::ExitTMQInSpawn(void)
{
	m_pActor->EnableVisible(FALSE);

	CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
	pSobCharProxy->SetAlpha(255);
	pSobCharProxy->SetInkThickness(DEFAULT_INK_THICKNESS);

	if(m_pActor->GetClassID() == SLCLASS_PET)
		pSobCharProxy->SetScale(1.0f);
}

void CNtlBehaviorCharSpawn::Exit(void)
{
	if(m_byType == SPAWN_NORMAL)
		ExitNormalSpawn();
	else if(m_byType == SPAWN_TMQ_IN)
		ExitTMQInSpawn();
	else
		ExitNormalSpawn();
}

void CNtlBehaviorCharSpawn::UpdateNormalSpawn(RwReal fElapsed)
{
	CNtlSobCharProxy *pSobCharProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );

	if(!m_bEnterShow)
	{
		m_pActor->EnableVisible(TRUE);
		m_bEnterShow = TRUE;
	}

	if(m_bSpawnAnimExist)
		return;

	if(!m_pActor->GetSobProxy()->IsExitSystemEffect(m_pVSEAlpha))
	{
		Finish();
		return;
	}

	// 현재 pet 이면... 
	if(m_pActor->GetClassID() == SLCLASS_PET)
	{
		m_fTime += fElapsed;
		RwReal fDeltaTime = m_fTime / m_fSpawnDelayTime;

		RwReal fScale = 0.01f + fDeltaTime;
		if(fScale > 1.0f)
			fScale = 1.0f;

		pSobCharProxy->SetScale(fScale);
	}
}

void CNtlBehaviorCharSpawn::UpdateTMQSpawn(RwReal fElapsed)
{
	Finish();
}

void CNtlBehaviorCharSpawn::Update(RwReal fElapsed)
{
	if(m_byType == SPAWN_NORMAL)
		UpdateNormalSpawn(fElapsed);
	else if(m_byType == SPAWN_TMQ_IN)
		UpdateTMQSpawn(fElapsed);
	else
		UpdateNormalSpawn(fElapsed);
}

void CNtlBehaviorCharSpawn::SetSpawnType(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	RwUInt8 byTeleportType = pCtrlStuff->uExtra.sTeleport.byTeleportType;

	if(byTeleportType == TELEPORT_TYPE_TMQ_WORLD_MOVE)
	{
		m_byType = SPAWN_TMQ_IN;
	}
	else if(byTeleportType == TELEPORT_TYPE_DEFAULT || byTeleportType == TELEPORT_TYPE_GAME_IN || 
			byTeleportType == TELEPORT_TYPE_GAME_OUT)
	{
		m_byType = SPAWN_NORMAL;
	}
	else
	{
		m_byType = SPAWN_SPECIAL;
	}
}

void CNtlBehaviorCharSpawn::SetNormalSpawnAnim(RwReal fSpawnTime)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	RwUInt32 uiCurrAnimKey = pSobProxy->GetBaseAnimationKey();
	RwUInt32 uiNextAnimKey = 0;

	if(m_pActor->GetClassID() == SLCLASS_PET)
	{
		pSobProxy->SetBaseAnimation(NML_SPAWN, FALSE);

		m_fSpawnDelayTime = 1.0f;

		m_pVSEAlpha	= m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 1.0f, 1.0f, 1.0f, FALSE);
	}
	else
	{
		CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
		m_bSpawnAnimExist = pPLCharacter->IsExistAnim(NML_SPAWN);
		
		if(m_bSpawnAnimExist)
		{
			pSobProxy->SetBaseAnimation(NML_SPAWN, FALSE);
		}
		else
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
				uiNextAnimKey = NML_STAFF_IDLE_LOOP;
			else
				uiNextAnimKey = NML_IDLE_LOOP;

			if(uiCurrAnimKey != uiNextAnimKey)
				pSobProxy->SetBaseAnimation(uiNextAnimKey);
		}

		m_pVSEAlpha	= m_pActor->GetSobProxy()->AddVisualSystemEffectAlphaBlend(0.0f, 1.0f, fSpawnTime, fSpawnTime, FALSE);
	}
}

void CNtlBehaviorCharSpawn::SetNormalSpawnVisualEffect(void)
{
	if(m_pActor->GetClassID() != SLCLASS_PET && !m_bSpawnAnimExist)
	{
        // GM 이면 이펙트를 표시하지 않는다.
        if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
            if(pAgent->IsTransparent())
                return;
        }
        
        m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_WORLD_IN, NULL, TRUE);
	}
}

void CNtlBehaviorCharSpawn::SetSpecialSpawnVisualEffect(void)
{
	if(m_pActor->GetClassID() != SLCLASS_PET && !m_bSpawnAnimExist)
	{
        // GM 이면 이펙트를 표시하지 않는다.
        if(m_pActor->GetClassID() == SLCLASS_PLAYER)
        {
            CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)m_pActor->GetFSMLayer()->GetFSMAgent();
            if(pAgent->IsTransparent())
                return;
        }

        m_pActor->GetSobProxy()->CreatePLEffect(NTL_VID_TELEPORT_SPAWN, NULL, TRUE);
	}
}

RwUInt32 CNtlBehaviorCharSpawn::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharSpawn::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		if(m_bSpawnAnimExist)
			Finish();
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorSmoothHeading)

CNtlBehaviorSmoothHeading::CNtlBehaviorSmoothHeading()
{
	SetBehaviorId(NTL_BEID_CHAR_SMOOTHHEADING);
	SetBehaviorName(NTL_BENAME_CHAR_SMOOTHHEADING);

	m_fDeltaAngleY = 0.0f;
	m_fDeltaAngleX = 0.0f;
	m_bSetAngleX = false;
}

CNtlBehaviorSmoothHeading::~CNtlBehaviorSmoothHeading()
{
}

void CNtlBehaviorSmoothHeading::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	m_bSetAngleX = pCtrlStuff->uExtra.sRotate.bSetAngleX;

	SetAnim(pCtrlStuff->uExtra.sRotate.fDeltaAngleY, pCtrlStuff->uExtra.sRotate.fDeltaAngleX);
	
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorSmoothHeading::Exit(void)
{
	// 나중에 base class exit를 호출한다.
	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorSmoothHeading::Update(RwReal fElapsed)
{
	bool bFinish = false;

	// 120 degrees per second.
	RwReal fDeltaAngleY = m_fUpdateAngleY * fElapsed;

	if(m_fDeltaAngleY < 0.0f)
	{
		if(m_fDeltaAngleY + fDeltaAngleY > 0.0f)
		{
			m_pActor->SetDeltaAngleY(m_fDeltaAngleY);
			bFinish = true;
		}
		else
		{
			m_pActor->SetDeltaAngleY(-fDeltaAngleY);
			m_fDeltaAngleY += fDeltaAngleY;
		}
	}
	else
	{
		if(m_fDeltaAngleY - fDeltaAngleY < 0.0f)
		{
			m_pActor->SetDeltaAngleY(m_fDeltaAngleY);
			bFinish = true;
		}
		else
		{
			m_pActor->SetDeltaAngleY(fDeltaAngleY);
			m_fDeltaAngleY -= fDeltaAngleY;
		}
	}

	// angle X (only set while flying)
	if (m_bSetAngleX)
	{
		RwReal fDeltaAngleX = m_fUpdateAngleX * fElapsed;

		if (m_fDeltaAngleX < 0.0f)
		{
			if (m_fDeltaAngleX + fDeltaAngleX > 0.0f)
			{
				m_pActor->SetDeltaAngleX(m_fDeltaAngleX);
				bFinish = true;
			}
			else
			{
				m_pActor->SetDeltaAngleX(-fDeltaAngleX);
				m_fDeltaAngleX += fDeltaAngleX;
			}
		}
		else
		{
			if (m_fDeltaAngleX - fDeltaAngleY < 0.0f)
			{
				m_pActor->SetDeltaAngleX(m_fDeltaAngleX);
				bFinish = true;
			}
			else
			{
				m_pActor->SetDeltaAngleX(fDeltaAngleX);
				m_fDeltaAngleX -= fDeltaAngleX;
			}
		}
	}

	if (bFinish)
		Finish();
}

void CNtlBehaviorSmoothHeading::UpdateData(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	m_bSetAngleX = pCtrlStuff->uExtra.sRotate.bSetAngleX;

	SetAnim(pCtrlStuff->uExtra.sRotate.fDeltaAngleY, pCtrlStuff->uExtra.sRotate.fDeltaAngleX);
}

void CNtlBehaviorSmoothHeading::SetAnim(RwReal fDeltaAngleY, RwReal fDeltaAngleX)
{
	CNtlSobProxy *pSobProxy = m_pActor->GetSobProxy();
	RwUInt32 uiCurrKey = pSobProxy->GetBaseAnimationKey();

	m_fDeltaAngleY += fDeltaAngleY;

	RwReal fAbsDeltaAngleY = (RwReal)fabs(m_fDeltaAngleY);
		
	if(m_fDeltaAngleY < 0.0f)
	{
		if(m_fDeltaAngleY < -SMOOTH_TURN_LIMIT_ANGLE)
		{
			m_pActor->SetDeltaAngleY(m_fDeltaAngleY + SMOOTH_TURN_LIMIT_ANGLE);
			m_fDeltaAngleY = -SMOOTH_TURN_LIMIT_ANGLE;
		}
		
		if(fAbsDeltaAngleY > 3.0f)
		{
			if(uiCurrKey != TURN_LEFT)
				pSobProxy->SetBaseAnimation(TURN_LEFT, TRUE);
		}
	}
	else
	{
		if(m_fDeltaAngleY > SMOOTH_TURN_LIMIT_ANGLE)
		{
			m_pActor->SetDeltaAngleY(m_fDeltaAngleY - SMOOTH_TURN_LIMIT_ANGLE);
			m_fDeltaAngleY = SMOOTH_TURN_LIMIT_ANGLE;
		}

		if(fAbsDeltaAngleY > 3.0f)
		{
			if(uiCurrKey != TURN_RIGHT)
				pSobProxy->SetBaseAnimation(TURN_RIGHT, TRUE);
		}
	}

	m_fUpdateAngleY = (RwReal)fabs(120*m_fDeltaAngleY/SMOOTH_TURN_LIMIT_ANGLE);
	if(m_fUpdateAngleY < 20.0f)
		m_fUpdateAngleY = 20.0f;


	// ANGLE X (up/down)
	if (m_bSetAngleX) // only set while flying
	{
		m_fDeltaAngleX += fDeltaAngleX;

		RwReal fAbsDeltaAngleX = (RwReal)fabs(m_fDeltaAngleX);

		if (m_fDeltaAngleX < 0.0f)
		{
			if (m_fDeltaAngleX < -SMOOTH_TURN_LIMIT_ANGLE)
			{
				m_pActor->SetDeltaAngleX(m_fDeltaAngleX + SMOOTH_TURN_LIMIT_ANGLE);
				m_fDeltaAngleX = -SMOOTH_TURN_LIMIT_ANGLE;
			}
		}
		else
		{
			if (m_fDeltaAngleX > SMOOTH_TURN_LIMIT_ANGLE)
			{
				m_pActor->SetDeltaAngleX(m_fDeltaAngleX - SMOOTH_TURN_LIMIT_ANGLE);
				m_fDeltaAngleX = SMOOTH_TURN_LIMIT_ANGLE;
			}
		}

		m_fUpdateAngleX = (RwReal)fabs(120 * m_fDeltaAngleX / SMOOTH_TURN_LIMIT_ANGLE);
		if (m_fUpdateAngleX < 20.0f)
			m_fUpdateAngleX = 20.0f;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharNormalAttack)


CNtlBehaviorCharNormalAttack::CNtlBehaviorCharNormalAttack()
{
	SetBehaviorId(NTL_BEID_CHAR_NORMALATTACK);
	SetBehaviorName(NTL_BENAME_CHAR_NORMALATTACK);

	m_iHitIdx		= 0;
	m_iHitNum		= 0;
	m_fAnimSpeed	= 1.0f;
	m_iHitGroupNum = 0;
}

CNtlBehaviorCharNormalAttack::~CNtlBehaviorCharNormalAttack()
{
	Delete();
}

void CNtlBehaviorCharNormalAttack::Enter(void)
{
	Init();

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	CNtlAttackData *pAttData = pBeData->GetAttackData();
	SAttackStuff *pAttackStuff = NULL;

    if(pAttData->GetAttackStuffCount() > 0)
        pAttackStuff = pAttData->GetAttackStuff(0);

	if(pAttackStuff)
	{
		CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pAttackStuff->hDefenderSerialId);
		if(pTarSobObj == NULL)
		{
			Finish();
			return;
		}

		SetTransform(pAttackStuff);
		SetAnim(pAttackStuff);

		m_iHitGroupNum = pAttData->GetAttackStuffCount();

		for(RwInt32 i = 0; i < m_iHitGroupNum; ++i)
		{
			pAttackStuff = pAttData->GetAttackStuff(i);
			CalcHitStuff(pAttackStuff, (RwInt8)i);
		}
	}
	else
	{
		SetAnimNoAttackStuff();
	}
	
	
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharNormalAttack::Exit(void)
{
	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData());
	if (pBeData)
	{
		CNtlAttackData* pAttData = pBeData->GetAttackData();
		if (pAttData)
		{
			SAttackStuff* pAttackStuff = NULL;

			if (pAttData->GetAttackStuffCount() > 0)
				pAttackStuff = pAttData->GetAttackStuff(0);

			if (pAttackStuff)
			{
				CNtlSLEventGenerator::SobBattleNotification(pAttackStuff->hAttackerSerialId);
			}

			m_pActor->GetSobProxy()->SetAnimSpeed(1.0f);

			for (int i = m_iHitIdx; i < m_iHitNum; ++i)
				EventProcHit(TRUE);
		}
	}

	Delete();
	
	CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharNormalAttack::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	if(m_pPushFollow)
	{
		m_pPushFollow->Update(fElapsed);
		if(m_pPushFollow->IsFinish())
		{
			DeletePushFollow();
		}
	}
}

RwUInt32 CNtlBehaviorCharNormalAttack::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFSMCharActStateBase::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		DeletePushFollow();
		Finish();
	}

	else if(pMsg.Id == g_EventAnimHit)
	{
		EventProcHit(FALSE);
	}

	else if(pMsg.Id == g_EventSobSkillAffectingCancel)
	{
		EventProcSkillAffectingCancel();
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharNormalAttack::UpdateData(void)
{
}

void CNtlBehaviorCharNormalAttack::Init(void)
{
	m_pPushFollow = NULL;
}

void CNtlBehaviorCharNormalAttack::Delete(void)
{
	for(RwInt32 j = 0; j < m_iHitGroupNum; ++j)
	{
		for(RwInt32 i = 0; i < m_iHitNum; ++i)
		{
			NTL_DELETE( m_ppHit[j][i] );
		}
	}

	m_iHitGroupNum = 0;
}

void CNtlBehaviorCharNormalAttack::SetTransform(const SAttackStuff *pAttackStuff)
{
	if(pAttackStuff == NULL)
		return;

	RwV3dAssignMacro(&m_vPushDir, &pAttackStuff->vPushDir);

	if(m_pActor->GetSerialID() == pAttackStuff->hDefenderSerialId)
		return;

	CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pAttackStuff->hDefenderSerialId);
	RwV3d vAttPos = m_pActor->GetPosition();
	RwV3d vTarPos = pTarSobObj->GetPosition();

	RwV3d vAttDir;
	RwV3dSubMacro(&vAttDir, &vTarPos, &vAttPos);
	vAttDir.y = 0.0f; 
	RwV3dNormalize(&vAttDir, &vAttDir);
	m_pActor->SetDirection(&vAttDir);
}

void CNtlBehaviorCharNormalAttack::SetAnim(const SAttackStuff *pAttackStuff)
{
	RwUInt32 uiAnimId;
	if(pAttackStuff->uiType.bitChainAttack)
	{
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
        if(Logic_IsTransformGreatNamek(m_pActor))  // 변신
        {
            uiAnimId = TRANS_ATK_DEF_1 + NtlRandomNumber(0, 1);
        }
		else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
        {
			uiAnimId = ATK_STAFF_1 + pAttackStuff->byAttSeq - NTL_BATTLE_CHAIN_ATTACK_START;
        }
		else
		{
			RwUInt8 byClass = Logic_GetPlayerClass(m_pActor);            
			if(byClass == PC_CLASS_STREET_FIGHTER)
			{
				uiAnimId = ATK_GLOVE_1 + pAttackStuff->byAttSeq - NTL_BATTLE_CHAIN_ATTACK_START;
			}
			else
				uiAnimId = ATK_DEF_1 + pAttackStuff->byAttSeq - NTL_BATTLE_CHAIN_ATTACK_START;
		}

		// 공격 animation 속도를 setting 한다.
		m_pActor->GetSobProxy()->SetAnimSpeed(Logic_GetAttackAnimSpeed(m_pActor));
	}
	else
	{
		m_fAnimSpeed = 1.0f;

		if(pAttackStuff->uiType.bitLogic == NTL_ATTACK_LOGIC_NORMAL)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
            if(Logic_IsTransformGreatNamek(m_pActor))  // 변신
            {
                uiAnimId = TRANS_ATK_DEF_1 + NtlRandomNumber(0, 1);
            }
			else if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
				uiAnimId = ATK_STAFF_1 + pAttackStuff->byAttSeq;
			else
				uiAnimId = ATK_DEF_1 + pAttackStuff->byAttSeq;

			if(pAttackStuff->bAnimSpeedApply)
				m_fAnimSpeed = Logic_GetAttackAnimSpeed(m_pActor);

			m_pActor->GetSobProxy()->SetAnimSpeed(m_fAnimSpeed);
		}
		else // NTL_ATTACK_LOGIC_SKILL
		{
			CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pAttackStuff->uiAttrLogicTblId) );
            uiAnimId = pSkillTblData->wAction_Animation_Index;

			if (pAttackStuff->bAnimSpeedApply)
			{
				float fSpeed = (float)pSkillTblData->dwAnimation_Time / 1000.0f;

				m_fAnimSpeed = Logic_GetSkillAnimSpeed(m_pActor, pSkillTblData);
				m_fAnimSpeed /= fSpeed;
			}

			m_pActor->GetSobProxy()->SetAnimSpeed(m_fAnimSpeed);
		}
	}

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(uiAnimId, FALSE);

	// create attack sync status
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );

	SAttackStuff * pAttStuff;
	CNtlSob *pTarSobObj;
	RwReal fDurationAnimTime = pPLCharacter->GetDurationAnimTime(uiAnimId);
//	DBO_WARNING_MESSAGE("fDurationAnimTime: " << fDurationAnimTime << " m_fAnimSpeed: " << m_fAnimSpeed);
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	CNtlAttackData *pAttData = pBeData->GetAttackData();
	RwInt32 iHitGroupNum = pAttData->GetAttackStuffCount();

	for(RwInt32 i = 0; i < iHitGroupNum; ++i)
	{
		pAttStuff = pAttData->GetAttackStuff(i);
		pTarSobObj = GetNtlSobManager()->GetSobObject(pAttStuff->hDefenderSerialId);

		if(pTarSobObj)
		{
			CNtlSobAttackSyncStatus *pAttSyncStatus = NTL_NEW CNtlSobAttackSyncStatus();
			pAttSyncStatus->ActiveTime(fDurationAnimTime);                        
			pTarSobObj->AddSob(pAttSyncStatus);
		}
	}
}

void CNtlBehaviorCharNormalAttack::SetAnimNoAttackStuff(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	CNtlAttackData *pAttData = pBeData->GetAttackData();

	if(pAttData->GetAttackStuffLogic() == NTL_ATTACK_LOGIC_SKILL)
	{
		SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
		
		CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pSkillStuff->uiSkillTblId) );

		// Set the skill animation speed.
		float fSpeed = (float)pSkillTblData->dwAnimation_Time / 1000.0f;
		m_fAnimSpeed = Logic_GetSkillAnimSpeed(m_pActor, pSkillTblData);
		m_fAnimSpeed /= fSpeed;

		m_pActor->GetSobProxy()->SetAnimSpeed(m_fAnimSpeed);

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(pSkillTblData->wAction_Animation_Index, FALSE);
	}
}

RwUInt8 CNtlBehaviorCharNormalAttack::CalcPowerHitNum(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
		
	SAnimPlayInfo *pAnimPlayInfo = pPLCharacter->GetBaseAnimPlayInfo();
	RwInt32 iHitNum = pAnimPlayInfo->vecAnimHit.size();

	SEventAnimHit *pAnimHit;

	RwUInt8 byPowerHit = 0;

	for(RwInt32 i = 0; i < iHitNum; ++i)
	{
		pAnimHit = pAnimPlayInfo->vecAnimHit[i];
		if(pAnimHit->bPowerEffect)
			byPowerHit++;
	}

	return byPowerHit;
}

SAttackDamageStuff CNtlBehaviorCharNormalAttack::CalcDamage(SAttackDamageStuff sTotDamageStuff, RwUInt8 byTotHitNum, RwUInt8 byPowerHitNum, RwBool bPowerHit)
{
	SAttackDamageStuff sDamageStuff;
    ZeroMemory(&sDamageStuff, sizeof(SAttackDamageStuff));

	RwUInt8 byCalcTotHitNum = byPowerHitNum + byTotHitNum;

	RwInt32 arrDamage[3] = { sTotDamageStuff.iDamage, sTotDamageStuff.iAbsorbedDamage, sTotDamageStuff.iReflectedDamage };

	for(RwInt32 i = 0; i < 3; i++)
	{
		if(arrDamage[i] > 0)
		{
			RwInt32 iDivDamage = arrDamage[i]/byCalcTotHitNum;
			if(iDivDamage == 0)
			{
				if(rand()%2)
					iDivDamage = 1;
				else
					iDivDamage = 0;

				arrDamage[i] = iDivDamage;
			}
			else
			{
				if(bPowerHit)
					iDivDamage += iDivDamage;

				RwUInt32 uiWeightValue = 3 + iDivDamage/10;
				RwUInt8 byRandValue = (RwUInt8)(rand() % uiWeightValue);

				arrDamage[i] = NtlRandomNumber((RwInt32)(iDivDamage - byRandValue/2), (RwInt32)(iDivDamage + byRandValue/2)); 
			}
		}

		if(arrDamage[i] < 0)
			arrDamage[i] = 0;
	}

	sDamageStuff.iDamage			= arrDamage[0];
	sDamageStuff.iAbsorbedDamage	= arrDamage[1];
	sDamageStuff.iReflectedDamage	= arrDamage[2];

	return sDamageStuff;
}

void CNtlBehaviorCharNormalAttack::CalcHitStuff(const SAttackStuff *pAttackStuff, RwInt8 byHitGroupIdx)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>( m_pActor->GetSobProxy() );
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );

	m_iHitNum = 0;
	m_iHitIdx = 0;
		
	SAnimPlayInfo *pAnimPlayInfo = pPLCharacter->GetBaseAnimPlayInfo();

	if(pAnimPlayInfo == NULL)
		return;
	
	// 현재 animation 시간을 구한다.
	m_fAnimTime = pPLCharacter->GetBaseDurationAnimTime();
	m_iHitNum = pAnimPlayInfo->vecAnimHit.size();

	SAttackDamageStuff sTotDamageStuff		= pAttackStuff->sDamage;
	SAttackDamageStuff sRemainDamageStuff	= pAttackStuff->sDamage;

	RwUInt8 byPowerHit = CalcPowerHitNum();
	SHitStuff *pTempHit;

	SEventAnimHit *pAnimHit;

	for(RwInt32 i = 0; i < m_iHitNum; ++i)
	{
		pAnimHit = pAnimPlayInfo->vecAnimHit[i];

		m_fHitTime[i] = pAnimHit->fTime;
		pTempHit = NTL_NEW SHitStuff;
		m_ppHit[byHitGroupIdx][i] = pTempHit;
		pTempHit->Reset();

		//-------------------------------------------------------
		// hit 속성 data

		pTempHit->hAttackerSerialId			= pAttackStuff->hAttackerSerialId;
		pTempHit->hDefenderSerialId			= pAttackStuff->hDefenderSerialId;
		pTempHit->uiAttrLogicTblId			= pAttackStuff->uiAttrLogicTblId;
		pTempHit->uiProjectileBehaviorId	= pAnimHit->uiProjectileEffectType;
		pTempHit->byShotType				= (RwUInt8)pAnimHit->eProjectileShotType;
		pTempHit->bySubWeaponShotFlag		= (RwUInt8)pAnimHit->nSubWeaponFlag;
		pTempHit->fSpeed					= pAnimHit->fProjectileSpeed;
		if(pAnimHit->eHandType == HAND_TYPE_LEFT)
			pTempHit->bLeftHand = TRUE;
		else
			pTempHit->bLeftHand = FALSE;

		if(pAnimHit->eProjectileShotType == BONE_WEAPON)
			strcpy_s(pTempHit->chBoneName, ITEM_WEAPON_SHOT_NAME);
		else if(pAnimHit->eProjectileShotType == BONE_SUB_WEAPON)
			strcpy_s(pTempHit->chBoneName, ITEM_SUBWEAPON_SHOT1_NAME);
		else 
			strcpy_s(pTempHit->chBoneName, pAnimHit->chBoneName);

		strcpy_s(pTempHit->chProjEffName, pAnimHit->chProjectileEffectName);

        // It is determined whether the main target or the sub target is an effect.      
        if(byHitGroupIdx != 0 &&  strlen(pAnimHit->chSubTargetEffect) > 0)
        {
            strcpy_s(pTempHit->chTargetEffName, pAnimHit->chSubTargetEffect);
        }
        else
        {
            strcpy_s(pTempHit->chTargetEffName, pAnimHit->chTargetEffectName);
        }
		
		pTempHit->byTargetAttach		= (RwUInt8)pAnimHit->bTargetAttach;
		pTempHit->fTargetLocHeight		= pAnimHit->fTargetHeight;
		pTempHit->byTargetEffectDir		= (RwUInt8)pAnimHit->eTargetEffectType;
		strcpy_s(pTempHit->chWordEffName, NTL_MAX_EFFECT_NAME, pAnimHit->chWordEffect);

		pTempHit->pProjExtData			= &pAnimHit->uEffectTypeExtraData;

		//-------------------------------------------------------
		// damage 계산.
		//	pHitStuff[i].uiDamage = pAnimHit->uiDamage * pAttackStuff->uiDamage / 100;
		if(i < m_iHitNum-1)
		{
			pTempHit->sDamageStuff = CalcDamage(sTotDamageStuff, (RwUInt8)m_iHitNum, byPowerHit, pAnimHit->bPowerEffect);
			sRemainDamageStuff.iDamage			-= pTempHit->sDamageStuff.iDamage;
			sRemainDamageStuff.iAbsorbedDamage	-= pTempHit->sDamageStuff.iAbsorbedDamage;
			sRemainDamageStuff.iReflectedDamage -= pTempHit->sDamageStuff.iReflectedDamage;
		}
		else
		{
			if(sRemainDamageStuff.iDamage < 0)
				sRemainDamageStuff.iDamage = 0;

			pTempHit->sDamageStuff.iDamage = sRemainDamageStuff.iDamage;

			if(sRemainDamageStuff.iAbsorbedDamage < 0)
				sRemainDamageStuff.iAbsorbedDamage = 0;

			pTempHit->sDamageStuff.iAbsorbedDamage = sRemainDamageStuff.iAbsorbedDamage;

			if(sRemainDamageStuff.iReflectedDamage < 0)
				sRemainDamageStuff.iReflectedDamage = 0;

			pTempHit->sDamageStuff.iReflectedDamage = sRemainDamageStuff.iReflectedDamage;

            // LP_EP Steal
            pTempHit->sDamageStuff.LP_EP_STEAL_fLpDamage = pAttackStuff->sDamage.LP_EP_STEAL_fLpDamage;
            pTempHit->sDamageStuff.LP_EP_STEAL_fLpGained = pAttackStuff->sDamage.LP_EP_STEAL_fLpGained;
            pTempHit->sDamageStuff.LP_EP_STEAL_fEpDamage = pAttackStuff->sDamage.LP_EP_STEAL_fEpDamage;
            pTempHit->sDamageStuff.LP_EP_STEAL_fEpGained = pAttackStuff->sDamage.LP_EP_STEAL_fEpGained;
            pTempHit->sDamageStuff.LP_EP_STEAL_fAbsorbedDamage = pAttackStuff->sDamage.LP_EP_STEAL_fAbsorbedDamage;
            pTempHit->sDamageStuff.LP_EP_STEAL_fReflectedDamage = pAttackStuff->sDamage.LP_EP_STEAL_fReflectedDamage;

			pTempHit->sDamageStuff.lpEpRecovered = pAttackStuff->sDamage.lpEpRecovered;
		}

		//-------------------------------------------------------
		// 공격 속성 정의.
		pTempHit->uiAttr.bitResult		= pAttackStuff->uiType.bitResult;
		pTempHit->uiAttr.bitCritical	= pAttackStuff->uiType.bitCritical;
		pTempHit->uiAttr.bitPowerEffect = pAnimHit->bPowerEffect;
		pTempHit->uiAttr.bitLogic		= pAttackStuff->uiType.bitLogic;
		pTempHit->uiAttr.bitChainAttack = pAttackStuff->uiType.bitChainAttack;
		pTempHit->uiAttr.bitHitSeq		= i;
		pTempHit->uiAttr.bitBlocking	= pAttackStuff->uiType.bitBlocking;	
		
		//-------------------------------------------------------
		// physical attack or energy attack
		if(pAnimHit->eAttackType == ATTACK_TYPE_PHYSICAL)
		{
			pTempHit->uiAttr.bitPhysic = NTL_ATTACK_PHYSIC_MELEE;

			// 형석
			if(pAttackStuff->uiType.bitResult == NTL_ATTACK_RESULT_BLOCK)
				strcpy_s(pTempHit->chTargetEffName, NTL_MAX_EFFECT_NAME, NTL_VID_TARGET_BLOCK);
			else
			{
				// 스킬 공격이 아닌 일반 공격 이면...
				if(pAttackStuff->uiType.bitLogic == NTL_ATTACK_LOGIC_NORMAL)
				{
					strcpy_s(pTempHit->chTargetEffName, Logic_GetVisualHitMark(m_pActor, pAnimHit->eAttackType, pTempHit->uiAttr.bitCritical, pTempHit->uiAttr.bitPowerEffect));
				}
			}
		}
		else
		{
            // 에너지 공격은 타겟 이펙트가 없을때만 디폴트 이펙트를 표시한다.
            if(pAttackStuff->uiType.bitLogic == NTL_ATTACK_LOGIC_NORMAL && strlen(pAnimHit->chTargetEffectName) == 0)
            {
                strcpy_s(pTempHit->chTargetEffName, Logic_GetVisualHitMark(m_pActor, pAnimHit->eAttackType, pTempHit->uiAttr.bitCritical, pTempHit->uiAttr.bitPowerEffect));
            }

			pTempHit->uiAttr.bitPhysic = NTL_ATTACK_PHYSIC_PROJECTILE;
		}

		//-------------------------------------------------------
		// target behavior 설정.

		RwUInt8 byAttResult = pAttackStuff->uiType.bitResult;

		if(byAttResult == NTL_ATTACK_RESULT_SUCCESS)
		{
			/*if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_PUSH)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_PUSH;
			else*/ if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_TOSS)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_UP;
			else if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_FALL_DOWN)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_DOWN;
		}
		else if(byAttResult == NTL_ATTACK_RESULT_BLOCK)
		{
			if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_TOSS)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_UP;
			else if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_FALL_DOWN)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_DOWN;
		}
		else if(byAttResult == NTL_ATTACK_RESULT_DODGE)
		{
			if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_TOSS)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_UP;
			else if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_FALL_DOWN)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_TOSS_DOWN;
		}
		else if(byAttResult == NTL_ATTACK_RESULT_KNOCKDOWN)
		{
			/*if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_PUSH)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_PUSH;
			else*/ 
			/*if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_KNOCK_DOWN)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_KNOCKDOWN;*/
		}
		else if(byAttResult == NTL_ATTACK_RESULT_SLIDING)
		{
			/*if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_PUSH)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_PUSH;
			else*/ if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_KNOCK_DOWN)
				pTempHit->uiAttr.bitBehavior = NTL_ATTACK_SLIDING;

			// 하드코딩...(HTB 을 blocking으로 막았을 때)
			if(pAttackStuff->uiType.bitBlocking == DBO_GUARD_TYPE_HTB)
			{
				if(i == m_iHitNum - 1)
				{
					pTempHit->uiAttr.bitBehavior = NTL_ATTACK_SLIDING;
				}
			}
		}

		// If the attack result is not knockdown, the target behavior is knockdown
		// Set the behavior to push if the push flag is set.
		/*if(byAttResult != NTL_ATTACK_RESULT_KNOCKDOWN)
		{
			if(pAnimHit->eTargetBehavior == TARGET_BEHAVIOR_KNOCK_DOWN)
			{
				if(pAnimHit->bKB2Push)
				{
					pTempHit->uiAttr.bitBehavior = NTL_ATTACK_PUSH;
				}
			}
		}*/

		//-------------------------------------------------------
		// push direction 설정.
		RwV3dAssignMacro(&pTempHit->vPushDir, &pAttackStuff->vPushDir);

		//-------------------------------------------------------
		// 카메라 셰이크
		pTempHit->bCameraShake = pAnimHit->bCameraShake;
        pTempHit->fShakeFactor = pAnimHit->fCameraShakeFactor;
        pTempHit->fShakeMaxHeight = pAnimHit->fCameraShakeMaxHeight;


        //-------------------------------------------------------
        // Hit Sound Echo        
        pTempHit->bHitSoundEcho = pAnimHit->bHitSoundEcho;

		//-------------------------------------------------------
		// Sync Event connect.
		if(pAttackStuff->uiSyncEventId != INVALID_SYNCEVENT_ID)
		{
			pTempHit->hSyncEventId = pAttackStuff->uiSyncEventId;
			CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();

			RwInt32 iDamageValue = pTempHit->sDamageStuff.iDamage;
			if(byAttResult == NTL_ATTACK_RESULT_HEALING)
				iDamageValue = -iDamageValue;

			if (pTempHit->sDamageStuff.lpEpRecovered.bIsLpRecoveredWhenHit)
				iDamageValue -= pTempHit->sDamageStuff.lpEpRecovered.targetLpRecoveredWhenHit;

			if (iDamageValue < 0)
				iDamageValue = 0;

		//	DBO_WARNING_MESSAGE("reduce LP by iDamageValue: " << iDamageValue);
		//	DBO_WARNING_MESSAGE("targetLpRecoveredWhenHit: " << pTempHit->sDamageStuff.lpEpRecovered.targetLpRecoveredWhenHit);

			pAnimSyncManager->AddLPAnimSyncUnit(pAttackStuff->hDefenderSerialId, pAttackStuff->uiSyncEventId, iDamageValue);
		}
	}

	//-------------------------------------------------------
	// Sync Event 연결.
	if(pAttackStuff->uiSyncEventId != INVALID_SYNCEVENT_ID)
	{
		CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();
		pAnimSyncManager->SetLPAnimSyncUnitLifeTime(pAttackStuff->hDefenderSerialId, pAttackStuff->uiSyncEventId, m_fAnimTime);
	}
}

void CNtlBehaviorCharNormalAttack::CreatePushFollow(RwBool bOneStepFollow)
{
	DeletePushFollow();

	if(bOneStepFollow)
	{
		RwV3d vPos = m_pActor->GetPosition();
        RwV3d vDestPos = vPos + m_vPushDir * NTL_BATTLE_PUSH_DISTANCE;
        vDestPos.y = vPos.y;
        
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(m_pActor, &vDestPos, sHStuff);
		vDestPos.y = sHStuff.fFinialHeight;

		m_pActor->SetPosition(&vDestPos);
	}
	else
	{
		RwReal fPushSpeed = 1.0f;
		if(m_iHitIdx + 1 >= m_iHitNum)
		{
			RwReal fHitDeltaTime = m_fAnimTime - m_fHitTime[m_iHitIdx];
			if(fHitDeltaTime > 0.0f)
				fPushSpeed = 1.0f / fHitDeltaTime;
		}
		else
		{
			RwReal fHitDeltaTime = m_fHitTime[m_iHitIdx+1] - m_fHitTime[m_iHitIdx];
			if(fHitDeltaTime > 0.0f)
				fPushSpeed = 1.0f / fHitDeltaTime;
		}

		m_pPushFollow = NTL_NEW CNtlBehaviorCharPushFollow;
		m_pPushFollow->SetActor(m_pActor);
		m_pPushFollow->SetPushSpeed(fPushSpeed*2.0f);
		m_pPushFollow->SetPushDir(&m_vPushDir);
		m_pPushFollow->Enter();
	}
}

void CNtlBehaviorCharNormalAttack::DeletePushFollow(void)
{
	if(m_pPushFollow)
	{
		m_pPushFollow->Exit();

		NTL_DELETE( m_pPushFollow );
	}
}

void CNtlBehaviorCharNormalAttack::EventProcPhysicHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx)
{
	// 카메라 흔들기.
	if(pHitStuff->uiAttr.bitResult != NTL_ATTACK_RESULT_DODGE)
	{
		if(pHitStuff->bCameraShake && m_pActor->GetFlags() & SLFLAG_CAMERASHAKE_ENABLE)
        {
            CNtlCameraShakeController::SetShakeFactor(pHitStuff->fShakeFactor, pHitStuff->fShakeMaxHeight);
			CNtlSLEventGenerator::CameraShake(m_pActor->GetSerialID(), TRUE);
        }
	}

	LuaExec_Attack(pHitStuff->hAttackerSerialId, pHitStuff->hDefenderSerialId, pHitStuff);

	// by daneos: Disabled the knockdown cinematic

	// matrix 연출.
	//RwBool bMatrixDirect = FALSE;


	//if(m_pActor->GetClassID() == SLCLASS_AVATAR && pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
	//{
		//bMatrixDirect = Logic_ProcKnockdownMatrixDirection(m_pActor, pHitStuff->hDefenderSerialId, pHitStuff);
	//}

	SERIAL_HANDLE hAvatarSerialId = Logic_GetAvatarHandle();

	for(RwInt32 i = 0; i < m_iHitGroupNum; ++i)
	{
		pHitStuff = m_ppHit[i][byHitIdx];
		CNtlSLEventGenerator::SobHit(pHitStuff->hDefenderSerialId, pHitStuff);

		// avatar가 공격했을 경우.
	//	if(bMatrixDirect)
	//	{
	//		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hDefenderSerialId);
	//		if(pSobObj)
	//		{
	//			GetNtlSobElapsedControlManager()->CreateController(pSobObj, g_fMatrixTime, g_fMatrixValue, g_fMatrixDelayTime);
	//		}
	//	}

		// avatar가 맞았을 경우.
		//if(pHitStuff->hDefenderSerialId == hAvatarSerialId && pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
		//{
			// by daneos: Disabled the knockdown animation
			//Logic_ProcKnockdownMatrixDirection(Logic_GetAvatarActor(), hAvatarSerialId, pHitStuff);
		//}
	}
}

void CNtlBehaviorCharNormalAttack::EventProcProjectileHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx)
{
	void *pSobObj = CNtlSLEventGenerator::SobProjectileCreate(SLCLASS_PROJECTILE, m_pActor->GetSerialID(), pHitStuff->hDefenderSerialId, 
															pHitStuff->uiProjectileBehaviorId, pHitStuff->byShotType, 
															pHitStuff->fSpeed, pHitStuff->bLeftHand, pHitStuff->bCreateHitStuffEffect, pHitStuff->chBoneName, 
															pHitStuff->chProjEffName, pHitStuff->fTargetLocHeight, pHitStuff->pProjExtData, m_fAnimSpeed);
	if(pSobObj)
	{
		CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(pSobObj);
		pSobProj->SetTargetID(pHitStuff->hDefenderSerialId);
		for(RwInt32 i = 0; i < m_iHitGroupNum; ++i)
			pSobProj->AddHit(*m_ppHit[i][byHitIdx]);
	}
}

void CNtlBehaviorCharNormalAttack::EventProcHit(RwBool bOneStepFollow)
{
	if(m_iHitNum == 0 || m_iHitIdx >= m_iHitNum)
	{
		DBO_WARNING_MESSAGE("CNtlBehaviorCharNormalAttack::HandleEvents => m_iHitIdx: " << m_iHitIdx << " is range out: " << m_iHitNum);
		return;
	}
		
	SHitStuff *pHitStuff = m_ppHit[0][m_iHitIdx];

	// avater나 avatar의 pet종류가 아니면 ? 
	if( !(m_pActor->GetFlags() & SLFLAG_SERVER_SENDER) )
	{
		if( !Logic_IsCreateHitEffect() )
		{
			pHitStuff->bCreateHitStuffEffect = FALSE;
		}
	}

	// status animation sync 처리.
	if(pHitStuff->hSyncEventId != INVALID_SYNCEVENT_ID)
	{
		CNtlSobStatusAnimSyncManager *pAnimSyncManager = GetNtlSobManager()->GetAnimSyncManager();
		pAnimSyncManager->PopLPAnimSyncUnit(pHitStuff->hDefenderSerialId, pHitStuff->hSyncEventId);
	}

	if(pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_PUSH && pHitStuff->uiAttr.bitPhysic == NTL_ATTACK_PHYSIC_MELEE)
		CreatePushFollow(bOneStepFollow);

	if(pHitStuff->uiAttr.bitPhysic == NTL_ATTACK_PHYSIC_MELEE)
	{
		EventProcPhysicHit(pHitStuff, m_iHitIdx);		
	}
	else if(m_ppHit[0][m_iHitIdx]->uiAttr.bitPhysic == NTL_ATTACK_PHYSIC_PROJECTILE)
	{
		EventProcProjectileHit(pHitStuff, m_iHitIdx);

		if(pHitStuff->byShotType == BONE_SUB_WEAPON)
		{
            // 넉다운 속성인경우, 서브웨폰에서도 발생해버리면 넉다운이 중복되어 시간 계산이 중복된다.
            // 여기서 넉다운 속성을 없애야 한다.
            pHitStuff->uiAttr.bitBehavior = NTL_ATTACK_NORMAL;

			if(pHitStuff->bySubWeaponShotFlag & SUB_WEAPON_PROJECTILE_2)
			{
				strcpy_s(pHitStuff->chBoneName, NTL_MAX_BONE_NAME, ITEM_SUBWEAPON_SHOT2_NAME);
				EventProcProjectileHit(pHitStuff, m_iHitIdx);
			}

			if(pHitStuff->bySubWeaponShotFlag & SUB_WEAPON_PROJECTILE_3)
			{
				strcpy_s(pHitStuff->chBoneName, NTL_MAX_BONE_NAME, ITEM_SUBWEAPON_SHOT3_NAME);
				EventProcProjectileHit(pHitStuff, m_iHitIdx);
			}
		}
	}
	
	m_iHitIdx++;
}

void CNtlBehaviorCharNormalAttack::EventProcSkillAffectingCancel(void)
{
	for(RwInt32 i = m_iHitIdx; i < m_iHitNum; ++i)
	{
		SHitStuff *pHitStuff = m_ppHit[0][i];

		if(pHitStuff->uiAttr.bitLogic == NTL_ATTACK_LOGIC_NORMAL)
			continue;

		if( !(pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN || pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_SLIDING) )
			continue;

		for(RwInt32 j = 0; j < m_iHitGroupNum; ++j)
		{
			m_ppHit[j][i]->uiAttr.bitBehavior = NTL_ATTACK_NORMAL;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

DEFINITION_MEMORY_POOL(CNtlBehaviorCharPushFollow)


CNtlBehaviorCharPushFollow::CNtlBehaviorCharPushFollow()
{
	SetBehaviorId(NTL_BEID_CHAR_PUSHFOLLOW);
	SetBehaviorName(NTL_BENAME_CHAR_PUSHFOLLOW);
}

CNtlBehaviorCharPushFollow::~CNtlBehaviorCharPushFollow()
{
}

void CNtlBehaviorCharPushFollow::Enter(void)
{
	m_fCurrDist = 0.0f;

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharPushFollow::Exit(void)
{
	if(m_fCurrDist < NTL_BATTLE_PUSH_DISTANCE)
	{
		RwV3d vPos = m_pActor->GetPosition();
		RwReal fDist = NTL_BATTLE_PUSH_DISTANCE - m_fCurrDist;
		CNtlMath::MathRwV3dAddMul(&vPos, &m_vPushDir, fDist);

		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
		vPos.y = sHStuff.fFinialHeight;

		m_pActor->SetPosition(&vPos);
		m_fCurrDist = NTL_BATTLE_PUSH_DISTANCE;
	}
}

void CNtlBehaviorCharPushFollow::Update(RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();
	RwReal fDist = m_fPushSpeed * fElapsed * NTL_BATTLE_PUSH_DISTANCE;

	if(m_fCurrDist + fDist >= NTL_BATTLE_PUSH_DISTANCE)
	{
		Finish();
		fDist = NTL_BATTLE_PUSH_DISTANCE - m_fCurrDist;
		m_fCurrDist = NTL_BATTLE_PUSH_DISTANCE;
	}

	m_fCurrDist += fDist;
    RwV3d vDestPos = vPos + m_vPushDir * fDist;

    SWorldHeightStuff sHStuff;
    Logic_GetWorldHeight(m_pActor, &vDestPos, sHStuff);
    vDestPos.y = sHStuff.fFinialHeight;

    // Check collision. There was a bug that caused the push to move to an area that could not move.
    RwBool bCollMoveImpossible = FALSE;
    RwBool bColliResult = Logic_CharacterCollisionEx(m_pActor, &vDestPos, m_fPushSpeed, vDestPos, bCollMoveImpossible, fElapsed);
    if(bColliResult != NTL_CHARACTER_COLLI_NONE && bCollMoveImpossible)
        return;

	m_pActor->SetPosition(&vDestPos);
}

RwUInt32 CNtlBehaviorCharPushFollow::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharPushFollow::HandleEvents");
	
	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharPushFollow::SetPushSpeed(RwReal fPushSpeed)
{
	m_fPushSpeed = fPushSpeed;
}

void CNtlBehaviorCharPushFollow::SetPushDir(const RwV3d *pPushDir)
{
	RwV3dAssignMacro(&m_vPushDir, pPushDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 


DEFINITION_MEMORY_POOL(CNtlBehaviorCharPush)

CNtlBehaviorCharPush::CNtlBehaviorCharPush()
{
	SetBehaviorId(NTL_BEID_CHAR_PUSH);
	SetBehaviorName(NTL_BENAME_CHAR_PUSH);
	m_pSeq = NULL;
}

CNtlBehaviorCharPush::~CNtlBehaviorCharPush()
{
	DeleteTimeSequence();
}

void CNtlBehaviorCharPush::CreateTimeSequence(void)
{
	m_pSeq = NTL_NEW CNtlDistTimeSequence;
	m_pSeq->SetTime(0.1f);
	m_pSeq->SetDistance(0.25f * NTL_BATTLE_PUSH_DISTANCE / 0.3f);
	
	CNtlDistTimeSequence *pSeq = m_pSeq;
	CNtlDistTimeSequence *pTempSeq = NULL;
	
	pTempSeq = NTL_NEW CNtlDistTimeSequence;
	pTempSeq->SetTime(0.2f);
	pTempSeq->SetDistance(0.275f * NTL_BATTLE_PUSH_DISTANCE / 0.3f);
	pSeq->Add(pTempSeq);
	pSeq = pTempSeq;

	pTempSeq = NTL_NEW CNtlDistTimeSequence;
	pTempSeq->SetTime(MAX_PUSH_TIME);
	pTempSeq->SetDistance(NTL_BATTLE_PUSH_DISTANCE);
	pSeq->Add(pTempSeq);
	pSeq = pTempSeq;
}

void CNtlBehaviorCharPush::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}

RwReal CNtlBehaviorCharPush::GetMaxTime(void) const
{
	return MAX_PUSH_TIME;
}

RwReal CNtlBehaviorCharPush::GetMaxDist(void) const
{
	return NTL_BATTLE_PUSH_DISTANCE;
}

void CNtlBehaviorCharPush::Enter(void)
{
    m_fTime = 0.0f;
    m_fPrevDist = 0.0f;
    m_fDistScale = 1.0f;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
    m_vPushDir = pHitStuff->vPushDir;
    
    m_vDestPos = m_pActor->GetPosition() + m_vPushDir * GetMaxDist();
    m_vDestPos.y = 0.0f;

	// time sequence setting
	CreateTimeSequence();
				
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}


void CNtlBehaviorCharPush::Update(RwReal fElapsed)
{
	m_fTime += fElapsed;

	if(m_fTime > GetMaxTime())
	{
		Finish();
		m_fTime = GetMaxTime();
	}
	
    UpdatePosition(fElapsed);
}

void CNtlBehaviorCharPush::Exit(void)
{
	DeleteTimeSequence();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        // 서버에 마지막 위치를 알려준다.
        API_GetSLPacketGenerator()->SendCharLocAfterPushNfy(m_pActor->GetPosition(), m_pActor->GetDirection());
    }
}

RwUInt32 CNtlBehaviorCharPush::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharPush::HandleEvents");

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharPush::UpdateData(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
    m_vPushDir = pHitStuff->vPushDir;	

	RwV3d vNewDestPos = m_vDestPos + m_vPushDir * GetMaxDist();
    vNewDestPos.y = 0.0f;	

	RwV3d vTarPos = m_pActor->GetPosition();
	
	m_vPushDir = vNewDestPos - vTarPos;	    
	m_vPushDir.y = 0.0f;

	RwReal fLen = RwV3dLength(&m_vPushDir);
	m_fDistScale = fLen / GetMaxDist();
	
	RwV3dNormalize(&m_vPushDir, &m_vPushDir); 
    m_vDestPos = vNewDestPos;

	m_fTime = 0.0f;
	m_fPrevDist = 0.0f;
}

void CNtlBehaviorCharPush::UpdatePosition(RwReal fElapsed)
{
    RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
    RwReal fDelta = (fDist - m_fPrevDist) * m_fDistScale;	    
    RwV3d vPos = m_pActor->GetPosition() + m_vPushDir * fDelta;

    // 충돌 체크를 한다
    RwBool bCollMoveImpossible = FALSE;
    Logic_CharacterCollisionEx(m_pActor, &vPos, fDelta / fElapsed, vPos, bCollMoveImpossible, fElapsed);
    if(bCollMoveImpossible) // 어딘가에 충돌했다. 
    {
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            // 서버에 자신이 충돌했다는 패킷을 보낸다.
            API_GetSLPacketGenerator()->SendCharLocAfterPushNfy(vPos, m_pActor->GetDirection());
        }
    }

    if( !(m_pActor->GetCurrentState()->GetFlags() & NTL_FSMSF_NOT_GROUNDMOVE) ||
        ((m_pActor->GetFlags() & SLFLAG_CAN_GROUND_FLY) && (m_pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)))
    {
        SWorldHeightStuff sHStuff;
        Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
        vPos.y = sHStuff.fFinialHeight;
    }

    m_pActor->SetPosition(&vPos);

    m_fPrevDist = fDist;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 


DEFINITION_MEMORY_POOL(CNtlBehaviorCharSliding)

CNtlBehaviorCharSliding::CNtlBehaviorCharSliding()
{
	SetBehaviorId(NTL_BEID_CHAR_SLIDING);
	SetBehaviorName(NTL_BENAME_CHAR_SLIDING);
}

CNtlBehaviorCharSliding::~CNtlBehaviorCharSliding()
{
	DeleteTimeSequence();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        // 종료시에 서버에 자신의 위치를 알려준다.
        API_GetSLPacketGenerator()->SendCharLocAfterSlidingNfy(m_pActor->GetPosition(), m_pActor->GetDirection());
    }
}

void CNtlBehaviorCharSliding::CreateTimeSequence(void)
{

#define SLIDING_SEQ_CNT	10
	
	RwReal arrTime[SLIDING_SEQ_CNT] = { 0.067f, 0.133f, 0.200f, 0.267f,  0.333f, 0.400f, 0.487f, 0.533f, 0.600f, MAX_SLIDING_TIME };
	RwReal arrDist[SLIDING_SEQ_CNT] = { 0.713f, 1.380f, 1.980f, 2.500f,  2.965f, 3.336f, 3.625f, 3.830f, 3.955f, MAX_SLIDING_DIST };

	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for(int i = 0; i < SLIDING_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDist[i]);
		if(i == 0)
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharSliding::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}

RwReal CNtlBehaviorCharSliding::GetMaxTime(void) const
{
	return MAX_SLIDING_TIME;
}

RwReal CNtlBehaviorCharSliding::GetMaxDist(void) const
{
	return MAX_SLIDING_DIST;
}



void CNtlBehaviorCharSliding::UpdateSlidingLanding(RwReal fElapsed)
{
	m_fTime += fElapsed;

	if(m_fTime > GetMaxTime())
	{
		m_fTime = GetMaxTime();
		UpdatePosition(m_fTime);
		ChangeSlidingState(SS_SLIDING_STOP);
		return;
	}
	
	UpdatePosition(fElapsed);
}

void CNtlBehaviorCharSliding::UpdateSlidingStop(RwReal fElapsed)
{
	m_fTime += fElapsed;

	if(m_fTime > 0.5f)
	{
		Finish();
	}
}

void CNtlBehaviorCharSliding::UpdatePosition(RwReal fElapsed)
{
	RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
	RwReal fDelta = (fDist - m_fPrevDist) * m_fDistScale;
	RwV3d vPos = m_pActor->GetPosition() + m_vPushDir * fDelta;

    // 충돌 체크를 한다
    RwBool bCollMoveImpossible = FALSE;
    Logic_CharacterCollisionEx(m_pActor, &vPos, fDelta / fElapsed, vPos, bCollMoveImpossible, fElapsed);
    if(bCollMoveImpossible) // 어딘가에 충돌했다. 
    {
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            // 서버에 자신이 충돌했다는 패킷을 보낸다.
            API_GetSLPacketGenerator()->SendCharLocAfterSlidingNfy(vPos, m_pActor->GetDirection());
        }
    }

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);

	m_fPrevDist = fDist;
}

void CNtlBehaviorCharSliding::ChangeSlidingState(RwUInt32 uiState)
{
	m_fTime = 0.0f;
	m_uiSlidingState = uiState;
}

void CNtlBehaviorCharSliding::Enter(void)
{
	m_fTime = 0.0f;
	m_fPrevDist = 0.0f;
	m_fDistScale = 1.0f;
	ChangeSlidingState(SS_SLIDING_LADING);
	
	RwV3d vPos = m_pActor->GetPosition();
	CNtlMath::MathRwV3dAssign(&m_vDestPos, vPos.x + m_vPushDir.x*GetMaxDist(), 0.0f, vPos.z + m_vPushDir.z*GetMaxDist());

	// time sequence setting
	CreateTimeSequence();

	// animation setting
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(BTL_DEF_BLOCK_DEF, FALSE);
		
	// visual effect
	RwV3d vOffset;
	CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, 0.0f, 0.0f);
	pSobProxy->CreatePLChildEffect(NTL_VID_TARGET_SLIDING, vOffset);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}


void CNtlBehaviorCharSliding::Update(RwReal fElapsed)
{
	if(m_uiSlidingState == SS_SLIDING_LADING)
		UpdateSlidingLanding(fElapsed);
	else
		UpdateSlidingStop(fElapsed);
}

void CNtlBehaviorCharSliding::Exit(void)
{
	DeleteTimeSequence();
}

RwUInt32 CNtlBehaviorCharSliding::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharSliding::HandleEvents");

	if(pMsg.Id == g_EventSobStanding)
	{
		FSMEvent_CharActStanding(m_pActor, reinterpret_cast<SNtlEventSobStanding*>( pMsg.pData ));
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharSliding::UpdateData(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
	
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId);
	RwV3d vAttPos = pSobObj->GetPosition();
	
	RwV3d vNewPushDir;
	RwV3dSubMacro(&vNewPushDir, &m_vDestPos, &vAttPos);
	vNewPushDir.y = 0.0f;
	RwV3dNormalize(&vNewPushDir, &vNewPushDir); 

	RwV3d vNewDestPos;
	CNtlMath::MathRwV3dAssign(&vNewDestPos, m_vDestPos.x + vNewPushDir.x*GetMaxDist(), 0.0f, m_vDestPos.z + vNewPushDir.z*GetMaxDist());

	RwV3d vPos = m_pActor->GetPosition();
	RwV3dSubMacro(&m_vPushDir, &vNewDestPos, &vPos);
	m_vPushDir.y = 0.0f;

	m_fDistScale = RwV3dLength(&m_vPushDir)/GetMaxDist();
	RwV3dNormalize(&m_vPushDir, &m_vPushDir); 

	m_fTime = 0.0f;
	m_fPrevDist = 0.0f;
}

void CNtlBehaviorCharSliding::SetSlidingDir(const RwV3d *pSlidingDir)
{
	RwV3dAssignMacro(&m_vPushDir, pSlidingDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 



DEFINITION_MEMORY_POOL(CNtlBehaviorCharShock)

CNtlBehaviorCharShock::CNtlBehaviorCharShock()
{
	SetBehaviorId(NTL_BEID_CHAR_SHOCK);
	SetBehaviorName(NTL_BENAME_CHAR_SHOCK);

	m_pSeq = NULL;
}

CNtlBehaviorCharShock::~CNtlBehaviorCharShock()
{
	DeleteTimeSequence();
}

void CNtlBehaviorCharShock::CreateTimeSequence(void)
{
	m_pSeq = NTL_NEW CNtlDistTimeSequence;
	m_pSeq->SetTime(0.333333f*MAX_SHOCK_TIME);
	m_pSeq->SetDistance(MAX_SHOCK_DIST);
	
	CNtlDistTimeSequence *pSeq = m_pSeq;
	CNtlDistTimeSequence *pTempSeq = NULL;
	
	pTempSeq = NTL_NEW CNtlDistTimeSequence;
	pTempSeq->SetTime(0.833333f*MAX_SHOCK_TIME);
	pTempSeq->SetDistance(-MAX_SHOCK_DIST);
	pSeq->Add(pTempSeq);
	pSeq = pTempSeq;

	pTempSeq = NTL_NEW CNtlDistTimeSequence;
	pTempSeq->SetTime(MAX_SHOCK_TIME);
	pTempSeq->SetDistance(0.0f);
	pSeq->Add(pTempSeq);
	pSeq = pTempSeq;
}

void CNtlBehaviorCharShock::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}

void CNtlBehaviorCharShock::ResetTimeSequence(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
	
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId);
	if(pSobObj == NULL)
	{
		Finish();
		return;
	}

	RwV3d vAttPos = pSobObj->GetPosition();
	RwV3d vDefPos = m_pActor->GetPosition();

	RwV3dSubMacro(&m_vPushDir, &vDefPos, &vAttPos);
	m_vPushDir.y = 0.0f;
	RwV3dNormalize(&m_vPushDir, &m_vPushDir); 
	
	m_fTime = 0.0f;
	m_fDistScale = 1.0f;

	RwV3dAssignMacro(&m_vDestPos, &vDefPos);
}

void CNtlBehaviorCharShock::UpdateTimeSequence(RwReal fOldTime, RwReal fCurrTime)
{
	if(m_pSeq == NULL)
		return;

	RwReal fDelta = *(reinterpret_cast<RwReal*>(m_pSeq->GetDeltaData(fOldTime, fCurrTime)));
	RwV3d vPos = m_pActor->GetPosition();
	
	vPos.x += m_vPushDir.x*fDelta;
	vPos.z += m_vPushDir.z*fDelta;

	if( !(m_pActor->GetCurrentState()->GetFlags() & NTL_FSMSF_NOT_GROUNDMOVE) || 
        m_pActor->GetCurrentState()->GetStateId() == NTL_FSMSID_FAINTING)
	{
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
		vPos.y = sHStuff.fFinialHeight;
	}

	m_pActor->SetPosition(&vPos);
}

RwReal CNtlBehaviorCharShock::GetMaxTime(void) const
{
	return MAX_SHOCK_TIME;
}

RwReal CNtlBehaviorCharShock::GetMaxDist(void) const
{
	return MAX_SHOCK_DIST;
}

void CNtlBehaviorCharShock::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();

	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId);
	if(pSobObj == NULL)
	{
		Finish();
		return;
	}

	// reset tim sequence
	ResetTimeSequence();
	
	// time sequence setting
	CreateTimeSequence();
				
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}


void CNtlBehaviorCharShock::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	RwReal fOldTime = m_fTime;
	m_fTime += fElapsed;

	if(m_pSeq == NULL || m_fTime > GetMaxTime())
	{
		Finish();
		m_fTime = GetMaxTime();
	}

	UpdateTimeSequence(fOldTime, m_fTime);
}

void CNtlBehaviorCharShock::Exit(void)
{
	DeleteTimeSequence();
}

RwUInt32 CNtlBehaviorCharShock::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharShock::HandleEvents");

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharShock::UpdateData(void)
{
	RwReal fOldTime = m_fTime;
	m_fTime = GetMaxTime();
	UpdateTimeSequence(fOldTime, m_fTime);
	ResetTimeSequence();
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 



DEFINITION_MEMORY_POOL(CNtlBehaviorCharKnockDown)

CNtlBehaviorCharKnockDown::CNtlBehaviorCharKnockDown()
{
	SetBehaviorId(NTL_BEID_CHAR_KNOCKDOWN);
	SetBehaviorName(NTL_BENAME_CHAR_KNOCKDOWN);
	m_pSeq = NULL;
	m_pLandingSeq = NULL;
	CNtlMath::MathRwV2dAssign(&m_vFallDelta, 0.0f, -1.0f);
}

CNtlBehaviorCharKnockDown::~CNtlBehaviorCharKnockDown()
{
	DeleteTimeSequence();    
}

void CNtlBehaviorCharKnockDown::CreateFallupTimeSequence(void)
{
#define KNOCKDOWN_FALLUP_SEQ_CNT	12

	RwReal arrTime[KNOCKDOWN_FALLUP_SEQ_CNT]  = { 0.067f, 0.133f, 0.200f, 0.267f, 0.333f, 0.400f, 0.487f, 0.533f, 0.600f, 0.767f, 1.000f, 1.200f };
	RwReal arrDistX[KNOCKDOWN_FALLUP_SEQ_CNT] = { 0.427f, 0.868f, 1.320f, 1.750f, 2.196f, 2.630f, 3.080f, 3.490f, 3.900f, 4.780f, 5.540f, 5.930f };
	RwReal arrDistY[KNOCKDOWN_FALLUP_SEQ_CNT] = { 0.522f, 0.865f, 1.086f, 1.188f, 1.184f, 1.060f, 0.820f, 0.490f, 0.000f, -1.95f, -5.96f, -10.23f };


	CNtl2DTimeSequence *pSeq = NULL;
	CNtl2DTimeSequence *pTempSeq = NULL;

	for(RwInt32 i = 0; i < KNOCKDOWN_FALLUP_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtl2DTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDistX[i], arrDistY[i]);

		if(i == 0)
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharKnockDown::CreateLandingTimeSequence(void)
{
#define KNOCKDOWN_LANDING_SEQ_CNT	10

	RwReal arrTime[KNOCKDOWN_LANDING_SEQ_CNT] = { 0.067f, 0.133f, 0.200f, 0.267f, 0.333f, 0.400f, 0.487f, 0.533f, 0.600f, 0.700f };
	RwReal arrDist[KNOCKDOWN_LANDING_SEQ_CNT] = { 0.356f, 0.640f, 0.880f, 1.080f, 1.250f, 1.390f, 1.510f, 1.600f, 1.660f, 1.700f };

	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for(int i = 0; i < KNOCKDOWN_LANDING_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDist[i]);
		if(i == 0)
		{
			m_pLandingSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharKnockDown::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	pSeq = m_pLandingSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq			= NULL;
	m_pLandingSeq	= NULL;
}

void CNtlBehaviorCharKnockDown::UpdateFallUp(RwReal fElapsed)
{
	m_fTime += fElapsed;

	if(m_fTime > MAX_KNOCKDOWN_FALLDOWN_TIME)
	{
		ChangeKnockDownState(KD_FALLDOWN);
		UpdateFallDown(fElapsed);
		return;
	}

	RwV3d vPos = m_pActor->GetPosition();
	
	CNtl2DTimeSequence::S2DDist* ps2DDist = (CNtl2DTimeSequence::S2DDist*)m_pSeq->GetData(m_fTime);    
	CNtlMath::MathRwV2dAssign(&m_vFallDelta, (ps2DDist->fX - m_vPrevDist.x) * m_fDistScale, (ps2DDist->fY - m_vPrevDist.y) * m_fDistScale );    
    CNtlMath::MathRwV2dAssign(&m_vPrevDist, ps2DDist->fX, ps2DDist->fY);	
    CNtlMath::MathRwV3dAdd(&vPos, m_vKnockDownDir.x * m_vFallDelta.x, 0.0f, m_vKnockDownDir.z * m_vFallDelta.x);

    // 충돌 체크
    vPos = UpdatePosition(fElapsed, vPos, m_vFallDelta.x / fElapsed);
	
	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

	if(vPos.y + m_vFallDelta.y < sHStuff.fFinialHeight)
	{
		vPos.y = sHStuff.fFinialHeight;
		ChangeKnockDownState(KD_LANDING);

		// visual effect
		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->CreatePLEffect(NTL_VID_TARGET_KNOCKDOWN, &vPos);
	}
	else
	{
		vPos.y += m_vFallDelta.y;
	}

	m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharKnockDown::UpdateFallDown(RwReal fElapsed)
{
	RwV3d vPos = m_pActor->GetPosition();

	m_vFallDelta.y += fElapsed * (-0.2f);

	vPos.x += m_vKnockDownDir.x * m_vFallDelta.x;
	vPos.z += m_vKnockDownDir.z * m_vFallDelta.x;

    // 충돌 체크
    vPos = UpdatePosition(fElapsed, vPos, m_vFallDelta.x / fElapsed);

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);

	if(vPos.y + m_vFallDelta.y < sHStuff.fFinialHeight)
	{
		vPos.y = sHStuff.fFinialHeight;
		ChangeKnockDownState(KD_KNOCK);

		// visual effect
		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->CreatePLEffect(NTL_VID_TARGET_KNOCKDOWN, &vPos);
	}
	else
	{
		vPos.y += m_vFallDelta.y;
	}

	m_pActor->SetPosition(&vPos);
}


void CNtlBehaviorCharKnockDown::UpdateLanding(RwReal fElapsed)
{
	m_fTime += fElapsed;

	if(m_fTime > MAX_KNOCKDOWN_SLIDING_TIME)
	{
		m_fTime = MAX_KNOCKDOWN_SLIDING_TIME;
		CalcLanding(fElapsed);
		ChangeKnockDownState(KD_KNOCK);
		return;
	}
	
	CalcLanding(fElapsed);
}


void CNtlBehaviorCharKnockDown::UpdateKnock(RwReal fElapsed)
{
	if(m_bStanding)
	{
		ChangeKnockDownState(KD_KNOCK_EXIT);
		return;
	}
	if(m_bFainting)
	{
		Finish();
		return;
	}

	m_fTime += fElapsed;
	if(m_fTime >= 8.0f)
	{
		ChangeKnockDownState(KD_KNOCK_EXIT);
		return;
	}
}

void CNtlBehaviorCharKnockDown::CalcLanding(RwReal fElapsed)
{
	RwReal fDistX = *(reinterpret_cast<RwReal*>(m_pLandingSeq->GetData(m_fTime)));
	RwReal fDeltaX = (fDistX - m_vPrevDist.x)*m_fDistScale;

	RwV3d vPos = m_pActor->GetPosition();

	vPos.x += m_vKnockDownDir.x * fDeltaX;
	vPos.z += m_vKnockDownDir.z * fDeltaX;

    // 충돌 체크
    vPos = UpdatePosition(fElapsed, vPos, fDeltaX / fElapsed);

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(m_pActor, &vPos, sHStuff);
	vPos.y = sHStuff.fFinialHeight;

	m_pActor->SetPosition(&vPos);

	m_vPrevDist.x = fDistX;
}

void CNtlBehaviorCharKnockDown::ChangeKnockDownState(RwUInt32 uiState)
{
	NTL_FUNCTION("CNtlBehaviorCharKnockDown::ChangeKnockDownState");

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	RwBool bEquipedStaff = FALSE;
	if(pItemTblData)
		bEquipedStaff = Logic_IsEquipedStaffWeapon(pItemTblData);

	switch(uiState)
	{
	case KD_FALLUP:
		{
			m_fTime = 0.0f;
			m_fDistScale = 1.0f;

			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			if(bEquipedStaff)
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_FLYING, FALSE);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_FLYING, FALSE);
		}
		break;
	case KD_FALLDOWN:
		{
			m_fTime = 0.0f;
		}
		break;
	case KD_LANDING:
		{
			m_fTime = 0.0f;
			m_vPrevDist.x = 0.0f;
			
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

			if(bEquipedStaff)
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_LANDING, FALSE);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_LANDING, FALSE);
		}
		break;
	case KD_KNOCK:
		{
			m_fTime = 0.0f;
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f);

			if(bEquipedStaff)
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_LANDING, FALSE, 1.0f);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_LANDING, FALSE, 1.0f);

			if(m_pActor->GetFlags() == SLFLAG_SERVER_SENDER)
				API_GetSLPacketGenerator()->SendChangePosition(m_pActor->GetPosition(), m_pActor->GetDirection());
		}
		break;
	case KD_KNOCK_EXIT:
		{
			if(m_pActor->GetClassID() == SLCLASS_AVATAR)
			{
				CNtlSLEventGenerator::KnockDownWakeUpNfy();
			}

			m_fTime = 0.0f;
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN);

			if(bEquipedStaff)
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_STAND_UP, FALSE);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_STAND_UP, FALSE);	
		}
		break;
	default:
		NTL_ASSERTFAIL("not exist knock down state");
		break;
	}
	
	m_uiKnockDownState = uiState;

	NTL_RETURNVOID();
}


void CNtlBehaviorCharKnockDown::Enter(void)
{
	// ground fly actor이면?
	// knock down일 경우 잠시 뺴준다. (SLFLAG_CAN_GROUND_FLY)
	if(Logic_IsGroundFlyActor(m_pActor))
	{
		RwUInt32 uiFlags = m_pActor->GetFlags();
		uiFlags = uiFlags & (~SLFLAG_CAN_GROUND_FLY);
		m_pActor->SetFlags(uiFlags);
	}

	RwV3d vTargetDir;
	RwV3dScale(&vTargetDir, &m_vKnockDownDir, -1.0f); 
	m_pActor->SetDirection(&vTargetDir);

	CNtlMath::MathRwV2dAssign(&m_vPrevDist, 0.0f, 0.0f);

	// time sequence setting
	CreateFallupTimeSequence();
	CreateLandingTimeSequence();

	ChangeKnockDownState(KD_FALLUP);

	m_bStanding = FALSE;
	m_bFainting = FALSE;

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        CNtlSLEventGenerator::KnockDownNfy();
    }
				
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharKnockDown::Exit(void)
{
    // ground fly actor이면?
    // 빼주었던 ground fly 기능(SLFLAG_CAN_GROUND_FLY)을 다시 넣어준다.
    if(Logic_IsGroundFlyActor(m_pActor))
    {
        RwUInt32 uiFlags = m_pActor->GetFlags();
        uiFlags = uiFlags | SLFLAG_CAN_GROUND_FLY;
        m_pActor->SetFlags(uiFlags);
    }

    CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
    CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
    pPLCharacter->SetBlend(BLEND_TWEEN);

    DeleteTimeSequence();

    if(m_pActor->GetClassID() == SLCLASS_AVATAR)
    {
        CNtlSLEventGenerator::KnockDownWakeUpNfy();

        // 서버에 자신이 충돌했다는 패킷을 보낸다.
        API_GetSLPacketGenerator()->SendCharLocAfterKnockdownNfy(m_pActor->GetPosition(), m_pActor->GetDirection());
    }        
}


void CNtlBehaviorCharKnockDown::Update(RwReal fElapsed)
{
	if(m_uiKnockDownState == KD_FALLUP)
	{
		UpdateFallUp(fElapsed);
	}
	else if(m_uiKnockDownState == KD_LANDING)
	{
		UpdateLanding(fElapsed);
	}
	else if(m_uiKnockDownState == KD_FALLDOWN)
	{
		UpdateFallDown(fElapsed);
	}
	else if(m_uiKnockDownState == KD_KNOCK)
	{
		UpdateKnock(fElapsed);
	}
}



RwUInt32 CNtlBehaviorCharKnockDown::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharKnockDown::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		if(m_uiKnockDownState == KD_KNOCK_EXIT)
		{
			Finish();
		}
	}
	else if(pMsg.Id == g_EventSobStanding)
	{
		m_bStanding = TRUE;
		FSMEvent_CharActStanding(m_pActor, reinterpret_cast<SNtlEventSobStanding*>( pMsg.pData ));
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobFainting)
	{
		m_bFainting = TRUE;
		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}
	else if(pMsg.Id == g_EventSobHit)
	{
		if(m_uiKnockDownState == KD_KNOCK)
		{
			sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
			RwBool bEquipedStaff = FALSE;
			if(pItemTblData)
				bEquipedStaff = Logic_IsEquipedStaffWeapon(pItemTblData);

			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

			if(bEquipedStaff)
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_HURT, FALSE);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_HURT, FALSE);
		}
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharKnockDown::UpdateData(void)
{
	
}

void CNtlBehaviorCharKnockDown::SetKnockDownDir(const RwV3d *pKnockDownDir)
{
	RwV3dAssignMacro(&m_vKnockDownDir, pKnockDownDir);
	RwV3dNormalize(&m_vKnockDownDir, &m_vKnockDownDir);
}

RwV3d CNtlBehaviorCharKnockDown::UpdatePosition(RwReal fElapsed, RwV3d& vPos, RwReal fSpeed)
{
    RwReal fOrgPosY = vPos.y;

    // 충돌 체크를 한다
    RwBool bCollMoveImpossible = FALSE;
    Logic_CharacterCollisionEx(m_pActor, &vPos, fSpeed, vPos, bCollMoveImpossible, fElapsed);
    vPos.y = fOrgPosY;

    if(bCollMoveImpossible) // 어딘가에 충돌했다. 
    {
        if(m_pActor->GetClassID() == SLCLASS_AVATAR)
        {
            // 서버에 자신이 충돌했다는 패킷을 보낸다.
            API_GetSLPacketGenerator()->SendCharLocAfterKnockdownNfy(vPos, m_pActor->GetDirection());
        }
    } 

    
    return vPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharHurt)

CNtlBehaviorCharHurt::CNtlBehaviorCharHurt()
{
	SetBehaviorId(NTL_BEID_CHAR_HURT);
	SetBehaviorName(NTL_BENAME_CHAR_HURT);
}

CNtlBehaviorCharHurt::~CNtlBehaviorCharHurt()
{
}

void CNtlBehaviorCharHurt::SetTrans(SHitStuff *pHitStuff)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId);
	if(pSobObj == NULL)
	{
		Finish();
		return;
	}

	if(m_pActor->GetFlags() & SLFLAG_HURT_NOT_HEADING_CHANGE)
		return;

	RwV3d vAttPos = pSobObj->GetPosition();
	RwV3d vDefPos = m_pActor->GetPosition();
	RwV3d vHurtDir;

	RwV3dSubMacro(&vHurtDir, &vAttPos, &vDefPos);
	vHurtDir.y = 0.0f;
	RwV3dNormalize(&vHurtDir, &vHurtDir); 
	m_pActor->SetDirection(&vHurtDir);
}

void CNtlBehaviorCharHurt::SetAnim(SHitStuff *pHitStuff)
{
	// anim setting
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
	RwBool bEquipedStaff = FALSE;
	if(pItemTblData)
		bEquipedStaff = Logic_IsEquipedStaffWeapon(pItemTblData);

	if(pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_BLOCK)
	{
		if(bEquipedStaff)
			pSobProxy->SetBaseAnimation(BTL_STAFF_BLOCK_DEF, FALSE);
		else
			pSobProxy->SetBaseAnimation(BTL_DEF_BLOCK_DEF, FALSE);
	}
	else if(pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_DODGE)
	{
		if(bEquipedStaff)
			pSobProxy->SetBaseAnimation(BTL_STAFF_DODGE, FALSE);
		else
			pSobProxy->SetBaseAnimation(BTL_DEF_DODGE, FALSE);
	}
	else
	{
		RwUInt32 uiClassId = m_pActor->GetClassID();
		if( uiClassId == SLCLASS_PLAYER || uiClassId == SLCLASS_AVATAR)
		{
			RwBool bFightingMode = 0;
			CNtlSLEventGenerator::SobGetFightingMode(m_pActor->GetSerialID(), bFightingMode);
			if(bFightingMode)
			{
				if(bEquipedStaff)
					pSobProxy->SetBaseAnimation(BTL_STAFF_FP_HURT, FALSE);
				else
					pSobProxy->SetBaseAnimation(BTL_DEF_FP_HURT, FALSE);
			}
			else
				pSobProxy->SetBaseAnimation(NML_IDLE_HURT, FALSE);
		}
		else
			pSobProxy->SetBaseAnimation(NML_IDLE_HURT, FALSE);
	}
}

void CNtlBehaviorCharHurt::Enter(void)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHitStuff *pHitStuff = pBeData->GetHitStuff();
	
	// transform
	SetTrans(pHitStuff);

	// animation
	SetAnim(pHitStuff);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharHurt::Update(RwReal fElapsed)
{
	
}

RwUInt32 CNtlBehaviorCharHurt::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharHurt::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		Finish();
	}

	else if(pMsg.Id == g_EventSobHit)
	{
		SNtlEventSobHit *pSobHit = reinterpret_cast<SNtlEventSobHit*>( pMsg.pData );
		SetAnim(&pSobHit->sHitStuff);
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharHurt::UpdateData(void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharHurtTossUp)

CNtlBehaviorCharHurtTossUp::CNtlBehaviorCharHurtTossUp()
{
	SetBehaviorId(NTL_BEID_CHAR_HURTTOSS_UP);
	SetBehaviorName(NTL_BENAME_CHAR_HURTTOSS_UP);

	m_byTossUpState = TOSS_UP_UP;
	m_pSeq = NULL;
	m_fTime = 0.0f;
	m_fPrevDist = 0.0f;
}

CNtlBehaviorCharHurtTossUp::~CNtlBehaviorCharHurtTossUp()
{
	DeleteTimeSequence();
}

void CNtlBehaviorCharHurtTossUp::SetAnim(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(BTL_DEF_HTB_TOSS_UP, FALSE);
}

void CNtlBehaviorCharHurtTossUp::CreateTimeSequence(void)
{
#define HURT_TOSS_UP_SEQ_CNT	12
	
	RwReal arrTime[HURT_TOSS_UP_SEQ_CNT] = { 0.067f, 0.133f, 0.200f, 0.267f, 0.333f, 0.400f, 0.487f, 0.533f, 0.600f, 0.667f, 0.733f };
	RwReal arrDist[HURT_TOSS_UP_SEQ_CNT] = { 0.966f, 1.850f, 2.600f, 3.400f, 4.050f, 4.620f, 5.100f, 5.500f, 5.780f, 5.960f, 6.000f };
	
	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for(RwInt32 i = 0; i < HURT_TOSS_UP_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDist[i]);

		if(i == 0)
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharHurtTossUp::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}

void CNtlBehaviorCharHurtTossUp::UpdateUp(RwReal fElapsed)
{
#define MAX_TOSS_UP_TIME				0.733f

	m_fTime += fElapsed;

	if(m_fTime >= MAX_TOSS_UP_TIME)
	{
		m_byTossUpState = TOSS_UP_IDLE;
		m_fTime = MAX_TOSS_UP_TIME;

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(BTL_DEF_HTB_TOSS_IDLE, TRUE);
	}

	RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
	RwReal fDelta = (fDist - m_fPrevDist);
	RwV3d vPos = m_pActor->GetPosition();

	vPos.y += fDelta*2.0f;

	m_pActor->SetPosition(&vPos);

	m_fPrevDist = fDist;
}

void CNtlBehaviorCharHurtTossUp::UpdateIdle(RwReal fElapsed)
{
}

void CNtlBehaviorCharHurtTossUp::UpdateDown(RwReal fElapsed)
{
}

void CNtlBehaviorCharHurtTossUp::Enter(void)
{
	// time sequence setting
	CreateTimeSequence();

	// animation setting
	SetAnim();

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharHurtTossUp::Exit(void)
{
	DeleteTimeSequence();

	// 나중에 base class exit를 호출한다.
	CNtlBehaviorBase::Exit();
}

void CNtlBehaviorCharHurtTossUp::Update(RwReal fElapsed)
{
// HTB (toss up) - CNtlBehaviorCharHurtTossUp::CreateTimeSequence function table 참조.
	if(m_byTossUpState == TOSS_UP_UP)
		UpdateUp(fElapsed);
	else if(m_byTossUpState == TOSS_UP_IDLE)
		UpdateIdle(fElapsed);
	else
		UpdateDown(fElapsed);
}

RwUInt32 CNtlBehaviorCharHurtTossUp::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharHurtTossUp::HandleEvents");

	if(pMsg.Id == g_EventSobHit)
	{
		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(BTL_DEF_HTB_TOSS_HURT, FALSE);
	}
	else if(pMsg.Id == g_EventAnimEnd)
	{
		SNtlEventAnimEnd *pSobAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>(pMsg.pData);
		if(pSobAnimEnd->uiBaseAnimKey == BTL_DEF_HTB_TOSS_HURT)
		{
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			pSobProxy->SetBaseAnimation(BTL_DEF_HTB_TOSS_IDLE, FALSE);
		}
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharHurtTossUp::UpdateData(void)
{
}



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

DEFINITION_MEMORY_POOL(CNtlBehaviorCharHurtTossDown)

CNtlBehaviorCharHurtTossDown::CNtlBehaviorCharHurtTossDown()
{
	SetBehaviorId(NTL_BEID_CHAR_HURTTOSS_DOWN);
	SetBehaviorName(NTL_BENAME_CHAR_HURTTOSS_DOWN);

	m_byTossDownState	= TOSS_DOWN_DOWN;
}

CNtlBehaviorCharHurtTossDown::~CNtlBehaviorCharHurtTossDown()
{
}

void CNtlBehaviorCharHurtTossDown::SetAnim(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(BTL_DEF_HTB_FALL_DOWN, FALSE);
}

void CNtlBehaviorCharHurtTossDown::UpdateDown(RwReal fElapsed)
{
	// HTB (toss down) - CNtlBehaviorCharHurtTossDown::CreateTimeSequence function table 참조.
#define MAX_TOSS_DOWN_SPEED			46.0f

	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(&vPos, sHStuff);

	vPos.y -= MAX_TOSS_DOWN_SPEED*fElapsed;
	
	if(vPos.y <= sHStuff.fFinialHeight)
	{
		vPos.y = sHStuff.fFinialHeight;
		m_byTossDownState = TOSS_DOWN_IDLE;

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(BTL_DEF_HTB_FALL_DOWN_LANDING, FALSE);
	}

	m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorCharHurtTossDown::UpdateIdle(RwReal fElapsed)
{
	
}


void CNtlBehaviorCharHurtTossDown::Enter(void)
{
    // ground fly actor이면?
    // knock down일 경우 잠시 뺴준다. (SLFLAG_CAN_GROUND_FLY)
    if(Logic_IsGroundFlyActor(m_pActor))
    {
        RwUInt32 uiFlags = m_pActor->GetFlags();
        uiFlags = uiFlags & (~SLFLAG_CAN_GROUND_FLY);
        m_pActor->SetFlags(uiFlags);
    }

	SetAnim();

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharHurtTossDown::Exit(void)
{
    // ground fly actor이면?
    // 빼주었던 ground fly 기능(SLFLAG_CAN_GROUND_FLY)을 다시 넣어준다.
    if(Logic_IsGroundFlyActor(m_pActor))
    {
        RwUInt32 uiFlags = m_pActor->GetFlags();
        uiFlags = uiFlags | SLFLAG_CAN_GROUND_FLY;
        m_pActor->SetFlags(uiFlags);
    }

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharHurtTossDown::Update(RwReal fElapsed)
{
	if(m_byTossDownState == TOSS_DOWN_DOWN)
		UpdateDown(fElapsed);
	else
		UpdateIdle(fElapsed);
}

RwUInt32 CNtlBehaviorCharHurtTossDown::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharHurtTossDown::HandleEvents");

	if(pMsg.Id == g_EventSobHit)
	{
		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		pSobProxy->SetBaseAnimation(BTL_DEF_HTB_FALL_DOWN_HURT, FALSE);
	}
	else if(pMsg.Id == g_EventSobStanding)
	{
		FSMEvent_CharActStanding(m_pActor, reinterpret_cast<SNtlEventSobStanding*>( pMsg.pData ));

		CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
		CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
		pPLCharacter->SetBlend(BLEND_TWEEN);

		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(m_pActor);
		if(pItemTblData)
		{
			if(Logic_IsEquipedStaffWeapon(pItemTblData))
				pSobProxy->SetBaseAnimation(BTL_STAFF_KD_STAND_UP, FALSE);
			else
				pSobProxy->SetBaseAnimation(BTL_DEF_KD_STAND_UP, FALSE);	
		}
		else
		{
			pSobProxy->SetBaseAnimation(BTL_DEF_KD_STAND_UP, FALSE);	
		}

		NTL_RETURN(NTL_FSM_EVENTRES_BLOCK);
	}

	else if(pMsg.Id == g_EventAnimEnd)
	{
		SNtlEventAnimEnd *pSobAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>(pMsg.pData);
		if(pSobAnimEnd->uiBaseAnimKey == BTL_STAFF_KD_STAND_UP || pSobAnimEnd->uiBaseAnimKey == BTL_DEF_KD_STAND_UP)
		{
			Finish();
		}
	}
	
	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharHurtTossDown::UpdateData(void)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharHomingUp)

CNtlBehaviorCharHomingUp::CNtlBehaviorCharHomingUp()
{
	SetBehaviorId(NTL_BEID_CHAR_HOMING_UP);
	SetBehaviorName(NTL_BENAME_CHAR_HOMING_UP);

	m_bySpeedSearch = 0;
}

CNtlBehaviorCharHomingUp::~CNtlBehaviorCharHomingUp()
{
}

void CNtlBehaviorCharHomingUp::SetAnim(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(BTL_DEF_HTB_HOMING_UP, FALSE);
}

void CNtlBehaviorCharHomingUp::Enter(void)
{
	SetAnim();

	// target position setting
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	CNtlSob *pTargetSobObj = GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);

	if(pTargetSobObj == NULL)
	{
		Finish();
		return;
	}
	else
	{
		m_vOldTargetPos = pTargetSobObj->GetPosition();
	}

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharHomingUp::Exit(void)
{
	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharHomingUp::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	CNtlSob *pTargetSobObj = GetNtlSobManager()->GetSobObject(pHTBStuff->hTargetSerialId);

	if(pTargetSobObj == NULL)
	{
		Finish();
		return;
	}

	RwV3d vTargetPos = pTargetSobObj->GetPosition();

	RwV3d vSub;
	RwV3dSubMacro(&vSub, &vTargetPos, &m_vOldTargetPos);
	RwReal fDeltaDist = RwV3dLength(&vSub);
	if(fDeltaDist < 0.1f*fElapsed)
		m_bySpeedSearch++;
	else
		m_bySpeedSearch = 0;

	m_vOldTargetPos = vTargetPos;

	if(m_bySpeedSearch >= 3)
	{
		RwV3d vPos = m_pActor->GetPosition();
//		vPos.y = vTargetPos.y + 3.5f;
		vPos.y = vTargetPos.y;
		m_pActor->SetPosition(&vPos);

		Finish();
	}
}

RwUInt32 CNtlBehaviorCharHomingUp::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharHomingUp::HandleEvents");

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharHomingUp::UpdateData(void)
{
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 
DEFINITION_MEMORY_POOL(CNtlBehaviorCharHomingDown)

CNtlBehaviorCharHomingDown::CNtlBehaviorCharHomingDown()
{
	SetBehaviorId(NTL_BEID_CHAR_HOMING_DOWN);
	SetBehaviorName(NTL_BENAME_CHAR_HOMING_DOWN);

	m_pSeq				= NULL;		
	m_fTime				= 0.0f;
	m_fPrevDist			= 0.0f;
	m_bLandingEnd		= FALSE;
	m_bLandingAnimPlay	= FALSE;
	m_bHomingDown		= TRUE;
}

CNtlBehaviorCharHomingDown::~CNtlBehaviorCharHomingDown()
{
	DeleteTimeSequence();
}

void CNtlBehaviorCharHomingDown::SetAnim(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());

	if(pSobProxy->IsExistBaseAnimation(BTL_DEF_HTB_HOMING_DOWN))
	{
		pSobProxy->SetBaseAnimation(BTL_DEF_HTB_HOMING_DOWN, FALSE);
	}
	else
	{
		pSobProxy->SetBaseAnimation(NML_IDLE_LOOP, TRUE);
		m_bHomingDown = FALSE;
	}
}

void CNtlBehaviorCharHomingDown::CreateTimeSequence(void)
{
#define HURT_HOMING_DOWN_SEQ_CNT	12

	RwReal arrTime[HURT_HOMING_DOWN_SEQ_CNT] = { 0.067f, 0.133f, 0.200f, 0.267f, 0.366f, 0.487f, 0.567f, 0.633f, 0.700f, 0.766f, 0.833f, 0.866f };
	RwReal arrDist[HURT_HOMING_DOWN_SEQ_CNT] = { 0.170f, 0.759f, 1.945f, 3.514f, 6.357f, 9.675f, 13.34f, 15.86f, 17.88f, 19.24f, 19.92f, 20.00f };

	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for(RwInt32 i = 0; i < HURT_HOMING_DOWN_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDist[i]);

		if(i == 0)
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharHomingDown::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}


void CNtlBehaviorCharHomingDown::Enter(void)
{
	CreateTimeSequence();

	SetAnim();

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}


void CNtlBehaviorCharHomingDown::Exit(void)
{
	DeleteTimeSequence();

	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
	pPLCharacter->SetBlend(BLEND_TWEEN);

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharHomingDown::UpdateHomingDown(RwReal fElapsed)
{
	if(m_bLandingEnd)
		return;

	// HTB (homing down) - CNtlBehaviorCharHomingDown::CreateTimeSequence function table 참조.
#define MAX_HOMMING_DOWN_TIME				0.866f

	m_fTime += fElapsed;

	if(m_fTime >= MAX_HOMMING_DOWN_TIME)
	{
		if(!m_bLandingAnimPlay)
		{
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
			pSobProxy->SetBaseAnimation(BTL_DEF_HTB_HOMING_LANDING, FALSE);
			m_bLandingAnimPlay	= TRUE;
		}

		m_bLandingEnd		= TRUE;
		m_fTime = MAX_HOMMING_DOWN_TIME;
	}

	RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
	RwReal fDelta = (fDist - m_fPrevDist);
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(&vPos, sHStuff);

	vPos.y -= fDelta;

	if(vPos.y <= sHStuff.fFinialHeight + 2.0f)
	{
		if(!m_bLandingAnimPlay)
		{
			CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
			CNtlPLCharacter *pPLCharacter = reinterpret_cast<CNtlPLCharacter*>( pSobProxy->GetPLMainEntity() );
			pPLCharacter->SetBlend(BLEND_TWEEN, 0.0f, 0.0f);
			pSobProxy->SetBaseAnimation(BTL_DEF_HTB_HOMING_LANDING, FALSE);
			m_bLandingAnimPlay	= TRUE;
		}
	}
	
	if(vPos.y <= sHStuff.fFinialHeight)
	{
		m_bLandingEnd		= TRUE;
		
		vPos.y = sHStuff.fFinialHeight;
	}

	if(m_bLandingEnd)
	{
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(&vPos, sHStuff);
		vPos.y = sHStuff.fFinialHeight;
	}
	
	m_pActor->SetPosition(&vPos);

	m_fPrevDist = fDist;

}

void CNtlBehaviorCharHomingDown::UpdateIdleDown(RwReal fElapsed)
{
	// HTB (homing down) - CNtlBehaviorCharHomingDown::CreateTimeSequence function table 참조.
#define MAX_HOMMING_DOWN_TIME				0.866f

	RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
	RwReal fDelta = (fDist - m_fPrevDist);
	RwV3d vPos = m_pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	Logic_GetWorldHeight(&vPos, sHStuff);

	vPos.y -= fDelta;

	if(vPos.y <= sHStuff.fFinialHeight)
	{
		vPos.y = sHStuff.fFinialHeight;

		Finish();
	}
	
	m_pActor->SetPosition(&vPos);

	m_fPrevDist = fDist;
}

void CNtlBehaviorCharHomingDown::Update(RwReal fElapsed)
{
	if(m_bHomingDown)
		UpdateHomingDown(fElapsed);
	else
		UpdateIdleDown(fElapsed);
}

RwUInt32 CNtlBehaviorCharHomingDown::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharHomingDown::HandleEvents");

	if(pMsg.Id == g_EventAnimEnd)
	{
		if(m_bLandingAnimPlay)
		{
			Finish();
		}
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharHomingDown::UpdateData(void)
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

DEFINITION_MEMORY_POOL(CNtlBehaviorCharJumpingUp)

CNtlBehaviorCharJumpingUp::CNtlBehaviorCharJumpingUp()
{
	SetBehaviorId(NTL_BEID_CHAR_JUMPING_UP);
	SetBehaviorName(NTL_BENAME_CHAR_JUMPING_UP);

	m_pSeq		= NULL;		
	m_fTime		= 0.0f;
	m_fPrevDist = 0.0f;
}

CNtlBehaviorCharJumpingUp::~CNtlBehaviorCharJumpingUp()
{
	DeleteTimeSequence();
}

void CNtlBehaviorCharJumpingUp::SetAnim(void)
{
	CNtlSobCharProxy *pSobProxy = reinterpret_cast<CNtlSobCharProxy*>(m_pActor->GetSobProxy());
	pSobProxy->SetBaseAnimation(BTL_DEF_HTB_HOMING_UP, FALSE);
}


void CNtlBehaviorCharJumpingUp::CreateTimeSequence(void)
{
#define HURT_JUMP_UP_SEQ_CNT	5

	RwReal arrTime[HURT_JUMP_UP_SEQ_CNT] = { 0.067f, 0.133f, 0.200f, 0.267f, 0.333f };
	RwReal arrDist[HURT_JUMP_UP_SEQ_CNT] = { 4.120f, 7.720f, 9.930f, 11.28f, 12.00f };

	CNtlDistTimeSequence *pSeq = NULL;
	CNtlDistTimeSequence *pTempSeq = NULL;

	for(RwInt32 i = 0; i < HURT_JUMP_UP_SEQ_CNT; ++i)
	{
		pTempSeq = NTL_NEW CNtlDistTimeSequence;
		pTempSeq->SetTime(arrTime[i]);
		pTempSeq->SetDistance(arrDist[i]);

		if(i == 0)
		{
			m_pSeq = pTempSeq;
			pSeq = pTempSeq;
		}
		else
		{
			pSeq->Add(pTempSeq);
			pSeq = pTempSeq;
		}
	}
}

void CNtlBehaviorCharJumpingUp::DeleteTimeSequence(void)
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}

	m_pSeq = NULL;
}

void CNtlBehaviorCharJumpingUp::Enter(void)
{
	CreateTimeSequence();

	SetAnim();

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharJumpingUp::Exit(void)
{
	DeleteTimeSequence();

	// 나중에 base class enter를 호출한다.
	CNtlBehaviorBase::Exit(); 
}

void CNtlBehaviorCharJumpingUp::Update(RwReal fElapsed)
{
	if(IsFinish())
		return;

#define MAX_JUMPING_UP_TIME				0.333f

	m_fTime += fElapsed;

	if(m_fTime >= MAX_JUMPING_UP_TIME)
	{
		Finish();
		m_fTime = MAX_JUMPING_UP_TIME;
	}

	RwReal fDist = *(reinterpret_cast<RwReal*>(m_pSeq->GetData(m_fTime)));
	RwReal fDelta = (fDist - m_fPrevDist);
	RwV3d vPos = m_pActor->GetPosition();

	vPos.y += fDelta;

	m_pActor->SetPosition(&vPos);

	m_fPrevDist = fDist;
}

RwUInt32 CNtlBehaviorCharJumpingUp::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlBehaviorCharJumpingUp::HandleEvents");

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharJumpingUp::UpdateData(void)
{
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharStretchAttack)
CNtlBehaviorCharStretchAttack::CNtlBehaviorCharStretchAttack() 
{
    m_pCharacter    = NULL;
    m_uiTargetId    = INVALID_SERIAL_ID;
    m_pStretchEvent  = NULL;
    m_bPulling      = FALSE;
    m_bOrgBoneScale = FALSE;
    m_fStretchPullingSpeed = 0.0f;
    m_vTargetPos = ZeroAxis;

    for(int i = 0; i < 3; ++i)
    {
        m_fOrgBoneLength[i] = 1.0f;
        m_fOrgBoneWidth[i] = 1.0f;
    }
}

CNtlBehaviorCharStretchAttack::~CNtlBehaviorCharStretchAttack() 
{

}

void CNtlBehaviorCharStretchAttack::Enter( void ) 
{
    m_uiTargetId = Logic_GetActorTargetSerialId(m_pActor);
    if(m_uiTargetId == INVALID_SERIAL_ID)
    {
        Finish();
        return;
    }

    m_pCharacter = (CNtlPLCharacter*)m_pActor->GetSobProxy()->GetPLMainEntity();    

    // 애니메이션을 멈춘다.    
    m_pCharacter->GetBoneData()->bBoneScale = TRUE;
    m_pCharacter->SetAnimUpdate(FALSE);

    CNtlBehaviorBase::Enter(); 
}

void CNtlBehaviorCharStretchAttack::Exit( void ) 
{
    if(m_pCharacter)
    {
        RwUInt32 uiScaleBoneIndex = m_pCharacter->GetBoneIndex(m_pStretchEvent->szScaleBone);
        RwUInt32 uiBone1 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[0]);
        RwUInt32 uiBone2 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[1]);

        m_pCharacter->GetBoneData()->bBoneScale = m_bOrgBoneScale;        
        m_pCharacter->GetBoneData()->BoneScale[uiBone1].fLength = m_fOrgBoneLength[0];
        m_pCharacter->GetBoneData()->BoneScale[uiBone1].fWidth = m_fOrgBoneWidth[0];
        m_pCharacter->GetBoneData()->BoneScale[uiBone2].fLength = m_fOrgBoneLength[1];
        m_pCharacter->GetBoneData()->BoneScale[uiBone2].fWidth = m_fOrgBoneWidth[1];
        m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fLength = m_fOrgBoneLength[2];
        m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth = m_fOrgBoneWidth[2];
        m_bPulling = FALSE;
        m_pCharacter->SetAnimUpdate(TRUE);
        m_pCharacter->GetBoneData()->bBoneScale = m_bOrgBoneScale;
        m_pCharacter->SetSBoneStretchTargetPos(ZeroAxis, "");
        Finish();            
    }    
}

void CNtlBehaviorCharStretchAttack::Update( RwReal fElapsed ) 
{
    if(!m_pStretchEvent || !m_pCharacter)
        return;

    CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(m_uiTargetId);
    if(!pTargetActor)
        return;

    RwMatrix* pMatHand = m_pCharacter->GetBoneMatrix(m_pStretchEvent->szScaleBone);
    RwUInt32 uiScaleBoneIndex = m_pCharacter->GetBoneIndex(m_pStretchEvent->szScaleBone);
    RwUInt32 uiBone1 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[0]);
    RwUInt32 uiBone2 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[1]);

    RwV3d posHand = *RwMatrixGetPos(pMatHand);
    RwV3d posActor = m_pActor->GetPosition();
    RwV3d posTarget = m_vTargetPos;
    posHand.y = posActor.y = posTarget.y = 0.0f;
    RwReal fBoneDist = CNtlMath::GetLength(posActor, posHand);
    RwReal fTargetDist = GetTargetDistance(pTargetActor);

    if(!m_bPulling && (fBoneDist >= fTargetDist || fBoneDist > 26.0f))
    {
        m_fStretchPullingSpeed = m_pStretchEvent->fStretchSpeed;
        m_bPulling = TRUE;

        // 타겟 이펙트
        if(strlen(m_pStretchEvent->szTargetEffect) > 0)
        {
            CNtlInstanceEffect* pEffect =(CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pStretchEvent->szTargetEffect);
            m_pCharacter->AttachBone(pEffect, m_pStretchEvent->szScaleBone);            
        }        
    }


    m_fStretchPullingSpeed += m_fStretchPullingSpeed * m_pStretchEvent->fAccel * fElapsed;      // 가속도 
    if(m_bPulling)              // 당겨오는 상태
    {
        // NOTE : 어린애는 이미 Bone이 조절되어 있기 때문에, 아마 겹치는 문제가 생길지도 모른다.
        // 우선 성인 캐릭터용 스킬로 사용하고, 차후에 어린상태에서도 사용하게 된다면 그때 수정을 생각한다. (by agebreak 08.10.20)
        if(fabs(fTargetDist) <= DBO_SYSTEM_EFFECT_PULL_DISTANCE || m_pCharacter->GetBoneData()->BoneScale[uiBone1].fLength <= 0.0f ||
            fTargetDist > 30.0f || m_pCharacter->GetBoneData()->BoneScale[uiBone2].fLength <= 0.0f)
        {
            // 마지막 위치는 이동 싱크를 위해서 강제로 맞춘다.
            RwV3d vTargetDir = posTarget - posActor;
            RwV3dNormalize(&vTargetDir, &vTargetDir);
            RwReal fActorRadius = max(m_pActor->GetSobProxy()->GetPLEntityWidth(), m_pActor->GetSobProxy()->GetPLEntityDepth());
            RwV3d vDestPos = posActor + vTargetDir * (fActorRadius + DBO_SYSTEM_EFFECT_PULL_DISTANCE);
            SWorldHeightStuff hStuff;
            Logic_GetWorldHeight(&vDestPos, hStuff);

         //   vDestPos.y = hStuff.fWorldHeight;
			vDestPos.y = posActor.y;

            pTargetActor->SetPosition(&vDestPos);

            Finish();            
            return;
        }   
               
        if(m_pStretchEvent->eType == E_STRETCH_PULLING) // If hit, the target is not pulled.
        {
            // 타겟의 위치
            SWorldHeightStuff hStuff;
            RwV3d vHandPos = pMatHand->pos;
            Logic_GetWorldHeight(&vHandPos, hStuff);
            vHandPos.y = hStuff.fWorldHeight;
            pTargetActor->SetPosition(&vHandPos);            
        }

        m_pCharacter->GetBoneData()->BoneScale[uiBone1].fLength -= m_fStretchPullingSpeed * fElapsed;            
        m_pCharacter->GetBoneData()->BoneScale[uiBone2].fLength -= m_fStretchPullingSpeed * fElapsed;
        
    }
    else    // 뻗어나가는 상태                        
    {
        m_pCharacter->GetBoneData()->BoneScale[uiBone1].fLength += m_fStretchPullingSpeed * fElapsed;
        m_pCharacter->GetBoneData()->BoneScale[uiBone1].fWidth = m_pStretchEvent->fWidth;
        m_pCharacter->GetBoneData()->BoneScale[uiBone2].fLength += m_fStretchPullingSpeed * fElapsed;
        m_pCharacter->GetBoneData()->BoneScale[uiBone2].fWidth = m_pStretchEvent->fWidth;
        m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fLength = m_pStretchEvent->fScaleSize;
        m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth = m_pStretchEvent->fScaleSize;        
    }             
}

RwUInt32 CNtlBehaviorCharStretchAttack::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimStretch)
    {
        SNtlEventStretch* pEvent = (SNtlEventStretch*)pMsg.pData;
        m_pStretchEvent = (SEventStretch*)pEvent->pData;
        m_fStretchPullingSpeed = m_pStretchEvent->fStretchSpeed;
        CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(m_uiTargetId);

        if(pTargetActor)
        {
            m_vTargetPos = pTargetActor->GetPosition();
            m_vTargetPos.y += pTargetActor->GetSobProxy()->GetPLEntityHeight() * 0.8f;
            m_pCharacter->SetSBoneStretchTargetPos(m_vTargetPos, m_pStretchEvent->szAxisBone);

            // 기존 Bone 정보 저장            
            RwUInt32 uiScaleBoneIndex = m_pCharacter->GetBoneIndex(m_pStretchEvent->szScaleBone);
            RwUInt32 uiBone1 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[0]);
            RwUInt32 uiBone2 = m_pCharacter->GetBoneIndex(m_pStretchEvent->szBoneName[1]);

            m_bOrgBoneScale = m_pCharacter->GetBoneData()->bBoneScale;
            m_fOrgBoneLength[0] = m_pCharacter->GetBoneData()->BoneScale[uiBone1].fLength;
            m_fOrgBoneLength[1] = m_pCharacter->GetBoneData()->BoneScale[uiBone2].fLength;
            m_fOrgBoneLength[2] = m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fLength;
            m_fOrgBoneWidth[0] = m_pCharacter->GetBoneData()->BoneScale[uiBone1].fWidth;
            m_fOrgBoneWidth[1] = m_pCharacter->GetBoneData()->BoneScale[uiBone2].fWidth;
            m_fOrgBoneWidth[2] = m_pCharacter->GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth;
        }
    }    

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharStretchAttack::UpdateData( void ) 
{

}

RwReal CNtlBehaviorCharStretchAttack::GetTargetDistance(CNtlSobActor* pTargetActor)
{
    if(!pTargetActor)
        return 0.0f;

    // 서버에서는 살과살 거리(Radius)라고 계산한다. 클라도 맞춰서 계산한다.
    RwReal fDistance = CNtlMath::GetLength(m_pActor->GetPosition(), pTargetActor->GetPosition());
    fDistance -= max(m_pActor->GetSobProxy()->GetPLEntityWidth(), m_pActor->GetSobProxy()->GetPLEntityDepth());
    fDistance -= max(m_pActor->GetSobProxy()->GetPLEntityWidth(), m_pActor->GetSobProxy()->GetPLEntityDepth());

    return fDistance;
}
//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharActiveWarpBackAttack)
CNtlBehaviorCharActiveWarpBackAttack::CNtlBehaviorCharActiveWarpBackAttack() 
{
    
}

CNtlBehaviorCharActiveWarpBackAttack::~CNtlBehaviorCharActiveWarpBackAttack() 
{

}

void CNtlBehaviorCharActiveWarpBackAttack::Enter( void ) 
{
}

void CNtlBehaviorCharActiveWarpBackAttack::Exit( void ) 
{

}

void CNtlBehaviorCharActiveWarpBackAttack::Update( RwReal fElapsed ) 
{

}

RwUInt32 CNtlBehaviorCharActiveWarpBackAttack::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimTrigger)
    {
        OnWarpBack();
        Finish();
    }

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharActiveWarpBackAttack::SetFinalLoc(RwV3d& loc)
{
	m_vFinalLoc = loc;
}

void CNtlBehaviorCharActiveWarpBackAttack::OnWarpBack() 
{
    RwUInt32 uiTargetID = Logic_GetActorTargetSerialId(m_pActor);
    CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiTargetID);
    if(!pTargetActor)
        return;

    //// Jump to the 1M position behind the opponent (calculate thickness)
    //RwV3d vPos = pTargetActor->GetPosition();
    //vPos += pTargetActor->GetDirection() * -(pTargetActor->GetSobProxy()->GetPLEntityDepth() + 1.0f);
    //
    //SWorldHeightStuff sHStuff;
    //Logic_GetWorldHeight( &vPos, sHStuff );
    //vPos.y = sHStuff.fFinialHeight;
    
    m_pActor->SetPosition(&m_vFinalLoc);
    m_pActor->SetDirection(&pTargetActor->GetDirection());
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharActiveWarp)

CNtlBehaviorCharActiveWarp::CNtlBehaviorCharActiveWarp() 
{

}

CNtlBehaviorCharActiveWarp::~CNtlBehaviorCharActiveWarp() 
{

}

RwUInt32 CNtlBehaviorCharActiveWarp::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimTrigger)
    {
        OnWarpTarget();
        Finish();
    }

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharActiveWarp::SetFinalLoc(RwV3d& loc)
{
	m_vFinalLoc = loc;
}

void CNtlBehaviorCharActiveWarp::OnWarpTarget() 
{
    /// Warp around the target.
    RwUInt32 uiTargetID = Logic_GetActorTargetSerialId(m_pActor);
    CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiTargetID);
    if(!pTargetActor)
        return;

    //// Move to the other party.
    //RwV3d vPos = pTargetActor->GetPosition();
    //vPos += pTargetActor->GetDirection() * (pTargetActor->GetSobProxy()->GetPLEntityDepth() + 1.0f);

    //// Judge whether you can move
    //if(GetSceneManager()->GetWorldAttribute(vPos) & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE)
    //{
    //    // If it can not move, search around the area randomly.               
    //    for(;;)
    //    {
    //        vPos.x = NtlRandomNumber(pTargetActor->GetPosition().x - 1.0f, pTargetActor->GetPosition().x + 1.0f);
    //        vPos.z = NtlRandomNumber(pTargetActor->GetPosition().z - 1.0f, pTargetActor->GetPosition().z + 1.0f);
    //        if(!(GetSceneManager()->GetWorldAttribute(vPos) & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE))
    //        {
    //            break;
    //        }
    //    }
    //}

    //SWorldHeightStuff sHStuff;
    //Logic_GetWorldHeight( &vPos, sHStuff );
    //vPos.y = sHStuff.fFinialHeight;

    RwV3d vDir = pTargetActor->GetPosition() - m_vFinalLoc;
    vDir.y = 0.0f;
    RwV3dNormalize(&vDir, &vDir);

    m_pActor->SetPosition(&m_vFinalLoc);
    m_pActor->SetDirection(&vDir);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharActiveThroughAttack)

CNtlBehaviorCharActiveThroughAttack::CNtlBehaviorCharActiveThroughAttack() 
{
}

CNtlBehaviorCharActiveThroughAttack::~CNtlBehaviorCharActiveThroughAttack() 
{
}


void CNtlBehaviorCharActiveThroughAttack::Enter( void ) 
{
}

RwUInt32 CNtlBehaviorCharActiveThroughAttack::HandleEvents( RWS::CMsg &pMsg ) 
{
	if (pMsg.Id == g_EventAnimTrigger)
	{
		OnThroughAttack();
		Finish();
	}

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharActiveThroughAttack::Update( RwReal fElapsed ) 
{
}

void CNtlBehaviorCharActiveThroughAttack::SetFinalLoc(RwV3d& loc)
{
	m_vFinalLoc = loc;
}

void CNtlBehaviorCharActiveThroughAttack::OnThroughAttack()
{
	// info:Logic_GetActorTargetSerialId can return INVALID SERIAL NUMBER !

	//RwUInt32 uiTargetID = Logic_GetActorTargetSerialId(m_pActor);
	//CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiTargetID);
	//if (!pTargetActor)
	//{
	//	DBO_WARNING_MESSAGE("RIP: USER ID: " << uiTargetID);
	//	return;
	//}

//	RwV3d vPrevPos = m_pActor->GetPosition();
//	RwV3d vTargetPos = pTargetActor->GetPosition();
//	vPrevPos.y = vTargetPos.y = 0.0f;

//	RwV3d vDir = vTargetPos - vPrevPos;
//	RwV3dNormalize(&vDir, &vDir);

	m_pActor->SetPosition(&m_vFinalLoc);
}

void CNtlBehaviorCharActiveThroughAttack::CalcHitDamage()
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(m_pActor->GetBehaviorData()); 
    CNtlAttackData *pAttData = pBeData->GetAttackData();
    SAttackStuff *pAttackStuff = NULL;

    if(pAttData->GetAttackStuffCount() == 0)
        return;

    for(int i = 0; i < pAttData->GetAttackStuffCount(); ++i)
    {
        pAttackStuff = pAttData->GetAttackStuff(i);
        if(!pAttackStuff)
            continue;

        CNtlSobActor* pTarget = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pAttackStuff->hDefenderSerialId);
        if(!pTarget)
            continue;

        FSMEvent_CharActHit(pTarget, pAttackStuff->uiType.bitResult, (WORD)pAttackStuff->sDamage.iDamage);
    }
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharTransform)

CNtlBehaviorCharTransform::CNtlBehaviorCharTransform() 
{
    m_uiAnimAction = 0;
    m_uiAnimLoop = 0;
}

CNtlBehaviorCharTransform::~CNtlBehaviorCharTransform() 
{

}

void CNtlBehaviorCharTransform::Enter( void ) 
{

}

void CNtlBehaviorCharTransform::SetAnimation( RwUInt32 uiAction, RwUInt32 uiActionLoop ) 
{
    m_uiAnimAction = uiAction;
    m_uiAnimLoop = uiActionLoop;
}

void CNtlBehaviorCharTransform::Update( RwReal fElapsed ) 
{

}

RwUInt32 CNtlBehaviorCharTransform::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventAnimEnd)
    {
        SNtlEventAnimEnd *pAnimEnd = reinterpret_cast<SNtlEventAnimEnd*>( pMsg.pData );

        if(pAnimEnd->uiBaseAnimKey == m_uiAnimAction)
        {
            // 일반 Anim이 끝나면 Loop Anim을 재생한다.
            m_pActor->GetSobProxy()->SetBaseAnimation(m_uiAnimLoop);
        }        
    }
    else if(pMsg.Id == g_EventMobTransform)
    {
        // 변신 패킷이 날라오면 상태를 종료한다.         
        Finish();
    }

    NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorCharActivePull)
CNtlBehaviorCharActivePull::CNtlBehaviorCharActivePull()
{
}

CNtlBehaviorCharActivePull::~CNtlBehaviorCharActivePull()
{
}

void CNtlBehaviorCharActivePull::Enter(void)
{
}

void CNtlBehaviorCharActivePull::Update(RwReal fElapsed)
{
}

RwUInt32 CNtlBehaviorCharActivePull::HandleEvents(RWS::CMsg & pMsg)
{
	if (pMsg.Id == g_EventAnimTrigger)
	{
		OnPull();
		Finish();
	}

	NTL_RETURN(NTL_FSM_EVENTRES_PASS);
}

void CNtlBehaviorCharActivePull::SetFinalLoc(RwV3d & loc)
{
	m_vFinalLoc = loc;
}

void CNtlBehaviorCharActivePull::OnPull()
{
	RwUInt32 uiTargetID = Logic_GetActorTargetSerialId(m_pActor);
	CNtlSobActor* pTargetActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiTargetID);
	if (!pTargetActor)
		return;

	pTargetActor->SetPosition(&m_vFinalLoc);
}

