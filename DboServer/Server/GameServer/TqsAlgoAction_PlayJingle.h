#ifndef __TQS_ALGO_DBOG_ACTION_PLAY_JINGLE_H__
#define __TQS_ALGO_DBOG_ACTION_PLAY_JINGLE_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_PlayJingle : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_PlayJingle(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_PlayJingle();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bApplyFadeInOut;

	CNtlString					m_strName;

};

#endif