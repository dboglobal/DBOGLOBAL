#include "StdAfx.h"
#include "NtlDebug.h"
#include "NtlText.h"
#include "NtlAtomic.h"
#include "ModelToolApplication.h"
#include "MTCharacterInfo.h"

SToonData   CMTClump::m_ToonData;                     ///< Toon에 사용될 객체
int         CMTClump::m_nTotalVertices   = 0;
int         CMTClump::m_nTotalTriangels  = 0;
RpAtomic*   CMTClump::m_pTempAtomic      = NULL;

CMTClump::CMTClump(void)
{
    m_fClumpWidth  = 0.0f;
    m_fClumpHeight = 0.0f;
    m_fClumpDepth  = 0.0f;
    m_pTempAtomic  = NULL;
    m_bVisible     = TRUE;    
    m_pClump       = NULL;
    m_pResourceClump = NULL;
    ZeroMemory(m_strClumpName, sizeof(m_strClumpName));
}

CMTClump::~CMTClump(void)
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
// CallBack 함수 설정
//////////////////////////////////////////////////////////////////////////
RpAtomic* CMTClump::GetClumpAllAtomics(RpAtomic* pAtomic, void* pData)
{
	ATOMIC_TABLE_MAP *pTable = (ATOMIC_TABLE_MAP *)pData;

	int count = RpGeometryGetUserDataArrayCount(RpAtomicGetGeometry(pAtomic));
	RpUserDataArray *pUserData = RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
	char *chBuffer = RpUserDataArrayGetString(pUserData, 0);

    Helper_SetAtomicAllMaterialSkinInfo(pAtomic);

	if(count > 1)
	{
		(*pTable)[chBuffer] = pAtomic;
	}

    // 화면에 표시될 정보들을 설정한다.
    RpGeometry* pGeom = RpAtomicGetGeometry(pAtomic);
    if(pGeom)
    {
        m_nTotalTriangels += RpGeometryGetNumTriangles(pGeom);
        m_nTotalVertices  += RpGeometryGetNumVertices(pGeom);
    }

	return pAtomic;
}

RwFrame* CMTClump::GetChildFrame(RwFrame* pFrame, void* pData)
{
	FRAME_TABLE_MAP *pTable = (FRAME_TABLE_MAP *)pData;

	RpUserDataArray *pUserData = RwFrameGetUserDataArray(pFrame, 0);
	if(pUserData != NULL)
	{
		char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
		{
			(*pTable)[chBuffer] = pFrame;
		}
		RwFrameForAllChildren(pFrame, GetChildFrame, pData);
	}

	return pFrame;
}

RpMaterial* CMTClump::GetAllMaterials(RpMaterial* material, void* pData)
{
    CMTClump* pThisCharInfo = (CMTClump*)pData;	

	RpUserDataArray *pUserData = RpMaterialGetUserDataArray(material, 0);

	if(pUserData != NULL)
	{
		char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
		{
			pThisCharInfo->m_mapMaterialtable[chBuffer] = material;

            // Material - Atomic Map을 만든다.
            pThisCharInfo->m_mapMaterialAtomicTable[material] = m_pTempAtomic;
		}
	}
	return material;
}
//////////////////////////////////////////////////////////////////////////


RwBool CMTClump::Load( RwChar* szFileName ) 
{
    USES_CONVERSION;

    if(!szFileName)
        return FALSE;

    if(m_pResourceClump)
    {
        Destroy();
    }

    m_pResourceClump = CNtlPLResourceManager::GetInstance()->LoadClump(szFileName, CModelToolApplication::GetInstance()->GetTexturePath());
    if(!m_pResourceClump)
        return FALSE;

    m_pClump = m_pResourceClump->GetClump();
    if(!m_pClump)
        return FALSE;

    sprintf_s(m_strClumpName, 1024, "%s", szFileName);    

    SetClumpInfo();
    SetCartoon();

    // world에 clump를 추가한다.
    RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pClump);

    return TRUE;
}

RwBool CMTClump::Save( RwChar* szFileName ) 
{
    if(!szFileName || !m_pClump)
        return FALSE;

    RwStream* pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, szFileName);  
    if(!pStream)
        return FALSE;

    RpClumpStreamWrite(m_pClump, pStream);
    RwStreamClose(pStream, NULL);    

    return TRUE;
}

/**
 * clump의 정보 테이블을 생성한다.
 * \param pClump 정보를 생성할 clump
 * return 성공 유무
 */
RwBool CMTClump::Create(RpClump* pClump, RwChar* szClumpName)
{
	NTL_PRE(pClump);

    if(m_pResourceClump)
    {
        Destroy();
    }

	m_pClump = pClump;
    sprintf_s(m_strClumpName, 1024, "%s", szClumpName);    

	SetClumpInfo();     // Clump 내부 정보 생성

	return TRUE;
}

void CMTClump::Destroy()
{
	m_mapFrameTable.clear();
	m_mapMaterialtable.clear();
	m_mapAtomicTable.clear();
    m_mapMaterialAtomicTable.clear();

    for(size_t i = 0; i < m_vMultiTexture.size(); ++i)
    {
        if(m_vMultiTexture[i])
        {
            CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_vMultiTexture[i]);
            m_vMultiTexture[i] = NULL;
        }
    }

    ClearBB();

    if(m_pResourceClump)
    {
        RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pClump);
        CNtlPLResourceManager::GetInstance()->UnLoad(m_pResourceClump);  
        m_pClump = NULL;
        m_pResourceClump = NULL;
    }
}

void CMTClump::SetClumpInfo()
{
	NTL_PRE(m_pClump);

    m_nTotalTriangels = 0;
    m_nTotalVertices  = 0;

	// 모든 Atomic을 맵에 담는다.
	RpClumpForAllAtomics(m_pClump, GetClumpAllAtomics, &m_mapAtomicTable);
	// 모든 Frame을 맵에 답는다.
	RwFrameForAllChildren(RpClumpGetFrame(m_pClump), GetChildFrame, &m_mapFrameTable);

	// 모든 Material을 맵에 담는다.
	RpGeometry* pGeom = NULL;
	ATOMIC_TABLE_ITER mapIter;
	for(mapIter = m_mapAtomicTable.begin(); mapIter != m_mapAtomicTable.end(); ++mapIter)
	{
		pGeom = RpAtomicGetGeometry(mapIter->second);
		if(pGeom)
		{
            // 현재 Atomic 포인터를 저장한다.
            m_pTempAtomic = mapIter->second;

            // Material - Atomic 세트를 만들기 위해서 this 포인터를 넘겨준다.
			RpGeometryForAllMaterials(pGeom, GetAllMaterials, this);

            // 머테리얼 렌더링 파이프라인을 설정한다.
            RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);
            for(int i = 0; i < nMatCount; i++)
            {//
                RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
                if(pMaterial)
                {
                    RwRGBA	sColor;
                    sColor.red		= 255;
                    sColor.green	= 255;
                    sColor.blue		= 255;
                    sColor.alpha	= 255;
                    RpNtlMaterialExtSetSkinColor(pMaterial, &sColor);

                    // get through the proper renderpipe
                    RpNtlMaterialSetRenderCB(pMaterial, CNtlPLCharacter::fpRenderCB);
                }
            }
		}
	}
    
    // 높이와 넓이를 갱신한다.
    UpdateHeightWidth();

}

void CMTClump::UpdateHeightWidth()
{
    m_fClumpWidth  = 0.0f;
    m_fClumpHeight = 0.0f;

    // 바운딩 박스를 만들어서 계산한다.
    RwBBox boundingBox;
    boundingBox.sup.x = boundingBox.sup.y = boundingBox.sup.z = -RwRealMAXVAL;
    boundingBox.inf.x = boundingBox.inf.y = boundingBox.inf.z = RwRealMAXVAL;

    ATOMIC_TABLE_ITER it;
    for(it = m_mapAtomicTable.begin(); it != m_mapAtomicTable.end(); ++it)
    {
        RpAtomic* pAtomic = it->second;
        if(!pAtomic)
            continue;

        RpGeometry* pGeom = NULL;
        RwV3d *vertIn = NULL, *vertOut = NULL;
        RwInt32 numVerts = 0;
        RwMatrix transForm;	

        pGeom = RpAtomicGetGeometry(pAtomic);
        numVerts = RpGeometryGetNumVertices(pGeom);

        // Vertex positions in atomic local-space
        vertIn = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(pGeom, 0));

        vertOut = new RwV3d[numVerts];

        transForm = *RwFrameGetLTM(RpAtomicGetFrame(pAtomic));

        RwV3dTransformPoints(vertOut, vertIn, numVerts, &transForm);

        for(int i = 0; i < numVerts; ++i)
        {
            RwBBoxAddPoint(&boundingBox, &vertOut[i]);
        }

        delete[] vertOut;
    }

    m_bboxCharacter = boundingBox;
    m_fClumpWidth = boundingBox.sup.x - boundingBox.inf.x;
    m_fClumpHeight = boundingBox.sup.y - boundingBox.inf.y;
    m_fClumpDepth  = boundingBox.sup.z - boundingBox.inf.z;
}

RpAtomic* CMTClump::GetAtomicByName(RwChar* chAtomicName)
{
	std::string atomicName = chAtomicName;

	if(m_mapAtomicTable.find(atomicName) == m_mapAtomicTable.end())
	{
		return NULL;
	}

	return m_mapAtomicTable[atomicName];
}

RpAtomic* CMTClump::GetAtomicByMaterial(RpMaterial* pMaterial)
{
    if(!pMaterial)
        return NULL;

    if(m_mapMaterialAtomicTable.find(pMaterial) == m_mapMaterialAtomicTable.end())
    {
        return NULL;
    }

    return m_mapMaterialAtomicTable[pMaterial];
}


RpMaterial* CMTClump::GetMaterialByName(RwChar* chMaterialName)
{
	std::string materialName = chMaterialName;

	if(m_mapMaterialtable.find(materialName) == m_mapMaterialtable.end())
	{
		return NULL;
	}

	return m_mapMaterialtable[materialName];
}


RwFrame* CMTClump::GetFrameByName(RwChar* chFrameName)
{
	std::string frameName = chFrameName;

	if(m_mapFrameTable.find(chFrameName) == m_mapFrameTable.end())
	{
		return NULL;
	}

	return m_mapFrameTable[frameName];
}


void CMTClump::DisplayInfo(RtCharset* pCharSet)
{
    if(!pCharSet)
        return;

    RwChar caption[256] = {0,};

    // Atomic 개수
    RsSprintf(caption, RWSTRING("Atomics : %d"),m_mapAtomicTable.size());
    RsCharsetPrint(pCharSet, caption, 0, -4, rsPRINTPOSBOTTOMRIGHT);

    // Triangle 개수
    RsSprintf(caption, RWSTRING("Triangels : %d"), m_nTotalTriangels);
    RsCharsetPrint(pCharSet, caption, 0, -3, rsPRINTPOSBOTTOMRIGHT);
    
    // vertices 개수
    RsSprintf(caption, RWSTRING("Vertices : %d"), m_nTotalVertices);
    RsCharsetPrint(pCharSet, caption, 0, -2, rsPRINTPOSBOTTOMRIGHT);

    // 넓이와 높이
    RsSprintf(caption, RWSTRING("Hieght : %.2f (m)"), m_fClumpHeight);
    RsCharsetPrint(pCharSet, caption, 0, -1, rsPRINTPOSBOTTOMRIGHT);
    RsSprintf(caption, RWSTRING("Width : %.2f (m)"), m_fClumpWidth);
    RsCharsetPrint(pCharSet, caption, 0, 0, rsPRINTPOSBOTTOMRIGHT);

}

RwBool CMTClump::RemoveAtomic( RwChar* chAtomicName ) 
{
    NTL_PRE(chAtomicName);

    RpAtomic* pAtomic = GetAtomicByName(chAtomicName);
    if(!pAtomic)
        return FALSE;

    RpClumpRemoveAtomic(m_pClump, pAtomic);
    RpSkinAtomicSetHAnimHierarchy(pAtomic, NULL);

    RpWorldRemoveAtomic(CNtlPLGlobal::m_pRpWorld, pAtomic);
    RpAtomicDestroy(pAtomic);

    return TRUE;
}

std::vector<char*> CMTClump::GetAtomicNames() 
{
    std::vector<char*> vAtomicNames;

    ATOMIC_TABLE_ITER mapIt;
    for(mapIt = m_mapAtomicTable.begin(); mapIt != m_mapAtomicTable.end(); ++mapIt)
    {
        char* atomicName = (char*)(mapIt->first).c_str();  
        vAtomicNames.push_back(atomicName);
    }

    return vAtomicNames;
}

std::vector<char*> CMTClump::GetMaterialNames() 
{
    std::vector<char*> vMaterialNames;

    MATERIAL_TABLE_ITER mapIt;
    for(mapIt = m_mapMaterialtable.begin(); mapIt != m_mapMaterialtable.end(); ++mapIt)
    {
        char* chMaterialName = (char*)(mapIt->first).c_str();
        vMaterialNames.push_back(chMaterialName);
    }

    return vMaterialNames;
}

void CMTClump::SetViewEdge( RwBool bVisible ) 
{
    if(!m_pClump)
        return;

    ATOMIC_TABLE_ITER it;
    for(it = m_mapAtomicTable.begin(); it != m_mapAtomicTable.end(); ++it)
    {
        RpAtomic* pAtomic = it->second;
        if(pAtomic)
        {
            RwInt32 flag = RpNtlAtomicGetFlag(pAtomic);

            if(!bVisible)
            {
                flag |= NTL_TOON_NOT_EDGE;        
            }
            else if(flag & NTL_TOON_NOT_EDGE)
            {
                flag ^= NTL_TOON_NOT_EDGE;        
            }

            RpNtlAtomicSetFlag(pAtomic, flag);
        }
    }
}

void CMTClump::SetAtomicFlag( ENtlAtomicFlag nFlagType, RwChar* szAtomicName, RwBool bEnable ) 
{
    if(!szAtomicName)
        return;

    RpAtomic* pAtomic = GetAtomicByName(szAtomicName);
    if(!pAtomic)
        return;

    SetAtomicFlag(nFlagType, pAtomic, bEnable);    
}

void CMTClump::SetAtomicFlag( ENtlAtomicFlag nFlag, RpAtomic* pAtomic, RwBool bEnable ) 
{
    RwInt32 flag = RpNtlAtomicGetFlag(pAtomic);

    if(bEnable)
    {
        flag |= nFlag;
    }
    else if(flag & nFlag)
    {
        flag ^= nFlag;
    }

    RpNtlAtomicSetFlag(pAtomic, flag);
}

void CMTClump::SetMaterialColor( RwChar* chMaterialName, int r, int g, int b ) 
{
    NTL_PRE(chMaterialName);

    RpMaterial* pMaterial = GetMaterialByName(chMaterialName);
    if(!pMaterial)
        return;

    RwRGBA color;
    color.red = r;
    color.green = g;
    color.blue = b;
    color.alpha = 255;
    
	//by HongHoDong (2006. 7. 19.) Material 관련 삭제
    //RpNtlToonMaterialSetColor(pMaterial, &color);

    // Color를 설정할때만 MATERIAL_COLOR Flag를 세팅한다.
    RpAtomic* pAtomic = GetAtomicByMaterial(pMaterial);
    RwInt32 flag = RpNtlAtomicGetFlag(pAtomic);
   // flag |= NTL_TOON_MATERIAL_COLOR;
    RpNtlAtomicSetFlag(pAtomic, flag);  
}

RwBool CMTClump::SetMultiTexture( RwChar* chMaterialName, RwChar* chTextureName, RwChar* chTexturePath ) 
{
    if(!chMaterialName)
        return FALSE;

    RpMaterial* pMaterial = GetMaterialByName(chMaterialName);
    if(!pMaterial)
        return FALSE;

    // 멀티 텍스쳐 제거
    if(chTextureName == NULL)
    {
        RwBool bReturn = NtlMatExtSetMultiTexture(pMaterial, NULL);
        return bReturn;
    }


    // Texture를 생성한다.    
    RwTexture* pMultiTexture = CNtlPLResourceManager::GetInstance()->LoadTexture(chTextureName, chTexturePath);

    if(!pMultiTexture)
        return FALSE;

    // 나중에 해제를 위해서 Vector에 넣는다.
    m_vMultiTexture.push_back(pMultiTexture);

    // Material에 적용한다.
    RwBool bReturn = NtlMatExtSetMultiTexture(pMaterial, pMultiTexture);

    return bReturn;
}

void CMTClump::SetVisible( RwBool bVisible ) 
{
    if(!m_pClump || m_bVisible == bVisible || !CNtlPLGlobal::m_pRpWorld)
        return;

    m_bVisible = bVisible;
    if(m_bVisible)
    {
        RpWorldAddClump(CNtlPLGlobal::m_pRpWorld, m_pClump);
    }
    else
    {
        RpWorldRemoveClump(CNtlPLGlobal::m_pRpWorld, m_pClump);
    }    
}

void CMTClump::CreateBB( RwChar* chAtomicName ) 
{
    NTL_PRE(chAtomicName);
    
    RwChar* szAtomicName = new RwChar[64];
    sprintf(szAtomicName,"%s", chAtomicName);
    m_vecBB.push_back(szAtomicName);
}

void CMTClump::ClearBB() 
{
    for(UINT i = 0; i < m_vecBB.size(); ++i)
    {
        RwChar* szChar = m_vecBB[i];
        delete[] szChar;
        szChar = NULL;
    }

    m_vecBB.clear();
}

void CMTClump::RenderAtomicBB() 
{
    if(m_bVisible)
    {
        UpdateAtomicBB();
    }    
}

void CMTClump::UpdateAtomicBB() 
{
    for(UINT i = 0; i < m_vecBB.size(); ++i)
    {
        // 1. 우선 Name에 해당하는 Atomic을 찾아낸다.
        RpAtomic* pAtomic = GetAtomicByName(m_vecBB[i]);
        if(!pAtomic)
            return;

        RwBBox boundingBox;
        boundingBox.sup.x = boundingBox.sup.y = boundingBox.sup.z = -RwRealMAXVAL;
        boundingBox.inf.x = boundingBox.inf.y = boundingBox.inf.z = RwRealMAXVAL;

        RpGeometry* pGeom = NULL;
        RwV3d *vertIn = NULL, *vertOut = NULL;
        RwInt32 numVerts = 0;
        RwMatrix transForm;	

        pGeom = RpAtomicGetGeometry(pAtomic);
        numVerts = RpGeometryGetNumVertices(pGeom);

        // Vertex positions in atomic local-space
        vertIn = RpMorphTargetGetVertices(RpGeometryGetMorphTarget(pGeom, 0));

        vertOut = new RwV3d[numVerts];

        transForm = *RwFrameGetLTM(RpAtomicGetFrame(pAtomic));

        RwV3dTransformPoints(vertOut, vertIn, numVerts, &transForm);

        // Add bounding box
        for(int i = 0; i < numVerts; ++i)
        {
            RwBBoxAddPoint(&boundingBox, &vertOut[i]);
        }

        RenderBB(&boundingBox);

        delete[] vertOut;
    }  
}

void CMTClump::RenderBB( RwBBox* pBBox, int r /*= 255*/, int g /*= 255*/, int b /*= 0*/ ) 
{
    NTL_PRE(pBBox);

    RwIm3DVertex imVertex[8];
    RwMatrix* ltm = NULL;
    RwImVertexIndex indices[24] = 
    {
        0, 1, 1, 3, 3, 2, 2, 0, 4, 5, 5, 7,
        7, 6, 6, 4, 0, 4, 1, 5, 2, 6, 3, 7
    };

    for(int i = 0; i < 8; ++i)
    {
        RwIm3DVertexSetPos(&imVertex[i],
            i & 1 ? pBBox->sup.x : pBBox->inf.x,
            i & 2 ? pBBox->sup.y : pBBox->inf.y,
            i & 4 ? pBBox->sup.z : pBBox->inf.z);

        RwIm3DVertexSetRGBA(&imVertex[i], r, g, b, 255);
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)NULL);

    if(RwIm3DTransform(imVertex, 8, ltm, rwIM3D_ALLOPAQUE))
    {
        RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indices, 24);
        RwIm3DEnd();
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void*)rwSHADEMODEGOURAUD);
}

void CMTClump::CreateCartoon() 
{
    SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
    m_ToonData.pTexture = CNtlPLResourceManager::GetInstance()->LoadTexture("Smooth", "texture/toon/");

    RwRGBA color = {0,0,0,255};
    m_ToonData.pToonInk = RpToonInkCreate();	
    RpToonInkSetOverallThickness(m_ToonData.pToonInk, 1.2f);
    RpToonInkSetColor(m_ToonData.pToonInk, color );
    RpToonInkSetName(m_ToonData.pToonInk, "silhouette" );

    m_ToonData.pToonPaint = RpToonPaintCreate();
    RpToonPaintSetGradientTexture(m_ToonData.pToonPaint, m_ToonData.pTexture);
}

void CMTClump::SetToonThickness( RwReal fThickness ) 
{
    RpToonInkSetOverallThickness(m_ToonData.pToonInk, fThickness);
}

void CMTClump::SetToonInkColor( RwRGBA color ) 
{
    RpToonInkSetColor(m_ToonData.pToonInk, color );    
}

void CMTClump::SetCartoon() 
{
    // Toon Setting
    Helper_SetToonClump(m_pClump, &m_ToonData);
}

void CMTClump::DestroyCartoon() 
{
    if(m_ToonData.pToonPaint)
        RpToonPaintDestroy(m_ToonData.pToonPaint);
    if(m_ToonData.pToonInk)
        RpToonInkDestroy(m_ToonData.pToonInk);
    if(m_ToonData.pTexture)
        CNtlPLResourceManager::GetInstance()->UnLoadTexture(m_ToonData.pTexture);

}

RwReal CMTClump::GetMaxWidthHeight() 
{
    if(m_fClumpWidth > m_fClumpHeight)
    {
        return m_fClumpWidth;
    }

    return m_fClumpHeight;
}

RwBool CMTClump::CollisionDataBuild( RwChar* szAtomicName ) 
{
    if(!szAtomicName)
        return FALSE;

    RpAtomic* pAtomic = GetAtomicByName(szAtomicName);
    if(!pAtomic)
        return FALSE;

    if(RpCollisionGeometryQueryData(RpAtomicGetGeometry(pAtomic)) == FALSE)
    {
        if(RpCollisionGeometryBuildData(RpAtomicGetGeometry(pAtomic), NULL) == NULL)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void CMTClump::RenderWireFrame() 
{
	if(!m_pClump)
		return;

	RpClumpForAllAtomics(m_pClump, API_PL_RenderWireMesh, NULL);
}



