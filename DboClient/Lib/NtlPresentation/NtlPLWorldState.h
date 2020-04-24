#pragma once

#include "ceventhandler.h"

class CNtlPLEntity;
class CNtlPLObject;

class CNtlPLWorldState
{	
public:
	static void					Init();
	static void					Terminate();

	static void					InitSpace();

	static void					Update(RwV3d& vPos);

	static CNtlPLObject*		GetActiveMiniIndoor();

protected:
	static void					UpdateSpace(RwV3d& vPos);

public:
	static RwUInt32				eActiveWorldType;
	static CNtlPLObject*		pActiveMiniIndoor;
	static CNtlPLObject*		pActiveMiniIndoorNew;
};

// RESTURN : TRUE(내부), FALSE(외부)
RwBool GetNtlWorldSpace(RwV3d* pvPos, CNtlPLEntity** ppPLEntity, RwReal* pfDist);

// Evnet Check
RwBool CheckEventsWorldEffectChanged(RWS::CMsg& Msg);
RwBool CheckEventsWorldEffectChangedWithOutDelay(RWS::CMsg& Msg);