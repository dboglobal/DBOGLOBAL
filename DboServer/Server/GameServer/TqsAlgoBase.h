#ifndef __TQS_SCRIPT_ALGO_DBOG_BASE_H__
#define __TQS_SCRIPT_ALGO_DBOG_BASE_H__

#include "ScriptAlgoDefine.h"
#include "ControlStateComposite.h"

#include "TqsAlgoObject.h"


class CTqsAlgoBase : public CControlStateComposite
{

public:

	CTqsAlgoBase(CTqsAlgoObject* pOwner, eTQS_ALGOCONTROLID controlID, const char* lpszControlName) :
		m_pOwner(pOwner),
		CControlStateComposite(controlID, lpszControlName)
	{
	}

	virtual	~CTqsAlgoBase() {}

public:

	CTqsAlgoObject*		GetOwner() { return m_pOwner; }

private:

	CTqsAlgoObject*		m_pOwner;

};

#endif