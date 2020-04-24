#ifndef __SERVER_WPSNODE_CONDITION_CHECKLP__
#define __SERVER_WPSNODE_CONDITION_CHECKLP__


#include "ControlScriptNodeCondition.h"

enum eOBJTYPE;

class CWPSNodeCondition_CheckLP : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_CheckLP(const char* lpszName);
	virtual ~CWPSNodeCondition_CheckLP();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

private:

	void						Init() {}

public:

	eOBJTYPE					m_eObjType;
	TBLIDX						m_index;
	SPAWNGROUPID				m_group;
	BYTE						m_byPercent;

};


#endif
