#pragma once


using std::vector;
using std::map;
#include <vector>
#include <map>


class CSpawn;
class CSpawnParty;
class CSpawnPartyPath;

class CMergeSpawn;

class CSpawnPartyContainer
{
public:
	CSpawnPartyContainer(void) {;}
	virtual ~CSpawnPartyContainer(void) {;}

	typedef std::map<DWORD, CSpawnParty*>				SPAWN_PARTY_MAP;
	typedef std::map<DWORD, CSpawnParty*>::value_type	SPAWN_PARTY_VALUE;
	typedef std::map<DWORD, CSpawnParty*>::iterator		SPAWN_PARTY_ITER;

private:
	static DWORD	m_PartyIdx;
	static RwInt32	m_PartyColorIdx;
	static RwRGBA	m_PartyColor[7];

public:
	static SPAWN_PARTY_MAP	m_mapSpawnParty;

public:
	static	DWORD			GetNextPartyIdx() { return ++m_PartyIdx; }
	static	RwRGBA			GetNextPartyColor();
	static	CSpawnParty*	GetSpawnParty(DWORD Index);

public:
	static	VOID Save(FILE* _pFile);
	static	VOID Load(FILE* _pFile);
	static	VOID Create();
	static	VOID Destroy();
};

class CSpawnParty
{
public:
	CSpawnParty(DWORD _PartyIdx);
	CSpawnParty(void);
	virtual ~CSpawnParty(void);

protected:
	RwRGBA				m_RGBA;
	CSpawn*				m_pLeaderSpawn;
	RwBool				m_IsFollow;
	vector<CSpawn*>		m_vecSpawn;
	CSpawnPartyPath*	m_pPath;
	DWORD				m_CurPartyIdx;

public:
	VOID	InsertSpawn(CSpawn* _pSpawn);
	RwInt32	DeleteSpawn(CSpawn* _pSpawn);
	RwInt32	GetPartyMemberCnt() { return m_vecSpawn.size(); }
	CSpawn*	GetLeader() const { return m_pLeaderSpawn; }
	RwBool	GetFollow() { return m_IsFollow; }
	VOID	SetLeader(CSpawn* _pSpawn, RwBool _IsLeader);
	VOID	SetFollow(RwBool _IsFollow);
	VOID	SetColor(RwRGBA RGBA) { m_RGBA = RGBA; }
	RwRGBA	GetColor() { return m_RGBA; }
	RwInt32 GetPartyIdx();
	RwBool	RefreshFollowDist4AllMembers();
	VOID	Save(FILE* _pFile);
};

// CSpawnParty를 상속 받으려 했으나 생성자에서 Singleton을 사용하기 때문에 일단은 따로 만든다.
// 나중에 시간이 있을 때 합치는 방법을 검토해 본다.
class CMergeSpawnParty 
{
public:
	CMergeSpawnParty();
	~CMergeSpawnParty();

	RwBool							Load(FILE* pFile);
	RwBool							Save(FILE* pFile);

public:
	RwRGBA							rgba;
	RwBool							bIsFollow;
	DWORD							dwPartyIdx;
	DWORD							dwCurPartyIdx;


	std::vector<CMergeSpawn*>		vecSpawn;
};