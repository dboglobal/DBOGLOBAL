//***********************************************************************************
//	File		:	CNtlLightingController.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************
#pragma once

#include ".\ntllighting.h"

//-------------------------------------------------------------
// texture name

#define	LIGHTNING_TEX_NAME				"lightning"
#define	LIGHTNING_GLOW_TEX_NAME			"lightning_glow"
#define	LIGHTNING_FLASH_TEX_NAME		"lightning_flash"
#define	LIGHTNING_CLOUDFLASH_TEX_NAME	"lightning_cloudflash"

//-------------------------------------------------------------
// 번개 속성.

#define LIGHTNING_LEVEL				5
#define LIGHTNING_SUBCOUNT			2
#define LIGHTNING_SRATIO			0.2f		// start width 
#define LIGHTNING_ERATIO			0.2f		// end width
#define LIGHTNING_AMP				1.0f		// 번개가 옆으로 벗어나는 단위
#define LIGHTNING_ROUGH				1.0f		// 번개가 찢어지는 단위


#define	LIGHTNING_SHOW_TIME			30.0f


class CNtlLightingController
{
private:
	enum { MAX_RANDOM_VALUE = 20 };

	enum 
	{
		LIGHTNING_NONE,
		LIGHTNING_READY,
		LIGHTNING_ACTIVE,
		LIGHTNING_DESTROY
	};


	RwReal				m_fActiveTime;
	RwReal				m_fCurrTime;
	RwInt32				m_nState;
	RwInt32				m_nRVIdx;
	CNtlLighting*		m_pLightning;


private:
	RwInt32			FindEmpty(void);
	CNtlLighting*	CreateLighting(RwReal fWitdh, RwReal fDelayTime, RwV3d vStart, RwBool bRandom );

public:

	CNtlLightingController(void);
	~CNtlLightingController(void);

	HRESULT CreateData(void);
	HRESULT DeleteData(void);

	HRESULT	FrameMove(RwMatrix& matView, RwReal fElapsedTime);

	HRESULT	Render();

	void	EnterState(RwInt32 nState);
	void	ExitState(RwInt32 nState);
	void	SetState(RwInt32 nState);
	
	void	Active(RwBool bActive, RwUInt32 dwSeed, RwReal fElapsedTime);
};
