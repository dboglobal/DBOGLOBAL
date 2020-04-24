#include "precomp_dboclient.h"
#include "TutorialExampleGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEventFunc.h"

// dbo
#include "DboGlobal.h"
#include "DialogManager.h"

#define dTUTORIAL_FLASH_FOLDER		"\\tutorial\\"


CTutorialExampleGui::CTutorialExampleGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fSizeRate(0.f)
,m_iOffsetX(0)
,m_iOffsetY(0)
{

}

CTutorialExampleGui::~CTutorialExampleGui()
{

}

RwBool CTutorialExampleGui::Create()
{
	NTL_FUNCTION( "CTutorialExampleGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TutorialExample.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_TUTORIAL_FOCUS);

	m_pflashExample = (gui::CFlash*)GetComponent("flaExample");

	m_slotMovieEnd		= m_pflashExample->SigMovieEnd().Connect( this, &CTutorialExampleGui::OnMovieEnd );

	GetNtlGuiManager()->AddUpdateFunc( this );

	// default parameta
	m_ShowHide.fRemainTime		= 0.f;
	m_ShowHide.fElapsedTime		= 0.f;
	m_ShowHide.bShow			= FALSE;

	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTutorialExampleGui::OnMove );

	LinkMsg(g_EventResize);
	LinkMsg(g_EventTLDefaultSkin);
	LinkMsg(g_EventTLGuideIMG_ShowHide);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTutorialExampleGui::Destroy()
{
	NTL_FUNCTION("CTutorialExampleGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	FinishFlash();

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventTLDefaultSkin);
	UnLinkMsg(g_EventTLGuideIMG_ShowHide);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTutorialExampleGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pflashExample->Update(fElapsed);

	// 알파 변경
	if( m_ShowHide.fRemainTime > 0.f )
	{
		RwUInt8 byAlpha;
		RwReal fStartAlpha, fEndAlpha;

		m_ShowHide.fElapsedTime += fElapsed;
		if( m_ShowHide.fElapsedTime >= m_ShowHide.fRemainTime )
		{
			m_ShowHide.fElapsedTime	= m_ShowHide.fRemainTime;

			if( m_ShowHide.bShow == FALSE )
				GetDialogManager()->CloseDialog(DIALOG_TUTORIAL_EXAMPLE);
		}

		if( m_ShowHide.bShow )
		{
			fStartAlpha		= 0.f;
			fEndAlpha		= 255.f;
		}
		else
		{
			fStartAlpha		= 255.f;
			fEndAlpha		= 0.f;
		}

		byAlpha = (RwUInt8)CNtlMath::Interpolation(fStartAlpha, fEndAlpha, m_ShowHide.fElapsedTime/m_ShowHide.fRemainTime);

		m_pflashExample->SetAlpha(byAlpha);

		if( m_ShowHide.fElapsedTime == m_ShowHide.fRemainTime )
			m_ShowHide.fRemainTime = 0.f;
	}
}

RwBool CTutorialExampleGui::LoadFlash(const char* acFileName)
{
	std::string strFullName = dTUTORIAL_FLASH_FOLDER;
	strFullName += acFileName;

	if( m_pflashExample->Load(strFullName.c_str()) == false )
	{
		DBO_FAIL("Not eixst file : " << acFileName << ". You can continue the game.");
		FinishFlash();
		return FALSE;
	}

	LocateComponent();
	m_pflashExample->PlayMovie(TRUE);

	return TRUE;
}

VOID CTutorialExampleGui::LocateComponent()
{
	CRectangle rtRect = m_pflashExample->GetFrameResolution();
	RwInt32 iWidth = rtRect.GetWidth();
	RwInt32 iHeight = rtRect.GetHeight();

	rtRect.left		= (GetDboGlobal()->GetScreenWidth() - iWidth)/2 + m_iOffsetX;
	rtRect.right	= rtRect.left + iWidth;

	// 기획의 변경으로 화면 중앙 정렬에서 화면 하단 정렬로 변경
	rtRect.top		= GetDboGlobal()->GetScreenHeight() - (768 - ((768 - iHeight)/2 + m_iOffsetY));
	rtRect.bottom	= rtRect.top + iHeight;

	m_pflashExample->SetSize((RwInt32)(iWidth * m_fSizeRate), (RwInt32)(iHeight * m_fSizeRate));

	SetPosition(rtRect);
}

VOID  CTutorialExampleGui::FinishFlash()
{
	m_pflashExample->Unload();	
}

VOID CTutorialExampleGui::OnMovieEnd(gui::CComponent* pComponent)
{
	
}

VOID CTutorialExampleGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LocateComponent();
}

RwInt32 CTutorialExampleGui::SwitchDialog(bool bOpen)
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

VOID CTutorialExampleGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTutorialExampleGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		//SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocateComponent();
	}
	else if( msg.Id == g_EventTLDefaultSkin )
	{
		SNtlEventTLDefaultSkin* pEvent = reinterpret_cast<SNtlEventTLDefaultSkin*>( msg.pData );

		m_iOffsetX = (RwInt32)pEvent->vIMGOffset.x;
		m_iOffsetY = (RwInt32)pEvent->vIMGOffset.y;
	}
	else if( msg.Id == g_EventTLGuideIMG_ShowHide )
	{
		SNtlEventTLGuideIMG_ShowHide* pEvent = reinterpret_cast<SNtlEventTLGuideIMG_ShowHide*>( msg.pData );

		m_ShowHide.bShow			= pEvent->bShow;
		m_ShowHide.fRemainTime		= pEvent->fFadeInOutTime;
		m_ShowHide.fElapsedTime		= 0.f;

		if( pEvent->bShow )
		{
			m_fSizeRate = pEvent->fSizeRate;
			LoadFlash(pEvent->pIMGName);			
			GetDialogManager()->OpenDialog(DIALOG_TUTORIAL_EXAMPLE);
		}
	}

	NTL_RETURNVOID();
}