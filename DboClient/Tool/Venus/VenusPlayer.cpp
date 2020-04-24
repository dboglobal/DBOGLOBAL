//***********************************************************************************
//	File		:	VenusPlayer.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by NTL CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusplayer.h"

#include "NtlPLGlobal.h"
#include "NtlEffectAPI.h"



#include "NtlPLResourceManager.h"

#include "VenusAPI.h"
#include "VenusVisualManager.h"
#include "VenusConfig.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlInstanceMeshSystem.h"

#include ".\venusplayereventmanager.h"


CVenusPlayer::CVenusPlayer(void)
{
	m_pClumpResource		= NULL;
	m_pAnimationResource	= NULL;
    m_pToonData             = NULL;

	m_fCurrentTime			= 0.f;
	m_nMaxTime				= 0;

	m_fLifeTime				= 0.f;

	for (RwInt32 i = 0; i < 1; ++ i)
	{
		EVENT* pEvent = NTL_NEW EVENT;
		InitializeEvent(pEvent);
		m_svEvent.push_back(pEvent);
	}

    m_pBaseHierarchy = NULL;
}

CVenusPlayer::~CVenusPlayer(void)
{
	DeleteEvent();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::Delete()
{
	DeleteEffectEntity();

    if(m_pToonData)
    {
        RpToonPaintDestroy(m_pToonData->pToonPaint);
        RpToonInkDestroy(m_pToonData->pToonInk);
        CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pToonData->pTexture);
        NTL_DELETE(m_pToonData);        
    }

	if (m_pClumpResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pClumpResource);
		m_pClumpResource = NULL;
	}
	if (m_pAnimationResource != NULL)
	{
		CNtlPLResourceManager::GetInstance()->UnLoad(m_pAnimationResource);
		m_pAnimationResource = NULL;
	}
}

//------------------------------------------------------------------
//	FuncName	: DeleteEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::DeleteEvent()
{
	if (!m_svEvent.empty())
	{
		svdef_EVENT::iterator it;
		for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
		{
			EVENT* pEvent = (*it);
			NTL_DELETE(pEvent);
		}
		m_svEvent.clear();
	}
}
//------------------------------------------------------------------
//	FuncName	: DeleteEffectEntity
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::DeleteEffectEntity()
{
	if (!m_svEvent.empty())
	{
		svdef_EVENT::iterator it;
		for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
		{
			EVENT* pEvent = (*it);
			if (pEvent->pEffectEntity != NULL)
			{
				CVenusVisualManager::GetInstance().DeleteEventEffect(pEvent->pEffectEntity);
				pEvent->pEffectEntity = NULL;
			}
		}
	}
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::Create()
{
	if (!m_strClumpName.empty())
	{
		const RwChar* strPathName = CVenusPlayerEventManager::GetInstance().m_ClumpFileManager.GetPathName(m_strClumpName.c_str());
		if (strPathName != NULL)
		{
			m_pClumpResource = CNtlPLResourceManager::GetInstance()->LoadClump(strPathName, CVenusPlayerEventManager::GetInstance().m_strClumpTexturePath.c_str());
            CNtlInstanceMeshSystem::CreateToonData(m_pClumpResource->GetClump(), m_pToonData);
            
            // Hierarchy 정보를 설정한다.
            m_pBaseHierarchy = Helper_GetHierarchyClump(m_pClumpResource->GetClump());
            if(m_pBaseHierarchy)
            {
                Helper_SetHierarchyClumpSkin(m_pClumpResource->GetClump(), m_pBaseHierarchy);

                RpHAnimHierarchySetFlags( m_pBaseHierarchy,
                    (RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
                    rpHANIMHIERARCHYUPDATELTMS | 
                    rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));

                RpHAnimHierarchyAttach(m_pBaseHierarchy);                
            }
		}
	}

	if (m_pClumpResource != NULL && !m_strAnimationName.empty())
	{
		const RwChar* strPathName = CVenusPlayerEventManager::GetInstance().m_AnimationFileManager.GetPathName(m_strAnimationName.c_str());
		m_pAnimationResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(strPathName);
		if (m_pAnimationResource != NULL)
		{
			API_SetAnimation(m_pClumpResource->GetClump(), m_pAnimationResource->GetAnimation());
		}
	}

	svdef_EVENT::iterator it;
	for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
	{
		EVENT* pEvent = (*it);
		pEvent->bFadeIn		= TRUE;
		pEvent->bFadeOut	= TRUE;
	}

	m_fLifeTime = 0.f;
}


void CVenusPlayer::CopyData(CVenusPlayer* pSrcPlayer)
{
    DeleteEvent();

	m_strClumpName		= pSrcPlayer->m_strClumpName.c_str();
	m_strAnimationName	= pSrcPlayer->m_strAnimationName.c_str();

	svdef_EVENT::iterator it;
	for (it = pSrcPlayer->m_svEvent.begin(); it != pSrcPlayer->m_svEvent.end(); ++ it)
	{
		EVENT* pEvent = NTL_NEW EVENT;
		pEvent->strEffectName	= (*it)->strEffectName.c_str();
		pEvent->nBoneIndex		= (*it)->nBoneIndex;

		pEvent->bFadeIn			= (*it)->bFadeIn;
		pEvent->bFadeOut		= (*it)->bFadeOut;

		pEvent->fFadeInTime		= (*it)->fFadeInTime;
		pEvent->fFadeOutTime	= (*it)->fFadeOutTime;
		pEvent->nFadeOutType	= (*it)->nFadeOutType;

		pEvent->pEffectEntity	= NULL;

		m_svEvent.push_back(pEvent);
	}
}

//------------------------------------------------------------------
//	FuncName	: InitializeEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::InitializeEvent(EVENT* pEvent)
{
	pEvent->fFadeInTime	= 0.f;
	pEvent->fFadeOutTime	= 0.2f;
	pEvent->nFadeOutType	= FADE_OUT_NONE;

	pEvent->bFadeIn			= TRUE;
	pEvent->bFadeOut		= TRUE;
	pEvent->nBoneIndex		= -1;

	pEvent->pEffectEntity	= NULL;
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayer::Update(RwReal fElapsedTime)
{
	m_fLifeTime += fElapsedTime;

	svdef_EVENT::iterator it;
	for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
	{
		EVENT* pEvent = (*it);
		if (pEvent->bFadeIn && m_fLifeTime > pEvent->fFadeInTime)
		{
			if (pEvent->pEffectEntity == NULL && !pEvent->strEffectName.empty())
			{
				pEvent->pEffectEntity = CVenusVisualManager::GetInstance().CreateEventEffect(pEvent->strEffectName.c_str());
			}
			pEvent->bFadeIn = FALSE;
		}
		if (pEvent->bFadeOut && m_fLifeTime > pEvent->fFadeOutTime)
		{
			if (pEvent->pEffectEntity != NULL)
			{
				switch(pEvent->nFadeOutType)
				{
				case FADE_OUT_DELEETE:
					{
						CVenusVisualManager::GetInstance().DeleteEventEffect(pEvent->pEffectEntity);
						pEvent->pEffectEntity = NULL;
					}
					break;
				case FADE_OUT_STOP:
					{
						CNtlInstanceEffect* pInstanceEffect = (CNtlInstanceEffect*)pEvent->pEffectEntity;
						pInstanceEffect->Finish();
					}
					break;
				case FADE_OUT_BALL:
					{
						RwMatrix matWorld = pEvent->pEffectEntity->GetMatrix();

/*						RwV3d vPosition = pEvent->pEffectEntity->GetPosition();
						RwMatrix matWorld;
						RwMatrixTranslate(&matWorld, &vPosition, rwCOMBINEREPLACE);
*/

						RwV3d vTargetPosition;
						if (CVenusPlayerEventManager::GetInstance().m_EventEmemy.m_pClumpResource != NULL)
						{
							RwSphere sphere;
							API_CalculationShpere(sphere, CVenusPlayerEventManager::GetInstance().m_EventEmemy.m_pClumpResource->GetClump());
							vTargetPosition = sphere.center;
						}
						else
						{
							vTargetPosition.x = vTargetPosition.y = 0.f;
							vTargetPosition.z = CVenusConfig::GetInstance().m_fEventBallSpeed * CVenusConfig::GetInstance().m_fEventBallLifeTime;
						}

						pEvent->pEffectEntity->SetMatrix(matWorld);
						CVenusEventBall* pVenusEventBall = NTL_NEW CVenusEventBall(pEvent->pEffectEntity, vTargetPosition);
						CVenusPlayerEventManager::GetInstance().m_slEventBall.push_back(pVenusEventBall);
						pEvent->pEffectEntity = NULL;
					}
					break;
				}
			}
			pEvent->bFadeOut = FALSE;
		}
	}


	// Animation
	if (m_pAnimationResource && m_pAnimationResource->GetAnimation() != NULL)
	{
		RpHAnimHierarchyAddAnimTime(m_pBaseHierarchy, fElapsedTime);
		RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);

		if (m_fCurrentTime > m_pBaseHierarchy->currentAnim->currentTime)
		{
			m_fLifeTime		= m_pBaseHierarchy->currentAnim->currentTime;

			svdef_EVENT::iterator it;
			for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
			{
				EVENT* pEvent = (*it);
				pEvent->bFadeIn		= TRUE;
				pEvent->bFadeOut	= TRUE;
			}
		}
		m_nMaxTime		= m_pBaseHierarchy->currentAnim->pCurrentAnim->numFrames;
		m_fCurrentTime	= m_pBaseHierarchy->currentAnim->currentTime;
	}

	for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
	{
		EVENT* pEvent = (*it);
		if (pEvent->pEffectEntity != NULL && pEvent->nBoneIndex > 0)
		{
			const RwMatrix* matrices = GetNodeMatrix(pEvent->nBoneIndex);
			if (matrices != NULL)
			{
				RwMatrix matEffect = *matrices;
				pEvent->pEffectEntity->SetMatrix(matEffect);
			}
		}
		if (pEvent->pEffectEntity)
		{
			if (!pEvent->pEffectEntity->Update(fElapsedTime))
			{
				if (pEvent->nFadeOutType == FADE_OUT_STOP)
				{
					CVenusVisualManager::GetInstance().DeleteEventEffect(pEvent->pEffectEntity);
					pEvent->pEffectEntity = NULL;
				}
			}
		}
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayer::Render()
{
	if (m_pClumpResource && m_pClumpResource->GetClump() != NULL)
	{
		RpClumpRender(m_pClumpResource->GetClump());
	}
	svdef_EVENT::iterator it;
	for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
	{
		EVENT* pEvent = (*it);
		if (pEvent->pEffectEntity)
		{
			pEvent->pEffectEntity->Render();
		}
	}
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: SetWorldMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusPlayer::SetWorldMatrix(const RwMatrix& matWorld)
{
	if (m_pClumpResource && m_pClumpResource->GetClump() != NULL)
	{
		RwFrame* pRrame = RpClumpGetFrame(m_pClumpResource->GetClump());
		RwFrameTransform(pRrame, &matWorld, rwCOMBINEREPLACE);
	}
}

//------------------------------------------------------------------
//	FuncName	: GetNumNode
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwInt32 CVenusPlayer::GetNumNode()
{
    if(!m_pBaseHierarchy)
        return 0;

	return m_pBaseHierarchy->numNodes;
}

//------------------------------------------------------------------
//	FuncName	: GetNodeMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
const RwMatrix* CVenusPlayer::GetNodeMatrix(RwInt32 nNode)
{
    if(!m_pBaseHierarchy)
        return NULL;

    RwMatrix *matrices = RpHAnimHierarchyGetMatrixArray(m_pBaseHierarchy);
	return &matrices[nNode];
}


//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayer::Save(FILE* pFile)
{
	RwInt32 length = (RwInt32)m_strClumpName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strClumpName.c_str(),	sizeof(RwChar),								length,		pFile);

	length = (RwInt32)m_strAnimationName.size() + 1;
	fwrite(&length,					sizeof(RwInt32),							1,			pFile);
	fwrite(m_strAnimationName.c_str(),	sizeof(RwChar),							length,		pFile);

	// ============================================================================
	// Count
	// ============================================================================
	RwInt32 nEvent = (RwInt32)m_svEvent.size();
	fwrite(&nEvent,				sizeof(RwInt32),							1,			pFile);
	if (nEvent > 0)
	{
		svdef_EVENT::iterator it;
		for (it = m_svEvent.begin(); it != m_svEvent.end(); ++ it)
		{
			EVENT* pEvent = (*it);

			length = (RwInt32)pEvent->strEffectName.size() + 1;
			fwrite(&length,					sizeof(RwInt32),							1,			pFile);
			fwrite(pEvent->strEffectName.c_str(),	sizeof(RwChar),						length,		pFile);


			fwrite(&pEvent->nBoneIndex,				sizeof(RwInt32),					1,			pFile);

			fwrite(&pEvent->bFadeIn,				sizeof(RwBool),						1,			pFile);
			fwrite(&pEvent->bFadeOut,				sizeof(RwBool),						1,			pFile);

			fwrite(&pEvent->fFadeInTime,			sizeof(RwReal),						1,			pFile);
			fwrite(&pEvent->fFadeOutTime,			sizeof(RwReal),						1,			pFile);

			fwrite(&pEvent->nFadeOutType,			sizeof(RwInt32),					1,			pFile);
		}
	}

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayer::Load(FILE* pFile)
{
	DeleteEffectEntity();
	DeleteEvent();
	Delete();

	RwChar strName[MAX_PATH];
	RwInt32 length;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strClumpName = strName;

	fread(&length,					sizeof(RwInt32),							1,			pFile);
	memset(strName, 0, MAX_PATH);
	fread(strName,					sizeof(RwChar),								length,		pFile);

	m_strAnimationName = strName;

	// ============================================================================
	// Player Event 
	// ============================================================================
	RwInt32 nEvent;
	fread(&nEvent,					sizeof(RwInt32),							1,			pFile);
	if (nEvent > 0)
	{
		for (RwInt32 i = 0; i < nEvent; ++ i)
		{
			EVENT* pEvent = NTL_NEW EVENT;
			InitializeEvent(pEvent);

			RwInt32 length;
			fread(&length,							sizeof(RwInt32),					1,			pFile);

			memset(strName, 0, MAX_PATH);
			fread(strName,							sizeof(RwChar),						length,		pFile);
			pEvent->strEffectName = strName;


			fread(&pEvent->nBoneIndex,				sizeof(RwInt32),					1,			pFile);

			fread(&pEvent->bFadeIn,					sizeof(RwBool),						1,			pFile);
			fread(&pEvent->bFadeOut,				sizeof(RwBool),						1,			pFile);

			fread(&pEvent->fFadeInTime,				sizeof(RwReal),						1,			pFile);
			fread(&pEvent->fFadeOutTime,			sizeof(RwReal),						1,			pFile);

			fread(&pEvent->nFadeOutType,			sizeof(RwInt32),					1,			pFile);

			m_svEvent.push_back(pEvent);
		}
	}

	return TRUE;
}

RwChar* CVenusPlayer::GetBoneName(int nIndex)
{
    if(nIndex < 0  || !m_pBaseHierarchy)
        return NULL;

    RwFrame* pFrame = m_pBaseHierarchy->pNodeInfo[nIndex].pFrame;
    if(!pFrame)
        return NULL;

    RwChar* sBoneName = Helper_BoneName(pFrame);

    return sBoneName;
}
