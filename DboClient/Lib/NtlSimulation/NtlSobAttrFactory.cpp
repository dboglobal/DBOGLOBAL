#include "precomp_ntlsimulation.h"
#include "NtlSobAttrFactory.h"
#include "NtlDebug.h"
#include "NtlSobAttr.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobProjectileAttr.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobBuffAttr.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobVehicleAttr.h"
#include "NtlSobDynamicObjectAttr.h"


CNtlSobAttr* CNtlSobAttrFactory::CreateSobAttrFactory(RwUInt32 uiClassId)
{
	NTL_FUNCTION("CNtlSobAttrFactory::CreateSobAttrFactory");

	/*
	CNtlSLAttrFactory *pFactory = CNtlSLAttrFactory::m_pFactory[uiClassId];

	if(pFactory)
	{
		NTL_RETURN(pFactory->CreateFactory()); 
	}
	*/
	
	CNtlSobAttr *pObjAttr = NULL;
	switch(uiClassId)
	{
	case SLCLASS_AVATAR:
		pObjAttr = NTL_NEW CNtlSobAvatarAttr;
		break;
	case SLCLASS_PLAYER:
		pObjAttr = NTL_NEW CNtlSobPlayerAttr;
		break;
	case SLCLASS_MONSTER:
		pObjAttr = NTL_NEW CNtlSobMonsterAttr;
		break;
	case SLCLASS_NPC:
		pObjAttr = NTL_NEW CNtlSobNpcAttr;
		break;
	case SLCLASS_PET:
		pObjAttr = NTL_NEW CNtlSobPetAttr;
		break;
	case SLCLASS_SLOT_ITEM:
		pObjAttr = NTL_NEW CNtlSobItemAttr;
		break;
	case SLCLASS_PROJECTILE:
		pObjAttr = NTL_NEW CNtlSobProjectileAttr;
		break;
	case SLCLASS_SKILL:
		pObjAttr = NTL_NEW CNtlSobSkillAttr;
		break;
	case SLCLASS_BUFF:
		pObjAttr = NTL_NEW CNtlSobBuffAttr;
		break;
	case SLCLASS_WORLD_ITEM:
		pObjAttr = NTL_NEW CNtlSobWorldItemAttr;
		break;
	case SLCLASS_HTBSKILL:
		pObjAttr = NTL_NEW CNtlSobHTBSkillAttr;
		break;
	case SLCLASS_ACTIONSKILL:
		pObjAttr = NTL_NEW CNtlSobActionSkillAttr;
		break;
	case SLCLASS_SLOT_QUESTITEM:
		pObjAttr = NTL_NEW CNtlSobQuestItemAttr;
		break;
	case SLCLASS_TRIGGER_OBJECT:
		pObjAttr = NTL_NEW CNtlSobTriggerObjectAttr;
		break;
	case SLCLASS_VEHICLE:
		pObjAttr = NTL_NEW CNtlSobVehicleAttr;
		break;
	case SLCLASS_DYNAMIC_OBJECT:
		pObjAttr = NTL_NEW CNtlSobDynamicObjectAttr;
		break;
	}

	NTL_RETURN(pObjAttr);
}

void CNtlSobAttrFactory::DeleteSobAttrFactory(CNtlSobAttr *pSobObj)
{
	NTL_FUNCTION("CNtlSobAttrFactory::DeleteSobAttrFactory");

	NTL_DELETE( pSobObj );

	NTL_RETURNVOID();
}