/*****************************************************************************
 *
 * File			: NtlPLCameraRenderTexture.h
 * Author		: HongHoDong
 * Copyright	: (주)NTL
 * Date			: 2006. 8. 19.	
 * Abstract		: NtlPLCameraRenderTexture
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

/**
 * \ingroup NtlPresentation
 * RenderWare의 RenderToTexture 기능을 Wrapping 한 Class 이다.
 * Texture를 생성해서 사용을 하므로 사용하는쪽에서는 Texture를 받아서 Rendering
 * 을 하면 된다.
 * CameraRenderTexture를 각,각 생성시 Camera의 ZBuffer를 계속 생성을 하기 때문에
 * ZBuffer용 Memory가 좀더 사용이 될거 같다.
 */

#ifndef __NTL_CAMERA_RENDER_TEXTURE_
#define __NTL_CAMERA_RENDER_TEXTURE_

#include "NtlDebug.h"
#include "NtlPLEntity.h"

//#define RENDER_TEXTURE_ASPECTRATIO (4.0f/3.0f)

class CNtlPLCameraRenderTexture
{
protected:
	RwCamera	*m_pCamera;																///< RenderToTexture용 Camera
	RwTexture	*m_pRenderTexture;														///< RenderToTexture용 Texture
	RwBool		m_IsCamInWorld;
	
protected:
	void SetCalcFov(RwReal fFov, RwInt32 nWidth, RwInt32 nHeight);

public:
	CNtlPLCameraRenderTexture() : m_pCamera(NULL), m_pRenderTexture(NULL) {} ;
	~CNtlPLCameraRenderTexture() {};

	void Create( RwInt32	nTexWidth,														///< RenderTexture를 생성(nWidth:가로, nHeight; 세로) 
		         RwInt32	nTexHeight, 
				 RwInt32	nRealWidth, 
				 RwInt32	nRealHeight,
				 RwReal		fFov = 45.f,
				 RwBool		IsCamInWorld = FALSE);

	void SetNear(RwReal _Near);
	void SetFar(RwReal _Far);
	RwCamera* GetCamera() { return m_pCamera; }
																						///< Texture Size와는 별도로 찍는 영역이 있는 경우는?
	void Destroy();
	
	void RenderTexture();																// Render NTL World
	void RenderTexture(CNtlPLEntity *pEntity);											///< Entity를 Texture에 Rendering을 한다.
	void RenderTexture(std::list<CNtlPLEntity*>& listEntity);
																						///< Texture에 Rendering을 하는 Timing을 어떻게 할 것인가?
																						///< Ui를 Update를 할 때 Rendering을 할 경우 문제는 없긴 하다.
	
	void Render(RwCamera *pRenderTarget, RwV2d fPos, RwV2d fSize);	    				///< Test용 m_pRenderTexture를 Rendering
	
	RwTexture *GetTexture() { NTL_ASSERTE(m_pRenderTexture); return m_pRenderTexture; } ///< Texture Return(Reference 관리를 할까 고민중)
	void SetCameraPosition(const RwV3d *pCameraPos, const RwV3d *pLookAt);				///< Camera Transform
};

#endif