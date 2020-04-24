#ifndef __SCRIPT_ALGO_DBOG_ACTION_GETLOCATION_H__
#define __SCRIPT_ALGO_DBOG_ACTION_GETLOCATION_H__

#include "ScriptAlgoAction.h"

class CNtlVector;

class CWpsAlgoAction_GetLocation : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_GetLocation(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_GetLocation();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eSSD_TARGET_TYPE			m_eTargetType;

	DWORD						m_dwTargetIndex;

	std::string					m_strLoc;
	std::string					m_strDir;

	CNtlVector					m_cDefaultLoc;
	CNtlVector					m_cDefaultDir;


};

#endif