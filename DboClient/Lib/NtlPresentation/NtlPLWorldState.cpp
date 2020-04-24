#include "precomp_ntlpresentation.h"
#include "NtlPLWorldState.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"

RwUInt32		CNtlPLWorldState::eActiveWorldType			= AW_NONE;
CNtlPLObject*	CNtlPLWorldState::pActiveMiniIndoor			= NULL;
CNtlPLObject*	CNtlPLWorldState::pActiveMiniIndoorNew		= NULL;

void CNtlPLWorldState::Init(void)
{
	eActiveWorldType		= AW_NONE;

	pActiveMiniIndoor		= NULL;
	pActiveMiniIndoorNew	= NULL;
}

void CNtlPLWorldState::Terminate(void)
{
	Init();
}

void CNtlPLWorldState::Update(RwV3d& vPos)
{
	if (!GetSceneManager()->IsWorldReady())
	{
		return;
	}

	UpdateSpace(vPos);
}

void CNtlPLWorldState::InitSpace()
{
	pActiveMiniIndoor		= NULL;
	pActiveMiniIndoorNew	= NULL;
}

struct SGetWorldSpace
{
	CNtlPLObject*	pTestObject;

	RwReal			fDistMiniIndoor;
	CNtlPLObject*	pNtlPLObjectMiniIndoor;
	RwV3d			vCollisionMiniIndoor;
};

RpCollisionTriangle* GetWorldSpace(RpIntersection* pIntersection, RpCollisionTriangle* pCollTriangle, RwReal fDistance, void* pData)
{
	SGetWorldSpace*	pGetWorldSpace = (SGetWorldSpace*)pData;

	if (pGetWorldSpace->fDistMiniIndoor > fDistance)
	{
		pGetWorldSpace->pNtlPLObjectMiniIndoor	= pGetWorldSpace->pTestObject;
		pGetWorldSpace->fDistMiniIndoor			= fDistance;
		pGetWorldSpace->vCollisionMiniIndoor	= pIntersection->t.line.start + ((pIntersection->t.line.end - pIntersection->t.line.start) * fDistance);
	}

	return pCollTriangle;
}

RpAtomic* GetWorldSpace(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData)
{
	CNtlPLEntity *pPLEntity = reinterpret_cast<CNtlPLEntity*>(RpNtlAtomicGetData(pAtomic));
    if(!pPLEntity)
        return NULL;

	if (pPLEntity->GetClassType() != PLENTITY_OBJECT)
	{
		return pAtomic;
	}

	CNtlPLObject*	pPLObject		= (CNtlPLObject*)pPLEntity;
	SGetWorldSpace*	pGetWorldSpace	= (SGetWorldSpace*)pData;

	if (pPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN ||
		pPLObject->GetObjectType() == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE)
	{
		RpIntersection intersection;
		intersection.type			= rpINTERSECTLINE;
		intersection.t.line.start	= pIntersection->t.point;
		intersection.t.line.start.y	= pIntersection->t.point.y + 0.1f;
		intersection.t.line.end		= pIntersection->t.point;
		intersection.t.line.end.y	= pIntersection->t.point.y - 512.0f;

		pGetWorldSpace->pTestObject = pPLObject;
		RpAtomicForAllIntersections(pAtomic, &intersection, GetWorldSpace, pData);
	}

	return pAtomic;	
}

RwBool GetNtlWorldSpace(RwV3d* pvPos, CNtlPLEntity** ppPLEntity, RwReal* pfDist)
{
	if (!GetSceneManager()->IsWorldReady())
	{
		return FALSE;
	}

 	RpIntersection intersection;
//  	intersection.type		= rpINTERSECTPOINT;
//  	intersection.t.point	= *pvPos;

	intersection.type				= rpINTERSECTSPHERE;
	intersection.t.sphere.center	= *pvPos;
	intersection.t.sphere.radius	= 2.0f;

	SGetWorldSpace sGetWorldSpace;
	sGetWorldSpace.fDistMiniIndoor			= RwRealMAXVAL;
	sGetWorldSpace.pNtlPLObjectMiniIndoor	= NULL;

	RpWorldForAllAtomicIntersections(CNtlPLGlobal::m_pRpWorld, &intersection, GetWorldSpace, &sGetWorldSpace);

	if (sGetWorldSpace.pNtlPLObjectMiniIndoor)
	{		
		RwReal fTerrainHeight = -999.0f;
		if ((!(GetSceneManager()->GetWorldAttribute(*pvPos) & dNMAP_TRANSPARENCY_TILE_FLAG)) && GetSceneManager()->GetTerrainHeight(pvPos, fTerrainHeight))
		{
			if (fTerrainHeight < pvPos->y && fTerrainHeight > sGetWorldSpace.vCollisionMiniIndoor.y)
			{
				return FALSE;
			}			
		}

		if (ppPLEntity)
		{
			*ppPLEntity = static_cast<CNtlPLEntity*>(sGetWorldSpace.pNtlPLObjectMiniIndoor);
		}
		if (pfDist)
		{
			*pfDist		= sGetWorldSpace.fDistMiniIndoor;
		}
		return TRUE;
	}
	return FALSE;
}

void CNtlPLWorldState::UpdateSpace(RwV3d& vPos)
{
	RwReal fDist = RwRealMAXVAL;

	CNtlPLEntity* pPLEntity = NULL;
	if (GetNtlWorldSpace(&vPos, &pPLEntity, &fDist))
	{
		pActiveMiniIndoorNew = static_cast<CNtlPLObject*>(pPLEntity);
	}
	else
	{
		pActiveMiniIndoorNew = NULL;
	}	
	
	if (pActiveMiniIndoor != pActiveMiniIndoorNew)
	{
		CNtlWorldFieldManager*	pNtlWorldFieldManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
		RwInt32					iSectorIdx				= pNtlWorldFieldManager->GetSectorIdx(vPos);
		std::vector<RwInt32>	vecSectorIdx;
		if (iSectorIdx != -1)
		{		
			pNtlWorldFieldManager->GetNeighborSectors(iSectorIdx, vecSectorIdx);
			vecSectorIdx.push_back(iSectorIdx);
		}

		// Occluder Portal Trigger.
		for (int j = 0; j < (int)vecSectorIdx.size(); ++j)
		{				
			if (vecSectorIdx.at(j) == -1)
			{
				continue;
			}

			CNtlWorldSector* pSectors = pNtlWorldFieldManager->m_pSectors;

			for (int i = 0; i < (int)pSectors[vecSectorIdx.at(j)].m_vecNtlPLEntityOccluder_Quad.size(); ++i)
			{
				CNtlPLOccluder_Quad* pOccluder = (CNtlPLOccluder_Quad*)pSectors[vecSectorIdx.at(j)].m_vecNtlPLEntityOccluder_Quad.at(i);
				pOccluder->RunableTrigger((pActiveMiniIndoorNew ? pActiveMiniIndoorNew->GetName() : NULL));
			}
		}

		if (pActiveMiniIndoorNew)
		{
			if (pActiveMiniIndoor)
			{
				// 郴何 -> 郴何
			}
			else
			{
				// 寇何 -> 郴何
			}
		}
		else
		{
			// 郴何 -> 寇何
			pNtlWorldFieldManager->SetAnotherField();
		}

		pActiveMiniIndoor = pActiveMiniIndoorNew;
	}
}

RwBool CheckEventsWorldEffectChanged(RWS::CMsg& Msg)
{
	if (Msg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED || Msg.Id == NPEI_IS_ANOTHER_BLOCK_CHANGED)
	{
		if (CNtlPLWorldState::GetActiveMiniIndoor())
		{
			if (CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr() &&
				CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY)
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	else if (Msg.Id == NPEI_IS_ANOTHER_OBJECT_CHANGED)
	{
		if (!CNtlPLWorldState::GetActiveMiniIndoor())
		{
			return FALSE;
		}
		
		if (!CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr() ||
			!(CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY))
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

RwBool CheckEventsWorldEffectChangedWithOutDelay(RWS::CMsg& Msg)
{
	if (Msg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY || Msg.Id == NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY)
	{
		if (CNtlPLWorldState::GetActiveMiniIndoor())
		{
			if (CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr() &&
				CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY)
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	else if (Msg.Id == NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY)
	{
		if (!CNtlPLWorldState::GetActiveMiniIndoor())
		{
			return FALSE;
		}

		if (!CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr() ||
			!(CNtlPLWorldState::GetActiveMiniIndoor()->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY))
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

CNtlPLObject* CNtlPLWorldState::GetActiveMiniIndoor()
{
#ifdef dNTL_WORLD_TOOL_MODE
	if (CNtlPLGlobal::m_IsMinimapRendering)
	{
		return NULL;
	}
#endif
	return pActiveMiniIndoor;
}