#include "precomp_dboclient.h"
#include "TMQStatusGui.h"

// core
#include "NtlDebug.h"

// share
#include "TimeQuestTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventfunc.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlWorldconceptTMQ.h"

// dbo
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DboEventGenerator.h"
#include "SideDialogManager.h"
#include "LittleQuestContainer.h"

// test
#include "GUISoundDefine.h"

namespace
{
	#define dCOUPON_ICON_NAME			"TMQ_Coupon.png"
};


CTMQStatusGui* CTMQStatusGui::m_pInstance = NULL;

CTMQStatusGui::CTMQStatusGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pTMQQuestContainer(NULL)
{
}

CTMQStatusGui::~CTMQStatusGui()
{
	Destroy();
}

RwBool CTMQStatusGui::Create()
{
	NTL_FUNCTION( "CTMQStatusGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\TMQStatus.srf", "gui\\TMQStatus.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pDialogName = (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pInfo = (gui::CStaticBox*)GetComponent("stbInfo");
	m_pPoint = (gui::CStaticBox*)GetComponent("stbPoint");
	m_pCurPoint = (gui::CStaticBox*)GetComponent("stbCurPoint");
	m_pQuest = (gui::CStaticBox*)GetComponent("stbQuest");
	m_pQuestTitle = (gui::CStaticBox*)GetComponent("stbQuestTitle");
	m_pQuestAim_1 = (gui::CStaticBox*)GetComponent("stbQuestAim_1");
	m_pQuestAim_2 = (gui::CStaticBox*)GetComponent("stbQuestAim_2");
	m_pQuestAim_3 = (gui::CStaticBox*)GetComponent("stbQuestAim_3");
	m_pQuestAim_4 = (gui::CStaticBox*)GetComponent("stbQuestAim_4");

	m_pDialogName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_TITLE"));
	m_pInfo->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_INFO"));
	m_pPoint->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_POINT"));
	m_pCurPoint->SetText(L"0%");
	m_pQuest->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_QUEST"));

	// 스테이지 정보
	m_pStageButton = (gui::CButton*)GetComponent( "btnStageInfo" );
	m_pStageButton->SetToolTip(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_STATUS_INFO"));
	m_slotStageButton = m_pStageButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_InfoButton);

	// 최소화 버튼
	m_pMinimizeButton = (gui::CButton*)GetComponent( "btnMinimize" );
	m_slotMinimizeButton = m_pMinimizeButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_MinimizeButton);

	// 시나리오 버튼
	m_pSenarioButton = (gui::CButton*)GetComponent( "btnSenario" );
	m_pSenarioButton->SetToolTip(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_SENARIO"));
	m_slotSenarioButton = m_pSenarioButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_SenarioButton);	

	// 다음 퀘스트 버튼
	m_pNextButton = (gui::CButton*)GetComponent( "btnNextQuest" );
	m_slotNextButton = m_pNextButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_NextButton);

	// 시나리오 버튼 업데이트 알림
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfButtonEffect" );
	m_SenarioButtonFlicker.SetSurface( surface );
	m_SenarioButtonFlicker.SetTime( 1000.0f, 0.5f );
	m_SenarioButtonFlicker.SetAlpha( 0, 255 );

	// 스테이지 버튼 업데이트 알림
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfButtonEffect" );
	m_StageButtonFlicker.SetSurface( surface );
	m_StageButtonFlicker.SetTime( 1000.0f, 0.5f );
	m_StageButtonFlicker.SetAlpha( 0, 255 );

	// TMQ 포인트 게이지
	m_pTMQGauge = (gui::CProgressBar*)GetComponent( "TMQGauge" );
	m_pTMQGauge->SetToolTip(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_EXPLAIN_POINT"));

	// TMQ 포인트 배경
	m_srfGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfGaugeBack" ));
	m_srfGaugeBack.SetPositionfromParent(8, 116);

	// 쿠폰 슬롯
	m_CouponSlot.Create(m_pThis, SDIALOG_TMQ_STATUS, REGULAR_SLOT_SIMPLE_ICON, SDS_COUNT);
	m_CouponSlot.SetIcon(dCOUPON_ICON_NAME, 0);
	m_CouponSlot.SetPosition_fromParent(122, 93);

	// 구분선
	m_srfTMQContourLine_1.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));
	m_srfTMQContourLine_2.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));
	m_srfTMQContourLine_3.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));

	m_srfTMQContourLine_1.SetPositionfromParent(0, 30);
	m_srfTMQContourLine_2.SetPositionfromParent(0, 72);
	m_srfTMQContourLine_3.SetPositionfromParent(0, 135);

	// 쿠폰 슬롯 배경
	m_srfCouponSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfCouponSlot" ));
	m_srfCouponSlot.SetPositionfromParent(120, 91);

	m_pTMQQuestContainer = NTL_NEW CLittleQuestContainer;
	if( !m_pTMQQuestContainer->Create(eQUEST_SORT_TYPE_TIMEMACHINE, m_pQuestTitle, m_pQuestAim_1, m_pQuestAim_2,
									  m_pQuestAim_3, m_pQuestAim_4) )
		NTL_RETURN(FALSE);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQStatusGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTMQStatusGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTMQStatusGui::OnMouseLeave );	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTMQStatusGui::OnPaint );
	m_slotPostPaint		= m_pStageButton->SigPaint().Connect( this, &CTMQStatusGui::OnPostPaint );

	Show(false);	

	LinkMsg(g_EventCharReady);
	LinkMsg(g_EventTMQNotify);

	NTL_RETURN(TRUE);
}

VOID CTMQStatusGui::Destroy()
{
	NTL_FUNCTION("CTMQStatusGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	if( m_pTMQQuestContainer )
	{
		m_pTMQQuestContainer->Destroy();
		NTL_DELETE(m_pTMQQuestContainer);
	}

	m_CouponSlot.Destroy();

	UnLinkMsg(g_EventCharReady);
	UnLinkMsg(g_EventTMQNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQStatusGui::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CTMQStatusGui("TMQStatusGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);
	GetSideDialogManager()->RegistDialog( SDIALOG_TMQ_STATUS, m_pInstance, &CTMQStatusGui::SwitchDialog );
}

VOID CTMQStatusGui::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_TMQ_STATUS);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CTMQStatusGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_SenarioButtonFlicker.Update(fElapsed);
	m_StageButtonFlicker.Update(fElapsed);
}

VOID CTMQStatusGui::SetTMQPoint(RwInt32 iPoint)
{
	RwInt32 iCurPoint = iPoint % DBO_TIMEQUEST_COUNPON_CHANGE_POINT;

	// m_pCurPoint (with a maximum value of 100) and m_pTMQGauge (with a maximum value of 1000) as a percentage
	m_pCurPoint->Format(L"%i%%", iCurPoint/10);
	m_pTMQGauge->SetPos(iCurPoint);
}

VOID CTMQStatusGui::ShowBonusTime(RwReal fBonusTime)
{
}

VOID CTMQStatusGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_TMQBOARD )
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CTMQStatusGui::OnClicked_MinimizeButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_TMQ_STATUS, DIALOG_SIDEDIALOG_CONTROLLER);
}

VOID CTMQStatusGui::OnClicked_SenarioButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlTMQ* pTMQ = pAvatar->GetTMQ();
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
	if( !pTIMEQUEST_TBLDAT )
	{
		DBO_FAIL("Not exist TMQ table");
		return;
	}

	RwUInt32 uiFirstNum = pTIMEQUEST_TBLDAT->tblidx;

	m_SenarioButtonFlicker.EndProc();

	// In the scenario, the value of the second value is 0
	CDboEventGenerator::QuestMessage(uiFirstNum, 0);
}

VOID CTMQStatusGui::OnClicked_InfoButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlTMQ* pTMQ = pAvatar->GetTMQ();
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
	if( !pTIMEQUEST_TBLDAT )
	{
		DBO_FAIL("Not exist TMQ table");
		return;
	}

	RwUInt32 uiFirstNum = pTIMEQUEST_TBLDAT->tblidx;

	m_StageButtonFlicker.EndProc();

	CDboEventGenerator::QuestMessage(uiFirstNum, pTMQ->GetStageNum());
}

VOID CTMQStatusGui::OnClicked_NextButton(gui::CComponent* pComponent)
{
	m_pTMQQuestContainer->NextQuest();
}

VOID CTMQStatusGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtRect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	rtRect = m_pSenarioButton->GetPosition();
	m_SenarioButtonFlicker.SetCenterPos(rtScreen.left + rtRect.left + rtRect.GetWidth()/2,
										rtScreen.top + rtRect.top + rtRect.GetHeight()/2);

	rtRect = m_pStageButton->GetPosition();
	m_StageButtonFlicker.SetCenterPos(rtScreen.left + rtRect.left + rtRect.GetWidth()/2,
									  rtScreen.top + rtRect.top + rtRect.GetHeight()/2);

	m_srfGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfTMQContourLine_1.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTMQContourLine_2.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTMQContourLine_3.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfCouponSlot.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_CouponSlot.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CTMQStatusGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_CouponSlot.PtInRect(nX, nY) && m_CouponSlot.IsOnlyUsableIcon() )
	{
		if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_JUST_WTEXT )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
												rtScreen.left + m_CouponSlot.GetX_fromParent(),
												rtScreen.top + m_CouponSlot.GetY_fromParent(),
												(VOID*)GetDisplayStringManager()->GetString("DST_TMQ_STATUS_EXPLAIN_COUPON"), SDIALOG_TMQ_STATUS);
		}
		
		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQStatusGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQStatusGui::OnPaint()
{
	m_srfCouponSlot.Render();

	m_srfGaugeBack.Render();

	m_srfTMQContourLine_1.Render();
	m_srfTMQContourLine_2.Render();
	m_srfTMQContourLine_3.Render();	

	if (m_CouponSlot.GetCount() > 0)
		m_CouponSlot.Paint();
}

VOID CTMQStatusGui::OnPostPaint()
{
	m_SenarioButtonFlicker.Render();
	m_StageButtonFlicker.Render();
}

RwInt32 CTMQStatusGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE) == FALSE )
			return -1;

		CNtlWorldConceptTMQ* pTMQWorldConcept = reinterpret_cast<CNtlWorldConceptTMQ*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TIME_MACHINE) );
		if( !pTMQWorldConcept )
		{
			DBO_FAIL("Not exist TMQ world concept");
			return -1;
		}

		if( pTMQWorldConcept->GetTMQState() < WORLD_STATE_TMQ_WAIT ||
			pTMQWorldConcept->GetTMQState() >= WORLD_STATE_TMQ_END )
			return -1;

		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();
	}

	return 1;
}

VOID CTMQStatusGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQStatusGui::HandleEvents");

	if( msg.Id == g_EventCharReady )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE) == FALSE )
			NTL_RETURNVOID();


		if( GetSideDialogManager()->IsOpenDialog(SDIALOG_TMQ_STATUS) == FALSE )
		{
			CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
			if( !pTMQ )
			{
				DBO_FAIL("Not exist TMQ table");
				NTL_RETURNVOID();
			}

			// Open TMQ related window in TMQ status
			GetSideDialogManager()->OpenDialog(SDIALOG_TMQ_STATUS);
		}
	}
	else if( msg.Id == g_EventTMQNotify )
	{
		SNtlEventTMQNotify* pEvent = reinterpret_cast<SNtlEventTMQNotify*>( msg.pData );

		switch(pEvent->uiEventType)
		{
			case TET_INFOMATION_NFY:
			{
				if( pEvent->uiValue == TIMEQUEST_INFORMATION_TYPE_SCENARIO )
				{
					OnClicked_SenarioButton(NULL);
					m_SenarioButtonFlicker.StartProc(TRUE);
				}
				else
				{
					OnClicked_InfoButton(NULL);
					m_StageButtonFlicker.StartProc(TRUE);
				}
			}
			break;
			case TET_COUPON_COUNT:
			{
				m_CouponSlot.SetCount(pEvent->uiValue);
			}
			break;
			case TET_TMQ_POINT:
			{
				SetTMQPoint(pEvent->uiValue);
			}
			break;
			case TET_GAME_TIME:
			{
				ShowBonusTime((RwReal)pEvent->uiValue);
			}
			break;
		}
	}

	NTL_RETURNVOID();
}