#ifndef __SERVER_WPSNODE_ACTION_CCBD_EXEC_PATTERN__
#define __SERVER_WPSNODE_ACTION_CCBD_EXEC_PATTERN__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_CCBD_exec_pattern : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_CCBD_exec_pattern(const char* lpszName);
	virtual ~CWpsNodeAction_CCBD_exec_pattern();

	virtual bool				CheckParameter();


public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	std::string					m_strPatternList;

	std::vector<CNtlVector>		m_vecPatterns;

};


#endif
