#include "precomp_ntlsimulation.h"
#include "NtlBehaviorProj.h"

//core
#include "NtlMath.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLEntity.h"
#include "NtlPLSceneManager.h"

// simulation
#include "NtlSobActor.h"
#include "NtlSobProjectile.h"
#include "NtlSobManager.h"
#include "NtlSobProxy.h"
#include "NtlSobElapsedController.h"
#include "NtlSlLogic.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

// hisshidan
#define HISSHIDAN_INC_SPEED			13.0f
#define HISSHIDAN_DEFAULT_SPEED		50.0f

// hellzone
#define HELLZONE_INC_SPEED			13.0f
#define HELLZONE_DEFAULT_SPEED		50.0f

// drain
#define STEAL_INC_SPEED				2.0f
#define STEAL_DEFAULT_SPEED			0.5f

// Magare
#define MAGARE_RADIUS               3.0f
#define MAGARE_SPEED                4.0f

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


DEFINITION_MEMORY_POOL(CNtlBehaviorProjBall)

CNtlBehaviorProjBall::CNtlBehaviorProjBall()
{
	SetBehaviorId(NTL_BEID_PROJ_BALL);
	SetBehaviorName(NTL_BENAME_PROJ_BALL);

	m_fTargetHeight = 1.0f;
}

CNtlBehaviorProjBall::~CNtlBehaviorProjBall()
{
}

void CNtlBehaviorProjBall::Enter(void)
{
	RwUInt32 uiTargetSerialId = m_pActor->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor)
	{
		CNtlSobProxy *pSobProxy = pTargetActor->GetSobProxy();
		m_fTargetHeight = pSobProxy->GetPLEntityHeight();
	}
}

void CNtlBehaviorProjBall::Update(RwReal fElapsed)
{
    CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
    RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();

    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
    if(pTargetActor == NULL)
    {
        Finish();
        return;
    }

	RwV3d vPos = pSobProj->GetPosition();
	RwReal fSpeed = pSobProj->GetSpeed();

	RwV3d vDestPos = pTargetActor->GetPosition();
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vDestPos.y += m_fTargetHeight * fTargetHeightRate; 

	RwV3d vDir = vDestPos - vPos;		
	RwV3dNormalize(&vDir, &vDir);

	RwV3d vCurrDir = pSobProj->GetDirection() + vDir * fElapsed;
	RwV3dNormalize(&vCurrDir, &vCurrDir);

    // 현재 남은 거리를 구한다.
    RwV3d vDelta = vCurrDir * fSpeed * fElapsed;	
    RwV3d vNextPos = vPos + vDelta;	
    RwReal fDist = CNtlMath::GetLength(vPos, vDestPos);    

	CNtlPlane plane;	
    vDir = vCurrDir;
	RwV3dNormalize(&vDir, &vDir);
	plane.PlaneFromPointNormal(&vDestPos, &vDir);
	RwReal fDot = plane.DotProduct(&vNextPos); 

	// delta time 동안 이동 거리를 구한다.
	if(fDot >= 0.0f)
	{
		pSobProj->SetPosition(&vDestPos);
		Finish();
		return;
	}

	pSobProj->SetPosition(&vNextPos);
	pSobProj->SetDirection(&vCurrDir);
}

//////////////////////////////////////////////////////////////////////////
DEFINITION_MEMORY_POOL(CNtlBehaviorProjMagare)

CNtlBehaviorProjMagare::CNtlBehaviorProjMagare() 
{
    SetBehaviorId(NTL_BEID_PROJ_MAGARE);
    SetBehaviorName(NTL_BENAME_PROJ_MAGARE);

    m_eStatus = MAGARE_GO;
    m_fTotalAngle = 0.0f;
}

CNtlBehaviorProjMagare::~CNtlBehaviorProjMagare() 
{

}

void CNtlBehaviorProjMagare::Enter() 
{

}

void CNtlBehaviorProjMagare::Update( RwReal fElapsed ) 
{
    CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
    RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();

    CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
    if(pTargetActor == NULL)
    {
        Finish();
        return;
    }
    
    RwV3d vPos = pSobProj->GetPosition();
    RwReal fSpeed = pSobProj->GetSpeed();

    RwV3d vDestPos = pTargetActor->GetPosition();
    RwReal fTargetHeight = pTargetActor->GetSobProxy()->GetPLEntityHeight();
    RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
    vDestPos.y += fTargetHeight * fTargetHeightRate; 

    RwV3d vDir = vDestPos - vPos;		
    RwV3dNormalize(&vDir, &vDir);

    RwV3d vCurrDir = pSobProj->GetDirection() + vDir * fElapsed;
    RwV3dNormalize(&vCurrDir, &vCurrDir);

    // 현재 남은 거리를 구한다.
    RwV3d vDelta = vCurrDir * fSpeed * fElapsed;	
    RwV3d vNextPos = vPos + vDelta;	
    RwReal fDist = CNtlMath::GetLength(vPos, vDestPos);    

    // 뒤로 돌아가는 테스트용
    if(m_eStatus == MAGARE_GO)
    {
        if(fDist <= fTargetHeight * MAGARE_RADIUS)
        {
            m_eStatus = MAGARE_MAGARE;
        }        
    }
    else if(m_eStatus == MAGARE_MAGARE)
    {
        RwReal fAngle = 180.0f * fElapsed * MAGARE_SPEED;
        fAngle = min(fAngle, 180.0f);
        m_fTotalAngle += fAngle;
        RwMatrix matProj;
        RwMatrixSetIdentity(&matProj);
        RwV3d vInvDir = -vDir;        
        vInvDir.y = 0.0f;
        RwV3dNormalize(&vInvDir, &vInvDir);
        CNtlMath::MathRwMatrixAssign(&matProj, &ZeroAxis, &vInvDir);        
        RwMatrixRotate(&matProj, &YAxis, fAngle, rwCOMBINEPOSTCONCAT);
        RwV3d vTansform;
        RwV3dTransformVector(&vTansform, &ZAxis, &matProj);
        vNextPos = vDestPos + (vTansform * fDist); 
        vNextPos.y += (vDestPos.y - vPos.y) * fElapsed * MAGARE_SPEED;

        if(m_fTotalAngle >= 180.0f)
        {
            pSobProj->SetPosition(&vNextPos);
            Finish();            
            return;
        }
    }    

    pSobProj->SetPosition(&vNextPos);
    pSobProj->SetDirection(&vCurrDir);   
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlBehaviorProjBeam)


CNtlBehaviorProjBeam::CNtlBehaviorProjBeam()
{
	SetBehaviorId(NTL_BEID_PROJ_BEAM);
	SetBehaviorName(NTL_BENAME_PROJ_BEAM);
}

CNtlBehaviorProjBeam::~CNtlBehaviorProjBeam()
{
}

void CNtlBehaviorProjBeam::Enter(void)
{
}

void CNtlBehaviorProjBeam::Update(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwV3d vPos = pSobProj->GetPosition();
	RwV3d vDestPos = pSobProj->GetDestPosition();
	RwReal fSpeed = pSobProj->GetSpeed();
	
	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vDestPos, &vPos); 
		
	// 현재 남은 거리를 구한다.
	RwReal fCurrSqaredLen = CNtlMath::MathRwV3dSquaredLength(&vDir);

	RwV3d vDelta;
	RwV3dNormalize(&vDir, &vDir);
	CNtlMath::MathRwV3dAssign(&vDelta, vDir.x*fElapsed*fSpeed, vDir.y*fElapsed*fSpeed, vDir.z*fElapsed*fSpeed);

	// delta time 동안 이동 거리를 구한다.
	if(fCurrSqaredLen < CNtlMath::MathRwV3dSquaredLength(&vDelta))
	{
		m_pActor->SetPosition(&vDestPos);
		Finish();

		// Lua를 연동한다.
		return;
	}
	    
	RwV3dAdd(&vPos, &vPos, &vDelta);  
	m_pActor->SetPosition(&vPos);
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlBehaviorProjHisshidan)

CNtlBehaviorProjHisshidan::CNtlBehaviorProjHisshidan()
{
	SetBehaviorId(NTL_BEID_PROJ_HISSHIDAN);
	SetBehaviorName(NTL_BENAME_PROJ_HISSHIDAN);

	m_fTargetHeight = 0.0f;
	m_bAngleUse		= FALSE;
	m_fAngleX		= 0.0f;
	m_fAngleY		= 0.0f;
}

CNtlBehaviorProjHisshidan::~CNtlBehaviorProjHisshidan()
{
}


void CNtlBehaviorProjHisshidan::SetAngle(RwReal fAngleX, RwReal fAngleY)		
{
	m_bAngleUse = TRUE;
	m_fAngleX	= fAngleX;
	m_fAngleY	= fAngleY;
}

void CNtlBehaviorProjHisshidan::Enter(void)
{
	RwUInt32 uiTargetSerialId = m_pActor->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor)
	{
		CNtlSobProxy *pSobProxy = pTargetActor->GetSobProxy();
		m_fTargetHeight = pSobProxy->GetPLEntityHeight();
	}

	// hissidan projectile 방향 노이즈.
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);

	if(!m_bAngleUse)
	{
		RwBool bLeftHand = pSobProj->GetLeftHand();
		RwV3d vDir = pSobProj->GetDirection();

		RwV3d vRight, vUp;
		RwV3dCrossProduct(&vRight, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dNormalize(&vRight, &vRight);
		RwV3dCrossProduct(&vUp, &vDir, &vRight);
		RwV3dNormalize(&vUp, &vUp);
		RwV3dScale(&vUp, &vUp, -1.0f);

		RwReal fRandValue = NtlRandomNumber(0.3f, 0.7f);
		
		// 오른쪽, 왼쪽 보정.
		if(bLeftHand)
		{
			vDir.x -= vRight.x*fRandValue;
			vDir.y -= vRight.y*fRandValue;
			vDir.z -= vRight.z*fRandValue;
		}
		else
		{
			vDir.x += vRight.x*fRandValue;
			vDir.y += vRight.y*fRandValue;
			vDir.z += vRight.z*fRandValue;
		}

		fRandValue = NtlRandomNumber(-0.2f, 0.5f);

		vDir.x += vUp.x*fRandValue;
		vDir.y += vUp.y*fRandValue;
		vDir.z += vUp.z*fRandValue;

		//vDir = pSobProj->GetDirection();
		
		RwV3dNormalize(&vDir, &vDir);
		pSobProj->SetDirection(&vDir);
	}
	else
	{
		RwV3d vDir = m_pActor->GetDirection();

		RwReal fAxisAngleX, fAxisAngleY, fAngleX, fAngleY;
		CNtlMath::LineToAngleXY(&vDir, fAxisAngleX, fAxisAngleY);

		fAngleX = fAxisAngleX + m_fAngleX;
		fAngleY = fAxisAngleY + m_fAngleY;

		RwMatrix matRot;
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		pSobProj->SetDirection(&matRot.at);
	}
}

void CNtlBehaviorProjHisshidan::Update(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = pSobProj->GetSpeed();

	RwV3d vDestPos = pTargetActor->GetPosition();
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vDestPos.y += m_fTargetHeight * fTargetHeightRate; 

	RwV3d vDir = vDestPos - vPos;	
    RwV3dNormalize(&vDir, &vDir);


	RwV3d vCurrDir = m_pActor->GetDirection();
	RwReal fIncSpeed = HISSHIDAN_INC_SPEED * fSpeed / HISSHIDAN_DEFAULT_SPEED;
    vCurrDir += vDir * fElapsed * fIncSpeed;
	RwV3dNormalize(&vCurrDir, &vCurrDir);

	// 현재 남은 거리를 구한다.	
	RwV3d vNextPos = vPos + (vCurrDir * fElapsed * fSpeed);	

	CNtlPlane plane;
	RwV3dAssignMacro(&vDir, &vCurrDir);
	RwV3dNormalize(&vDir, &vDir);
	plane.PlaneFromPointNormal(&vDestPos, &vDir);
	RwReal fDot = plane.DotProduct(&vNextPos); 
						
	// delta time 동안 이동 거리를 구한다.
	if(fDot >= 0.0f)
	{
		vDestPos.y += NtlRandomNumber(-1.0f, 1.0f);
		m_pActor->SetPosition(&vDestPos);
		Finish();
		return;
	}

	fSpeed += fSpeed*fElapsed;
	pSobProj->SetSpeed(fSpeed); 
	m_pActor->SetPosition(&vNextPos);
    m_pActor->SetDirection(&vCurrDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

CHisshidanPiece::CHisshidanPiece()
{
	m_pActor			= NULL;	
	m_fTargetHeight		= 1.0f;
	m_pEffect			= NULL;
	m_fIncSpeed			= HISSHIDAN_INC_SPEED;
	m_bExplosion		= FALSE;
	m_bFinish			= FALSE;
}

CHisshidanPiece::~CHisshidanPiece()
{
	if(m_pEffect)
	{
		GetSceneManager()->DeleteEntity(m_pEffect);
		m_pEffect = NULL;
	}
}

void CHisshidanPiece::Enter(CNtlSobActor *pActor, RwV3d vPos, RwV3d vCurrDir, const RwChar *pEffName, RwBool bExplosion)
{
	m_pActor = pActor;

	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	m_fSpeed = pSobProj->GetSpeed();

	RwUInt32 uiTargetSerialId = m_pActor->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
    if(!pTargetActor)
        return;
	
	CNtlSobProxy *pSobProxy = pTargetActor->GetSobProxy();
	m_fTargetHeight = pSobProxy->GetPLEntityHeight();	

	RwV3d vSub;
	RwV3d vDestPos = pTargetActor->GetPosition();
	RwV3dSubMacro(&vSub, &vDestPos, &vPos);
	RwReal fDist = RwV3dLength(&vSub);
	RwReal fWeightValue = fDist / 20.0f;
	if(fWeightValue > 1.0f)
		fWeightValue = 1.0f;
	else
		fWeightValue = 1.0f + (1.0f - fWeightValue);

	m_fIncSpeed = m_fIncSpeed * fWeightValue;

	RwV3dAssignMacro(&m_vPos, &vPos);
	RwV3dAssignMacro(&m_vCurrDir, &vCurrDir);

	m_pEffect = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEffName);
	if(m_pEffect == NULL)
		return;

	RwMatrix mat;
	CNtlMath::MathRwMatrixAssign(&mat, &m_vPos, &m_vCurrDir);
	m_pEffect->SetMatrix(mat); 
	
	if(m_pEffect->IsAutoDelete())
	{
		m_pEffect = NULL;
		return;
	}

	m_pEffect->SetSerialID(m_pActor->GetSerialID());
	RwReal fLifeTime;
	if(GetNtlSobElapsedControlManager()->GetControlFactor(reinterpret_cast<CNtlSob*>( m_pActor ), fLifeTime, fWeightValue))
		m_pEffect->CreateWeightElapsedController(fLifeTime, fWeightValue);
}

void CHisshidanPiece::Exit(void)
{
	if(m_bFinish)
		return;

	m_bFinish = TRUE;

	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
		return;

	RwV3d vDestPos = pTargetActor->GetPosition();
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vDestPos.y += m_fTargetHeight * fTargetHeightRate; 

	vDestPos.y += NtlRandomNumber(-1.0f, 1.0f);
	RwV3dAssignMacro(&m_vPos, &vDestPos);

	UpdateVisual();
}

void CHisshidanPiece::UpdateVisual(void)
{
	if(m_pEffect)
	{
		CNtlMath::MathRwMatrixAssign(&m_matTrans, &m_vPos, &m_vCurrDir);
		m_pEffect->SetMatrix(m_matTrans);
	}
}

void CHisshidanPiece::Update(RwReal fElapsed)
{
	if(m_bFinish)
		return;

	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		m_bFinish = TRUE;
		return;
	}
	
	RwReal fSpeed = m_fSpeed;

	RwV3d vDestPos = pTargetActor->GetPosition();
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vDestPos.y += m_fTargetHeight * fTargetHeightRate; 

	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vDestPos, &m_vPos); 

	// 현재 남은 거리를 구한다.
	RwV3dNormalize(&vDir, &vDir);
		
	RwReal fIncSpeed = m_fIncSpeed*fSpeed/HISSHIDAN_DEFAULT_SPEED;

	m_vCurrDir.x += vDir.x*fElapsed*fIncSpeed;
	m_vCurrDir.y += vDir.y*fElapsed*fIncSpeed;
	m_vCurrDir.z += vDir.z*fElapsed*fIncSpeed;

	RwV3dNormalize(&m_vCurrDir, &m_vCurrDir);

	// 현재 남은 거리를 구한다.
	RwV3d vDelta;
	CNtlMath::MathRwV3dAssign(&vDelta, m_vCurrDir.x*fElapsed*fSpeed, m_vCurrDir.y*fElapsed*fSpeed, m_vCurrDir.z*fElapsed*fSpeed);

	RwV3d vNextPos;
	RwV3dAdd(&vNextPos, &m_vPos, &vDelta);  

	CNtlPlane plane;
	RwV3dAssignMacro(&vDir, &m_vCurrDir);
	RwV3dNormalize(&vDir, &vDir);
	plane.PlaneFromPointNormal(&vDestPos, &vDir);
	RwReal fDot = plane.DotProduct(&vNextPos); 
						
	// delta time 동안 이동 거리를 구한다.
	if(fDot >= 0.0f)
	{
		vDestPos.y += NtlRandomNumber(-1.0f, 1.0f);
		RwV3dAssignMacro(&m_vPos, &vDestPos);
		UpdateVisual();
		m_bFinish = TRUE;
		return;
	}

	m_fSpeed += m_fSpeed*fElapsed;
	RwV3dAssignMacro(&m_vPos, &vNextPos);

	UpdateVisual();
}


RwBool CHisshidanPiece::IsFinish(void) const
{
	return m_bFinish;
}




DEFINITION_MEMORY_POOL(CNtlBehaviorProjMultiHisshidan)

CNtlBehaviorProjMultiHisshidan::CNtlBehaviorProjMultiHisshidan()
{
	m_bExplosion = FALSE;
}

CNtlBehaviorProjMultiHisshidan::~CNtlBehaviorProjMultiHisshidan()
{
	CHisshidanPiece *pPiece;

	ListPiece::iterator it;

	for(it = m_listPiece.begin(); it != m_listPiece.end(); it++)
	{
		pPiece = (*it);
		NTL_DELETE(pPiece);
	}

	m_listPiece.clear();
}

void CNtlBehaviorProjMultiHisshidan::SetPieceStuff(std::list<SHisshidanPieceSutff>& listPieceStuff)
{
	std::list<SHisshidanPieceSutff>::iterator it;
	for(it = listPieceStuff.begin(); it != listPieceStuff.end(); it++)
	{
		m_listPieceStuff.push_back( (*it) );
	}
}


void CNtlBehaviorProjMultiHisshidan::Enter(void)
{
	if(m_listPieceStuff.size() == 0)
		return;

	RwInt32 iExplosionIdx = m_listPieceStuff.size() / 2;

	RwV3d vDir = m_pActor->GetDirection();

	RwReal fAxisAngleX, fAxisAngleY, fAngleX, fAngleY;
	CNtlMath::LineToAngleXY(&vDir, fAxisAngleX, fAxisAngleY);

	RwInt32		iIdx = 0;
	RwBool		bExplosion;
	RwV3d		vCurrDir;
	RwMatrix	matRot;
	
	ListPieceStuff::iterator it;

	for(it = m_listPieceStuff.begin(); it != m_listPieceStuff.end(); it++)
	{
		CHisshidanPiece *pPiece = NTL_NEW CHisshidanPiece;
		RwV3dAssignMacro(&vCurrDir, &vDir);

		fAngleX = fAxisAngleX + (*it).fAngleX;
		fAngleY = fAxisAngleY + (*it).fAngleY;

		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vXAxisV3, fAngleX, rwCOMBINEREPLACE);
		RwMatrixRotate(&matRot, &CNtlPLGlobal::m_vYAxisV3, fAngleY, rwCOMBINEPOSTCONCAT);

		if(iIdx == iExplosionIdx)
			bExplosion = TRUE;
		else
			bExplosion = FALSE;

		iIdx++;

		pPiece->Enter(m_pActor, m_pActor->GetPosition(), matRot.at, (*it).pEffName, bExplosion);

		m_listPiece.push_back(pPiece);
	}
}

void CNtlBehaviorProjMultiHisshidan::Update(RwReal fElapsed)
{
	if(m_listPiece.size() == 0)
	{
		Finish();
		return;
	}

	CHisshidanPiece *pPiece;
	ListPiece::iterator it;

	for(it = m_listPiece.begin(); it != m_listPiece.end(); )
	{
		pPiece = (*it);
		pPiece->Update(fElapsed);

		if(pPiece->IsFinish())
		{
			if(pPiece->m_bExplosion)
			{
				m_bExplosion = TRUE;
			}

			NTL_DELETE(pPiece);

			it = m_listPiece.erase(it);
		}
		else
		{
			it++;
		}
	}

	if(m_listPiece.size() == 0)
		Finish();
}


RwBool CNtlBehaviorProjMultiHisshidan::IsExplosion(void)
{
	return m_bExplosion;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlBehaviorProjHelljone)

CNtlBehaviorProjHelljone::CNtlBehaviorProjHelljone()
{
	SetBehaviorId(NTL_BEID_PROJ_HELLZONE_GRENADE);
	SetBehaviorName(NTL_BENAME_PROJ_HELLZONE_GRENADE);

	m_fTargetHeight		= 1.0f;
	m_fDelayTime		= 1.0f;
	m_fCurrTime			= 0.0f;
	m_fSeqTime			= 0.0f;
	CNtlMath::MathRwV3dAssign(&m_vCreateOffset, 0.0f, 0.0f, 0.0f);
	m_byHelljoneState	= PROJ_HELLJONE_WAIT;
	m_pSeq				= NULL;
}

CNtlBehaviorProjHelljone::~CNtlBehaviorProjHelljone()
{
	CNtlTimeSequence *pTempSeq;
	CNtlTimeSequence *pSeq = m_pSeq;
	while(pSeq)
	{
		pTempSeq = pSeq->GetNext();

		NTL_DELETE( pSeq );

		pSeq = pTempSeq;
	}
}

void CNtlBehaviorProjHelljone::Enter(void)
{
	RwUInt32 uiTargetSerialId = m_pActor->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	CNtlSobProxy *pSobProxy = pTargetActor->GetSobProxy();
	m_fTargetHeight = pSobProxy->GetPLEntityHeight();

	// hellzone init position setting
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	
	RwV3d vTarPos = pTargetActor->GetPosition();
	RwV3dAdd(&m_fBasePos, &vTarPos, &m_vCreateOffset);
	pSobProj->SetPosition(&m_fBasePos);

	RwV3d vDir;
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vTarPos.y += m_fTargetHeight * fTargetHeightRate; 
	RwV3dSubMacro(&vDir, &vTarPos, &m_fBasePos);
	RwV3dNormalize(&vDir, &vDir);
	pSobProj->SetDirection(&vDir);

#define KNOCKDOWN_FALLUP_SEQ_CNT	12

	RwReal arrTime[KNOCKDOWN_FALLUP_SEQ_CNT]  = { 0.083f, 0.168f, 0.25f,  0.333f, 0.417f, 0.5f,   0.583f, 0.667f, 0.75f,  0.833f, 0.917f, 1.0f };
	RwReal arrDistX[KNOCKDOWN_FALLUP_SEQ_CNT] = { 0.0f,	  0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f };
	RwReal arrDistY[KNOCKDOWN_FALLUP_SEQ_CNT] = { 0.150f, 0.250f, 0.300f, 0.250f, 0.150f, 0.000f, -0.15f, -0.25f, -0.30f, -0.25f, -0.15f, 0.000f };

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

void CNtlBehaviorProjHelljone::UpdateWait(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	m_fCurrTime += fElapsed;
	m_fSeqTime	+= fElapsed*0.35f;

	if(m_fCurrTime >= m_fDelayTime)
	{
		m_byHelljoneState = PROJ_HELLJONE_LOOP;
		return; 
	}

	while(1)
	{
		if(m_fSeqTime >= 1.0f)
			m_fSeqTime -= 1.0f;
		else
			break;
	}

	if(m_pSeq)
	{
		CNtl2DTimeSequence::S2DDist s2DDist = *(reinterpret_cast<CNtl2DTimeSequence::S2DDist*>(m_pSeq->GetData(m_fSeqTime)));

		RwV3d vPos;
		CNtlMath::MathRwV3dAssign(&vPos, m_fBasePos.x, m_fBasePos.y + s2DDist.fY, m_fBasePos.z);

		pSobProj->SetPosition(&vPos);
	}
}

void CNtlBehaviorProjHelljone::UpdateLoop(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = pSobProj->GetSpeed();

	RwV3d vDestPos = pTargetActor->GetPosition();
	RwReal fTargetHeightRate = pSobProj->GetHitTargetHeight();
	vDestPos.y += m_fTargetHeight * fTargetHeightRate; 

	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

	// 현재 남은 거리를 구한다.
	RwV3dNormalize(&vDir, &vDir);

	RwV3d vCurrDir = m_pActor->GetDirection();
		
	RwReal fIncSpeed = HELLZONE_INC_SPEED*fSpeed/HELLZONE_DEFAULT_SPEED;

	vCurrDir.x += vDir.x*fElapsed*fIncSpeed;
	vCurrDir.y += vDir.y*fElapsed*fIncSpeed;
	vCurrDir.z += vDir.z*fElapsed*fIncSpeed;

	RwV3dNormalize(&vCurrDir, &vCurrDir);

	// 현재 남은 거리를 구한다.
	RwV3d vDelta;
	CNtlMath::MathRwV3dAssign(&vDelta, vCurrDir.x*fElapsed*fSpeed, vCurrDir.y*fElapsed*fSpeed, vCurrDir.z*fElapsed*fSpeed);

	RwV3d vNextPos;
	RwV3dAdd(&vNextPos, &vPos, &vDelta);  

	CNtlPlane plane;
	RwV3dAssignMacro(&vDir, &vCurrDir);
	RwV3dNormalize(&vDir, &vDir);
	plane.PlaneFromPointNormal(&vDestPos, &vDir);
	RwReal fDot = plane.DotProduct(&vNextPos); 
						
	// delta time 동안 이동 거리를 구한다.
	if(fDot >= 0.0f)
	{
		vDestPos.y += NtlRandomNumber(-0.5f, 0.5f);
		m_pActor->SetPosition(&vDestPos);
		Finish();
		return;
	}

	fSpeed += fSpeed*fElapsed;
	pSobProj->SetSpeed(fSpeed); 
	m_pActor->SetPosition(&vNextPos);
    m_pActor->SetDirection(&vCurrDir);
}

void CNtlBehaviorProjHelljone::Update(RwReal fElapsed)
{
	if(m_byHelljoneState == PROJ_HELLJONE_WAIT)
		UpdateWait(fElapsed);
	else
		UpdateLoop(fElapsed);
}


void CNtlBehaviorProjHelljone::SetCreateTargetOffset(RwV3d vOffset)
{
	m_vCreateOffset = vOffset;
}

void CNtlBehaviorProjHelljone::SetDelayTime(RwReal fDelayTime)
{
	m_fDelayTime = fDelayTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlBehaviorProjSteal)

RwReal CNtlBehaviorProjSteal::m_ffIncSpeed		= STEAL_INC_SPEED;
RwReal CNtlBehaviorProjSteal::m_fWaitCheckTime	= 3.0f;


CNtlBehaviorProjSteal::CNtlBehaviorProjSteal()
{
	SetBehaviorId(NTL_BEID_PROJ_STEAL);
	SetBehaviorName(NTL_BENAME_PROJ_STEAL);

	m_fTargetHeight = 1.0f;
	m_fWaitTime		= 0.0f;
	m_byStealState	= PROJ_STEAL_WAIT;
}

CNtlBehaviorProjSteal::~CNtlBehaviorProjSteal()
{
}


void CNtlBehaviorProjSteal::Enter(void)
{
	RwUInt32 uiTargetSerialId = m_pActor->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	// hissidan projectile 방향 노이즈.
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwV3d vDir = pSobProj->GetDirection();
	pSobProj->SetSpeed(STEAL_DEFAULT_SPEED);

	CNtlSobActor *pOwnerActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(m_pActor->GetOwnerID()));
	if(pOwnerActor == NULL)
	{
		Finish();
		return;
	}

	CNtlSobProxy *pSobProxy = pOwnerActor->GetSobProxy();
	RwReal fOwerHeight = pSobProxy->GetPLEntityHeight();
	RwV3d vPos = m_pActor->GetPosition();
	vPos.y += fOwerHeight * 0.9f; 

	pSobProxy = pTargetActor->GetSobProxy();
	m_fTargetHeight = pSobProxy->GetPLEntityHeight();

	/*
	RwV3d vRight, vUp;
	RwV3dCrossProduct(&vRight, &vDir, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&vRight, &vRight);
	RwV3dCrossProduct(&vUp, &vDir, &vRight);
	RwV3dNormalize(&vUp, &vUp);
	RwV3dScale(&vUp, &vUp, -1.0f);
	*/

	CNtlMath::MathRwV3dAssign(&vDir, 0.0f, 1.0f, 0.0f);
	pSobProj->SetPosition(&vPos);
	pSobProj->SetDirection(&vDir);
}

void CNtlBehaviorProjSteal::UpdateWait(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);

	m_fWaitTime += fElapsed;
	if(m_fWaitTime >= m_fWaitCheckTime)
	{
		m_byStealState = PROJ_STEAL_LOOP;
		RwReal fSpeed = pSobProj->GetSpeed();
		pSobProj->SetSpeed(fSpeed/2.0f);
		return;
	}

	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = pSobProj->GetSpeed();
	
	fSpeed += fSpeed*fElapsed*0.5f;
	vPos.y += fSpeed*fElapsed*0.5f;

	pSobProj->SetSpeed(fSpeed); 
	m_pActor->SetPosition(&vPos);
}

void CNtlBehaviorProjSteal::UpdateLoop(RwReal fElapsed)
{
	CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(m_pActor);
	RwUInt32 uiTargetSerialId = pSobProj->GetTargetID();
	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSerialId));
	if(pTargetActor == NULL)
	{
		Finish();
		return;
	}

	RwV3d vPos = m_pActor->GetPosition();
	RwReal fSpeed = pSobProj->GetSpeed();

	RwV3d vDestPos = pTargetActor->GetPosition();
	vDestPos.y += m_fTargetHeight * 0.5f; 

	RwV3d vDir;
	RwV3dSubMacro(&vDir, &vDestPos, &vPos); 

	// 현재 남은 거리를 구한다.
	RwReal fCurrLen = RwV3dLength(&vDir);
	RwV3dNormalize(&vDir, &vDir);

	RwV3d vCurrDir = m_pActor->GetDirection();

	RwReal fIncSpeed = fSpeed*m_ffIncSpeed*fElapsed;
		
	vCurrDir.x += vDir.x*fIncSpeed;
	vCurrDir.y += vDir.y*fIncSpeed;
	vCurrDir.z += vDir.z*fIncSpeed;

	RwV3dNormalize(&vCurrDir, &vCurrDir);

	// 현재 남은 거리를 구한다.
	RwV3d vDelta;
	CNtlMath::MathRwV3dAssign(&vDelta, vCurrDir.x*fElapsed*fSpeed, vCurrDir.y*fElapsed*fSpeed, vCurrDir.z*fElapsed*fSpeed);

	RwV3d vNextPos;
	RwV3dAdd(&vNextPos, &vPos, &vDelta);  

	CNtlPlane plane;
	RwV3dAssignMacro(&vDir, &vCurrDir);
	RwV3dNormalize(&vDir, &vDir);

	if(vDir.y > 0.0f)
		vDestPos.y = vNextPos.y + 0.01f;
	else
		vDestPos.y = vNextPos.y - 0.01f;

	plane.PlaneFromPointNormal(&vDestPos, &vDir);
	RwReal fDot = plane.DotProduct(&vNextPos); 
						
	// delta time 동안 이동 거리를 구한다.
	if(fDot >= 0.0f)
	{
		m_pActor->SetPosition(&vDestPos);
		Finish();
		return;
	}

	fSpeed += fSpeed*fElapsed;
	pSobProj->SetSpeed(fSpeed); 
	m_pActor->SetPosition(&vNextPos);
    m_pActor->SetDirection(&vCurrDir);
}

void CNtlBehaviorProjSteal::Update(RwReal fElapsed)
{
	if(m_byStealState == PROJ_STEAL_WAIT)
		UpdateWait(fElapsed);
	else
		UpdateLoop(fElapsed);
}

