#ifndef __SERVER_SPSONODE_ACTION_CHANGEAIBITFLAG__
#define __SERVER_SPSONODE_ACTION_CHANGEAIBITFLAG__


#include "ControlScriptNodeAction.h"


class CSPSNodeAction_ChangeAiBitFlag : public CControlScriptNodeAction
{

public:

	enum eSCRIPTTYPE
	{
		eAIS,
		eSPS,
		INVALID_SCRIPTYPE = 0xFFFFFFFF,
	};

	CSPSNodeAction_ChangeAiBitFlag(const char* lpszName, eSCRIPTTYPE eScriptType = eAIS);
	virtual ~CSPSNodeAction_ChangeAiBitFlag();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eSCRIPTTYPE					m_eScriptType;
	DWORD						m_dwAi_Bit_Flag;
	DWORD						m_dwAi_Bit_Mask;

};


#endif
