#include "precomp_dboclient.h"
#include "NameNotifyGui.h"

// core
#include "NtlDebug.h"

/// Sound
#include "GuiSoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// dbo
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "Dboglobal.h"

#define dTIME_FADE_IN		0.01f
#define dTIME_IDLE			5.f
#define dTIME_FADE_OUT		2.f

#define dALPHA_MIN			0
#define dALPHA_MAX			255


CNameNotifyGui::CNameNotifyGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fElapsedTime(0.f)
,m_pStateUpdate(NULL)
{
}

CNameNotifyGui::~CNameNotifyGui()
{
}

RwBool CNameNotifyGui::Create()
{
	NTL_FUNCTION( "CNameNotifyGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\NameNotify.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	// ¸Þ¼¼Áö
	rect.SetRectWH(0, 0, m_pThis->GetWidth(), m_pThis->GetHeight());
	m_pMessageStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pMessageStatic->CreateFontStd(DEFAULT_FONT, 120, DEFAULT_FONT_ATTR);
	m_pMessageStatic->SetTextColor( RGB(255, 219, 71) );
	m_pMessageStatic->SetEffectMode(TE_OUTLINE);
	m_pMessageStatic->SetEffectColor(DEFAULT_OUTLINE_EFFECT_VALUE);
	m_pMessageStatic->Enable(false);
	
	PositionAlign(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	LinkMsg(g_EventResize);
	LinkMsg(g_EventNameNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CNameNotifyGui::Destroy()
{
	NTL_FUNCTION("CNameNotifyGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventNameNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CNameNotifyGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_fElapsedTime += fElapsed;
	(this->*m_pStateUpdate)();
}

VOID CNameNotifyGui::Update_FadeIn()
{
	RwReal fDelta = m_fElapsedTime/(RwReal)dTIME_FADE_IN;

	if( m_fElapsedTime >= dTIME_FADE_IN )
	{	
		fDelta = 1.f;
		m_pStateUpdate = &CNameNotifyGui::Update_Idle;		
	}

	RwReal fCurAlpha = CNtlMath::Interpolation((RwReal)dALPHA_MIN, (RwReal)dALPHA_MAX, fDelta);
	m_pMessageStatic->SetAlpha((RwUInt8)fCurAlpha);

	if( fDelta == 1.f )
		m_fElapsedTime = 0.f;
}

VOID CNameNotifyGui::Update_Idle()
{
	if( m_fElapsedTime >= dTIME_IDLE )
	{
		m_fElapsedTime = 0.f;
		m_pStateUpdate = &CNameNotifyGui::Update_FadeOut;
	}
}

VOID CNameNotifyGui::Update_FadeOut()
{
	RwReal fDelta = m_fElapsedTime/(RwReal)dTIME_FADE_OUT;

	if( m_fElapsedTime >= dTIME_FADE_OUT )
	{
		fDelta = 1.f;
		m_pStateUpdate = NULL;
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
		Show(false);
		return;
	}

	RwReal fCurAlpha = CNtlMath::Interpolation((RwReal)dALPHA_MAX, (RwReal)dALPHA_MIN, fDelta);
	m_pMessageStatic->SetAlpha((RwUInt8)fCurAlpha);
}

VOID CNameNotifyGui::PositionAlign(RwInt32 iScreenWidth, RwInt32 iScreenHeight)
{
	RwReal fY = (RwReal)iScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fNameNotiry_YPosRate;
	m_pThis->SetPosition( (iScreenWidth - m_pThis->GetWidth())/2, (RwInt32)fY );
}

VOID CNameNotifyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNameNotifyGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign(pEvent->iWidht, pEvent->iHeight);
	}
	else if( msg.Id == g_EventNameNotify )
	{
		SDboEventNameNotify* pEvent = reinterpret_cast<SDboEventNameNotify*>( msg.pData );

		m_fElapsedTime = 0.f;
		m_pMessageStatic->SetAlpha(dALPHA_MIN);
		m_pMessageStatic->SetText(pEvent->pwcText);
		m_pStateUpdate = &CNameNotifyGui::Update_FadeIn;
		GetNtlGuiManager()->AddUpdateFunc( this );

		Logic_PlayGUISound(GSD_SYSTEM_NOTICE);
		
		Show(true);
	}

	NTL_RETURNVOID();
}