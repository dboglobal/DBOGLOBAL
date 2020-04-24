//***********************************************************************************
//	File		:	CNtlLighting.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\ntllighting.h"

#include "NtlDebug.h"

RwReal CNtlLighting::RandomNumber(RwReal fMin, RwReal fMax)
{
	if(fMin > fMax)
	{
		RwReal fTemp = fMin;
		fMin = fMax;
		fMax = fTemp;
	}
	
	if (fMin == fMax) 
		return(fMin);
	RwReal fRandom = (RwReal)rand() / (RwReal)RAND_MAX;
	return((fRandom * (RwReal)fabs(fMax-fMin))+fMin);
}

RwInt32 CNtlLighting::RandomNumber(RwInt32 nMin, RwInt32 nMax)
{
	if(nMin > nMax)
	{
		RwInt32 nTemp = nMin;
		nMin = nMax;
		nMax = nTemp;
	}
	
	if (nMin == nMax) 
		return(nMin);
	RwInt32 nSub = nMax - nMin;
	if(nSub)
		return nMin + rand()%nSub;
	else
		return nMin;
}

RwV3d CNtlLighting::RandomNumber(RwV3d& vMin, RwV3d& vMax)
{
	RwReal x = RandomNumber(vMin.x, vMax.x);
	RwReal y = RandomNumber(vMin.y, vMax.y);
	RwReal z = RandomNumber(vMin.z, vMax.z);
	RwV3d v = {x,y,z};
	return v;
}


CNtlLighting::CNtlLighting(void)
{
	Init();
}

CNtlLighting::~CNtlLighting(void)
{
	Delete();
}

void CNtlLighting::CreateLightningStrip()
{
	if(m_nSubCount)
	{
		m_pSubStrip = NTL_NEW CNtlLightingStrip[m_nSubCount];

		RwInt32 nSubLevel = max(1, m_RootStrip.m_nLevel-1);

		for(RwInt32 i = 0; i< m_nSubCount; i++)
		{
			m_pSubStrip[i].m_nLevel = nSubLevel;
			m_pSubStrip[i].Load((1 << nSubLevel)+1);
			m_pSubStrip[i].m_fSRatio = m_RootStrip.m_fSRatio;
			m_pSubStrip[i].m_fERatio = m_RootStrip.m_fERatio;
		}
	}
}

void CNtlLighting::Init(void)
{
	m_nSubCount		= 0; 
	m_pSubStrip		= NULL;
	m_pTex			= NULL;
	m_fDelayTime	= 0.0f;
	m_fCurrTime		= 0.0f;

	RwMatrixSetIdentity(&m_matWorld);
}

RwBool CNtlLighting::Create(const sCreateStream& Stream)
{
	Delete();

	m_fDelayTime = Stream.fDelayTime; 

	RwInt32 nLevel = Stream.nLevel;
	RwInt32 nSubCount = Stream.nSubCount;

	if(nLevel <= 0)	nLevel = 1;
	if(nLevel > MAX_LIGHTNING_LEVEL) nLevel = MAX_LIGHTNING_LEVEL;

	if(	nSubCount < 0)	nSubCount = 0;
	if(nSubCount > MAX_LIGHTNING_SUB) nSubCount = MAX_LIGHTNING_SUB;

	m_nSubCount = nSubCount ;

	m_RootStrip.m_nLevel = nLevel;
	m_RootStrip.Load((1 << nLevel)+1);
	m_RootStrip.SetWidth(Stream.fWidth);
	m_RootStrip.m_vStart = Stream.vStart;
	m_RootStrip.m_vEnd	 = Stream.vEnd;
	m_RootStrip.m_fSRatio = Stream.fSRatio;
	m_RootStrip.m_fERatio = Stream.fERatio;
	m_RootStrip.m_fAmp	  = Stream.fAmp;
	m_RootStrip.m_fRough  = Stream.fRough;

	SetTexture(Stream.strTex.c_str());

	CreateLightningStrip();


	RwChar aaa[256];
	GetCurrentDirectory(256, aaa);

//	const RwChar* asdfas = RwImageSetPath("d:\\Work\\Dbo-x\\DboClient\\Tool\\Venus\\Venus\\Textures\\");
//	m_pTex = RwTextureRead("lightning", NULL);
	return true;
}

void CNtlLighting::Delete(void)
{
	m_RootStrip.Unload();
	NTL_ARRAY_DELETE(m_pSubStrip)

	Init();
}		


void CNtlLighting::UpdateVertex()
{
	m_RootStrip.CalculateVertices();

	RwInt32 nIdx;

	RwV3d vStart, vEnd, vDist, vMix, vAxisX;
	RwV3dSubMacro(&vDist, &m_RootStrip.m_vEnd, &m_RootStrip.m_vStart);
	vAxisX = m_RootStrip.GetAxisX();

	RwIm3DVertex* pVertices = m_RootStrip.GetVertices();

	RwReal fRandom;
	for(RwInt32 i = 0; i < m_nSubCount; i++)
	{
		nIdx = rand() % (m_RootStrip.GetNum()/2);

		vStart = RandomNumber(pVertices[nIdx*2+0].objVertex, pVertices[nIdx*2+1].objVertex);

		fRandom = RandomNumber(0.5f, 0.7f);
		vEnd.x = vStart.x + vDist.x * fRandom;
		vEnd.y = vStart.y + vDist.y * fRandom;
		vEnd.z = vStart.z + vDist.z * fRandom;

		vMix = vEnd;

		RwV3dSubMacro(&vEnd, &vStart, &m_RootStrip.m_vStart);

		fRandom = RandomNumber(0.5f, 0.7f);
		vMix.x += vAxisX.x * fRandom * (rand()%2 > 0 ? 1 : -1);
		vMix.y += vAxisX.y * fRandom * (rand()%2 > 0 ? 1 : -1);
		vMix.z += vAxisX.z * fRandom * (rand()%2 > 0 ? 1 : -1);

		m_pSubStrip[i].m_vStart = vStart;
		m_pSubStrip[i].m_vEnd = vMix;
		m_pSubStrip[i].SetColor(m_RootStrip.GetColor());
		m_pSubStrip[i].m_fAmp = m_RootStrip.m_fAmp;
		m_pSubStrip[i].m_fRough = m_RootStrip.m_fRough;
		m_pSubStrip[i].SetWidth(m_RootStrip.GetWidth()/4); 

		m_pSubStrip[i].CalculateVertices();
	}
}

void CNtlLighting::BeginState()
{
	RwD3D9SetTransform(D3DTS_WORLD, &m_matWorld);
	
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)TRUE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND,			(void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,		(void *)rwBLENDONE);

//	RwRenderStateSet(rwRENDERSTATESTENCILZFAIL, (void *)TRUE);
//	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);
//	RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)rwCULLMODECULLNONE);

	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)m_pTex);
//	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)RwTextureGetRaster(m_pTex));

//	RwD3D9SetStreamSource(0, pPrimitive->GetVertexBuffer(), pPrimitive->GetVertexOffset(), sizeof(NTL_LIGHTING_VERTEX));
}


void CNtlLighting::EndState(void)
{
//	RwRenderStateSet(rwRENDERSTATECULLMODE,				(void *)rwCULLMODECULLBACK);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE,	(void *)FALSE);

	RwRenderStateSet(rwRENDERSTATESRCBLEND,				(void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND,			(void *)rwBLENDINVSRCALPHA);
}

HRESULT	CNtlLighting::FrameMove(RwReal fElapsedTime)
{
	m_fCurrTime += fElapsedTime;

	if (m_fCurrTime > 1.f)
	{
		m_fCurrTime = 0.f;
		UpdateVertex();
	}

	return S_OK;
}

	
HRESULT	CNtlLighting::Render()
{
	return S_OK;

//	if(m_fCurrTime < m_fDelayTime)
//		return S_OK;

	BeginState();

	if( RwIm3DTransform(m_RootStrip.GetVertices(), m_RootStrip.GetVertexCount(), (RwMatrix *)&m_matWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV) )
	{
		RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
		RwIm3DEnd();
	}
	for(RwInt32 i = 0; i < m_nSubCount; i++)
	{
		if( RwIm3DTransform(m_pSubStrip[i].GetVertices(), m_pSubStrip[i].GetVertexCount(), (RwMatrix *)&m_matWorld, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);        
			RwIm3DEnd();
		}
	}
	EndState();

	return S_OK;
}

void CNtlLighting::SetScale(RwReal fScale)
{
	m_fScale = fScale;
//	m_matWorld.SetScale(fScale);
	RwV3d vScale = { fScale, fScale, fScale };
	RwMatrixScale(&m_matWorld, &vScale, rwCOMBINEPOSTCONCAT);
}

void CNtlLighting::SetPosition(RwV3d vTr)
{
	*RwMatrixGetPos(&m_matWorld) = vTr;
}

RwV3d CNtlLighting::GetPosition(void)
{
	return *RwMatrixGetPos(&m_matWorld);
}

void CNtlLighting::SetTexture(const char *pName)
{
//	m_pTex = NULL;
}

RwV3d CNtlLighting::GetStartPos(void)
{
	return m_RootStrip.m_vStart;
}

RwV3d CNtlLighting::GetEndPos(void)
{
	return m_RootStrip.m_vEnd;
}

RwV3d CNtlLighting::GetSubStartPos(void)
{
	if(m_pSubStrip)
		return m_pSubStrip->m_vStart;
	else
		return m_RootStrip.m_vStart;
}

RwV3d CNtlLighting::GetSubEndPos(void)
{
	if(m_pSubStrip)
		return m_pSubStrip->m_vStart;
	else
		return m_RootStrip.m_vEnd;
}
