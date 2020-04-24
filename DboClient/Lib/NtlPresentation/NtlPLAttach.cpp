#include "precomp_ntlpresentation.h"
#include "NtlPLAttach.h"
#include "NtlDebug.h"
#include "NtlCoreUtil.h"
#include "NtlPLSceneManager.h"
#include "NtlMath.h"

RwFreeList* CNtlPLAttach::m_pAttachAttrFreeList = NULL;

RwBool CNtlPLAttach::AllocFreeList(void)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Create");

	NTL_PRE(m_pAttachAttrFreeList == 0);

	RwUInt32 iBlockSize = 50;
    m_pAttachAttrFreeList = RwFreeListCreate(sizeof(SPLAttachAttr), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pAttachAttrFreeList, "attach entity failed to allocate free list");

    NTL_RETURN(TRUE);
}

void CNtlPLAttach::DeAllocFreeList(void)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Destroy");

 
	if(m_pAttachAttrFreeList)
	{
		RwFreeListDestroy(m_pAttachAttrFreeList);
		m_pAttachAttrFreeList = 0;
	}

    NTL_RETURNVOID();
}

CNtlPLAttach::CNtlPLAttach()
{
	m_pAttachParent = NULL;
	SetFlags(NTL_PLEFLAG_ATTACH);
}

CNtlPLAttach::~CNtlPLAttach()
{
	AttachDestroy();
}

RwBool CNtlPLAttach::Find(const CNtlPLAttach *pPLAttachEntity)
{
	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr->pPLEntity == pPLAttachEntity)
			return TRUE;
	}

	return FALSE;
}

void CNtlPLAttach::AttachDestroy(void)
{
	if(m_pAttachParent)
	{
		m_pAttachParent->Detach(this); 
		m_pAttachParent = NULL;
	}

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
        if(pAttachAttr->pPLEntity)
        {
		    pAttachAttr->pPLEntity->SetAttachParent(NULL);
        }
		RwFreeListFree(m_pAttachAttrFreeList, pAttachAttr);
	}

	m_listAttachAttr.clear(); 
}

RwBool CNtlPLAttach::Update(RwReal fElapsed)
{
	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr->eType == PL_ATTACH_BONE)
		{
            if(GetBoneMatrix(pAttachAttr->chBoneName))
            {
                RwMatrix* pMatBone = RwMatrixCreate();

                RwMatrixCopy(pMatBone, GetBoneMatrix(pAttachAttr->chBoneName));			    
                RwMatrixScale(pMatBone, &pAttachAttr->vScale, rwCOMBINEPRECONCAT);
                RwMatrixRotate(pMatBone, &CNtlPLGlobal::m_vXAxisV3,  pAttachAttr->vRot.x, rwCOMBINEPRECONCAT);
                RwMatrixTranslate(pMatBone, &pAttachAttr->vOffsetPos, rwCOMBINEPRECONCAT);
                pAttachAttr->pPLEntity->SetMatrix(*pMatBone);

                RwMatrixDestroy(pMatBone);
            }
		}
		else if(pAttachAttr->eType == PL_ATTACH_BONE_OFFSET_POS_ROOT_ROTATE_Y)
		{
			RwMatrix *pMatBone = GetBoneMatrix(pAttachAttr->chBoneName);
			RwFrame	*pFrame    = GetFrame();

			if(pMatBone)
			{
				RwMatrix *pMat1 = RwMatrixCreate();
				RwMatrixCopy(pMat1, &pFrame->ltm);
				RwV3d *pPos = RwMatrixGetPos(pMat1);

				pPos->x = pMatBone->pos.x + pAttachAttr->vOffsetPos.x;
				pPos->y = pMatBone->pos.y + pAttachAttr->vOffsetPos.y;
				pPos->z = pMatBone->pos.z + pAttachAttr->vOffsetPos.z;
				pAttachAttr->pPLEntity->SetMatrix(*pMat1);                
				
				RwMatrixDestroy(pMat1);
			}
		}
		else if(pAttachAttr->eType == PL_ATTACH_WORLD_POS)
		{
			RwV3d vPos = GetPosition() + pAttachAttr->vOffsetPos;			
			pAttachAttr->pPLEntity->SetPosition(&vPos);  
		}
		else if(pAttachAttr->eType == PL_ATTACH_WORLD_POSANDROTATE)
		{
			RwV3d vResultOffset = ZeroAxis;

			RwMatrix mat = GetMatrix();					

			RwV3d vPos = mat.pos;
			mat.pos = ZeroAxis;
			
			RwV3dTransformPoint(&vResultOffset, &pAttachAttr->vOffsetPos, &mat);			
			mat.pos = vPos + vResultOffset;
			RwMatrixUpdate(&mat);

			pAttachAttr->pPLEntity->SetMatrix(mat);
		}
        else if(pAttachAttr->eType == PL_ATTACH_WORLD_POS_HEIGHT)
        {
            RwV3d vPos = GetPosition();
            RwReal fWorldHeight = 0.0f;
            GetSceneManager()->GetWorldHeight(&vPos, fWorldHeight, NULL);
            vPos.y = fWorldHeight;
            pAttachAttr->pPLEntity->SetPosition(&vPos);              
        }
	}

	CNtlPLEntity::Update(fElapsed);

	return TRUE;
}

RwBool CNtlPLAttach::RenderToTexture(void)
{
	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		pAttachAttr->pPLEntity->RenderToTexture();  
	}

	return CNtlPLEntity::RenderToTexture();
}

RwBool CNtlPLAttach::AttachBone(CNtlPLAttach *pPLAttachEntity, const RwChar *pBoneName, SPLAttachAttr* _pSPLAttachAttr /*= NULL*/)
{
	if(Find(pPLAttachEntity))
		return FALSE;

	if(pPLAttachEntity->GetAttachParent())
	{
		DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
		DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
		return FALSE;
	}

	RwMatrix *pMatBone = GetBoneMatrix(pBoneName);
	if(pMatBone == NULL)
	{
		DBO_TRACE(GetName(), "presentation object not exist bone");
		DBO_TRACE(!GetName(), GetName() << "- presentation object not exist bone");
		return FALSE;
	}

	SPLAttachAttr *pAttachAttr	= reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
	pAttachAttr->pPLEntity		= pPLAttachEntity;
	pAttachAttr->eType			= PL_ATTACH_BONE;

	if(_pSPLAttachAttr)
	{
		pAttachAttr->vOffsetPos	= _pSPLAttachAttr->vOffsetPos;
		pAttachAttr->vScale		= _pSPLAttachAttr->vScale;
		pAttachAttr->vRot		= _pSPLAttachAttr->vRot;
	}
	else
	{
		pAttachAttr->vOffsetPos.x	= 0.0f;
		pAttachAttr->vOffsetPos.y	= 0.0f;
		pAttachAttr->vOffsetPos.z	= 0.0f;
		pAttachAttr->vScale.x		= 1.0f;
		pAttachAttr->vScale.y		= 1.0f;
		pAttachAttr->vScale.z		= 1.0f;
		pAttachAttr->vRot.x			= 0.0f;
		pAttachAttr->vRot.y			= 0.0f;
		pAttachAttr->vRot.z			= 0.0f;
	}

	DBO_ASSERT(strlen(pBoneName) < MAX_ATTR_BONE_NAME, pPLAttachEntity->GetName() << "(" << pBoneName << ") - AttachBone Name length MAX_ATTR_BONE_NAME over");
	
	strcpy_s(pAttachAttr->chBoneName, MAX_ATTR_BONE_NAME, pBoneName);

	pPLAttachEntity->SetAttachParent(this);

	m_listAttachAttr.push_back(pAttachAttr); 
	
    // Items and effects apply immediately.
    if(pPLAttachEntity->GetClassType() == PLENTITY_ITEM ||
       pPLAttachEntity->GetClassType() == PLENTITY_EFFECT ||
	   pPLAttachEntity->GetClassType() == PLENTITY_LIGHT_OBJECT)
    {
        CNtlPLAttach::Update(0.0f);       
    }
    
	return TRUE;
}

/*
RwBool CNtlPLAttach::AttachBoneOffset( CNtlPLAttach* pPLAttachEntity, const RwChar* pBoneName, RwV3d vOffset ) 
{
    if(AttachBone(pPLAttachEntity, pBoneName))
    {
        SPLAttachAttr* pAttachAttr = *(m_listAttachAttr.rbegin());
        if(pAttachAttr)
        {
            pAttachAttr->vOffsetPos = vOffset;
        }

        return TRUE;
    }
    
    return FALSE;
}
*/

RwBool CNtlPLAttach::AttachBoneOffsetPosRootRotateY(CNtlPLAttach *pPLAttachEntity, const RwChar* pBoneName, RwV3d vOffset)
{
	if(Find(pPLAttachEntity))
		return FALSE;

	if(pPLAttachEntity->GetAttachParent())
	{
		DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
		DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
		return FALSE;
	}

	SPLAttachAttr *pAttachAttr = reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
	pAttachAttr->pPLEntity = pPLAttachEntity;
	pAttachAttr->eType = PL_ATTACH_BONE_OFFSET_POS_ROOT_ROTATE_Y;
	pAttachAttr->vOffsetPos = vOffset;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
    strcpy_s(pAttachAttr->chBoneName, MAX_ATTR_BONE_NAME, pBoneName);
#else
    strcpy(pAttachAttr->chBoneName, pBoneName);
#endif

	pPLAttachEntity->SetAttachParent(this);

	m_listAttachAttr.push_back(pAttachAttr); 

    if(pPLAttachEntity->GetClassType() == PLENTITY_EFFECT)
    {
        CNtlPLAttach::Update(0.0f);       
    }

	return TRUE;
}

RwBool CNtlPLAttach::AttachWorldPos(CNtlPLAttach *pPLAttachEntity, RwV3d vOffset)
{
	if(Find(pPLAttachEntity))
		return FALSE;

	if(pPLAttachEntity->GetAttachParent())
	{
		DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
		DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
		return FALSE;
	}

	SPLAttachAttr *pAttachAttr = reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
	pAttachAttr->pPLEntity = pPLAttachEntity;
	pAttachAttr->eType = PL_ATTACH_WORLD_POS;
	pAttachAttr->vOffsetPos = vOffset;

	pPLAttachEntity->SetAttachParent(this);

	m_listAttachAttr.push_back(pAttachAttr); 

    if(pPLAttachEntity->GetClassType() == PLENTITY_EFFECT)
    {
        CNtlPLAttach::Update(0.0f);       
    }

	return TRUE;
}

RwBool CNtlPLAttach::AttachWorldPosAndRotate(CNtlPLAttach *pPLAttachEntity, RwV3d vOffset)
{
	if(Find(pPLAttachEntity))
		return FALSE;

	if(pPLAttachEntity->GetAttachParent())
	{
		DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
		DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
		return FALSE;
	}

	SPLAttachAttr *pAttachAttr = reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
	pAttachAttr->pPLEntity = pPLAttachEntity;
	pAttachAttr->eType = PL_ATTACH_WORLD_POSANDROTATE;
	pAttachAttr->vOffsetPos = vOffset;

	pPLAttachEntity->SetAttachParent(this);

	m_listAttachAttr.push_back(pAttachAttr); 

    if(pPLAttachEntity->GetClassType() == PLENTITY_EFFECT)
    {
        CNtlPLAttach::Update(0.0f);       
    }

	return TRUE;
}

RwBool CNtlPLAttach::AttachWorldPosHeight( CNtlPLAttach* pPLAttachEntity ) 
{
    if(Find(pPLAttachEntity))
        return FALSE;

    if(pPLAttachEntity->GetAttachParent())
    {
        DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
        DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
        return FALSE;
    }

    SPLAttachAttr *pAttachAttr = reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
    pAttachAttr->pPLEntity = pPLAttachEntity;
    pAttachAttr->eType = PL_ATTACH_WORLD_POS_HEIGHT;    

    pPLAttachEntity->SetAttachParent(this);

    m_listAttachAttr.push_back(pAttachAttr); 

    if(pPLAttachEntity->GetClassType() == PLENTITY_EFFECT)
    {
        CNtlPLAttach::Update(0.0f);       
    }

    return TRUE;
}

RwBool CNtlPLAttach::Attach( CNtlPLAttach* pPLAttachEntity ) 
{
    if(!pPLAttachEntity || Find(pPLAttachEntity))
        return FALSE;

    if(pPLAttachEntity->GetAttachParent())
    {
		DBO_TRACE(pPLAttachEntity->GetName(), "already attached other presentation object");
		DBO_TRACE(!pPLAttachEntity->GetName(), pPLAttachEntity->GetName() << "- already attached other presentation object");
        return FALSE;
    }

    SPLAttachAttr *pAttachAttr = reinterpret_cast<SPLAttachAttr*>(RwFreeListAlloc(m_pAttachAttrFreeList, rwMEMHINTDUR_GLOBAL));
    ZeroMemory(pAttachAttr, sizeof(SPLAttachAttr));

    pAttachAttr->pPLEntity = pPLAttachEntity;
    pAttachAttr->eType = PL_ATTACH_NONE;

    pPLAttachEntity->SetAttachParent(this);

    m_listAttachAttr.push_back(pAttachAttr);

    return TRUE;
}

void CNtlPLAttach::Detach(CNtlPLAttach *pPLAttachEntity)
{
	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr->pPLEntity == pPLAttachEntity)
		{
			RwFreeListFree(m_pAttachAttrFreeList, pAttachAttr);
			m_listAttachAttr.erase(it);
			pPLAttachEntity->SetAttachParent(NULL);
			return;
		}
	}
}

void CNtlPLAttach::SetAttachParent(CNtlPLAttach *pPLAttachEntity)
{
	m_pAttachParent = pPLAttachEntity;

    // If the parent is hidden, the child is also hidden
    if(pPLAttachEntity)
    {
       SetVisible(pPLAttachEntity->IsVisible(0)); // < 0 so effects like quest icon work, when npc is not in camera
    }
}

CNtlPLAttach* CNtlPLAttach::GetAttachParent(void) const
{
	return m_pAttachParent;
}

void CNtlPLAttach::CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue)
{
	CNtlPLEntity::CreateWeightElapsedController(fLifeTime, fWeightValue);

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		pAttachAttr->pPLEntity->CreateWeightElapsedController(fLifeTime, fWeightValue);  
	}
}

void CNtlPLAttach::DeleteWeightElapsedController(void)
{
	CNtlPLEntity::DeleteWeightElapsedController();

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		pAttachAttr->pPLEntity->DeleteWeightElapsedController();  
	}
}

void CNtlPLAttach::SetVisible(RwBool bVisible)
{
	CNtlPLEntity::SetVisible(bVisible);

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr && pAttachAttr->pPLEntity)
		{
			pAttachAttr->pPLEntity->SetVisible(bVisible);  
		}
		
	}
}

void CNtlPLAttach::SetCullFlags(RwUInt32 uiFlag, RwBool bCulled)
{
	CNtlPLEntity::SetCullFlags(uiFlag, bCulled);

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr && pAttachAttr->pPLEntity)
		{
			pAttachAttr->pPLEntity->SetCullFlags(uiFlag, bCulled);
		}

	}
}

void CNtlPLAttach::SetCullFlags(RwUInt32 uiFlag)
{
	CNtlPLEntity::SetCullFlags(uiFlag);

	ListAttachAttr::iterator it;
	for(it = m_listAttachAttr.begin(); it != m_listAttachAttr.end(); ++it)
	{
		SPLAttachAttr *pAttachAttr = (*it);
		if(pAttachAttr && pAttachAttr->pPLEntity)
		{
			pAttachAttr->pPLEntity->SetCullFlags(uiFlag);
		}

	}
}
