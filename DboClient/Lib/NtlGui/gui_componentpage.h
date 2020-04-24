#ifndef __GUI_COMPONENTPAGE_H__
#define __GUI_COMPONENTPAGE_H__

#include "gui_define.h"
#include "gui_guifilepaser.h"

START_GUI

class CComponentPage
{
//!Construction:
public:

	CComponentPage();
	CComponentPage(const std::string &strFileName);
	~CComponentPage();

//!Attributes:
public:
	
//!Operations:
public:

	bool CreateComponents(CComponent *pParent,CSurfaceManager *pSurfaceManager);
	VOID DestroyComponents(VOID);
	
	CComponent* GetComponent(const std::string& strComponentID) const;

	bool IsCreate(VOID) const;
		
//!Implementation:
private:

	//: component manager script data file parsing
	VOID Parse(VOID);
	VOID CreateComponents(const CGUIFileParser::stlCOMPONENT_SORT_MAP &stlSortMap);
	VOID DestroyComponets(const CGUIFileParser::stlCOMPONENT_SORT_MAP &stlSortMap);
		
private:

	bool m_bCreate;
	std::string m_strFileName;
	CGUIFileParser *m_pPaser;
	CComponent *m_pParent;
	CSurfaceManager *m_pSurfaceManager;
};

END_GUI


#endif