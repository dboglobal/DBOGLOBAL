/*****************************************************************************
 *
 * File			: NtlPLGui.h
 * Author		: HyungSuk Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 8. 11	
 * Abstract		: Presentation layer gui entity class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PL_GUI_H__
#define __NTL_PL_GUI_H__

#include <string>
#include <list>

// core
#include "NtlSerializer.h"

// gui
#include "gui_precomp.h"
#include "guicore_precomp.h"

// presenation gui serialize id

#define PLGUI_SERIALIZE_POS_X	0
#define PLGUI_SERIALIZE_POS_Y	1
#define PLGUI_SERIALIZE_END		0xff


class CNtlPLGui 
{
protected:
	
	std::string		m_strName;
	std::string		m_strSurPage;
	std::string		m_strResPage;
	std::string		m_strCompPage;
	COMPPAGE_HANDLE m_hCompPage;
	RwBool			m_bSerialize;

	gui::CDialog	*m_pThis;
	typedef std::list<gui::CDialog*> ListDialog;
	ListDialog		m_listLinked;

protected:

	virtual void AddResPage (const std::string& str);
	virtual void AddSurPage	(const std::string& str);
	virtual void AddCompPage (const std::string& str);

	virtual void RemoveResPage (const std::string& str);
	virtual void RemoveSurPage (const std::string& str);
	virtual void RemoveCompPage	(const std::string& str);

public:

	CNtlPLGui();
	CNtlPLGui(const RwChar *pName);
	virtual ~CNtlPLGui();

	virtual RwBool	Create(const std::string& strResPage, const std::string& strSurPage, const std::string& strCompPage);

	virtual void	Destroy(void);

	virtual void	Update(RwReal fElapsed);

	virtual void	UpdateBeforeCamera(RwReal fElapsed);

	virtual void	Render(void);

	virtual RwBool	LoadUserOption(CNtlSerializer& s);

	virtual RwBool	SaveUserOption(CNtlSerializer& s);

public:

	void	SetName( const RwChar* pName );
	const RwChar* GetName(void) { return m_strName.c_str(); }
	gui::CDialog* GetDialog(void) { return m_pThis; }
        
	virtual void CreateComponents(gui::CComponent*	pParent);
	virtual void DestroyComponents(void); 
	virtual gui::CComponent* GetComponent(const std::string& str);

	virtual void	Resize(int iWidth, int iHeight);	
	virtual void	RaiseTop(void);
	virtual void	Lower(void);
	virtual void	Popup(bool bPopup);
	virtual void	Show(bool bShow);				
	virtual RwBool	IsShow(void);				
	virtual void	Enable(bool bEnable);
	virtual RwBool	IsEnable(void);
	virtual void	SetMovable(RwBool bMovable);
	virtual void	SetPosition(int nX,int nY);
	virtual void	SetPosition(const CRectangle &rect);
	virtual CRectangle GetPosition (void);
	virtual RwInt32 GetWidth();
	virtual RwInt32 GetHeight();

	virtual void	SetAlpha(RwUInt8 byAlpha);

	void	Link(gui::CDialog* pDialog);
	void	UnLink(gui::CDialog* pDialog);
	void	UnLinkAll();

	void	RaiseLinked();
	void	ShowLinked(bool bShow);	
	RwBool	IsHaveLinkedGui();

	void	EnableSerialize(RwBool bSerialize);
	RwBool	IsEnableSerialize(void) const;

	CRectangle GetLinkedArea();

	void	CenterAlign( RwUInt32 uiWidth, RwUInt32 uiHeight );
	/// woosungs_test
	const char*	GetCompPageName()				{		return m_strCompPage.c_str();		}
protected:
	void	MoveLinkedPLGui(int nAdjustX, int nAdjustY);
};



#endif
