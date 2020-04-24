#include "stdafx.h"
#include "AspectState_Vehicle.h"
#include "CPlayer.h"

CAspectState_Vehicle::CAspectState_Vehicle(CCharacter *pParent)
	:CAspectState(ASPECTSTATE_VEHICLE, pParent)
{
}

CAspectState_Vehicle::~CAspectState_Vehicle()
{
}


int CAspectState_Vehicle::CopyAspectTo(sASPECTSTATE_DETAIL* pStateDetail)
{
	CPlayer* pPlayer = (CPlayer*)m_pParent;

	pStateDetail->sVehicle.idVehicleItemHandle = pPlayer->GetVehicleItemHandle();
	pStateDetail->sVehicle.idVehicleTblidx = pPlayer->GetVehicleTblidx();

	return sizeof(sASPECTSTATE_VEHICLE);
}

void CAspectState_Vehicle::CopyAspectFrom(sASPECTSTATE_DETAIL* pStateDetail)
{
}