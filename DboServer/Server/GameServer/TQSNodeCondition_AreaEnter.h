#ifndef __SERVER_TQSONODE_CONDITION_AREAENTER__
#define __SERVER_TQSONODE_CONDITION_AREAENTER__


#include "ControlScriptNodeCondition.h"

class CNtlVector;

class CTQSNodeCondition_AreaEnter : public CControlScriptNodeCondition
{

public:
	CTQSNodeCondition_AreaEnter(const char* lpszName);
	virtual ~CTQSNodeCondition_AreaEnter();


	virtual sPARAMETER_INFO*	GetParameterMap();

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

	DWORD						m_dwTimeLimit;

	bool						m_bTmqFail;

};


#endif
