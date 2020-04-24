#include "precomp_dboclient.h"
#include "DBCSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSlApi.h"
#include "TableContainer.h"
#include "DragonBallTable.h"

// Sound
#include "GUISoundDefine.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "NtlWorldConceptDBC.h"
#include "DBOLogic.h"

//////////////////////////////////////////////////////////////////////////
// CDBCSideIconGui
//////////////////////////////////////////////////////////////////////////

CDBCSideIconGui::CDBCSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
,m_eIconImageType(E_ICON_NO_SEASON)
{
	m_bDBCComplete = FALSE;
}

CDBCSideIconGui::~CDBCSideIconGui()
{

}

RwBool CDBCSideIconGui::Create()
{
	NTL_FUNCTION( "CDBCSideIconGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\DBCSideIcon.srf", "gui\\DBCSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 비시즌 아이콘 버튼
	m_pBtnNoSeason = (gui::CButton*)GetComponent("btnDBCSideIconNoSeason");
	m_slotNoSeasonButton = m_pBtnNoSeason->SigClicked().Connect(this, &CDBCSideIconGui::OnIconButtonClicked);

	// 시즌 아이콘 버튼
	m_pBtnSeason = (gui::CButton*)GetComponent("btnDBCSideIconSeason");
	m_slotSeasonButton = m_pBtnSeason->SigClicked().Connect(this, &CDBCSideIconGui::OnIconButtonClicked);

	// 수집 완료 아이콘 버튼
	m_pBtnCollect = (gui::CButton*)GetComponent("btnDBCSideIconCollect");
	m_slotCollectButton = m_pBtnCollect->SigClicked().Connect(this, &CDBCSideIconGui::OnIconButtonClicked);

	LinkMsg(g_EventDBCShedule_Info);
	LinkMsg(g_EventSobCheckItemInOut);

	ResetSheduleInfo();

	NTL_RETURN(TRUE);
}

VOID CDBCSideIconGui::Destroy()
{
	NTL_FUNCTION("CDBCSideIconGui::Destroy");

	UnLinkMsg(g_EventDBCShedule_Info);
	UnLinkMsg(g_EventSobCheckItemInOut);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CDBCSideIconGui::OnSideViewClosed()
{
}

void CDBCSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_DBC, (VOID*)&m_eIconImageType);
}

VOID CDBCSideIconGui::SetIconImageType(eDBCSideIconType eType)
{
	m_eIconImageType = eType;

	m_pBtnNoSeason->Show(false);
	m_pBtnSeason->Show(false);
	m_pBtnCollect->Show(false);

	switch(eType)
	{
	case E_ICON_NO_SEASON:	m_pBtnNoSeason->Show(true); break;
	case E_ICON_SEASON:		m_pBtnSeason->Show(true); break;
	case E_ICON_COLLECT:	m_pBtnCollect->Show(true); break;
	default:			NTL_ASSERTFAIL("CDBCSideIconGui::SetIconImageType, Wrong type : " << eType);
	}
}

eDBCSideIconType CDBCSideIconGui::GetIconImageType()
{
	return m_eIconImageType;
}

VOID CDBCSideIconGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDBCSideIconGui::HandleEvents");

	if(msg.Id == g_EventDBCShedule_Info || msg.Id == g_EventSobCheckItemInOut)		// 서버로부터 정보를 받거나 인벤토리의 아이템이 변경되었을때
	{
		if(IsCollectComplete())					// 드래곤볼을 다 모았는지 체크한다.
		{
			if( GetIconImageType() != E_ICON_COLLECT )
			{
				Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
				SetIconImageType(E_ICON_COLLECT);
			}
			
			NTL_RETURNVOID();
		}
		else
		{
			ResetSheduleInfo();
		}
	}	

	NTL_RETURNVOID();
}

RwBool CDBCSideIconGui::IsCollectComplete() 
{
	// 인벤토리에서 드래곤볼을 검사한다.
	for(BYTE type = DRAGON_BALL_TYPE_BASIC; type < DRAGON_BALL_TYPE_COUNT; ++type)
	{
		sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)API_GetTableContainer()->GetDragonBallTable()->GetDBTbldat(type);
		if(!pData)
			continue;

		m_bDBCComplete = TRUE;

		for(int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
		{
			if(Logic_FindInventoryItemByTableId(pData->aBallTblidx[i]) == NULL)
			{
				m_bDBCComplete = FALSE;
				break;
			}
		}
		
		if(m_bDBCComplete)
			return m_bDBCComplete;		
	}

	return m_bDBCComplete;
}

VOID CDBCSideIconGui::ResetSheduleInfo() 
{
	// Kell's comment : 드래곤볼 수집 기간이 아닌 경우에는 출력하지 않음으로 변경( 08. 07. 03 )
	if(GetDboGlobal()->GetDBCScheduleInfo()->bIsAlive)	// 기간중인지 유무
	{
		SetIconImageType(E_ICON_SEASON);

		if( IsShow() == FALSE )
		{
			Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
			Show( true );
		}
	}
	else
	{
		/*SetIconImageType(E_ICON_NO_SEASON);*/
		Show( false );
	}
}



//////////////////////////////////////////////////////////////////////////
// CDBCSideViewGui
//////////////////////////////////////////////////////////////////////////

CDBCSideViewGui::CDBCSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
{
}

CDBCSideViewGui::~CDBCSideViewGui()
{

}

RwBool CDBCSideViewGui::Create()
{
	NTL_FUNCTION( "CDBCSideViewGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\DBCSideView.srf", "gui\\DBCSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 닫기 버튼
	m_pExitButton = (gui::CButton*)GetComponent("btnClose");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect(this, &CDBCSideViewGui::OnExitClicked);

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DBCSideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DBCSideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DBCSideView.srf", "srfDialogBackDown"));	

	// Side View 제목
	rect.SetRectWH(20, 20, 250, 14);
	m_pTitle = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pTitle->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);	    
	m_pTitle->Enable(false);	

	// 알림 메세지
	rect.SetRectWH(20, 50, 250, 14);
	m_pNotify = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pNotify->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNotify->SetTextColor( RGB(255, 255, 255) );	
	m_pNotify->Enable(false);

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CDBCSideViewGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect(this, &CDBCSideViewGui::OnMove);
	m_slotResize		= m_pThis->SigMove().Connect(this, &CDBCSideViewGui::OnResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDBCSideViewGui::Destroy()
{
	NTL_FUNCTION("CDBCSideViewGui::Destroy");


	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CDBCSideViewGui::OnPressESC()
{

}

void CDBCSideViewGui::OnSideViewOpen(const void* pData)
{
	eDBCSideIconType* pType = (eDBCSideIconType*)pData;
	SetState(*pType);		

	Show(true);
}

void CDBCSideViewGui::OnSideViewClose()
{
	Show(false);
}

void CDBCSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	RwInt32 iHeight = 100;
	m_pThis->SetHeight(iHeight);
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - iHeight);
}

VOID CDBCSideViewGui::SetState(eDBCSideIconType eState)
{
	std::wstring wstrTitle;
	std::wstring wstrTerm;

	switch( eState )
	{
	case E_ICON_NO_SEASON:		
		wstrTitle = GetDisplayStringManager()->GetString("DST_DBC_NO_SEASON");
		m_pTitle->SetTextColor(RGB(152, 255, 210));
		m_pTitle->SetText(wstrTitle.c_str());
		m_pNotify->Clear();
		break;		
	case E_ICON_SEASON:
		{
			wstrTitle = GetDisplayStringManager()->GetString("DST_DBC_SEASON");
			m_pTitle->SetTextColor(RGB(43, 255, 243));
			m_pTitle->SetText(wstrTitle.c_str());

			// 기간 : -월 -일 ~ -월 -일			
			WCHAR buf[64] = {0,};
			tm tmStart, tmEnd;
			localtime_s(&tmStart, &GetDboGlobal()->GetDBCScheduleInfo()->nStartTime);
			localtime_s(&tmEnd, &GetDboGlobal()->GetDBCScheduleInfo()->nEndTime);						
			swprintf_s(buf, L"%s : %d%s %d%s ~ %d%s %d%s", GetDisplayStringManager()->GetString("DST_TERM"), 
										 tmStart.tm_mon + 1, GetDisplayStringManager()->GetString("DST_TIME_MONTH"),
										 tmStart.tm_mday, GetDisplayStringManager()->GetString("DST_TIME_DAY"),
										 tmEnd.tm_mon + 1, GetDisplayStringManager()->GetString("DST_TIME_MONTH"),
										 tmEnd.tm_mday, GetDisplayStringManager()->GetString("DST_TIME_DAY"));
			m_pNotify->SetText(buf);
			
		}
		break;		
	case E_ICON_COLLECT:
		wstrTitle = GetDisplayStringManager()->GetString("DST_DBC_COLLECT_COMPLETE");
		wstrTerm = GetDisplayStringManager()->GetString("DST_DBC_REWARD_COMMENT");
		m_pTitle->SetTextColor(RGB(198, 255, 0));
		m_pTitle->SetText(wstrTitle.c_str());
		m_pNotify->SetText(wstrTerm.c_str());
		break;
	default:
		NTL_ASSERTFAIL("CDBCSideViewGui::SetState, Wrong index : " << eState);
		break;
	}	
}

VOID CDBCSideViewGui::LocateComponent()
{
	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_BackPanel.SetRect( rtScreen );
}

VOID CDBCSideViewGui::OnExitClicked(gui::CComponent* pComponent)
{
	Show(false);
}


VOID CDBCSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LocateComponent();
}

VOID CDBCSideViewGui::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	LocateComponent();
}

VOID CDBCSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

VOID CDBCSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDBCSideViewGui::HandleEvents");

	
	NTL_RETURNVOID();
}