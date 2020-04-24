#pragma once


class CSpawnTreeNode;
class CNtlPLEntity;
class CNtlWorldFieldManager;
class CSpawnInstNode;
class CSpawnTreeElement;


#include "Spawn.h"

using std::map;
using std::string;
#include <map>
#include <string>


class CSpawnContainer
{
public:
	CSpawnContainer(void);
	~CSpawnContainer(void);

	typedef std::map<RwInt32, CSpawnInstNode*>				SPAWN_INST_NODE_MAP;
	typedef std::map<RwInt32, CSpawnInstNode*>::value_type	SPAWN_INST_NODE_VALUE;
	typedef std::map<RwInt32, CSpawnInstNode*>::iterator	SPAWN_INST_NODE_ITER;

	typedef std::map<RwInt32, CSpawnTreeNode*>				SPAWN_TREE_NODE_MAP;
	typedef std::map<RwInt32, CSpawnTreeNode*>::value_type	SPAWN_TREE_NODE_VALUE;
	typedef std::map<RwInt32, CSpawnTreeNode*>::iterator	SPAWN_TREE_NODE_ITER;

private:	
	SPAWN_TREE_NODE_MAP m_mapSpawnTreeNode;
	SPAWN_INST_NODE_MAP	m_mapSpawnInstNode;

public:
	SPAWN_TREE_NODE_ITER	STN_BEGIN() { return m_mapSpawnTreeNode.begin(); }
	SPAWN_TREE_NODE_ITER	STN_END()  { return m_mapSpawnTreeNode.end(); }
	SPAWN_INST_NODE_ITER	SIN_BEGIN() { return m_mapSpawnInstNode.begin(); }
	SPAWN_INST_NODE_ITER	SIN_END()  { return m_mapSpawnInstNode.end(); }

	SPAWN_TREE_NODE_ITER	FindSpawnTree(RwInt32 _SortCodeIdx);
	SPAWN_INST_NODE_ITER	FindSpawnInst(RwInt32 _FieldIdx);

public:
	VOID	MoveSpawn(RwV3d& Pos, CSpawn* pSpawn, CNtlWorldFieldManager* pFMgr, eSPAWN_TYPE _CurSpawnType);

	CSpawn* GetSpawn(CNtlPLEntity* _pNtlPLEntity, CNtlWorldFieldManager* pFMgr);
	RwInt32	GetSpawnCnt(RwInt32 _FieldIdx);

	VOID	DeleteSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad);
	VOID	InsertSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad);

	RwBool	InsertSpawnTree(RwInt32 _SortCode, RwUInt32 _MdlGUID, BYTE _MdlLvl, string& _Name, string& _MdlName);
	CSpawn* InsertSpawnInst(CNtlPLEntity* pNtlPLEntity, CNtlWorldFieldManager* pFMgr, eSPAWN_TYPE eSpawnType);
	CSpawn* InsertSpawnInst(CSpawn* _pSpawn, CNtlWorldFieldManager* pFMgr);
	CSpawn* InsertSpawnLoad(CSpawn* _pSpawn, CNtlWorldFieldManager* pFMgr);
	VOID	DeleteSpawnInst(CSpawn* pSpawn, CNtlWorldFieldManager* pFMgr);
	VOID	DeleteSpawnInst(RwV3d& _Pos, CNtlWorldFieldManager* pFMgr);

	RwBool	FindSpawnTree(RwUInt32 uiMdlGUID, CSpawnTreeElement* pOutSpawnTreeElement);

	// only for createing or destroying entities in spawns
	VOID	CreateSpawnEntity(RwInt32 FieldIdx, CNtlWorldFieldManager* pFMgr);
	VOID	DestroySpawnEntity(RwInt32 FieldIdx);

	VOID	Load(FILE* _pFile, eSPAWN_TYPE _eSpawnType, CNtlWorldFieldManager* pMgr);
	VOID	Save(FILE* _pFile);
};
