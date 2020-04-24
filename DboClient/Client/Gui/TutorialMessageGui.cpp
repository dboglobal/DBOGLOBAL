#include "precomp_dboclient.h"
#include "TutorialMessageGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlWorldConceptTutorial.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"

#define dBACKGROUND_FLASH					"\\tutorial\\Tu_bg.swf"


CTutorialMessageGui::CTutorialMessageGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CTutorialMessageGui::~CTutorialMessageGui()
{

}

RwBool CTutorialMessageGui::Create()
{
	NTL_FUNCTION( "CTutorialMessageGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TutorialMessage.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_TUTORIAL_FOCUS);

	m_pflashBackground = (gui::CFlash*)GetComponent("flaExample");

	m_pMessage = (gui::CHtmlBox*)GetComponent("hbxMessage");

	m_pSkipButton = (gui::CButton*)GetComponent( "btnSkip" );
	m_slotSkipButton = m_pSkipButton->SigClicked().Connect(this, &CTutorialMessageGui::OnClick_SkipButton);	

	// default parameta
	m_ShowHideMessage.fRemainTime		= 0.f;
	m_ShowHideMessage.fElapsedTime		= 0.f;
	m_ShowHideMessage.bShow				= FALSE;

	m_ChangeColor.fRemainTime	= 0.f;
	m_ChangeColor.fElapsedTime	= 0.f;
	m_ChangeColor.srcColor.red	= 0;
	m_ChangeColor.srcColor.green= 0;
	m_ChangeColor.srcColor.blue	= 0;
	m_ChangeColor.srcColor.blue	= 255;
	m_ChangeColor.destColor		= m_ChangeColor.srcColor;


	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTutorialMessageGui::OnMove );

	LinkMsg(g_EventTLDefaultSkin);
	LinkMsg(g_EventTLGuideMsg_ShowHide);
	LinkMsg(g_EventTLGuideMsg_ChangeColor);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTutorialMessageGui::Destroy()
{
	NTL_FUNCTION("CTutorialMessageGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventTLDefaultSkin);
	UnLinkMsg(g_EventTLGuideMsg_ShowHide);
	UnLinkMsg(g_EventTLGuideMsg_ChangeColor);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTutorialMessageGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pflashBackground->Update(fElapsed);


	// 메세지 알파 변경
	if( m_ShowHideMessage.fRemainTime > 0.f )
	{
		RwUInt8 byAlpha;

		m_ShowHideMessage.fElapsedTime += fElapsed;
		if( m_ShowHideMessage.fElapsedTime >= m_ShowHideMessage.fRemainTime )
		{
			m_ShowHideMessage.fElapsedTime = m_ShowHideMessage.fRemainTime;
		}

		if( m_ShowHideMessage.bShow )
		{
			byAlpha = (RwUInt8)CNtlMath::Interpolation(0.f, 255.f, m_ShowHideMessage.fElapsedTime/m_ShowHideMessage.fRemainTime);
		}
		else
		{
			byAlpha = (RwUInt8)CNtlMath::Interpolation(255.f, 0.f, m_ShowHideMessage.fElapsedTime/m_ShowHideMessage.fRemainTime);
		}
		

		m_pMessage->SetAlpha(byAlpha);

		if( m_ShowHideMessage.fElapsedTime == m_ShowHideMessage.fRemainTime )
		{
			m_ShowHideMessage.fRemainTime = 0.f;

			if( !m_ShowHideMessage.bShow )
				m_pflashBackground->Unload();
		}
	}	

	// 색상 변경
	if( m_ChangeColor.fRemainTime > 0.f )
	{
		m_ChangeColor.fElapsedTime += fElapsed;
		if( m_ChangeColor.fElapsedTime >= m_ChangeColor.fRemainTime )
			m_ChangeColor.fElapsedTime = m_ChangeColor.fRemainTime;


		//RwRGBA curColor = CNtlMath::Interpolation(m_ChangeColor.srcColor, m_ChangeColor.destColor, m_ChangeColor.fElapsedTime/m_ChangeColor.fRemainTime);

		if( m_ChangeColor.fElapsedTime == m_ChangeColor.fRemainTime )
			m_ChangeColor.fRemainTime = 0.f;
	}
}

VOID CTutorialMessageGui::OnClick_SkipButton(gui::CComponent* pComponent)
{
	CNtlWorldConceptTutorial* pWorldConcept = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( !pWorldConcept )
		return;

	pWorldConcept->NextStep();
}

VOID CTutorialMessageGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	// 기획의 변경으로 화면 중앙 정렬에서 화면 상단 정렬로 변경
	CRectangle rtFlashBackground = m_pflashBackground->GetFrameResolution();
	
	CRectangle rtRect, rtButton;

	rtRect.left		= (GetDboGlobal()->GetScreenWidth() - rtFlashBackground.GetWidth())/2 + m_DefaultMessageStyle.iOffsetBackgroundX;
	rtRect.top		= GetDboGlobal()->GetScreenHeight()/2 + m_DefaultMessageStyle.iOffsetBackgroundY;
	rtRect.right	= rtRect.left + rtFlashBackground.GetWidth();
	rtRect.bottom	= rtRect.top + rtFlashBackground.GetHeight();
	
	m_pThis->SetPosition(rtRect);
	

	rtButton.left	= 0;
	rtButton.top	= 0;
	rtButton.right	= rtFlashBackground.GetWidth();
	rtButton.bottom	= rtFlashBackground.GetHeight();

	m_pSkipButton->SetPosition(rtButton);
	m_pflashBackground->SetPosition(rtButton);


	RwInt32 iRealMeesageX = (GetDboGlobal()->GetScreenWidth() - m_pMessage->GetWidth())/2 + m_DefaultMessageStyle.iOffsetX;
	RwInt32 iRealMeesageY = GetDboGlobal()->GetScreenHeight()/2 + m_DefaultMessageStyle.iOffsetY;
	m_pMessage->SetPosition(iRealMeesageX - rtRect.left, iRealMeesageY - rtRect.top);
}

RwInt32 CTutorialMessageGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CTutorialMessageGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTutorialMessageGui::HandleEvents");

	if( msg.Id == g_EventTLDefaultSkin )
	{
		SNtlEventTLDefaultSkin* pEvent = reinterpret_cast<SNtlEventTLDefaultSkin*>( msg.pData );

		m_DefaultMessageStyle.strFontName			= pEvent->pFontName;
		m_DefaultMessageStyle.FontColor				= pEvent->rgbFontColor;
		m_DefaultMessageStyle.FontBackgroundColor	= pEvent->rgbFontBackColor;
		m_DefaultMessageStyle.byFontSize			= (RwUInt8)pEvent->nFontSize;
		m_DefaultMessageStyle.byAlign				= (RwUInt8)pEvent->uiFontAllign;
		m_DefaultMessageStyle.iOffsetX				= (RwInt32)pEvent->vFontOffset.x;
		m_DefaultMessageStyle.iOffsetY				= (RwInt32)pEvent->vFontOffset.y;
		m_DefaultMessageStyle.iOffsetBackgroundX	= (RwInt32)pEvent->vMsgLayerOffset.x;
		m_DefaultMessageStyle.iOffsetBackgroundY	= (RwInt32)pEvent->vMsgLayerOffset.y;		
	}
	else if( msg.Id == g_EventTLGuideMsg_ShowHide )
	{
		SNtlEventTLGuideMsg_ShowHide* pEvent = reinterpret_cast<SNtlEventTLGuideMsg_ShowHide*>( msg.pData );
		CQuestTextDataTable* pQuestTable = API_GetTableContainer()->GetQuestTextDataTable();		


		if( FALSE == pEvent->bShow )
		{
			CNtlWorldConceptTutorial* pWorldConcept = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
			if( pWorldConcept && pWorldConcept->IsStepSkipping() )
			{
				m_ShowHideMessage.fRemainTime	= 0.f;
				m_ShowHideMessage.fElapsedTime	= 0.f;
				m_ShowHideMessage.bShow			= FALSE;

				GetDialogManager()->CloseDialog(DIALOG_TUTORIAL_MESSAGE);
				NTL_RETURNVOID();
			}
		}


		// 기존에 텍스트가 보여지고 있지 않았다면 배경 플래쉬를 로딩한다
		if( pEvent->bShow )
		{
			if( !m_ShowHideMessage.bShow )
			{
				m_pflashBackground->Load(dBACKGROUND_FLASH, true);
				m_pflashBackground->PlayMovie(TRUE);

				CRectangle rtFlashBackground = m_pflashBackground->GetFrameResolution();
				rtFlashBackground.right		-= 25;
				rtFlashBackground.bottom	-= 20;

				m_pMessage->SetPosition(rtFlashBackground);

				OnMove(GetWidth(), GetHeight());
			}
		}


		m_pMessage->Clear();

		sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTable->FindData(pEvent->uiTextTblIdx) );
		if( !pQUEST_TEXT_DATA_TBLDAT )
		{
			WCHAR awcBuffer[256];
			swprintf_s(awcBuffer, 256, L"Not exist quest text table data of index : %u", pEvent->uiTextTblIdx);
			m_pMessage->SetHtmlFromMemory(awcBuffer, wcslen(awcBuffer));
		}
		else
		{
			m_pMessage->SetHtmlFromMemory(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), wcslen(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str()));
		}


		m_ShowHideMessage.fRemainTime	= pEvent->fFadeInOutTime;
		m_ShowHideMessage.fElapsedTime	= 0.f;
		m_ShowHideMessage.bShow			= pEvent->bShow;

		GetDialogManager()->OpenDialog(DIALOG_TUTORIAL_MESSAGE);
	}
	else if( msg.Id == g_EventTLGuideMsg_ChangeColor )
	{
		SNtlEventTLGuideMsg_ChangeColor* pEvent = reinterpret_cast<SNtlEventTLGuideMsg_ChangeColor*>( msg.pData );

		m_ChangeColor.fRemainTime	= pEvent->fFadeInOutTime;
		m_ChangeColor.fElapsedTime	= 0.f;
		m_ChangeColor.srcColor		= m_ChangeColor.destColor;
		m_ChangeColor.destColor		= pEvent->rgbFontColor;
	}

	NTL_RETURNVOID();
}