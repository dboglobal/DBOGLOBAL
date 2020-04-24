#include "precomp_ntlpresentation.h"
#include "NtlPLAtomicSorter.h"
#include "NtlDebug.h"
#include "NtlAtomic.h"
#include "NtlPLDef.h"
#include "NtlPLRenderState.h"
#include "ntlworldplugin.h"
#include "NtlPLSceneManager.h"
#include "NtlPLObject.h"
#include "NtlProfiler.h"


void SortedAtomicRender( RpAtomic *pAtomic )
{
	NTL_SPROFILE("SortedAtomicRender")

	RwUInt32 uiRenderFlags = RpNtlAtomicGetFlag( pAtomic );
	CNtlPLEntity* pEntity = (CNtlPLEntity*)RpNtlAtomicGetData(pAtomic);
	
	if ( uiRenderFlags & NTL_ALPHATEST )	BeginNtlAotmicAlphaTestState();
	if ( uiRenderFlags & NTL_TWOSIDE )		BeginNtlAtomicTwoSideState();

	if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(TRUE);

	pAtomic->renderCallBack( pAtomic );

	if ( uiRenderFlags & NTL_ALPHA)		UpdateAlphaState(FALSE);

	if ( uiRenderFlags & NTL_TWOSIDE )		EndNtlAtomicTwoSideState();
	if ( uiRenderFlags & NTL_ALPHATEST )	EndNtlAtomicAlphaTestState();

	NTL_EPROFILE()
}

int DepthSortFunc(const void *a, const void *b)
{
	const CNtlPLAtomicSorter::SSortData **ptrA = (const CNtlPLAtomicSorter::SSortData**)(a);
	const CNtlPLAtomicSorter::SSortData **ptrB = (const CNtlPLAtomicSorter::SSortData**)(b);
	
	RwReal fDepthA = (*ptrA)->fDepth;
	RwReal fDepthB = (*ptrB)->fDepth;
	
	if (fDepthA > fDepthB) return -1;
	else if (fDepthA == fDepthB) return 0;
	else return 1;
}

CNtlPLAtomicSorter::CNtlPLAtomicSorter()
{
	m_uiNumItems	= 0;
	m_pSortData		= NULL;
	m_pSortList		= NULL;
}

CNtlPLAtomicSorter::~CNtlPLAtomicSorter()
{
}

void CNtlPLAtomicSorter::Create(RwUInt32 uiSizeSortList)
{
	if(m_pSortList == NULL)
	{
		m_uiGrowSize = uiSizeSortList;
		m_uiSizeSortList = uiSizeSortList;
		m_pSortData = reinterpret_cast<SSortData*>(RwMalloc(sizeof(SSortData) * m_uiSizeSortList, rwMEMHINTDUR_GLOBAL));
		m_pSortList = reinterpret_cast<SSortData**>(RwMalloc(sizeof(SSortData*) * m_uiSizeSortList, rwMEMHINTDUR_GLOBAL));
	}
}

void CNtlPLAtomicSorter::Destroy(void)
{
	if(m_pSortData)
	{
		RwFree(m_pSortData);
	}

	if(m_pSortList)
	{
		RwFree(m_pSortList);
	}
}

void CNtlPLAtomicSorter::Reset(void)
{
	m_uiNumItems = 0;
}

void CNtlPLAtomicSorter::PreSort(void)
{
	if (m_uiNumItems >= m_uiSizeSortList)
	{
		m_uiSizeSortList += m_uiGrowSize;

		SSortData *pTempSortData = reinterpret_cast<SSortData*>(RwMalloc(sizeof(SSortData) * m_uiSizeSortList, rwMEMHINTDUR_GLOBAL));
		memcpy(pTempSortData, m_pSortData, sizeof(SSortData)*m_uiNumItems);

		RwFree(m_pSortData);
		RwFree(m_pSortList);

		m_pSortData = pTempSortData;

		m_pSortList = reinterpret_cast<SSortData**>(RwMalloc(sizeof(SSortData*) * m_uiSizeSortList, rwMEMHINTDUR_GLOBAL));

		for(RwInt32 i = 0; i < (RwInt32)m_uiNumItems; i++)
			m_pSortList[i] = &m_pSortData[i];
	}
}

void CNtlPLAtomicSorter::DepthSort(void)
{
	qsort((void*)m_pSortList, m_uiNumItems, sizeof(SSortData**), DepthSortFunc);
}

void CNtlPLAtomicSorter::Push(EPlSortType eType, void *pRenderEntity, RwReal fDepth)
{
	PreSort();

	m_pSortList[m_uiNumItems]					= &m_pSortData[m_uiNumItems];
	m_pSortList[m_uiNumItems]->eSortType		= eType;
	m_pSortList[m_uiNumItems]->pRenderEntity	= pRenderEntity;
	m_pSortList[m_uiNumItems]->fDepth			= fDepth;

	m_uiNumItems++;
}
  
RwUInt32 CNtlPLAtomicSorter::GetSortNum(void)
{
	return m_uiNumItems;
}

CNtlPLAtomicSorter::SSortData** CNtlPLAtomicSorter::GetSortList(void)
{
	return m_pSortList;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

CNtlPLAtomicSorterContainter* CNtlPLAtomicSorterContainter::m_pInstance = NULL;

CNtlPLAtomicSorterContainter::CNtlPLAtomicSorterContainter()
{
	NTL_ASSERTE(m_pInstance == NULL);

	m_pInstance = this;

	m_pWaterInAlphaSorter	= NULL;
	m_pWaterSorter			= NULL;
	m_pWaterWaveSorter		= NULL;
	m_pWaterOutAlphaSorter	= NULL;
}


CNtlPLAtomicSorterContainter::~CNtlPLAtomicSorterContainter()
{
	Destroy();

	m_pInstance = NULL;
}

CNtlPLAtomicSorterContainter* CNtlPLAtomicSorterContainter::GetInstance(void)
{
	return m_pInstance;
}

RwBool CNtlPLAtomicSorterContainter::Crete(void)
{
	NTL_FUNCTION("CNtlPLAtomicSorterContainter::Crete");

	CreatSorter();

	NTL_RETURN(TRUE);
}

void CNtlPLAtomicSorterContainter::Destroy(void)
{
	if(m_pWaterInAlphaSorter)
	{
		m_pWaterInAlphaSorter->Destroy(); 
		NTL_DELETE(m_pWaterInAlphaSorter);
		m_pWaterInAlphaSorter = NULL;
	}

	if(m_pWaterSorter)
	{
		m_pWaterSorter->Destroy();
		NTL_DELETE(m_pWaterSorter);
		m_pWaterSorter = NULL;
	}

	if(m_pWaterWaveSorter)
	{
		m_pWaterWaveSorter->Destroy();
		NTL_DELETE(m_pWaterWaveSorter);
		m_pWaterWaveSorter = NULL;
	}

	if(m_pWaterOutAlphaSorter)
	{
		m_pWaterOutAlphaSorter->Destroy();
		NTL_DELETE(m_pWaterOutAlphaSorter);
		m_pWaterOutAlphaSorter = NULL;
	}
}

void CNtlPLAtomicSorterContainter::Reset(void)
{
	NTL_SPROFILE("CNtlPLAtomicSorterContainter::Reset");

	if(m_pWaterInAlphaSorter)
		m_pWaterInAlphaSorter->Reset();

	if(m_pWaterSorter)
		m_pWaterSorter->Reset();

	if(m_pWaterWaveSorter)
		m_pWaterWaveSorter->Reset();

	if(m_pWaterOutAlphaSorter)
		m_pWaterOutAlphaSorter->Reset();

	NTL_EPROFILE()
}

void CNtlPLAtomicSorterContainter::CreatSorter(void)
{
	NTL_FUNCTION("CNtlPLAtomicSorterContainter::CreatSorter");

	if(NULL == m_pWaterInAlphaSorter)
	{
		m_pWaterInAlphaSorter = NTL_NEW CNtlPLAtomicSorter;
		m_pWaterInAlphaSorter->Create(100);
	}
	
	if(NULL == m_pWaterSorter)
	{
		m_pWaterSorter = NTL_NEW CNtlPLAtomicSorter;
		m_pWaterSorter->Create(20);
	}

	if(NULL == m_pWaterWaveSorter)
	{
		m_pWaterWaveSorter = NTL_NEW CNtlPLAtomicSorter;
		m_pWaterWaveSorter->Create(50);
	}

	if(NULL == m_pWaterOutAlphaSorter)
	{
		m_pWaterOutAlphaSorter = NTL_NEW CNtlPLAtomicSorter;
		m_pWaterOutAlphaSorter->Create(100);
	}

	NTL_RETURNVOID();
}

void CNtlPLAtomicSorterContainter::DestroySorter(void)
{
	NTL_FUNCTION("CNtlPLAtomicSorterContainter::DestroySorter");

	if(m_pWaterInAlphaSorter)
	{
		m_pWaterInAlphaSorter->Destroy(); 
		NTL_DELETE(m_pWaterInAlphaSorter);
		m_pWaterInAlphaSorter = NULL;
	}

	if(m_pWaterSorter)
	{
		m_pWaterSorter->Destroy();
		NTL_DELETE(m_pWaterSorter);
		m_pWaterSorter = NULL;
	}

	if(m_pWaterWaveSorter)
	{
		m_pWaterWaveSorter->Destroy();
		NTL_DELETE(m_pWaterWaveSorter);
		m_pWaterWaveSorter = NULL;
	}

	if(m_pWaterOutAlphaSorter)
	{
		m_pWaterOutAlphaSorter->Destroy();
		NTL_DELETE(m_pWaterOutAlphaSorter);
		m_pWaterOutAlphaSorter = NULL;
	}
	
	NTL_RETURNVOID();
	
}

void CNtlPLAtomicSorterContainter::Push(EPlSortType eType, void *pRenderEntity, RwReal fDepth)
{
	if(eType == PLSORT_ATOMIC)
	{
		RwReal			fDist			= 0.0f;
		RwReal			fWaterHeight	= 0.0f;
		CNtlPLEntity*	pPLEntity		= static_cast<CNtlPLEntity*>(RpNtlAtomicGetData((RpAtomic*)pRenderEntity));
		
		if (pPLEntity &&
			pPLEntity->GetClassType() == PLENTITY_OBJECT &&
			static_cast<CNtlPLObject*>(pPLEntity)->GetProperty()->IsSortByPos())
		{
			RwV3d vPos = pPLEntity->GetPosition();

			fDist			= vPos.y;
			fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(vPos);
		}
		else
		{
			const RwSphere *atomicBoundingSphere = RpAtomicGetWorldBoundingSphere((RpAtomic*)pRenderEntity);

			fDist			= atomicBoundingSphere->center.y + atomicBoundingSphere->radius + 0.1f;
			fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(atomicBoundingSphere->center);
		}

		if(fWaterHeight > fDist)
			m_pWaterInAlphaSorter->Push(eType, pRenderEntity, fDepth);
		else
			m_pWaterOutAlphaSorter->Push(eType, pRenderEntity, fDepth);
	}
	else if(eType == PLSORT_WATER)
	{
		m_pWaterSorter->Push(eType, pRenderEntity, fDepth);
	}
	else if(eType == PLSORT_ENTITY)
	{
		CNtlPLEntity *pPLEntity = (CNtlPLEntity*)(pRenderEntity);

		if(pPLEntity->GetClassType() == PLENTITY_SHORELINE)
		{
			m_pWaterWaveSorter->Push(eType, pRenderEntity, fDepth);
		}
		if(pPLEntity->GetClassType() == PLENTITY_WEATHER)
		{
			m_pWaterOutAlphaSorter->Push(eType, pRenderEntity, RwRealMINVAL);
		}
		else
		{
// 			const RwSphere *pSphere = pPLEntity->GetBoundingSphere();
// 			if(pSphere)
// 			{
// 			}
// 			else
// 			{
			RwV3d vPos = pPLEntity->GetPosition();

			// water height 빠르게 구하는 방식을 써야 한다.
			RwReal fWaterHeight = GetSceneManager()->GetWorldWaterHeight(vPos);

			if(fWaterHeight > vPos.y)
				m_pWaterInAlphaSorter->Push(eType, pRenderEntity, fDepth);
			else
				m_pWaterOutAlphaSorter->Push(eType, pRenderEntity, fDepth);
//			}
		}
	}
}

void CNtlPLAtomicSorterContainter::Push(EPlSortType eType, void *pRenderEntity, const RwV3d *pCameraPos)
{
	if(eType == PLSORT_ENTITY)
	{
		CNtlPLEntity *pPLEntity = (CNtlPLEntity*)pRenderEntity;
		RwV3d vPos, temp;
		vPos = pPLEntity->GetPosition();

		RwV3dSubMacro(&temp, &vPos, pCameraPos);
		RwReal fDepth = RwV3dDotProduct(&temp, &temp);

		Push(eType, pRenderEntity, fDepth);
	}
	else if(eType == PLSORT_WATER)
	{
		Push(eType, pRenderEntity, 0.0f);
	}
	else if(eType == PLSORT_ATOMIC)
	{
		RwV3d atomicPos, temp;
	  
		RwSphere *atomicBSphere = RpAtomicGetBoundingSphere((RpAtomic*)pRenderEntity);
		RwV3dTransformPoints(&atomicPos, &atomicBSphere->center, 1, RwFrameGetLTM(RpAtomicGetFrame((RpAtomic*)pRenderEntity)));
	   
		RwV3dSubMacro(&temp, &atomicPos, pCameraPos);
		RwReal fDepth = RwV3dDotProduct(&temp, &temp);

		Push(eType, pRenderEntity, fDepth);
	}
}


void CNtlPLAtomicSorterContainter::DepthSort(void)
{
	NTL_SPROFILE("CNtlPLAtomicSorterContainter::DepthSort")

	if(m_pWaterInAlphaSorter)
		m_pWaterInAlphaSorter->DepthSort();

	if(m_pWaterSorter)
		m_pWaterSorter->DepthSort();

	if(m_pWaterWaveSorter)
		m_pWaterWaveSorter->DepthSort();

	if(m_pWaterOutAlphaSorter)
		m_pWaterOutAlphaSorter->DepthSort();

	NTL_EPROFILE()
}

void CNtlPLAtomicSorterContainter::Render(void)
{
	NTL_SPROFILE("CNtlPLAtomicSorterContainter::Render")	

	RwUInt32 i;
	RwUInt32 uiSortNum;
	CNtlPLAtomicSorter::SSortData **pSortList;
	EPlSortType eSortType;

	// water before
	uiSortNum = m_pWaterInAlphaSorter->GetSortNum(); 
	pSortList = m_pWaterInAlphaSorter->GetSortList(); 
	for(i = 0; i < uiSortNum; ++i)
	{
		eSortType = pSortList[i]->eSortType;
		if(eSortType == PLSORT_ATOMIC)
			SortedAtomicRender( (RpAtomic*)pSortList[i]->pRenderEntity );
		else
		{
			CNtlPLEntity *pPLEntity = (CNtlPLEntity*)pSortList[i]->pRenderEntity;
			RwUInt32 uiRenderLayer = pPLEntity->GetLayer();
			BeginRenderGroupLayer(uiRenderLayer);
			pPLEntity->Render();
			EndRenderGroupLayer(uiRenderLayer);
		}
	}

	// water
	uiSortNum = m_pWaterSorter->GetSortNum(); 
	pSortList = m_pWaterSorter->GetSortList(); 
	SetWaterRenderStateBegin(dGET_WORLD_PARAM()->ClrDayAndNight);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
	for(i = 0; i < uiSortNum; ++i)
	{
		RpNtlWorldSectorWaterRender( (CNtlWorldSector*)pSortList[i]->pRenderEntity );
	}
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE); 
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
	SetWaterRenderStateEnd();

	CNtlPLEntity *pPLEntity = NULL;
	RwUInt32 uiRenderLayer;

	// water wave

	uiSortNum = m_pWaterWaveSorter->GetSortNum(); 
	pSortList = m_pWaterWaveSorter->GetSortList(); 
	for(i = 0; i < uiSortNum; ++i)
	{
		eSortType = pSortList[i]->eSortType;
		if(eSortType == PLSORT_ATOMIC)
			SortedAtomicRender( (RpAtomic*)pSortList[i]->pRenderEntity );
		else
		{
			pPLEntity = (CNtlPLEntity*)pSortList[i]->pRenderEntity;
			uiRenderLayer = pPLEntity->GetLayer();
			BeginRenderGroupLayer(uiRenderLayer);
			pPLEntity->Render();
			EndRenderGroupLayer(uiRenderLayer);
		}
	}

	// water after
	uiSortNum = m_pWaterOutAlphaSorter->GetSortNum(); 
	pSortList = m_pWaterOutAlphaSorter->GetSortList(); 
	for(i = 0; i < uiSortNum; ++i)
	{
		eSortType = pSortList[i]->eSortType;
		if(eSortType == PLSORT_ATOMIC)
			SortedAtomicRender( (RpAtomic*)pSortList[i]->pRenderEntity );
		else
		{
			pPLEntity = (CNtlPLEntity*)pSortList[i]->pRenderEntity;
			uiRenderLayer = pPLEntity->GetLayer();
			BeginRenderGroupLayer(uiRenderLayer);
			pPLEntity->Render();
			EndRenderGroupLayer(uiRenderLayer);
		}
	}

	NTL_EPROFILE()
}
