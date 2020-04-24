#include "precomp_ntlsimulation.h"
#include "NtlSobFactory.h"
#include "NtlDebug.h"
#include "NtlSob.h"
#include "NtlSobPlayer.h"
#include "NtlSobAvatar.h"
#include "NtlSobMonster.h"
#include "NtlSobNpc.h"
#include "NtlSobPet.h"
#include "NtlSobItem.h"
#include "NtlSobQuestItem.h"
#include "NtlSobSyncStatus.h"
#include "NtlSobProjectile.h"
#include "NtlSobInfluence.h"
#include "NtlSobBuff.h"
#include "NtlSobSkill.h"
#include "NtlSobWorldItem.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobActionSkill.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobEventObject.h"
#include "NtlSobVehicle.h"
#include "NtlSobDynamicObject.h"

#define SOB_FACTORY_KEY		0x9000000 // changed from 0x80000000 to 0x8000000 because 0x80000000 is too high

SERIAL_HANDLE CNtlSobFactory::m_uiSerialId = SOB_FACTORY_KEY;


SERIAL_HANDLE CNtlSobFactory::AcquireSerailId(void)
{
	m_uiSerialId++;

	if(m_uiSerialId == INVALID_SERIAL_ID)
		m_uiSerialId = SOB_FACTORY_KEY;

	return m_uiSerialId;
}

SERIAL_HANDLE CNtlSobFactory::AcquireTriggerSerialId(void)
{
	static SERIAL_HANDLE uiTriggerSerialId = 0x01000000;
	uiTriggerSerialId++;

	if(uiTriggerSerialId == INVALID_SERIAL_ID)
		uiTriggerSerialId = 0x01000000;

	return uiTriggerSerialId;
}

RwBool CNtlSobFactory::IsClientCreateSerialId(SERIAL_HANDLE hSerialId)
{
	if(hSerialId & SOB_FACTORY_KEY)
		return TRUE;

	return FALSE;
}

CNtlSob* CNtlSobFactory::CreateSobFactroy(RwUInt32 uiClassId)
{
	NTL_FUNCTION("CNtlSobFactory::CreateSobFactroy");

	CNtlSob *pObj = NULL;
				
	switch(uiClassId)
	{
	case SLCLASS_AVATAR:
		pObj = NTL_NEW CNtlSobAvatar;
		break;
	case SLCLASS_PLAYER:
		pObj = NTL_NEW CNtlSobPlayer;
		break;
	case SLCLASS_MONSTER:
		pObj = NTL_NEW CNtlSobMonster;
		break;
	case SLCLASS_NPC:
		pObj = NTL_NEW CNtlSobNpc;
		break;
	case SLCLASS_PET:
		pObj = NTL_NEW CNtlSobPet;
		break;
	case SLCLASS_SLOT_ITEM:
		pObj = NTL_NEW CNtlSobItem;
		break;
	case SLCLASS_SLOT_QUESTITEM:
		pObj = NTL_NEW CNtlSobQuestItem;
		break;
	case SLCLASS_PROJECTILE:
		pObj = NTL_NEW CNtlSobProjectile;
		break;
	case SLCLASS_BUFF:
		pObj = NTL_NEW CNtlSobBuff;
		break;
	case SLCLASS_SKILL:
		pObj = NTL_NEW CNtlSobSkill;
		break;
	case SLCLASS_WORLD_ITEM:
		pObj = NTL_NEW CNtlSobWorldItem;
		break;
	case SLCLASS_HTBSKILL:
		pObj = NTL_NEW CNtlSobHTBSkill;
		break;
	case SLCLASS_ACTIONSKILL:
		pObj = NTL_NEW CNtlSobActionSkill;
		break;
	case SLCLASS_TRIGGER_OBJECT:
		pObj = NTL_NEW CNtlSobTriggerObject;
		break;
    case SLCLASS_EVENT_OBJECT:
        pObj = NTL_NEW CNtlSobEventObject();
        break;
	case SLCLASS_VEHICLE:
		pObj = NTL_NEW CNtlSobVehicle;
		break;
	case SLCLASS_DYNAMIC_OBJECT:
		pObj = NTL_NEW CNtlSobDynamicObject;
		break;
	}

	NTL_RETURN(pObj);
}

void CNtlSobFactory::DeleteSobFactory(CNtlSob *pSobObj)
{
	NTL_FUNCTION("CNtlSobFactory::DeleteSobFactory");

	NTL_DELETE( pSobObj );

	NTL_RETURNVOID();
}

