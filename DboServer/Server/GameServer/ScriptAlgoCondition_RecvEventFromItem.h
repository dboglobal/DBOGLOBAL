#ifndef __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMITEM_H__
#define __SCRIPT_ALGO_DBOG_CONDITION_RECVEVENTFROMITEM_H__

#include "ScriptAlgoCondition.h"


class CWpsAlgoCondition_RecvEventFromItem : public CWpsAlgoCondition
{

public:

	CWpsAlgoCondition_RecvEventFromItem(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoCondition_RecvEventFromItem();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_itemTblidx;

};

#endif