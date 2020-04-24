#ifndef __SERVER_WPSNODE_ACTION_DIRECTIONINDICATE__
#define __SERVER_WPSNODE_ACTION_DIRECTIONINDICATE__


#include "ControlScriptNodeAction.h"

enum eDIRECTION_INDICATE_TYPE;
class CNtlVector;

class CWPSNodeAction_Direction_Indicate : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_Direction_Indicate(const char* lpszName);
	virtual ~CWPSNodeAction_Direction_Indicate();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	bool						m_bIndicate;

	eDIRECTION_INDICATE_TYPE	m_eIndicateType;

	bool						m_bAutoHide;

	TBLIDX						m_tblidx;

	CNtlVector					m_vLoc;

};


#endif
