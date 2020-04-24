//***********************************************************************************
//	File		:	NtlEffectSystemFreeList.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "precomp_ntlpresentation.h"
#include "NtlEffectAPI.h"

//------------------------------------------------------------------
//	FuncName	: SetAnimation
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
static RwFrame* GetChildFrameHierarchy(RwFrame *frame, void *data)
{
    RpHAnimHierarchy *hierarchy = *(RpHAnimHierarchy **)data;
    hierarchy = RpHAnimFrameGetHierarchy(frame);
    if( hierarchy == NULL )
    {
        RwFrameForAllChildren(frame, GetChildFrameHierarchy, data);
        return frame;
    }
    *(void **)data = (void *)hierarchy;
    return NULL;
}

RpHAnimHierarchy* GetHierarchy(RpClump *clump)
{
	RpHAnimHierarchy *hierarchy = NULL;
	RwFrameForAllChildren(RpClumpGetFrame(clump), GetChildFrameHierarchy, (void *)&hierarchy);
	return hierarchy;
}


static RpAtomic* SetHierarchyForSkinAtomic(RpAtomic *atomic, void *data)
{
	RpSkinAtomicSetHAnimHierarchy(atomic, (RpHAnimHierarchy *)data);
	return atomic;
}

RpAtomic* API_GetSkinHierarchy(RpAtomic *atomic, void *data)
{
	*(void **)data = (void *)RpSkinAtomicGetHAnimHierarchy(atomic);
	return NULL;
}

//------------------------------------------------------------------
//	FuncName	: API_SetAnimation();
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void API_SetAnimation(RpClump* pClump, RtAnimAnimation* pAnimation)
{
	// Hierarchy
    RpHAnimHierarchy *hierarchy = GetHierarchy(pClump);
    RpClumpForAllAtomics(pClump, SetHierarchyForSkinAtomic, (void *)hierarchy);
	RpHAnimHierarchySetCurrentAnim(hierarchy, pAnimation);
}


//------------------------------------------------------------------
//	FuncName	: API_MeshColorUpdate();
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
static RpMaterial* EffectGeometryColorUpdate(RpMaterial *pMaterial, void *data)
{
	RpMaterialSetColor(pMaterial,(RwRGBA*)data);  
	return pMaterial;
}

RpAtomic* API_MeshColorUpdate(RpAtomic *pAtomic, void *data __RWUNUSED__)
{
    RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);
    if(pGeometry)
    {
		RwUInt32 uFlags = RpGeometryGetFlags(pGeometry); 
		uFlags |= rpGEOMETRYMODULATEMATERIALCOLOR;
		RpGeometrySetFlags(pGeometry, uFlags);
		RpGeometryForAllMaterials(pGeometry, EffectGeometryColorUpdate, data);
    }

    return pAtomic;
}

struct MaterialCBAndData
{
    RpMaterialCallBack cb;
    void *data;
};

RpAtomic* AtomicForAllMaterials(RpAtomic *atomic, void *data)
{
    MaterialCBAndData *cbAndData = (MaterialCBAndData *)data;

    RpGeometryForAllMaterials(
        RpAtomicGetGeometry(atomic), cbAndData->cb, cbAndData->data);

    return atomic;
}

RpClump* ClumpForAllMaterials(RpClump *clump, void *data)
{
    RpClumpForAllAtomics(clump, AtomicForAllMaterials, data);

    return clump;
}

RpMaterial* AddAnimatedMaterialToList(RpMaterial *material, void *data)
{
    RwSList *list = (RwSList *)data;
    if (RpMaterialUVAnimExists(material))
    {
        RpMaterial **current=(RpMaterial**)rwSListGetBegin(list);

        /* No duplicates */
        while (current != rwSListGetEnd(list))
        {
            if (material==*current)
            {
                /* Don't add, and that's OK */
                return material;
            }
            ++current;
        }

        /* Add it */
        *(RpMaterial**)rwSListGetNewEntry(list, rwID_NAOBJECT)=material;
    }

    

    return material;
}
//------------------------------------------------------------------
//	FuncName	: API_AddAnimatedMaterialsList();
//	Desc		: UVAnim이 적용된 Material을 찾아서 리스트에 담는다.
//	Parameter	: 
//	Return		: UVAnim이 적용도니 Material들의 List
//------------------------------------------------------------------
RwSList* API_AddAnimatedMaterialsList(RwSList* pList, RpClump* pClump)
{
	MaterialCBAndData cbAndData = { AddAnimatedMaterialToList, NULL };
	cbAndData.data = pList;

	ClumpForAllMaterials(pClump, &cbAndData);

	return pList;
}

//------------------------------------------------------------------
//	FuncName	: API_MaterialsInterpolatorsAddAnimTime();
//	Desc		: UVAnim을 업데이트한다. (시간을 더한다)
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void API_MaterialsInterpolatorsAddAnimTime(RwSList *animatedMaterials, RwReal deltaTime)
{
	RpMaterial **material = (RpMaterial **)rwSListGetBegin(animatedMaterials);
	while (material!=rwSListGetEnd(animatedMaterials))
	{
		RpMaterialUVAnimAddAnimTime(*material, deltaTime);
		++material;
	}
}

/**
 * UVAnim을 업데이트 한다. (현재 시간을 적용한다)
 * \param animatedMaterials 업데이트할 Material List
 * \param fTime 적용한 Time 
 */
void API_MaterialsInterpolatorsSetCurrentTime(RwSList* animatedMaterials, RwReal fTime)
{
    RpMaterial** material = (RpMaterial**)rwSListGetBegin(animatedMaterials);
    while(material != rwSListGetEnd(animatedMaterials))
    {
        RpMaterialUVAnimSetCurrentTime(*material, fTime);
        ++material;
    }        
}


//------------------------------------------------------------------
//	FuncName	: API_MaterialsAnimApply();
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void API_MaterialsAnimApply(RwSList *animatedMaterials)
{
	RpMaterial **material = (RpMaterial **)rwSListGetBegin(animatedMaterials);
	while (material!=rwSListGetEnd(animatedMaterials))
	{
		RpMaterialUVAnimApplyUpdate(*material);
		++material;
	}
}
