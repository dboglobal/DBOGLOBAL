/******************************************************************************
* File			: NtlPLCameraRT.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 3. 7
* Abstract		: 
*****************************************************************************
* Desc			: Camera render texture 생성 순서
*				  1. CameraBeginUpdate()의 true 면 Rendering
*				  2. CameraEndUpdate() 호출
*				  3. GetTexture() 로 텍스처 반환
* 주의 ) CameraBeginUpdate()에 매번 새로운 Framde buffer를 할당한다. 따라서
*		 CameraBeginUpdate()로 텍스처를 생성하고 반드시 GetTexture()로 텍스처를
*		 얻어와서 텍스처를 해주어야 한다.
*****************************************************************************/
#pragma once

class CNtlPLCameraRT
{
public:
	CNtlPLCameraRT();
	CNtlPLCameraRT(const CNtlPLCameraRT& camera) {}
	virtual ~CNtlPLCameraRT();

	VOID			Create(RwInt32 iWidth, RwInt32 iHeigth);
	VOID			Destroy();

	RwBool			CameraBeginUpdate();			///< t : 카메라 텍스처에 랜더링할 준비가 되었다
	VOID			CameraEndUpdate();

	RwTexture*		GetTexture();

protected:
	RwCamera*				m_pCamera;				///< Raster camera
	RwTexture*				m_pRenderTexture;		///< Render target texture

	RwInt32					m_iWidth, m_iHeigth;
};