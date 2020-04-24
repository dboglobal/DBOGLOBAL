#ifndef __SERVER_SPSONODE_ACTION_TARGETFIXATTACK__
#define __SERVER_SPSONODE_ACTION_TARGETFIXATTACK__


#include "ControlScriptNodeAction.h"
enum eOBJTYPE;

class CSPSNodeAction_TargetFixAttack : public CControlScriptNodeAction
{

public:
	CSPSNodeAction_TargetFixAttack(const char* lpszName);
	virtual ~CSPSNodeAction_TargetFixAttack();

	enum eENDTYPE
	{
		ePERCENT_TARGET_LP,
		INVALID_ENDTYPE = 0xFF,
	};

	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eOBJTYPE					m_eObjType;

	TBLIDX						m_tblidxBot;

	float						m_fScanRange;

	float						m_fEndValue_Percent_Target_LP;

	eENDTYPE					m_eEndType;

};


#endif
