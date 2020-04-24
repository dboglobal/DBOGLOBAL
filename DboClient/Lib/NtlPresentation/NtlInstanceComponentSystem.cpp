//***********************************************************************************
//	File		:	NtlInstanceParticleSystem.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include ".\ntlinstancecomponentsystem.h"

#include "NtlDebug.h"
#include "NtlMath.h"

#include "NtlEffectDefine.h"
#include "NtlEffectSystemFreeList.h"

#include "NtlPLPropertyContainer.h"
#include "NtlPLSceneManager.h"
#include "NtlPLAttach.h"

CNtlInstanceComponentSystem::CNtlInstanceComponentSystem(void)
{
	m_bReady		= TRUE;    
}

CNtlInstanceComponentSystem::~CNtlInstanceComponentSystem(void)
{
	Delete();
}


//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::Init()
{
	m_bStart	= TRUE;
	m_bReady	= FALSE;
	m_bUpdate	= FALSE;    
    m_bFinish   = FALSE;
    m_bIsApplyFlagGround = FALSE;
	m_fLifeTime = 0.f;

	m_bAttach	= FALSE;

	m_nRandomSeed			= 0;

	m_pResourceComponentSystem	= NULL;
	m_pSplinePath				= NULL;
    m_pAttachParent             = NULL;

	m_pStandardTexture		= NULL;
	m_ppMultiTexture		= NULL;

	m_fMultiTextureDeltaTime	= 0.f;

	m_nMultiTextureCount	= 0;
	m_pCurrentTexture		= NULL;
	m_nCurrentTexture		= -1;

	m_fFadeOutTime			= 0.f;
	m_pFadeOutSystem		= NULL;
    
    m_fFinishTime           = 0.0f;
    m_v3dFinishSize.x = m_v3dFinishSize.y = m_v3dFinishSize.z = 1.0f;
    m_bVisible              = TRUE;
    ZeroMemory(&m_BoundingSphere, sizeof(m_BoundingSphere));
    m_fScale                = 1.0f;
    m_uiMemoryUseSize       = 0;
    

	RwMatrixSetIdentity(&m_matAttach);
	RwMatrixSetIdentity(&m_matScale);
	RwMatrixSetIdentity(&m_matRotate);
	RwMatrixSetIdentity(&m_matPathRender);
    RwMatrixSetIdentity(&m_matRender);
    RwMatrixSetIdentity(&m_matWorld);
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::Delete()
{
	if(m_pCurrentTexture != NULL)
	{
		m_pCurrentTexture = NULL;
	}

	if (m_pStandardTexture != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pStandardTexture);
		m_pStandardTexture = NULL;
	}

	if (m_ppMultiTexture != NULL)
	{
		for (RwInt32 i = 0; i < m_nMultiTextureCount; ++ i)
		{
			if (m_ppMultiTexture[i] != NULL)
			{
				CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_ppMultiTexture[i]);
				m_ppMultiTexture[i] = NULL;
			}
		}
		NTL_ARRAY_DELETE(m_ppMultiTexture);
	}
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceComponentSystem::Create( CNtlResourceEffect* pResourceEffect, 
										    CNtlResourceComponentSystem* pResourceComponentSystem, 
											const RwMatrix& matWorld)
{
	m_pResourceComponentSystem = pResourceComponentSystem;

	// Hurricane Action 이 없을때만 Spline Path 을 생성한다.
	if (!pResourceComponentSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE) &&
		pResourceComponentSystem->m_nPathEmitterDataFlag == rpPRTADVEMITTERDATAFLAGSPLINEPATH)
	{
		m_pSplinePath = (CNtlEffectSplinePath*)CNtlPLPropertyContainer::GetInstance()->GetProperty( PLENTITY_SPLINE, 
			                                               pResourceComponentSystem->m_EmitterPrtSplinePath.strName);
	}

	if (pResourceComponentSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		CreateMultiTextures(&pResourceComponentSystem->m_EmitterMultiTextures);
	}

	m_nRandomSeed = rand(); 

	SetWorldMatrix(matWorld);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: CreateTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwTexture* CNtlInstanceComponentSystem::CreateTexture(std::string& strTexture)
{
	if (strTexture.empty()) return NULL;

	return CNtlEffectSystemFreeList::FindTexture(strTexture.c_str());
}

//------------------------------------------------------------------
//	FuncName	: CreateMultiTextures
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceComponentSystem::CreateMultiTextures(RpPrtAdvEmtPrtMultiTextures* pEmitterMultiTextures)
{
	NTL_ASSERTE(pEmitterMultiTextures->numTextures);

	m_nMultiTextureCount = pEmitterMultiTextures->numTextures;

	m_ppMultiTexture = NTL_NEW RwTexture*[m_nMultiTextureCount];
	NTL_ASSERTE(m_ppMultiTexture);

	for (RwInt32 i = 0;i < m_nMultiTextureCount; ++ i)
	{
		m_ppMultiTexture[i] = NULL;
		if (pEmitterMultiTextures->list[i].strTexture.empty())
		{
			continue;
		}
		m_ppMultiTexture[i] = CNtlEffectSystemFreeList::FindTexture(pEmitterMultiTextures->list[i].strTexture.c_str());
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: UpdateMultiTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceComponentSystem::UpdateMultiTexture(RwReal fElapsedTime)
{
	RwTexture* pTexture = RebuildMultiTexture(m_nCurrentTexture, m_nRandomSeed, fElapsedTime, &m_pResourceComponentSystem->m_EmitterMultiTextures);
	if (pTexture != m_pCurrentTexture)
	{
		m_pCurrentTexture = pTexture;
		return TRUE;
	}
	return FALSE;
}


//------------------------------------------------------------------
//	FuncName	: RebuildColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildColor(RwRGBA& destCol, RwInt32 seed, RwReal fDeltaTime, RpPrtStdEmitterPrtColor* pEmitterPrtCol)
{
	RwReal rr;
    RpPrtStdParticleColor prtCol;

    //* Start color.
    NTLPRTSTD_SRAND(seed);
	rr = NTLPRTSTD_RSRAND(seed);
    prtCol.startCol.red = pEmitterPrtCol->prtStartCol.red + (rr * pEmitterPrtCol->prtStartColBias.red);
    prtCol.startCol.green = pEmitterPrtCol->prtStartCol.green + (rr * pEmitterPrtCol->prtStartColBias.green);
    prtCol.startCol.blue = pEmitterPrtCol->prtStartCol.blue + (rr * pEmitterPrtCol->prtStartColBias.blue);
    prtCol.startCol.alpha = pEmitterPrtCol->prtStartCol.alpha + (rr * pEmitterPrtCol->prtStartColBias.alpha);

    // Delta color    
    prtCol.deltaCol.red = pEmitterPrtCol->prtEndCol.red + (rr * pEmitterPrtCol->prtEndColBias.red) - prtCol.startCol.red;
    prtCol.deltaCol.green = pEmitterPrtCol->prtEndCol.green + (rr * pEmitterPrtCol->prtEndColBias.green) - prtCol.startCol.green;
    prtCol.deltaCol.blue = pEmitterPrtCol->prtEndCol.blue + (rr * pEmitterPrtCol->prtEndColBias.blue) - prtCol.startCol.blue;
    prtCol.deltaCol.alpha = pEmitterPrtCol->prtEndCol.alpha + (rr * pEmitterPrtCol->prtEndColBias.alpha) - prtCol.startCol.alpha;

    destCol.red		= RwInt32FromRealMacro(prtCol.startCol.red	+ (fDeltaTime * prtCol.deltaCol.red));
    destCol.green	= RwInt32FromRealMacro(prtCol.startCol.green + (fDeltaTime * prtCol.deltaCol.green));
    destCol.blue	= RwInt32FromRealMacro(prtCol.startCol.blue	+ (fDeltaTime * prtCol.deltaCol.blue));
    destCol.alpha	= RwInt32FromRealMacro(prtCol.startCol.alpha + (fDeltaTime * prtCol.deltaCol.alpha));
}

//------------------------------------------------------------------
//	FuncName	: RebuildSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildSize(RwV3d& destSize, RwInt32 seed, RwReal fDeltaTime, SNtlPrtStdEmitterPrtSize* pEmitterPtrSize)
{
	RwReal rr;
	RwV3d startSize, deltaSize;

    // Start size. 
    NTLPRTSTD_SRAND(seed);
    rr = NTLPRTSTD_RSRAND(seed);
    startSize.x = pEmitterPtrSize->prtStartSize.x + (rr * pEmitterPtrSize->prtStartSizeBias.x);
    startSize.y = pEmitterPtrSize->prtStartSize.y + (rr * pEmitterPtrSize->prtStartSizeBias.y);
    startSize.z = pEmitterPtrSize->prtStartSize.z + (rr * pEmitterPtrSize->prtStartSizeBias.z);

    // Delta size.
    deltaSize.x = (pEmitterPtrSize->prtEndSize.x + (rr * pEmitterPtrSize->prtEndSizeBias.x)) - startSize.x;
    deltaSize.y = (pEmitterPtrSize->prtEndSize.y + (rr * pEmitterPtrSize->prtEndSizeBias.y)) - startSize.y;
    deltaSize.z = (pEmitterPtrSize->prtEndSize.z + (rr * pEmitterPtrSize->prtEndSizeBias.z)) - startSize.z;

    destSize.x	= startSize.x	+ (fDeltaTime * deltaSize.x);
    destSize.y	= startSize.y	+ (fDeltaTime * deltaSize.y);
    destSize.z	= startSize.z	+ (fDeltaTime * deltaSize.z);
}


//------------------------------------------------------------------
//	FuncName	: RebuildRotate
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildRotate(RwV3d& destRotate, RwInt32 seed, RwReal fDeltaTime, SNtlPrtStdEmitterPrtRotate* pEmitterPtrRotate)
{
	RwReal rr;
	RwV3d startRotate, deltaRotate;

    // Start size. 
    NTLPRTSTD_SRAND(seed);
    rr = NTLPRTSTD_RSRAND(seed);
    startRotate.x = pEmitterPtrRotate->prtStartRotate.x + (rr * pEmitterPtrRotate->prtStartRotateBias.x);
    startRotate.y = pEmitterPtrRotate->prtStartRotate.y + (rr * pEmitterPtrRotate->prtStartRotateBias.y);
    startRotate.z = pEmitterPtrRotate->prtStartRotate.z + (rr * pEmitterPtrRotate->prtStartRotateBias.z);

    // Delta size.
    deltaRotate.x = (pEmitterPtrRotate->prtEndRotate.x + (rr * pEmitterPtrRotate->prtEndRotateBias.x)) - startRotate.x;
    deltaRotate.y = (pEmitterPtrRotate->prtEndRotate.y + (rr * pEmitterPtrRotate->prtEndRotateBias.y)) - startRotate.y;
    deltaRotate.z = (pEmitterPtrRotate->prtEndRotate.z + (rr * pEmitterPtrRotate->prtEndRotateBias.z)) - startRotate.z;

    destRotate.x	= startRotate.x	+ (fDeltaTime * deltaRotate.x);
    destRotate.y	= startRotate.y	+ (fDeltaTime * deltaRotate.y);
    destRotate.z	= startRotate.z	+ (fDeltaTime * deltaRotate.z);
}

//------------------------------------------------------------------
//	FuncName	: RebuildColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildMultiColor(RwRGBA& destCol, RwInt32 seed, RwReal fDeltaTime, RpPrtStdEmitterPrtColor* pEmitterPrtCol, RpPrtAdvPrtMultiColorEmitter* pEmitterPrtMultiCol)
{
	RwReal fStartTime, fEndTime;
	RpPrtStdEmitterPrtColor curColor;
	memcpy(&curColor, pEmitterPrtCol, sizeof(RpPrtStdEmitterPrtColor));

	fStartTime = 0.f;
	fEndTime = 1.f;

	RwUInt32 i;
	for (i = 0; i < pEmitterPrtMultiCol->numCol; ++i)
	{
		if (fDeltaTime < pEmitterPrtMultiCol->list[i].time)
		{
			break;
		}
	}

	if (i >= pEmitterPrtMultiCol->numCol)
	{
		curColor.prtStartCol = pEmitterPrtMultiCol->list[pEmitterPrtMultiCol->numCol - 1].midCol;
		curColor.prtStartColBias = pEmitterPrtMultiCol->list[pEmitterPrtMultiCol->numCol - 1].midColBias;
		fStartTime = pEmitterPrtMultiCol->list[pEmitterPrtMultiCol->numCol - 1].time;
	}
	else if (i > 0)
	{
		curColor.prtStartCol = pEmitterPrtMultiCol->list[i-1].midCol;
		curColor.prtStartColBias = pEmitterPrtMultiCol->list[i-1].midColBias;
		fStartTime = pEmitterPrtMultiCol->list[i-1].time;

		if (i < pEmitterPrtMultiCol->numCol)
		{
			curColor.prtEndCol = pEmitterPrtMultiCol->list[i].midCol;
			curColor.prtEndColBias = pEmitterPrtMultiCol->list[i].midColBias;
			fEndTime = pEmitterPrtMultiCol->list[i].time;
		}
	}
	else if (i == 0)
	{
		curColor.prtEndCol = pEmitterPrtMultiCol->list[0].midCol;
		curColor.prtEndColBias = pEmitterPrtMultiCol->list[0].midColBias;
		fEndTime = pEmitterPrtMultiCol->list[0].time;
	}

	fDeltaTime = (fDeltaTime - fStartTime) / (fEndTime - fStartTime);


	RebuildColor(destCol, seed, fDeltaTime, &curColor);
}

//------------------------------------------------------------------
//	FuncName	: RebuildMultiSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildMultiSize(RwV3d& destSize, RwInt32 seed, RwReal fDeltaTime, SNtlPrtStdEmitterPrtSize* pEmitterPtrSize, SNtlAdvMultiSizeEmitter* pEmitterPrtMultiSize)
{
	RwReal fStartTime, fEndTime;
	SNtlPrtStdEmitterPrtSize curSize;
	memcpy(&curSize, pEmitterPtrSize, sizeof(SNtlPrtStdEmitterPrtSize));

	fStartTime = 0.f;
	fEndTime = 1.f;

	RwUInt32 i;
	for (i = 0; i < pEmitterPrtMultiSize->numSize; ++i)
	{
		if (fDeltaTime < pEmitterPrtMultiSize->list[i].time)
		{
			break;
		}
	}

	if (i >= pEmitterPrtMultiSize->numSize)
	{
		curSize.prtStartSize = pEmitterPrtMultiSize->list[pEmitterPrtMultiSize->numSize - 1].midSize;
		curSize.prtStartSizeBias = pEmitterPrtMultiSize->list[pEmitterPrtMultiSize->numSize - 1].midSizeBias;
		fStartTime = pEmitterPrtMultiSize->list[pEmitterPrtMultiSize->numSize - 1].time;
	}
	else if (i > 0)
	{
		curSize.prtStartSize = pEmitterPrtMultiSize->list[i-1].midSize;
		curSize.prtStartSizeBias = pEmitterPrtMultiSize->list[i-1].midSizeBias;
		fStartTime = pEmitterPrtMultiSize->list[i-1].time;

		if (i < pEmitterPrtMultiSize->numSize)
		{
			curSize.prtEndSize = pEmitterPrtMultiSize->list[i].midSize;
			curSize.prtEndSizeBias = pEmitterPrtMultiSize->list[i].midSizeBias;
			fEndTime = pEmitterPrtMultiSize->list[i].time;
		}
	}
	else if (i == 0)
	{
		curSize.prtEndSize = pEmitterPrtMultiSize->list[0].midSize;
		curSize.prtEndSizeBias = pEmitterPrtMultiSize->list[0].midSizeBias;
		fEndTime = pEmitterPrtMultiSize->list[0].time;
	}

	fDeltaTime = (fDeltaTime - fStartTime) / (fEndTime - fStartTime);

	RebuildSize(destSize, seed, fDeltaTime, &curSize);
}

//------------------------------------------------------------------
//	FuncName	: RebuildMultiSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceComponentSystem::RebuildMultiRotate(RwV3d& destRotate, RwInt32 seed, RwReal fDeltaTime, SNtlPrtStdEmitterPrtRotate* pEmitterPtrRotate, SNtlAdvMultiRotateEmitter* pEmitterPrtMultiRotate)
{
    if(pEmitterPrtMultiRotate->numRotate <= 0)
        return;

	RwReal fStartTime, fEndTime;
	SNtlPrtStdEmitterPrtRotate curRotate;
	memcpy(&curRotate, pEmitterPtrRotate, sizeof(SNtlPrtStdEmitterPrtRotate));

	fStartTime = 0.f;
	fEndTime = 1.f;

	RwUInt32 i = 0;
	for (i = 0; i < pEmitterPrtMultiRotate->numRotate; ++i)
	{
		if (fDeltaTime < pEmitterPrtMultiRotate->list[i].time)
		{
			break;
		}
	}

	if (i >= pEmitterPrtMultiRotate->numRotate)
	{
		curRotate.prtStartRotate = pEmitterPrtMultiRotate->list[pEmitterPrtMultiRotate->numRotate - 1].midRotate;
		curRotate.prtStartRotateBias = pEmitterPrtMultiRotate->list[pEmitterPrtMultiRotate->numRotate - 1].midRotateBias;
		fStartTime = pEmitterPrtMultiRotate->list[pEmitterPrtMultiRotate->numRotate - 1].time;
	}
	else if (i > 0)
	{
		curRotate.prtStartRotate = pEmitterPrtMultiRotate->list[i-1].midRotate;
		curRotate.prtStartRotateBias = pEmitterPrtMultiRotate->list[i-1].midRotateBias;
		fStartTime = pEmitterPrtMultiRotate->list[i-1].time;

		if (i < pEmitterPrtMultiRotate->numRotate)
		{
			curRotate.prtEndRotate = pEmitterPrtMultiRotate->list[i].midRotate;
			curRotate.prtEndRotateBias = pEmitterPrtMultiRotate->list[i].midRotateBias;
			fEndTime = pEmitterPrtMultiRotate->list[i].time;
		}
	}
	else if (i == 0)
	{
		curRotate.prtEndRotate = pEmitterPrtMultiRotate->list[0].midRotate;
		curRotate.prtEndRotateBias = pEmitterPrtMultiRotate->list[0].midRotateBias;
		fEndTime = pEmitterPrtMultiRotate->list[0].time;
	}

	fDeltaTime = (fDeltaTime - fStartTime) / (fEndTime - fStartTime);


	RebuildRotate(destRotate, seed, fDeltaTime, &curRotate);
}

//------------------------------------------------------------------
//	FuncName	: RebuildMultiTexture
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwTexture* CNtlInstanceComponentSystem::RebuildMultiTexture(RwInt32 nCurrentIndex, RwInt32 seed, RwReal fElapsedTime, RpPrtAdvEmtPrtMultiTextures* pEmitterMultiTexture)
{
	m_fMultiTextureDeltaTime += fElapsedTime;
	if (m_nCurrentTexture == -1)
	{
		if (m_fMultiTextureDeltaTime < pEmitterMultiTexture->time)
		{
			return m_pCurrentTexture;
		}
		m_fMultiTextureDeltaTime -= pEmitterMultiTexture->time;
		m_nCurrentTexture = 0;
	}

	RwReal rr;
    NTLPRTSTD_SRAND(seed);
	rr = NTLPRTSTD_RSRAND(seed);
	RwReal fLocalTime = pEmitterMultiTexture->list[m_nCurrentTexture].time + (rr * pEmitterMultiTexture->list[m_nCurrentTexture].timeBias);
	if (m_fMultiTextureDeltaTime > fLocalTime)
	{
		if (m_nCurrentTexture >= ((RwInt32)pEmitterMultiTexture->numTextures - 1))
		{
			if (pEmitterMultiTexture->bLoop)
			{
				m_nCurrentTexture = 0;
				m_fMultiTextureDeltaTime -= fLocalTime;
			}
		}
		else
		{
			++ m_nCurrentTexture;
			m_fMultiTextureDeltaTime -= fLocalTime;
		}
	}
	return m_ppMultiTexture[m_nCurrentTexture];
}

RwBool CNtlInstanceComponentSystem::Finish() 
{
    Stop();

    m_bFinish = TRUE;

	return FALSE;
}

RwBool CNtlInstanceComponentSystem::UpdateFinish( RwReal fElapsedTime ) 
{
    return FALSE;
}

RwUInt8 CNtlInstanceComponentSystem::UpdateFinishAlpha( RwUInt8 uiOriginAlpha, RwReal fElapsedTime, RwReal fFinishTime ) 
{
    if(uiOriginAlpha == 0)
        return 0;

    RwReal fDeltaAlpha = (RwReal)uiOriginAlpha / 255 * fElapsedTime / fFinishTime;
    RwInt32 nDeltaAlpha = (RwInt32)(fDeltaAlpha + 0.5f);
    
    if(nDeltaAlpha < 1)
    {
        // 값이 너무 작아서 1보다 작게 나와서 적용이 안되는 경우가 있다.
        // 이런 경우엔 강제로 1을 제거한다.
        return 1;
    }    

    return (RwUInt8)nDeltaAlpha;
}

RwV3d CNtlInstanceComponentSystem::UpdateFinishSize( RwV3d v3dOriginSize, RwReal fElapsedTime, RwReal fFinishTime ) 
{
    m_v3dFinishSize -= v3dOriginSize * fElapsedTime / fFinishTime;

    if(m_v3dFinishSize.x > 0 && m_v3dFinishSize.y > 0 && m_v3dFinishSize.z >0)
    {
        RwMatrixScale(&m_matScale, &m_v3dFinishSize, rwCOMBINEREPLACE);

        SetWorldMatrix(m_matWorld);
    }

    return m_v3dFinishSize;
}

RwBool CNtlInstanceComponentSystem::Update( RwReal fElapsedTime ) 
{
    // Effect check located on the terrain
    if(m_pResourceComponentSystem->m_sGroundFlag.bGround && !m_bIsApplyFlagGround)
    {
        RwV3d vPos = ZeroAxis;
        RwReal fHeight = 0.0f;

        if(m_pAttachParent)
        {
            vPos = *RwMatrixGetPos(&m_pAttachParent->GetMatrix());
        }
        else
        {
            vPos = *RwMatrixGetPos(&m_matWorld);        
        }
        
        if(GetSceneManager()->GetWorldHeight(&vPos, fHeight, NULL))
        {
            vPos.y = fHeight + m_pResourceComponentSystem->m_sGroundFlag.fOffsetY;

            *RwMatrixGetPos(&m_matWorld) = vPos;
            RwMatrixUpdate(&m_matWorld);   
            SetWorldMatrix(m_matWorld);
        }

        m_bIsApplyFlagGround = TRUE;                    // When Create is processed, it is not processed because no resources have been created yet.
    }

    return TRUE;
}

RwSphere* CNtlInstanceComponentSystem::GetBoundingSphere() 
{
    m_BoundingSphere.center = m_matRender.pos;
    m_BoundingSphere.radius = m_pResourceComponentSystem->GetBoundingSphereRadius();
    m_BoundingSphere.radius = m_BoundingSphere.radius == 0.0f ? 5.0f : m_BoundingSphere.radius;

    return &m_BoundingSphere;
}