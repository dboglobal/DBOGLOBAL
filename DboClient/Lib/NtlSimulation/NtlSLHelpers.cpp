#include "precomp_ntlsimulation.h"
#include "NtlSLHelpers.h"

// shared
#include "ItemTable.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"

RwInt32 Helper_GetSLEventDataSize(RWS::CMsg& pMsg)
{
	RwInt32 iSize = -1;

	if(pMsg.Id == g_EventNetSendError)
	{
		iSize = 0;
	}
	else if(pMsg.Id == g_EventKeyboardMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventKeyboardMove);
	}
	else if(pMsg.Id == g_EventKeyboardMoveStop)
	{
		iSize = 0;
	}
	else if(pMsg.Id == g_EventCameraShake)
	{
		iSize = (RwInt32)sizeof(SNtlEventCameraShake);
	}
	else if(pMsg.Id == g_EventActionMapJump)
	{
		iSize = 0;
	}
	else if (pMsg.Id == g_EventActionMapAirJump)
	{
		iSize = 0;
	}
	else if(pMsg.Id == g_EventActionMapSitAndStand)
	{
		iSize = 0;
	}
	else if(pMsg.Id == g_EventActionMapSkillUse)
	{
		iSize = (RwInt32)sizeof(SNtlEventActionMapSkillUse);
	}
	else if(pMsg.Id == g_EventActionMapHTBUse)
	{
		iSize = (RwInt32)sizeof(SNtlEventActionMapHTBUse);
	}
	else if(pMsg.Id == g_EventActionMapItemUse)
	{
		iSize = (RwInt32)sizeof(SNtlEventActionMapItemUse);
	}
	else if(pMsg.Id == g_EventTerrainClick)
	{
		iSize = (RwInt32)sizeof(SNtlEventTerrainClick);
	}
	else if(pMsg.Id == g_EventSobTargetSelect)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobTargetSelect);
	}
	else if(pMsg.Id == g_EventSobTargetSelectRelease)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobTargetSelectRelease);
	}
	else if(pMsg.Id == g_EventSobAttackSelect)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAttackSelect);
	}
	else if(pMsg.Id == g_EventSobGotFocus)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobGotFocus);
	}
	else if(pMsg.Id == g_EventSobLostFocus)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobLostFocus);
	}
	else if(pMsg.Id == g_EventSobCreate)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobBaseCreate);
	}
	else if(pMsg.Id == g_EventSobDelete)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobDelete);
	}
	else if(pMsg.Id == g_EventSobMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobMove);
	}
	else if(pMsg.Id == g_EventSobSecondDestMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSecondDestMove);
	}
	else if(pMsg.Id == g_EventSobMoveStop)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobMoveStop);
	}
	else if(pMsg.Id == g_EventSobAdjustMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAdjustMove);
	}
	else if(pMsg.Id == g_EventSobFollowMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobFollowMove);
	}
	else if(pMsg.Id == g_EventSobItemMove)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobItemMove);
	}
	else if(pMsg.Id == g_EventSobAttack)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAttack);
	}
	else if (pMsg.Id == g_EventSobSpecialAttack)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSpecialAttack);
	}
	else if(pMsg.Id == g_EventSobHit)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobHit);
	}
	else if(pMsg.Id == g_EventSobAttackFollow)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAttackFollow);
	}
	else if(pMsg.Id == g_EventSobAttackMode)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAttackMode);
	}
	else if(pMsg.Id == g_EventSobJump)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobJump);
	}
	else if(pMsg.Id == g_EventSobSit)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSit);
	}
	else if(pMsg.Id == g_EventSobSkillCasting)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSkillCasting);
	}
	else if(pMsg.Id == g_EventSobSkillCancel)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSkillCancel);
	}
	else if(pMsg.Id == g_EventSobSkillAction)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobSkillAction);
	}
	else if(pMsg.Id == g_EventSobInfoUpdate)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobInfoUpdate);
	}
	else if(pMsg.Id == g_EventSobNpcCommunity)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobNpcCommunity);
	}
	else if(pMsg.Id == g_EventSobTargetInfoUpdate)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobTargetInfoUpdate);
	}
	else if(pMsg.Id == g_EventSobVehicleEngine)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobVehicleEngine);
	}
	else if (pMsg.Id == g_EventSobAirJump)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAirJump);
	}
	else if (pMsg.Id == g_EventSobAirAccel)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAirAccel);
	}
	else if (pMsg.Id == g_EventSobAnimPlay)
	{
		iSize = (RwInt32)sizeof(SNtlEventSobAnimPlay);
	}
	
	return iSize;
}
