/*****************************************************************************
 *
 * File			: NtlPLGuiManager.h
 * Author		: HyungSuk Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer gui manager class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PL_GUIMANAGER_H__
#define __NTL_PL_GUIMANAGER_H__

#include <map>
#include <rwcore.h>
#include "NtlSerializer.h"
#include "gui_precomp.h"
#include "guicore_precomp.h"

class CNtlPLGui;
class CNtlPLGuiGroup;

class CNtlPLGuiManager
{
private:

	typedef std::map<CNtlPLGui*, RwInt32> MapUpdateGui;
	typedef std::map<std::string, CNtlPLGui*> MapGui;

	MapGui			m_mapGui;
	MapUpdateGui	m_mapUpdateGui;
	MapUpdateGui	m_mapRemoveUpdateGui;
	MapUpdateGui	m_mapUpdateBeforeGui;
	MapUpdateGui	m_mapRemoveUpdateBeforeGui;

	// manager  
	gui::CResourceManager	*m_pResMgr;
	gui::CSurfaceManager	*m_pSurMgr;
	gui::CComponentManager	*m_pCompMgr;
	gui::CGUIManager		*m_pGuiMgr;
	CListenerHandle			m_hListener;

	CNtlPLGuiGroup*			m_pGuiGroup;

	// 
	RwBool m_bRender;

private:

	CNtlPLGui* FindPLGui(const RwChar *pName);

public:

	CNtlPLGuiManager();
	~CNtlPLGuiManager();

//! operation

	static CNtlPLGuiManager* GetInstance(void);

	RwBool	Create(RwInt32 iWidth, RwInt32 iHeight);

	void	Destroy(void);

	void	Update(RwReal fElapsed);

	void	UpdateBeforeCamera(RwReal fElapsed);

	void	Render(void);

	void	PostRender(void);

	RwBool	LoadUserOption(CNtlSerializer& s);

	RwBool	SaveUserOption(CNtlSerializer& s);

	void	AddGui(CNtlPLGui *pGui);

	void	RemoveGui(CNtlPLGui *pGui);

	void	AddUpdateFunc( CNtlPLGui* pUpdateGui );
	void	RemoveUpdateFunc( CNtlPLGui* pUpdateGui );
	void	RemoveUpdateFuncWithoutRefCheck( CNtlPLGui* pUpdateGui );
	void	RemoveAllUpdateFunc(VOID);

	void	AddUpdateBeforeFunc( CNtlPLGui* pUpdateGui );
	void	RemoveUpdateBeforeFunc( CNtlPLGui* pUpdateGui );
	void	RemoveUpdateBeforeFuncWithoutRefCheck( CNtlPLGui* pUpdateGui );
	void	RemoveAllUpdateBeforeFunc(VOID);

	void	CreatePLGuiGroup();
	void	DestroyPLGuiGroup();

//! attaribute
	
	void RenderEnable(RwBool bRender = TRUE);

	RwBool IsGuiFocus(void);

	gui::CResourceManager* GetReourceManager(void) { return m_pResMgr; }

	gui::CSurfaceManager* GetSurfaceManager(void) { return m_pSurMgr; }

	gui::CComponentManager* GetComponentManager(void) { return m_pCompMgr; }

	gui::CGUIManager* GetGuiManager(void) { return m_pGuiMgr; }
};


static CNtlPLGuiManager* GetNtlGuiManager(void)
{
	return CNtlPLGuiManager::GetInstance(); 
}

#endif