#ifndef __SCRIPT_ALGO_DBOG_CONDITION_AREAENTER_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_AREAENTER_H__

#include "ScriptAlgoCondition.h"

class CWpsAlgoCondition_AreaEnter : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_AreaEnter(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_AreaEnter();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	CNtlVector					m_vStart;

	CNtlVector					m_vEnd;

	CNtlVector					m_vOrigin;

	float						m_fRadius;

	bool						m_bAllMember;

};

#endif