#include "precomp_dboclient.h"
#include "DojoInfo_Battle.h"


// shared
#include "NtlDojo.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLDojo.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// cleint
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"
#include "AlarmManager.h"
#include "DialogManager.h"
#include "MsgBoxManager.h"


#define dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_START_X		(14)
#define dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_START_Y		(109)
#define dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_GAP_Y		(23)


CDojoInfo_Battle::CDojoInfo_Battle()
:m_uiDojoTableIndex(INVALID_TBLIDX)
{

}

CDojoInfo_Battle::~CDojoInfo_Battle(VOID)
{

}

RwBool CDojoInfo_Battle::Create(CNtlPLGui* pParent)
{
	NTL_ASSERT(pParent, "CDojoInfo_Battle::Create, Invalid parent point");
	m_pParent = pParent;

	m_pAskDuelButton					= (gui::CButton*)pParent->GetComponent( "btnAskDuel" );
	m_pAskScrambleButton				= (gui::CButton*)pParent->GetComponent( "btnAskScramble" );

	m_pDuelDefence_PlayerStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelDefence_PlayerStatic" );
	m_pDuelDefence_NameStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelDefence_NameStatic" );
	m_pDuelDefence_LevelStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelDefence_LevelStatic" );
	m_pDuelDefence_RaceStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelDefence_RaceStatic" );
	
	m_pDuelOffenceGuild_Static			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelOffenceGuild_Static" );
	m_pDuelOffenceGuild_NameStatic		= (gui::CStaticBox*)pParent->GetComponent( "stbDuelOffenceGuild_NameStatic" );
	m_pDuelOffenceGuild_Name			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelOffenceGuild_Name" );
	m_pDuelOffenceGuild_MasterStatic	= (gui::CStaticBox*)pParent->GetComponent( "stbDuelOffenceGuild_MasterStatic" );
	m_pDuelOffenceGuild_Master			= (gui::CStaticBox*)pParent->GetComponent( "stbDuelOffenceGuild_Master" );
	
	m_pScrambleOffenceGuild_Static		= (gui::CStaticBox*)pParent->GetComponent( "stbScrambleOffenceGuild_Static" );
	m_pScrambleOffenceGuild_NameStatic	= (gui::CStaticBox*)pParent->GetComponent( "stbScrambleOffenceGuild_NameStatic" );
	m_pScrambleOffenceGuild_Name		= (gui::CStaticBox*)pParent->GetComponent( "stbScrambleOffenceGuild_Name" );
	m_pScrambleOffenceGuild_MasterStatic= (gui::CStaticBox*)pParent->GetComponent( "stbScrambleOffenceGuild_MasterStatic" );
	m_pScrambleOffenceGuild_Master		= (gui::CStaticBox*)pParent->GetComponent( "stbScrambleOffenceGuild_Master" );


	char acBuffer[256] = "";
	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		sprintf_s(acBuffer, 256, "stbDefencePlayerName_%d", i);
		m_DuelDefencePlayerGui[i].pName		= (gui::CStaticBox*)pParent->GetComponent( acBuffer );

		sprintf_s(acBuffer, 256, "stbDefencePlayerLevel_%d", i);
		m_DuelDefencePlayerGui[i].pLevel	= (gui::CStaticBox*)pParent->GetComponent( acBuffer );

		sprintf_s(acBuffer, 256, "stbDefencePlayerRace_%d", i);
		m_DuelDefencePlayerGui[i].Race		= (gui::CStaticBox*)pParent->GetComponent( acBuffer );

		sprintf_s(acBuffer, 256, "stbDefencePlayerButton_%d", i);
		m_DuelDefencePlayerGui[i].pButton	= (gui::CButton*)pParent->GetComponent( acBuffer );
	}	


	m_srf3DuelListBack.SetType(CWindowby3::WT_VERTICAL);
	m_srf3DuelListBack.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfDuelPlayerBackLineLeft" ) );
	m_srf3DuelListBack.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfDuelPlayerBackLineCenter" ) );
	m_srf3DuelListBack.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfDuelPlayerBackLineRight" ) );
	m_srf3DuelListBack.SetSize(281, 111);
	m_srf3DuelListBack.SetPositionfromParent(24, 109);

	m_srfDuelDefencePlayerBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfDuelDefencePlayerBar.SetPositionfromParent(10, 63);

	m_srfDuelOffenceGuildBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfDuelOffenceGuildBar.SetPositionfromParent(10, 264);

	m_srfScrambleOffenceGuildBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfScrambleOffenceGuildBar.SetPositionfromParent(10, 361);


	// 스트링
	m_pAskDuelButton					->SetText(GetDisplayStringManager()->GetString("DST_DOJO_BATTLE_APPLICATION_FOR_BATTLE"));
	m_pAskScrambleButton				->SetText(GetDisplayStringManager()->GetString("DST_DOJO_BATTLE_APPLICATION_FOR_BATTLE"));

	m_pDuelDefence_PlayerStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_BATTLE_DUEL_DEFFENCE_PLAYER"));
	m_pDuelDefence_NameStatic			->SetText(GetDisplayStringManager()->GetString("DST_STATUS_NAME"));
	m_pDuelDefence_LevelStatic			->SetText(GetDisplayStringManager()->GetString("DST_STATUS_LEVEL"));
	m_pDuelDefence_RaceStatic			->SetText(GetDisplayStringManager()->GetString("DST_RACE"));

	m_pDuelOffenceGuild_Static			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_BATTLE_DULE_OFFENCE_PLAYER"));
	m_pDuelOffenceGuild_NameStatic		->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NAME"));
	m_pDuelOffenceGuild_MasterStatic	->SetText(GetDisplayStringManager()->GetString("DST_GUILD_MASTER"));
	
	m_pScrambleOffenceGuild_Static		->SetText(GetDisplayStringManager()->GetString("DST_DOJO_BATTLE_SCRAMBLE_OFFENCE_PLAYER"));
	m_pScrambleOffenceGuild_NameStatic	->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NAME"));
	m_pScrambleOffenceGuild_MasterStatic->SetText(GetDisplayStringManager()->GetString("DST_GUILD_MASTER"));
	

	// defule value
	/*
	RwInt32 iX = dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_START_X;
	RwInt32 iY = dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_START_Y;
	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		m_DuelDefencePlayerGui[i].srfClass.SetSurface( Logic_GetPCClassIconSurface(0, false) );
		m_DuelDefencePlayerGui[i].srfClass.SetPositionfromParent(iX, iY);

		m_DuelDefencePlayerGui[i].pName	->SetText("QWERTYUIOPLKJHGF");
		m_DuelDefencePlayerGui[i].pLevel->SetText("100");
		m_DuelDefencePlayerGui[i].Race	->SetText("QWERTYU");

		m_DuelDefencePlayerGui[i].sloButton = m_DuelDefencePlayerGui[i].pButton->SigClicked().Connect(this, &CDojoInfo_Battle::OnClicked_DefencePlayerButton);


		iY += dDUEL_DEFENCE_PLAYER_CLASS_SURFACE_GAP_Y;
	}*/

	// sig
	m_slotAskDuelButton				= m_pAskDuelButton->SigClicked().Connect(this, &CDojoInfo_Battle::OnClicked_AskDuelButton);
	m_slotAskScrambleButton			= m_pAskScrambleButton->SigClicked().Connect(this, &CDojoInfo_Battle::OnClicked_AskScrambleButton);

	return true;
}

VOID CDojoInfo_Battle::Destroy()
{

}

VOID CDojoInfo_Battle::Clear()
{
	m_pDuelOffenceGuild_Name			->Clear();
	m_pDuelOffenceGuild_Master			->Clear();
	m_pScrambleOffenceGuild_Name		->Clear();
	m_pScrambleOffenceGuild_Master		->Clear();

	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		m_DuelDefencePlayerGui[i].pName		->Clear();
		m_DuelDefencePlayerGui[i].pLevel	->Clear();
		m_DuelDefencePlayerGui[i].Race		->Clear();

		m_DuelDefencePlayerGui[i].srfClass.UnsetTexture();
	}

	m_uiDojoTableIndex = INVALID_TBLIDX;
}

VOID CDojoInfo_Battle::ActiveTab(bool bActive)
{
	m_pAskDuelButton					->Show(bActive);
	m_pAskScrambleButton				->Show(bActive);

	m_pDuelDefence_PlayerStatic			->Show(bActive);
	m_pDuelDefence_NameStatic			->Show(bActive);
	m_pDuelDefence_LevelStatic			->Show(bActive);
	m_pDuelDefence_RaceStatic			->Show(bActive);

	m_pDuelOffenceGuild_Static			->Show(bActive);
	m_pDuelOffenceGuild_NameStatic		->Show(bActive);
	m_pDuelOffenceGuild_Name			->Show(bActive);
	m_pDuelOffenceGuild_MasterStatic	->Show(bActive);
	m_pDuelOffenceGuild_Master			->Show(bActive);

	m_pScrambleOffenceGuild_Static		->Show(bActive);
	m_pScrambleOffenceGuild_NameStatic	->Show(bActive);
	m_pScrambleOffenceGuild_Name		->Show(bActive);
	m_pScrambleOffenceGuild_MasterStatic->Show(bActive);
	m_pScrambleOffenceGuild_Master		->Show(bActive);

	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		m_DuelDefencePlayerGui[i].pName	->Show(bActive);
		m_DuelDefencePlayerGui[i].pLevel->Show(bActive);
		m_DuelDefencePlayerGui[i].Race	->Show(bActive);
		m_DuelDefencePlayerGui[i].pButton->Show(bActive);
	}
}

VOID CDojoInfo_Battle::OnClicked_DefencePlayerButton(gui::CComponent* pComponent)
{

}

VOID CDojoInfo_Battle::OnClicked_AskDuelButton(gui::CComponent* pComponent)
{

}

VOID CDojoInfo_Battle::OnClicked_AskScrambleButton(gui::CComponent* pComponent)
{
	GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_SCRAMBLE_ASK_CHALLENGE", FALSE, NULL, Logic_FormatZeni(DBO_DOJO_SCRAMBLE_REQUEST_FARE));
}

VOID CDojoInfo_Battle::OnMove(RwInt32 nX, RwInt32 nY)
{
	CRectangle rtScreen = m_pParent->GetPosition();

	m_srf3DuelListBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfDuelDefencePlayerBar	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfDuelOffenceGuildBar	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfScrambleOffenceGuildBar.SetPositionbyParent(rtScreen.left, rtScreen.top);

	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		m_DuelDefencePlayerGui[i].srfClass.SetPositionbyParent(rtScreen.left, rtScreen.top);
	}
}

VOID CDojoInfo_Battle::OnPaint()
{
	m_srf3DuelListBack				.Render();
	m_srfDuelDefencePlayerBar		.Render();
	m_srfDuelOffenceGuildBar		.Render();
	m_srfScrambleOffenceGuildBar	.Render();

	for( RwUInt8 i = 0 ; i < dDUEL_PLAYER_NUM ; ++i )
	{
		m_DuelDefencePlayerGui[i].srfClass.Render();
	}
}

VOID CDojoInfo_Battle::SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex /* = INVALID_TBLIDX */)
{
	if(bOpen)
	{
		m_uiDojoTableIndex = uiDojoTableIndex;
	}
	else
	{
		Clear();
		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_SCRAMBLE_ASK_CHALLENGE");
	}
}

VOID CDojoInfo_Battle::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventDojoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_INFO) )
			return;


		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		switch(pEvent->byDojoEvent)
		{
		case DOJO_EVENT_LOST_DOJO:
			{
				if( m_uiDojoTableIndex == pEvent->uiParam )
					Clear();

				break;
			}
		case DOJO_EVENT_NPC_GUI_INFO:
			{
				if( m_uiDojoTableIndex != pEvent->uiParam )
					return;

				sDBO_DOJO_NPC_INFO* pDBO_DOJO_NPC_INFO = reinterpret_cast<sDBO_DOJO_NPC_INFO*>( pEvent->pExData );

				m_pScrambleOffenceGuild_Name	->SetText(pDBO_DOJO_NPC_INFO->wszChallengeName);
				m_pScrambleOffenceGuild_Master	->SetText(pDBO_DOJO_NPC_INFO->wszChallengeLeaderName);
				break;
			}
		}
	}
	else if( msg.Id == g_EventScrambleDefaultInfoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_INFO) )
			return;


		// 현재 보고 있는 정보가 자신이 공격 유파에 속하지 않는다면 굳이 정보를 갱신할 필요없다
		SNtlEventScrambleDefaultInfo* pEvent = reinterpret_cast<SNtlEventScrambleDefaultInfo*>( msg.pData );

		if( pEvent->uiDojoTableIndex != m_uiDojoTableIndex )
			return;

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;

		CNtlGuild*	pGuild		= pAvatar->GetGuild();
		CNtlDojo*	pDojo		= pAvatar->GetDojo();
		sDOJO_INFO* pDOJO_INFO	= pDojo->GetDojoInfo(m_uiDojoTableIndex );

		// 보고 있는 도장의 정보가 우리 도장이라면
		if( pGuild->GetGuildID() != pDOJO_INFO->guildID )
			return;

		sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey( pGuild->GetMasterID() ) );
		if( !pMember )
		{
			DBO_FAIL("Not exist guild member of charID : " << pGuild->GetMasterID());
			return;
		}


		m_pScrambleOffenceGuild_Name	->SetText( pGuild->GetGuildName() );
		m_pScrambleOffenceGuild_Master	->SetText( pMember->wszMemberName );
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_INFO) )
			return;


		// 현재 보고 있는 정보가 자신이 공격 유파에 속하지 않는다면 굳이 정보를 갱신할 필요없다
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );

		if( SLGE_APPOINT_MASTER		== pEvent->iMessage ||
			SLGE_CHANGE_GUILD_NAME	== pEvent->iMessage )
		{
			SERIAL_HANDLE hTarget	= Logic_GetAvatarTargetHandle();
			sDOJO_INFO* pDOJO_INFO	= Logic_GetDojoInfo(hTarget);

			if( !pDOJO_INFO )
			{
				DBO_FAIL("Invalid dojo info pointer");
				return;
			}

			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
				return;

			CNtlGuild* pGuild = pAvatar->GetGuild();
			if( pGuild->GetGuildID() != pDOJO_INFO->guildID )
				return;

			sGuildMember* pMember = reinterpret_cast<sGuildMember*>( pGuild->GetMemberbyKey( pGuild->GetMasterID() ) );
			if( !pMember )
			{
				DBO_FAIL("Not exist guild member of charID : " << pGuild->GetMasterID());
				return;
			}


			m_pScrambleOffenceGuild_Name	->SetText( pGuild->GetGuildName() );
			m_pScrambleOffenceGuild_Master	->SetText( pMember->wszMemberName );
		}
	}	
}