#include "precomp_ntlpresentation.h"
#include "NtlInstanceTrailSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"

void* CNtlInstanceTrailSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL));
}

void CNtlInstanceTrailSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL, pObj);
}

CNtlInstanceTrailSystem::CNtlInstanceTrailSystem(void)
{
    Init();
}

CNtlInstanceTrailSystem::~CNtlInstanceTrailSystem(void)
{
    Delete();

}

void CNtlInstanceTrailSystem::Reset() 
{
    Delete();
}

void CNtlInstanceTrailSystem::Init() 
{
    CNtlInstanceTraceSystem::Init();

    m_pEmitterTrail = NULL;
    m_pAttachComponent = NULL;
    ZeroMemory(&m_vPrevTrailPoint, sizeof(RwV3d));

    m_uiMemoryUseSize = sizeof(CNtlInstanceTrailSystem);
}

void CNtlInstanceTrailSystem::Delete() 
{
    CNtlInstanceTraceSystem::Delete();
}

/**
 * Trail 이펙트를 생성한다.
 * \param pComponentSystem Trail이 붙을 부모 InstanceComponentSystem 객체
 * \param pEmitterTrail Trail 생성 정보를 가지고 있는 프로퍼티 객체
 * return 성공 유무
 */
RwBool CNtlInstanceTrailSystem::Create(CNtlInstanceComponentSystem* pComponentSystem, SNtlPrtStdEmitterTrail* pEmitterTrail)
{
    if(!pEmitterTrail || !pComponentSystem)
        return FALSE;

    m_pEmitterTrail = pEmitterTrail;
    m_pAttachComponent = pComponentSystem;

    // Create an event object and populate it. (Because you can use the methods of the superclass as it is)
    m_pEventTrace = NTL_NEW SEventTrace();    
    m_pEventTrace->fLifeTime     = 999999;                      ///< Trail has no LifeTime.
    m_pEventTrace->fEdgeLifeTime = pEmitterTrail->fEdgeLifeTime;
    m_pEventTrace->fMaxLength    = pEmitterTrail->fMaxLength;
    m_pEventTrace->fEdgeGap      = pEmitterTrail->fEdgeGap;
    m_pEventTrace->nSplinePointCount = pEmitterTrail->nSplinePointCount;
    m_pEventTrace->eSrcBlend     = pEmitterTrail->eSrcBlend;
    m_pEventTrace->eDestBlend    = pEmitterTrail->eDestBlend;
    m_pEventTrace->colStartColor = pEmitterTrail->colStartColor;
    m_pEventTrace->nMaxEdgeCount = pEmitterTrail->nMaxEdgeCount;
    m_pEventTrace->colEndColor   = pEmitterTrail->colEndColor;  
    m_uiMemoryUseSize += sizeof(SEventTrace);

    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
		m_pEventTrace->fEdgeGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    // Vertex Buffer produce
    m_pVertices = NTL_NEW RwIm3DVertex[pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2];
    if(!m_pVertices)
        return FALSE;
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2;

    // Creates a pool.
    m_poolTraceEdge = NTL_NEW STraceEdge[pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount];
    if(!m_poolTraceEdge)
        return FALSE;
    ZeroMemory(m_poolTraceEdge, sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount;

    m_poolControlPoint = NTL_NEW STraceEdge[pEmitterTrail->nMaxEdgeCount];
    if(!m_poolControlPoint)
        return FALSE;
    ZeroMemory(m_poolControlPoint, sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount;
    
    // Texture settings
    std::string strTexture = pEmitterTrail->strTexture;
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(strTexture);

    m_vPrevTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);

    return TRUE;
}

/**
 * Add a new Edge to the end of the list.
 */
void CNtlInstanceTrailSystem::CreateEdge() 
{
    // Calculate if the maximum number of edges is exceeded.
    if(m_nControlPointIndex >= m_pEventTrace->nMaxEdgeCount)
    {
        // When the Index reaches the end of the Pool, it returns to the beginning.
        m_nControlPointIndex = 0;        

        m_listControlPoint.erase(m_listControlPoint.begin());
    }
    // Calculate if the maximum length has been exceeded.
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

    // Set the position of the two vertices.
    SetEdgePoint();
    
    // Add a new control point to the List.
    m_poolControlPoint[m_nControlPointIndex].vVertices[0].objVertex = m_vStartPoint;
    m_poolControlPoint[m_nControlPointIndex].vVertices[1].objVertex = m_vEndPoint;

    m_listControlPoint.push_back(&m_poolControlPoint[m_nControlPointIndex]);
    ++m_nControlPointIndex;

    // Create a Spline Curve Point and add it to the list.
    CreateSplinePath();  

}

void CNtlInstanceTrailSystem::SetEdgePoint() 
{
    //RwV3d vTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);
    //RwV3d vDir = vTrailPoint - m_vPrevTrailPoint;
    //RwV3dNormalize(&vDir, &vDir);

    //RwV3d vCameraPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
    //
    //RwV3d vAtCamera = vCameraPos - m_vPrevTrailPoint;
    //RwV3dNormalize(&vAtCamera, &vAtCamera);

    //RwV3d vRight;
    //RwV3dCrossProduct(&vRight, &vAtCamera, &vDir);
    //RwV3dNormalize(&vRight, &vRight);

    //m_vStartPoint = m_vPrevTrailPoint + vRight * m_pEmitterTrail->fWidth * 0.5f;
    //m_vEndPoint   = m_vPrevTrailPoint - vRight * m_pEmitterTrail->fWidth * 0.5f;

    //m_vPrevTrailPoint = vTrailPoint;

    RwV3d vTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);
    RwV3d vDir = vTrailPoint - m_vPrevTrailPoint;
    RwV3dNormalize(&vDir, &vDir);

    RwV3d vCameraPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
    RwV3d vAtCamera = vCameraPos - vTrailPoint;
    RwV3dNormalize(&vAtCamera, &vAtCamera);

    RwV3d vRight;
    RwV3dCrossProduct(&vRight, &vAtCamera, &vDir);
    RwV3dNormalize(&vRight, &vRight);

    m_vStartPoint = vTrailPoint + vRight * m_pEmitterTrail->fWidth * 0.5f;
    m_vEndPoint   = vTrailPoint - vRight * m_pEmitterTrail->fWidth * 0.5f;

    //Applied when Offset is set.
    if(RwV3dLength(&m_pEmitterTrail->vOffset) > 0.0f)
    {
        // In Matrix, we only take the rotation value and apply it to Offset and add it to Point.
        RwMatrix matComponent = m_pAttachComponent->m_matRender;
        *RwMatrixGetPos(&matComponent) = ZeroAxis;

        RwV3d vOffset;
        RwV3dTransformPoint(&vOffset, &m_pEmitterTrail->vOffset, &matComponent);

        m_vStartPoint += vOffset;
        m_vEndPoint += vOffset;
    }

    m_vPrevTrailPoint = vTrailPoint;
}