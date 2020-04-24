#ifndef __SCRIPT_ALGO_DBOG_ACTION_CHANGECHARATTRIBUTE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_CHANGECHARATTRIBUTE_H__

#include "ScriptAlgoAction.h"

enum eOBJTYPE;

class CWpsAlgoAction_ChangeCharAttribute : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_ChangeCharAttribute(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_ChangeCharAttribute();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eTargetType;

	DWORD						m_dwTargetIndex;

	bool						m_bUseNickname;

	TBLIDX						m_nicknameTblidx;

	BYTE						m_bySizeRate;

	BYTE						m_byImmortalMode;

	BYTE						m_byLpBlockRate;


};

#endif