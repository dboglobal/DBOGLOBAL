/*****************************************************************************
* File			: DboPackethandler_GUVehicle.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2009. 1. 6
* Abstract		: Vechicle 관련 패킷
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"



void PacketHandler_GUVehicleStartNfy(void *pPacket)
{
	// The riding on the riding starts or rides while riding appears
	sGU_VEHICLE_START_NFY* pResult = (sGU_VEHICLE_START_NFY*)pPacket;


	CNtlSLEventGenerator::SobVehicleStart(pResult->hDriverHandle,
										  pResult->hVehicleItem,
										  pResult->idVehicleItemTblidx);
}

void PacketHandler_GUVehicleEndRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_END_RES);

	sGU_VEHICLE_END_RES* pResult = (sGU_VEHICLE_END_RES*) pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_END_RES"), TRUE);
		return;
	}
}

void PacketHandler_GUVehicleStuntNfy(void *pPacket)
{
	sGU_VEHICLE_STUNT_NFY* pResult = (sGU_VEHICLE_STUNT_NFY*) pPacket;

	CNtlSLEventGenerator::SobVehicleStunt( pResult->hDriverHandle );
}

void PacketHandler_GUVehicleFuelInsertRes(void * pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_VEHICLE_FUEL_INSERT_RES);

	sGU_VEHICLE_FUEL_INSERT_RES* pResult = (sGU_VEHICLE_FUEL_INSERT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_VEHICLE_FUEL_INSERT_RES"), TRUE);
		return;
	}

	CNtlSLEventGenerator::SobVehicleEngine(Logic_GetAvatarHandle(), VEHICLE_ENGINE_REG_FUEL, pResult->hItem);
}
