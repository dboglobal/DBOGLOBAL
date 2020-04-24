#pragma once


class CNtlWeController
{
public:
	CNtlWeController(void);
	virtual ~CNtlWeController(void);

protected:
	bool	m_bActive;

public:
	virtual VOID Init() = 0;

	virtual void Update() { return; }
	virtual void UpdateControl(float fElapsed) = 0;
	virtual void DestroyControl(void) = 0;

	virtual void	SetActive(bool bFlag) { m_bActive = bFlag; }
	virtual void	SetFov(float fFov) = 0;
	virtual void	SetSpeed(float fSpeed) = 0;
	virtual void	SetZoomDist(float fZoom) = 0;
	virtual void	SetPosition(RwV3d *pPos) { return; }
};
