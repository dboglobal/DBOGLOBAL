#ifndef __SCRIPT_ALGO_DBOG_ACTION_ADDMOBGROUP_H__
#define __SCRIPT_ALGO_DBOG_ACTION_ADDMOBGROUP_H__

#include "ScriptAlgoAction.h"

enum eNPC_NEST_TYPE;

class CWpsAlgoAction_AddMobGroup : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_AddMobGroup(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_AddMobGroup();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	SPAWNGROUPID					m_spawnGroupId;

	CNtlVector						m_vLoc;
	CNtlVector						m_vDir;

	DWORD							m_dwPlayScriptNumber;
	DWORD							m_dwPlayScriptScene;
	DWORD							m_dwAIScriptNumber;
	DWORD							m_dwAIScriptScene;
	QWORD							m_qwCharConditionFlag;
	BYTE							m_bySpawnFuncFlag;

	int								m_nBindMobList;
	std::map<TBLIDX, sSSD_MOB_PAIR> m_mapBindMob;

	TBLIDX							m_dropItem_ProbabilityTblidx;

	bool							m_bPartyLeader;
	PARTYID							m_partyId;

	bool							m_bExecuterTargetFix;

	BYTE							m_byImmortalMode;

	eNPC_NEST_TYPE					m_eNestType;

	BYTE							m_byNestRange;

	TBLIDX							m_faintBuffIndex;

	BYTE							m_byFaintBuffApplyType;

	float							m_fFaintBuffRange;


};

#endif