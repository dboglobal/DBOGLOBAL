#ifndef __TQS_ALGO_DBOG_ACTION_PROGRESS_MESSAGE_H__
#define __TQS_ALGO_DBOG_ACTION_PROGRESS_MESSAGE_H__

#include "TqsAlgoAction_Base.h"

enum ePROGRESS_MESSAGE_TYPE;

class CTqsAlgoAction_ProgressMessage : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_ProgressMessage(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_ProgressMessage();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	ePROGRESS_MESSAGE_TYPE		m_eProgressType;

	eTIMEQUEST_PROGRESS_MESSAGE_TYPE	m_eMessageType;

	BYTE						m_byMessageValue;

};

#endif