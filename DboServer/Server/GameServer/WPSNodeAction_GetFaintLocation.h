#ifndef __SERVER_WPSNODE_ACTION_GETFAINTLOCATION__
#define __SERVER_WPSNODE_ACTION_GETFAINTLOCATION__


#include "ControlScriptNodeAction.h"

class CNtlVector;


class CWPSNodeAction_GetFaintLocation : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_GetFaintLocation(const char* lpszName);
	virtual ~CWPSNodeAction_GetFaintLocation();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();


public:

	eSSD_TARGET_TYPE			m_eTargetType;

	DWORD						m_dwTargetIndex;

	std::string					m_strLoc;
	std::string					m_strDir;
	std::string					m_strDefault;

	CNtlVector					m_cDefaultLoc;
	CNtlVector					m_cDefaultDir;

};


#endif
