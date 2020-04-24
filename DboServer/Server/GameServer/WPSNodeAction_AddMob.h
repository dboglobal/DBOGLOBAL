#ifndef __SERVER_WPSNODE_ACTION_ADDMOB__
#define __SERVER_WPSNODE_ACTION_ADDMOB__


#include "ControlScriptNodeAction.h"

class CNtlVector;
enum eNPC_NEST_TYPE;

class CWPSNodeAction_AddMob : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_AddMob(const char* lpszName);
	virtual ~CWPSNodeAction_AddMob();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

private:

	void						Init();

public:

	SPAWNGROUPID				m_spawnGroupId;

	TBLIDX						m_mobTblidx;

	BYTE						m_bySpawnCount;

	WORD						m_wSpawnTime;

	BYTE						m_bySpawn_Loc_Range;

	bool						m_bUseLocVar;

	std::string					m_strLocVar;

	std::string					m_strDirVar;

	CNtlVector					m_vLoc;

	CNtlVector					m_vDir;

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

	BYTE						m_byNestRange;
	eNPC_NEST_TYPE				m_eNestType;

	TBLIDX						m_faintBuffIndex;

	BYTE						m_byFaintBuffApplyType;

	float						m_fFaintBuffRange;

};


#endif
