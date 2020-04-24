#ifndef __SERVER_AISONODE_ACTION_ADDMOB__
#define __SERVER_AISONODE_ACTION_ADDMOB__


#include "ControlScriptNodeAction.h"
#include "NtlVector.h"

class CAISNodeAction_AddMob : public CControlScriptNodeAction
{

public:
	CAISNodeAction_AddMob(const char* lpszName);
	virtual ~CAISNodeAction_AddMob();

	enum eADDMOBTYPE
	{
		DIRECT,
		RECALL,
		ADDMOBTYPE_COUNT,
		INVALID_ADDMOBTYPE = 0xFFFFFFFF,
	};

	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

private:

	void						Init();
	

public:

	TBLIDX						m_mobTblidx;

	CNtlVector					m_vLoc;
	CNtlVector					m_vDir;

	BYTE						m_byPartyNumber;

	eADDMOBTYPE					m_eAddMobType;

};


#endif
