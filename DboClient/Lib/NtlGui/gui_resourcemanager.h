#ifndef __GUI_RESOURCEMANAGER_H__
#define __GUI_RESOURCEMANAGER_H__

#include "gui_define.h"
#include "gui_componenttype.h"
#include "gui_surface.h"
#include "gui_hypertext.h"

START_GUI

class CTexture;
class CComponentOptions;
class CResourceManager_Generic;
class CResourcePage;

class CResourceManager
{
//! Construction:
public:	
	CResourceManager();

	//: component manager 소멸자...	
	virtual ~CResourceManager();

//! Attributes:
public:

	//: resource page add
	bool AddPage(const std::string& str);

	//: resource page remove
	VOID RemovePage(const std::string& str);

	VOID CreateTexture (const std::string& strName);
	VOID DeleteTexture (const std::string& strName);

	CTexture* CreatureTexture(const std::string& strTex);
	//: strName => surface name
	//: surface name으로 surface를 정보를 얻는다.
	CTexture* GetTextureHandle(const std::string& strPage,const std::string& strName);

	static bool CreateHyperText (const std::string& strName);
	static CTextNode* GetHyperText (const std::string& strKey);

	// Append By Peessi
	CResourcePage*	GetResourcePage( const std::string& strFilename );
		
//! Implementation:
private:

	std::string m_stlResource;
	static CHyperTextBuilder m_HypertextBuilder;
	CResourceManager_Generic *m_pImpl;
};

END_GUI



#endif