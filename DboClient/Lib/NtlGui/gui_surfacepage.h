#ifndef __GUI_SURFACEPAGE_H__
#define __GUI_SURFACEPAGE_H__

#include "gui_define.h"
#include "gui_surfacefilepaser.h"
#include "gui_surface.h"

START_GUI

class CSurfaceFileParser;

class CSurfacePage
{
//!Construction:
public:

	CSurfacePage();
	CSurfacePage(const std::string &strFileName);
	~CSurfacePage();

//!Attributes:
public:
	
//!Operations:
public:
	
	
//!Implementation:
private:

	//: component manager script data file parsing
	VOID Parse(VOID);
	
private:

	std::string m_strFileName;
	CSurfaceFileParser *m_pPaser;
	typedef std::map<std::string, CSurfaceFileParser::ComponentInfo> stlCOMPONENT_MAP;
	stlCOMPONENT_MAP m_stlComponentMap;
	
	int m_nRefCount;

public:

	int AddRef(VOID) { return ++m_nRefCount; }

	int ReleaseRef(VOID)
	{
		m_nRefCount--;
		if (m_nRefCount < 0)
			m_nRefCount = 0;
		return m_nRefCount;
	}

	const CComponentOptions* GetOptions(const std::string& strName) const;
	// Append By Peessi
	CSurfaceFileParser* GetParser() { return m_pPaser; }
	
};

END_GUI


#endif