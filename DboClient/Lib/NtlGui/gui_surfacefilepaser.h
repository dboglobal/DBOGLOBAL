#ifndef __GUI_SURFACEFILEPARSER_H__
#define __GUI_SURFACEFILEPARSER_H__

#include "gui_define.h"
#include "gui_componentoption.h"

START_GUI

class CComponentType;

class CSurfaceFileParser
{
public:

	CSurfaceFileParser(const std::string &filename);

	struct ComponentInfo
	{
	public:
		ComponentInfo()
		{
			type = "";
		}

		std::string name,type;
		CComponentOptions Options;
		
		std::list<ComponentInfo *> m_stlChildren;
	};

	typedef std::map<std::string,ComponentInfo> stlCOMPONENT_MAP;
	typedef std::multimap<int, ComponentInfo *> stlCOMPONENT_SORT_MAP;
	typedef std::pair<std::string, std::string> stlVARIABLE_PAIR;
	typedef std::list<stlVARIABLE_PAIR> stlGLOBAL_LIST;

	stlCOMPONENT_MAP& GetComponents(VOID) { return m_stlCompMap; }
	stlCOMPONENT_SORT_MAP& GetSortedComponents(VOID) { return m_stlCompSortMap; }
	stlGLOBAL_LIST &Globals() { return m_stlGlobals; }
	
	VOID ReadError(std::string& strError);

private:

	std::string m_strError;

private:

	std::string ConcatenateStringlist(std::list<std::string> &expression);
	std::string EvaluateExpression(std::list<std::string> &expression);
	
	stlCOMPONENT_MAP						 m_stlCompMap;
	stlCOMPONENT_SORT_MAP				 m_stlCompSortMap;
	std::list<stlVARIABLE_PAIR>  m_stlGlobals;

};

END_GUI

#endif