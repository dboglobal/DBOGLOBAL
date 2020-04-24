#ifndef __SERVER_SPSONODE_ACTION_POINTMOVE__
#define __SERVER_SPSONODE_ACTION_POINTMOVE__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CSPSNodeAction_PointMove : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_PointMove(const char* lpszName);
	virtual ~CSPSNodeAction_PointMove();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	CNtlVector					m_vDestLoc;

	CNtlVector					m_vDestDir;

	bool						m_bRunMode;

	float						m_fMoveSpeed;
};


#endif
