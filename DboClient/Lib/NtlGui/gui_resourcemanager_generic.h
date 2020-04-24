#ifndef __GUI_RESOURCEMANAGER_GENERIC_H__
#define __GUI_RESOURCEMANAGER_GENERIC_H__

#include "gui_define.h"
#include "gui_resourcefilepaser.h"
#include "gui_resourcepage.h"
#include "gui_surface.h"

START_GUI


class CResourceManager_Generic
{
//!Construction:
public:

	CResourceManager_Generic();
	~CResourceManager_Generic();

//!Attributes:
public:
	
//!Operations:
public:
	
	int AddRef(VOID) { return ++m_nRefCount; }

	int ReleaseRef(VOID)
	{
		m_nRefCount--;
		if (m_nRefCount == 0)
		{
			CResourceManager_Generic* pThis = this;
			NTL_DELETE( pThis );
			return 0;
		}
		return m_nRefCount;
	}

	//: resource page add
	bool AddPage(const std::string& str);

	//: resource page remove
	VOID RemovePage(const std::string& str);

	//: Returns surface page pointer
	CResourcePage* GetPage(const std::string& str);

//!Implementation:
private:

private:

	int m_nRefCount;

	std::map<std::string,CResourcePage*> m_stlPage;
	std::map<std::string,CResourcePage*> m_stlPageKey;

public:
	
};

END_GUI


#endif
