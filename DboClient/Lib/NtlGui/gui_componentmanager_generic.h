#ifndef __GUI_COMPONENTMANAGER_GENERIC_H__
#define __GUI_COMPONENTMANAGER_GENERIC_H__

#include "gui_define.h"
#include "gui_guifilepaser.h"
#include "gui_componentpage.h"

START_GUI

class CComponent;

class CComponentManager_Generic
{
//!Construction:
public:
	CComponentManager_Generic();
	~CComponentManager_Generic();

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
			CComponentManager_Generic* pThis = this;
			NTL_DELETE( pThis );
			return 0;
		}

		return m_nRefCount;
	}

	//: Component page add
//	bool AddPage(const std::string& str);
	//: Component page add
	// 형석
	COMPPAGE_HANDLE AddPage(const std::string& str);

	//: Component page remove
//	VOID RemovePage(const std::string& str);
	//: Component page remove
	// 형석.
	VOID RemovePage(COMPPAGE_HANDLE& h);

	//: Create Components
//	bool CreateComponents(const std::string& strPage,CComponent *pParent,CSurfaceManager *pSurfaceManager);
	
	//: Create Components
	bool CreateComponents(COMPPAGE_HANDLE h,CComponent *pParent,CSurfaceManager *pSurfaceManager);

	//: Remove Components
//	VOID DestroyComponents(const std::string& strPage);

	//: Remove Components
	VOID DestroyComponents(COMPPAGE_HANDLE& h);


	//: Returns component page pointer
//	CComponentPage* GetPage(const std::string& str);

	//: Returns component page pointer
	CComponentPage* GetPage(COMPPAGE_HANDLE h);

//!Implementation:
private:

	//: component manager script data file parsing
		
	int m_nRefCount;
//	std::map<std::string,CComponentPage*> m_stlPage;
	std::list<CComponentPage*> m_stlPage;	
	
//: signales 
public:
	//VOID OnBeforeRemove(CComponent *pComp);
};

END_GUI


#endif
