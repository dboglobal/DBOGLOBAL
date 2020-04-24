#ifndef __SCRIPT_ALGO_DBOG_STATE_H__
#define __SCRIPT_ALGO_DBOG_STATE_H__

#include "ScriptAlgoBase.h"

class CScriptAlgoStage : public CScriptAlgoBase
{

public:
	CScriptAlgoStage(BYTE byStageNumber, CWpsAlgoObject* pOwner, eSCRIPTALGOCONTROLID controlID, const char* lpszControlName) :
		m_byStageNumber(byStageNumber),
		CScriptAlgoBase(pOwner, controlID, lpszControlName)
	{
	}

	virtual	~CScriptAlgoStage() {}


public:

	BYTE				GetStageNumber() { return m_byStageNumber; }

private:

	BYTE				m_byStageNumber;

};

#endif