#include "precomp_ntlpresentation.h"
#include "NtlInstanceLineSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"
#include "NtlInstanceEffect.h"

CNtlInstanceLineSystem::CNtlInstanceLineSystem(void)
{
    Init();
}

CNtlInstanceLineSystem::~CNtlInstanceLineSystem(void)
{
    Delete();
}

void* CNtlInstanceLineSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE));
}

void CNtlInstanceLineSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE, pObj);    
}

void CNtlInstanceLineSystem::Init()
{
    CNtlInstanceImVertexSystem::Init();

    m_pResourceLineSystem = NULL;
    m_poolLineSystemVertex = NULL;
    m_nPoolIndex = 0;
    m_fCreateGapTime = 0.0f;
    m_nRoateSeed = 0;

    m_uiMemoryUseSize += sizeof(CNtlInstanceLineSystem);
}

void CNtlInstanceLineSystem::Delete()
{
    NTL_ARRAY_DELETE(m_poolLineSystemVertex);
	CNtlInstanceImVertexSystem::Delete();
}

void CNtlInstanceLineSystem::Reset()
{
    Delete();
}

RwBool CNtlInstanceLineSystem::Create( CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, const RwMatrix& matWorld )
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pResourceEffect || !pResourceComponentSystem)
        NTL_RETURN(FALSE);

    CNtlInstanceImVertexSystem::Create(pResourceEffect, pResourceComponentSystem, matWorld);
    m_pResourceLineSystem = (CNtlResourceLineSystem*)pResourceComponentSystem;
 
    // Vertex Buffer 생성 (최대 라인의 개수 * 6)
    m_nVertexCount = m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount * 6;    
    m_pVertices = NTL_NEW RwIm3DVertex[m_nVertexCount];
    NTL_ASSERTE(m_pVertices);
    if(!m_pVertices) NTL_RETURN(FALSE);
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * m_nVertexCount);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * m_nVertexCount;

    // Line Pool 생성
    m_poolLineSystemVertex = NTL_NEW SLineSystemVertex[m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount];
    NTL_ASSERTE(m_poolLineSystemVertex);
    if(!m_poolLineSystemVertex) NTL_RETURN(FALSE);
    ZeroMemory(m_poolLineSystemVertex, sizeof(SLineSystemVertex) * m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount);
    m_uiMemoryUseSize += sizeof(SLineSystemVertex) * m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount;
    

    // 텍스쳐 설정
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(m_pResourceLineSystem->m_strTextureName);

    // 기본 Emitter 설정
    BuildEmitterStandard();

    m_nRoateSeed = rand();

    NTL_RETURN(TRUE);
}

void CNtlInstanceLineSystem::BuildEmitterStandard()
{
    // Color 설정
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
    {
        RwRGBA color;

        RwRGBAFromRwRGBAReal(&color, &m_pResourceLineSystem->m_EmitterPrtCol.prtStartCol);        

        SetVertexColor(color);
    }
}

RwBool CNtlInstanceLineSystem::Update( RwReal fElapsedTime )
{
    CNtlInstanceImVertexSystem::Update(fElapsedTime);

    if(!m_pResourceLineSystem)
        return FALSE;

    m_fLifeTime += fElapsedTime;
    if(m_bStart)
    {
        if(m_pResourceLineSystem->m_fStartTime < m_fLifeTime)
        {
            m_bStart = FALSE;
            m_bReady = TRUE;
            m_bUpdate = TRUE;

            m_fLifeTime = fElapsedTime;
        }
        else
        {
            return TRUE;
        }
    }

    if(m_bUpdate && m_pResourceLineSystem->m_bEternity == FALSE && m_fLifeTime >= m_pResourceLineSystem->m_fLifeTime)
    {
        Stop();                
    }

    if(m_bReady)
    {
        UpdateVertices(fElapsedTime);

        RwReal fDeltaTime = m_fLifeTime / m_pResourceLineSystem->m_fLifeTime;
        
        // Color Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
        {
            UpdateColor(fDeltaTime);
        }

        // Size Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
        {
            UpdateSize(fDeltaTime);
        }

        // Multi Texture Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
        {
            UpdateMultiTexture(fElapsedTime);
        }

        // Rotate Action
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
        {
            UpdateRotate(fDeltaTime);
        }
    }

    if((m_bFinish || !m_bUpdate) && m_listLineSystemVertex.size() == 0)
    {
        m_bReady = FALSE;
    }

    return m_bReady;
}

RwBool CNtlInstanceLineSystem::Render()
{
    if(!m_bReady || !m_bVisible || !m_pResourceLineSystem )
        return TRUE;

	if(m_listLineSystemVertex.empty())
		return TRUE;
    
    // Vertex 들을 렌더링 한다.
    if(m_pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable == FALSE)
    {
        RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void*)FALSE);
    }
    
    BeginRenderState(m_pResourceLineSystem);
    
    if(RwIm3DTransform(m_pVertices, (RwInt32)m_listLineSystemVertex.size() * 6,  &m_matRender, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
        RwIm3DEnd();
    }
    
    EndRenderState(m_pResourceLineSystem);

    if(m_pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable == FALSE)
    {
        RwRenderStateSet( rwRENDERSTATEZTESTENABLE, (void*)TRUE);
    }


    NTL_RETURN(TRUE);
}

void CNtlInstanceLineSystem::UpdateVertices( RwReal fElapsedTime )
{
    NTL_FUNCTION(__FUNCTION__);

    if(!m_pResourceLineSystem)
        NTL_RETURNVOID();

    m_fCreateGapTime += fElapsedTime;

    // Gap Time이 지났으면 새로운 라인을 생성한다.
    RwReal fEmitterGap = m_pResourceLineSystem->m_EmitterStandard.m_fEmitterGap;
    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        fEmitterGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    if(m_fCreateGapTime >= fEmitterGap && m_bUpdate && !m_bFinish)
    {
        for(int i = 0; i < m_pResourceLineSystem->m_EmitterStandard.m_nEmitterCount; ++i)
        {
            CreateLine();
        }
        
        m_fCreateGapTime = 0.0f;
    }

    int nCount = 0;
    ListLineSystemVertex::iterator it = m_listLineSystemVertex.begin();
    while(it != m_listLineSystemVertex.end())
    {
        SLineSystemVertex* pLineSystemVertex = (SLineSystemVertex*)(*it);
        if(pLineSystemVertex)
        {
            pLineSystemVertex->fLifeTime += fElapsedTime;

            if(pLineSystemVertex->fLifeTime < m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime)
            {
                // 끝점을 설정된 방향으로 이동 시킨다.
                pLineSystemVertex->vUpdatePoint += pLineSystemVertex->vDir * m_pResourceLineSystem->m_EmitterStandard.m_fVelocity * fElapsedTime;

                // 라인이 길이가 정해진 길이보다 길어지면 StartPoint도 이동한다.
                if(m_pResourceLineSystem->m_EmitterStandard.m_bMoveLine)
                {
                    RwV3d vLenth = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;

                    if(RwV3dLength(&vLenth) >= pLineSystemVertex->vLineSize.y)
                    {
                        pLineSystemVertex->vStrartPoint += pLineSystemVertex->vDir * m_pResourceLineSystem->m_EmitterStandard.m_fVelocity * fElapsedTime;
                    }
                }

                // Shake 옵션이 켜져있으면 랜덤으로 흔든다
                if(m_pResourceLineSystem->m_EmitterStandard.m_bShake)
                {
                    UpdateShake(pLineSystemVertex);
                }

                // 정해진 두점을 가지고 Z축 빌보드 면을 생성한다.
                if(m_pResourceLineSystem->m_EmitterStandard.m_bZBiilBoard)
                {
                    UpdateZBillBoard(pLineSystemVertex);
                }
                else
                {
                    UpdateNoneZBillBoard(pLineSystemVertex);
                }
                

                // List에 있는 Vertex들을 Vertex Buffer에 넣는다.
                for(int i = 0; i < 6; ++i)
                {
                    m_pVertices[nCount++] = pLineSystemVertex->imVertices[i];                    
                }

                ++it;
            }
            else    // Life Time이 지나면 리스트에서 제거한다.
            {
                it = m_listLineSystemVertex.erase(it);

            }            
        }
    }
}

/**
 * 새로운 라인을 추가하여 List에 추가한다. 
 */
void CNtlInstanceLineSystem::CreateLine()
{
    if(m_listLineSystemVertex.size() >= (RwUInt32)m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount)
    {
        return;
    }

    if(m_nPoolIndex >= (RwInt32)m_pResourceLineSystem->m_EmitterStandard.m_nMaxCount)
    {
        m_nPoolIndex = 0;
    }

    // Emitter내의 위치를 결정한다.
    RwReal fRadian = NtlRandomNumber(0.0f, (2.0f * rwPI));        
    
    RwV3d vStartPoint;
    RwV3d vDir;
    
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
    {
        vStartPoint.x = (RwReal)RwCos(fRadian);
        vStartPoint.y = (RwReal)RwSin(fRadian);  
        vStartPoint.z = 0.0f;

        RwMatrix matRot;
        RwMatrixSetIdentity(&matRot);
        RwMatrixRotate(&matRot, &VEC_Y, NtlRandomNumber(0.0f, 360.0f), rwCOMBINEPOSTCONCAT);
        RwV3dTransformVector(&vStartPoint, &vStartPoint, &matRot);

        RwV3dNormalize(&vStartPoint, &vStartPoint);
        vDir = vStartPoint;        

        vStartPoint.x = vStartPoint.x * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.x;
        vStartPoint.y = vStartPoint.y * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.y;
        vStartPoint.z = vStartPoint.z * m_pResourceLineSystem->m_EmitterSphere.m_vEmitterSize.z;
        
    }
    else
    {
        vStartPoint.x = (RwReal)RwCos(fRadian);
        vStartPoint.y = (RwReal)RwSin(fRadian);    
        vStartPoint.z = 0.0f;    

        // Line의 진행 방향을 결정한다.
        vDir = vStartPoint * m_pResourceLineSystem->m_EmitterStandard.m_fTargetRadius;    
        vDir.z = -1.0f;
        RwV3dNormalize(&vDir, &vDir);
    }

    m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint = vStartPoint * m_pResourceLineSystem->m_EmitterStandard.m_fEmitterRaduis;
    m_poolLineSystemVertex[m_nPoolIndex].vUpdatePoint = m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint;

    m_poolLineSystemVertex[m_nPoolIndex].vDir = vDir;    

    // Emitter Bias를 설정한다.
    if(m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias != 0.0f)
    {
        RwReal fEmitterBias = NtlRandomNumber(-(m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias), m_pResourceLineSystem->m_EmitterStandard.m_fEmitterBias);
        vDir = vDir * fEmitterBias;
        m_poolLineSystemVertex[m_nPoolIndex].vStrartPoint += vDir;
    }

    // UV 설정
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[0].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[0].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[1].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[1].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[2].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[2].v = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[3].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[3].v = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[4].u = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[4].v = 1.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[5].u = 0.0f;
    m_poolLineSystemVertex[m_nPoolIndex].imVertices[5].v = 1.0f;

    // Color 설정
    for(int i = 0; i < 6; ++i)
    {
        RwIm3DVertexSetRGBA(&m_poolLineSystemVertex[m_nPoolIndex].imVertices[i], m_sColor.red, m_sColor.green, m_sColor.blue, m_sColor.alpha);
    }

    // Size 설정    
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
    {
        // Size Action이 설정되면 기본 Size는 무시된다. (Size Action의 z값은 사용하지 않는다.)
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize.x = m_pResourceLineSystem->m_EmitterPrtSize.prtStartSize.x;        
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize.y = m_pResourceLineSystem->m_EmitterPrtSize.prtStartSize.y;
    }
    else
    {
        m_poolLineSystemVertex[m_nPoolIndex].vLineSize = m_pResourceLineSystem->m_EmitterStandard.m_sizeLine;        
    }

    m_poolLineSystemVertex[m_nPoolIndex].fLifeTime = 0.0f;

    m_listLineSystemVertex.push_back(&m_poolLineSystemVertex[m_nPoolIndex]);

    ++m_nPoolIndex;
}

void CNtlInstanceLineSystem::UpdateZBillBoard( SLineSystemVertex* pLineSystemVertex )
{
    if(!pLineSystemVertex)
        return;

    const RwV3d* pViewPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

    RwV3d vAxisZ = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;
    if(RwV3dLength(&vAxisZ) == 0.0f)
        return;

    RwV3dNormalize(&vAxisZ, &vAxisZ);

    RwV3d vAxisStartX = *pViewPos - pLineSystemVertex->vStrartPoint;
    RwV3dNormalize(&vAxisStartX, &vAxisStartX);

    RwV3d vAxisEndX = *pViewPos - pLineSystemVertex->vUpdatePoint;
    RwV3dNormalize(&vAxisEndX, &vAxisEndX);

    RwV3d vAxisStartY;
    RwV3dCrossProduct(&vAxisStartY, &vAxisStartX, &vAxisZ);
    RwV3dNormalize(&vAxisStartY, &vAxisStartY);    

    RwV3d vAxisEndY;
    RwV3dCrossProduct(&vAxisEndY, &vAxisEndX, &vAxisZ);
    RwV3dNormalize(&vAxisEndY, &vAxisEndY);
    
    CreatePlane(pLineSystemVertex, vAxisStartY, vAxisEndY);
}

void CNtlInstanceLineSystem::UpdateNoneZBillBoard( SLineSystemVertex* pLineSystemVertex ) 
{
    // ZBillBoard가 적용되지 않은 면을 생성한다.
    // 라인의 Dir 벡터와 라인위치와 원점과의 벡터를 외적한 벡터 방향으로 면을 늘린다.

    if(!pLineSystemVertex)
        return;

    // 진행 벡터
    RwV3d vDir = pLineSystemVertex->vUpdatePoint - pLineSystemVertex->vStrartPoint;
    if(RwV3dLength(&vDir) == 0.0f)
        return;

    RwV3dNormalize(&vDir, &vDir);

    // 원점과의 벡터
    RwV3d vLinePos = pLineSystemVertex->vUpdatePoint;
    vLinePos.z = m_pResourceLineSystem->m_vPosition.z;

    RwV3d vOrigin = vLinePos - m_pResourceLineSystem->m_vPosition;
    RwV3dNormalize(&vOrigin, &vOrigin);

    // 두 벡터를 외적하여 면의 방향을 결정한다.
    RwV3d vAxisWidth;
    RwV3dCrossProduct(&vAxisWidth, &vDir, &vOrigin);
    RwV3dNormalize(&vAxisWidth, &vAxisWidth);

    CreatePlane(pLineSystemVertex, vAxisWidth, vAxisWidth);
}


/**
 * 라인의 정보로 평면을 생성한다.
 * \param pLineSystemVertex 정보를 가지고 있는 구조체의 포인터
 * \param vStartAxis 시작점에서 구성될 평면의 벡터 (단위벡터)
 * \param vEndAxis 끝점에서 구성될 편면의 벡터 (단위 벡터) 
 */
void CNtlInstanceLineSystem::CreatePlane(SLineSystemVertex* pLineSystemVertex, const RwV3d& vStartAxis, const RwV3d& vEndAxis)
{
    if(!pLineSystemVertex)
        return;
    
    RwV3d vStartWidth = vStartAxis * pLineSystemVertex->vLineSize.x * 0.5f;
    RwV3d vEndWidth = vEndAxis * pLineSystemVertex->vLineSize.x * 0.5f;

    pLineSystemVertex->imVertices[0].objVertex = pLineSystemVertex->vStrartPoint + vStartWidth;
    pLineSystemVertex->imVertices[1].objVertex = pLineSystemVertex->vUpdatePoint + vEndWidth;
    pLineSystemVertex->imVertices[2].objVertex = pLineSystemVertex->vUpdatePoint - vEndWidth;

    pLineSystemVertex->imVertices[3].objVertex = pLineSystemVertex->imVertices[0].objVertex;
    pLineSystemVertex->imVertices[4].objVertex = pLineSystemVertex->imVertices[2].objVertex;
    pLineSystemVertex->imVertices[5].objVertex = pLineSystemVertex->vStrartPoint - vStartWidth;
}

void CNtlInstanceLineSystem::UpdateShake( SLineSystemVertex* pLineSystemVertex )
{
    if(!pLineSystemVertex)
        return;

    RwReal fShakeRatio = NtlRandomNumber(0.0f, m_pResourceLineSystem->m_EmitterStandard.m_fShakeBias);

    RwV3d fShakeDir;
    fShakeDir.x = NtlRandomNumber(-1.0f, 1.0f);
    fShakeDir.y = NtlRandomNumber(-1.0f, 1.0f);
    fShakeDir.z = NtlRandomNumber(-1.0f, 1.0f);
    RwV3dNormalize(&fShakeDir, &fShakeDir);

    fShakeDir = fShakeDir * fShakeRatio;

    // 끝라인만 Shake를 적용한다.
    pLineSystemVertex->vUpdatePoint = pLineSystemVertex->vUpdatePoint + fShakeDir;
}

void CNtlInstanceLineSystem::UpdateRotate( RwReal fDeltaTime ) 
{
    RwV3d vRotate;
    if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
    {
        RebuildMultiRotate(vRotate, m_nRoateSeed, fDeltaTime, &m_pResourceLineSystem->m_EmitterPrtRotate, &m_pResourceLineSystem->m_EmitterPrtMultiRotate);
    }
    else
    {
        RebuildRotate(vRotate, m_nRoateSeed, fDeltaTime, &m_pResourceLineSystem->m_EmitterPrtRotate);
    }

    RwMatrixSetIdentity(&m_matRotate);
    RwMatrixRotate(&m_matRotate, &VEC_X, vRotate.x, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate(&m_matRotate, &VEC_Y, vRotate.y, rwCOMBINEPOSTCONCAT);
    RwMatrixRotate(&m_matRotate, &VEC_Z, vRotate.z, rwCOMBINEPOSTCONCAT);

    SetWorldMatrix(m_matWorld);
}

void CNtlInstanceLineSystem::UpdateColor( RwReal fDeltaTime ) 
{
    // 각각의 라인마다 Color를 Update한다.
    RwRGBA color;

    for each(SLineSystemVertex* pLineSystemVertex in m_listLineSystemVertex)
    {
        RwReal fLineDeltaTime = pLineSystemVertex->fLifeTime / m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime;
        
        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
        {
            RebuildMultiColor(color, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtCol, &m_pResourceLineSystem->m_EmitterPrtMultiCol);
        }
        else
        {
            RebuildColor(color, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtCol);            
        }

        for(int i = 0; i < 6; ++i)
        {
            RwIm3DVertexSetRGBA(&pLineSystemVertex->imVertices[i], color.red, color.green, color.blue, color.alpha);
        }
    }
}
void CNtlInstanceLineSystem::UpdateSize( RwReal fDeltaTime ) 
{
    // 각각의 라인마다 Size를 Update한다.
    RwV3d size;

    for each(SLineSystemVertex* pLineSystemVertex in m_listLineSystemVertex)
    {
        RwReal fLineDeltaTime = pLineSystemVertex->fLifeTime / m_pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime;

        if(m_pResourceLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
        {
            RebuildMultiSize(size, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtSize, &m_pResourceLineSystem->m_EmitterPrtMultiSize);
        }
        else
        {
            RebuildSize(size, m_nRandomSeed, fLineDeltaTime, &m_pResourceLineSystem->m_EmitterPrtSize);
        }

        pLineSystemVertex->vLineSize.x = size.x;
        pLineSystemVertex->vLineSize.y = size.y;
    }
}
