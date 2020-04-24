#ifndef __SCRIPT_ALGO_DBOG_BASE_H__
#define __SCRIPT_ALGO_DBOG_BASE_H__

#include "ScriptAlgoDefine.h"
#include "ControlStateComposite.h"

#include "WpsAlgoObject.h"


class CScriptAlgoBase : public CControlStateComposite
{

public:


	CScriptAlgoBase(CWpsAlgoObject* pOwner, eSCRIPTALGOCONTROLID controlID, const char* lpszControlName) :
		m_pOwner(pOwner),
		CControlStateComposite(controlID, lpszControlName)
	{
	}

	virtual	~CScriptAlgoBase() {}


public:

	CWpsAlgoObject*		GetOwner() { return m_pOwner; }

private:

	CWpsAlgoObject*		m_pOwner;

};

#endif