#include "precomp_dboclient.h"
#include "SubGaugeGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobProxy.h"

// dbo
#include "DialogManager.h"
#include "DboEvent.h"
#include "GuiFunctor.h"
#include "AlarmManager.h"
#include "HpGui.h"

#define CASTGAUGE_POS_OFFSET_X	-50
#define CASTGAUGE_POS_OFFSET_Y	20

CSubGaugeGui::STATE_GUARDGAUGE	CSubGaugeGui::m_eGuardGaugeState = CSubGaugeGui::STATE_NONE;

CCastGauge::CCastGauge( gui::CComponent* pParent, CNtlSobActor* pActor )
: m_pActor( pActor ), m_pCastGauge( NULL ), m_pCastBack( NULL )
{
	gui::CSurfaceManager* pSurfaceManager = GetNtlGuiManager()->GetSurfaceManager();
	m_pCastBack = NTL_NEW gui::CPanel( CRectangle( 0, 0, 100, 13 ), pParent, pSurfaceManager, pSurfaceManager->GetSurface( "RPCGauge.srf", "srfCastGaugeBack" ) );
	m_pCastGauge = NTL_NEW gui::CProgressBar( CRectangle( 0, 0, 100, 13 ), pParent, pSurfaceManager, pSurfaceManager->GetSurface( "RPCGauge.srf", "srfCastGauge" ) );	

	PositionUpdate();
}

CCastGauge::~CCastGauge(VOID)
{
	NTL_DELETE( m_pCastGauge );
	NTL_DELETE( m_pCastBack );
}

VOID CCastGauge::Update( RwReal fElapsedTime )
{
	RwInt32 nCurrTime = (RwInt32)( Logic_GetCurrentCastingTime( m_pActor ) * 1000 );
	RwInt32 nMaxTime  = (RwInt32)( Logic_GetMaxCastingTime( m_pActor ) * 1000 );

	m_pCastGauge->SetRange( 0, nMaxTime );
	m_pCastGauge->SetPos( nCurrTime );

	PositionUpdate();
}

VOID CCastGauge::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset( CASTGAUGE_POS_OFFSET_X, CASTGAUGE_POS_OFFSET_Y );

    RwV3d vActorPos = m_pActor->GetPosition();
	if( GetGuiFuntor()->Calc3DPosTo2D( vActorPos, 0.0f, &posOffset, &posRes ) )
	{
		m_pCastBack->SetPosition( posRes.x, posRes.y );
		m_pCastGauge->SetPosition( posRes.x, posRes.y );
		
		m_pCastBack->Show( true );
		m_pCastGauge->Show( true );
	}
	else
	{
		m_pCastBack->Show( false );
		m_pCastGauge->Show( false );
	}
}

#define SUBGAUGE_POS_OFFSET_X	-50
#define SUBGAUGE_POS_OFFSET_Y	20

#define SUBGAUGE_GUARD_START_TIME		0.2f

#define SUBGAUGE_GUARD_OFFSET_MARK_X	-70	//68	//70//71
#define SUBGAUGE_GUARD_OFFSET_MARK_Y	30	//77	//77//80
#define SUBGAUGE_GUARD_OFFSET_GAUGE_X	19	//87	//87//89
#define SUBGAUGE_GUARD_OFFSET_GAUGE_Y	12	//89	//89//90

CSubGaugeGui::CSubGaugeGui( const RwChar* pName )
:CNtlPLGui( pName )
{
	m_pRPGauge = NULL;
	m_pRPBack = NULL;	

	m_bRPShow = FALSE;
	m_bRPBallMax = FALSE;

	m_eGuardGaugeState = STATE_NONE;
	m_fGuardCoolTime = 0.0f;
	m_fMaxGuardCoolTime = 0.0f;

	m_bGuardShow	= FALSE;
	m_hSound		= INVALID_SOUND_HANDLE;
}

CSubGaugeGui::~CSubGaugeGui(VOID)
{
	
}

RwBool CSubGaugeGui::Create(VOID)
{
	NTL_FUNCTION( "CSubGaugeGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\RPCGauge.srf", "gui\\RPCGauge.frm" ) )
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgPcStatus" );

	m_pRPGauge = (gui::CProgressBar*)GetComponent( "pgbRPGauge" );
	m_pRPBack = (gui::CPanel*)GetComponent( "pnlRPGaugeBack" );
	m_ppnlGuardMark = (gui::CPanel*)GetComponent( "pnlGuardMark" );

	m_slotOnMoveGuard = m_ppnlGuardMark->SigMove().Connect( this, &CSubGaugeGui::OnMoveGaurd );
	m_slotOnPaintGuard = m_pThis->SigPaint().Connect( this, &CSubGaugeGui::OnPaintGuard );
	
	m_pThis->SetPriority( dDIALOGPRIORITY_SUBGAUGE );
	m_pRPGauge->Show( false );
	m_pRPBack->Show( false );

	// Guard Gauge 초기세팅
	m_ppnlGuardMark->Show( false );
	m_surGuardGauge.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RPCGauge.srf", "srfGuardGauge" ) );
	m_surGuardGauge.SetPositionfromParent( SUBGAUGE_GUARD_OFFSET_GAUGE_X, SUBGAUGE_GUARD_OFFSET_GAUGE_Y );
	SetGuardMark( "srfGuardUp" );
	SetGuardValue( 0, 300 );
	SetGuardState( STATE_NONE );

	GetNtlGuiManager()->AddUpdateFunc( this );
		
	LinkMsg( g_EventChargingNfy );
	LinkMsg( g_EventSobCastingDirect );
	LinkMsg( g_EventSobRevivalNotify );
	LinkMsg( g_EventCharObjDelete );
	LinkMsg( g_EventUpdateRpStock );
	LinkMsg( g_EventBlockingNfy );
	
	NTL_RETURN( TRUE );
}

VOID CSubGaugeGui::Destroy(VOID)
{
	NTL_FUNCTION( "CSubGaugeGui::Destroy" );

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	MAP_CASTGAUGE_ITER iter;

	for( iter = m_mapCastGauge.begin() ; iter != m_mapCastGauge.end() ; ++iter )
	{
		CCastGauge* pCastGauge = iter->second;
		NTL_DELETE( pCastGauge );
	}
	m_mapCastGauge.clear();

	UnLinkMsg( g_EventChargingNfy );
	UnLinkMsg( g_EventSobCastingDirect );
	UnLinkMsg( g_EventSobRevivalNotify );
	UnLinkMsg( g_EventCharObjDelete );
	UnLinkMsg( g_EventUpdateRpStock );
	UnLinkMsg( g_EventBlockingNfy );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();	
}

VOID CSubGaugeGui::Update( RwReal fElapsed )
{
	if( m_bRPShow && !m_bRPBallMax )
	{
		CHpGui* pHPGui = reinterpret_cast<CHpGui*>( GetDialogManager()->GetDialog( DIALOG_HP ) );
		RwInt32 nMin, nMax;
		pHPGui->GetRpGauge()->GetRange( nMin, nMax );

		m_pRPGauge->SetRange( 0, nMax );
		m_pRPGauge->SetPos( pHPGui->GetRpGauge()->GetPos() );		

		PositionUpdate();
	}	

	MAP_CASTGAUGE_ITER iter;

	for( iter = m_mapCastGauge.begin() ; iter != m_mapCastGauge.end() ; ++iter )
	{
		CCastGauge* pCastGauge = iter->second;
		pCastGauge->Update( fElapsed );
	}

	GuardGaugeUpdate( fElapsed );
}

VOID CSubGaugeGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventChargingNfy )
	{
		SNtlEventActionMapCharging* pData = reinterpret_cast<SNtlEventActionMapCharging*>( msg.pData );        
    
		if( pData->bStartOrEnd )
			m_bRPShow = TRUE;			
		else
			m_bRPShow = FALSE;		
		
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

		CheckRPGaugeState(pAvatarInfo->sCharPf.byCurRPBall);
	}
	else if(msg.Id == g_EventSobRevivalNotify)
	{
		if(m_pRPBack->IsVisible())
		{
			m_pRPBack->Show( false );
			m_pRPGauge->Show( false );
			m_bRPShow = FALSE;
		}
	}
	else if( msg.Id == g_EventSobCastingDirect )
	{
		SNtlEventSobCastingDirect* pData = reinterpret_cast<SNtlEventSobCastingDirect*>( msg.pData );
		MAP_CASTGAUGE_ITER iter = m_mapCastGauge.find( pData->pActor->GetSerialID() );

		if( pData->bStartOrEnd )
		{// Casting On
			if( iter == m_mapCastGauge.end() )
			{// 없을때만 추가
				m_mapCastGauge[pData->pActor->GetSerialID()] = NTL_NEW CCastGauge( m_pThis, pData->pActor );
			}
		}
		else
		{// Casting Off
			if( iter != m_mapCastGauge.end() )
			{// 있을때만 삭제
				CCastGauge* pCastGauge = iter->second;
				NTL_DELETE( pCastGauge );
				m_mapCastGauge.erase( iter );
			}
		}
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		SERIAL_HANDLE handle = *(SERIAL_HANDLE*)msg.pData;
		MAP_CASTGAUGE_ITER iter = m_mapCastGauge.find( handle );
		
		if( iter != m_mapCastGauge.end() )
		{// 있을때만 삭제
			CCastGauge* pCastGauge = iter->second;
			NTL_DELETE( pCastGauge );
			m_mapCastGauge.erase( iter );
		}
	}
	else if( msg.Id == g_EventUpdateRpStock )
	{
		SDboEventUpdateRpStock* pData = reinterpret_cast<SDboEventUpdateRpStock*>( msg.pData );

		CheckRPGaugeState( pData->nRpStock );
	}
	else if( msg.Id == g_EventBlockingNfy )
	{
		SNtlEventBlockingNfy* pData = reinterpret_cast<SNtlEventBlockingNfy*>( msg.pData );

		if( pData->bStartOrEnd )
		{
			SetGuardMark( "srfGuardFocus" );			
			SetGuardState( STATE_START );	
			m_ppnlGuardMark->Show( true );
		}
		else
		{
			m_fMaxGuardCoolTime = pData->dwCoolingTime * 0.001f;
			SetGuardMark( "srfGuardDown" );			
			SetGuardState( STATE_RUN );		

			// Sound 시작
			sNtlSoundPlayParameta tSoundParam;
			tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
			tSoundParam.pcFileName		= GSD_SYSTEM_GUARD_GAGE;
			tSoundParam.bLoop			= true;

			GetSoundManager()->Play(&tSoundParam);

			m_hSound	= tSoundParam.hHandle;

		}

		m_bGuardShow = TRUE;
	}	
}

VOID CSubGaugeGui::CheckRPGaugeState( RwInt32 nCurrentRPBall )
{
	if( m_bRPShow )
	{
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

		if(pAvatarInfo->byMaxRpStock == nCurrentRPBall )
		{
			m_pRPGauge->Show( false );
			m_pRPBack->Show( false );
			m_bRPBallMax = TRUE;
		}				
		else
		{
			m_pRPGauge->Show( true );
			m_pRPBack->Show( true );
			m_bRPBallMax = FALSE;

			// RP Gauge이미지 변경
			RwChar buf[256];
			sprintf_s( buf, 256, "srfNewRPGauge%d", nCurrentRPBall );
			m_pRPGauge->ClearSurface();
			m_pRPGauge->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", buf ) );
			m_pRPGauge->SetPos( m_pRPGauge->GetPos() );			
		}
	}
	else
	{
		m_pRPGauge->Show( false );
		m_pRPBack->Show( false );
	}
}

VOID CSubGaugeGui::PositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset( SUBGAUGE_POS_OFFSET_X, SUBGAUGE_POS_OFFSET_Y );

    RwV3d vAvatarPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
	if( GetGuiFuntor()->Calc3DPosTo2D( vAvatarPos, 0.0f, &posOffset, &posRes ) )
	{
		m_pRPGauge->SetPosition( posRes.x, posRes.y );
		m_pRPBack->SetPosition( posRes.x, posRes.y );
		m_pRPGauge->Show( true );
		m_pRPBack->Show( true );		
	}
	else
	{
		m_pRPGauge->Show( false );
		m_pRPBack->Show( false );		
	}
}

VOID CSubGaugeGui::GuardPositionUpdate(VOID)
{
	CPos posRes;
	CPos posOffset( SUBGAUGE_GUARD_OFFSET_MARK_X, SUBGAUGE_GUARD_OFFSET_MARK_Y );

    RwV3d vAvatarPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
	if( GetGuiFuntor()->Calc3DPosTo2D( vAvatarPos, 0.0f, &posOffset, &posRes ) )
	{
		m_ppnlGuardMark->SetPosition( posRes.x, posRes.y );
		m_ppnlGuardMark->Show( true );		
	}
	else
	{
		m_ppnlGuardMark->Show( false );		
	}
}

VOID CSubGaugeGui::SetGuardValue( RwInt32 nValue, RwInt32 nMaxValue )
{
	gui::CSurface* pSurface = m_surGuardGauge.GetSurface();

	RwInt32 nOriginalWidth = pSurface->m_Original.rtRect.GetWidth();
	RwInt32 nResultWidth = nOriginalWidth * nValue / nMaxValue;

	RwReal  fOriginalUVsWidth = pSurface->m_Original.UVs[2] - pSurface->m_Original.UVs[0];
	RwReal	fResultUVs = fOriginalUVsWidth * nValue / nMaxValue;

	pSurface->m_SnapShot.rtRect.right = pSurface->m_SnapShot.rtRect.left + nResultWidth;
	pSurface->m_SnapShot.UVs[0] = pSurface->m_SnapShot.UVs[2] - fResultUVs;
}

VOID CSubGaugeGui::SetGuardMark( const CHAR* pSurfaceName )
{
	m_ppnlGuardMark->GetSurface()->clear();
	m_ppnlGuardMark->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "RPCGauge.srf", pSurfaceName ) );	
}

VOID CSubGaugeGui::GuardGaugeUpdate( RwReal fElapsed )
{
	if( m_bGuardShow )
	{
		GuardPositionUpdate();

		if( m_eGuardGaugeState != STATE_NONE )
		{
			RwReal fValue = 0.0f;
			m_fGuardCoolTime += fElapsed;

			if( m_eGuardGaugeState == STATE_START )
			{
				if( m_fGuardCoolTime < SUBGAUGE_GUARD_START_TIME )
				{
					fValue = gui::GetResultLineInterpolation( m_fGuardCoolTime, SUBGAUGE_GUARD_START_TIME, 0, 300 );
					SetGuardValue( (RwInt32)fValue, 300 );
				}
				else
				{
					SetGuardValue( 300, 300 );				
				}							
			}
			else if( m_eGuardGaugeState == STATE_RUN )
			{
				RwReal fMaxTime = m_fMaxGuardCoolTime;

				if( m_fGuardCoolTime < fMaxTime )
				{
					fValue = gui::GetResultLineInterpolation( m_fGuardCoolTime, fMaxTime, 300, 0 );
					SetGuardValue( (RwInt32)fValue, 300 );
				}
				else
				{
					SetGuardValue( 0, 300 );
					SetGuardState( STATE_NONE );	
					SetGuardMark( "srfGuardUp" );	

					m_bGuardShow = FALSE;
					m_ppnlGuardMark->Show( false );

					// 기존 음악 끄고
					GetSoundManager()->Stop(m_hSound);

					// 새로운 음악
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
					tSoundParam.pcFileName		= GSD_SYSTEM_GUARD_GAGE_END;

					GetSoundManager()->Play(&tSoundParam);
				}
			}						
		}				
	}	
}

VOID CSubGaugeGui::SetGuardState( STATE_GUARDGAUGE eType )
{
	//if( eType == STATE_NONE )
	//	m_ppnlGuardMark->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_GUARD_GAUGE_CAN_USE" ) ) );
	//else
	//	m_ppnlGuardMark->SetToolTip( std::wstring( GetDisplayStringManager()->GetString( "DST_GUARD_GAUGE_CANNOT_USE" ) ) );

	m_eGuardGaugeState = eType;
	m_fGuardCoolTime = 0.0f;	
}

VOID CSubGaugeGui::OnPaintGuard(VOID)
{
	m_surGuardGauge.Render();
}

VOID CSubGaugeGui::OnMoveGaurd( RwInt32 nX, RwInt32 nY )
{
	CRectangle rect = m_ppnlGuardMark->GetScreenRect();
	m_surGuardGauge.SetPositionbyParent( rect.left, rect.top );
}