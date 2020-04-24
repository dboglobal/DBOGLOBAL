#pragma once


class CNtlPLCameraRenderTexture;
class CNtlPLEntity;


class CNtlWorldEntityPreview
{
public:
	CNtlWorldEntityPreview(void);
	virtual ~CNtlWorldEntityPreview(void);

private:
	RwTexture*					m_pPreviewTexFrame;
	RwIm2DVertex				m_PreviewTexPanel[4];
	CNtlPLCameraRenderTexture*	m_pR2T;

public:
	RwV3d						m_Rot;
	RwReal						m_Scale;

public:
	RwCamera*	GetCamera();
	VOID		SetRotation(RwV3d& m_Rot);
	VOID		SetCameraAtObj(CNtlPLEntity* pNtlPLEntity);

	VOID Update(CNtlPLEntity* pNtlPLEntity);
	VOID Render();
};