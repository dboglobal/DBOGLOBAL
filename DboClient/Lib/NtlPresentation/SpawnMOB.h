#pragma once


#include "Spawn.h"


class CNtlPLEntity;


class CSpawnMOB : public CSpawn
{
public:
	CSpawnMOB();
	virtual ~CSpawnMOB(void);

	virtual	eSPAWN_TYPE GetSpawnType() { return eSPAWN_MOB; }
};
