#ifndef __SERVER_TQSONODE_ACTION_PLAYBGM__
#define __SERVER_TQSONODE_ACTION_PLAYBGM__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_PlayBgm : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_PlayBgm(const char* lpszName);
	virtual ~CTQSNodeAction_PlayBgm();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	bool						m_bPlay;

	CNtlString					m_strName;

	bool						m_bLoop;

	bool						m_bPrevBgmFadeOut;

	DWORD						m_dwDelay;

};


#endif
