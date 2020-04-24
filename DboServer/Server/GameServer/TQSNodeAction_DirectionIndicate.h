#ifndef __SERVER_TQSONODE_ACTION_DIRECTIONINDICATE__
#define __SERVER_TQSONODE_ACTION_DIRECTIONINDICATE__


#include "ControlScriptNodeAction.h"

class CNtlVector;
enum eDIRECTION_INDICATE_TYPE;

class CTQSNodeAction_DirectionIndicate : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_DirectionIndicate(const char* lpszName);
	virtual ~CTQSNodeAction_DirectionIndicate();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	bool						m_bIndicate;

	eDIRECTION_INDICATE_TYPE	m_eIndicateType;

	bool						m_bAutoHide;

	TBLIDX						m_tblidx;

	CNtlVector					m_vLoc;

};


#endif
