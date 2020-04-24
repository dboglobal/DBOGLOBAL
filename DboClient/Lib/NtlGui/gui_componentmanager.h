#ifndef __GUI_COMPONENTMANAGER_H__
#define __GUI_COMPONENTMANAGER_H__

#include "gui_define.h"
#include "gui_componenttype.h"

START_GUI

class CComponent;
class CComponentManager_Generic;
class CSurfaceManager;

class CComponentManager
{
//! Construction:
public:
	
	CComponentManager();
	//: component manager 소멸자...	
	virtual ~CComponentManager();

//! Attributes:
public:
	//: List of known component types.
	typedef std::map<std::string,CComponentType *> stlCOMPTYPE_MAP;
	static stlCOMPTYPE_MAP m_stlCompType;

	static CComponent *CreateComponent(
		const std::string &type,
		CComponent *pParent,
		CSurfaceManager *pSurfaceManager)
	{
		stlCOMPTYPE_MAP::const_iterator it(m_stlCompType.find(type));
		if (it != m_stlCompType.end())
			return (*it).second->CreateComponent(pParent,pSurfaceManager);

		return NULL;
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

	//: Components Create
//	bool CreateComponents(const std::string& strPage,CComponent *pParent,CSurfaceManager *pSurfaceManager);

	//: Components Create
// 형석.
	bool CreateComponents(COMPPAGE_HANDLE h,CComponent *pParent,CSurfaceManager *pSurfaceManager);

	//: Components Destroy
//	VOID DestroyComponents(const std::string& strPage);

	//: Components Destroy
	VOID DestroyComponents(COMPPAGE_HANDLE& h);

	//: Get Component 
//	CComponent* GetComponent(const std::string& strPage,const std::string &name) const;

	//: GetComponent
	CComponent* GetComponent(COMPPAGE_HANDLE h,const std::string &name) const;
	
//! Implementation:
private:

	CComponentManager_Generic *m_pImpl;
};

END_GUI



#endif