#include "precomp_ntlpresentation.h"
#include "NtlInstanceUpgradeEffectSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlResourceComponentSystem.h"
#include "NtlPLVisualManager.h"
#include "NtlPLItem.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"

#define EFFECT_ANIM_SIZE 0.7f
#define EFFECT_ANIM_SPEED 1.5f

void* CNtlInstanceUpgradeEffectSystem::operator new(size_t size)
{
    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_UPGRADE));
}

void CNtlInstanceUpgradeEffectSystem::operator delete(void* pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_UPGRADE, pObj);
}

CNtlInstanceUpgradeEffectSystem::CNtlInstanceUpgradeEffectSystem(void)
{
    Init();
}

CNtlInstanceUpgradeEffectSystem::~CNtlInstanceUpgradeEffectSystem(void)
{
    Delete();
}

void CNtlInstanceUpgradeEffectSystem::Init() 
{
    CNtlInstanceImVertexSystem::Init();

    m_pItem = NULL;    
    m_fEffetWidthRatio = 1.0f - EFFECT_ANIM_SIZE;
    m_bWidthSizeUp = FALSE;
}

void CNtlInstanceUpgradeEffectSystem::Reset() 
{
    Delete();
}

void CNtlInstanceUpgradeEffectSystem::Delete() 
{
    m_pItem = NULL;

    CNtlInstanceImVertexSystem::Delete();
}

/**
 * Grade Effect를 생성한다.
 * \param pItem Grade Effect가 붙을 Item
 * \param eBoneIndex Grade Effect가 붙을 Bone쌍 
 * return 
 */
RwBool CNtlInstanceUpgradeEffectSystem::Create(CNtlPLItem* pItem, EGradeBoneIndex eBoneIndex /* = GRADE_EFFECT_BONE1 */)
{
    if(!pItem)
        return FALSE;

    m_pItem = pItem;   
    m_eGradeBone = eBoneIndex;

    // Vertex Buffer 생성
    m_pVertices = (RwIm3DVertex*)RwMalloc(6 * sizeof(RwIm3DVertex), rwID_NAOBJECT);
    if(!m_pVertices)
        return FALSE;

    for(int i = 0; i < 6; ++i)
    {
        RwIm3DVertexSetRGBA(&m_pVertices[i], 255, 255, 255, 255);
    }

    RwIm3DVertexSetRGBA(&m_pVertices[2], 255, 255, 255, 255);
    RwIm3DVertexSetRGBA(&m_pVertices[3], 255, 255, 255, 255);

    m_pVertices[0].u = 0.0f;
    m_pVertices[0].v = 0.0f;
    m_pVertices[1].u = 0.0f;
    m_pVertices[1].v = 1.0f;
    m_pVertices[2].u = 0.5f;
    m_pVertices[2].v = 0.0f;
    m_pVertices[3].u = 0.5f;
    m_pVertices[3].v = 1.0f;
    m_pVertices[4].u = 1.0f;
    m_pVertices[4].v = 0.0f;
    m_pVertices[5].u = 1.0f;
    m_pVertices[5].v = 1.0f;

    return TRUE;
}


RwBool CNtlInstanceUpgradeEffectSystem::Update( RwReal fElapsedTime ) 
{
    if(!m_pItem)
        return FALSE;

    if(!UpdateVertices(fElapsedTime))
        return FALSE;


    return TRUE;
}

RwBool CNtlInstanceUpgradeEffectSystem::Render() 
{
    if(!m_bVisible)
        return TRUE;

    if(!m_pItem)
        return FALSE;

    RwD3D9SetTexture(m_pCurrentTexture, 0);

    BeginEffectImVertexSystemState(TRUE, rwBLENDSRCALPHA, rwBLENDONE);

    if(RwIm3DTransform(m_pVertices  , 6, &m_matRender, rwIM3D_VERTEXXYZ|rwIM3D_VERTEXRGBA|rwIM3D_VERTEXUV))
    {
        RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);
        RwIm3DEnd();
    }

    EndEffectImVertexSystemState(TRUE);

    return TRUE;
}

void CNtlInstanceUpgradeEffectSystem::SetTexture( RwChar* szTextureName ) 
{
    std::string strTextureName = szTextureName;
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(strTextureName);
}

RwBool CNtlInstanceUpgradeEffectSystem::UpdateVertices( RwReal fElapsedTime ) 
{

    // 사각형을 생성하고 무기의 축에 따라 빌보드를 생성한다.

    RwV3d vStartPos, vEndPos;
    RwMatrix *pMatStart, *pMatEnd;

    if(m_eGradeBone == GRADE_EFFECT_BONE1)
    {
        pMatStart = m_pItem->GetBoneMatrix(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_strStartBoneName.c_str());
        if(pMatStart)
        {
            vStartPos = *RwMatrixGetPos(pMatStart);
        }

        pMatEnd = m_pItem->GetBoneMatrix(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_strEndBoneName.c_str());
        if(pMatEnd)
        {
            vEndPos = *RwMatrixGetPos(pMatEnd);
        }
    }
    else
    {
        pMatStart = m_pItem->GetBoneMatrix(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_strStartBoneName2.c_str());
        if(pMatStart)
        {
            vStartPos = *RwMatrixGetPos(pMatStart);
        }

        pMatEnd = m_pItem->GetBoneMatrix(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_strEndBoneName2.c_str());
        if(pMatEnd)
        {
            vEndPos = *RwMatrixGetPos(pMatEnd);
        }
    }

    // Bone Offset
    if(pMatStart && pMatEnd &&
       (RwV3dLength(&(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_vStartBoneOffset)) > 0 ||
        RwV3dLength(&(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_vEndBoneOffset)) > 0))
    {
        RwMatrix matStartTemp = *pMatStart;
        RwMatrix matEndTemp = *pMatEnd;

        // Matrix에서 회전값만 가져와서 Offset에 적용한후 Point에 더한다.
        matStartTemp.pos.x = matStartTemp.pos.y = matStartTemp.pos.z = 0.0f;
        matEndTemp.pos.x = matEndTemp.pos.y = matEndTemp.pos.z = 0.0f;

        RwV3d v3dStartOffset, v3dEndOffset;
        RwV3dTransformPoint(&v3dStartOffset, &(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_vStartBoneOffset), &matStartTemp);
        RwV3dTransformPoint(&v3dEndOffset, &(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_vEndBoneOffset), &matEndTemp);

        // 두번째 본은 첫번째것하고 Offset에서 x가 반대 값이다. (양쪽 글러브)
        if(m_eGradeBone == GRADE_EFFECT_BONE1)
        {
            vStartPos += v3dStartOffset;
            vEndPos += v3dEndOffset;
        }
        else
        {
            vStartPos.x -= v3dStartOffset.x;
            vStartPos.y += v3dStartOffset.y;
            vStartPos.z += v3dStartOffset.z;
            
            vEndPos.x -= v3dEndOffset.x;
            vEndPos.y += v3dEndOffset.y;
            vEndPos.z += v3dEndOffset.z;
        }
    }

    RwV3d vDir = vEndPos - vStartPos;
    RwV3dNormalize(&vDir, &vDir);

    // Length Offset
    if(m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_fLengthOffset != 0.0f)
    {
        vStartPos -= vDir * m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_fLengthOffset * 0.5f;
        vEndPos += vDir * m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_fLengthOffset * 0.5f;
    }    

    RwV3d vLookAtStart = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) - vStartPos;
    RwV3d vLookAtEnd = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera))) - vEndPos;

    RwV3dNormalize(&vLookAtStart, &vLookAtStart);
    RwV3dNormalize(&vLookAtEnd, &vLookAtEnd);

    RwV3d vStartRight, vEndRight;
    RwV3dCrossProduct(&vStartRight, &vLookAtStart, &vDir);
    RwV3dCrossProduct(&vEndRight, &vLookAtEnd, &vDir);
    RwV3dNormalize(&vStartRight, &vStartRight);
    RwV3dNormalize(&vEndRight, &vEndRight);

    RwReal fSizeGap = 1.0f - EFFECT_ANIM_SIZE;
    if(m_bWidthSizeUp)
    {
        m_fEffetWidthRatio += fSizeGap * fElapsedTime / EFFECT_ANIM_SPEED;

        if(m_fEffetWidthRatio > fSizeGap)
        {
            m_fEffetWidthRatio = fSizeGap;
            m_bWidthSizeUp = !m_bWidthSizeUp;
        }
    }
    else
    {
        m_fEffetWidthRatio -= fSizeGap * fElapsedTime / EFFECT_ANIM_SPEED;

        if(m_fEffetWidthRatio < 0.0f)
        {
            m_fEffetWidthRatio = 0.0f;
            m_bWidthSizeUp = !m_bWidthSizeUp;
        }
    }
    
    vStartRight = vStartRight * m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_fStartWidth * 0.5f * (EFFECT_ANIM_SIZE + m_fEffetWidthRatio);
    vEndRight = vEndRight * m_pItem->GetProperty()->GetUpgradeEffectProperty()->m_fEndWidth * 0.5f * (EFFECT_ANIM_SIZE + m_fEffetWidthRatio);

    m_pVertices[0].objVertex = vStartPos + vStartRight;
    m_pVertices[1].objVertex = vStartPos - vStartRight;
    m_pVertices[4].objVertex = vEndPos + vEndRight;
    m_pVertices[5].objVertex = vEndPos - vEndRight;
    m_pVertices[2].objVertex = m_pVertices[0].objVertex + (m_pVertices[4].objVertex - m_pVertices[0].objVertex) * 0.5f;
    m_pVertices[3].objVertex = m_pVertices[1].objVertex + (m_pVertices[5].objVertex - m_pVertices[1].objVertex) * 0.5f;

    //for(int i = 0; i < 6; ++i)
    //{
    //    m_pVertices[i].u += fElapsedTime * 5.0f;
    //}
    return TRUE;
}