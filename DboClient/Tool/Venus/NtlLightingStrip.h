//***********************************************************************************
//	File		:	NtlLightingStrip.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************


#pragma once

#include "rwcore.h"
#include "d3dx9.h"

#define MAX_LINE_STRIP	1024	

class CNtlLightingStrip
{
public:
	static RwMatrix				m_matView;

protected:
	static	RwV3d				m_vLoc[MAX_LINE_STRIP];
	
protected:
	RwInt32						m_nMaxSize;
	RwInt32						m_nNum;
	RwReal						m_fWidth;			// Line Æø 

	D3DCOLOR					m_Color;

	RwMatrix					m_matTM;
	RwIm3DVertex*				m_pVertices;

public:
	RwInt32						m_nLevel;
	RwV3d						m_vStart;
	RwV3d						m_vEnd;

	RwReal						m_fSRatio;
	RwReal						m_fERatio;
	RwReal						m_fAmp;
	RwReal						m_fRough;

protected:
	void						CalcMatrix(RwMatrix& mat, const RwV3d& vStart, const RwV3d& vEnd);

public:

	CNtlLightingStrip();
	virtual ~CNtlLightingStrip();

	BOOL	Load(RwInt32 nMaxSize);
	void	Unload();

public:
	void	CalculatePoints(const RwV3d& vStart, const RwV3d &vEnd, RwReal fAmp, RwReal fRough);
	void	CalculateVertices(void);
	RwReal	GetLineWidth(RwReal fAlpha);

public:
	void					SetView(RwMatrix& matView)					{ m_matView = matView; }

	void					SetColor(D3DCOLOR Color)					{ m_Color = Color; }
	D3DCOLOR				GetColor(void) const						{ return m_Color; }

	void					SetWidth(RwReal fWidth)						{ m_fWidth = fWidth; }
	RwReal					GetWidth(void) const						{ return m_fWidth; }

	RwInt32					GetNum(void) const							{ return m_nNum; }
	RwInt32					GetVertexCount(void) const					{ return m_nNum*2; }
	RwIm3DVertex*			GetVertices(void) const						{ return m_pVertices; }
	RwInt32					GetPrimitiveCount(void) const				{ return (m_nNum-1)*2; }
	RwV3d					GetAxisX(void)								{ return *RwMatrixGetRight(&m_matTM); }
};
