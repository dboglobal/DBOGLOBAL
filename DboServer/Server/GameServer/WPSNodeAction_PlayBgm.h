#ifndef __SERVER_WPSNODE_ACTION_PLAYBGM__
#define __SERVER_WPSNODE_ACTION_PLAYBGM__


#include "ControlScriptNodeAction.h"

class CNtlString;

class CWPSNodeAction_PlayBgm : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_PlayBgm(const char* lpszName);
	virtual ~CWPSNodeAction_PlayBgm();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();


public:

	bool						m_bPlay;

	CNtlString					m_strName;

	bool						m_bLoop;

	bool						m_bPrevBgmFadeOut;

	DWORD						m_dwDelay;

};


#endif
