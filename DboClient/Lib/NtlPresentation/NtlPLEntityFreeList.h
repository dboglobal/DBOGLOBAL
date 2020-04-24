/*****************************************************************************
 *
 * File			: NtlPLEntityFreeList.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer entity free list
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLENTITY_FREELIST_H__
#define __NTL_PLENTITY_FREELIST_H__

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLDef.h"

/**
 * \ingroup NtlPresentation
 * memory 단편화를 방지하기 위한 entity memory pool class.
 * renderware의 free list를 사용한다.
 *
 */

class CNtlPLEntityFreeList
{
private:

	static RwFreeList *m_pObjFreeList;			/* object memory pool **/
	static RwFreeList *m_pCharFreeList;			/* character memory pool **/
	// by agebreak
	static RwFreeList *m_pEffectFreeList;		/* effect memory pool **/
	// woody1019
	static RwFreeList *m_pWorldFreeList;
	static RwFreeList *m_pSoundFreeList;
	static RwFreeList *m_pSoundBGMFreeList;
	// jhssugi
	static RwFreeList *m_pDummyWorldFreeList;	/** dummy world free list */
	// by hodong
	static RwFreeList *m_pItemFreeList;
    static RwFreeList *m_pPlayerNameFreeList;
	static RwFreeList *m_pDamageBoxFreeList;
	static RwFreeList *m_pDecalFreeList;
	static RwFreeList *m_pSunFreeList;			// Sun free list by agebreak
	static RwFreeList *m_pPlanetFreeList;		// Sun free list 문제로 Planet freelist 추가
public:

	/**
	*  entity free list 객체를 initialize시키는 함수.
	*  \see Destroy
	*/
	static RwBool AllocFreeList(void);

	/**
	*  entity free list 객체를 terminate시키는 함수.
	*  \see Destroy
	*/
	static void DeAllocFreeList(void);

	/**
	*  free list memory를 할당한다.
	*  \param eType entity type
	*  \return 성공하면 요청한 free list memory , 실패하면 NULL
	*  \see Free
	*/
	static void* Alloc(const ENtlPLEntityType eType);

	/**
	*  free list memory를 해제한다.
	*  \param eType entity type
	*  \param pObj 해제할 free list memory.
	*  \see Alloc
	*/
	static void Free(const ENtlPLEntityType eType, void *pObj);
};

#endif
