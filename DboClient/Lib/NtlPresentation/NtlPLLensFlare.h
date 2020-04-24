/*****************************************************************************
*
* File			: NtlPlLensFlare
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2007. 4. 19	
* Abstract		: Presentation layer LensFlare entity class
*****************************************************************************
* Desc         : 태양의 위치에 영향을 받는 Lensflare를 구현한 클래스
*
*****************************************************************************/

#pragma once

#include "NtlPLAttach.h"
#include "NtlDebug.h"
#include "NtlEffectDefine.h"

#define	LENSFLARE_FADE_VELOCITY		4.0f
#define	LENSFLARE_PICK_TICK			0.3f

///< LensFlare를 구성하는 Spot의 정보를 담고 있는 구조체
struct SNtlPLLensFlareSpot
{
	RwTexture*	m_pTexture;					///< Spot의 Texture
	RwReal		m_fSize;					///< Spot의 Size (0.0 ~ 1.0)
	RwReal		m_fLinePos;					///< Spot의 위치 (1 ~ -1)
	RwRGBAReal	m_color;					///< Spot의 Vertex Color	
	RwIm2DVertex* m_pIm2dBuffer;			///< Spot을 구성하는 Vertex 버퍼

	SNtlPLLensFlareSpot()
	{
		m_pTexture = NULL;
		m_fSize = 1.0f;
		m_fLinePos = 0.5f;
		m_color.red = m_color.green = m_color.blue = m_color.alpha = 1.0f;
		m_pIm2dBuffer = NULL;
	}

	SNtlPLLensFlareSpot(RwTexture* pTexture, RwReal fSize, RwReal fLinePos, RwReal red, RwReal green, RwReal blue, RwReal alpha)
	{
		m_pTexture = pTexture;
		m_fSize = fSize;
		m_fLinePos = fLinePos;		
		m_color.red = red;
		m_color.green = green;
		m_color.blue = blue;
		m_color.alpha = alpha;
		m_pIm2dBuffer = NULL;
	}

	~SNtlPLLensFlareSpot()
	{
		if(m_pIm2dBuffer)
		{
			NTL_ARRAY_DELETE(m_pIm2dBuffer);
		}
	}
};

class CNtlPLLensFlare : public CNtlPLAttach
{
public:
	CNtlPLLensFlare(void);
	virtual ~CNtlPLLensFlare(void);

	virtual RwBool	Create( const SPLEntityCreateParam * pParam = NULL );
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);
	virtual void	Destroy(void);
	virtual RwBool	Update(RwReal fElapsed);
	virtual RwBool	Render(void);

	virtual void	SetMatrix(RwMatrix& matWorld);

	void	AddSpot(const SNtlPLLensFlareSpot& spot);				///< Spot을 추가한다.
	void	AddSpotForTest(const std::string& strXMLFileName);		///< XML파일에서 Spot 정보를 읽어와서 설정한다

	void	SetMaxIntensity(RwReal fMaxIntensity) {m_fIntensity = fMaxIntensity;}	///< 최대 강도를 설정한다. (0.0 ~ 1.0 사이)
	RwReal  GetMaxIntensity() {return m_fIntensity;}				///<  최대 강도를 반환한다.

protected:
	void	AddSpots();												///< Spot들을 벡터에 추가한다.
	void	UpdateVertices(RwReal fElapsedTime);					///< vertex들의 정보를 Update한다.
	void	SetRhwVertex(RwIm2DVertex* pVertex, RwReal nStartX, RwReal nStartY, RwInt32 width, RwInt32 height, RwRGBAReal color);		///< 2D Vertex들을 설정한다
	RwBool	CalcPicking(const RwV2d& posLight);						///< 광원을 Object와 Terrain Collision을 체크하여 보일지 안보일지 결정한다.

protected:
	std::vector<SNtlPLLensFlareSpot>		m_vecSpots;
	std::vector<RwTexture*>					m_vecTex;	
	RwReal									m_fIntensity;			///< Flare의 최대 강도
	RwReal									m_fIntensityBorder;		///< Flare의 범위 (강도에 영향을 준다)	
	RwMatrix								m_matWorld;				///< World Matrix (Pos를 설정하기 위해서 사용한다)
	RwBool									m_bRender;				///< Render 유무
	RwReal									m_fFadeIntensity;		///< Fade In/Out용 변수
	EFadeStatus								m_eFadeStatus;

	// Pick Tick 설정용 변수들
	RwBool									m_bPrevPickingResult;	///< 이전 Tick에서 저장하고 있던 Picking 결과값
	RwReal									m_fPickingTickTime;		///< Pick에 Tick을 지정하기 위한 Time 변수									
};	
