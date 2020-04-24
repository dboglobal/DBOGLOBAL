#ifndef __SERVER_SRV_SCR_VARIABLE_MAP__
#define __SERVER_SRV_SCR_VARIABLE_MAP__


#include "ControlScriptGlobal.h"
#include <map>

class CSvrScrVariableMap
{

public:
	CSvrScrVariableMap();
	virtual ~CSvrScrVariableMap();


public:

	bool								Create(){ return true; }

	eSSD_VARIABLE_RESULT				Declare(std::string name, sSSD_VARIABLE& sVariable);

	eSSD_VARIABLE_RESULT				Update(std::string name, sSSD_VARIABLE& sVariable);

	sSSD_VARIABLE*						GetVariable(std::string name);

	int									GetVariableCount() { return (int)m_mapVariable.size(); }


protected:

	std::map<std::string, sSSD_VARIABLE> m_mapVariable;

};


#endif