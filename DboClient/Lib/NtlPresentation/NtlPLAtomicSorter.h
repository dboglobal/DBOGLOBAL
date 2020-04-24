
/*****************************************************************************
 *
 * File			: NtlPLAtomicSorter.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 06	
 * Abstract		: Presentation atomic sorter
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLATOMICSORTER_H__
#define __NTL_PLATOMICSORTER_H__

#include <rwcore.h>
#include <rpworld.h>
#include <map>
#include "NtlPLDef.h"

enum EPlSortType
{
	PLSORT_ATOMIC,
	PLSORT_WATER,
	PLSORT_ENTITY
};

class CNtlPLAtomicSorter
{
public:

	struct SSortData
	{
		EPlSortType eSortType;
		void* pRenderEntity;		// 레더링할 객체의 포인터
		RwReal fDepth;				// 카메라와 렌더링 객체와의 거리
	};
	
private:
    
    SSortData *m_pSortData;
	SSortData **m_pSortList;
    	
	RwUInt32 m_uiGrowSize;
    RwUInt32 m_uiSizeSortList;
    RwUInt32 m_uiNumItems;

public:
    
    CNtlPLAtomicSorter();
	~CNtlPLAtomicSorter();

	void Create(RwUInt32 uiSizeSortList);
	void Destroy(void);

	void Reset(void);
	void Push(EPlSortType eType, void *pRenderEntity, RwReal fDepth);
	
	void PreSort(void);
    void DepthSort(void);

	RwUInt32 GetSortNum(void);
	SSortData** GetSortList(void);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLAtomicSorterContainter
{
private:
	
	static CNtlPLAtomicSorterContainter *m_pInstance;
	
	CNtlPLAtomicSorter *m_pWaterInAlphaSorter;
	CNtlPLAtomicSorter *m_pWaterSorter;
	CNtlPLAtomicSorter *m_pWaterWaveSorter;
	CNtlPLAtomicSorter *m_pWaterOutAlphaSorter;

private:

	void Push(EPlSortType eType, void *pRenderEntity, RwReal fDepth);

public:

	CNtlPLAtomicSorterContainter();
	~CNtlPLAtomicSorterContainter();

	static CNtlPLAtomicSorterContainter* GetInstance(void);

	RwBool Crete(void);
	void Destroy(void);

	void Reset(void);

	void CreatSorter(void);
	void DestroySorter(void);
	
	void Push(EPlSortType eType, void *pRenderEntity, const RwV3d *pCameraPos);

	void DepthSort(void);
	void Render(void);
};

static CNtlPLAtomicSorterContainter* GetAtomicSorterContainer(void)
{
	return CNtlPLAtomicSorterContainter::GetInstance(); 
};



#endif
