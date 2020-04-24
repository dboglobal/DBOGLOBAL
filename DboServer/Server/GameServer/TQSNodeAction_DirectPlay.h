#ifndef __SERVER_TQSONODE_ACTION_DIRECTPLAY__
#define __SERVER_TQSONODE_ACTION_DIRECTPLAY__


#include "ControlScriptNodeAction.h"
#include "NtlDirection.h"

class CTQSNodeAction_DirectPlay : public CControlScriptNodeAction
{

	enum eTYPE
	{
		DEFAULT_DIRECTION_EXCEPTION_TIME = 0x2BF20,
	};

public:
	CTQSNodeAction_DirectPlay(const char* lpszName);
	virtual ~CTQSNodeAction_DirectPlay();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	bool						m_bCanSkip;

	DWORD						m_dwWaitTime;

	bool						m_bEnding;

};


#endif
