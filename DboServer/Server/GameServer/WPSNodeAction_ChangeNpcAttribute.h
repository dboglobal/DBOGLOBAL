#ifndef __SERVER_WPSNODE_ACTION_CHANGENPCATTRIBUTE__
#define __SERVER_WPSNODE_ACTION_CHANGENPCATTRIBUTE__


#include "ControlScriptNodeAction.h"

enum eRELATION_TYPE;

class CWPSNodeAction_ChangeNpcAttribute : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ChangeNpcAttribute(const char* lpszName);
	virtual ~CWPSNodeAction_ChangeNpcAttribute();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	TBLIDX						m_targetTblidx;

	bool						m_bApplyRelationPc;

	eRELATION_TYPE				m_eRelationPc;

};


#endif
