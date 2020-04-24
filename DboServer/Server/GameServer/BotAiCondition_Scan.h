#ifndef __AI_DBOG_BOTCONDITION_SCAN_H__
#define __AI_DBOG_BOTCONDITION_SCAN_H__

#include "BotAiCondition.h"

class CBotAiCondition_Scan : public CBotAiCondition
{

public:
	CBotAiCondition_Scan(CNpc* pBot);
	virtual	~CBotAiCondition_Scan();


public:

	virtual bool AttachControlScriptNode(CControlScriptNode* pControlScriptNode);
	virtual void CopyTo(CControlState* pTo);

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE m_eObjType;
	TBLIDX m_tblidx;
	WORD m_wRange;
	DWORD m_dwTime;
	DWORD m_scriptnodeID;

};

#endif