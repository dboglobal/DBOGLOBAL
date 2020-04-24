//***********************************************************************************
//	File		:	CNtlLighting.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\ntllightingcontroller.h"

#include "NtlDebug.h"

CNtlLightingController::CNtlLightingController(void)
{
}

CNtlLightingController::~CNtlLightingController(void)
{
}

CNtlLighting* CNtlLightingController::CreateLighting(RwReal fWitdh, RwReal fDelayTime, RwV3d vStart, RwBool bRandom)
{
	m_pLightning = NTL_NEW CNtlLighting;

	CNtlLighting::sCreateStream Stream;

	Stream.fDelayTime = fDelayTime;
	Stream.strTex = LIGHTNING_TEX_NAME;
	Stream.nLevel = LIGHTNING_LEVEL;
	Stream.nSubCount = LIGHTNING_SUBCOUNT;	
	Stream.fWidth = fWitdh;

	Stream.vStart = vStart;

	Stream.vEnd = Stream.vStart;

	if(bRandom)
	{
		Stream.vEnd.x = CNtlLighting::RandomNumber(Stream.vEnd.x-1, Stream.vEnd.x+1);
		Stream.vEnd.z = CNtlLighting::RandomNumber(Stream.vEnd.z-1, Stream.vEnd.z+1);
	}

	Stream.vEnd.z  = 5.0f;

	Stream.fSRatio	= LIGHTNING_SRATIO;	
	Stream.fERatio	= LIGHTNING_ERATIO;	
	Stream.fAmp		= LIGHTNING_AMP;		
	Stream.fRough	= LIGHTNING_ROUGH;		

	m_pLightning->Create(Stream);

	return m_pLightning;
}

HRESULT CNtlLightingController::CreateData(void)
{
	SetState(LIGHTNING_NONE);

	return S_OK;
}

HRESULT CNtlLightingController::DeleteData(void)
{
	NTL_DELETE(m_pLightning)

	return S_OK;
}

HRESULT CNtlLightingController::FrameMove( RwMatrix& matView, RwReal fElapsedTime )
{
	CNtlLightingStrip::m_matView = matView;

	if( m_nState == LIGHTNING_NONE )
		return S_OK;
	else if( m_nState == LIGHTNING_READY )
	{
		m_fCurrTime += fElapsedTime;
		if(m_fCurrTime >= m_fActiveTime)
			SetState(LIGHTNING_ACTIVE);
	}
	else if( m_nState == LIGHTNING_ACTIVE )
	{
		m_fCurrTime += fElapsedTime;

		m_pLightning->FrameMove(fElapsedTime);

		if(m_fCurrTime >= LIGHTNING_SHOW_TIME)
			SetState(LIGHTNING_DESTROY);
	}
	else
	{
		SetState(LIGHTNING_READY);
	}

	return S_OK;
}


HRESULT CNtlLightingController::Render()
{
	if(m_nState == LIGHTNING_NONE || m_nState == LIGHTNING_READY)
		return S_OK;

	m_pLightning->Render();

	return S_OK;
}

void CNtlLightingController::Active(RwBool bActive, RwUInt32 dwSeed, RwReal fElapsedTime)
{
	if(bActive)
	{
		srand(dwSeed);
		SetState(LIGHTNING_READY);
		m_fActiveTime = 3.f;
	}
	else
		SetState(LIGHTNING_NONE);
}

void CNtlLightingController::EnterState(RwInt32 nState)
{
	if(nState == LIGHTNING_NONE)
	{
		m_fCurrTime = 0.0f;
	}
	else if(nState == LIGHTNING_READY)
	{
		m_fCurrTime = 0.0f;
	}
	else if(nState == LIGHTNING_ACTIVE)
	{
		// 번개 생성.
		m_fCurrTime = 0.0f;

		RwReal fDelayTime;
		CNtlLighting *pLightning;
		// 첫번째 번개.
		fDelayTime = 1.f;

		RwV3d vTemp = {0, 0, 0};
		pLightning = CreateLighting(0.1f, fDelayTime, vTemp, false);
		pLightning->UpdateVertex();
	}
}

void CNtlLightingController::ExitState(RwInt32 nState)
{
	if(nState == LIGHTNING_NONE)
	{
	}
	else if(nState == LIGHTNING_ACTIVE)
	{
	}
	else if(nState == LIGHTNING_DESTROY)
	{
	}
}

void CNtlLightingController::SetState(RwInt32 nState)
{
	if(m_nState != nState)
	{
		ExitState(m_nState);
		EnterState(nState);
		m_nState = nState;
	}
}