#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// gui
#include "gui_define.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlOtherParam.h"

// table
#include "HelpTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "HelpWindowGui.h"

// etc

// define
#define dHELPWINDOW_LIST_SCROLL_WIDTH			12
#define dHELPWINDOW_LIST_SCROLL_HEIGHT			19
#define dHELPWINDOW_LIST_SLIDER_WIDTH			12
#define dHELPWINDOW_LIST_SLIDER_HEIGHT			39

#define dHELPWINDOW_CATEGORY_TITLE_X			22
#define dHELPWINDOW_CATEGORY_TITLE_Y			3
#define dHELPWINDOW_CATEGORY_TITLE_WIDTH		196
#define dHELPWINDOW_CATEGORY_TITLE_HEGIHT		20
#define dHELPWINDOW_CATEGORY_TITLE_COLOR		RGB(255, 210, 0)


#define dHELPWINDOW_CATEGORY_BUTTON_X			0
#define dHELPWINDOW_CATEGORY_BUTTON_Y			3
#define dHELPWINDOW_CATEGORY_BUTTON_WIDTH		196
#define dHELPWINDOW_CATEGORY_BUTTON_HEIGHT		20

#define dHELPWINDOW_CATEGORY_BUTTON_COLOR			RGB(255, 255, 255)
#define dHELPWINDOW_CATEGORY_BUTTON_COLOR_EX		RGB(255, 219, 71)

#define dHELPWINDOW_CONTENT_BUTTON_X			22
#define dHELPWINDOW_CONTENT_BUTTON_Y			0
#define dHELPWINDOW_CONTENT_BUTTON_WIDTH		244
#define dHELPWINDOW_CONTENT_BUTTON_HEIGHT		20

#define dHELPWINDOW_CONTENT_BUTTON_TEXT_COORD_X	10
#define dHELPWINDOW_CONTENT_BUTTON_TEXT_COORD_Y	3

#define dHELPWINDOW_LIST_X						20
#define dHELPWINDOW_LIST_Y						122
#define dHELPWINDOW_LIST_WIDTH					288
#define dHELPWINDOW_LIST_HEIGHT					287

#define dHELPWINDOW_LIST_LINE_HEIGHT			20
#define dHELPWINDOW_LIST_LINE_MARGIN			4
#define dHELPWINDOW_LIST_CHILD_MARGIN_WIDTH		0

//////////////////////////////////////////////////////////////////////////
// CHelpWindowList
//////////////////////////////////////////////////////////////////////////
CHelpWindowList::CHelpWindowList()
{
}

CHelpWindowList::~CHelpWindowList()
{
}

RwBool CHelpWindowList::Create(CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth)
{
	if (!CGuiLineTree::Create(rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth))
	{
		DBO_FAIL("Failed to create.");
		return FALSE;
	}

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
	m_pScroll->SetButtonSize(dHELPWINDOW_LIST_SCROLL_WIDTH, dHELPWINDOW_LIST_SCROLL_HEIGHT);
	m_pScroll->SetSliderSize(dHELPWINDOW_LIST_SLIDER_WIDTH, dHELPWINDOW_LIST_SLIDER_HEIGHT);

	

	return TRUE;
}

void CHelpWindowList::AddNode(sHELP_TBLDAT* pHelpTableData, RwBool bAddCategory, RwBool bAddContent)
{
	CGuiLineTreeNode* pParentNode = FindNode(pHelpTableData->byCategory);
	if (bAddCategory && pParentNode == NULL)
	{
		pParentNode = NTL_NEW CHelpWindowCategoryNode(this, pHelpTableData);
		CGuiLineTree::AddNode(pParentNode, GUILINETREE_ROOTNODE_ID);
	}

	if (pParentNode)
	{
		CGuiLineTreeNode* pChildNode = pParentNode->FindChildNode(pHelpTableData->tblidx);
		if (bAddContent && pChildNode == NULL)
		{
			pChildNode = NTL_NEW CHelpWindowContentNode(this, pHelpTableData);
			CGuiLineTree::AddNode(pChildNode, pParentNode->GetID());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// CHelpWindowCategoryNode
//////////////////////////////////////////////////////////////////////////
CHelpWindowCategoryNode::CHelpWindowCategoryNode(CGuiLineTree* pMgr, sHELP_TBLDAT* pHelpTableData)
:CGuiLineTreeNode(pMgr, pHelpTableData->byCategory)
,m_pBtnExpand(NULL)
,m_pBtnReduce(NULL)
{
	CRectangle rect;

	rect.SetRectWH(dHELPWINDOW_CATEGORY_BUTTON_X, dHELPWINDOW_CATEGORY_BUTTON_Y, dHELPWINDOW_CATEGORY_BUTTON_WIDTH, dHELPWINDOW_CATEGORY_BUTTON_HEIGHT);
	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));
	m_pBtnExpand->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnExpand->SetTextCoord( 25, -2 );
	m_pBtnExpand->SetText( API_GetTableContainer()->GetTextAllTable()->GetHelpTbl()->GetText(pHelpTableData->byCategory).c_str());
	m_pBtnExpand->SetTextFocusColor(  dHELPWINDOW_CATEGORY_BUTTON_COLOR_EX  );
	m_pBtnExpand->SetTextDownColor(  dHELPWINDOW_CATEGORY_BUTTON_COLOR  );
	m_pBtnExpand->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR );

	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));
	m_pBtnReduce->SetText( API_GetTableContainer()->GetTextAllTable()->GetHelpTbl()->GetText(pHelpTableData->byCategory).c_str());
	m_pBtnReduce->SetTextStyle( COMP_TEXT_LEFT );
	m_pBtnReduce->SetTextCoord( 25, -2 );
	m_pBtnReduce->SetTextFocusColor(  dHELPWINDOW_CATEGORY_BUTTON_COLOR_EX  );
	m_pBtnReduce->SetTextDownColor(  dHELPWINDOW_CATEGORY_BUTTON_COLOR  );
	m_pBtnReduce->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR );

	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &CHelpWindowCategoryNode::OnClickBtnExpand);
	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &CHelpWindowCategoryNode::OnClickBtnReduce);
}

CHelpWindowCategoryNode::~CHelpWindowCategoryNode()
{
	NTL_DELETE(m_pBtnReduce);
	NTL_DELETE(m_pBtnExpand);
}

void CHelpWindowCategoryNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();
	if (IsExpand())
	{
		m_pBtnReduce->Show(true);		
		m_pBtnExpand->Show(false);
	}
	else
	{
		m_pBtnExpand->Show(true);
		m_pBtnReduce->Show(false);		
	}

	m_pBtnExpand->SetPosition(m_nPosX + dHELPWINDOW_CATEGORY_BUTTON_X, m_nPosY + dHELPWINDOW_CATEGORY_BUTTON_Y);
	m_pBtnReduce->SetPosition(m_nPosX + dHELPWINDOW_CATEGORY_BUTTON_X, m_nPosY + dHELPWINDOW_CATEGORY_BUTTON_Y);
}

void CHelpWindowCategoryNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pBtnExpand->Show(false);
	m_pBtnReduce->Show(false);
}


void CHelpWindowCategoryNode::OnClickBtnExpand(gui::CComponent* pComponent)
{
	Expand(TRUE);
	m_pBtnExpand->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR_EX );
	m_pBtnReduce->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR_EX );
}

void CHelpWindowCategoryNode::OnClickBtnReduce(gui::CComponent* pComponent)
{
	Expand(FALSE);
	m_pBtnExpand->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR );
	m_pBtnReduce->SetTextUpColor( dHELPWINDOW_CATEGORY_BUTTON_COLOR );
}

//////////////////////////////////////////////////////////////////////////
// CHelpWindowContentNode
//////////////////////////////////////////////////////////////////////////
CHelpWindowContentNode::CHelpWindowContentNode(CGuiLineTree* pMgr, sHELP_TBLDAT* pHelpTableData)
:CGuiLineTreeNode(pMgr, pHelpTableData->tblidx)
,m_pBtnTitle(NULL)
{
	CRectangle rect;

	rect.SetRectWH(dHELPWINDOW_CONTENT_BUTTON_X, dHELPWINDOW_CONTENT_BUTTON_Y, dHELPWINDOW_CONTENT_BUTTON_WIDTH, dHELPWINDOW_CONTENT_BUTTON_HEIGHT);
	m_pBtnTitle = NTL_NEW gui::CButton(rect, std::string(), pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
	m_pBtnTitle->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpWindow.srf", "srfBgHelpContentItemUp"));
	m_pBtnTitle->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpWindow.srf", "srfBgHelpContentItemDown"));
	m_pBtnTitle->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpWindow.srf", "srfBgHelpContentItemFocus"));
	m_pBtnTitle->SetTextFont( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	m_pBtnTitle->SetTextStyle(COMP_TEXT_LEFT);
	m_pBtnTitle->SetTextCoord(dHELPWINDOW_CONTENT_BUTTON_TEXT_COORD_X, dHELPWINDOW_CONTENT_BUTTON_TEXT_COORD_Y);
	m_pBtnTitle->SetText(API_GetTableContainer()->GetTextAllTable()->GetHelpTbl()->GetText(pHelpTableData->dwHelpTitle).c_str());
	
	m_slotClickedBtnTitle = m_pBtnTitle->SigClicked().Connect(this, &CHelpWindowContentNode::OnClickedBtnTitle);
}

CHelpWindowContentNode::~CHelpWindowContentNode()
{
	NTL_DELETE(m_pBtnTitle);
}

void CHelpWindowContentNode::ShowProc()
{
	CGuiLineTreeNode::ShowProc();

	m_pBtnTitle->Show(true);

	m_pBtnTitle->SetPosition(m_nPosX + dHELPWINDOW_CONTENT_BUTTON_X, m_nPosY + dHELPWINDOW_CONTENT_BUTTON_Y);
}

void CHelpWindowContentNode::HideProc()
{
	CGuiLineTreeNode::HideProc();

	m_pBtnTitle->Show(false);
}

void CHelpWindowContentNode::OnClickedBtnTitle(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenHelpWindow(GetID());
}

//////////////////////////////////////////////////////////////////////////
// CHelpWindowGui
//////////////////////////////////////////////////////////////////////////
CHelpWindowGui::CHelpWindowGui(const RwChar* pName)
: CNtlPLGui(pName)
,m_pHelpWindowList(NULL)
,m_pDlgList(NULL)
,m_pDlgContent(NULL)
,m_pBtnClose(NULL)
,m_pBtnGoList(NULL)
,m_pHtmlContent(NULL)
,m_pStbTitle(NULL)
,m_pStbListTopTitle(NULL)
,m_pStbContentTopTitleParent(NULL)
,m_pStbContentTopTitleChild(NULL)
,m_pStbHelpContents(NULL)
,m_pStbGoList(NULL)
,m_eHelpWindowType(eHELPWINDOW_TYPE_NONE)
,m_tableIndex(INVALID_TBLIDX)
{
}

CHelpWindowGui::~CHelpWindowGui()
{
}

RwBool CHelpWindowGui::Create()
{
	NTL_FUNCTION("CHelpWindowGui::Create");

	if (!CNtlPLGui::Create("", "gui\\HelpWindow.srf", "gui\\HelpWindow.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");
	m_pDlgList					= (gui::CDialog*)GetComponent("dlgList");
	m_pDlgContent				= (gui::CDialog*)GetComponent("dlgContent");
	m_pBtnClose					= (gui::CButton*)GetComponent("btnClose");
	m_pBtnGoList				= (gui::CButton*)GetComponent("btnGoList");
	m_pHtmlContent				= (gui::CHtmlBox*)GetComponent("htmlContent");
	m_pStbTitle					= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pStbListTopTitle			= (gui::CStaticBox*)GetComponent("stbListTopTitle");
	m_pStbContentTopTitleParent	= (gui::CStaticBox*)GetComponent("stbContentTopTitleParent");
	m_pStbContentTopTitleChild	= (gui::CStaticBox*)GetComponent("stbContentTopTitleChild");
	m_pStbHelpContents			= (gui::CStaticBox*)GetComponent("stbHelpContents");
	m_pStbGoList				= (gui::CStaticBox*)GetComponent("stbGoList");
	m_pBtnRequestPetition		= (gui::CButton*)GetComponent("btnRequestPetition");

	m_slotClickedBtnClose	= m_pBtnClose->SigClicked().Connect(this, &CHelpWindowGui::OnClickedBtnClose);
	m_slotRequestPetition	= m_pBtnRequestPetition->SigClicked().Connect(this, &CHelpWindowGui::OnClickedBtnRequestPetition);
	m_slotClickedBtnGoList	= m_pBtnGoList->SigClicked().Connect(this, &CHelpWindowGui::OnClickedBtnGoList);	
	m_slotCaptureMouseDown		= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CHelpWindowGui::OnCaptureMouseDown);
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CHelpWindowGui::OnCaptureWheelMove );

	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_HELPWND_TITLE"));
	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pStbListTopTitle->SetText(GetDisplayStringManager()->GetString("DST_HELPWND_TOP_TITLE"));
	m_pStbHelpContents->SetText(GetDisplayStringManager()->GetString("DST_HELPWND_HELPCONTENTS"));
	m_pStbGoList->SetText(GetDisplayStringManager()->GetString("DST_HELPWND_GOLIST"));
	m_pBtnRequestPetition->SetText(GetDisplayStringManager()->GetString("DST_HELPWND_PETITION_TO_GM"));

	// 클베 버전에서는 진성 시스템을 이용할 수 없도록 막는다
	m_pBtnRequestPetition->Show(false);
	

	if (!CreateHelpWindowList(m_pDlgList))
	{
		NTL_RETURN(FALSE);
	}

	LinkMsg(g_EventOpenHelpWindow, 0);
	LinkMsg(g_EventOpenHelpContent, 0);
	LinkMsg(g_EventHelpHint, 0);
	LinkMsg(g_EventRegSideHintIcon, 0);
	

	ClearUserInterface();
	Show(false);
	NTL_RETURN(TRUE);
}

RwBool CHelpWindowGui::CreateHelpWindowList(gui::CComponent* pParent)
{
	CRectangle rect;
	rect.SetRectWH(dHELPWINDOW_LIST_X, dHELPWINDOW_LIST_Y, dHELPWINDOW_LIST_WIDTH, dHELPWINDOW_LIST_HEIGHT);

	m_pHelpWindowList = NTL_NEW CHelpWindowList;
	if (!m_pHelpWindowList->Create(rect, m_pDlgList, dHELPWINDOW_LIST_LINE_HEIGHT, dHELPWINDOW_LIST_LINE_MARGIN, dHELPWINDOW_LIST_CHILD_MARGIN_WIDTH, dHELPWINDOW_LIST_SCROLL_WIDTH))
		return FALSE;

	CHelpTable* pHelpTable	= API_GetTableContainer()->GetHelpTable();
	for (CTable::TABLEIT it = pHelpTable->Begin(); it != pHelpTable->End(); ++it)
	{
		sHELP_TBLDAT* pHelpTableData = (sHELP_TBLDAT*)it->second;

//		if (pHelpTableData->byConditionCheck == INVALID_BYTE)
//		{
		m_pHelpWindowList->AddNode(pHelpTableData);
//		}

	}

	return TRUE;
}

void CHelpWindowGui::Destroy()
{
	NTL_FUNCTION("CHelpWindowGui::Destory");

	if (m_pHelpWindowList)
	{
		m_pHelpWindowList->Destroy();
		NTL_DELETE(m_pHelpWindowList);
	}

	UnLinkMsg(g_EventOpenHelpWindow);
	UnLinkMsg(g_EventOpenHelpContent);
	UnLinkMsg(g_EventHelpHint);
	UnLinkMsg(g_EventRegSideHintIcon);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CHelpWindowGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	if (bOpen)
	{
		if (m_eHelpWindowType == eHELPWINDOW_TYPE_NONE)
		{
			OpenUserInterface(eHELPWINDOW_TYPE_LIST, INVALID_TBLIDX);
		}
	}
	else
	{
		ClearUserInterface();
	}
	return TRUE;
}

void CHelpWindowGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CHelpWindowGui::HandleEvents");
	if (msg.Id == g_EventOpenHelpWindow)
	{
		SDboEventOpenHelpWindow* pEvnet = reinterpret_cast<SDboEventOpenHelpWindow*>(msg.pData);
		OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, pEvnet->uiTableIndex);
		GetDialogManager()->OpenDialog(DIALOG_HELPWND);
	}
	else if(msg.Id == g_EventOpenHelpContent)
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_HELP_BUTTON_IN_DIALOG ) )
			NTL_RETURNVOID();

		SDboEventOpenHelpContent* pData = reinterpret_cast<SDboEventOpenHelpContent*>(msg.pData);
		
		switch ( pData->uiDialogType )
		{
			case DIALOG_STATUS:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 1001);
			}
			break;
			case DIALOG_TARGER_UI:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 1002);
			}
			break;
			case DIALOG_DROPITEM_INFO:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 2002);
			}
			break;
			case DIALOG_COMMUNITY:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 8002);
			}
			break;
			case DIALOG_RBBOARD:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 9002);
			}
			break;
			case DIALOG_DOGI:
			{

			}
			break;
			case DIALOG_DOJO_INFO:
			{

			}
			break;
			case DIALOG_DOJO_UPGRADE:
			{

			}
			break;
			case DIALOG_FRIEND_LIST:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 8004);
			}
			break;
			case DIALOG_BAGSLOT:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 1003);
			}
			break;
			case DIALOG_QUESTLIST:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 6002);
			}
			break;
			case DIALOG_QUESTPROGRESS:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 6003);
			}
			break;
			case DIALOG_QUESTPROPOSAL:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 6002);
			}
			break;
			case DIALOG_SKILL:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 5000);
			}
			break;
			default:
			{
				OpenUserInterface(eHELPWINDOW_TYPE_CONTECT, 1001);
			}
			break;
		}

		GetDialogManager()->OpenDialog(DIALOG_HELPWND);
	}
	else if (msg.Id == g_EventHelpHint)
	{
		SNtlEventHelpHint*	pEvnet			= reinterpret_cast<SNtlEventHelpHint*>(msg.pData);
		RwUInt32			uiHelpHint		= pEvnet->uiHelpHint;
		CHelpTable*			pHelpTable		= API_GetTableContainer()->GetHelpTable();

		for (CTable::TABLEIT it = pHelpTable->Begin(); it != pHelpTable->End(); ++it)
		{
			sHELP_TBLDAT* pHelpTableData = reinterpret_cast<sHELP_TBLDAT*>(it->second);
			
			if (pHelpTableData->byConditionCheck != INVALID_BYTE)
			{
				RwUInt32 uiCheck = 0x1 << pHelpTableData->byConditionCheck;
				if (uiHelpHint & uiCheck)
				{
					m_pHelpWindowList->AddNode(pHelpTableData);
				}
			}
		}
		UnLinkMsg(g_EventHelpHint);
	}
	else if (msg.Id == g_EventRegSideHintIcon)
	{
		SNtlEventRegSideHintIcon* pEvnet = reinterpret_cast<SNtlEventRegSideHintIcon*>(msg.pData);

		if (pEvnet->uiEventType == HINTICON_HELP)
		{
			CHelpTable*		pHelpTable		= API_GetTableContainer()->GetHelpTable();
			sHELP_TBLDAT*	pHelpTableData	= reinterpret_cast<sHELP_TBLDAT*>(pHelpTable->FindData(pEvnet->uiTableIndex));

			m_pHelpWindowList->AddNode(pHelpTableData);
		}
	}
	NTL_RETURNVOID();
}

void CHelpWindowGui::OpenUserInterface(eHELPWINDOW_TYPE ehelpWindowType, TBLIDX tableIndex)
{
	m_eHelpWindowType = ehelpWindowType;
	switch (m_eHelpWindowType)
	{
	case eHELPWINDOW_TYPE_LIST:
		m_pDlgList->Show(true);
		m_pDlgContent->Show(false);
		OpenHelpList(tableIndex);
		break;
	case eHELPWINDOW_TYPE_CONTECT:
		m_pDlgList->Show(false);
		m_pDlgContent->Show(true);
		OpenHelpContent(tableIndex);
		break;
	}
}

/**
* \brief 유저 인터페이스를 초기화한다.
* 열려있던 트리 구조 그대로 수정.
*/
void CHelpWindowGui::ClearUserInterface()
{
	m_eHelpWindowType	= eHELPWINDOW_TYPE_NONE;
	m_tableIndex		= INVALID_TBLIDX;
}

void CHelpWindowGui::CloseUserInterface()
{
	GetDialogManager()->CloseDialog(DIALOG_HELPWND);
}

void CHelpWindowGui::OpenHelpList(TBLIDX tableIndex)
{	
	/*if (tableIndex != INVALID_TBLIDX)
	{
		CHelpTable*		pHelpTable		= API_GetTableContainer()->GetHelpTable();
		sHELP_TBLDAT*	pHelpTableData	= reinterpret_cast<sHELP_TBLDAT*>(pHelpTable->FindData(tableIndex));
		CGuiLineTreeNode* pParentNode	= m_pHelpWindowList->GetRootNode()->FindChildNode(pHelpTableData->byCategory);

		if (pParentNode)
		{
			m_pHelpWindowList->GetRootNode()->ExpandChild(FALSE);
			pParentNode->Expand(TRUE);
		}	
		m_tableIndex = INVALID_TBLIDX;
	}*/
	m_pBtnGoList->Enable(false);
}
void CHelpWindowGui::OpenHelpContent(TBLIDX tableIndex)
{
	CHelpTable*		pHelpTable		= API_GetTableContainer()->GetHelpTable();
	sHELP_TBLDAT*	pHelpTableData	= reinterpret_cast<sHELP_TBLDAT*>(pHelpTable->FindData(tableIndex));

	m_tableIndex = tableIndex;
	if (pHelpTableData == NULL)
	{
		DBO_FAIL("Table is empty.");
		return;
	}

	CHAR	acBuffer[1024];	
	CHAR	acResource[1024];

	::WideCharToMultiByte(GetACP(), 0, pHelpTableData->wszHelpHTMLName, -1, acResource, 1024, NULL, NULL);
	sprintf_s(acBuffer, 1024, "%s%s", dHELPWINDOW_HTML_DIRECTORY, acResource);

	m_pBtnGoList->Enable(true);
	m_pHtmlContent->Clear();
	m_pHtmlContent->SetHtmlFromFile(acBuffer);
	m_pStbContentTopTitleParent->SetText(API_GetTableContainer()->GetTextAllTable()->GetHelpTbl()->GetText(pHelpTableData->byCategory).c_str());
	m_pStbContentTopTitleChild->SetText(API_GetTableContainer()->GetTextAllTable()->GetHelpTbl()->GetText(pHelpTableData->dwHelpTitle).c_str());
}

void CHelpWindowGui::OnClickedBtnClose(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_HELP_WND_CLOSE_BTN ) )
		return;

	CloseUserInterface();
}
void CHelpWindowGui::OnClickedBtnRequestPetition(gui::CComponent* pComponent)
{
	CDboEventGenerator::PetitionEvent(PETITON_OPEN_DILAOG);
}

void CHelpWindowGui::OnClickedBtnGoList(gui::CComponent* pComponent)
{
	OpenUserInterface(eHELPWINDOW_TYPE_LIST, m_tableIndex);
}

void CHelpWindowGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_HELPWND, key.m_fX, key.m_fY);
}

void CHelpWindowGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if( !m_pThis->IsVisible() )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	if( m_pDlgList->IsVisible() )
	{
		RwInt32 nValue = m_pHelpWindowList->GetScrollBar()->GetValue();
		RwInt32 nMaxValue = m_pHelpWindowList->GetScrollBar()->GetMaxValue();
		RwInt32 nDelta = nValue - sDelta / GUI_MOUSE_WHEEL_DELTA;

		if( nDelta < 0 )
			nDelta = 0;
		else if( nDelta > nMaxValue )
			nDelta = nMaxValue;
	
		m_pHelpWindowList->GetScrollBar()->SetValue( nDelta );	
	}
	else if( m_pDlgContent->IsVisible() )
	{
		RwInt32 nValue = m_pHtmlContent->GetScrollBar()->GetValue();
		RwInt32 nMaxValue = m_pHtmlContent->GetScrollBar()->GetMaxValue();
		RwInt32 nDelta = nValue - (sDelta*18) / GUI_MOUSE_WHEEL_DELTA;

		if( nDelta < 0 )
			nDelta = 0;
		else if( nDelta > nMaxValue )
			nDelta = nMaxValue;

		m_pHtmlContent->GetScrollBar()->SetValue( nDelta );	
	}
}

