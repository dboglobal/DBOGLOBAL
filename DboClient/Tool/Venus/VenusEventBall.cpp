//***********************************************************************************
//	File		:	VenusEventBall.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venuseventball.h"

#include "VenusVisualManager.h"
#include "VenusConfig.h"

CVenusEventBall::CVenusEventBall(CNtlPLEntity* pEntity, RwV3d& vTargetPosition)
{
	m_fLifeTime	= 0.f;

	m_pEntity = pEntity;

	m_vTargetPosition = vTargetPosition;
}

CVenusEventBall::~CVenusEventBall(void)
{
	CVenusVisualManager::GetInstance().DeleteEventEffect(m_pEntity);
}


//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventBall::Update(RwReal fElapsedTime)
{
	m_fLifeTime += fElapsedTime;
	if (m_fLifeTime > CVenusConfig::GetInstance().m_fEventBallLifeTime)
	{
		return FALSE;
	}

	RwMatrix matWorld = m_pEntity->GetMatrix();

	RwV3d vTargetDirection, vBallDirection, vDirection;
    m_vTargetPosition.y = 1.0f;
    vTargetDirection = m_vTargetPosition - *RwMatrixGetPos(&matWorld);	

	RwReal fLength = RwV3dLength(&vTargetDirection);
	if (fLength < 0.5f)
	{
		return FALSE;
	}
    
	RwV3dNormalize(&vTargetDirection, &vTargetDirection);
    vTargetDirection *= m_fLifeTime * fElapsedTime * CVenusConfig::GetInstance().m_fEventBallSpeed;

	//vBallDirection = *RwMatrixGetAt(&matWorld);
    vBallDirection = ZeroAxis;
    vDirection = vBallDirection + vTargetDirection;
	RwV3dNormalize(&vDirection, &vDirection);

	RwV3d vPosition = m_pEntity->GetPosition();

	*RwMatrixGetAt(&matWorld) = vDirection;
	RwV3dCrossProduct(RwMatrixGetRight(&matWorld), &VEC_Y, RwMatrixGetAt(&matWorld));
	RwV3dCrossProduct(RwMatrixGetUp(&matWorld), RwMatrixGetAt(&matWorld), RwMatrixGetRight(&matWorld));

    vDirection *= CVenusConfig::GetInstance().m_fEventBallSpeed * fElapsedTime;	
    vPosition += vDirection;
    *RwMatrixGetPos(&matWorld) = vPosition;

	m_pEntity->SetMatrix(matWorld);
	m_pEntity->Update(fElapsedTime);

	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusEventBall::Render()
{
	m_pEntity->Render();
	return TRUE;
}