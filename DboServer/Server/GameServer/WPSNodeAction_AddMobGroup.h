#ifndef __SERVER_WPSNODE_ACTION_ADDMOBGROUP__
#define __SERVER_WPSNODE_ACTION_ADDMOBGROUP__


#include "ControlScriptNodeAction.h"
#include <map>

class CNtlVector;
enum eNPC_NEST_TYPE;

class CWPSNodeAction_AddMobGroup : public CControlScriptNodeAction
{

	enum eBIND_STR_STATE
	{
		BIND_STR_STATE_START,
		BIND_STR_STATE_LEFT,
		BIND_STR_STATE_EQUAL,
		BIND_STR_STATE_RIGHT,
		BIND_STR_STATE_COMMA,
		INVALID_BIND_STR_STATE = 0xFF,
	};

public:
	CWPSNodeAction_AddMobGroup(const char* lpszName);
	virtual ~CWPSNodeAction_AddMobGroup();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

private:

	void						Init();

	void						ReadBindList();

public:

	std::string					m_strBindMob;

	SPAWNGROUPID				m_spawnGroupId;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	DWORD						m_dwPlayScriptNumber;
	DWORD						m_dwPlayScriptScene;
	DWORD						m_dwAIScriptNumber;
	DWORD						m_dwAIScriptScene;

	QWORD						m_qwCharConditionFlag;

	BYTE						m_bySpawnFuncFlag;

	int								m_nBindMobList;
	std::map<TBLIDX, sSSD_MOB_PAIR> m_mapBindMob;

	TBLIDX						m_dropItem_ProbabilityTblidx;

	bool						m_bPartyLeader;
	PARTYID						m_partyId;

	bool						m_bExecuterTargetFix;

	BYTE						m_byImmortalMode;

	eNPC_NEST_TYPE				m_eNestType;

	BYTE						m_byNestRange;

	TBLIDX						m_faintBuffIndex;

	BYTE						m_byFaintBuffApplyType;

	float						m_fFaintBuffRange;

};


#endif
