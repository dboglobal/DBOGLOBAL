#ifndef __SERVER_SRV_SCR_VARIABLE_CONTAINER__
#define __SERVER_SRV_SCR_VARIABLE_CONTAINER__

#include "ControlScriptGlobal.h"

class CControlState;
class CControlScriptNode;
class CSvrScrVariableMap;

class CSvrScrVariableContainer
{

	enum eCONTAINER_TYPE
	{
		CONTAINER_VARIABLEMAP,
		CONTAINER_SCRIPTNODE,
		CONTAINER_CONTROLSTATE,
	};

public:

	CSvrScrVariableContainer(CControlState* pControlSate);
	CSvrScrVariableContainer(CControlScriptNode* pScriptNode);
	virtual ~CSvrScrVariableContainer();


public:

	sSSD_VARIABLE*			GetVariable(std::string name);

	eSSD_VARIABLE_RESULT	UpdateVariable(std::string name, sSSD_VARIABLE& sVariable);


private:

	eCONTAINER_TYPE			m_eContainerType;

	CSvrScrVariableMap*		m_pVarialbeMap;

	CControlScriptNode*		m_pScriptNode;

	CControlState*			m_pControlState;
};


#endif