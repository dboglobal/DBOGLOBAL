#ifndef __TQS_ALGO_DBOG_ACTION_SEND_TQS_EVENT_H__
#define __TQS_ALGO_DBOG_ACTION_SEND_TQS_EVENT_H__

#include "TqsAlgoAction_Base.h"

enum eOBJTYPE;

class CTqsAlgoAction_SendTqsEvent : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_SendTqsEvent(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_SendTqsEvent();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

};

#endif