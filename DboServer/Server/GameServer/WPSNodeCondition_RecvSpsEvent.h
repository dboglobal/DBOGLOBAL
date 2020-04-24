#ifndef __SERVER_WPSNODE_CONDITION_RECVSPSEVENT__
#define __SERVER_WPSNODE_CONDITION_RECVSPSEVENT__


#include "ControlScriptNodeCondition.h"


class CWPSNodeCondition_RecvSpsEvent : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_RecvSpsEvent(const char* lpszName);
	virtual ~CWPSNodeCondition_RecvSpsEvent();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

private:

	void						Init() {}

public:

	TBLIDX						m_targetTblidx;

	DWORD						m_eventID;

	DWORD						m_dwTime;

};


#endif
