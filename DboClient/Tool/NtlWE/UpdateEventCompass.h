#pragma once

class CUpdateEventCompass
{
public:
	CUpdateEventCompass();
	virtual ~CUpdateEventCompass();

	virtual void CreateCompass();
	virtual void UpdateCompass();
	virtual void RenderCompass();
	virtual void RestoreCompass();

private:
	virtual RwCamera* CreateTextureCamera(RpWorld* world);


private:
	RwCamera*				m_pCompassCam;
	RwTexture*				m_pCompassTex;
	RwTexture*				m_pCompassTexR2S;
	RwIm2DVertex			m_CompassVert[4];
	RwIm3DVertex			m_CompassGeom[4];
};