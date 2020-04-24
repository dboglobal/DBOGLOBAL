#ifndef __TQS_ALGO_DBOG_ACTION_DIRECTPLAY_H__
#define __TQS_ALGO_DBOG_ACTION_DIRECTPLAY_H__

#include "TqsAlgoAction_Base.h"

enum eDIRECTION_TYPE;


class CTqsAlgoAction_DirectPlay : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_DirectPlay(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_DirectPlay();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual void				OnEnter();

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	bool						m_bCanSkip;

	DWORD						m_dwWaitTime;

	bool						m_bEnding;

};

#endif