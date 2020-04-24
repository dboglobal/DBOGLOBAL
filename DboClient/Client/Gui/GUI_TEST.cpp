#include "precomp_dboclient.h"

//core
#include "NtlDebug.h"
#include "ceventhandler.h"

//gui
#include "gui_define.h"

//presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"


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
//#include "DisplayStringManager.h"
#include "GUI_TEST.h"


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


CGUI_test::CGUI_test( const RwChar* pName )
: CNtlPLGui( pName )
,m_pDigStatic(NULL)
,m_pDigEdit(NULL)
,m_pBtnStatic(NULL)
,m_pBtnEdit(NULL)
,m_pBtnClose(NULL)
,m_pStbTitle(NULL)
,m_pStbStatic(NULL)
,m_pStbEdit(NULL)
,m_pGuiTest(NULL)
,m_bButCheck(false)
,m_pScroll(NULL)
,m_pBtnDlgStatic(NULL)
,m_pBtnDlgEdit(NULL)
,m_bdlgEdit(false)
,m_bdlgStatic(false)
,m_pInput(NULL)
{

}

CGUI_test::~CGUI_test()
{

}

RwBool CGUI_test::Create( )
{
	NTL_FUNCTION("CGUI_test::Create");
	// GUI_TEST. srf,frm 로컬로 추가
	if (!CNtlPLGui::Create("", "gui\\GUI_TEST.srf", "gui\\GUI_TEST.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbTitle					= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pBtnClose					= (gui::CButton*)GetComponent("btnClose");

	m_pDigStatic				= (gui::CDialog*)GetComponent("dlgStatic");
	m_pDigEdit					= (gui::CDialog*)GetComponent("dlgEdit");

	m_pStbStatic				= (gui::CStaticBox*)GetComponent("stbStatic");
	m_pStbEdit					= (gui::CStaticBox*)GetComponent("stbEdit");
	
	m_pScroll					= (gui::CScrollBar*)GetComponent("scbScroll");

	m_pBtnDlgStatic					= (gui::CButton*)GetComponent("btnEditClose");
	m_pBtnDlgEdit					= (gui::CButton*)GetComponent("btnStaticClose");

	m_pInput						= (gui::CInputBox*)GetComponent( "inbChat" );

	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pInput->SetMaxLength(NTL_MAX_LENGTH_OF_CHAT_MESSAGE);

	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	m_pInput->SetMaxLength(NTL_MAX_SIZE_USERID);
	m_slotInput = m_pInput->SigReturnPressed().Connect(this, &CGUI_test::OnEditInput);

	m_pStbStatic->SetText("Dlg....11");
	m_pBtnDlgStatic->SetText("Dlg1...Btn");
	m_pDigStatic->Show(false);

	m_pStbEdit->SetText("Dlg....22");
	m_pBtnDlgEdit->SetText("Dlg2...Btn");
	m_pDigEdit->Show(false);

	m_slotClickedBtnClose	= m_pBtnClose->SigClicked().Connect(this, &CGUI_test::OnClickedBtnClose);

	m_slotClickedBtnStatic	= m_pBtnDlgStatic->SigClicked().Connect(this, &CGUI_test::OnClickedBtnDlgStatic);
	m_slotClickedBtnEdit	= m_pBtnDlgEdit->SigClicked().Connect(this, &CGUI_test::OnClickedBtnDlgEdit);

	m_slotScroll        = m_pScroll->SigValueChanged().Connect(this, &CGUI_test::OnScrollChanged);
	m_slotScrollMove    = m_pScroll->SigSliderMoved().Connect(this, &CGUI_test::OnScrollChanged);


	m_pStbTitle->SetText("GUI_TEST 중입니다....!!");
	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	
//	if( !CreateDig())
	{
//		NTL_RETURN(FALSE);
	}

	/*LinkMsg(g_EventNPCDialogOpen);
	LinkMsg(g_EventPortalStartRes);
	LinkMsg(g_EventPortalInfoRes);
	LinkMsg(g_EventPortalAddRes);
	LinkMsg(g_EventPortalRes);
	LinkMsg(g_EventMsgBoxResult);
	LinkMsg(g_EventDialog);*/

	Show(true);

	NTL_RETURN(TRUE);

	return TRUE;
}

void CGUI_test::Destroy( )
{
	NTL_FUNCTION("CGUI_TEST::Destory");

	/*UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventPortalStartRes);
	UnLinkMsg(g_EventPortalInfoRes);
	UnLinkMsg(g_EventPortalAddRes);
	UnLinkMsg(g_EventPortalRes);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventDialog);*/

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CGUI_test::HandleEvents( RWS::CMsg& msg )
{	
}

RwInt32	CGUI_test::SwitchDialog(bool bOpen)
{
	return TRUE;
}

void CGUI_test::OnclickedBtnStatic( gui::CComponent* pComponent )
{

}

void CGUI_test::OnclickedBtnEdit( gui::CComponent* pComponent )
{

}

void CGUI_test::OnClickedBtnClose( gui::CComponent* pComponent )
{
	m_bButCheck = ( m_bButCheck ) ? false : true;

	if( m_bButCheck )
	{
		m_pStbTitle->SetText("버튼 클릭!");
		m_pDigStatic->Show(true);
		m_pDigEdit->Show(false);
	}
	else
	{
		m_pStbTitle->SetText("button click!");
		m_pDigStatic->Show(false);
		m_pDigEdit->Show(true);
	}
}

void CGUI_test::OnClickedBtnDlgEdit( gui::CComponent* pComponent )
{
	m_bdlgEdit = ( m_bdlgEdit ) ? false : true;

	if( m_bdlgEdit )
	{
		m_pBtnDlgEdit->SetText("Dlg 버튼 클릭11");
	}
	else
	{
		m_pBtnDlgEdit->SetText(m_pStbTitle->GetText());
	}

}

void CGUI_test::OnClickedBtnDlgStatic( gui::CComponent* pComponent )
{
	m_bdlgStatic = ( m_bdlgStatic ) ? false : true;
	

	if( m_bdlgStatic )
	{
		m_pBtnDlgStatic->SetText("Dlg 버튼 클릭22");
	}
	else
	{
		m_pBtnDlgStatic->SetText(m_pStbTitle->GetText());
	}
}

VOID CGUI_test::OnScrollChanged( RwInt32 nNewOffset )
{

}

void CGUI_test::OnEditInput()
{		
		m_pStbTitle->SetText(m_pInput->GetText());

		WCHAR* pwcText = (WCHAR*)m_pInput->GetText();

		GetAlarmManager()->AlarmMessage( pwcText, CAlarmManager::ALARM_TYPE_LOBBY_MSGBOX );

//		VOID	AlarmMessage( WCHAR* szText, RwUInt32 uType = ALARM_TYPE_CHAT_WARN, RwUInt32 uWorkID = MBW_NONE, RwUInt32 uMsgBoxStyle = MBTF_OK,
//			RwBool bPopup = TRUE, RwReal fShowTime = 0.0f, sMsgBoxData* pExData = NULL, std::list<sMsgBoxCustomBtn>* plistCustomBtn = NULL );

}