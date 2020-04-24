#ifndef __GUI_RESOURCEPAGE_H__
#define __GUI_RESOURCEPAGE_H__

#include "gui_define.h"
#include "gui_resourcefilepaser.h"
#include "gui_surface.h"

START_GUI

class CTexture;
class CResourceFileParser;

class CResourcePage
{
//!Construction:
public:

	CResourcePage();
	CResourcePage(const std::string &strFileName);
	~CResourcePage();

//!Attributes:
public:

	VOID CreateTexture(VOID);
	VOID DeleteTexture(VOID);
	
//!Operations:
public:
	
//!Implementation:
private:

	//: component manager script data file parsing
	VOID Parse(VOID);

private:

	std::string m_strFileName;
	CResourceFileParser *m_pPaser;
	typedef std::map<std::string, CResourceFileParser::ComponentInfo> stlCOMPONENT_MAP;
	typedef std::map<std::string, CTexture*> stlTEXTURE_MAP;
	stlTEXTURE_MAP m_stlTextureMap;
	stlCOMPONENT_MAP m_stlComponentMap;

	int m_nRefCount;
	BOOL m_bCreate;
	
public:

	int AddRef(VOID) { return ++m_nRefCount; }

	int ReleaseRef(VOID)
	{
		m_nRefCount--;
		if (m_nRefCount < 0)
			m_nRefCount = 0;
		return m_nRefCount;
	}

	const CComponentOptions* GetOptions(std::string strName) const;
	const std::map<std::string,CTexture*>* GetTextureMap(VOID) const;
	CTexture* GetTextureHandle(std::string str);
	// Append By Peessi
	CResourceFileParser* GetParser() { return m_pPaser; }
	
};

END_GUI


#endif
