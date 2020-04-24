#include "precomp_ntlsimulation.h"
#include "NtlSobProjectile.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlInstanceEffect.h"
#include "NtlPLSceneManager.h"
#include "NtlPLItem.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobActorCtrlData.h"
#include "NtlFSMDef.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlBehaviorBase.h"
#include "NtlBehaviorProj.h"
#include "NtlBehaviorData.h"
#include "NtlSLEventFunc.h"
#include "NtlSlLuaGlue.h"
#include "NtlSLLogic.h"
#include "NtlSobElapsedController.h"
#include "NtlCameraController.h"

DEFINITION_MEMORY_POOL(CNtlSobProjectile)

CNtlSobProjectile::CNtlSobProjectile()
{
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_LUA_INTERLOCKING | SLFLAG_LUA_TRANSFORM_ENABLE);

	m_fSpeed			= 10.0f;
	m_bLeftHand			= TRUE;
	m_fTargetLocHeight	= 0.7f;
	CNtlMath::MathRwV3dAssign(&m_vDest, 0.0f, 0.0f, 0.0f);

	m_bDamageProc		= FALSE;
	m_bFinish			= FALSE;
}

CNtlSobProjectile::~CNtlSobProjectile()
{
	m_bFinish = FALSE;
}

RwBool CNtlSobProjectile::Create(void)
{
	NTL_FUNCTION("CNtlSobProjectile::Create");
	
	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if(!CNtlSobActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_PROJECTILE);
		
	NTL_RETURN(TRUE);
}

void CNtlSobProjectile::Destroy(void)
{
	NTL_FUNCTION("CNtlSobProjectile::Destroy");

	// actor destroy

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

CNtlFSMLayer* CNtlSobProjectile::CreateFSM(void)
{
	return NULL;
}

CNtlSobInfluence* CNtlSobProjectile::CreateSobInfluence(void)
{
	return NULL;
}

void CNtlSobProjectile::Update(RwReal fElapsed)
{
	CNtlSobActor::Update(fElapsed);
	RwBool bFinish = UpdateBehavior(fElapsed);

	if(bFinish)
	{
		m_bFinish = TRUE;
		DamageProc();
		return;
	}
	
	UpdateVisual();
}

RwBool CNtlSobProjectile::IsFinish(void)
{
	return m_bFinish;
}

void CNtlSobProjectile::HandleEvents(RWS::CMsg &pMsg)
{
	//Fsm Handler로 Event도 보내준다.
	CNtlSobActor::HandleEvents(pMsg); 

	if(pMsg.Id == g_EventSobCreate)
	{
		CreateEventHandler(pMsg);
	}
}

RwBool CNtlSobProjectile::UpdateBehavior(RwReal fElapsed)
{
	if(m_listBehavior.size() == 0)
		return TRUE;

	std::list<CNtlBehaviorBase*>::iterator it = m_listBehavior.begin();
	CNtlBehaviorBase *pBehavior = (*it);
	pBehavior->Update(fElapsed);

	CNtlBehaviorProjectile *pProjBehavior = (CNtlBehaviorProjectile*)pBehavior;
	if(pProjBehavior->IsExplosion())
	{
		DamageProc();
	}

	if(pBehavior->IsFinish())
	{
		pBehavior->Exit(); 

		NTL_DELETE( pBehavior );

		m_listBehavior.erase(it);

		if(m_listBehavior.size() <= 0)
		{
			return TRUE;
		}
		else
		{
			it = m_listBehavior.begin();
			pBehavior = (*it);
			pBehavior->Enter(); 
		}
	}

	return FALSE;
}

void CNtlSobProjectile::UpdateVisual(void)
{
	CNtlMath::MathRwMatrixAssign(&m_matTrans, &m_vPos, &m_vDirection);
	GetSobProxy()->TransformPLChildEntity(m_matTrans);
}

void CNtlSobProjectile::CreateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobProjectileCreate *pSobCreate = reinterpret_cast<SNtlEventSobProjectileCreate*>(pMsg.pData);

	// When firing toward yourself.
	if(pSobCreate->hOwnerSerialId == pSobCreate->hTargetSerialId)
		return;

	//-----------------------------------------
	// attribute 설정.

	SetOwnerID(pSobCreate->hOwnerSerialId);
	SetTargetID(pSobCreate->hTargetSerialId); 
	SetSpeed(pSobCreate->fSpeed);
	SetLeftHand(pSobCreate->bLeftHand);

	//-----------------------------------------
	// position setting

	CNtlSob *pOwnerSobObj = GetNtlSobManager()->GetSobObject(pSobCreate->hOwnerSerialId);
	CNtlSob *pTarSobObj = GetNtlSobManager()->GetSobObject(pSobCreate->hTargetSerialId);

	if(pOwnerSobObj == NULL || pTarSobObj == NULL)
		return;

	// visible Determination of presence or absence.
	if(!pOwnerSobObj->IsVisible())	// maybe this makes the effect invisible for other people TO DO BY DANEOS
		EnableVisible(FALSE);

	RwV3d vPos = pOwnerSobObj->GetPosition();
	RwV3d vTarPos = pTarSobObj->GetPosition();

	CNtlSobProxy *pSobProxy = pOwnerSobObj->GetSobProxy();
	CNtlSobProxy *pSobTarProxy = pTarSobObj->GetSobProxy();
	
	RwReal fTargetOffset = pSobTarProxy->GetPLEntityHeight() * pSobCreate->fTargetHeight;
	vTarPos.y += fTargetOffset;
			
	// If you're wearing a weapon.
	// In case of staff, the projectile is fired from the staff.
	
	if(pSobCreate->byShotType == BONE_WEAPON)
	{
		sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(pOwnerSobObj);
		if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		{
			vPos = pSobProxy->GetWeaponBonePosition(pSobCreate->chBoneName);
		}
	}
	else if(pSobCreate->byShotType == BONE_SUB_WEAPON)
	{
		vPos = pSobProxy->GetSubWeaponBonePosition(pSobCreate->chBoneName);
	}
	else
	{
		if(pSobCreate->chBoneName && strlen(pSobCreate->chBoneName) > 0)
			vPos = pSobProxy->GetBonePosition(pSobCreate->chBoneName);
	}
	
	SetPosition(&vPos);

	//-----------------------------------------
	// direction setting

	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vTarPos, &vPos);
	
	if(pTarSobObj && pSobCreate->uiBehavior == NTL_BEID_PROJ_BEAM)
	{
		// beam 형태의 발사체는 거리가 가까이 있을 때 target 중심을 향할 경우 beam이 꺽여서 
		// 이상하게 보일 수 있다. 이것에 대한 보정.
		RwV3dSubMacro(&vDir, &vTarPos, &vPos);
		vDir.y = 0.0f;
		RwReal fLen = RwV3dLength(&vDir);
	
		RwReal fSubHeight = fabs(vTarPos.y - vPos.y);

		if(fLen < 2.0f && fSubHeight < 2.0f)
		{
			vDir = pOwnerSobObj->GetDirection();
			vTarPos.y = vPos.y;
		}
		else
			vDir.y = vTarPos.y - vPos.y;

		RwV3dAssignMacro(&m_vDest, &vTarPos);
	}

	RwV3dNormalize(&vDir, &vDir);
	SetDirection(&vDir);

	//-----------------------------------------
	// behavior setting

	if(pTarSobObj)
	{
		if(pSobCreate->uiBehavior == NTL_BEID_PROJ_BALL)
		{
			CNtlBehaviorProjBall *pProjBall = NTL_NEW CNtlBehaviorProjBall;
			pProjBall->SetActor(this); 
			AddBehavior(pProjBall);
			pProjBall->Enter(); 
		}
        else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_MAGARE)
        {
            CNtlBehaviorProjMagare* pProjMagare = NTL_NEW CNtlBehaviorProjMagare();
            pProjMagare->SetActor(this);
            AddBehavior(pProjMagare);
            pProjMagare->Enter();
        }
		else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_BEAM)
		{
			CNtlBehaviorProjBeam *pProjBeam = NTL_NEW CNtlBehaviorProjBeam;
			pProjBeam->SetActor(this); 
			AddBehavior(pProjBeam);
			pProjBeam->Enter(); 
		}
		else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_HISSHIDAN)
		{
			CNtlBehaviorProjHisshidan *pProjHisshidan = NTL_NEW CNtlBehaviorProjHisshidan;
			pProjHisshidan->SetActor(this); 
			UEffectTypeExtraData *pExtData = (UEffectTypeExtraData*)pSobCreate->pProjExtData;
			if(pExtData->hissidanData.bApplyAngle)
				pProjHisshidan->SetAngle(pExtData->hissidanData.v2dAngle.x, pExtData->hissidanData.v2dAngle.y);		

			AddBehavior(pProjHisshidan);
			pProjHisshidan->Enter(); 
		}
		else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_HELLZONE_GRENADE)
		{
			CNtlBehaviorProjHelljone *pProjHelljone = NTL_NEW CNtlBehaviorProjHelljone;
			pProjHelljone->SetActor(this); 
			AddBehavior(pProjHelljone);
			
			UEffectTypeExtraData *pExtData = (UEffectTypeExtraData*)pSobCreate->pProjExtData;
			pProjHelljone->SetCreateTargetOffset(pExtData->hellZoneData.vTargetEffectOffset);
			pProjHelljone->SetDelayTime(pExtData->hellZoneData.fTargetEffectStartWaitTime);

			pProjHelljone->Enter(); 
		}
		else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_MULTI_HISSHIDAN)
		{
			CNtlBehaviorProjMultiHisshidan *pHisshidan = NTL_NEW CNtlBehaviorProjMultiHisshidan;
			pHisshidan->SetActor(this);
			AddBehavior(pHisshidan);
			
			SHisshidanPieceSutff sStuff;
			std::list<SHisshidanPieceSutff> listPieceStuff;

			UEffectTypeExtraData *pExtData = (UEffectTypeExtraData*)pSobCreate->pProjExtData;

			SMultiHissidanData *pMultiHissidanData = &pExtData->multiHissidanData;
			for(RwInt32 i = 0; i < pMultiHissidanData->nCount; i++)
			{
				sStuff.fAngleX = pMultiHissidanData->pArrayAngle[i].x;
				sStuff.fAngleY = pMultiHissidanData->pArrayAngle[i].y;
				
				if(pSobCreate->bCreateHitStuffEffect)
				{
					sStuff.pEffName = pSobCreate->chEffName;
				}
				else
				{
					sStuff.pEffName = NULL;
				}

				listPieceStuff.push_back(sStuff);
			}

			pHisshidan->SetPieceStuff(listPieceStuff);
			pHisshidan->Enter(); 
		}
		else if(pSobCreate->uiBehavior == NTL_BEID_PROJ_STEAL)
		{
			CNtlBehaviorProjSteal *pProjSteal = NTL_NEW CNtlBehaviorProjSteal;
			pProjSteal->SetActor(this); 
			AddBehavior(pProjSteal);
			pProjSteal->Enter(); 
		}
	}

	//-----------------------------------------
	// effect 설정.

	if( pSobCreate->fWeightElapsedValue < 0.95f || pSobCreate->fWeightElapsedValue > 1.05f )
	{
		GetNtlSobElapsedControlManager()->CreateController(this, 100.0f, pSobCreate->fWeightElapsedValue);
	}

	if(pSobCreate->bCreateHitStuffEffect && pSobCreate->uiBehavior != NTL_BEID_PROJ_MULTI_HISSHIDAN)
	{
		GetSobProxy()->CreatePLChildEffect(pSobCreate->chEffName);
	}

	//-----------------------------------------
	// elapsed controller setting
	GetNtlSobElapsedControlManager()->CreateController(this, pOwnerSobObj);
}

void CNtlSobProjectile::DamageProc(void)
{
	if(m_bDamageProc)
		return;

	SERIAL_HANDLE hAvatarSerialId = Logic_GetAvatarHandle();

	RwInt32 iIdx = 0;

	// by daneos: Disabled the knockdown cinematic
	//RwBool bMatrixDirect = FALSE;

	std::list<SHitStuff>::iterator it;
	for(it = m_listHitStuff.begin(); it != m_listHitStuff.end(); ++it)
	{
		SHitStuff sHit = (*it);

		//// matrix 연출.
		//if(iIdx == 0)
		//{
		//	CNtlSob *pOwnerSobObj = GetNtlSobManager()->GetSobObject(GetOwnerID());
		//	if(pOwnerSobObj && pOwnerSobObj->GetClassID() == SLCLASS_AVATAR && 
		//		sHit.uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
		//	{
		//		bMatrixDirect = Logic_ProcKnockdownMatrixDirection(pOwnerSobObj, sHit.hDefenderSerialId, &sHit);
		//	}
		//}

		//// avatar가 공격 했을 경우.
		//if(bMatrixDirect)
		//{
		//	CNtlSob *pSobTarObj = GetNtlSobManager()->GetSobObject(sHit.hDefenderSerialId);
		//	if(pSobTarObj)
		//	{
		//		GetNtlSobElapsedControlManager()->CreateController(pSobTarObj, g_fMatrixTime, g_fMatrixValue, g_fMatrixDelayTime);
		//	}
		//}

		//// avatar가 맞았을 경우.
		//if(sHit.hDefenderSerialId == hAvatarSerialId && sHit.uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN &&
  //         sHit.uiAttr.bitResult != NTL_ATTACK_RESULT_STEAL)
		//{
		//	Logic_ProcKnockdownMatrixDirection(Logic_GetAvatarActor(), hAvatarSerialId, &sHit);
		//}

		// 카메라 흔들기.
		CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(sHit.hAttackerSerialId);
		if(pSobObj && sHit.uiAttr.bitResult != NTL_ATTACK_RESULT_DODGE)
		{
			if(sHit.bCameraShake && pSobObj->GetFlags() & SLFLAG_CAMERASHAKE_ENABLE)
            {
                CNtlCameraShakeController::SetShakeFactor(sHit.fShakeFactor, sHit.fShakeMaxHeight);
				CNtlSLEventGenerator::CameraShake(pSobObj->GetSerialID(), TRUE);
            }
		}

		CNtlSLEventGenerator::SobHit(sHit.hDefenderSerialId, &sHit);
		LuaExec_Attack(sHit.hAttackerSerialId, sHit.hDefenderSerialId, &sHit);

		iIdx++;
	}

	m_bDamageProc = TRUE;
}

void CNtlSobProjectile::AddHit(SHitStuff& sHit)
{
	if(m_listHitStuff.size() == 0)
	{
		m_fTargetLocHeight = sHit.fTargetLocHeight;
	}

	m_listHitStuff.push_back(sHit); 
}

RwInt32 CNtlSobProjectile::GetHitCount(void) const
{
	return (RwInt32)m_listHitStuff.size();
}

RwReal CNtlSobProjectile::GetHitTargetHeight(void)
{
	return m_fTargetLocHeight;
}

void CNtlSobProjectile::AddBehavior(CNtlBehaviorBase *pBehavior)
{
	m_listBehavior.push_back(pBehavior); 
}


RwInt32 CNtlSobProjectile::GetBehaviorCount(void) const
{
	return (RwInt32)m_listBehavior.size();
}



