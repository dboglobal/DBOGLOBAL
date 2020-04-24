#include "precomp_dboclient.h"
#include "ExpGui.h"

// Core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// client
#include "DboEvent.h"
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExpEffect

#define EXP_EFFECT_POSITION_X		200
#define EXP_EFFECT_MAX_ALPHA		255
#define EXP_EFFECT_MIN_ALPHA		0
#define EXP_EFFECT_MOVE_QUANTATY	100
#define EXP_EFFECT_SIZE_QUANTITY	1.5f		// Magnification of changing size 1.0 (actual size) ~ x
#define EXP_EFFECT_MOVE_TIME		1.0f
#define EXP_EFFECT_DELAY_TIME		0.5f		// Delay in succession
#define EXP_EFFECT_START_DELAY_TIME	2.0f		// Compensate for the time difference between when the mob dies and the packet.

CExpEffect::CExpEffect(VOID)
: m_bShowEffect( FALSE ), m_fCurrentTime( 0.0f ), m_nInitYPos( 0 ), m_nDestYPos( 0 ), m_nNumXPos( 0 ), m_nSurXPos( 0 ), m_bSound( FALSE ),
  m_uiAcquisitionExp(0), m_uiBonusExp(0)
{
	
}

CExpEffect::~CExpEffect(VOID)
{

}

VOID CExpEffect::Create(VOID)
{
	m_surEXP.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPText" ) );
	m_nInitWidth = m_surEXP.GetWidth();
	m_nInitHeight = m_surEXP.GetHeight();

	m_numEXP.Create( ND_RIGHT );
	m_numEXP.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum0" ) );
	m_numEXP.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum1" ) );
	m_numEXP.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum2" ) );
	m_numEXP.SetSurface( 3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum3" ) );
	m_numEXP.SetSurface( 4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum4" ) );
	m_numEXP.SetSurface( 5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum5" ) );
	m_numEXP.SetSurface( 6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum6" ) );
	m_numEXP.SetSurface( 7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum7" ) );
	m_numEXP.SetSurface( 8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum8" ) );
	m_numEXP.SetSurface( 9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum9" ) );
	m_numEXP.SetRate( 0.5f );
	m_numEXP.SetNumGap(-5);
}

VOID CExpEffect::Destroy(VOID)
{
	m_surEXP.UnsetTexture();
	m_numEXP.Destroy();
}

VOID CExpEffect::StartProc(RwInt32 nScreenXPos, RwInt32 nScreenYPos, RwUInt32 nValue, RwReal fDelayTime, RwUInt32 uiAcquisitionExp /* = 0 */, RwUInt32 uiBonusExp /* = 0  */)
{
	m_bShowEffect = TRUE;
	m_bSound = TRUE;

	m_fCurrentTime = -fDelayTime - EXP_EFFECT_START_DELAY_TIME;
	m_nInitYPos = nScreenYPos - m_nInitHeight;
	m_nDestYPos = nScreenYPos - m_nInitHeight - EXP_EFFECT_MOVE_QUANTATY;
	m_nSurXPos = nScreenXPos + EXP_EFFECT_POSITION_X;
	m_nNumXPos = nScreenXPos + EXP_EFFECT_POSITION_X + m_surEXP.GetWidth();
	m_nDestWidth = (RwInt32)( m_nInitWidth * EXP_EFFECT_SIZE_QUANTITY );	
	m_nDestHeight = (RwInt32)( m_nInitHeight * EXP_EFFECT_SIZE_QUANTITY );

	m_numEXP.SetNumber( nValue );
	m_surEXP.SetPosition( m_nSurXPos, nScreenYPos );
	m_surEXP.SetAlpha( EXP_EFFECT_MAX_ALPHA );
	m_numEXP.SetPosition( m_nNumXPos, nScreenYPos );
	m_numEXP.SetAlpha( EXP_EFFECT_MAX_ALPHA );
	m_numEXP.EffectRate( 1.0f, EXP_EFFECT_SIZE_QUANTITY, EXP_EFFECT_MOVE_TIME );	

    m_uiAcquisitionExp = uiAcquisitionExp;
    m_uiBonusExp = uiBonusExp;
}

VOID CExpEffect::EndProc(VOID)
{
	m_bShowEffect = FALSE;
	m_fCurrentTime = 0.0f;	
}

RwBool CExpEffect::Update( RwReal fElapsed )
{
	if( m_bShowEffect )
	{
		m_fCurrentTime += fElapsed;

		if( m_fCurrentTime >= 0.0f )	// DelayTime;
		{
			if( m_bSound )
			{
				RwUInt32 uiExp = (RwUInt32)m_numEXP.GetNumber();
                if(m_uiBonusExp == 0)
                {
				    GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GAIN_EXP", FALSE, NULL, uiExp );
                }
                else    // PC방 메시지
                {
                    GetAlarmManager()->FormattedAlarmMessage( "DST_NOTIFY_GAIN_EXP_AND_BONUS", FALSE, NULL, m_uiAcquisitionExp, m_uiBonusExp );
                }

				Logic_PlayGUISound(GSD_GET_EXP);
				m_bSound = FALSE;
			}

			if( m_fCurrentTime < EXP_EFFECT_MOVE_TIME )
			{
				RwReal fYPosControl1 = gui::GetValueByPercentOfStart( (RwReal)m_nInitYPos, (RwReal) m_nDestYPos, 80 );
				RwReal fYPosControl2 = gui::GetValueByPercentOfStart( (RwReal)m_nInitYPos, (RwReal)m_nDestYPos, 90 );
				RwReal fAlphaControl1 = gui::GetValueByPercentOfStart( EXP_EFFECT_MAX_ALPHA, EXP_EFFECT_MIN_ALPHA, 10 );
				RwReal fAlphaControl2 = gui::GetValueByPercentOfStart( EXP_EFFECT_MAX_ALPHA, EXP_EFFECT_MIN_ALPHA, 20 );

				RwInt32 nYPos = (RwInt32)( gui::GetResult3BezierInterpolation( m_fCurrentTime, EXP_EFFECT_MOVE_TIME, (RwReal)m_nInitYPos, fYPosControl1,fYPosControl2, (RwReal)m_nDestYPos ) );
				RwInt32 nAlpha = (RwInt32)( gui::GetResult3BezierInterpolation( m_fCurrentTime, EXP_EFFECT_MOVE_TIME, (RwReal)EXP_EFFECT_MAX_ALPHA, fAlphaControl1, fAlphaControl2, (RwReal)EXP_EFFECT_MIN_ALPHA ) ); 
				RwInt32 nWidth = (RwInt32)( gui::GetResultLineInterpolation( m_fCurrentTime, EXP_EFFECT_MOVE_TIME, (RwReal)m_nInitWidth, (RwReal)m_nDestWidth ) );
				RwInt32 nHeight = (RwInt32)( gui::GetResultLineInterpolation( m_fCurrentTime, EXP_EFFECT_MOVE_TIME, (RwReal)m_nInitHeight, (RwReal)m_nDestHeight ) );
				
				m_surEXP.SetPosition( m_nSurXPos - ( nWidth - m_nInitWidth ), nYPos );
				m_surEXP.SetAlpha((RwUInt8)nAlpha );
				m_surEXP.SetSize( nWidth, nHeight );
				m_numEXP.SetPosition( m_nNumXPos, nYPos );
				m_numEXP.SetAlpha((RwUInt8)nAlpha );
				m_numEXP.Update( fElapsed );	
			}
			else
			{
				EndProc();
				return FALSE;			
			}
		}		
	}

	return TRUE;
}

VOID CExpEffect::Render(VOID)
{
	// Delay Time이 길어져서 시간으로 랜더여부 판별 -_-;;
	if( m_fCurrentTime > 0.0f )
	{
		m_surEXP.Render( true );
		m_numEXP.Render( true );
	}
}

VOID CExpEffect::SetInitXPos( RwInt32 nScreenXPos )
{
	m_nSurXPos = nScreenXPos + EXP_EFFECT_POSITION_X;
	m_nNumXPos = nScreenXPos + EXP_EFFECT_POSITION_X + m_surEXP.GetWidth();
}

VOID CExpEffect::SetInitYPos( RwInt32 nScreenYPos )
{
	m_nInitYPos = nScreenYPos - m_surEXP.GetHeight();
	m_nDestYPos = nScreenYPos - m_surEXP.GetHeight() - EXP_EFFECT_MOVE_QUANTATY;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CExpGui

CExpGui::CExpGui()
{
}

CExpGui::CExpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	
}

CExpGui::~CExpGui()
{
}

RwBool CExpGui::Create(VOID)
{
	NTL_FUNCTION("CExpGui::Create");

	if(!CNtlPLGui::Create("", "gui\\ExpBar.srf", "gui\\ExpBar.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgExpBar");	
	m_ppgbExp = (gui::CProgressBar*)GetComponent( "pgbExp" );

	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );

	m_slotMouseEnter = m_ppgbExp->SigMouseEnter().Connect( this, &CExpGui::OnMouseEnter );
	m_slotMouseLeave = m_ppgbExp->SigMouseLeave().Connect( this, &CExpGui::OnMouseLeave );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CExpGui::OnPaint );	
		
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventUpdateExp );
	LinkMsg( g_EventResize, 0, 0x7000 );

	GetNtlGuiManager()->AddUpdateFunc( this );	

	NTL_RETURN(TRUE);
}

VOID CExpGui::Destroy(VOID)
{
	NTL_FUNCTION("CExpGui::Destroy");

	std::list<CExpEffect*>::iterator it;
	for( it = m_listExpEffect.begin() ; it != m_listExpEffect.end() ; ++it )
	{
		CExpEffect* pExpEffect = (*it);
		NTL_DELETE( pExpEffect );
	}
	m_listExpEffect.clear();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventUpdateExp );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CExpGui::Update( RwReal fElapsed )
{
	std::list<CExpEffect*>::iterator it;
	for( it = m_listExpEffect.begin() ; it != m_listExpEffect.end() ; )
	{
		CExpEffect* pExpEffect = *it;
		if( pExpEffect->Update( fElapsed ) )
		{
			++it;
		}
		else
		{
			NTL_DELETE( pExpEffect );
			it = m_listExpEffect.erase( it );
		}
	}
}

RwInt32 CExpGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CExpGui::HandleEvents( RWS::CMsg &pMsg )
{
	if( pMsg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			
			if( pUpdate->hSerialId == pSobAvatar->GetSerialID() )
			{
				RwUInt32 uiCurrentExp = Logic_GetExp( pSobAvatar );
				RwUInt32 uiMaxExp = Logic_GetMaxExp( pSobAvatar );				

				m_ppgbExp->SetRange( 0, uiMaxExp );
				m_ppgbExp->SetPos( uiCurrentExp );

				if( GetInfoWndManager()->GetRequestGui() == DIALOG_EXP )
					SetExpInfoWnd();											
			}			
		}
	}
	else if( pMsg.Id == g_EventUpdateExp )
	{
		SDboEventUpdateExp* pData = reinterpret_cast<SDboEventUpdateExp*>( pMsg.pData );

		CRectangle rect = m_pThis->GetScreenRect();
		CExpEffect* pExpEffect = NTL_NEW CExpEffect();
		pExpEffect->Create();
		pExpEffect->StartProc( rect.left, rect.bottom, pData->uiIncreasedExp, m_listExpEffect.size() * EXP_EFFECT_DELAY_TIME, pData->uiAcquisitionExp, pData->uiBonusExp);
		m_listExpEffect.push_back( pExpEffect );							
	}
	else if( pMsg.Id == g_EventResize )
	{
		if( !m_listExpEffect.empty() )
		{
			CRectangle rect = m_pThis->GetScreenRect();
			std::list<CExpEffect*>::iterator it;

			for( it = m_listExpEffect.begin() ; it != m_listExpEffect.end() ; ++it )
			{
				CExpEffect* pExpEffect = *it;
				pExpEffect->SetInitXPos( rect.left );
				pExpEffect->SetInitYPos( rect.bottom );
			}			
		}
	}
}

VOID CExpGui::SetExpInfoWnd(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

	WCHAR wBuf[32];

	if( pAvatarAttr->m_uiMaxExp )
	{
		const WCHAR* pString = GetDisplayStringManager()->GetString( "DST_EXP_INFO" );
		swprintf_s( wBuf, 32, pString, pAvatarAttr->m_uiExp, pAvatarAttr->m_uiMaxExp );		
	}	
	else
	{
		swprintf_s( wBuf, 32, L"MAX EXP" );
	}

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, CMouse::GetX(), rtScreen.top, wBuf, DIALOG_EXP );	
}

VOID CExpGui::OnMouseEnter( gui::CComponent* pComponent )
{
	SetExpInfoWnd();
}

VOID CExpGui::OnMouseLeave( gui::CComponent* pComponent )
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CExpGui::OnPaint(VOID)
{
	std::list<CExpEffect*>::iterator it;
	for( it = m_listExpEffect.begin() ; it != m_listExpEffect.end() ; ++it )
	{
		CExpEffect* pExpEffect = (*it);
		pExpEffect->Render();
	}
}