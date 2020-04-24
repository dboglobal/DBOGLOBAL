#include "precomp_dboclient.h"
#include "GroupNotifyGui.h"

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
#include "ChatDefine.h"

#define dTIME_FADE_IN		0.01f
#define dTIME_IDLE			5.f
#define dTIME_FADE_OUT		2.f

#define dALPHA_MIN			0
#define dALPHA_MAX			255


CGroupNotifyGui::CGroupNotifyGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fElapsedTime(0.f)
,m_pStateUpdate(NULL)
{
}

CGroupNotifyGui::~CGroupNotifyGui()
{
}

RwBool CGroupNotifyGui::Create()
{
	NTL_FUNCTION( "CGroupNotifyGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\GroupNotify.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	// ¸Þ¼¼Áö
	rect.SetRectWH(0, 0, m_pThis->GetWidth(), m_pThis->GetHeight());
	m_pMessageStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pMessageStatic->CreateFontStd(DEFAULT_FONT, 140, DEFAULT_FONT_ATTR);	
	m_pMessageStatic->SetEffectMode(TE_OUTLINE);
	m_pMessageStatic->SetEffectColor(DEFAULT_OUTLINE_EFFECT_VALUE);
	m_pMessageStatic->Enable(false);

	PositionAlign(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	LinkMsg(g_EventResize);
	LinkMsg(g_EventGroupNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGroupNotifyGui::Destroy()
{
	NTL_FUNCTION("CGroupNotifyGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventGroupNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGroupNotifyGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_fElapsedTime += fElapsed;
	(this->*m_pStateUpdate)();
}
VOID CGroupNotifyGui::Update_FadeIn()
{
	RwReal fDelta = m_fElapsedTime/(RwReal)dTIME_FADE_IN;

	if( m_fElapsedTime >= dTIME_FADE_IN )
	{	
		fDelta = 1.f;
		m_pStateUpdate = &CGroupNotifyGui::Update_Idle;		
	}

	RwReal fCurAlpha = CNtlMath::Interpolation((RwReal)dALPHA_MIN, (RwReal)dALPHA_MAX, fDelta);
	m_pMessageStatic->SetAlpha((RwUInt8)fCurAlpha);

	if( fDelta == 1.f )
		m_fElapsedTime = 0.f;
}

VOID CGroupNotifyGui::Update_Idle()
{
	if( m_fElapsedTime >= dTIME_IDLE )
	{
		m_fElapsedTime = 0.f;
		m_pStateUpdate = &CGroupNotifyGui::Update_FadeOut;
	}	
}

VOID CGroupNotifyGui::Update_FadeOut()
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

VOID CGroupNotifyGui::PositionAlign(RwInt32 iScreenWidth, RwInt32 iScreenHeight)
{
	RwReal fY = (RwReal)iScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fGroupNotiry_YPosRate;
	m_pThis->SetPosition( (iScreenWidth - m_pThis->GetWidth())/2, (RwInt32)fY );
}

VOID CGroupNotifyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGroupNotifyGui::HandleEvents");
	
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign(pEvent->iWidht, pEvent->iHeight);
	}
	else if( msg.Id == g_EventGroupNotify )
	{
		SDboEventGroupNotify* pEvent = reinterpret_cast<SDboEventGroupNotify*>( msg.pData );

		switch(pEvent->byType)
		{
		case GROUP_NOTIFY_PARTY:
			{
				m_pMessageStatic->SetAlpha(dALPHA_MIN);
				m_pMessageStatic->SetTextColor( NTL_PARTY_NAME_COLOR );
				m_pMessageStatic->SetText(pEvent->pwcText);

				Logic_PlayGUISound( GSD_SYSTEM_PARTY_NOTIFY );
				break;
			}
		case GROUP_NOTIFY_GUILD:
			{				
				m_pMessageStatic->SetAlpha(dALPHA_MIN);
				m_pMessageStatic->SetTextColor( CHATGUI_COLOR_GUILD );
				m_pMessageStatic->SetText(pEvent->pwcText);

				Logic_PlayGUISound( GSD_SYSTEM_PARTY_NOTIFY );	
				break;
			}
		default:
			{
				DBO_FAIL("Invalid group notify type : " << pEvent->byType);
				NTL_RETURNVOID();
			}
		}

		m_fElapsedTime = 0.f;

		m_pStateUpdate = &CGroupNotifyGui::Update_FadeIn;
		GetNtlGuiManager()->AddUpdateFunc( this );

		Show(true);
	}

	NTL_RETURNVOID();
}