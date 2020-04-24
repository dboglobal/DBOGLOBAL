#pragma once


#include "SpawnMacro.h"

using std::vector;
using std::string;
#include <vector>
#include <string>


class CSpawnParty;
class CNtlPLEntity;
class CNtlWorldFieldManager;


enum eSPAWN_TYPE
{
	eSPAWN_NPC = 0,
	eSPAWN_MOB,
};

enum eSPAWN_ARROW_TYPE
{
	eSAT_NEST_DEFAULT = 0,
	eSAT_NEST_SELECT,
	eSAT_NEST_DELETE,
};


class CSpawn
{
public:
	CSpawn();
	virtual ~CSpawn(void);

protected:
	// Nest prop.
	RwInt32	m_NestRange;
	RwInt32	m_NestSpawnCnt;
	RwInt32	m_NestPathIndex;

	// Group Index
	DWORD	m_GroupIdx;

	DWORD	m_PlayScriptIdx;
	DWORD	m_PlayScriptSceneIdx;
	
	DWORD	m_AIScriptIdx;
	DWORD	m_AIScriptSceneIdx;

public:
	// Main spawn obj.
	CNtlPLEntity*	m_pNtlPLEntity;

	// Spawn nest members
	vector<CNtlPLEntity*>	m_vecNtlPLEntity;

protected:
	// Arrow UI
	CNtlPLEntity* m_pNestArrowEntity;
	CNtlPLEntity* m_pSelectedArrowEntity;
	CNtlPLEntity* m_pPlayerNameEntity;

	RwV3d			m_FollowDist;
	CSpawnParty*	m_pSpawnParty;
	CSpawnMacro		m_SpawnMacro;

	RwBool m_VisibleFlag;

public:
	// model name
	string	m_MdlName;

	// display name
	string	m_Name;

	RwReal	m_S;
	RwV3d	m_R;
	RwV3d	m_T;

	DWORD m_MdlGUID;

public:
	virtual VOID Update();

public:
	VOID			Move(RwV3d& Pos, CNtlWorldFieldManager* pMgr);
	VOID			Rotate(RwReal RotDelta);
	RwBool			Compare(CNtlPLEntity* _pNtlPLEntity);

	VOID			SetFollowDist(RwV3d _FollowDist) { m_FollowDist = _FollowDist; }
	VOID			SetSpawnMacro(CSpawnMacro _SpawnMacro) { m_SpawnMacro = _SpawnMacro; }
	VOID			SetSpawnName(string& _Name);
	VOID			SetNtlPLEntity(CNtlPLEntity* _NtlPLEntity);
	VOID			SetNestArrow(eSPAWN_ARROW_TYPE _eSpawnArrowType);
	VOID			SetNestMembers(RwInt32 _Cnt, CNtlWorldFieldManager* pMgr);
	VOID			SetPartyColor(RwRGBA& RGBA);
	VOID			SetPartyLeader(RwBool IsLeader);
	VOID			SetPartyFollow(RwBool IsFollow);
	VOID			SetVisible(RwBool IsVisble);
	VOID			SetPathIndex(RwInt32 _PathIndex) { m_NestPathIndex = _PathIndex; }
	VOID			SetGroupIdx(DWORD& _Idx) { m_GroupIdx = _Idx; }
	VOID			SetPlayScriptIdx(DWORD& _Idx) { m_PlayScriptIdx = _Idx; }
	VOID			SetPlayScriptSceneIdx(DWORD& _Idx) { m_PlayScriptSceneIdx = _Idx; }
	VOID			SetAIScriptIdx(DWORD& _Idx) { m_AIScriptIdx = _Idx; }
	VOID			SetAIScriptSceneIdx(DWORD& _Idx) { m_AIScriptSceneIdx = _Idx; }

	RwV3d&			GetFollowDist()	{ return m_FollowDist; }
	RwInt32&		GetNestMemberCnt() { return m_NestSpawnCnt; }
	RwInt32&		GetNestRange() { return m_NestRange; }	
	CSpawnMacro&	GetSpawnMacro() { return m_SpawnMacro; }
	CNtlPLEntity*	GetNtlPLEntity() const { return m_pNtlPLEntity; }
	CSpawnParty*	GetSpawnParty() const { return m_pSpawnParty; }
	RwInt32			GetPathIndex() const { return m_NestPathIndex; }
	DWORD			GetGroupIdx() { return m_GroupIdx; }
	DWORD			GetPlayScriptIdx() { return m_PlayScriptIdx; }
	DWORD			GetPlayScriptSceneIdx() { return m_PlayScriptSceneIdx; }
	DWORD			GetAIScriptIdx() { return m_AIScriptIdx; }
	DWORD			GetAIScriptSceneIdx() { return m_AIScriptSceneIdx; }

	VOID	CreateEntity(CNtlWorldFieldManager* pMgr);
	VOID	DestroyEntity();
	VOID	CreateParty(CSpawnParty* pSpawnParty);
	VOID	DestroyParty();
	VOID	RefreshNestMembers(CNtlWorldFieldManager* pMgr);
	VOID	RefreshNameBox();
	VOID	Save(FILE* _pFile);
	VOID	Load(FILE* _pFile);
	VOID	RenderText();


	virtual	eSPAWN_TYPE GetSpawnType() = 0;


	// VOID* GetSpawnTblDat() = 0
};

// 내부에서 싱글턴 사용으로 사용에 난해할 수 있어 일단은 따로 만든다.
// 나중에 시간에 될 때 전부 합치는 방안을 모색해 보자.
class CMergeSpawn
{
public:
	CMergeSpawn();
	~CMergeSpawn();

	RwBool					Load(FILE* pFile);
	RwBool					Save(FILE* pFile);

public:
	DWORD					dwMdlGUID;
	RwInt32					iNestRange;
	RwInt32					iNestSpawnCnt;
	RwInt32					iNestPathIndex;

	RwInt32					iPartyIndex;
	RwBool					bPartyLeader;

	RwV3d					vFollowDist;

	CSpawnMacro				SpawnMacro;

	RwInt32					iLenModelName;
	std::string				strModelName;

	RwInt32					iLenDisplayName;
	std::string				strDisplayName;

	RwReal					fS;
	RwV3d					vR;
	RwV3d					vT;

	DWORD					dwGroupIdx;
	DWORD					dwPlayScriptIdx;
	DWORD					dwPlayScriptSceneIdx;

	DWORD					dwAIScriptIdx;
	DWORD					dwAIScriptSceneIdx;

	RwUInt32				uiMergeTemp;
};