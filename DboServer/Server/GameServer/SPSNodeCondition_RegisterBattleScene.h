#ifndef __SERVER_SPSONODE_CONDITION_REGISTERBATTLESCENE__
#define __SERVER_SPSONODE_CONDITION_REGISTERBATTLESCENE__


#include "ControlScriptNodeCondition.h"


class CSPSNodeCondition_RegisterBattleScene : public CControlScriptNodeCondition
{

public:
	CSPSNodeCondition_RegisterBattleScene(const char* lpszName);
	virtual ~CSPSNodeCondition_RegisterBattleScene();


	virtual sPARAMETER_INFO*	GetParameterMap() { return NULL; }

public:

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);


public:

	eSPS_BS_PLAY_TYPE			m_ePlayType;

};


#endif
