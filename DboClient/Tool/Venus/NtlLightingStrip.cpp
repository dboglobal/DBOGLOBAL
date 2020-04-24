//***********************************************************************************
//	File		:	NtlLightingLinePrimitive.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\ntllightingstrip.h"

#include "NtlDebug.h"

RwV3d		CNtlLightingStrip::m_vLoc[MAX_LINE_STRIP];
RwMatrix	CNtlLightingStrip::m_matView;

CNtlLightingStrip::CNtlLightingStrip()
{
	m_nMaxSize	= 0;
	m_nNum		= 0;
	m_fWidth	= 0.3f;			// Line Æø 
	m_Color		= 0xffffffff;

	m_pVertices = NULL;

	RwMatrixSetIdentity(&m_matTM);


	m_nLevel	= 1;
	m_fSRatio	= 0.2f;
	m_fERatio	= 0.2f; 
	m_fAmp		= 0.0f;
	m_fRough	= 0.0f;
}

CNtlLightingStrip::~CNtlLightingStrip()
{
	Unload();
}


BOOL CNtlLightingStrip::Load(RwInt32 nMaxSize)
{
	Unload();

	m_nMaxSize = nMaxSize;

	m_pVertices = NTL_NEW RwIm3DVertex[m_nMaxSize*2];

	m_nNum = m_nMaxSize;

	return TRUE;
}

void CNtlLightingStrip::Unload()
{
	m_nMaxSize = 0;
	m_nNum = 0;

	NTL_ARRAY_DELETE(m_pVertices)
}

void CNtlLightingStrip::CalcMatrix(RwMatrix& mat, const RwV3d& vStart, const RwV3d& vEnd)
{
	static RwV3d vAxisZ, vEyeDir, vAxisX, vAxisY;

	RwMatrixSetIdentity(&mat);

	RwV3dSubMacro(&vAxisZ, &vEnd, &vStart);

    if(RwV3dLength(&vAxisZ) != 0)    
    	RwV3dNormalize(&vAxisZ, &vAxisZ);

	RwV3d vEye = *RwMatrixGetPos(&m_matView);
	RwV3dSubMacro(&vAxisY, &vEye, &vStart);

	RwV3dCrossProduct(&vAxisX, &vAxisY, &vAxisZ);
    
    if(RwV3dLength(&vAxisX) != 0)    
	    RwV3dNormalize(&vAxisX, &vAxisX);


	RwV3dCrossProduct(&vAxisY, &vAxisX, &vAxisZ);

	*RwMatrixGetRight(&mat) = vAxisX;
	*RwMatrixGetAt (&mat)	= vAxisZ;
	*RwMatrixGetUp (&mat)	= vAxisY;
}

void CNtlLightingStrip::CalculatePoints(const RwV3d& vStart, const RwV3d& vEnd, RwReal fAmp, RwReal fRough)
{
	if(m_nNum <= 1)
		return;

	RwInt32 nSize = m_nNum-1;
	RwInt32 nRectSize = nSize;
	RwReal fDH = (RwReal)fAmp;
	RwReal r = (RwReal)pow((RwReal)2,-1*fRough);
	RwInt32 i, ni, mi;

	m_vLoc[0].x = m_vLoc[0].y = m_vLoc[0].z = 0.f;
	m_vLoc[nSize].x = m_vLoc[nSize].y = m_vLoc[nSize].z = 0.f;
		
	while(nRectSize > 0)
	{
		for (i = 0; i < nSize; i += nRectSize)
		{
			ni = i+nRectSize;
			mi = (i+nRectSize/2);
			RwReal fRand = (((RwReal)rand()-(RwReal)rand())/(2L*RAND_MAX));
			m_vLoc[mi].x = (m_vLoc[i].x + m_vLoc[ni].x) * 0.5f + fDH * fRand;
			m_vLoc[mi].y = 0;//(m_arrPoints[i].y + m_arrPoints[ni].y) * 0.5f + dh * rnd();
		}

		nRectSize /= 2;
		fDH *= r;
	}
	
	RwV3d vDist;
	RwV3dSubMacro(&vDist, &vEnd, &vStart);
	RwReal fLength = RwV3dLength(&vDist);
	RwReal fDelta, fOffset;

	fDelta = fLength / (nSize*2);

	CalcMatrix(m_matTM, vStart, vEnd);
	*RwMatrixGetPos(&m_matTM) = vStart; 

	for (i = 0; i < nSize+1; ++i)
	{
		fOffset = ((RwReal)i / nSize); 
		RwReal fRand = (((RwReal)rand()-(RwReal)rand())/(2L*RAND_MAX));
		m_vLoc[i].z = fLength * fOffset + fDelta * fRand;
//		m_arrPoints[i].x *= offset; 
//		m_arrPoints[i].y *= offset; 

		RwV3dTransformPoint(&m_vLoc[i], &m_vLoc[i], &m_matTM);
	}

	m_vLoc[0] = vStart;
}

void CNtlLightingStrip::CalculateVertices(void)
{
	CalculatePoints(m_vStart, m_vEnd, m_fAmp, m_fRough);

	RwV3d vDist;
	RwV3dSubMacro(&vDist, &m_vEnd, &m_vStart);

	RwReal fLength = RwV3dLength(&vDist) * 3;

	RwV3d vAxisX = GetAxisX();

	for(RwInt32 i = 0; i < m_nNum; ++i)
	{
		m_pVertices[i*2+0].objVertex.x = m_vLoc[i].x + vAxisX.x * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+0].objVertex.y = m_vLoc[i].y + vAxisX.y * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+0].objVertex.z = m_vLoc[i].z + vAxisX.z * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+0].color = m_Color;
		m_pVertices[i*2+0].u = 0;
		m_pVertices[i*2+0].v = fLength * ((RwReal)i / m_nNum);

		m_pVertices[i*2+1].objVertex.x = m_vLoc[i].x - vAxisX.x * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+1].objVertex.y = m_vLoc[i].y - vAxisX.y * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+1].objVertex.z = m_vLoc[i].z - vAxisX.z * GetLineWidth((RwReal)i/m_nNum);
		m_pVertices[i*2+1].color = m_Color;
		m_pVertices[i*2+1].u = 1;
		m_pVertices[i*2+1].v = m_pVertices[i*2+0].v;
	}

//	m_vEnd = m_pVertices[(m_nNum-1)*2+1].objVertex;
}

RwReal CNtlLightingStrip::GetLineWidth(RwReal fAlpha)
{
/*	if (fAlpha < 0.5f)
		return m_fWidth * (m_fSRatio + (1.0f - m_fSRatio) * fAlpha);
	else
		return m_fWidth * (m_fERatio + (1.0f - m_fERatio) * (1.0f - fAlpha));
	*/
	if (fAlpha < 0.5f)
		return m_fSRatio / 2.f;
	else
		return m_fERatio / 2.f;
}
