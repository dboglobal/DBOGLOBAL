#pragma once

#include "ntlworldcommon.h"
#include "spawn.h"
#include "spawnparty.h"
#include "pathlist.h"

#define dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_DELETE 0x00000001
#define dSPAWN_MERGE_TEMP_FLAG_SPAWN_POST_INSERT 0x00000002

class CNtlSpawnMergeContainer
{
public:
	typedef std::map<DWORD, CMergeSpawnParty*>					SPAWN_PARTY_MAP;
	typedef std::map<DWORD, CMergeSpawnParty*>::value_type		SPAWN_PARTY_VALUE;
	typedef std::map<DWORD, CMergeSpawnParty*>::iterator		SPAWN_PARTY_ITER;

	typedef std::vector<CMergeSpawn*>							SPAWN_VEC;						
	typedef std::vector<CMergeSpawn*>::iterator					SPAWN_VEC_ITER;

public:
	CNtlSpawnMergeContainer();
	~CNtlSpawnMergeContainer();

	RwBool				Load(const char* pFilename);
	RwBool				Save(const char* pFilename);
	void				Free();

	RwInt32				GetSpawnType() { return m_eSpawnType; } 

	RwBool				MergePathUnlinkedSpawn(RwBBox* pBBoxDst, RwBBox* pBBoxSrc, CNtlSpawnMergeContainer* pContainerSrc, FILE* pLogFile);
	RwBool				MergePathLinkedSpawn(std::vector<RwInt32>* pvecDeletePathIdxList, std::vector<RwInt32>* pvecInsertPathIdxList, CNtlSpawnMergeContainer* pContainerSrc, FILE* pLogFile);
	
	RwBool				GetPathIdxListFromPathLinkedSpawnParty(std::vector<RwInt32>* pvPathMergeList, FILE* pLogFile);

protected:
	RwBool				Load(FILE* pFile);
	RwBool				Save(FILE* pFile);

	RwBool				LoadSpawnType(FILE* pFile);
	RwBool				LoadSpawnParty(FILE* pFile);
	RwBool				LoadSpawnObject(FILE* pFile);

	RwBool				SaveSpawnType(FILE* pFile);
	RwBool				SaveSpawnParty(FILE* pFile);
	RwBool				SaveSpawnObject(FILE* pFile);

	CMergeSpawnParty*	GetSpawnParty(DWORD dwIdx);

	RwBool				InsertSpawnParty(CMergeSpawnParty* pSpawnParty);
	RwBool				RemoveSpawnParty(CMergeSpawnParty* pSpawnParty);

	RwBool				InsertSpawn(CMergeSpawn* pSpawn);
	RwBool				RemoveSpawn(CMergeSpawn* pSpawn);

	DWORD				IndexingSpawnParty(DWORD dwStartIdx = 0);

protected:
	RwInt32				m_eSpawnType;

	DWORD				m_dwPartyIdx;
	RwInt32				m_iPartyColorIdx;

	SPAWN_VEC			m_vecSpawn;
	SPAWN_PARTY_MAP		m_mapSpawnParty;	
};

class CNtlPathListMergeContainer
{
public:
	typedef std::map<RwInt32, CMergePathList*>				PATH_LIST_MAP;
	typedef std::map<RwInt32, CMergePathList*>::value_type	PATH_LIST_VALUE;
	typedef std::map<RwInt32, CMergePathList*>::iterator	PATH_LIST_ITER;

public:
	CNtlPathListMergeContainer();
	~CNtlPathListMergeContainer();

	RwBool			Load(const char* pFilename);
	RwBool			Save(const char* pFilename);
	void			Free();

	RwBool			MergePathList(std::vector<RwInt32>* pvecDeletePathIdxList, std::vector<RwInt32>* pvecInsertPathIdxList, CNtlPathListMergeContainer* pContainerSrc);


	PATH_LIST_MAP*	GetPathListMap()	{ return &m_mapPathList; }

protected:
	RwBool			Load(FILE* pFile);
	RwBool			Save(FILE* pFile);	

	CMergePathList*	GetPathList(RwInt32 iIdx);

	RwBool			Insert(CMergePathList* pPathList);
	RwBool			Remove(CMergePathList* pPathList);

protected:
	PATH_LIST_MAP	m_mapPathList;	
};