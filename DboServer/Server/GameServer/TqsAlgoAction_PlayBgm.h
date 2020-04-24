#ifndef __TQS_ALGO_DBOG_ACTION_PLAY_BGM_H__
#define __TQS_ALGO_DBOG_ACTION_PLAY_BGM_H__

#include "TqsAlgoAction_Base.h"


class CTqsAlgoAction_PlayBgm : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_PlayBgm(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_PlayBgm();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bPlay;

	CNtlString					m_strName;

	bool						m_bLoop;

	bool						m_bPrevBgmFadeOut;

	DWORD						m_dwDelay;

};

#endif