/******************************************************************************
* File			: SurfaceCamera.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 1. 21
* Abstract		: 
*****************************************************************************
* Desc			: SurfaceGui용 Render texture camera
*****************************************************************************/
#pragma once

#include "SurfaceGui.h"

class CSurfaceCamera
{
public:
	CSurfaceCamera();
	CSurfaceCamera(const CSurfaceCamera& camera) {}
	virtual ~CSurfaceCamera();

	VOID			Create(RwInt32 iWidth, RwInt32 iHeight);
	VOID			Destroy();

	RwBool			CameraBeginUpdate();			///< t : 카메라 텍스처에 랜더링할 준비가 되었다
	VOID			CameraEndUpdate();

	VOID			CreateTexture();				///< gui::CTexture를 생성한다
	gui::CTexture*	GetTexture();

protected:
	RwCamera*				m_pCamera;				///< Raster camera
	RwTexture*				m_pRenderTexture;		///< Render target texture

	gui::CTexture			m_GUITexture;			///< Dbo Texture
};