#ifndef __SCRIPT_ALGO_DBOG_ACTION_CHANGENPCATTRIBUTE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CHANGENPCATTRIBUTE_H__

#include "ScriptAlgoAction.h"

enum eRELATION_TYPE;

class CWpsAlgoAction_ChangeNpcAttribute : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ChangeNpcAttribute(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ChangeNpcAttribute();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_targetTblidx;

	bool						m_bApplyRelationPc;

	eRELATION_TYPE				m_eRelationPc;


};

#endif