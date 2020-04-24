#ifndef __SCRIPT_ALGO_DBOG_ACTION_ADDMOB_H__
#define __SCRIPT_ALGO_DBOG_ACTION_ADDMOB_H__

#include "ScriptAlgoAction.h"

class CNtlVector;
enum eNPC_NEST_TYPE;

class CWpsAlgoAction_AddMob : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_AddMob(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_AddMob();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID				m_spawnGroupId;

	TBLIDX						m_mobTblidx;

	BYTE						m_bySpawnCount;

	WORD						m_wSpawnTime;

	BYTE						m_bySpawn_Loc_Range;

	CNtlVector					m_vLoc;

	CNtlVector					m_vDir;

	bool						m_bUseLocVar;

	std::string					m_strLocVar;

	std::string					m_strDirVar;

	DWORD						m_dwPlayScriptNumber;

	DWORD						m_dwPlayScriptScene;

	int							m_battleScriptNumber;

	DWORD						m_dwBattleScriptScene;

	DWORD						m_dwAIScriptNumber;

	DWORD						m_dwAIScriptScene;

	QWORD						m_qwCharConditionFlag;

	BYTE						m_bySpawnFuncFlag;

	PARTYID						m_partyId;

	bool						m_bPartyLeader;

	TBLIDX						m_dropItem_ProbabilityTblidx;

	bool						m_bExecuterTargetFix;

	BYTE						m_byImmortalMode;

	eNPC_NEST_TYPE				m_eNestType;
	BYTE						m_byNestRange;

	TBLIDX						m_faintBuffIndex;
	BYTE						m_byFaintBuffApplyType;
	float						m_fFaintBuffRange;


};

#endif