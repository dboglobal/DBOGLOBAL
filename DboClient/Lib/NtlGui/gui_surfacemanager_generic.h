#ifndef __GUI_SURFACEMANAGER_GENERIC_H__
#define __GUI_SURFACEMANAGER_GENERIC_H__

#include "gui_define.h"
#include "gui_surfacepage.h"
#include "gui_surface.h"

START_GUI

class CSurfaceManager_Generic
{
//!Construction:
public:

	CSurfaceManager_Generic();
	~CSurfaceManager_Generic();

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
			CSurfaceManager_Generic* pThis = this;
			NTL_DELETE( pThis );
			return 0;
		}
		return m_nRefCount;
	}
	
	//: Surface page add
	bool AddPage(const std::string& str);

	//: Surface page remove
	VOID RemovePage(const std::string& str);

	//: Returns surface page pointer
	CSurfacePage* GetPage(const std::string& str);

//!Implementation:
private:

	int m_nRefCount;

	std::map<std::string,CSurfacePage*> m_stlPage;
	std::map<std::string,CSurfacePage*> m_stlPageKey;
	
};

END_GUI


#endif
