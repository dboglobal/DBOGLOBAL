#include "precomp_dboclient.h"
#include "HpGui.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLLogic.h"
#include "NtlSobCharProxy.h"
#include "NtlSLMailSystem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// GameTable
#include "ItemTable.h"
#include "MobTable.h"
#include "FormulaTable.h"
#include "WorldTable.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"	
#include "BuffDispObject.h"
#include "DisplayStringManager.h"
#include "CursorManager.h"
#include "LobbyManager.h"

#define PCSTATUS_UPDATE_INFOWND_TIME	1.0f

CHpGui::CHpGui()
{
	Init();
}

CHpGui::CHpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	Init();
}

CHpGui::~CHpGui()
{
}

VOID CHpGui::Init(VOID)
{
	m_fCurrentRPChargingTime = 0.0f;
	m_fRPChargingTime = 0.0f;
	m_fDestValue = 0.0f;
	m_fCurrentValue = 0.0f;
	m_nMaxValue = 100;
	m_eRPChargeType = TYPE_NONE;

	m_hSound = INVALID_SOUND_HANDLE;
	m_pBuff = NULL;

	m_eAirColor = TYPE_DISABLE;
}

RwBool CHpGui::Create(void)
{
	NTL_FUNCTION("CHpGui::Create");

	if (!CNtlPLGui::Create("", "gui\\Aggro.srf", ""))
		NTL_RETURN(FALSE);

	if (!CNtlPLGui::Create("", "gui\\AirPoint.srf", ""))
		NTL_RETURN(FALSE);

	if (!CNtlPLGui::Create("", "gui\\HpScouter.srf", ""))
		NTL_RETURN(FALSE);

	if(!CNtlPLGui::Create("", "gui\\StatusBar.srf", "gui\\HP.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgPcStatus");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	m_ppgbHp = (gui::CProgressBar*)GetComponent("pgbLP");
	m_ppgbEp = (gui::CProgressBar*)GetComponent( "pgbEP" );
	m_ppgbRp = (gui::CProgressBar*)GetComponent( "pgbRP" );
	m_ppgbRpBall = (gui::CProgressBar*)GetComponent( "pgbRPBall" );
	m_sttLevel = (gui::CStaticBox*)GetComponent("sttLevel");
	m_sttHp = (gui::CStaticBox*)GetComponent("sttLP");
	m_sttMp = (gui::CStaticBox*)GetComponent("sttEP");

	m_ppnlAggro = (gui::CPanel*)GetComponent("pnlAggro");
	m_psttTargetAggroPoint = (gui::CStaticBox*)GetComponent("sttTargetAggroPoint");
	m_ppnlAggro->SetToolTip(GetDisplayStringManager()->GetString("DST_AGGROPT_AVATAR"));
	

	m_ppnlRpBack	= (gui::CPanel*)GetComponent( "pnlRPBack" );
	m_ppnlRpBallBack = (gui::CPanel*)GetComponent( "pnlRPBall" );
	m_psttNickName = (gui::CStaticBox*)GetComponent("sttNickName");
	m_pbtnAvatar = (gui::CButton*)GetComponent( "btnAvatarSelect" );
			
	m_pbtnPost = (gui::CButton*)GetComponent( "btnPostButton" );

	m_ppnlBattleCombating = (gui::CPanel*)GetComponent("pnlBattleCombating");
	m_ppnlBattleCombatingToolTip = (gui::CPanel*)GetComponent("pnlBattleCombatingToolTip");
	m_ppnlBattleCombatingToolTip->SetToolTip(GetDisplayStringManager()->GetString("DST_TIPS_BATTLECOMBATING"));

	// flight
	m_ppnlAirPoint = (gui::CPanel*)GetComponent("pnlAirPoint");
	m_ppnlAirPoint->SetToolTip(GetDisplayStringManager()->GetString("DST_AIR_CAN_NOT_FLY"));
	m_psttAirHeight = (gui::CStaticBox*)GetComponent("sttAirHeight");
	m_psttAirPoint = (gui::CStaticBox*)GetComponent("sttAirPoint");

	// scouter
	m_pPn_HpScouterBg = (gui::CPanel*)GetComponent("Pn_HpScouterBg");
	m_pBtn_HpScouterOption = (gui::CButton*)GetComponent("Btn_HpScouterOption");
	m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_NON"));

	m_slotMove = m_pThis->SigMove().Connect(this, &CHpGui::OnMove);
	m_sloTracking = m_pThis->SigResize().Connect(this, &CHpGui::OnResize);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CHpGui::OnPaint);
	m_slotPaintLast = m_pbtnPost->SigPaint().Connect(this, &CHpGui::OnPaintLast);
	m_slotClickPost = m_pbtnPost->SigClicked().Connect(this, &CHpGui::OnClickPost);
	m_slogPaintPost = m_pbtnPost->SigPaint().Connect(this, &CHpGui::OnPaintPost);
	/*m_slotPowerPaint = m_ppnlPowerText->SigPaint().Connect( this, &CHpGui::OnPowerPaint );
	m_slotPowerMove = m_ppnlPowerText->SigMove().Connect( this, &CHpGui::OnPowerMove )*/;
	m_slotAvatarFocused = m_pbtnAvatar->SigFocused().Connect(this, &CHpGui::OnAvatarFocused);
	m_slotAvatarClick = m_pbtnAvatar->SigClicked().Connect(this, &CHpGui::OnAvatarClick);

		// AvatarBtn은 버튼이 아닌것처럼. 사운드 없애기
	m_pbtnAvatar->SetClickSound( NULL );
	m_pbtnAvatar->SetDisableSound( NULL );
	m_pbtnAvatar->SetFocusSound( NULL );

	// RP는 0으로 세팅
	m_ppgbRp->SetRange( 0, 100 );
	m_ppgbRp->SetPos( 0 );

	m_ppgbRpBall->SetRange( 0, 7 );
	m_ppgbRpBall->SetPos( 0 );
	
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		NTL_RETURN(FALSE);
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
		NTL_RETURN(FALSE);
	}

	m_psttNickName->SetText(pLOBBY_CHARACTER->tSummary.awchName);

	// Tooltip
	m_ppgbRpBall->SetToolTip( GetDisplayStringManager()->GetString( "DST_RPBALL_TOOLTIP" ) );
	m_sttLevel->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_LEVEL" ) );

	CRectangle rect = m_pThis->GetScreenRect();
	m_surCharacter.SetRectWH( rect.left + 10, rect.top + 7, 81, 81 );
	m_surCharacter.SetFilterTexture( Logic_CreateTexture( "8181.png" ) );		



	// Buff
	m_pBuff = NTL_NEW CBuffDispGui;
	NTL_ASSERT( m_pBuff, "CHpGui::Create : Buff is not allocated. Not enough memory" );

	BYTE byShowFlag = MAKE_BIT_FLAG(BUFF_TYPE_BLESS) + MAKE_BIT_FLAG(BUFF_TYPE_CURSE) + MAKE_BIT_FLAG(BUFF_TYPE_SUB);
	m_pBuff->Create(byShowFlag, CBuffDispGui::NORMAL_ICONSIZE, 5, 5, DIALOG_HP );
	m_pBuff->Show( true );

	// Effect
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEmergencyEffect" );
	m_feEmergency.SetSurface( surface );
	m_feEmergency.SetTime( 0.0f, 0.5f );
	m_feEmergency.SetAlpha( 0, 255 );
	m_feEmergency.SetSize( surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetWidth() + 5,
						   surface.m_Original.rtRect.GetHeight(), surface.m_Original.rtRect.GetHeight() + 5 );

	m_feNewMail.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEffectNewMail" ) );
	m_feNewMail.SetTime( 0.0f, 0.5f );
	m_feNewMail.SetAlpha( 0, 150 );
	
	m_feMailFull.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", "srfEffectMailFull" ) );
	m_feMailFull.SetTime( 0.0f, 0.5f );
	m_feMailFull.SetAlpha( 0, 150 );

	m_feBattleCombat.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("StatusBar.srf", "srfBattleCombatingFlicker"));
	m_feBattleCombat.SetTime(0.0f, 0.5f);
	m_feBattleCombat.SetAlpha(0, 150);
	
	char acBuffer[256] = "";
	WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_STATUS"), -1, acBuffer, 256, NULL, NULL);
	m_pbtnPost->SetToolTip(acBuffer);

	// hidden by default
	m_ppnlAggro->Show(false);

	// hidden by default
	m_ppnlBattleCombating->Show(false);
	m_ppnlBattleCombatingToolTip->Show(false);
	m_feBattleCombat.SetRenderTop(true);

	// disable air default
	m_ppnlAirPoint->Show(false);


	// Event link
	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventPartyUpdateValue, 0);
	LinkMsg(g_EventUpdateRP);
    LinkMsg(g_EventUpdateRpStock);
	LinkMsg(g_EventMailSystemNfy, 0);
	LinkMsg(g_EventGameServerChangeOut);
	LinkMsg(g_EventRPCharge);
	LinkMsg(g_EventBattleCombatMode);
	LinkMsg(g_EventEnableAir);
	LinkMsg(g_EventSetAp);
	LinkMsg(g_EventSobUpdateLPStatusNfy);
	LinkMsg(g_EventChangeCharName);
	LinkMsg(g_EventScouter);

	// Update Link
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );
	GetNtlGuiManager()->AddUpdateFunc( this );

	NTL_RETURN(TRUE);
}

void CHpGui::Destroy(void)
{
	NTL_FUNCTION("CHpGui::Destroy");

	// Buff
	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	Logic_DeleteTexture( m_surCharacter.GetFilterTexture() );
	m_surCharacter.UnsetFilterTexture();
	
	// Event unlink
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventUpdateRP);	
    UnLinkMsg(g_EventUpdateRpStock);
	UnLinkMsg(g_EventMailSystemNfy);
	UnLinkMsg(g_EventGameServerChangeOut);
	UnLinkMsg(g_EventRPCharge);
	UnLinkMsg(g_EventBattleCombatMode);
	UnLinkMsg(g_EventEnableAir);
	UnLinkMsg(g_EventSetAp);
	UnLinkMsg(g_EventSobUpdateLPStatusNfy);
	UnLinkMsg(g_EventChangeCharName);
	UnLinkMsg(g_EventScouter);
	
	// Update Unlink
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CHpGui::Update( RwReal fElapsed )
{
	if( m_eRPChargeType != TYPE_NONE )
	{
		m_fCurrentRPChargingTime += fElapsed;
		
		if( m_fCurrentRPChargingTime >= m_fRPChargingTime )
		{
			m_fCurrentRPChargingTime = m_fRPChargingTime;
			
			if( m_eRPChargeType == TYPE_DECREASE_END )
				m_eRPChargeType = TYPE_NONE;
		}

		RwReal fValue =	gui::GetResultLineInterpolation( m_fCurrentRPChargingTime, m_fRPChargingTime, m_fCurrentValue, m_fDestValue );

		SetRPValue( (int)fValue, m_nMaxValue );
		Logic_SetRp( GetNtlSLGlobal()->GetSobAvatar(), (RwInt32)fValue );		

		//CHAR buf[256];
		//sprintf_s( buf, 256, "value : %f, charging : %d, CurrentTime : %f, ChargingTime : %f", fValue, m_eRPChargeType, m_fCurrentRPChargingTime, m_fRPChargingTime );		
		//NTL_WARNING( 0, buf );
	}	

	/*m_LPGaugeAnimation.Update( fElapsed, m_ppgbHp );
	m_EPGagueAnimation.Update( fElapsed, m_ppgbEp );*/
	
	m_pBuff->Update( fElapsed );

	UpdateAir();
	
	m_feEmergency.Update( fElapsed );
	m_feNewMail.Update( fElapsed );
	m_feMailFull.Update( fElapsed );
	m_feBattleCombat.Update(fElapsed);
}

void CHpGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return;

	CNtlSobCharProxy *pCharProxy = reinterpret_cast<CNtlSobCharProxy*>(pSobAvatar->GetSobProxy());
	m_texCharacter.Load(pCharProxy->UIPcStatusBarRender());
	m_surCharacter.SetTexture( &m_texCharacter );
}

RwInt32 CHpGui::SwitchDialog(bool bOpen)
{
	Show( bOpen );
	m_pBuff->Show( bOpen );
	m_pPn_HpScouterBg->Show(bOpen);
	
	return 1;
}

RwBool CHpGui::IsMouseOnAvatar(VOID)
{
	return ( m_pbtnAvatar->IsFocused() ) ? TRUE : FALSE;
}

void CHpGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CHpGui::HandleEvents");

	if(pMsg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>(pMsg.pData); 
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		if( pSobAvatar->GetSerialID() != pUpdate->hSerialId )
			NTL_RETURNVOID();

		if(pUpdate->uiUpdateType & EVENT_AIUT_ATTR)
		{
			RwUInt32 uiHp = Logic_GetLp(pSobAvatar);
			RwUInt32 uiMaxHp = Logic_GetMaxLp(pSobAvatar);
			RwUInt32 uiEp = Logic_GetEp(pSobAvatar);
			RwUInt32 uiMaxEp = Logic_GetMaxEp(pSobAvatar);
			RwUInt8 byLevel = Logic_GetLevel(pSobAvatar);

			m_nMaxValue = Logic_GetMaxRp(pSobAvatar);
			m_ppgbRp->SetRange(0, m_nMaxValue);
			int nRp = Logic_GetRp(pSobAvatar);
			m_ppgbRp->SetPos(nRp);

			m_sttLevel->SetText(byLevel); 
			m_ppgbHp->SetRange(0, uiMaxHp);
			m_ppgbHp->SetPos(uiHp);
			//m_LPGaugeAnimation.SetValue( uiHp, uiMaxHp, m_ppgbHp, TRUE );
			m_ppgbEp->SetRange( 0, uiMaxEp );
			m_ppgbEp->SetPos(uiEp);
			//m_EPGagueAnimation.SetValue( uiEp, uiMaxEp, m_ppgbEp, TRUE );
			m_sttHp->Format("%u / %u",uiHp,uiMaxHp);
			m_sttMp->Format("%u / %u",uiEp,uiMaxEp);
		}

		if( pUpdate->uiUpdateType & EVENT_AIUT_BUFF )			
		{
			m_pBuff->SetBuffAll(pSobAvatar->GetSerialID() );
		}				

		// Avatar가 초기 생성된 후 GUI에 데이터 입력하라는 최초 단일의 시그널이 EVENT_AIUT_SKILL.
		if( pUpdate->uiUpdateType & EVENT_AIUT_SKILL )
		{
			SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
			SetMaxRPBall(pAvatarInfo->byMaxRpStock);
		}

		// 실신 상태 일때는 모든 RP, RPBall은 수동으로 리셋한다.
		if( pUpdate->uiUpdateType & EVENT_AIUT_DIE )
		{
			SetRPValue( 0, m_nMaxValue );
			m_ppgbRpBall->SetPos( 0 );

			Logic_SetRp( pSobAvatar, 0, m_nMaxValue );
			Logic_SetRpStock( pSobAvatar, 0 );
			m_eRPChargeType = TYPE_NONE;
		}
	}
	else if( pMsg.Id == g_EventSobUpdateLPStatusNfy )
	{
		SNtlEventSobUpdateLPStatusNfy* pData = reinterpret_cast<SNtlEventSobUpdateLPStatusNfy*>( pMsg.pData );

		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			if( pData->bEmergency )
			{
				if( !m_feEmergency.IsWork() )
				{
					m_feEmergency.StartProc( TRUE );

					m_hSound = Logic_PlayGUISound(GSD_HEART_BEAT);
				}
			}
			else
			{
				if( m_feEmergency.IsWork() )
				{
					m_feEmergency.EndProc();
					GetSoundManager()->Stop( m_hSound );
				}
			}
		}		
	}
	else if(pMsg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(pMsg.pData);

		switch(pPacket->iMessage)
		{ 
			case PMT_PARTY_LEADER_CHANGE:
			{
				if( pPacket->hSerialId == Logic_GetAvatarHandle() )
					m_psttNickName->SetColor(255, 255, 0);
				else
					m_psttNickName->SetColor(255, 255, 255);

				break;
			}
		}
	}
	else if( pMsg.Id == g_EventRPCharge )
	{
		SDboEventRPCharge* pData = reinterpret_cast<SDboEventRPCharge*>( pMsg.pData );
		m_fCurrentRPChargingTime = 0.0f;

		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

		if( pData->bStop )
		{
			m_eRPChargeType = TYPE_NONE;
		}
		else
		{
			m_fCurrentValue = (RwReal)pAvatarInfo->sCharPf.wCurRP;

			if( pData->bIncrease )
			{
				m_fDestValue = (RwReal)pAttr->GetMaxRp();
				m_fRPChargingTime = ( m_fDestValue - m_fCurrentValue ) / ( pAttr->m_wRpRegen / 1 );
				m_eRPChargeType = TYPE_INCREASE;
			}
			else
			{
				m_fDestValue = 0.0f;
				m_fRPChargingTime = m_fCurrentValue / ( pAttr->m_wRpDimimutionRate / 4 );
				m_eRPChargeType = TYPE_DECREASE;
			}			
		}

		//CHAR buf[256];
		//sprintf_s( buf, 256, "RPCharge bIncrease : %d, bStop : %d, lastRPRegen : %u, lastDimimutionRate : %u, currentRP = %d", pData->bIncrease, pData->bStop, pAttr->m_wLastRpRegen, pAttr->m_wLastRpDimimutionRate, pAttr->GetRp() );
		//NTL_WARNING( 0, buf );
	}
	else if(pMsg.Id == g_EventUpdateRP)
	{
		SDboEventUpdateRP* pData = reinterpret_cast<SDboEventUpdateRP*>( pMsg.pData );
		CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );

		SetRPValue( pData->nRP, pData->nMaxRP );			

		if( m_eRPChargeType == TYPE_INCREASE )
		{
			m_fRPChargingTime = ( m_fDestValue - pData->nRP ) / ( pAttr->m_wRpRegen / 1 );
		}
		else if( m_eRPChargeType == TYPE_DECREASE || m_eRPChargeType == TYPE_DECREASE_END )
		{
			m_eRPChargeType = TYPE_NONE;
		}
	}
    else if(pMsg.Id == g_EventUpdateRpStock)
    {
        SDboEventUpdateRpStock* pData = reinterpret_cast<SDboEventUpdateRpStock*>(pMsg.pData);

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
       
		CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr() );
		RwInt32 nMaxRP = pAttr->GetMaxRp(); 
		RwInt32 uiRpStock = (RwUInt32)pAvatarInfo->sCharPf.byCurRPBall;
		RwInt32 uiMaxRpStock = (RwUInt32)pAvatarInfo->byMaxRpStock;

		// If RP increases, RP drops directly to zero
		if( pData->nRpStock > uiRpStock)
		{
			if( pData->nRpStock < uiMaxRpStock)
			{
				SetRPValue( 0, nMaxRP );
				Logic_SetRp(pAvatar, 0 );

				m_fCurrentRPChargingTime = 0.0f;
				m_fCurrentValue = 0.0f;
				m_fRPChargingTime = m_fDestValue / ( pAttr->m_wRpRegen / 1 );

			//	DBO_WARNING_MESSAGE("UpdateRPBallIncrease: " << pData->nRpStock);
			}    
			else
			{
				SetRPValue( 0, nMaxRP );
				Logic_SetRp(pAvatar, 0 );

			//	DBO_WARNING_MESSAGE("Reached MaxRPBall: " << pData->nRpStock);
				m_eRPChargeType = TYPE_NONE;
			}

			Logic_PlayGUISound(GSD_RP_FULL);
		}	
		else if( pData->nRpStock < uiRpStock)
		{
			if( pData->bDropByTime )
			{
				SetRPValue( nMaxRP, nMaxRP );
				Logic_SetRp(pAvatar, nMaxRP );

				m_fCurrentRPChargingTime = 0.0f;
				m_fCurrentValue = (RwReal)nMaxRP;
				m_fRPChargingTime = (RwReal)nMaxRP / ( pAttr->m_wRpDimimutionRate / 4 );

				if( pData->nRpStock == 0 )
					m_eRPChargeType = TYPE_DECREASE_END;

				//RwChar buf[256];
				//sprintf_s( buf, 256, "UpdateRPBallDecrease : %d, State : %d", pData->nRpStock, m_eRPChargeType );
				//NTL_WARNING( 0, buf );
			}								
		}

		if( pData->nRpStock < uiMaxRpStock)
		{
			// RP Gauge이미지 변경
			RwChar buf[256];
			sprintf_s( buf, 256, "srfNewRPGauge%d", pData->nRpStock );
			m_ppgbRp->ClearSurface();
			m_ppgbRp->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", buf ) );
			m_ppgbRp->SetPos( m_ppgbRp->GetPos() );				
		}			

		m_ppgbRpBall->SetPos( pData->nRpStock );

		// RpBall Max 갯수가 바뀌면 적용.
		if( pData->nMaxRpStock != uiMaxRpStock)
		{
			SetMaxRPBall( pData->nMaxRpStock );				
		}		
    }
	else if(pMsg.Id == g_EventMailSystemNfy)
	{
		SNtlEventMailSystemNfy* pEvent = reinterpret_cast<SNtlEventMailSystemNfy*>( pMsg.pData );

		// In Hp Gui, you always receive and handle the type that occurs.
		if( pEvent->uiNotifyType != CNtlMailSystem::MAILSYSTEM_NOTIFY_TYPE_NONE )
			return;

		char			acBuffer[1024];
		WCHAR			awcBuffer[1024];

		swprintf_s(awcBuffer, 1024, L"%s", GetDisplayStringManager()->GetString("DST_MAILSYSTEM_STATUS"));
		CNtlMailSystem* pMailSystem			= GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
		RwUInt32		uiUnreadMailCount	= pMailSystem->GetUnreadMailCountNormal() + pMailSystem->GetUnreadMailCountManager();
		RwBool			bMailFull			= FALSE;
		RwBool			bUnreadMail			= FALSE;

		if (uiUnreadMailCount > 0)
		{
			WCHAR awcTemp[1024];
			swprintf_s(awcTemp, 1024, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_STATUS_UNREAD"), uiUnreadMailCount);
			swprintf_s(awcBuffer, 1024, L"%s\\n%s", awcBuffer, awcTemp);

			bUnreadMail = TRUE;
		}

		if (pMailSystem->GetMailCount() >= NTL_MAX_MAIL_SLOT_COUNT)
		{
			swprintf_s(awcBuffer, 1024, L"%s\\n%s", awcBuffer, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_STATUS_FULL"));
			Logic_PlayGUISound("System\\GENERAL_BLINK.wav");
			bMailFull = TRUE;
		}

		SetEffectRunable(m_feNewMail, !bMailFull && bUnreadMail);
		SetEffectRunable(m_feMailFull, bMailFull);

		WideCharToMultiByte(GetACP(), 0, awcBuffer, -1, acBuffer, 1024, NULL, NULL);
		m_pbtnPost->SetToolTip(acBuffer);
	}
	else if (pMsg.Id == g_EventBattleCombatMode)
	{
		SDboEventBattleCombatMode* pEvent = reinterpret_cast<SDboEventBattleCombatMode*>(pMsg.pData);

		m_ppnlBattleCombating->Show(pEvent->bCombatMode);
		m_ppnlBattleCombatingToolTip->Show(pEvent->bCombatMode);

		SetEffectRunable(m_feBattleCombat, pEvent->bCombatMode);

		GetNtlSLGlobal()->GetAvatarInfo()->bCombatMode = pEvent->bCombatMode;
	}
	else if (pMsg.Id == g_EventEnableAir)
	{
		SNtlEventEnableAir* pEvent = reinterpret_cast<SNtlEventEnableAir*>(pMsg.pData);
		
		EnableAir(pEvent->bFlag);
	}
	else if (pMsg.Id == g_EventSetAp)
	{
		SNtlEventSetAp* pEvent = reinterpret_cast<SNtlEventSetAp*>(pMsg.pData);

		SetAP(pEvent->nAp / 1000, pEvent->nMaxAp / 1000);
	}
	else if( pMsg.Id == g_EventGameServerChangeOut )
	{
		m_pBuff->ClearBuff();
	}
	else if (pMsg.Id == g_EventChangeCharName)
	{
		SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(pMsg.pData);

		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		
		if (pSobAvatar->GetCharID() == pEvent->charid)
		{
			m_psttNickName->SetText(pEvent->wchCharName);
		}
	}
	else if (pMsg.Id == g_EventScouter)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (!pSobAvatar)
			return;

		SDboEventScouter* pEvent = (SDboEventScouter*)pMsg.pData;

		if (pEvent->iType == SCOUTER_EVENT_STATUS)
		{
			bool bChangeStatus = (pEvent->uiUserData > 0) ? true : false;
			bool bStatusOn = (pEvent->iUserData2 > 0) ? true : false;

			if (bChangeStatus)
			{
				if (Logic_IsValidScouterEquipped(false))
				{
					if (bStatusOn)
					{
						GetAlarmManager()->AlarmMessage("DST_SCOUTERITEM_ON_EQUIPMENT");
						m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_ON"));
						m_pBtn_HpScouterOption->ClickEnable(true);
					}
					else
					{
						GetAlarmManager()->AlarmMessage("DST_SCOUTERITEM_OFF_EQUIPMENT");
						m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_OFF"));
						m_pBtn_HpScouterOption->ClickEnable(false);
					}

					GetNtlStorageManager()->SetData(dSTORAGE_CHARACTER_SCOUTER_ON, bStatusOn);
					pSobAvatar->GetOtherParam()->SetScouterOn(bStatusOn);
				}
				else
				{
					m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_NON"));
					m_pBtn_HpScouterOption->ClickEnable(false);
					pSobAvatar->GetOtherParam()->SetScouterOn(false);
				}
			}
			else
			{
				if (Logic_IsValidScouterEquipped(false))
				{
					if (GetNtlStorageManager()->GetBoolData(dSTORAGE_CHARACTER_SCOUTER_ON))
					{
						GetAlarmManager()->AlarmMessage("DST_SCOUTERITEM_ON_EQUIPMENT");
						m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_ON"));
						m_pBtn_HpScouterOption->ClickEnable(true);
						pSobAvatar->GetOtherParam()->SetScouterOn(true);
					}
					else
					{
						GetAlarmManager()->AlarmMessage("DST_SCOUTERITEM_OFF_EQUIPMENT");
						m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_OFF"));
						m_pBtn_HpScouterOption->ClickEnable(false);
						pSobAvatar->GetOtherParam()->SetScouterOn(false);
					}
				}
				else
				{
					m_pBtn_HpScouterOption->SetToolTip(GetDisplayStringManager()->GetString("DST_SCOUTER_BUTTON_EXPLANATION_NON"));
					m_pBtn_HpScouterOption->ClickEnable(false);

					pSobAvatar->GetOtherParam()->SetScouterOn(false);
				}
			}
		}
	}

	
	NTL_RETURNVOID();
}

VOID CHpGui::SetEffectRunable(gui::CFlickerEffect& flickerEffect, RwBool bRun)
{
	if (bRun)
	{
		if( !flickerEffect.IsWork() )
			flickerEffect.StartProc( TRUE );
	}
	else
	{
		if( flickerEffect.IsWork() )
			flickerEffect.EndProc();
	}
}

VOID CHpGui::SetRPValue( RwInt32 nValue, RwInt32 nMaxValue )
{
	m_ppgbRp->SetRange( 0, nMaxValue );
	m_ppgbRp->SetPos( nValue );	
}

VOID CHpGui::SetMaxRPBall( RwInt32 nMaxRPBall )
{
	RwChar buf[256];
	sprintf_s( buf, "srfRPBallBack%d", nMaxRPBall );

	m_ppnlRpBallBack->GetSurface()->clear();
	m_ppnlRpBallBack->AddSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "StatusBar.srf", buf ) );
}

void CHpGui::UpdateAir()
{
	CheckAir();

	if (m_bIsWorldAirPossible)
	{
		CalculateAirHeight();
	}
}

void CHpGui::CheckAir()
{
	bool bIsWorldAirPossible = false;

	if (Logic_IsPassiveAirPossibleUi())
	{
		bIsWorldAirPossible = true;
	}

	if (m_bIsWorldAirPossible != bIsWorldAirPossible)
	{
		if (bIsWorldAirPossible == false)
		{
			m_ppnlAirPoint->SetToolTip(GetDisplayStringManager()->GetString("DST_AIR_CAN_NOT_FLY"));
			m_eAirColor = TYPE_DISABLE;
		}
		else
		{
			m_ppnlAirPoint->SetToolTip(GetDisplayStringManager()->GetString("DST_AIR_AP_GUI_COMMENT"));
		}
	}

	m_bIsWorldAirPossible = bIsWorldAirPossible;
}

void CHpGui::EnableAir(bool bFlag)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobAvatar->GetSobAttr());

	if (bFlag)
	{
		CheckAir();

		int nMaxAP = pSobAvatarAttr->GetMaxAp();
		if (nMaxAP == 0)
			nMaxAP = DBO_CHAR_DEFAULT_AP;

		SetAP(pSobAvatarAttr->GetAp() / 1000, nMaxAP / 1000);
		CalculateAirHeight();
	}

	m_ppnlAirPoint->Show(bFlag);
}

void CHpGui::SetAP(int nAP, int nMaxAP)
{
	int nPercent = nAP * 100 / nMaxAP;

	bool bChange = false;

	if (nPercent >= 0 && nPercent < 33) // RED
	{
		if (m_eAirColor != TYPE_RED)
		{
		//	m_surMidAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeMiddleRed"));
		//	m_surRoundAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeRoundRed"));
			m_eAirColor = TYPE_RED;
			bChange = true;
		}
	}
	else if (nPercent > 33 && nPercent < 66) // YELLOW
	{
		if (m_eAirColor != TYPE_YELLOW)
		{
		//	m_surMidAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeMiddleYellow"));
		//	m_surRoundAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeRoundYellow"));
			m_eAirColor = TYPE_YELLOW;
			bChange = true;
		}
	}
	else if (nPercent > 66) // BLUE
	{
		if (m_eAirColor != TYPE_BLUE)
		{
		//	m_surMidAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeMiddleBlue"));
		//	m_surRoundAir.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("AirPoint.srf", "srfTimeRoundBlue"));
			m_eAirColor = TYPE_BLUE;
			bChange = true;
		}
	}

	/*if (bChange)
	{
		CRectangle rec = m_psttAirPoint->GetScreenRect();

		m_surMidAir.SetPosition(rec.left - 3, rec.top - 44);
		m_surRoundAir.SetPosition(rec.left - 6, rec.top - 44);
	}*/

	m_psttAirPoint->Format("%u/%u", nAP, nMaxAP);
}

void CHpGui::CalculateAirHeight()
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (m_bIsWorldAirPossible && pSobAvatar->IsAirMode())
	{
		const RwV3d* pPos = &pSobAvatar->GetPosition();

		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(pPos, sHStuff);

		int nHeight = (int)(pPos->y - sHStuff.fFinialHeight);

		m_psttAirHeight->Format("%uM", nHeight);
	}
	else
	{
		m_psttAirHeight->SetText("AP");
	}
}

VOID CHpGui::OnPaint(VOID)
{
	m_surCharacter.Render();
}

VOID CHpGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_surCharacter.SetRectWH( rect.left + 10, rect.top + 7, 81, 81 );

	m_pBuff->SetMove( rect );

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEmergency.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEmergency.SetCenterPos( rect.left + nCenterX, rect.top + nCenterY );

	rect		= m_pbtnPost->GetScreenRect();
	nCenterX	= rect.left + ((rect.right - rect.left) / 2);
	nCenterY	= rect.top + ((rect.bottom - rect.top) / 2);
	m_feNewMail.SetCenterPos(nCenterX, nCenterY);
	m_feMailFull.SetCenterPos(nCenterX, nCenterY);


	rect = m_ppnlBattleCombating->GetScreenRect();
	nCenterX = rect.left + ((rect.right - rect.left) / 2);
	nCenterY = rect.top + ((rect.bottom - rect.top) / 2);

	m_feBattleCombat.SetCenterPos(nCenterX, nCenterY);
}

VOID CHpGui::OnResize(RwInt32 nX, RwInt32 nY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_surCharacter.SetRectWH(rect.left + 10, rect.top + 7, 81, 81);

	m_pBuff->SetMove(rect);
}

VOID CHpGui::OnPaintLast(VOID)
{
	m_feEmergency.Render();
}

VOID CHpGui::OnClickPost( gui::CComponent* pComponent )
{
	//GetDboGlobal()->GetGamePacketGenerator()->SendMailStartReq(INVALID_SERIAL_ID);
}

VOID CHpGui::OnPaintPost()
{
	m_feNewMail.Render();
	m_feMailFull.Render();
	m_feBattleCombat.Render();

	if (m_eAirColor != TYPE_DISABLE)
	{
		//m_surMidAir.Render();
		//m_surRoundAir.Render();
	}
}

//VOID CHpGui::OnPowerPaint(VOID)
//{
//	m_numPower.Render();
//}
//
//VOID CHpGui::OnPowerMove( RwInt32 nOldX, RwInt32 nOldY )
//{
//	CRectangle rtScreen = m_ppnlPowerText->GetScreenRect();
//	m_numPower.SetPosition( rtScreen.left, rtScreen.top );
//}

VOID CHpGui::OnAvatarFocused( gui::CComponent* pComponent, bool bFocused )
{
	if (GetDialogManager()->IsMode(DIALOGMODE_UNKNOWN))
	{
		if (bFocused)
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_BATTLE_TARGET);
		else
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_INVALID_CURSOR);
	}
}

VOID CHpGui::OnAvatarClick( gui::CComponent* pComponent )
{
	Logic_PlayGUISound(GSD_SYSTEM_MOUSE_CLICK_HV);

	if (GetNtlSLGlobal()->IsSkillSelectMode())
	{
		Logic_UseProc(GetNtlSLGlobal()->GetSkillSelectModeHandle(), INVALID_BYTE, Logic_GetAvatarHandle(), false);

		if (GetNtlSLGlobal()->IsSkillSelectMode())
		{
			CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
		}
	}
	else
	{
		Logic_AvatarTarget();
	}
}

