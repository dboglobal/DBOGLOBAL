#ifndef __SERVER_WPSNODE_CONDITION_CHECKBATTLE__
#define __SERVER_WPSNODE_CONDITION_CHECKBATTLE__


#include "ControlScriptNodeCondition.h"

enum eOBJTYPE;

class CWPSNodeCondition_CheckBattle : public CControlScriptNodeCondition
{

public:
	CWPSNodeCondition_CheckBattle(const char* lpszName);
	virtual ~CWPSNodeCondition_CheckBattle();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	eOBJTYPE					m_eObjType;
	unsigned int				m_index;
	unsigned int				m_group;
	bool						m_bIsBattle;

};


#endif
