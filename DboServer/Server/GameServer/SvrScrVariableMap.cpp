#include "stdafx.h"
#include "SvrScrVariableMap.h"



CSvrScrVariableMap::CSvrScrVariableMap()
{

}

CSvrScrVariableMap::~CSvrScrVariableMap()
{

}


eSSD_VARIABLE_RESULT CSvrScrVariableMap::Declare(std::string name, sSSD_VARIABLE& sVariable)
{
	if (GetVariable(name))
		return SSD_VARIABLE_RESULT_VARIABLE_IS_ALREADY_EXIST;

	m_mapVariable.insert(std::make_pair(name, sVariable));

	return SSD_VARIABLE_RESULT_SUCCESS;
}


eSSD_VARIABLE_RESULT CSvrScrVariableMap::Update(std::string name, sSSD_VARIABLE& sVariable)
{
	std::map<std::string, sSSD_VARIABLE>::iterator it = m_mapVariable.find(name);
	if (it == m_mapVariable.end())
		return SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST;

	it->second = sVariable;
	
	return it->second.eResult;
}



sSSD_VARIABLE* CSvrScrVariableMap::GetVariable(std::string name)
{
	std::map<std::string, sSSD_VARIABLE>::iterator it = m_mapVariable.find(name);
	if (it == m_mapVariable.end())
	{
		return NULL;
	}

	return &it->second;
}

