#pragma once

#include "GuiLineTree.h"

//#define	dHELPWINDOW_DEBUG
#define dHELPWINDOW_HTML_DIRECTORY	"script\\help\\"

struct sHELP_TBLDAT;

class CHelpWindowGui;
class CHelpWindowList;
class CHelpWindowCategoryNode;
class CHelpWindowContentNode;

//////////////////////////////////////////////////////////////////////////
// CHelpWindowList
//////////////////////////////////////////////////////////////////////////
class CHelpWindowList : public CGuiLineTree
{
public:
	CHelpWindowList();
	virtual ~CHelpWindowList();

	RwBool	Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth);

	void	AddNode(sHELP_TBLDAT* pHelpTableData, RwBool bAddCategory = TRUE, RwBool bAddContent = TRUE);

	gui::CScrollBar* GetScrollBar() { return static_cast<gui::CScrollBar*>(CGuiLineTree::GetScrollBar()); }	
};

//////////////////////////////////////////////////////////////////////////
// CHelpWindowCategoryNode
//////////////////////////////////////////////////////////////////////////
class CHelpWindowCategoryNode : public CGuiLineTreeNode
{
	/*gui::CStaticBox*	m_pStbTitle;*/
	gui::CButton*		m_pBtnExpand;
	gui::CButton*		m_pBtnReduce;

	gui::CSlot			m_slotClickedBtnExpand;
	gui::CSlot			m_slotClickedBtnReduce;

public:
	CHelpWindowCategoryNode(CGuiLineTree* pMgr, sHELP_TBLDAT* pHelpTableData);
	virtual ~CHelpWindowCategoryNode();

	virtual void	ShowProc();
	virtual void	HideProc();

	void			OnClickBtnExpand(gui::CComponent* pComponent);
	void			OnClickBtnReduce(gui::CComponent* pComponent);
};

//////////////////////////////////////////////////////////////////////////
// CHelpWindowContentNode
//////////////////////////////////////////////////////////////////////////
class CHelpWindowContentNode : public CGuiLineTreeNode
{
	gui::CButton*	m_pBtnTitle;

	gui::CSlot		m_slotClickedBtnTitle;

public:
	CHelpWindowContentNode(CGuiLineTree* pMgr, sHELP_TBLDAT* pHelpTableData);
	virtual ~CHelpWindowContentNode();

	virtual void	ShowProc();
	virtual void	HideProc();

	void			OnClickedBtnTitle(gui::CComponent* pComponent);
};

//////////////////////////////////////////////////////////////////////////
// CHelpWindowGui
//////////////////////////////////////////////////////////////////////////
class CHelpWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	enum eHELPWINDOW_TYPE
	{
		eHELPWINDOW_TYPE_NONE,
		eHELPWINDOW_TYPE_LIST,
		eHELPWINDOW_TYPE_CONTECT,

		eHELPWINDOW_TYPE_NUM,
	};

protected:
	gui::CDialog*		m_pDlgList;
	gui::CDialog*		m_pDlgContent;

	gui::CButton*		m_pBtnClose;
	gui::CButton*		m_pBtnGoList;

	gui::CHtmlBox*		m_pHtmlContent;

	gui::CStaticBox*	m_pStbTitle;
	gui::CStaticBox*	m_pStbListTopTitle;
	gui::CStaticBox*	m_pStbContentTopTitleParent;
	gui::CStaticBox*	m_pStbContentTopTitleChild;
	gui::CStaticBox*	m_pStbHelpContents;
	gui::CStaticBox*	m_pStbGoList;

	gui::CButton*		m_pBtnRequestPetition;

	gui::CSlot			m_slotRequestPetition;
	gui::CSlot			m_slotClickedBtnClose;
	gui::CSlot			m_slotClickedBtnGoList;
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCaptureWheelMove;

	
	eHELPWINDOW_TYPE	m_eHelpWindowType;
	CHelpWindowList*	m_pHelpWindowList;	
	TBLIDX				m_tableIndex;

public:
	CHelpWindowGui(const RwChar* pName);
	virtual ~CHelpWindowGui();

	RwBool	Create();
	RwBool	CreateHelpWindowList(gui::CComponent* pParent);
	void	Destroy();

	RwInt32	SwitchDialog(bool bOpen);
	void	HandleEvents(RWS::CMsg& msg);

	void	OnClickedBtnClose(gui::CComponent* pComponent);
	void	OnClickedBtnRequestPetition(gui::CComponent* pComponent);
	void	OnClickedBtnGoList(gui::CComponent* pComponent);
	void	OnCaptureMouseDown(const CKey& key);

	void	OpenUserInterface(eHELPWINDOW_TYPE ehelpWindowType, TBLIDX tableIndex = INVALID_TBLIDX);
	void	ClearUserInterface();
	void	CloseUserInterface();

	void	OpenHelpList(TBLIDX tableIndex);
	void	OpenHelpContent(TBLIDX tableIndex);
	void	OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos);
};