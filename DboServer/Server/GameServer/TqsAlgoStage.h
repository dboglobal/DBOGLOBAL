#ifndef __TQS_SCRIPT_ALGO_DBOG_STAGE_H__
#define __TQS_SCRIPT_ALGO_DBOG_STAGE_H__

#include "TqsAlgoBase.h"


class CTqsAlgoStage : public CTqsAlgoBase
{

public:

	CTqsAlgoStage(BYTE byStageNumber, CTqsAlgoObject* pOwner, eTQS_ALGOCONTROLID controlID, const char* lpszControlName);

	virtual	~CTqsAlgoStage();

public:

	virtual void		OnEnter();

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);


public:

	BYTE				GetStageNumber() { return m_byStageNumber; }

private:

	BYTE				m_byStageNumber;

};

#endif