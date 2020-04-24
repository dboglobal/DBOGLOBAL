#pragma once


#include "Spawn.h"

using std::vector;
#include <vector>


class CNtlPLEntity;
class CNtlWorldFieldManager;


class CSpawnInstNode
{
public:
	CSpawnInstNode(void);
	virtual ~CSpawnInstNode(void);

private:
	vector<CSpawn*> m_vecSpawn;

public:
	vector<CSpawn*>::iterator	BEGIN() { return m_vecSpawn.begin(); }
	vector<CSpawn*>::iterator	END()  { return m_vecSpawn.end(); }

public:
	CSpawn*	Insert(CNtlPLEntity* _pNtlPLEntity, eSPAWN_TYPE _eSpawnType);
	CSpawn*	Insert(CSpawn* _pSpawn);
	VOID	Delete(CNtlPLEntity* _pNtlPLEntity);
	VOID	Delete(CSpawn* _pSpawn);
	VOID	Delete(RwV3d& _Pos);
	VOID	DeleteSpawnElement(CSpawn* _pSpawn);

	VOID	CreateEntity(CNtlWorldFieldManager* pMgr);
	VOID	DestroyEntity();

	RwInt32 GetSpawnNestCnt();
	RwInt32	GetSpawnCnt() { return static_cast<RwInt32>(m_vecSpawn.size()); }
	CSpawn*	GetSpawn(CNtlPLEntity* _pNtlPLEntity);
	VOID	InsertSpawnLoadAtField(vector<CSpawn*>& _vecSpawnLoad);
	VOID	DeleteSpawnLoadAtField(vector<CSpawn*>& _vecSpawnLoad);

public:
	VOID Save(FILE* _pFile);
};
