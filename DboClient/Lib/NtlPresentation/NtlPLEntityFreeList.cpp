#include "precomp_ntlpresentation.h"
#include "NtlPLEntityFreeList.h"
#include "NtlDebug.h"
#include "NtlPLObject.h"
#include "NtlPLCharacter.h"
#include "NtlInstanceEffect.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLDummyWorld.h"
#include "NtlPLItem.h"
#include "NtlPLPlayerName.h"
#include "NtlPLDamageBox.h"
#include "NtlPLDecal.h"
#include "NtlPLSound.h"
#include "NtlPLSun.h"
#include "NtlPLPlanet.h"

RwFreeList *CNtlPLEntityFreeList::m_pObjFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pCharFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pEffectFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pWorldFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pSoundFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pSoundBGMFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pDummyWorldFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pItemFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pPlayerNameFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pDamageBoxFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pDecalFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pSunFreeList = NULL;
RwFreeList *CNtlPLEntityFreeList::m_pPlanetFreeList = NULL;

RwBool CNtlPLEntityFreeList::AllocFreeList(void)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Create");

	NTL_PRE(m_pObjFreeList == 0);

	RwUInt32 iBlockSize = 100;
    m_pObjFreeList = RwFreeListCreate(sizeof(CNtlPLObject), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pObjFreeList, "object failed to allocate free list");

	iBlockSize = 100;
    m_pCharFreeList = RwFreeListCreate(sizeof(CNtlPLCharacter), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pCharFreeList, "character failed to allocate free list");

	// by agebreak
	iBlockSize = 100;
    m_pEffectFreeList = RwFreeListCreate(sizeof(CNtlInstanceEffect), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pEffectFreeList, "effect failed to allocate free list");

	// woody1019
	iBlockSize = 2;
	m_pWorldFreeList = RwFreeListCreate(sizeof(CNtlPLWorldEntity), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pWorldFreeList, "world failed to allocate free list");

	iBlockSize = 2;
	m_pDummyWorldFreeList = RwFreeListCreate(sizeof(CNtlPLDummyWorld), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pDummyWorldFreeList, "dummy world failed to allocate free list");

	iBlockSize = 100;
	m_pSoundFreeList = RwFreeListCreate(sizeof(CNtlPLSound), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pDummyWorldFreeList, "sound failed to allocate free list");

	iBlockSize = 10;
	m_pSoundBGMFreeList = RwFreeListCreate(sizeof(CNtlPLSoundBGM), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pDummyWorldFreeList, "sound failed to allocate free list");

	// hodong
	iBlockSize = 100;
	m_pItemFreeList = RwFreeListCreate(sizeof(CNtlPLItem), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pItemFreeList, "effect failed to allocate free list");

	iBlockSize = 10;
	m_pPlayerNameFreeList = RwFreeListCreate(sizeof(CNtlPLPlayerName), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pPlayerNameFreeList, "player name failed to allocate free list");

	iBlockSize = 50;
	m_pDamageBoxFreeList = RwFreeListCreate(sizeof(CNtlPLDamageBox), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pDamageBoxFreeList, "damage box failed to allocate free list");

	iBlockSize = 50;
	m_pDecalFreeList = RwFreeListCreate(sizeof(CNtlPLDecal), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pDecalFreeList, "decal failed to allocate free list");

	// agebreak
	iBlockSize = 5;
	m_pSunFreeList = RwFreeListCreate(sizeof(CNtlPLSun), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pSunFreeList, "Sun Failed to allocate free list");

	iBlockSize = 5;
	m_pPlanetFreeList = RwFreeListCreate(sizeof(CNtlPLPlanet), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
	RWS_ASSERT(m_pPlanetFreeList, "Sun_Planet Failed to allocate free list");

    NTL_RETURN(TRUE);
}

void CNtlPLEntityFreeList::DeAllocFreeList(void)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Destroy");

	if (m_pObjFreeList)
    {
        RwFreeListDestroy(m_pObjFreeList);
        m_pObjFreeList = 0;
    }

	if (m_pCharFreeList)
	{
		RwFreeListDestroy(m_pCharFreeList);
        m_pCharFreeList = 0;
	}
	// by agebreak
	if (m_pEffectFreeList)
	{
		RwFreeListDestroy(m_pEffectFreeList);
        m_pEffectFreeList = 0;
	}

	// woody1019
	if (m_pWorldFreeList)
	{
		RwFreeListDestroy(m_pWorldFreeList);
		m_pWorldFreeList = 0;
	}

	if (m_pDummyWorldFreeList)
	{
		RwFreeListDestroy(m_pDummyWorldFreeList);
		m_pDummyWorldFreeList = 0;
	}

	if (m_pSoundFreeList)
	{
		RwFreeListDestroy(m_pSoundFreeList);
		m_pSoundFreeList = 0;
	}

	if (m_pSoundBGMFreeList)
	{
		RwFreeListDestroy(m_pSoundBGMFreeList);
		m_pSoundBGMFreeList = 0;
	}

	// hodong
	if (m_pItemFreeList)
	{
		RwFreeListDestroy(m_pItemFreeList);
		m_pItemFreeList = 0;
	}

	if(m_pPlayerNameFreeList)
	{
		RwFreeListDestroy(m_pPlayerNameFreeList);
		m_pPlayerNameFreeList = 0;
	}

	if(m_pDamageBoxFreeList)
	{
		RwFreeListDestroy(m_pDamageBoxFreeList);
		m_pDamageBoxFreeList = 0;
	}

	if(m_pDecalFreeList)
	{
		RwFreeListDestroy(m_pDecalFreeList);
		m_pDecalFreeList = 0;
	}
	if(m_pSunFreeList)
	{
		RwFreeListDestroy(m_pSunFreeList);
		m_pSunFreeList = NULL;
	}
	if (m_pPlanetFreeList)
	{
		RwFreeListDestroy(m_pPlanetFreeList);
		m_pPlanetFreeList = NULL;
	}

    NTL_RETURNVOID();
}

void* CNtlPLEntityFreeList::Alloc(const ENtlPLEntityType eType)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Alloc");

	// by agebreak 왜 있는지 이유를 모르겠다. 임시로 막아둔다.
	//NTL_PRE(m_pObjFreeList);

	switch(eType)
	{
	case PLENTITY_OBJECT:
		NTL_RETURN(RwFreeListAlloc(m_pObjFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case PLENTITY_CHARACTER:
		NTL_RETURN(RwFreeListAlloc(m_pCharFreeList, rwMEMHINTDUR_GLOBAL));
		break;

	// by agebreak
	case PLENTITY_EFFECT:
		NTL_RETURN(RwFreeListAlloc(m_pEffectFreeList, rwMEMHINTDUR_GLOBAL));
		break;

	// by woody
	case PLENTITY_WORLD:
		{
			NTL_RETURN(RwFreeListAlloc(m_pWorldFreeList, rwMEMHINTDUR_GLOBAL));
			break;
		}
	case PLENTITY_DUMMY_WORLD:
		{
			NTL_RETURN(RwFreeListAlloc(m_pDummyWorldFreeList, rwMEMHINTDUR_GLOBAL));
			break;
		}
	case PLENTITY_SOUND:
		{
			NTL_RETURN(RwFreeListAlloc(m_pSoundFreeList, rwMEMHINTDUR_GLOBAL));
			break;
		}
	case PLENTITY_SOUND_BGM:
		{
			NTL_RETURN(RwFreeListAlloc(m_pSoundBGMFreeList, rwMEMHINTDUR_GLOBAL));
			break;
		}
	// by hodong
	case PLENTITY_ITEM:
		NTL_RETURN(RwFreeListAlloc(m_pItemFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case PLENTITY_PLAYER_NAME:
		NTL_RETURN(RwFreeListAlloc(m_pPlayerNameFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case PLENTITY_DAMAGE_BOX:
		NTL_RETURN(RwFreeListAlloc(m_pDamageBoxFreeList, rwMEMHINTDUR_GLOBAL));
		break; 
	case PLENTITY_DECAL:
		NTL_RETURN(RwFreeListAlloc(m_pDecalFreeList, rwMEMHINTDUR_GLOBAL));
		break; 
	case PLENTITY_SUN:
		NTL_RETURN(RwFreeListAlloc(m_pSunFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case PLENTITY_SUN_PLANET:
		NTL_RETURN(RwFreeListAlloc(m_pPlanetFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	}
	
	NTL_RETURN(NULL);
}

void CNtlPLEntityFreeList::Free(const ENtlPLEntityType eType, void *pObj)
{
	NTL_FUNCTION("CNtlPLEntityFreeList::Free");

	NTL_PRE(pObj);
	NTL_PRE(m_pObjFreeList);

	switch(eType)
	{
	case PLENTITY_OBJECT:
		 RwFreeListFree(m_pObjFreeList, pObj);
		break;
	case PLENTITY_CHARACTER:
		RwFreeListFree(m_pCharFreeList, pObj);
		break;
	// by agebreak
	case PLENTITY_EFFECT:
		RwFreeListFree(m_pEffectFreeList, pObj);
		break;

	// by woody1019
	case PLENTITY_WORLD:
		{
			RwFreeListFree(m_pWorldFreeList, pObj);
			break;
		}
	case PLENTITY_SOUND:
		{
			RwFreeListFree(m_pSoundFreeList, pObj);
			break;
		}
	case PLENTITY_SOUND_BGM:
		{
			RwFreeListFree(m_pSoundBGMFreeList, pObj);
			break;
		}
	case PLENTITY_DUMMY_WORLD:
		{
			RwFreeListFree(m_pDummyWorldFreeList, pObj);
			break;
		}

	//by hodong
	case PLENTITY_ITEM:
		RwFreeListFree(m_pItemFreeList, pObj);
		break;
	case PLENTITY_PLAYER_NAME:
		RwFreeListFree(m_pPlayerNameFreeList, pObj);
		break;
	case PLENTITY_DAMAGE_BOX:
		RwFreeListFree(m_pDamageBoxFreeList, pObj);
		break;
	case PLENTITY_DECAL:
		RwFreeListFree(m_pDecalFreeList, pObj);
		break;
	case PLENTITY_SUN:
		RwFreeListFree(m_pSunFreeList, pObj);
		break;
	case PLENTITY_SUN_PLANET:
		RwFreeListFree(m_pPlanetFreeList, pObj);
		break;
	}

	NTL_RETURNVOID();
}