#pragma once


#include "Spawn.h"


class CNtlPLEntity;


class CSpawnNPC : public CSpawn
{
public:
	CSpawnNPC();
	virtual ~CSpawnNPC(void);

	virtual	eSPAWN_TYPE GetSpawnType() { return eSPAWN_NPC; }
};