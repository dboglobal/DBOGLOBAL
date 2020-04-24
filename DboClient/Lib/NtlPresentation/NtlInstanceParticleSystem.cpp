#include "precomp_ntlpresentation.h"
#include ".\ntlinstanceparticlesystem.h"
#include "NtlDebug.h"

#include ".\ntleffectsystemfreelist.h"

#include "NtlPLRenderState.h"
#include "NtlMath.h"
#include "NtlPLGlobal.h"

static RpPrtStdParticleBatch* NtlStdParticleUpdateCB(RpPrtStdEmitter *emt, RpPrtStdParticleBatch *prtBatch, void * data __RWUNUSED__);

#define PARTICLEPROP(_ptr, _offset) (((char *)(_ptr)) + (_offset))
#define RPPRTSTDPROP(_ptr, _offset) (((char *)(_ptr)) + (_offset))

RpPrtStdEmitterPTank* emtPTank = NULL;
RpPrtStdEmitterStandard* emtStd = NULL;
RwInt32 g_nVelocityOffset = 0;
RwReal g_fElapsedTime1 = 0;
RwV3d g_vOrbitPoint;

CNtlInstanceParticleSystem::CNtlInstanceParticleSystem(void)
:m_pEmitterPrtMatrix(NULL)
{
	m_pEmitter			= NULL;
	m_pEmitterStandard	= NULL;    

	m_pEmitterClass		= NULL;
	m_pParticleClass	= NULL;

	m_pAtomic			= NULL;
	m_pTankProperties	= NULL;
	m_pTrailSystem      = NULL;
    

    ZeroMemory(&m_vPosPrev, sizeof(m_vPosPrev));    
    ZeroMemory(&m_vPosDelta, sizeof(m_vPosDelta));

	Init();
}

CNtlInstanceParticleSystem::~CNtlInstanceParticleSystem(void)
{
	Delete();
}


void* CNtlInstanceParticleSystem::operator new(size_t size)
{
	NTL_FUNCTION("CNtlInstanceEffect::operator new");
    
	NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE));
}

void CNtlInstanceParticleSystem::operator delete(void *pObj)
{
	CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE, pObj);
}

//------------------------------------------------------------------
//	FuncName	: Init
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceParticleSystem::Init()
{
	CNtlInstanceComponentSystem::Init();

	m_pResourceParticleSystem	= NULL;

	NTL_ASSERTE(m_pEmitterClass		== NULL);
	NTL_ASSERTE(m_pParticleClass	== NULL);

	NTL_ASSERTE(m_pAtomic			== NULL);
	NTL_ASSERTE(m_pTankProperties	== NULL);

	m_nVelocityOffset		= 0;

	m_fHurricaneLifeTime	= 0.f;
	m_fHurricaneDeltaTime	= 0.f;

	m_pStandardTexture = NULL;

    m_uiMemoryUseSize += sizeof(CNtlInstanceParticleSystem);
}   

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceParticleSystem::Reset()
{
	Delete();
}

//------------------------------------------------------------------
//	FuncName	: Delete
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceParticleSystem::Delete()
{
    NTL_DELETE(m_pTrailSystem);

	if (m_pTankProperties != NULL)
	{
		if (m_pTankProperties->pTank != NULL)
		{
			RpPTankAtomicSetTexture(m_pTankProperties->pTank, NULL);
		}
	}
	if (m_pEmitterStandard != NULL)
	{
		if (m_pEmitterStandard->texture != NULL)
		{			
			CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_pEmitterStandard->texture);
			m_pEmitterStandard->texture = NULL;
		}
	}

	if(NULL != m_pAtomic)
	{
		RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
		if(NULL != pFrame)
		{
			RpAtomicSetFrame(m_pAtomic, NULL);
			RwFrameDestroy(pFrame);
		}
		RpAtomicDestroy(m_pAtomic);
		m_pAtomic = NULL;
	}
	if(NULL != m_pEmitterClass)
	{
		RpPrtStdEClassDestroy(m_pEmitterClass);
		m_pEmitterClass = NULL;
		m_pTankProperties = NULL;
	}
	if(NULL != m_pParticleClass)
	{
		RpPrtStdPClassDestroy(m_pParticleClass);
		m_pParticleClass = NULL;
	}
	m_pEmitter			= NULL;
	m_pEmitterStandard	= NULL;

	CNtlInstanceComponentSystem::Delete();
}

//------------------------------------------------------------------
//	FuncName	: Create
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceParticleSystem::Create(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    
	m_pResourceParticleSystem = (CNtlResourceParticleSystem*) pResourceComponentSystem;

	if (!m_pResourceParticleSystem->CreateParticleClass(m_pEmitterClass, m_pParticleClass))
	{
		return FALSE;
	}

	m_pAtomic = RpPrtStdAtomicCreate(m_pEmitterClass, NULL);    
	if(NULL == m_pAtomic)
	{
		return FALSE;
	}

	RwInt32 nOffset = 0;

	/* Get the emitter Pointer */
	m_pEmitter = RpPrtStdAtomicGetEmitter(m_pAtomic);

	/* Set particle class and particle batch size */
	RpPrtStdEmitterSetPClass(m_pEmitter, m_pParticleClass, m_pResourceParticleSystem->m_nParticleBatchMaxCount);

	/* Get the Emitter Standard properties pointer */
	nOffset = RpPrtStdPropTabGetPropOffset(m_pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERSTANDARD);
	m_pEmitterStandard = (RpPrtStdEmitterStandard *) (((RwChar *)m_pEmitter) + nOffset);	
	// Standard Resource Data Copy
	memcpy(m_pEmitterStandard, &m_pResourceParticleSystem->m_EmitterStandard, sizeof(RpPrtStdEmitterStandard));
	

	if (!m_pResourceParticleSystem->ResourceDataCopy(m_pEmitterClass, m_pEmitter))
	{
		return FALSE;
	}

	// rpPRTSTDEMITTERDATAFLAGPRTMATRIX It is an error to use attributes and Sub systems together.
	if (!m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX) && 
		m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER) && 
		!m_pResourceParticleSystem->m_strSubSystem.empty())
	{
		CNtlResourceComponentSystem* pSubSystem = pResourceEffect->GetResourceSystem(m_pResourceParticleSystem->m_strSubSystem.c_str());
		if (pSubSystem != NULL)
		{
			NTL_ASSERTE(pSubSystem);
			NTL_ASSERTE(pSubSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE);

			RpPrtStdEmitter* subEmitter = SubEmitterCreate((CNtlResourceParticleSystem*)pSubSystem);
			subEmitter->flag &= ~rpPRTSTDEMITTERFLAGEMIT;
			m_pEmitter = RpPrtStdEmitterAddEmitter(subEmitter, m_pEmitter);
		}
	}

	nOffset = RpPrtStdPropTabGetPropOffset(m_pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPTANK);
	m_pTankProperties = (RpPrtStdEmitterPTank *) (((RwChar *)m_pEmitter) + nOffset);   

	m_nVelocityOffset = RpPrtStdPropTabGetPropOffset(m_pParticleClass->propTab, rpPRTSTDPROPERTYCODEPARTICLEVELOCITY);
	

	/* Setup a frame for the emitter */
	//SetWorldMatrix(matWorld); // this crashes
	RwFrame *frame = RwFrameCreate();
	RwFrameTransform(frame, &m_matRender, rwCOMBINEREPLACE);
	RpAtomicSetFrame(m_pAtomic, frame);

	// Put GetTickCount in the seed to make it random
	m_pEmitterStandard->seed = GetTickCount();

	if (!m_pResourceParticleSystem->m_strTextureName.empty())
	{
		m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceParticleSystem->m_strTextureName);
		m_pEmitterStandard->texture = m_pCurrentTexture;
	}

	if (m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
	{
		RpPrtStdParticleCallBackArray prtCB[2];
		for (RwInt32 j = 0; j < 2; j++)
		{
			for (RwInt32 i = 0; i < rpPRTSTDPARTICLECALLBACKMAX; i++)
			{
				prtCB[j][i] = NULL;
			}
		}

		prtCB[1][rpPRTSTDPARTICLECALLBACKUPDATE] = RpPrtStdParticleStdUpdateCB;        
		prtCB[0][rpPRTSTDPARTICLECALLBACKUPDATE] = NtlStdParticleUpdateCB;

		RpPrtStdPClassSetCallBack(m_pParticleClass, 2, prtCB);
	}

    // Trail Action
    if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        m_pTrailSystem = NTL_NEW CNtlInstanceTrailSystem();
        m_pTrailSystem->Create(this, &m_pResourceParticleSystem->m_EmitterTrail);
        m_uiMemoryUseSize += m_pTrailSystem->GetMemoryUseSize();
    }

    // Accept the Matrix pointer.
    if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
    {
        nOffset = RpPrtStdPropTabGetPropOffset(m_pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTMATRIX);
        m_pEmitterPrtMatrix = (RpPrtStdEmitterPrtMatrix *) (((RwChar *)m_pEmitter) + nOffset);
        m_vOrgAt = m_pEmitterPrtMatrix->prtPosMtxAt;
        m_vOrgUp = m_pEmitterPrtMatrix->prtPosMtxUp;
    }   

    // Low Spec
    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        m_pEmitterStandard->emtEmitGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Stop
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceParticleSystem::Stop()
{
	// update 가 더 이상 필요 없으면 생성되는 파티클갯수를 0으로 해 버린다
	m_pEmitterStandard->emtPrtEmit = 0;
	m_pEmitterStandard->emtPrtEmitBias = 0;
	m_fFadeOutTime = 0.f;
	m_bUpdate = FALSE; 
    m_bStart = FALSE;
}

RpPrtStdEmitter* CNtlInstanceParticleSystem::SubEmitterCreate(CNtlResourceParticleSystem* pResourceSystem)
{
	if (pResourceSystem == NULL) return NULL;

	RpPrtStdEmitterClass        *eClass;
	RpPrtStdParticleClass       *pClass;
	RpPrtStdEmitter             *emitter;
	RpPrtStdEmitterStandard     *emtStd;

	if (!pResourceSystem->CreateParticleClass(eClass, pClass))
	{
		return NULL;
	}

	RwInt32 nOffset;

    emitter = RpPrtStdEmitterCreate(eClass, NULL);

    /* Set particle class and particle batch size */
    RpPrtStdEmitterSetPClass(emitter, pClass, pResourceSystem->m_nParticleBatchMaxCount);

	nOffset = RpPrtStdPropTabGetPropOffset(eClass->propTab, rpPRTSTDPROPERTYCODEEMITTERSTANDARD);
	emtStd = (RpPrtStdEmitterStandard *) (((RwChar *)emitter) + nOffset);

	if (!pResourceSystem->ResourceDataCopy(eClass, emitter))
	{
		return FALSE;
	}

	memcpy(emtStd, &pResourceSystem->m_EmitterStandard, sizeof(RpPrtStdEmitterStandard));
	if (!pResourceSystem->m_strTextureName.empty())
	{
		emtStd->texture = CreateTexture(pResourceSystem->m_strTextureName);
	}
	emtStd->seed = GetTickCount();

    return emitter;
}


//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceParticleSystem::Update(RwReal fElapsedTime)
{
    CNtlInstanceComponentSystem::Update(fElapsedTime);

	if (m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
	{
		emtStd = m_pEmitterStandard;
		emtPTank = m_pTankProperties;
		g_nVelocityOffset = m_nVelocityOffset;
		g_fElapsedTime1 = fElapsedTime;
		g_vOrbitPoint = m_pResourceParticleSystem->m_EmitterPrtOrbitPoint.point;
	}

	m_fLifeTime += fElapsedTime;
	if (m_bStart)
	{
		if (m_pResourceParticleSystem->m_fStartTime < m_fLifeTime)
		{
			m_bStart	= FALSE;
			m_bReady	= TRUE;
			m_bUpdate	= TRUE;

			// 스타트가 되면 라이프 타임을 초기화 한다.
			m_fLifeTime = fElapsedTime;
		}
		else
		{
			return TRUE;
		}
	}

	
	if (m_bUpdate && 
		m_pResourceParticleSystem->m_bEternity == FALSE && 
		m_fLifeTime >= m_pResourceParticleSystem->m_fLifeTime)
	{
		Stop();        
	}
	

	if (m_bReady)
	{
        // spline
		if (m_pSplinePath != NULL)
		{
			SetWorldMatrix(m_matWorld);
			m_pSplinePath->FrameAnimation(m_fLifeTime, m_matPathWorld, m_pResourceParticleSystem->m_EmitterPrtSplinePath.loop, m_pResourceParticleSystem->m_EmitterPrtSplinePath.useRotate, GetScale());

			RwMatrixMultiply(&m_matPathRender, &m_matPathWorld, &m_pResourceParticleSystem->m_matPathWorld);

			m_matTempRender = m_matRender;
			RwMatrixMultiply(&m_matRender, &m_matPathRender, &m_matTempRender);

			RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
			RwFrameTransform(pFrame, &m_matRender, rwCOMBINEREPLACE);
		}

        // Hurricane
		else if (m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE))
		{
			m_fHurricaneDeltaTime += fElapsedTime;
			RwReal fAngle = m_pResourceParticleSystem->m_EmitterPrtHurricane.revolutionAngle / 360.f;
			while(m_fHurricaneDeltaTime > fAngle) 
			{
				m_fHurricaneLifeTime += fAngle;
				m_fHurricaneDeltaTime -= fAngle;
				RwV3d vPosition;
				BuildHurricanePoint(vPosition, m_fHurricaneLifeTime);

				SetWorldMatrix(m_matWorld);

				RwMatrix matHurricane;
				RwMatrixTranslate(&matHurricane, &vPosition, rwCOMBINEREPLACE);

				m_matTempRender = m_matRender;
				RwMatrixMultiply(&m_matRender, &m_matTempRender, &matHurricane);

				RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
				RwFrameTransform(pFrame, &m_matRender, rwCOMBINEREPLACE);

				RpPrtStdAtomicUpdate(m_pAtomic, &fAngle);
			}
		}

        //// Follow Particles Update
        if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTADVEMITTERDATAFLAGFOLLOW))        
        {
            UpdateFollow();
        }


        if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
        {
            if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD) ||
               m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD) ||
               m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD))
            {
                UpdateBillboard();                        
            }            

            //UpdateRotate();
        }

		// Render Ware Paricle System Update
		RpPrtStdAtomicUpdate(m_pAtomic, &fElapsedTime);

		if (m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
		{
			if (UpdateMultiTexture(fElapsedTime))
			{
				RpPTankAtomicSetTexture(m_pTankProperties->pTank, m_pCurrentTexture);
			}
		}

        // Trail Aciton
        if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
        {
            if(m_pTrailSystem)
            {
                m_pTrailSystem->Update(fElapsedTime);
            }
        }
	}

    if (m_bUpdate == FALSE)
    {
        if (m_pEmitter->prtActive <= 0)
        {
            m_bReady = FALSE;
            return FALSE;
        }
    }

    // 바운딩 박스 계산    
    if(CNtlResourceEffect::m_bUpdateBoundingSphere)
    {
        m_pResourceParticleSystem->SetBoundingSphereRadius(RpAtomicGetWorldBoundingSphere(m_pAtomic)->radius);
    }    

	return m_bReady;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CNtlInstanceParticleSystem::Render()
{
	if (!m_bReady || !m_bVisible) return TRUE;

	if (m_pResourceParticleSystem->m_bAlphaBlend)
	{
		RpPTankAtomicSetVertexAlpha(m_pTankProperties->pTank, TRUE);
		RpPTankAtomicSetBlendModes(m_pTankProperties->pTank, m_pResourceParticleSystem->m_nSrcBlend, m_pResourceParticleSystem->m_nDestBlend);        
		BeginEffectParticleSystemState();	
	}

	// 2Side
	if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
		BeginNtlAtomicTwoSideState();
	}	

	// Z Write
	if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE))
	{
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);
		RwD3D9SetRenderState(D3DRS_DEPTHBIAS, *((DWORD*)&m_pResourceParticleSystem->m_fDepthBias));	
	}	

	RpAtomicRender(m_pAtomic);

	if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE))
	{
		RwD3D9SetRenderState(D3DRS_DEPTHBIAS, 0x00000000);	
	}

	// 2Side
	if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
		EndNtlAtomicTwoSideState();
	}

	if (m_pResourceParticleSystem->m_bAlphaBlend)
	{
		EndEffectParticleSystemState();
		RpPTankAtomicSetVertexAlpha(m_pTankProperties->pTank, FALSE);
		RpPTankAtomicSetBlendModes(m_pTankProperties->pTank, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
	}


    // Trail Action Render
    if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        if(m_pTrailSystem)
        {
            m_pTrailSystem->Render();
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
void CNtlInstanceParticleSystem::SetWorldMatrix(const RwMatrix& matWorld)
{
	CNtlInstanceComponentSystem::SetWorldMatrix(matWorld);

	if (m_pAtomic != NULL)
	{
		RwFrame* pFrame = RpAtomicGetFrame(m_pAtomic);
		
		
		RwFrameTransform(pFrame, &m_matRender, rwCOMBINEREPLACE);
		
        UpdateRotate();
		RwFrameUpdateObjects(pFrame);
	}
}

//------------------------------------------------------------------
//	FuncName	: BuildHurricanePoint
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CNtlInstanceParticleSystem::BuildHurricanePoint(RwV3d& vPosition, RwReal fLifeTime)
{
	RwReal constant = (2.0f * rwPI) * (fLifeTime * m_pResourceParticleSystem->m_EmitterPrtHurricane.revolutionCount);

	vPosition.x = (RwReal)RwCos(constant) * m_pResourceParticleSystem->m_EmitterPrtHurricane.radius;
	vPosition.y = (RwReal)RwSin(constant) * m_pResourceParticleSystem->m_EmitterPrtHurricane.radius;
	vPosition.z = fLifeTime * m_pResourceParticleSystem->m_EmitterPrtHurricane.fSpeed;
}

/**
 * Follow 파티클의 위치를 갱신한다. 
 */
void CNtlInstanceParticleSystem::UpdateFollow()
{
    if(!(m_vPosPrev.x == 0.0f && m_vPosPrev.y == 0.0f && m_vPosPrev.z == 0.0f))
    {    
        m_vPosDelta = m_matRender.pos - m_vPosPrev;
    }

    m_vPosPrev = m_matRender.pos;

    if(!m_pTankProperties->pTank)
        return;    

    if(m_vPosDelta.x != 0.0f || m_vPosDelta.y != 0.0f || m_vPosDelta.z != 0.0f)
    {
        RpPTankLockStruct posLock;        
        if(RpPTankAtomicLock(m_pTankProperties->pTank, &posLock, rpPTANKLFLAGPOSITION, rpPTANKLOCKWRITE))
        {
            for(size_t i = 0; i < m_pTankProperties->numPrt; ++i)
            {
                RwV3d* pos = (RwV3d*)posLock.data;

                *pos += m_vPosDelta;                

                posLock.data += posLock.stride;
            }

            m_vPosDelta.x = 0.0f;
            m_vPosDelta.y = 0.0f;
            m_vPosDelta.z = 0.0f;
            RpPTankAtomicUnlock(m_pTankProperties->pTank);
        }        
    }
}

void CNtlInstanceParticleSystem::UpdateBillboard() 
{
    if(!m_pTankProperties->pTank)
        return;

    RpPTankLockStruct posLock;        
    if(RpPTankAtomicLock(m_pTankProperties->pTank, &posLock, rpPTANKLFLAGMATRIX, rpPTANKLOCKWRITE))
    {
        RwMatrix matCamera = *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));        

        for(size_t i = 0; i < m_pTankProperties->numPrt; ++i)
        {
            RwV3d vCameraPos = *RwMatrixGetPos(&matCamera);      
            RwMatrix* pMatParticle = (RwMatrix*)posLock.data;

            RwV3d vParticlePos = *RwMatrixGetPos(pMatParticle);
            *RwMatrixGetPos(pMatParticle) = ZeroAxis;

            RwV3d vAtParticle = *RwMatrixGetAt(pMatParticle);

            RwMatrix matParticle, matBillboard;

            if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD))
            {
                vAtParticle.y = 0.0f;
                if(RwV3dLength(&vAtParticle) == 0)
                    continue;

                RwV3dNormalize(&vAtParticle, &vAtParticle);
                RwV3d vRightParticle;
                RwV3dCrossProduct(&vRightParticle, &vAtParticle, &YAxis);
                RwV3dNormalize(&vRightParticle, &vRightParticle);

                // 파티클의 At 방향을 가지고 파티클의 기본축 행렬을 만든다.
                matParticle = *pMatParticle;
                *RwMatrixGetAt(&matParticle) = vAtParticle;
                *RwMatrixGetRight(&matParticle) = vRightParticle;
                *RwMatrixGetUp(&matParticle) = YAxis;
                *RwMatrixGetPos(&matParticle) = ZeroAxis;

                vCameraPos.y = 0.0f;
                RwV3d vParticlePosTemp = vParticlePos;
                vParticlePosTemp.y = 0.0f;
                RwV3d vAt = vCameraPos - vParticlePosTemp;
                RwV3dNormalize(&vAt, &vAt);

                RwV3d vRight;
                RwV3dCrossProduct(&vRight, &vAt, &YAxis);
                RwV3dNormalize(&vRight, &vRight);

                matBillboard = *pMatParticle;            
                *RwMatrixGetAt(&matBillboard) = vAt;
                *RwMatrixGetRight(&matBillboard) = vRight;
                *RwMatrixGetUp(&matBillboard) = YAxis;
                *RwMatrixGetPos(&matBillboard) = ZeroAxis;
            }
            else if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD))
            {
                vAtParticle.x = 0.0f;
                if(RwV3dLength(&vAtParticle) == 0)
                    continue;

                RwV3dNormalize(&vAtParticle, &vAtParticle);
                RwV3d vRightParticle;
                RwV3dCrossProduct(&vRightParticle, &vAtParticle, &XAxis);
                RwV3dNormalize(&vRightParticle, &vRightParticle);

                // 파티클의 At 방향을 가지고 파티클의 기본축 행렬을 만든다.
                matParticle = *pMatParticle;
                *RwMatrixGetAt(&matParticle) = vAtParticle;
                *RwMatrixGetRight(&matParticle) = vRightParticle;
                *RwMatrixGetUp(&matParticle) = XAxis;
                *RwMatrixGetPos(&matParticle) = ZeroAxis;

                vCameraPos.x = 0.0f;
                RwV3d vParticlePosTemp = vParticlePos;
                vParticlePosTemp.x = 0.0f;
                RwV3d vAt = vCameraPos - vParticlePosTemp;
                RwV3dNormalize(&vAt, &vAt);

                RwV3d vRight;
                RwV3dCrossProduct(&vRight, &vAt, &XAxis);
                RwV3dNormalize(&vRight, &vRight);

                matBillboard = *pMatParticle;            
                *RwMatrixGetAt(&matBillboard) = vAt;
                *RwMatrixGetRight(&matBillboard) = vRight;
                *RwMatrixGetUp(&matBillboard) = XAxis;
                *RwMatrixGetPos(&matBillboard) = ZeroAxis;
            }
            else if(m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD))
            {
                vAtParticle.z = 0.0f;
                if(RwV3dLength(&vAtParticle) == 0)
                    continue;

                RwV3dNormalize(&vAtParticle, &vAtParticle);
                RwV3d vRightParticle;
                RwV3dCrossProduct(&vRightParticle, &vAtParticle, &ZAxis);
                RwV3dNormalize(&vRightParticle, &vRightParticle);

                // 파티클의 At 방향을 가지고 파티클의 기본축 행렬을 만든다.
                matParticle = *pMatParticle;
                *RwMatrixGetAt(&matParticle) = vAtParticle;
                *RwMatrixGetRight(&matParticle) = vRightParticle;
                *RwMatrixGetUp(&matParticle) = ZAxis;
                *RwMatrixGetPos(&matParticle) = ZeroAxis;

                vCameraPos.z = 0.0f;
                RwV3d vParticlePosTemp = vParticlePos;
                vParticlePosTemp.z = 0.0f;
                RwV3d vAt = vCameraPos - vParticlePosTemp;
                RwV3dNormalize(&vAt, &vAt);

                RwV3d vRight;
                RwV3dCrossProduct(&vRight, &vAt, &ZAxis);
                RwV3dNormalize(&vRight, &vRight);

                matBillboard = *pMatParticle;            
                *RwMatrixGetAt(&matBillboard) = vAt;
                *RwMatrixGetRight(&matBillboard) = vRight;
                *RwMatrixGetUp(&matBillboard) = ZAxis;
                *RwMatrixGetPos(&matBillboard) = ZeroAxis;
            }
            else
            {
                break;
            }

            // A * M = B
            // M = A(-1) * B
            // 기본축 행렬을 빌보드 행렬로 변환하는 변환행렬을 구한다.
            RwMatrix matTemp = matParticle;
            RwMatrixInvert(&matParticle, &matTemp);            
            RwMatrix matResult;            
            RwMatrixMultiply(&matResult, &matParticle, &matBillboard);

            // 변환행렬을 원래 파티클의 행렬에 적용한다.
            matTemp = *pMatParticle;
            RwMatrixMultiply(pMatParticle, &matTemp, &matResult);
            *RwMatrixGetPos(pMatParticle) = vParticlePos;

            posLock.data += posLock.stride;
        }
        
        RpPTankAtomicUnlock(m_pTankProperties->pTank);
    }        
}

void CNtlInstanceParticleSystem::UpdateRotate() 
{
    // Matrix Action이 적용되어 있지 않거나, 빌보드가 적용되어 있으면 Rotate를 먹지 않는다.
    if(!m_pEmitterPrtMatrix || m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD) ||
                               m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD) || 
                               m_pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD))
        return;
    
    RwMatrix matRender = m_matRender;
    *RwMatrixGetPos(&matRender) = ZeroAxis;

    RwV3dTransformVector(&m_pEmitterPrtMatrix->prtPosMtxUp, &m_vOrgUp, &matRender);
    RwV3dTransformVector(&m_pEmitterPrtMatrix->prtPosMtxAt, &m_vOrgAt, &matRender);    
}

void CNtlInstanceParticleSystem::SetScale( RwReal fScale ) 
{
    RwReal fFinalScale = fScale / m_fScale;         // Scale의 누적을 막기위해, 현재 스케일값을 나눈후에 다시 곱해준다.

    // 기본 Size
    m_pEmitterStandard->emtSize *= fFinalScale;
    m_pEmitterStandard->prtSize *= fFinalScale;
	
	// size Action
    if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
    {
        RwInt32 nOffset = RpPrtStdPropTabGetPropOffset(m_pEmitterClass->propTab, rpPRTSTDPROPERTYCODEEMITTERPRTSIZE);
        RpPrtStdEmitterPrtSize* pEmitterPrtSize = (RpPrtStdEmitterPrtSize*) (((RwChar *)m_pEmitter) + nOffset);	

        memcpy(pEmitterPrtSize, &m_pResourceParticleSystem->m_EmitterPrtSize, sizeof(RpPrtStdEmitterPrtSize));
        pEmitterPrtSize->prtStartSize		*= fFinalScale;
        pEmitterPrtSize->prtStartSizeBias	*= fFinalScale;
        pEmitterPrtSize->prtEndSize			*= fFinalScale;
        pEmitterPrtSize->prtEndSizeBias		*= fFinalScale;
    }

	// multi size Action		
	if(m_pResourceParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		RwInt32 nOffset = RpPrtStdPropTabGetPropOffset(m_pEmitterClass->propTab, rpPRTADVPROPERTYCODEEMITTERPRTMULTISIZE);
		RpPrtAdvPrtMultiSizeEmitter* pEmitterMultiSize = (RpPrtAdvPrtMultiSizeEmitter*)(((RwChar*)m_pEmitter) + nOffset);

		memcpy(pEmitterMultiSize->list, m_pResourceParticleSystem->m_EmitterPrtMultiSize.list, sizeof(RpPrtAdvEmtPrtSizeItem) * pEmitterMultiSize->numSize);
		for(RwUInt32 i = 0; i < pEmitterMultiSize->numSize; ++i)
		{
			pEmitterMultiSize->list[i].midSize *= fFinalScale;
			pEmitterMultiSize->list[i].midSizeBias *= fFinalScale;			
		}
	}

    CNtlInstanceComponentSystem::SetScale(fScale);
}

void CNtlInstanceParticleSystem::SetAlpha( RwUInt8 byValue ) 
{
    if(m_pEmitterStandard)
    {
        m_pEmitterStandard->prtColor.alpha = byValue;
    }
}

static RpPrtStdParticleBatch* NtlStdParticleUpdateCB(RpPrtStdEmitter *emt, RpPrtStdParticleBatch *prtBatch, void * data __RWUNUSED__)
{
    if(!emtPTank || !emtStd)
        return prtBatch;

	RwChar*						pTankPosIn = NULL;
	RwChar*						pTankPosOut = NULL;
	RwInt32                     nTankPosStride = 0;

	RwChar*						prtIn;
	RwChar*						prtOut;

	RwInt32						i;
	RwInt32                     numPrtIn;
	RwInt32                     numPrtOut;
	RwInt32                     prtStride;

    RwV3d*						prtVelIn;

    // emPTank는 인스턴스의 EmitterPTank 포인터이다.
    // Static 메소드 이기때문에 Static 변수에다 할당해서 사용한다.
	if (emtPTank)
	{
		i = 0;

		if (emtPTank->dataFlags & rpPTANKDFLAGPOSITION)
		{
			pTankPosIn = (RwChar *) emtPTank->dataInPtrs[i];
			pTankPosOut = (RwChar *) emtPTank->dataOutPtrs[i];

			if (emtPTank->updateFlags & rpPTANKDFLAGPOSITION)
			{
				nTankPosStride = emtPTank->dataStride[i];
			}
			i++;
		}
	}


	RwV3d emtPos;
	if (emtStd->ltm)
	{
		RwV3dAdd(&emtPos, &emtStd->ltm->pos, &g_vOrbitPoint);
	}
	else
	{
		emtPos = g_vOrbitPoint;
	}

	prtStride = emt->prtClass->objSize;
	prtIn = ((RwChar *)prtBatch) + prtBatch->offset;
	prtOut = prtIn;

	numPrtIn = prtBatch->numPrt;
	numPrtOut = 0;

    while (numPrtIn-- > 0)
    {
        if (((RpPrtStdParticleStandard *)prtIn)->currTime < ((RpPrtStdParticleStandard *)prtIn)->endTime)
        {
            prtVelIn = (RwV3d *) RPPRTSTDPROP(prtIn, g_nVelocityOffset);

            if (emtPTank)
            {
                if (emtPTank->dataFlags & emtPTank->updateFlags & rpPTANKDFLAGPOSITION)
				{
					RwV3d *pInPos = (RwV3d*)pTankPosIn;
					RwV3d *pOutPos = (RwV3d*)pTankPosOut;

					if (g_nVelocityOffset >= 0)
					{
						RwV3d dir = emtPos - *pInPos;                        						

					    RwV3dNormalize(&dir, &dir);

						/* velocity v = u + at */						
                        *prtVelIn = *prtVelIn + (dir * g_fElapsedTime1);
					}

					pTankPosOut += nTankPosStride;
				}
            }

            prtOut += prtStride;
            numPrtOut++;
        }

		prtIn += prtStride;
		pTankPosIn += nTankPosStride;
    }

    // 전역 변수들인데 Update할때마다 새로 갱신되기 때문에
    // 처리가 끝난후에 NULL을 넣어준다.
    emtPTank = NULL;
    emtStd   = NULL;

    return (prtBatch);
}

//static RpPrtStdParticleBatch* NtlStdParticleBillboardCB(RpPrtStdEmitter *emt, RpPrtStdParticleBatch *prtBatch, void * data __RWUNUSED__)
//{
//    
//    int     nOffsetMat = RpPrtStdPropTabGetPropOffset(prtBatch->prtClass->propTab, rpPRTSTDPROPERTYCODEPARTICLEMATRIX);
//
//    RwChar*						prtIn;    
//    
//    RwInt32                     numPrtIn;    
//    RwInt32                     prtStride;
//
//    prtStride = prtBatch->prtClass->objSize;
//    prtIn = ((RwChar *)prtBatch) + prtBatch->offset;    
//
//    numPrtIn = prtBatch->numPrt;    
//
//    RwMatrix matCamera = *RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));
//    RwV3d* pvCameraPos = RwMatrixGetPos(&matCamera);
//
//    while (numPrtIn-- > 0)
//    {
//        if (((RpPrtStdParticleStandard *)prtIn)->currTime < ((RpPrtStdParticleStandard *)prtIn)->endTime)
//        {
//            RwMatrix* pMatPaticle = (RwMatrix*) RPPRTSTDPROP(prtIn, nOffsetMat);
//            RwV3d* pAt = RwMatrixGetAt(pMatPaticle);
//            pAt->x = 1.0f;
//        }
//
//        prtIn += prtStride;        
//    }
//
//    return (prtBatch);
//}