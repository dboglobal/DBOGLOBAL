#include "precomp_dboclient.h"
#include "TutorialConfirmGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"

// dbo
#include "DboGlobal.h"
#include "DialogManager.h"

#define dFLASH_CONFIRM_NAME			"\\tutorial\\Tu_OK.swf"


CTutorialConfirmGui::CTutorialConfirmGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iOffsetX(0)
,m_iOffsetY(0)
{
}

CTutorialConfirmGui::~CTutorialConfirmGui()
{

}

RwBool CTutorialConfirmGui::Create()
{
	NTL_FUNCTION( "CTutorialConfirmGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TutorialConfirm.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_TUTORIAL_FOCUS);

	m_pflashConfirm = (gui::CFlash*)GetComponent("flaConfirm");

	GetNtlGuiManager()->AddUpdateFunc( this );

	// default value
	m_ShowHide.bShow		= FALSE;
	m_ShowHide.fElapsedTime	= 0.f;
	m_ShowHide.fRemainTime	= 0.f;

	LinkMsg(g_EventResize);
	LinkMsg(g_EventTLDefaultSkin);
	LinkMsg(g_EventTLGuideIMGOk_ShowHide);

	NTL_RETURN(TRUE);
}

VOID CTutorialConfirmGui::Destroy()
{
	NTL_FUNCTION("CTutorialConfirmGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	FinishFlash();

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventTLDefaultSkin);
	UnLinkMsg(g_EventTLGuideIMGOk_ShowHide);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTutorialConfirmGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pflashConfirm->Update(fElapsed);

	if( m_ShowHide.fRemainTime > 0.f )
	{
		RwUInt8 byAlpha;

		m_ShowHide.fElapsedTime += fElapsed;
		if( m_ShowHide.fElapsedTime >= m_ShowHide.fRemainTime )
		{
			m_ShowHide.fElapsedTime	= m_ShowHide.fRemainTime;

			if( m_ShowHide.bShow == FALSE )
				GetDialogManager()->CloseDialog(DIALOG_TUTORIAL_CONFIRM);
		}

		if( m_ShowHide.bShow )
		{
			byAlpha = (RwUInt8)CNtlMath::Interpolation(0, 255, m_ShowHide.fElapsedTime/m_ShowHide.fRemainTime);
		}
		else
		{
			byAlpha = (RwUInt8)CNtlMath::Interpolation(255, 0, m_ShowHide.fElapsedTime/m_ShowHide.fRemainTime);
		}

		
		m_pflashConfirm->SetAlpha(byAlpha);

		if( m_ShowHide.fElapsedTime == m_ShowHide.fRemainTime )
			m_ShowHide.fRemainTime = 0.f;
	}
}

RwBool CTutorialConfirmGui::LoadFlash(const char* acFileName)
{
	if( m_pflashConfirm->Load(acFileName) == false )
	{
		DBO_FAIL("Not eixst file : " << acFileName << ". You can continue the game.");
		FinishFlash();
		return FALSE;
	}

	LocateComponent();
	m_pflashConfirm->PlayMovie(TRUE);

	return TRUE;
}

VOID CTutorialConfirmGui::LocateComponent()
{
	CRectangle rtRect = m_pflashConfirm->GetFrameResolution();
	RwInt32 iWidth = rtRect.GetWidth();
	RwInt32 iHeight = rtRect.GetHeight();

	rtRect.left		= (GetDboGlobal()->GetScreenWidth() - iWidth)/2 + m_iOffsetX;
	rtRect.right	= rtRect.left + iWidth;
	rtRect.top		= (GetDboGlobal()->GetScreenHeight() - iHeight)/2 + m_iOffsetY;
	rtRect.bottom	= rtRect.top + iHeight;

	m_pflashConfirm->SetSize(iWidth, iHeight);

	SetPosition(rtRect);
}

VOID  CTutorialConfirmGui::FinishFlash()
{
	m_pflashConfirm->Unload();	
}

RwInt32 CTutorialConfirmGui::SwitchDialog(bool bOpen)
{
	if(bOpen)
	{
		Show(true);
	}
	else
	{
		Show(false);
		FinishFlash();
	}

	return 1;
}

VOID CTutorialConfirmGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTutorialConfirmGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		//SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocateComponent();
	}
	else if( msg.Id == g_EventTLDefaultSkin )
	{
		SNtlEventTLDefaultSkin* pEvent = reinterpret_cast<SNtlEventTLDefaultSkin*>( msg.pData );

		m_iOffsetX = (RwInt32)pEvent->vOkOffset.x;
		m_iOffsetY = (RwInt32)pEvent->vOkOffset.y;
	}
	else if( msg.Id == g_EventTLGuideIMGOk_ShowHide )
	{
		SNtlEventTLGuideIMGOk_ShowHide* pEvent = reinterpret_cast<SNtlEventTLGuideIMGOk_ShowHide*>( msg.pData );

		m_ShowHide.bShow			= pEvent->bShow;
		m_ShowHide.fRemainTime		= pEvent->fFadeInOutTime;

		if( pEvent->bShow )
		{
			LoadFlash(dFLASH_CONFIRM_NAME);
			GetDialogManager()->OpenDialog(DIALOG_TUTORIAL_CONFIRM);
		}
	}

	NTL_RETURNVOID();
}