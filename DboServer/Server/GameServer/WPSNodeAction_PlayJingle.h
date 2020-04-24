#ifndef __SERVER_WPSNODE_ACTION_PLAYJINGLE__
#define __SERVER_WPSNODE_ACTION_PLAYJINGLE__


#include "ControlScriptNodeAction.h"

class CNtlString;

class CWPSNodeAction_PlayJingle : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_PlayJingle(const char* lpszName);
	virtual ~CWPSNodeAction_PlayJingle();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();


public:

	bool						m_bApplyFadeInOut;

	CNtlString					m_strName;

};


#endif
