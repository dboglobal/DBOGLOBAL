#ifndef __SERVER_WPSNODE_CONDITION_AREAENTER__
#define __SERVER_WPSNODE_CONDITION_AREAENTER__


#include "ControlScriptNodeCondition.h"

class CNtlVector;

class CWPSNodeCondition_AreaEnter : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_AreaEnter(const char* lpszName);
	virtual ~CWPSNodeCondition_AreaEnter();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	CNtlVector					m_vStart;

	CNtlVector					m_vEnd;

	CNtlVector					m_vOrigin;

	float						m_fRadius;

	bool						m_bAllMember;

};


#endif
