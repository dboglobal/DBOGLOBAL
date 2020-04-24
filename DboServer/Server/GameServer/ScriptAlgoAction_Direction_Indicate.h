#ifndef __SCRIPT_ALGO_DBOG_ACTION_DIRECTION_INDICATION_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DIRECTION_INDICATION_H__

#include "ScriptAlgoAction.h"

enum eDIRECTION_INDICATE_TYPE;
class CNtlVector;


class CWpsAlgoAction_Direction_Indicate : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_Direction_Indicate(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_Direction_Indicate();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bIndicate;

	eDIRECTION_INDICATE_TYPE	m_eIndicateType;

	bool						m_bAutoHide;

	TBLIDX						m_tblidx;

	CNtlVector					m_vLoc;

};

#endif