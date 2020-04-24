#pragma once
#include "NtlWeController.h"

class CNtlWeControlUi
{
public:
	CNtlWeControlUi(void);
	~CNtlWeControlUi(void);

private:
	CNtlWeController *m_pActiveController;
	
	float	m_fFov;
	float	m_fSpeed;
	float   m_fZoom;

public:
	void	Create();
	void	Update(float fElapsed);
	void	Update() { m_pActiveController->Update(); }
	void	Destroy();

	void	SetActiveWeController(CNtlWeController *pController);
	
	void	SetSpeed(float fSpeed);
	float	GetSpeed() { return m_fSpeed; }
	float	GetZoomDist() { return m_fZoom; }

	void	SetFov(float fFov);
	float	GetFov() { return m_fFov; }
	void	SetZoomDist(float fZoom);
	void	SetPosition(RwV3d *pPos);

	static	CNtlWeControlUi &GetInstance();
};

#define WE_CHARACTER_VIEW