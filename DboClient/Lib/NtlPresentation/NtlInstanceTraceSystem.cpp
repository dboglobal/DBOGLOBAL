#include "precomp_ntlpresentation.h"
#include "NtlInstanceTraceSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"
#include "D3Dx9math.h"
#include "NtlPLCharacter.h"
#include "NtlInstanceEffect.h"

void* CNtlInstanceTraceSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);
    

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE));
}

void CNtlInstanceTraceSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRACE, pObj);
}

CNtlInstanceTraceSystem::CNtlInstanceTraceSystem(void)
{
    Init();
}

CNtlInstanceTraceSystem::~CNtlInstanceTraceSystem(void)
{
    Delete();
}

void CNtlInstanceTraceSystem::Init()
{
    CNtlInstanceImVertexSystem::Init();

    m_pEventTrace           = NULL;    
    m_pAttach               = NULL;    
    m_nPoolIndex            = 0;
    m_nControlPointIndex    = 0;
    m_poolTraceEdge         = NULL;    
    m_poolControlPoint      = NULL;
    m_fEdgeGapTemp          = 10.0f;                ///< 첫 프레임때 무조건 생성하기 위해 큰값을 설정한다.    
    m_bUpdate               = TRUE;
    m_uiMemoryUseSize       = sizeof(CNtlInstanceTraceSystem);
}

void CNtlInstanceTraceSystem::Reset()
{
    Delete();
}

void CNtlInstanceTraceSystem::Delete()
{
    NTL_ARRAY_DELETE(m_poolTraceEdge);
    NTL_ARRAY_DELETE(m_poolControlPoint);    
    NTL_DELETE(m_pEventTrace);

    CNtlInstanceImVertexSystem::Delete();

}

/**
 * 궤적 이펙트를 생성한다.
 * \param pEventTrace 궤적 이펙트에 대한 정보를 가지고 있는 Event 객체
 * \param pCharacter 궤적 이펙트를 붙일 캐릭터의 포인터
 * return 성공 유무
 */
RwBool CNtlInstanceTraceSystem::Create(SEventTrace* pEventTrace, CNtlPLAttach* pAttach)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!pEventTrace || !pAttach)
        NTL_RETURN(FALSE);

    m_pEventTrace = NTL_NEW SEventTrace();
    memcpy(m_pEventTrace, pEventTrace, sizeof(SEventTrace));
    m_uiMemoryUseSize += sizeof(SEventTrace);
    
    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        m_pEventTrace->fEdgeGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    // Vertex Buffer 생성    
    m_pVertices = NTL_NEW RwIm3DVertex[pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount];    
    if(!m_pVertices) NTL_RETURN(FALSE);
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount));
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount);
    
    // Pool을 생성한다.
    m_poolTraceEdge = NTL_NEW STraceEdge[pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount];
    ZeroMemory(m_poolTraceEdge, sizeof(STraceEdge) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount));
    m_uiMemoryUseSize += sizeof(STraceEdge) * (pEventTrace->nMaxEdgeCount * pEventTrace->nSplinePointCount);
    
    m_poolControlPoint = NTL_NEW STraceEdge[pEventTrace->nMaxEdgeCount];
    ZeroMemory(m_poolControlPoint, sizeof(STraceEdge) * pEventTrace->nMaxEdgeCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEventTrace->nMaxEdgeCount;

    // 텍스쳐 설정
    std::string strTextureName = pEventTrace->strTexture;
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(strTextureName);

    // Bone의 위치 포인터 설정
    SetEdgePoint(pAttach);

    m_pAttach = pAttach;    

    NTL_RETURN(TRUE);
}

/**
 * 두개의 Bone으로부터 위치를 가져와서 설정한다.
 * \param pAttach Bone의 가져올 Entity 객체 (캐릭터 Or Item)
 * return 
 */
void CNtlInstanceTraceSystem::SetEdgePoint(CNtlPLAttach* pAttach)
{
    if(!pAttach)
        return;

    if(strlen(m_pEventTrace->strStartBoneName) == 0 ||
        strlen(m_pEventTrace->strEndBoneName) == 0)
        return;

    
    RwMatrix *pMatStartBone, *pMatEndBone;

    if(pAttach->GetClassType() == PLENTITY_CHARACTER)
    {
        CNtlPLCharacter* pCharacter = (CNtlPLCharacter*)pAttach;

        pMatStartBone = pCharacter->GetBoneMatrix(m_pEventTrace->strStartBoneName);
        pMatEndBone = pCharacter->GetBoneMatrix(m_pEventTrace->strEndBoneName);
    }
    else if(pAttach->GetClassType() == PLENTITY_ITEM)
    {
        CNtlPLItem* pItem = (CNtlPLItem*)pAttach;

        pMatStartBone = pItem->GetBoneMatrix(m_pEventTrace->strStartBoneName);
        pMatEndBone = pItem->GetBoneMatrix(m_pEventTrace->strEndBoneName);
    }

    if(!pMatStartBone || !pMatEndBone)
        return;

    m_vStartPoint = *RwMatrixGetPos(pMatStartBone);
    m_vEndPoint = *RwMatrixGetPos(pMatEndBone);    

    // Offset이 설정되어있으면 적용한다.
    if(RwV3dLength(&(m_pEventTrace->v3dStartBoneOffset)) > 0.0f ||
       RwV3dLength(&(m_pEventTrace->v3dEndBoneOffset)) > 0.0f)
    {
        // Matrix에서 회전값만 가져와서 Offset에 적용한후 Point에 더한다.
        RwMatrix matStartOffset = *pMatStartBone;
        RwMatrix matEndOffset = *pMatEndBone;   
        matStartOffset.pos.x = matStartOffset.pos.y = matStartOffset.pos.z = 0.0f;
        matEndOffset.pos.x = matEndOffset.pos.y = matEndOffset.pos.z = 0.0f;

        RwV3d v3dStartOffset, v3dEndOffset;
        RwV3dTransformPoint(&v3dStartOffset, &(m_pEventTrace->v3dStartBoneOffset), &matStartOffset);
        RwV3dTransformPoint(&v3dEndOffset, &(m_pEventTrace->v3dEndBoneOffset), &matEndOffset);

        m_vStartPoint = m_vStartPoint + v3dStartOffset;
        m_vEndPoint   = m_vEndPoint + v3dEndOffset;
    }
}

RwBool CNtlInstanceTraceSystem::Update(RwReal fElapsedTime)
{
    NTL_FUNCTION(__FUNCTION__);

    if(!m_pEventTrace)
        NTL_RETURN(FALSE);

    m_fLifeTime += fElapsedTime;    

    if(m_fLifeTime >= m_pEventTrace->fLifeTime)
    {
        Stop();
    }

    if(!UpdateVertices(fElapsedTime))
    {
        NTL_RETURN(FALSE);
    }
    
    NTL_RETURN(TRUE);    
}

RwBool CNtlInstanceTraceSystem::Render()
{
    if(!m_pEventTrace || !m_bVisible || m_listTraceEdge.empty())
        return TRUE;

    // 리스트안의 Vertex들을 렌더링 한다.
    RwD3D9SetTexture(m_pCurrentTexture, 0);

    BeginEffectTraceSystem(TRUE, m_pEventTrace->eSrcBlend, m_pEventTrace->eDestBlend);    

    if(RwIm3DTransform(m_pVertices, m_listTraceEdge.size() * 2, NULL, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }

    EndEffectTraceSystem(TRUE);

    return TRUE;
}

RwBool CNtlInstanceTraceSystem::UpdateVertices(RwReal fElapsedTime)
{
    // 새로운 Edge를 추가한다.
    if(m_fEdgeGapTemp >= m_pEventTrace->fEdgeGap &&
       m_bUpdate)
    {
        CreateEdge();
        m_fEdgeGapTemp = 0.0f;
    }
    else
    {
        m_fEdgeGapTemp += fElapsedTime;
    }

    // UV를 udpate한다.
    UpdateUV();

    // Color를 Udpate한다.
    UpdateColor();
    
    // 리스트에 있는 버텍스들의 LifeTime을 Update한다.    
    int nCount = 0;
    ListTraceEdge::iterator it = m_listTraceEdge.begin();
    while(it != m_listTraceEdge.end())
    {
        (*it)->fLifeTime += fElapsedTime;

        if((*it)->fLifeTime >= m_pEventTrace->fEdgeLifeTime)
        {
            // Life 타임이 종료되면 리스트에서 제거한다.
            it = m_listTraceEdge.erase(it);
        }
        else
        {
            m_pVertices[nCount++] = (*it)->vVertices[0];
            m_pVertices[nCount++] = (*it)->vVertices[1];            

            ++it;
        }
    }

    if(!m_bUpdate && m_listTraceEdge.size() == 0)
    {
        return FALSE;
    }    

    return TRUE;
}

void CNtlInstanceTraceSystem::CreateEdge() 
{
    // 새로운 Edge를 리스트의 맨끝에 추가한다.    

    // 최대 Edge 개수를 초과했는지 계산한다.
    if(m_nControlPointIndex >= m_pEventTrace->nMaxEdgeCount)
    {
        // Index가 Pool의 끝에 달하면 처음으로 되돌린다.
        m_nControlPointIndex = 0;        
                
        m_listControlPoint.erase(m_listControlPoint.begin());
    }

    // 최대 길이를 초과 했는지 계산한다.
    if(m_listTraceEdge.size() > 2)
    {
        RwReal fLength = 0.0f;               

        ListTraceEdge::iterator it = m_listTraceEdge.begin();
        RwV3d  v3dPrevPos = (*it)->vVertices[0].objVertex;
        ++it;

        while(it != m_listTraceEdge.end())
        {            
            RwV3d vLength = (*it)->vVertices[0].objVertex - v3dPrevPos;
            fLength += RwV3dLength(&vLength);

            v3dPrevPos = (*it)->vVertices[0].objVertex;
            ++it;
        }

        if(fLength >= m_pEventTrace->fMaxLength)
        {
            m_listTraceEdge.erase(m_listTraceEdge.begin());            
        }
    }

    // 두개의 Vertex의 위치를 설정한다.
    SetEdgePoint(m_pAttach);

    // List에 새로운 컨트롤 포인트를 추가한다.
    m_poolControlPoint[m_nControlPointIndex].vVertices[0].objVertex = m_vStartPoint;
    m_poolControlPoint[m_nControlPointIndex].vVertices[1].objVertex = m_vEndPoint;
    
    m_listControlPoint.push_back(&m_poolControlPoint[m_nControlPointIndex]);
    ++m_nControlPointIndex;

    // Spline Curve Point를 생성하여 리스트에 추가한다.
    CreateSplinePath();   
}

/**
 * Spline Path Point를 생성하여 버텍스 버퍼에 추가한다.
 * \param nIndex Spline을 생성한 리스트의 인덱스 (nIndex, nIndex-1, nIndex-2, nIndex-3의 4개의 버텍스로 Spline Path를 생성한다.
 * return 
 */
void CNtlInstanceTraceSystem::CreateSplinePath()
{
    // Catmull-rom Spline을 생성한다.
    // DX에 있는 함수를 사용한다.

    if(m_listControlPoint.size() < 4 || m_pEventTrace->nSplinePointCount < 2)
    {
        // Spline을 생성하기 위한 조건은 최소한 4개의 버텍스가 있어야만 한다.

        ListTraceEdge::reverse_iterator it = m_listControlPoint.rbegin();
        STraceEdge* pEdge = *it;

        m_poolTraceEdge[m_nPoolIndex].fLifeTime = 0.0f;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex = pEdge->vVertices[0].objVertex;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex = pEdge->vVertices[1].objVertex;

        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[0], m_pEventTrace->colStartColor.red,
                                                                         m_pEventTrace->colStartColor.green,
                                                                         m_pEventTrace->colStartColor.blue,
                                                                         m_pEventTrace->colStartColor.alpha);
        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[1], m_pEventTrace->colEndColor.red,
                                                                         m_pEventTrace->colEndColor.green,
                                                                         m_pEventTrace->colEndColor.blue,
                                                                         m_pEventTrace->colEndColor.alpha);

        ++m_nPoolIndex;

        return;
    }

    D3DXVECTOR3 vStartOut, vEndOut, vStart[4], vEnd[4];
    RwReal fWeight;

    ListTraceEdge::reverse_iterator it = m_listControlPoint.rbegin();
    for(int i = 3; i >= 0 ; --i)
    {
        STraceEdge* pEdge = *it;

        vStart[i].x = pEdge->vVertices[0].objVertex.x;
        vStart[i].y = pEdge->vVertices[0].objVertex.y;
        vStart[i].z = pEdge->vVertices[0].objVertex.z;

        vEnd[i].x = pEdge->vVertices[1].objVertex.x;
        vEnd[i].y = pEdge->vVertices[1].objVertex.y;
        vEnd[i].z = pEdge->vVertices[1].objVertex.z;

        ++it;
    }

    for(int j = 0; j < m_pEventTrace->nSplinePointCount; ++j)
    {
        fWeight = (RwReal)j / (m_pEventTrace->nSplinePointCount - 1) ;

        D3DXVec3CatmullRom(&vStartOut, &vStart[0], &vStart[1], &vStart[2], &vStart[3], fWeight);
        D3DXVec3CatmullRom(&vEndOut, &vEnd[0], &vEnd[1], &vEnd[2], &vEnd[3],fWeight);

        if(m_nPoolIndex >= m_pEventTrace->nMaxEdgeCount)
        {
            m_nPoolIndex = 0;            
        }

        if(m_listTraceEdge.size() >= (RwUInt32)m_pEventTrace->nMaxEdgeCount)
        {
            m_listTraceEdge.erase(m_listTraceEdge.begin());
        }

        m_poolTraceEdge[m_nPoolIndex].fLifeTime = 0.0f;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.x = vStartOut.x;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.y = vStartOut.y;
        m_poolTraceEdge[m_nPoolIndex].vVertices[0].objVertex.z = vStartOut.z;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.x = vEndOut.x;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.y = vEndOut.y;
        m_poolTraceEdge[m_nPoolIndex].vVertices[1].objVertex.z = vEndOut.z;

        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[0], m_pEventTrace->colStartColor.red,
                                                                        m_pEventTrace->colStartColor.green,
                                                                        m_pEventTrace->colStartColor.blue,
                                                                        m_pEventTrace->colStartColor.alpha);
        RwIm3DVertexSetRGBA(&m_poolTraceEdge[m_nPoolIndex].vVertices[1], m_pEventTrace->colEndColor.red,
                                                                        m_pEventTrace->colEndColor.green,
                                                                        m_pEventTrace->colEndColor.blue,
                                                                        m_pEventTrace->colEndColor.alpha);
                                                                        
        m_listTraceEdge.push_back(&m_poolTraceEdge[m_nPoolIndex]);
        ++m_nPoolIndex;        
    }    
}

void CNtlInstanceTraceSystem::UpdateUV() 
{
    RwUInt32 uiCount = 0;
    RwUInt32 uiListSize = m_listTraceEdge.size();
    
    for each(STraceEdge* pTraceEdge in m_listTraceEdge)
    {
        if(!pTraceEdge)
            continue;

        RwReal fUPos = (RwReal)uiCount / (RwReal)uiListSize;

        pTraceEdge->vVertices[0].u = fUPos;
        pTraceEdge->vVertices[0].v = 0.0f;

        pTraceEdge->vVertices[1].u = fUPos;
        pTraceEdge->vVertices[1].v = 1.0f;        

        ++uiCount;
    }
}

void CNtlInstanceTraceSystem::UpdateColor()
{
    if(m_pEventTrace->colStartColor.red == m_pEventTrace->colEndColor.red &&
       m_pEventTrace->colStartColor.green == m_pEventTrace->colEndColor.green &&
       m_pEventTrace->colStartColor.blue == m_pEventTrace->colEndColor.blue &&
       m_pEventTrace->colStartColor.alpha == m_pEventTrace->colEndColor.alpha)
        return;

    RwRGBA color;
    RwReal fDeltaTime = 0.0f;

    for each(STraceEdge* pTraceEdge in m_listTraceEdge)
    {
        if(!pTraceEdge)
            continue;

        fDeltaTime = pTraceEdge->fLifeTime / m_pEventTrace->fEdgeLifeTime;
        if(fDeltaTime > 1.0f)
        {
            fDeltaTime = 1.0f;
        }

        color = CNtlMath::Interpolation(m_pEventTrace->colStartColor, m_pEventTrace->colEndColor, fDeltaTime);        
        RwIm3DVertexSetRGBA(&pTraceEdge->vVertices[0], color.red, color.green, color.blue, color.alpha);
        RwIm3DVertexSetRGBA(&pTraceEdge->vVertices[1], color.red, color.green, color.blue, color.alpha);
    }
}

