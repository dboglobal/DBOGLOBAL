#include "StdAfx.h"
#include "ModelToolApplication.h"
#include "NtlText.h"
#include "NtlPLMaterialExtWrapper.h"
#include "MTCharacter.h"
#include "NtlTypeAnimData.h"
#include "NtlInstanceEffect.h"
#include "NtlSoundManager.h"
#include "NtlPLRenderState.h"
#include "NtlMath.h"
#include "NtlPLEntityBlend.h"
 
static RwUInt32             NumIm3DVerts = 0;
static RwIm3DVertex         Im3DVerts[MAX_NUM_SCALE_BONE];
static RwImVertexIndex      Im3DIndices[MAX_NUM_SCALE_BONE << 1];

static RwFrame* GetChildFrame(RwFrame *pFrame, void *pData)
{
    FRAME_MAP *pTable = (FRAME_MAP *)pData;

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

void* CMTCharacter::operator new(size_t size)
{
    return ::operator new(size);
}


void CMTCharacter::operator delete(void *pObj)
{
    ::operator delete(pObj);
}

CMTCharacter::CMTCharacter(void)
{
    SetClassType(PLENTITY_CHARACTER);
    m_bAnim         = FALSE;
    m_nBoneCount    = 0;
    m_pAnimation    = NULL;    
    m_bRenderBone   = FALSE;    
    m_uiCurrentAnimKey = 0;
    m_pEventHissidianDir = NULL;
    m_pAlphaBlend = NULL;
    m_bRemoveColorChange = FALSE;    
    m_bStretchUpdate = FALSE;
    m_bStretchPulling = FALSE;
    m_pStretchEvent = NULL;
    m_fShakeFactor = 0.0f;
    m_fShakeMaxHeight = 0.0f;
    m_fShakeElapsedTime = 0.0f;

    m_pProperty = new CNtlPLCharacterProperty();
}

CMTCharacter::~CMTCharacter(void)
{
    if(m_pAnimation)
    {
        RtAnimAnimationDestroy(m_pAnimation);
        m_pAnimation = NULL;
    }

    NTL_DELETE(m_pProperty);
}

void CMTCharacter::Destroy() 
{
    CNtlPLCharacter::Destroy();
}

RwBool CMTCharacter::Create(const SPLEntityCreateParam *pParam /* = NULL  */)
{
	//BoneData 변경으로 인해 수정을 했습니다(2006.4.26 HongHoDong)
	//ScaleBone Data
    m_pTypeBoneData = m_pProperty->GetBoneScaleData();

	m_pTypeBoneData->bBoneScale = FALSE;    

    // Bone을 그리기위해 Vertex Indices를 초기화한다.
	RwUInt32	i, j;

	for (i=0, j=0; i < MAX_NUM_SCALE_BONE; i++)
	{
		Im3DIndices[j++] = 0;
		Im3DIndices[j++] = (RwImVertexIndex) i + 1;
		RwIm3DVertexSetRGBA(&Im3DVerts[i], 255, 0, 0, 255);
	}

	return TRUE;
}

/*!
 * \brief dff 파일을 로딩한다. (path 설정 때문에)
 * \param szDffName 로딩할 파일 이름 (*.dff) 
 * \returns 성공 유무 
 */
RwBool CMTCharacter::CreateMesh(const char *szDffName)
{
    USES_CONVERSION;

	NTL_PRE(szDffName);

    // 필요한 초기화
    m_nCurrentSelectBoneIndex = 0;

    m_pResourceClump = CNtlPLResourceManager::GetInstance()->LoadClump(szDffName, CModelToolApplication::GetInstance()->GetTexturePath());
    
	if(!m_pResourceClump)
		return FALSE;
    
    // 프로퍼티에 Mesh 정보를 설정한다.
    if(m_pProperty)
    {
        std::string strMeshFileName = GetFileNameFromPath(szDffName);
        CString strWorkPath = (CModelToolApplication::GetInstance()->GetWorkDir());
        CString strFileName = (strMeshFileName.c_str());
        CString strMeshFilePath = (szDffName);
        strWorkPath.MakeUpper();
        strMeshFilePath.MakeUpper();
        strFileName.MakeUpper();

        strMeshFilePath.Replace(strWorkPath, "");
        strMeshFilePath.Replace(strFileName, "");

        m_pProperty->SetBaseMeshFilePath((LPCSTR)(strMeshFilePath));
        m_pProperty->SetBaseMeshFileName(strMeshFileName);
    }

    // Hierarchy 정보를 가져온다.
    m_pBaseHierarchy = Helper_GetHierarchyClump(GetClump());
    NTL_ASSERTE(m_pBaseHierarchy);
    if(m_pBaseHierarchy == NULL)
        NTL_RETURN(FALSE);

    RpHAnimHierarchyAttach(m_pBaseHierarchy);
    m_nBoneCount = m_pBaseHierarchy->numNodes;
    m_pProperty->GetBoneScaleData()->nBoneCount = m_nBoneCount;    

    // world에 clump를 추가한다.
    AddWorld();

	//GetBone List
	RwFrameForAllChildren( RpClumpGetFrame(GetClump()), GetChildFrame, &m_mapFrame);

    SetAnimUpdate(TRUE);

	m_AnimLayer[CHARACTER_ANIM_LAYER_BASE].SetCallBack(this, &CMTCharacter::CallBackBaseAnim);

    m_pEquipItem = NTL_NEW CNtlPLEquipItem;
    m_pEquipItem->SetOwner(this);

    m_sScheduleResInfo.bLoadComplete = TRUE;
    m_sScheduleResInfo.bPCFlag = FALSE;

    return TRUE;

}

RwBool CMTCharacter::LoadClump(RwChar * filename)
{
	NTL_PRE(filename);


	// 이미 로딩되어있으면 제거해준다.
	if(m_pResourceClump)
	{
        Destroy();
	}
	
	if(!CreateMesh(filename))
		return FALSE;

    strcpy(m_strClumpPathName, filename);

	// Clump 관련 정보를 설정하고 표시한다.
	UpdateClumpInfo();

    // Skin Color를 위한 세팅    
    Helper_SetClumpAllAtomics(GetClump(), &m_vecAtomicList);

    for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
    {
        RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
        RpNtlAtomicSetRenderCallBack( pAtomic, RpAtomicGetRenderCallBack(pAtomic) );
        RpAtomicSetRenderCallBack(pAtomic, CNtlPLCharacter::RenderCallBack);
        RpNtlAtomicSetData(pAtomic, this);
		Helper_SetAtomicAllMaterialSkinInfo(pAtomic);        
    }

    m_bAnim = FALSE;

    // Link Effect를 설정한다.
    for(UINT i = 0; i < m_pProperty->m_vLinkEffect.size(); ++i)
    {
        SEventLinkEffect* pEventLinkEffect = m_pProperty->m_vLinkEffect[i];        
        AttachLinkEffect(pEventLinkEffect);
    }

	return TRUE;
}

void CMTCharacter::SetVisible(RwBool bVisible)
{
    CNtlPLCharacter::SetVisible(bVisible);

    if(m_pEquipItem)
    {
        // 무기를 들고있으면 무기도 사라진다.
        for each(CNtlPLItem* pitem in *(m_pEquipItem->GetEquipItemList()))
        {
            if(!pitem)
                continue;

            pitem->SetVisible(bVisible);
        }
    }

    // Link Effect도 지운다.
    m_LinkEffectInstance.SetVisible(bVisible);    

    // Loop Effect도 지운다.
    if(!bVisible)
    {
        //SetAnimUpdate(FALSE);
        ClearLoopEffect();
        ClearLoopSound();
    }
}

RwBool CMTCharacter::SaveClump(RwChar* fileName)
{
    return m_charInfo.Save(fileName);
}

void CMTCharacter::UpdateClumpInfo()
{
    m_charInfo.Destroy();    
    if(GetClump())
    {
        m_charInfo.Create(GetClump(), m_strClumpPathName);   

        // toon을 생성한다.
        CreateToonData("smooth.png", "texture/toon/");
        Helper_SetToonClump(GetClump(), &m_ToonData);
    }    
}

RwBBox CMTCharacter::CreateDefaultAnimBBox()
{
    m_pProperty->SetAnimBBox(m_charInfo.GetDefaultBBox());

    return m_charInfo.GetDefaultBBox();
}

void CMTCharacter::RenderAnimBBox()
{
    if(!m_pProperty)
        return;

    m_charInfo.RenderBB(m_pProperty->GetAnimBBox(), 0, 255, 255);
}

RwBool CMTCharacter::Update(RwReal fElapsed)
{
    // 상위 클래스에서 Bone Update에 관해서 설정된 부분이 있어서, 툴에서는 무조건 True로 한다.
    GetCharScheduleResInfo()->bPCFlag = TRUE;

    // 이벤트에 따른 처리
    UpdateStretch(fElapsed);
    UpdateCameraShake(fElapsed);

    if(m_bAnim)
    {
		CNtlPLCharacter::Update(fElapsed);
    }

    // 카메라 거리에 따른 알파처리
    if(CModelToolApplication::GetInstance()->GetEnableCameraDistAlpha())
    {
        RwReal fCamDist = CModelToolApplication::GetInstance()->GetCamera()->GetDistance();
        if(fCamDist < GetAlphaDistance())
        {
            if(!m_pAlphaBlend)
            {
                m_pAlphaBlend = GetAlphaBlendController()->AddAlpha(100.0f / 255.0f);
            }            
        }
        else if(m_pAlphaBlend)
        {
            GetAlphaBlendController()->RemoveAlpha(m_pAlphaBlend);
            SetWeightAlpha(1.0f);
            m_pAlphaBlend = NULL;
        }
    }
    else if(m_pAlphaBlend)
    {
        GetAlphaBlendController()->RemoveAlpha(m_pAlphaBlend);
        SetWeightAlpha(1.0f);
        m_pAlphaBlend = NULL;        
    }

	return TRUE;
}

RwBool CMTCharacter::Render()
{
    if(m_bRenderBone)
    {
        // 현재 선택된 Bone을 렌더링 한다.
        RenderSelectedBoneScale(m_pBaseHierarchy, m_nCurrentSelectBoneIndex);       
    }

    if(m_pEventHissidianDir)
    {
        // 설정된 Hissidian의 StartDirecton을 라인으로 그린다.
        RenderHissidainDir();
    }

	return TRUE;
}


void CMTCharacter::DisplayInfo(RtCharset* pCharSet)
{
    if(!pCharSet)
        return;
    
    RwChar caption[256] = {0,};

    // Bone 개수
    RsSprintf(caption, RWSTRING("Bones : %d"),m_nBoneCount);
    RsCharsetPrint(pCharSet, caption, 0, 0, rsPRINTPOSBOTTOMLEFT);
}

void CMTCharacter::RenderWireFrame()
{
    if(!GetClump())
        return;

    //RpClumpForAllAtomics(GetClump(), AtomicRenderWireMesh, NULL);
	RpClumpForAllAtomics(GetClump(), API_PL_RenderWireMesh, NULL);
}

void CMTCharacter::RenderHierarchy()
{
    if(!GetClump())
        return;

    RpClumpForAllAtomics(GetClump(), AtomicRenderSkeleton, NULL);
}

RpAtomic* CMTCharacter::AtomicRenderSkeleton(RpAtomic *atomic, void * data __RWUNUSED__)
{
    RpGeometry *geometry;

    geometry = RpAtomicGetGeometry(atomic);

    if( geometry )
    {
        RwFrame *frame = RwFrameGetRoot(RpAtomicGetFrame(atomic));

        if( frame != NULL)
        {
            RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
            RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
            RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

            RwFrameForAllChildren(frame, HierarchyRender, NULL);
        }
    }

    return atomic;
}

void CMTCharacter::SkeletonRender(RwMatrix *LTM, RpHAnimHierarchy *hier)
{

    if (hier)
    {
        RwMatrix *matrices;
        RpHAnimNodeInfo *finfo;
        RwMatrix *mstack[128],*parent;
        RwInt32 tos,i;
        RwIm3DVertex *im3DVertices; 
        RwIm3DVertex *imVertex;

        matrices = RpHAnimHierarchyGetMatrixArray(hier);
        finfo = hier->pNodeInfo;
        tos = 0;
        parent = mstack[0] = LTM;

        im3DVertices = (RwIm3DVertex *)RwMalloc(sizeof(RwIm3DVertex) * 128 * 8, rwID_NAOBJECT);
        imVertex = im3DVertices;
        for (i=0; i<hier->numNodes; i++)
        {
            RwV3d tmp;

            RwIm3DVertexSetPos(imVertex, parent->pos.x,parent->pos.y,parent->pos.z);
            RwIm3DVertexSetRGBA(imVertex, 255,255,0,255);
            imVertex++;

            RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
            RwIm3DVertexSetRGBA(imVertex, 255,255,0,255);
            imVertex++;

            /* drawn bone axes */
            RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
            RwIm3DVertexSetRGBA(imVertex, 255,0,0,255);
            imVertex++;
            RwV3dAdd(&tmp, &matrices[i].pos, &matrices[i].right);
            RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
            RwIm3DVertexSetRGBA(imVertex, 255,0,0,255);
            imVertex++;

            RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
            RwIm3DVertexSetRGBA(imVertex, 0,255,0,255);
            imVertex++;
            RwV3dAdd(&tmp, &matrices[i].pos, &matrices[i].up);
            RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
            RwIm3DVertexSetRGBA(imVertex, 0,255,0,255);
            imVertex++;

            RwIm3DVertexSetPos(imVertex, matrices[i].pos.x,matrices[i].pos.y,matrices[i].pos.z);
            RwIm3DVertexSetRGBA(imVertex, 0,0,255,255);
            imVertex++;
            RwV3dAdd(&tmp, &matrices[i].pos, &matrices[i].at);
            RwIm3DVertexSetPos(imVertex, tmp.x,tmp.y,tmp.z);
            RwIm3DVertexSetRGBA(imVertex, 0,0,255,255);
            imVertex++;

            if (finfo[i].flags & rpHANIMPUSHPARENTMATRIX)
            {
                mstack[++tos] = &matrices[i];
            }

            if (finfo[i].flags & rpHANIMPOPPARENTMATRIX)
            {
                parent = mstack[tos--];
            }
            else
            {
                parent = &matrices[i];
            }
        }

        if( RwIm3DTransform(im3DVertices, imVertex - im3DVertices, (RwMatrix *)NULL, 0) )
        {
            RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
            RwIm3DEnd();
        }

        RwFree(im3DVertices);
    }
}


RwFrame* CMTCharacter::HierarchyRender(RwFrame *frame, void * data __RWUNUSED__)
{
    RwMatrix *mat[2];
    RwIm3DVertex    imVertex[2];
    RwRGBA Color = {255, 0, 0, 255};

    mat[0] = RwFrameGetLTM(RwFrameGetParent(frame));
    mat[1] = RwFrameGetLTM(frame);

    RwIm3DVertexSetPos(&imVertex[0],
        (RwMatrixGetPos(mat[0]))->x,
        (RwMatrixGetPos(mat[0]))->y,
        (RwMatrixGetPos(mat[0]))->z);

    RwIm3DVertexSetRGBA(&imVertex[0], Color.red, Color.green, Color.blue, Color.alpha);

    RwIm3DVertexSetPos(&imVertex[1],
        (RwMatrixGetPos (mat[1]))->x,
        (RwMatrixGetPos (mat[1]))->y,
        (RwMatrixGetPos (mat[1]))->z);

    RwIm3DVertexSetRGBA(&imVertex[1], Color.red, Color.green, Color.blue, Color.alpha);

    if( RwIm3DTransform(imVertex, 2, (RwMatrix *)NULL, 0) )
    {
        RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
        RwIm3DEnd();
    }

    SkeletonRender(RwFrameGetLTM(frame), RpHAnimGetHierarchy(frame));

    return RwFrameForAllChildren(frame, HierarchyRender, NULL);

}

RwBool CMTCharacter::SetAnim(RwUInt32 uiKey)
{
    if( !GetClump())
        return FALSE;
	

    // 애니메이션이 변경되면, LoopEffect를 모두 소멸시킨다.
    if(m_uiCurrentAnimKey != uiKey)
    {
        ClearLoopEffect();
        ClearLoopSound();
    }
    
    m_uiCurrentAnimKey = uiKey;

    if(m_bAnim == FALSE)
    {
        m_InstanceAnimTable.Create(m_pProperty->GetAnimTable());

        if(!CreateAnim(uiKey))
            NTL_RETURN(FALSE);
        
        m_bAnim = TRUE;        
    }
    else
    {
        CNtlTypeAnimTable *pTypeAnimTable = m_pProperty->GetAnimTable();
        STypeAnimData *pTypeAnimData = pTypeAnimTable->Get(uiKey);
        if(pTypeAnimData != NULL)
        {
            // 새로 Animaiton를 읽어들일 경우(by HoDong 2006.4.26)
            pTypeAnimData->m_bReLoad = FALSE;
            if(!SetBaseAnimation(uiKey))
                return FALSE;
        }
    }

    SetAnimUpdate(TRUE);

    return TRUE;
}

RwBool CMTCharacter::SetBaseAnimation(RwUInt32 uiAnimKey, RwReal fStartTime , RwBool bLoop )
{
    SetAlpha(255);    

    if(m_bRemoveColorChange)
    {
        SetInkColor(0, 0, 0);
        SetColor(255, 255, 255);
        SetAddColor(0, 0, 0);

        m_bRemoveColorChange = FALSE;
    }

    return CNtlPLCharacter::SetBaseAnimation(uiAnimKey, fStartTime, bLoop);
}

void CMTCharacter::RenderSelectedBoneScale(RpHAnimHierarchy *pHierarchy, RwInt32 nCurrentBone)
{
	if(pHierarchy == NULL)
		return;

	RwMatrix    *matrices = RpHAnimHierarchyGetMatrixArray(pHierarchy);
    RwV3d       *rootPos;

    /*
     *  Render the currently selected bone as white lines to child bones.
     *  First set the vertex for the origin of the current bone.
     */
    rootPos = RwMatrixGetPos(&matrices[nCurrentBone]);    
    NumIm3DVerts = 0;

    /* 
     *  Does the current bone have any children?
     */
    if (pHierarchy->pNodeInfo[nCurrentBone].flags & rpHANIMPOPPARENTMATRIX)
    {
        RwV3d    pos;

        /*
         *  No children. We just draw a line along the bone's x axis.
         */
        pos = *rootPos;
        RwV3dIncrementScaled(
            &pos, RwMatrixGetRight(&matrices[nCurrentBone]), 0.1f);
        RwIm3DVertexSetPos(&Im3DVerts[NumIm3DVerts], pos.x, pos.y, pos.z);
        NumIm3DVerts++;
    }
    else
    {
        RwInt32     parentsOnStack = 0;
        RwUInt32    currentIndex = nCurrentBone;

        /*
         *  Pick up all children of the current bone and draw a line
         *  to each of them.
         */
        while (parentsOnStack > -1)
        {
            RwUInt32    flags;

            currentIndex++;
            flags = pHierarchy->pNodeInfo[currentIndex].flags;

            /*
             *  Is this bone an immediate child of the current bone?
             */
            if (parentsOnStack == 0)
            {
                RwV3d   *pos = RwMatrixGetPos(matrices + currentIndex);

                RwIm3DVertexSetPos(&Im3DVerts[NumIm3DVerts], 
                    pos->x, pos->y, pos->z);
                NumIm3DVerts++;

                if (!(flags & rpHANIMPUSHPARENTMATRIX))
                {
                    /* 
                     *  This was the last sibling, so quit.
                     */
                    break;
                }
            }

            if (flags & rpHANIMPUSHPARENTMATRIX)
            {
                parentsOnStack++;
            }

            if (flags & rpHANIMPOPPARENTMATRIX)
            {
                parentsOnStack--;
            }
        }
    }

    /*
     *  Now render the lines.
     */
    if (RwIm3DTransform(Im3DVerts, NumIm3DVerts, NULL, 
            rwIM3D_ALLOPAQUE | rwIM3D_VERTEXXYZ))
    {
        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)FALSE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)FALSE);

        RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, Im3DIndices, 
            (NumIm3DVerts - 1) << 1);
        RwIm3DEnd();

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);
    }
}

RwBool CMTCharacter::SetSelectBone(int nBoneIndex)
{
    if(nBoneIndex > m_nBoneCount)
        return FALSE;

    m_nCurrentSelectBoneIndex = nBoneIndex;

    return TRUE;
}

void CMTCharacter::ResetProperty()
{
    if(!m_pProperty)
        return;
    
    //  우선은 애니메이션 데이터와 BoneData만 초기화 한다.
    CNtlTypeAnimTable* pAnimTable = m_pProperty->GetAnimTable();
    pAnimTable->Destroy();

    STypeBoneData newBoneData;
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    *pBoneData = newBoneData;
}

int CMTCharacter::CallBackBaseAnim(void* pEventData)
{
    SEventAnim* pEventAnim = (SEventAnim*)pEventData;

    //  이 이벤트들은 모델툴에서는 처리하지 않는다.
    if(pEventAnim->eEventID == EVENT_ANIM_END ||
       pEventAnim->eEventID == EVENT_ANIM_FOOT_STEP ||
       pEventAnim->eEventID == EVENT_ANIM_SUB_WEAPON)
       return TRUE;

    if(pEventAnim->eEventID == EVENT_ANIM_ALPHA)
    {
        OnEventAlphaFade((SEventAlpha*)pEventData);
    }

    return CNtlPLCharacter::CallBackBaseAnim(pEventData);
}


void CMTCharacter::OnEventHit( SEventAnimHit* pEventHit ) 
{
    if(CModelToolApplication::GetInstance()->GetHitEffect() &&
       strlen(pEventHit->chWordEffect) > 0 )            
    {
        // Word Effect를 표시한다.
        CNtlPLEntity* pPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventHit->chWordEffect);
    }

    // 카메라 셰이크는 모델툴에서는 따로 구현한다. (시뮬 레이어이기 때문)
    if(pEventHit->bCameraShake)
    {
        m_fShakeFactor = pEventHit->fCameraShakeFactor;
        m_fShakeMaxHeight = pEventHit->fCameraShakeMaxHeight;
        m_fShakeElapsedTime = 0.0f;
    }    
}

void CMTCharacter::OnEventWeightTime( SEventWeightTime* pEventTime ) 
{
   // NOTE : 생성만하면 소멸은 시간이 되면 알아서 소멸된다.
   CreateWeightElapsedController(pEventTime->fLifeTime, pEventTime->fWeightValue);
}

CNtlPLItem* CMTCharacter::SetChangeEquipItemForTool( CNtlPLItem *pItem ) 
{
    if(!pItem)
        return NULL;

    CNtlPLItem* pRemoveItem = NULL;

    // Equip Item이 아닌 맨몸일때는 Item에 해당하는 Atomic을 제거한다.    
    if(pItem->GetResType() == ITEM_RES_EQUIP_BODY)
    {
        RwBool bExist = FALSE;
        for each(CNtlPLItem* pEquipItem in *(m_pEquipItem->GetEquipItemList()))
        {
            if(!pEquipItem)
                continue;

            if(pEquipItem->GetResType() == ITEM_RES_EQUIP_BODY &&
               pEquipItem->GetEquipSlotType() == pItem->GetEquipSlotType())
            {
                // 이미 아이템이 존재하면 그냥 나간다.
                pRemoveItem = pEquipItem;                
                bExist = TRUE;
                break;
            }
        }

        if(bExist == FALSE)
        {
            // 기존의 아이템이 존재하지 않으면 맨몸을 제거한다.
            CMTItem* pMTItem = (CMTItem*)pItem;        
            std::vector<char*> vAtomicNames = pMTItem->GetClumpInfo()->GetAtomicNames();
            for(UINT i = 0; i < vAtomicNames.size(); ++i)
            {
                m_charInfo.RemoveAtomic(vAtomicNames[i]);
            }
        }

        
    } 
    else    
    {
        // 무기인류 경우엔 기존의 아이템이 자동으로 제거 되지 않는다.
        // 이곳에서 수동으로 제거해 준다.
        for each(CNtlPLItem* pEquipItem in *(m_pEquipItem->GetEquipItemList()))
        {
            if(!pEquipItem)
                continue;

            if(pEquipItem->GetEquipSlotType() == pItem->GetEquipSlotType())
            {
                // 이미 장착된 아이템과 장착할 아이템이 같으면 NULL을 리턴한다.
                if(pEquipItem == pItem)
                    return NULL;

                pRemoveItem = pEquipItem;                
                SetRemoveEquipItem(pEquipItem);
                break;
            }
        }
    }
    
    CNtlPLCharacter::SetChangeEquipItem(pItem);

    pItem->SetVisible(TRUE);

    return pRemoveItem;
}

RwBool CMTCharacter::SetRemoveEquipItem( CNtlPLItem *pItem ) 
{
    return CNtlPLCharacter::SetRemoveEquipItem(pItem);
}

CNtlInstanceEffect* CMTCharacter::GetLinkEffectFromName(RwChar* szName)
{
    return m_LinkEffectInstance.GetLinkEffectFromName(szName);
}

void CMTCharacter::RenderHissidainDir() 
{
    if(strlen(m_pEventHissidianDir->chBoneName) <= 0)
        return;

    RwMatrix* pBoneMat = GetBoneMatrix(m_pEventHissidianDir->chBoneName);
    if(!pBoneMat)
        return;

    RwIm3DVertex* vertices = NULL;
    RwInt32 nVertexCnt = 0;
    RwV3d* pvBonePos = RwMatrixGetPos(pBoneMat);
    RwReal fDirectionLength = 10.0f;
    RwV3d  vDirEnd;

    if(m_pEventHissidianDir->uiProjectileEffectType == BEID_PROJ_HISSIDAN &&
       m_pEventHissidianDir->uEffectTypeExtraData.hissidanData.bApplyAngle)
    {
        RwMatrix matRot;
        RwMatrixSetIdentity(&matRot);
        RwMatrixRotate(&matRot, &YAxis, m_pEventHissidianDir->uEffectTypeExtraData.hissidanData.v2dAngle.y, rwCOMBINEPOSTCONCAT);
        RwMatrixRotate(&matRot, &XAxis, m_pEventHissidianDir->uEffectTypeExtraData.hissidanData.v2dAngle.x, rwCOMBINEPOSTCONCAT);
        RwV3dTransformVector(&vDirEnd, &GetDirection(), &matRot);
        RwV3dNormalize(&vDirEnd, &vDirEnd);

        vDirEnd = *pvBonePos + vDirEnd * fDirectionLength;

        vertices = NTL_NEW RwIm3DVertex[2];
        nVertexCnt = 2;
        vertices[0].objVertex = *pvBonePos;
        vertices[1].objVertex = vDirEnd;  
    }
    else if(m_pEventHissidianDir->uiProjectileEffectType == BEID_PROJ_MULTI_HISSIDAN &&
            m_pEventHissidianDir->uEffectTypeExtraData.multiHissidanData.nCount > 0)
    {
        RwInt32 nMultiHissidanCnt = m_pEventHissidianDir->uEffectTypeExtraData.multiHissidanData.nCount;
        vertices = NTL_NEW RwIm3DVertex[nMultiHissidanCnt * 2];        
        
        for(int i = 0; i < nMultiHissidanCnt; ++i)
        {
            RwMatrix matRot;
            RwMatrixSetIdentity(&matRot);
            RwMatrixRotate(&matRot, &YAxis, m_pEventHissidianDir->uEffectTypeExtraData.multiHissidanData.pArrayAngle[i].y, rwCOMBINEPOSTCONCAT);
            RwMatrixRotate(&matRot, &XAxis, m_pEventHissidianDir->uEffectTypeExtraData.multiHissidanData.pArrayAngle[i].x, rwCOMBINEPOSTCONCAT);
            RwV3dTransformVector(&vDirEnd, &GetDirection(), &matRot);
            RwV3dNormalize(&vDirEnd, &vDirEnd);        
            
            vDirEnd = *pvBonePos + vDirEnd * fDirectionLength;

            vertices[i * 2].objVertex = *pvBonePos;           // 시작점은 Bone의 위치
            vertices[i * 2 + 1].objVertex = vDirEnd;          // 끝점은 방향.. -> 시작,1번째 방향,시작,2번째 방향... 이런식으로 배열에 채워진다. (LineList를 위해서)
            nVertexCnt += 2;
        }
    }

    if(vertices && nVertexCnt > 0)
    {
        for(int i = 0; i < nVertexCnt; ++i)
        {
            RwIm3DVertexSetRGBA(&vertices[i], 255, 0, 0, 255);            
        }

        RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
        RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

        if(RwIm3DTransform(vertices, nVertexCnt, NULL, rwIM3D_ALLOPAQUE | rwIM3D_VERTEXXYZ))
        {
            RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);
            RwIm3DEnd();
        }      

        NTL_ARRAY_DELETE(vertices);
    }
}

void CMTCharacter::UpdateAtomicsColor() 
{
    Helper_DestroyAllAtomics(&m_vecAtomicList);
    m_vecAtomicList.clear();

    Helper_SetClumpAllAtomics(GetClump(), &m_vecAtomicList);

    for(RwUInt32 i = 0; i < m_vecAtomicList.size(); i++)
    {
        RpAtomic *pAtomic = m_vecAtomicList[i]->pData;
        RpNtlAtomicSetData(pAtomic, this);
		Helper_SetAtomicAllMaterialSkinInfo(pAtomic);
        //Helper_SetAtomicAllMaterialData(m_vecAtomicList[i]->pData);
    }

    // 모델툴에서는 RenderCallBack을 사용하지 않고 그대로 적용한다
    for each(SEntityAtomicData* pAtomicData in m_vecAtomicList)
    {
        RpAtomic* pAtomic = pAtomicData->pData;
        if(!pAtomic)
            continue;

        RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
        if(pGeom)
        {
            RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

            for(int i = 0; i < nMatCount; i++)
            {
                RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, i);
                if(pMaterial)
                {

                    RwRGBAReal	sColorReal;
                    RwRGBARealFromRwRGBA(&sColorReal, GetColor());
                    
                    if( NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_SKIN_COLOR )
                    {
                        RwRGBAReal	sSkinColorReal;
                        RwRGBARealFromRwRGBA(&sSkinColorReal, GetSkinColor());

                        sColorReal.red		= sColorReal.red	* sSkinColorReal.red;
                        sColorReal.green	= sColorReal.green	* sSkinColorReal.green;
                        sColorReal.blue		= sColorReal.blue	* sSkinColorReal.blue;
                    }
                    else if( NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_HEAD_COLOR)
                    {
                        RwRGBAReal	sHeadColorReal;
                        RwRGBARealFromRwRGBA(&sHeadColorReal, GetHeadColor());

                        sColorReal.red		= sColorReal.red	* sHeadColorReal.red;
                        sColorReal.green	= sColorReal.green	* sHeadColorReal.green;
                        sColorReal.blue		= sColorReal.blue	* sHeadColorReal.blue;
                    }

                    RwRGBA	sColor;
                    sColor.red		= RwUInt8((sColorReal.red)*255.f);
                    sColor.green	= RwUInt8((sColorReal.green)*255.f);
                    sColor.blue		= RwUInt8((sColorReal.blue)*255.f);
                    sColor.alpha	= RwUInt8((sColorReal.alpha)*255.f);
                    RpMaterialSetColor(pMaterial, &sColor);
                }
            }
        }        
    }    
}

void CMTCharacter::SetSkinColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue ) 
{
    CNtlPLCharacter::SetSkinColor(byRed, byGreen, byBlue);

    UpdateAtomicsColor();
}

void CMTCharacter::SetHeadColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue ) 
{
    CNtlPLCharacter::SetHeadColor(byRed, byGreen, byBlue);

    UpdateAtomicsColor();
}

void CMTCharacter::OnEventAlphaFade( SEventAlpha* pEventAlpha ) 
{
    // CLUMP의 경우는 SL단에서 처리하기 때문에, Model Tool에서 테스트를 위해서 여기서 수동으로 처리한다.
    // 하지만 Atomic의 경우는 PL단(PLCharacter)에서 처리하기 때문에 상위 클래스에 넘기면 된다.
    if(pEventAlpha->eAlphaEventType == SEventAlpha::E_ALPHA_EVENT_CLUMP)
    {
        GetAlphaBlendController()->AddAlpha(pEventAlpha->nStartAlpha / 255.0f, 
                                            pEventAlpha->nDestAlpha / 255.0f,
                                            pEventAlpha->fFadeTime,
                                            pEventAlpha->fLifeTime,
                                            FALSE);
    }
    else if(pEventAlpha->eAlphaEventType == SEventAlpha::E_ALPHA_EVENT_ATOMIC)
    {
        CNtlPLCharacter::OnEventAlphaFade(pEventAlpha);
    }    
}

void CMTCharacter::SetAlpha( RwUInt8 byValue ) 
{
    RpToonInkSetOverallThickness(m_ToonData.pToonInk, byValue / 255.0f);
    
    CNtlPLCharacter::SetAlpha(byValue);

    // RunTime Alpha를 넣어준다.
    SetApplyRunTimeAlpha(TRUE);

    // RW's DXT1 Alpha Attr check
    SetDXT1AlphaCheck(byValue);
}

void CMTCharacter::SetWeightAlpha( RwReal fWeightValue ) 
{
    CNtlPLCharacter::SetWeightAlpha(fWeightValue);
 
    // Clump Alpha의 처리는 모델툴용으로 여기서 한다.
    SetAlpha((RwUInt8)(255.0f * fWeightValue));

    if(fWeightValue <= 0.0f)
    {
        m_LinkEffectInstance.SetVisible(FALSE);
    }
    else
    {
        m_LinkEffectInstance.SetVisible(TRUE);
    }
}

void CMTCharacter::OnEventPostEffect( SEventPostEffect* pEventPostEffect ) 
{
    CNtlInstanceEffect* pPLEntity = (CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, pEventPostEffect->szPostEffectName);

    if(!pPLEntity)
        return;

    // Center Fix Enable
    pPLEntity->SetPostEffect_CenterFixEnable(pEventPostEffect->bCenterFixEnable);

    if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_SELF)
    {
        if(strlen(pEventPostEffect->szPCBoneName))
        {
            SPLAttachAttr sAttachAttr;
            sAttachAttr.vOffsetPos = pEventPostEffect->v3dOffset;
            AttachBone(pPLEntity, pEventPostEffect->szPCBoneName, &sAttachAttr);
        }
        else
        {
            AttachWorldPos((CNtlPLAttach*)pPLEntity, pEventPostEffect->v3dOffset);
        }		
    }
    else if(pEventPostEffect->eTarget == POST_EFFECT_TARGET_TYPE_TARGET)
    {
        // 타겟으로 설정되어 있으면 타겟에 붙인다.
    }

}

void CMTCharacter::OnEventColorChange( SEventColorChange* pEventColorChange ) 
{
    if(pEventColorChange->eType == COLOR_CHANGE_TYPE_START)
    {
        SetInkColor(pEventColorChange->colorEdge.red,
                    pEventColorChange->colorEdge.green,
                    pEventColorChange->colorEdge.blue);

        SetColor(pEventColorChange->colorBody.red,
                 pEventColorChange->colorBody.green,
                 pEventColorChange->colorBody.blue);

        SetAddColor(pEventColorChange->colorAdd.red,
                    pEventColorChange->colorAdd.green,
                    pEventColorChange->colorAdd.blue);
    }
    else if(pEventColorChange->eType == COLOR_CHANGE_TYPE_END)
    {
        SetInkColor(0, 0, 0);
        SetColor(255, 255, 255);
        SetAddColor(0, 0, 0);
    }
    else if(pEventColorChange->eType == COLOR_CHANGE_TYPE_ANIM)
    {
        SetInkColor(pEventColorChange->colorEdge.red,
            pEventColorChange->colorEdge.green,
            pEventColorChange->colorEdge.blue);

        SetColor(pEventColorChange->colorBody.red,
            pEventColorChange->colorBody.green,
            pEventColorChange->colorBody.blue);

        SetAddColor(pEventColorChange->colorAdd.red,
            pEventColorChange->colorAdd.green,
            pEventColorChange->colorAdd.blue);        

        m_bRemoveColorChange = TRUE;
    }
}

void CMTCharacter::OnEventStretch( SEventStretch* pEventStretch ) 
{
    SetAnimUpdate(FALSE);        
    m_bStretchUpdate = TRUE;    
    m_pStretchEvent = pEventStretch;
    m_fStretchPullingSpeed = pEventStretch->fStretchSpeed;

    // 기존 Bone 정보 저장            
    RwUInt32 uiScaleBoneIndex = GetBoneIndex(m_pStretchEvent->szScaleBone);
    RwUInt32 uiBone1 = GetBoneIndex(m_pStretchEvent->szBoneName[0]);
    RwUInt32 uiBone2 = GetBoneIndex(m_pStretchEvent->szBoneName[1]);

    m_bOrgBoneScale = GetBoneData()->bBoneScale;
    GetBoneData()->bBoneScale = TRUE;
    m_fOrgBoneLength[0] = GetBoneData()->BoneScale[uiBone1].fLength;
    m_fOrgBoneLength[1] = GetBoneData()->BoneScale[uiBone2].fLength;
    m_fOrgBoneLength[2] = GetBoneData()->BoneScale[uiScaleBoneIndex].fLength;
    m_fOrgBoneWidth[0] = GetBoneData()->BoneScale[uiBone1].fWidth;
    m_fOrgBoneWidth[1] = GetBoneData()->BoneScale[uiBone2].fWidth;
    m_fOrgBoneWidth[2] = GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth;
}

void CMTCharacter::UpdateStretch(RwReal fElapsed)
{
    if(!m_bStretchUpdate)
        return;

    if(strlen(m_pStretchEvent->szScaleBone) == 0)
        return;

    RwV3d vTarget = GetDirection() * 24.0f;    
    vTarget.y = 1.0f;

    RwMatrix* pMatHand = GetBoneMatrix(m_pStretchEvent->szScaleBone);
    RwUInt32 uiScaleBoneIndex = GetBoneIndex(m_pStretchEvent->szScaleBone);
    RwUInt32 uiBone1 = GetBoneIndex(m_pStretchEvent->szBoneName[0]);
    RwUInt32 uiBone2 = GetBoneIndex(m_pStretchEvent->szBoneName[1]);
    RwV3d posHand = *RwMatrixGetPos(pMatHand);
    RwReal fBoneDist = CNtlMath::GetLength(ZeroAxis, posHand);
    RwReal fTargetDist = CNtlMath::GetLength(ZeroAxis, vTarget);    
    SetSBoneStretchTargetPos(vTarget, m_pStretchEvent->szAxisBone);

    if(!m_bStretchPulling && (fBoneDist >= fTargetDist || fBoneDist > 26.0f))
    {
        m_fStretchPullingSpeed = m_pStretchEvent->fStretchSpeed;
        m_bStretchPulling = TRUE;

        // 타겟 이펙트
        if(strlen(m_pStretchEvent->szTargetEffect) > 0)
        {
            CNtlInstanceEffect* pEffect =(CNtlInstanceEffect*)GetSceneManager()->CreateEntity(PLENTITY_EFFECT, m_pStretchEvent->szTargetEffect);
            AttachBone(pEffect, m_pStretchEvent->szScaleBone);            
        }        
    }

    m_fStretchPullingSpeed += m_fStretchPullingSpeed * m_pStretchEvent->fAccel * fElapsed;      // 가속도 
    if(m_bStretchPulling)              // 당겨오는 상태
    {
        GetBoneData()->BoneScale[uiBone1].fLength -= m_fStretchPullingSpeed * fElapsed;            
        GetBoneData()->BoneScale[uiBone2].fLength -= m_fStretchPullingSpeed * fElapsed;

        if(fBoneDist <= 2.0f || GetBoneData()->BoneScale[uiBone1].fLength <= 0.0f ||
           fBoneDist > 30.0f || GetBoneData()->BoneScale[uiBone2].fLength <= 0.0f)
        {
            GetBoneData()->bBoneScale = m_bOrgBoneScale;        
            GetBoneData()->BoneScale[uiBone1].fLength = m_fOrgBoneLength[0];
            GetBoneData()->BoneScale[uiBone1].fWidth = m_fOrgBoneWidth[0];
            GetBoneData()->BoneScale[uiBone2].fLength = m_fOrgBoneLength[1];
            GetBoneData()->BoneScale[uiBone2].fWidth = m_fOrgBoneWidth[1];
            GetBoneData()->BoneScale[uiScaleBoneIndex].fLength = m_fOrgBoneLength[2];
            GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth = m_fOrgBoneWidth[2];             
            m_bStretchUpdate = FALSE;
            m_bStretchPulling = FALSE;
            SetAnimUpdate(TRUE);

            SetSBoneStretchTargetPos(ZeroAxis, "");
        }           
    }
    else    // 뻗어나가는 상태                        
    {
        GetBoneData()->BoneScale[uiBone1].fLength += m_fStretchPullingSpeed * fElapsed;
        GetBoneData()->BoneScale[uiBone1].fWidth = m_pStretchEvent->fWidth;
        GetBoneData()->BoneScale[uiBone2].fLength += m_fStretchPullingSpeed * fElapsed;
        GetBoneData()->BoneScale[uiBone2].fWidth = m_pStretchEvent->fWidth;
        GetBoneData()->BoneScale[uiScaleBoneIndex].fLength = m_pStretchEvent->fScaleSize;
        GetBoneData()->BoneScale[uiScaleBoneIndex].fWidth = m_pStretchEvent->fScaleSize;        
    } 
}

void CMTCharacter::SetEnvMap(RwChar* _pName)
{
	if(_pName)
	{
		if(m_pSEMTex)
		{
			if(m_pSEMTex->refCount > 1)
				RwTextureDestroy(m_pSEMTex);
			else
			{
				RwTextureDestroy(m_pSEMTex);
				m_pSEMTex = NULL;
			}
		}

		m_pSEMTex = GetNtlResourceManager()->LoadTexture(_pName, ".\\Texture\\EnvMap\\");
	}
	else
	{
		if(m_pSEMTex)
		{
			if(m_pSEMTex->refCount > 1)
				RwTextureDestroy(m_pSEMTex);
			else
			{
				RwTextureDestroy(m_pSEMTex);
				m_pSEMTex = NULL;
			}
		}
	}
}

void CMTCharacter::UpdateCameraShake( RwReal fElapsed) 
{   
    if(m_fShakeFactor <= 0.0f)  
    {
        m_fShakeFactor = m_fShakeMaxHeight = m_fShakeElapsedTime = 0.0f;
        CModelToolApplication::GetInstance()->GetCamera()->SetCameraOffset(ZeroAxis);
        CModelToolApplication::GetInstance()->GetCamera()->SetCameraLookAtOffset(ZeroAxis);
        return;
    }

    RwReal fDeltaShake = cos(m_fShakeElapsedTime * 110.f)*(exp(-m_fShakeElapsedTime * 110.f/m_fShakeFactor))*m_fShakeMaxHeight;
    m_fShakeElapsedTime += fElapsed;

    RwV3d vOffset = ZeroAxis;
    vOffset.y = fDeltaShake;    

    CModelToolApplication::GetInstance()->GetCamera()->SetCameraOffset(vOffset);
    CModelToolApplication::GetInstance()->GetCamera()->SetCameraLookAtOffset(vOffset);    

    if(fabs(fDeltaShake) < NTL_EPSILON)
    {
        m_fShakeFactor = m_fShakeMaxHeight = m_fShakeElapsedTime = 0.0f;
        CModelToolApplication::GetInstance()->GetCamera()->SetCameraOffset(ZeroAxis);
        CModelToolApplication::GetInstance()->GetCamera()->SetCameraLookAtOffset(ZeroAxis);
    }
}