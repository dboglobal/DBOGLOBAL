#include "precomp_ntlpresentation.h"
#include "NtlPLSceneChangerGui.h"

// core
#include "NtlDebug.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

namespace
{
	#define	dDEFAULT_ALPHA		0
	#define dDEFAULT_RED		0
	#define dDEFAULT_GREEN		0
	#define dDEFAULT_BLUE		0
};


CNtlPLSceneChangerGui::CNtlPLSceneChangerGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byTransitionMode(CINEMATIC_INVALID)
,m_eFadeInOut(EFADE_NONE)
{
	
}

CNtlPLSceneChangerGui::~CNtlPLSceneChangerGui()
{

}

RwBool CNtlPLSceneChangerGui::Create()
{
	NTL_FUNCTION( "CNtlPLSceneChangerGui::Create" );

	m_pThis = NTL_NEW gui::CDialog(CRectangle(0, 0, 0, 0), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager());

	Show(false);

	// default
	SetColor(dDEFAULT_RED, dDEFAULT_GREEN, dDEFAULT_BLUE);
	SetAlpha(dDEFAULT_ALPHA);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CNtlPLSceneChangerGui::OnPaint );

	LinkMsg(g_EventCinematicFadeIn);
	LinkMsg(g_EventCinematicFadeOut);
	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CNtlPLSceneChangerGui::Destroy()
{
	NTL_FUNCTION("CNtlPLSceneChangerGui::Destroy");

	UnLinkMsg(g_EventCinematicFadeIn);
	UnLinkMsg(g_EventCinematicFadeOut);
	UnLinkMsg(g_EventResize);

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	NTL_DELETE( m_pThis );

	NTL_RETURNVOID();
}

VOID CNtlPLSceneChangerGui::Update(RwReal fElapsed)
{
	if( IsShow() == false )
		return;


	m_InterData.fCurTime += fElapsed;

	if( m_InterData.fCurTime >= m_InterData.fUpdateTime )
	{
		m_InterData.fCurTime = m_InterData.fUpdateTime;

		if( m_eFadeInOut == EFADE_IN )
			SwitchDialog(false);
	}

	RwReal fRate = m_InterData.fCurTime / m_InterData.fUpdateTime;

	SetColor( -(RwUInt8)((RwReal)(m_InterData.byStartRed - m_InterData.byEndRed) * fRate) + m_InterData.byStartRed,
		-(RwUInt8)((RwReal)(m_InterData.byStartGreen - m_InterData.byEndGreen) * fRate) + m_InterData.byStartGreen,
		-(RwUInt8)((RwReal)(m_InterData.byStartBlue - m_InterData.byEndBlue) * fRate) + m_InterData.byStartBlue);

	SetAlpha(-(RwUInt8)((RwReal)(m_InterData.byStartAlpha - m_InterData.byEndAlpha) * fRate) + m_InterData.byStartAlpha);
}

VOID CNtlPLSceneChangerGui::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_srfSurface.m_SnapShot.uRed	= byRed;
	m_srfSurface.m_SnapShot.uGreen	= byGreen;
	m_srfSurface.m_SnapShot.uBlue	= byBlue;
}

VOID CNtlPLSceneChangerGui::SetAlpha(RwUInt8 byAlpha)
{
	m_srfSurface.m_SnapShot.uAlpha	= byAlpha;
}

VOID CNtlPLSceneChangerGui::OnPaint()
{
	switch( m_byTransitionMode )
	{	
	case CINEMATIC_FADEINOUT_SCREEN_PANEL:
		{
			OnPaintScreenPanel();
			break;
		}
	case CINEMATIC_FADEINOUT_SHUTDOWN_TV:
		{
			OnPaintShutDownTV();
			break;
		}
	}
}

VOID CNtlPLSceneChangerGui::OnPaintShutDownTV()
{
	CRectangle rtScreen = GetNtlGuiManager()->GetGuiManager()->GetScreenRect();
	RwReal fRate = m_InterData.fCurTime/m_InterData.fUpdateTime;	
	RwReal fHalfHeight = (RwReal)rtScreen.GetHeight()/2.f + 1; // 1 : 소수점 오차로 인한 수치 보정

	if( m_eFadeInOut == EFADE_IN )
	{

		RwInt32 iHeight = (RwInt32)(fHalfHeight - (fHalfHeight * fRate));

		// 상단 그리기
		m_srfSurface.m_SnapShot.rtRect.SetRectWH(rtScreen.left, rtScreen.top, rtScreen.right, iHeight);
		g_GuiRenderer.RenderQueue(&m_srfSurface.m_SnapShot, m_srfSurface.m_pTexture, true, m_srfSurface.m_fAngle);		

		// 하단 그리기
		m_srfSurface.m_SnapShot.rtRect.SetRectWH(rtScreen.left, rtScreen.bottom - iHeight, rtScreen.right, iHeight);
		g_GuiRenderer.RenderQueue(&m_srfSurface.m_SnapShot, m_srfSurface.m_pTexture, true, m_srfSurface.m_fAngle);
	}
	else if( m_eFadeInOut == EFADE_OUT )
	{
		RwInt32 iHeight = (RwInt32)(fHalfHeight * fRate);

		// 상단 그리기
		m_srfSurface.m_SnapShot.rtRect.SetRectWH(rtScreen.left, rtScreen.top, rtScreen.right, iHeight);
		g_GuiRenderer.RenderQueue(&m_srfSurface.m_SnapShot, m_srfSurface.m_pTexture, true, m_srfSurface.m_fAngle);

		// 하단 그리기
		m_srfSurface.m_SnapShot.rtRect.SetRectWH(rtScreen.left, rtScreen.bottom - iHeight, rtScreen.right, iHeight);
		g_GuiRenderer.RenderQueue(&m_srfSurface.m_SnapShot, m_srfSurface.m_pTexture, true, m_srfSurface.m_fAngle);
	}	
}

VOID CNtlPLSceneChangerGui::OnPaintScreenPanel()
{
	CRectangle rtScreen = GetNtlGuiManager()->GetGuiManager()->GetScreenRect();

	m_srfSurface.m_SnapShot.rtRect.SetRectWH(rtScreen.left, rtScreen.top, rtScreen.right, rtScreen.bottom);
	g_GuiRenderer.RenderQueue(&m_srfSurface.m_SnapShot, m_srfSurface.m_pTexture, true, m_srfSurface.m_fAngle);
}

RwInt32 CNtlPLSceneChangerGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		// 다이얼로그가 닫힐 때 초기화
		Show(false);
		SetColor(dDEFAULT_RED, dDEFAULT_GREEN, dDEFAULT_BLUE);
		SetAlpha(dDEFAULT_ALPHA);
		m_byTransitionMode = CINEMATIC_INVALID;
		m_eFadeInOut = EFADE_NONE;

		m_InterData.fUpdateTime		= 0.f;
		m_InterData.fCurTime		= 0.f;
		m_InterData.byStartRed		= 0;
		m_InterData.byStartGreen	= 0;
		m_InterData.byStartBlue		= 0;
		m_InterData.byEndRed		= 0;
		m_InterData.byEndGreen		= 0;
		m_InterData.byEndBlue		= 0;
	}

	return 1;
}

VOID CNtlPLSceneChangerGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CNtlPLSceneChangerGui::HandleEvents");

	if( msg.Id == g_EventCinematicFadeOut )
	{
		SNtlEventCinematicFadeInOut* pEvent = reinterpret_cast<SNtlEventCinematicFadeInOut*>( msg.pData );

		m_InterData.fUpdateTime		= pEvent->fLifeTime;
		m_InterData.fCurTime		= 0.f;
		m_InterData.byStartRed		= 0;
		m_InterData.byStartGreen	= 0;
		m_InterData.byStartBlue		= 0;
		m_InterData.byEndRed		= pEvent->byRed;
		m_InterData.byEndGreen		= pEvent->byGreen;
		m_InterData.byEndBlue		= pEvent->byBlue;		
		m_InterData.byEndAlpha		= pEvent->byEndAlpha;

		m_byTransitionMode			= pEvent->byType;
		m_eFadeInOut				= EFADE_OUT;


		// avooo's comment : 차후에 이벤트의 인자로 받을 여지가 있다
		if( m_byTransitionMode == CINEMATIC_FADEINOUT_SCREEN_PANEL )
		{
			m_InterData.byStartAlpha	= 0;
		}
		else if( m_byTransitionMode == CINEMATIC_FADEINOUT_SHUTDOWN_TV )
		{
			m_InterData.byStartAlpha	= 255;
		}


		SwitchDialog(true);
	}
	else if( msg.Id == g_EventCinematicFadeIn )
	{
		SNtlEventCinematicFadeInOut* pEvent = reinterpret_cast<SNtlEventCinematicFadeInOut*>( msg.pData );

		m_InterData.fUpdateTime		= pEvent->fLifeTime;
		m_InterData.fCurTime		= 0.f;
		m_InterData.byStartRed		= pEvent->byRed;
		m_InterData.byStartGreen	= pEvent->byGreen;
		m_InterData.byStartBlue		= pEvent->byBlue;
		m_InterData.byStartAlpha	= 255;
		m_InterData.byEndRed		= 0;
		m_InterData.byEndGreen		= 0;
		m_InterData.byEndBlue		= 0;				
		m_InterData.byEndAlpha		= pEvent->byEndAlpha;

		m_byTransitionMode			= pEvent->byType;
		m_eFadeInOut				= EFADE_IN;
	}
	else if( msg.Id == g_EventResize )
	{
		OnPaint();
	}
	
	NTL_RETURNVOID();
}