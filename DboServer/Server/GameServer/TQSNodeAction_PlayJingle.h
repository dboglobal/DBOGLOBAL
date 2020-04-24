#ifndef __SERVER_TQSONODE_ACTION_PLAYJINGLE__
#define __SERVER_TQSONODE_ACTION_PLAYJINGLE__


#include "ControlScriptNodeAction.h"


class CTQSNodeAction_PlayJingle : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_PlayJingle(const char* lpszName);
	virtual ~CTQSNodeAction_PlayJingle();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	bool						m_bApplyFadeInOut;

	CNtlString					m_strName;

};


#endif
