//***********************************************************************************
//	File		:	CNtlLighting.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include ".\ntllightingstrip.h"

#include <string>

class CNtlLighting
{
private:
	RwMatrix					m_matWorld;

	RwTexture*					m_pTex;
	RwInt32						m_nSubCount;

	CNtlLightingStrip			m_RootStrip;
	CNtlLightingStrip*			m_pSubStrip;

	RwReal						m_fDelayTime;
	RwReal						m_fCurrTime;
	RwReal						m_fScale;

public:
	struct sCreateStream
	{
		RwReal			fDelayTime;
		std::string		strTex;		
		RwInt32			nLevel;		
		RwInt32			nSubCount;	
		RwReal			fWidth;		
		RwV3d			vStart;		
		RwV3d			vEnd;		
		RwReal			fSRatio;	
		RwReal			fERatio;	
		RwReal			fAmp;		
		RwReal			fRough;		
	};

	enum
	{
		MAX_LIGHTNING_LEVEL = 8,
		MAX_LIGHTNING_SUB	= 3
	};


private:

	void	Init(void);
	void	CreateLightningStrip();

public:
	static	RwReal	RandomNumber(RwReal fMin, RwReal fMax);
	static	RwInt32	RandomNumber(RwInt32 nMin, RwInt32 nMax);
	static	RwV3d	RandomNumber(RwV3d& vMin, RwV3d& vMax);


			CNtlLighting(void);
			~CNtlLighting(void);

	RwBool	Create(const sCreateStream& Stream);
	void	Delete(void);

	void	UpdateVertex(void);

	void	BeginState();
	void	EndState(void);

	HRESULT	FrameMove(RwReal fElapsedTime);
	HRESULT	Render();

	void	SetScale(RwReal fScale);
	void	SetPosition(RwV3d vTr);
	RwV3d	GetPosition(void);
	void	SetTexture(const RwChar* pName);

	RwV3d	GetStartPos(void);
	RwV3d	GetEndPos(void);

	RwV3d	GetSubStartPos(void);
	RwV3d	GetSubEndPos(void);
};
