#include "StdAfx.h"
#include "NtlDebug.h"
#include "NtlWeControlUi.h"

CNtlWeControlUi::CNtlWeControlUi(void) : m_pActiveController(NULL),
										 m_fFov(60.f),
										 m_fSpeed(10.f)
{
}

CNtlWeControlUi::~CNtlWeControlUi(void)
{

}

void CNtlWeControlUi::Create()
{
	
}

void CNtlWeControlUi::Update(float fElapsed)
{
	if(m_pActiveController)
	{
		m_pActiveController->UpdateControl(fElapsed);
	}
}

void CNtlWeControlUi::Destroy()
{
	if (m_pActiveController)
	{
		m_pActiveController->SetActive(false);
		m_pActiveController = NULL;
	}
}

void CNtlWeControlUi::SetActiveWeController(CNtlWeController *pController)
{
	NTL_ASSERTE(pController != NULL);

	// same camera
	if(m_pActiveController == pController)
	{
		return;
	}

	if(m_pActiveController)
		m_pActiveController->SetActive(false);
	
	pController->SetActive(true);
	m_pActiveController = pController;

	if(m_pActiveController)
	{
		m_pActiveController->SetSpeed(m_fSpeed);
		m_pActiveController->SetFov(m_fFov);
		m_pActiveController->SetZoomDist(m_fZoom);
		m_pActiveController->Init();
	}
}

CNtlWeControlUi &CNtlWeControlUi::GetInstance()
{
	static CNtlWeControlUi	s_WeControlUi;
	return s_WeControlUi;
}

void CNtlWeControlUi::SetSpeed(float fSpeed)
{
	if(m_pActiveController)
	{
		m_fSpeed = fSpeed;
		if( m_fSpeed < 0.1f)
			m_fSpeed = 0.1f;

		m_pActiveController->SetSpeed(m_fSpeed);
		dGET_WORLD_PARAM()->WorldCamSpeedPerSec = m_fSpeed;
	}
}

void CNtlWeControlUi::SetFov(float fFov)
{
	if(m_pActiveController)
	{
		m_fFov = fFov;
		m_pActiveController->SetFov(m_fFov);
	}
}

void CNtlWeControlUi::SetZoomDist(float fZoom)
{
	if(m_pActiveController)
	{
		m_fZoom = fZoom;
		m_pActiveController->SetZoomDist(fZoom);
	}
}

void CNtlWeControlUi::SetPosition(RwV3d *pPos)
{
	if(m_pActiveController)
	{
		m_pActiveController->SetPosition(pPos);
	}
}