#ifndef __SERVER_SPSONODE_ACTION_PATHMOVE__
#define __SERVER_SPSONODE_ACTION_PATHMOVE__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_PathMove : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_PathMove(const char* lpszName);
	virtual ~CSPSNodeAction_PathMove();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);
	
public:

	TBLIDX						m_pathTblidx;

	bool						m_bRunMode;

	float						m_fPointRadius;

};


#endif
