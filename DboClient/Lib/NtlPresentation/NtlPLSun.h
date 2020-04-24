#pragma once

/*****************************************************************************
*
* File			: NtlPLSun
* Author		: Agebreak 
* Copyright	: (주)NTL
* Date			: 2007. 4. 12	
* Abstract		: Presentation layer Sun & Moon entity class
*****************************************************************************
* Desc         : 태양과 달을 표현하기 위한 Entity. 빌보드 평면이며, 카메라의 거리를 계속 유지하며 움직인다.
*
*****************************************************************************/

#include "NtlPLAttach.h"
#include "NtlPLLensFlare.h"


enum eSKY_FLOATING_BILLBOARD_TYPE
{
	eSFBT_SUN = 1,
	eSFBT_PLANET,
};


struct SPLSunCreateParam : public SPLEntityCreateParam
{
	std::string		strTexture;						///< 적용할 Texture 이름 
	RwBool			bAttachLensFlare;				///< LensFlare 적용 유무
	RwReal			Scale;
};


class CNtlPLSun : public CNtlPLAttach
{
public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLSun(void);
	virtual ~CNtlPLSun(void);

	virtual RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy(void);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);


	virtual void	SetPosition(const RwV3d* pPos);
	virtual void	SetColor(RwRGBA color);
	virtual void	SetScale(RwReal fScale);
	virtual void	SetMatrix(RwMatrix& matWorld);
	virtual void	SetTexture(RwTexture* pTexture);
	virtual RwBool	GetVisibility();
	virtual void	SetVisibility(RwBool bVisible);
			
	void	SetFadeEffect(RwBool bFadeIn, RwReal fTime = 1.0f);					///< Fade 효과를 적용한다. True면 나타나고, False면 사라진다. time은 걸리는 시간
	void	SetFadeEffect(RwRGBA colorFade, RwReal fTime = 1.0f);				///< Fade 효과를 적용한다. time에 걸쳐 fadeColor로 변환된다.

	void	ReFreshLensFlareData();												///< LensFlare Test를 위한 Refresh 함수

protected:
	void	CreateVertexBuffer();
	void	UpdateFadeColor(RwReal fElapsedTime);

protected:
	RwIm3DVertex*	m_pIm3dBuffer;
	RwV3d			m_vPos;
	RwReal			m_fScale;
	RwTexture*		m_pTexture;
	RwMatrix		m_matWorld;
	RwRGBA			m_color;	

	RwBool			m_bVisible;									///< Sun은 구름과 SkyBox 사이에 Render 되므로 Entity Visible을 사용하면 안된다.

	// Fade용	
	RwBool			m_bFading;									///< Fade 중인지 유무 플래그
	RwRGBA			m_FadeColor;
	RwRGBA			m_OriginColor;
	RwReal			m_fFadeTime;
	RwReal			m_fFadingTime;

	// LensFlare 객체
	CNtlPLLensFlare* m_pLensFlare;
};
