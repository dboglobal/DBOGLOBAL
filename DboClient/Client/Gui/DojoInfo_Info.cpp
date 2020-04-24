#include "precomp_dboclient.h"
#include "DojoInfo_Info.h"


// shared
#include "NPCTable.h"
#include "DojoTable.h"
#include "TableContainer.h"
#include "NtlDojo.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLDojo.h"

// client
#include "DisplayStringManager.h"
#include "DboGlobal.h"
#include "GuildNoticeGui.h"
#include "DialogManager.h"
#include "MsgBoxManager.h"

#define dNOTICE_OUTPUT_LINE_GAP				(2)
#define dNOTICE_OUTPUT_MAX_LINE				(7)


CDojoInfo_Info::CDojoInfo_Info()
:m_pNoticeGui(NULL)
,m_uiDojoTableIndex(INVALID_TBLIDX)
{

}

CDojoInfo_Info::~CDojoInfo_Info(VOID)
{

}

RwBool CDojoInfo_Info::Create(CNtlPLGui* pParent)
{
	NTL_ASSERT(pParent, "CDojoInfo_Info::Create, Invalid parent point");
	m_pParent = pParent;

	m_pTenkaichiPlayerButton	= (gui::CButton*)pParent->GetComponent( "btnTenkaichiPlayer" );
	m_pNoticeButton				= (gui::CButton*)pParent->GetComponent( "btnNotice" );

	m_pDojoInfoStaticBar		= (gui::CStaticBox*)pParent->GetComponent( "stbDojoInfoStatic" );
	m_pDojoNameStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDojoNameStatic" );
	m_pDojoLevelStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbDojoLevelStatic" );
	m_pDojoAreaNameStatic		= (gui::CStaticBox*)pParent->GetComponent( "stbDojoAreaNameStatic" );
	m_pGuildNameStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbGuildNameStatic" );
	m_pGuildPointStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbGuildPointStatic" );
	m_pPublicPeaceStatic		= (gui::CStaticBox*)pParent->GetComponent( "stbPublicPeaceStatic" );
	m_pTenkaichiPlayerStatic	= (gui::CStaticBox*)pParent->GetComponent( "stbTenkaichiPlayerStatic" );
	m_pDojoName					= (gui::CStaticBox*)pParent->GetComponent( "stbDojoName" );
	m_pDojoLevel				= (gui::CStaticBox*)pParent->GetComponent( "stbDojoLevel" );
	m_pDojoAreaName				= (gui::CStaticBox*)pParent->GetComponent( "stbDojoAreaName" );
	m_pGuildName				= (gui::CStaticBox*)pParent->GetComponent( "stbGuildName" );
	m_pGuildPoint				= (gui::CStaticBox*)pParent->GetComponent( "stbGuildPoint" );
	m_pPublicPeace				= (gui::CStaticBox*)pParent->GetComponent( "stbPublicPeace" );
	m_pTenkaichiPlayer			= (gui::CStaticBox*)pParent->GetComponent( "stbTenkaichiPlayer" );

	m_pNoticeBarStatic			= (gui::CStaticBox*)pParent->GetComponent( "stbNoticeBarStatic" );
	m_pNoticeCharName			= (gui::CStaticBox*)pParent->GetComponent( "stbNoticeDay" );

	m_pNoticeOutput				= (gui::COutputBox*)pParent->GetComponent( "outputNotice" );


	m_srfDojoInfoBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfDojoInfoBar.SetPositionfromParent(10, 63);

	m_srfNotifyBar.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfBar" ) );
	m_srfNotifyBar.SetPositionfromParent(10, 300);

	m_NoticeBackground.SetType(CWindowby3::WT_VERTICAL);
	m_NoticeBackground.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfGuildNoticeLeft" ) );
	m_NoticeBackground.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfGuildNoticeCenter" ) );
	m_NoticeBackground.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DojoInfo.srf", "srfGuildNoticeRight" ) );
	m_NoticeBackground.SetSize(290, 140);
	m_NoticeBackground.SetPositionfromParent(15, 318);

	
	// 스트링
	m_pTenkaichiPlayerButton	->SetToolTip(GetDisplayStringManager()->GetString("DST_DOJO_INFO_SET_TENKAI_SEED_PLAYER"));
	m_pNoticeButton				->SetToolTip(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NOTICE"));

	m_pDojoInfoStaticBar		->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_DOJO_INFO"));
	m_pDojoNameStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_DOJO_NAME"));
	m_pDojoLevelStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_DOJO_LEVEL"));
	m_pDojoAreaNameStatic		->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_CITY_NAME"));
	m_pGuildNameStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NAME"));
	m_pGuildPointStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_POINT"));
	m_pPublicPeaceStatic		->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_PUBLIC_SECURITY"));
	m_pTenkaichiPlayerStatic	->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_TENKAI_SEED_PLAYER"));
	

	m_pNoticeBarStatic			->SetText(GetDisplayStringManager()->GetString("DST_DOJO_INFO_GUILD_NOTICE"));

	// setup
	m_pNoticeOutput->SetLineSpace(dNOTICE_OUTPUT_LINE_GAP);
	m_pNoticeOutput->SetMaxLine(dNOTICE_OUTPUT_MAX_LINE);


	// sig
	m_slotTenkaichiPlayerButton	= m_pTenkaichiPlayerButton->SigClicked().Connect(this, &CDojoInfo_Info::OnClicked_TenkaichiPlayerButton);
	m_slotNoticeButton			= m_pNoticeButton->SigClicked().Connect(this, &CDojoInfo_Info::OnClicked_NoticeButton);

	return TRUE;
}

VOID CDojoInfo_Info::Update(RwReal fElapsed)
{

}

VOID CDojoInfo_Info::Destroy()
{

}

VOID CDojoInfo_Info::Clear()
{
	m_pDojoName			->Clear();
	m_pDojoLevel		->Clear();
	m_pDojoAreaName		->Clear();
	m_pGuildName		->Clear();
	m_pGuildPoint		->Clear();
	m_pPublicPeace		->Clear();
	m_pTenkaichiPlayer	->Clear();
	m_pNoticeCharName	->Clear();
	m_pNoticeOutput		->Clear();

	m_uiDojoTableIndex = INVALID_TBLIDX;
	ClearNoticeGui();
}

VOID CDojoInfo_Info::ActiveTab(bool bActive)
{
	m_pTenkaichiPlayerButton		->Show(bActive);
	m_pNoticeButton					->Show(bActive);

	m_pDojoInfoStaticBar			->Show(bActive);
	m_pDojoNameStatic				->Show(bActive);
	m_pDojoLevelStatic				->Show(bActive);
	m_pDojoAreaNameStatic			->Show(bActive);
	m_pGuildNameStatic				->Show(bActive);
	m_pGuildPointStatic				->Show(bActive);
	m_pPublicPeaceStatic			->Show(bActive);
	m_pTenkaichiPlayerStatic		->Show(bActive);
	m_pDojoName						->Show(bActive);
	m_pDojoLevel					->Show(bActive);
	m_pDojoAreaName					->Show(bActive);
	m_pGuildName					->Show(bActive);
	m_pGuildPoint					->Show(bActive);
	m_pPublicPeace					->Show(bActive);
	m_pTenkaichiPlayer				->Show(bActive);

	m_pNoticeBarStatic				->Show(bActive);
	m_pNoticeCharName				->Show(bActive);

	m_pNoticeOutput					->Show(bActive);

	if( m_pNoticeGui )
		m_pNoticeGui->Active(bActive);
}

VOID CDojoInfo_Info::SetDojoInfo(sDBO_DOJO_NPC_INFO* pDBO_DOJO_NPC_INFO)
{
	if(!pDBO_DOJO_NPC_INFO)
	{
		DBO_FAIL("Invalid pointer");
		return;
	}

	sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(pDBO_DOJO_NPC_INFO->dojoTblidx) );
	if(!pDOJO_TBLDAT)
	{
		DBO_FAIL("Not exist dojo table of index : " << pDBO_DOJO_NPC_INFO->dojoTblidx);
		return;
	}

	// 지역 이름 얻어오기
	CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetTextTbl(CTextAllTable::MAP_NAME);
	std::wstring wstrRegionName = pTextTable->GetText(pDOJO_TBLDAT->mapName);

	if( wcslen(pDBO_DOJO_NPC_INFO->wszName) == 0 )
	{
		std::wstring wstrDojoName = wstrRegionName;
		wstrDojoName += L" ";
		wstrDojoName += GetDisplayStringManager()->GetString("DST_DOJO");
		m_pDojoName->SetText( wstrDojoName.c_str() );
	}
	else
	{
		std::wstring wstrDojoName = pDBO_DOJO_NPC_INFO->wszName;
		wstrDojoName += L" ";
		wstrDojoName += GetDisplayStringManager()->GetString("DST_DOJO");
		m_pDojoName->SetText( wstrDojoName.c_str() );
	}

	switch( pDBO_DOJO_NPC_INFO->byPeaceStatus )
	{
	case eDBO_DOJO_PEACE_TYPE_PANIC:
		{
			m_pPublicPeace->SetText( GetDisplayStringManager()->GetString("DST_DOJO_PEACE_STATE_PANIC") );
			break;
		}
	case eDBO_DOJO_PEACE_TYPE_FEAR:
		{
			m_pPublicPeace->SetText( GetDisplayStringManager()->GetString("DST_DOJO_PEACE_STATE_FEAR") );
			break;
		}
	case eDBO_DOJO_PEACE_TYPE_COMMON:
		{
			m_pPublicPeace->SetText( GetDisplayStringManager()->GetString("DST_DOJO_PEACE_STATE_COMMON") );
			break;
		}
	case eDBO_DOJO_PEACE_TYPE_QUIET:
		{
			m_pPublicPeace->SetText( GetDisplayStringManager()->GetString("DST_DOJO_PEACE_STATE_QUIET") );
			break;
		}
	case eDBO_DOJO_PEACE_TYPE_FESTIVAL:
		{
			m_pPublicPeace->SetText( GetDisplayStringManager()->GetString("DST_DOJO_PEACE_STATE_FESTIVAL") );
			break;
		}
	default:
		{
			DBO_FAIL("Invalid dojo peace status : " << pDBO_DOJO_NPC_INFO->byPeaceStatus);
			break;
		}
	}
	
	m_pDojoLevel				->Format(L"%d / %d", pDBO_DOJO_NPC_INFO->byLevel, DBO_MAX_LEVEL_DOJO_IN_WORLD);
	m_pDojoAreaName				->SetText( wstrRegionName.c_str() );
	m_pGuildName				->SetText( pDBO_DOJO_NPC_INFO->wszName );
	m_pGuildPoint				->Format(L"%d / %d", pDBO_DOJO_NPC_INFO->dwGuildReputation, pDBO_DOJO_NPC_INFO->dwMaxGuildPointEver);
	m_pTenkaichiPlayer			->SetText( pDBO_DOJO_NPC_INFO->wszSeedCharName );

	if( wcslen(pDBO_DOJO_NPC_INFO->wchNotice) > 0 )
	{
		m_pNoticeCharName			->SetText(pDBO_DOJO_NPC_INFO->wchLeaderName);
		m_pNoticeOutput				->AddText(pDBO_DOJO_NPC_INFO->wchNotice);
	}	

	m_uiDojoTableIndex			= pDBO_DOJO_NPC_INFO->dojoTblidx;
}

VOID CDojoInfo_Info::ClearNoticeGui()
{
	if( !m_pNoticeGui )
		return;

	m_pParent->UnLink( m_pNoticeGui->GetDialog() );
	CNtlPLGuiManager::GetInstance()->RemoveGui(m_pNoticeGui);

	m_pNoticeGui->Destroy();
	NTL_DELETE(m_pNoticeGui);
}

VOID CDojoInfo_Info::OnClicked_TenkaichiPlayerButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuild* pGuild = pAvatar->GetGuild();
	if( FALSE == pGuild->IsHaveGroup() )
	{
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
		return;
	}

	GetAlarmManager()->AlarmMessage("DST_DOJO_ADD_TENKAI_SEED");
}

VOID CDojoInfo_Info::OnClicked_NoticeButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsHaveGroup() == FALSE )
	{
		GetAlarmManager()->AlarmMessage("DST_GUILD_NOT_JOIN");
		return;
	}

	if( !Logic_I_am_GuildLeader() )
	{
		GetAlarmManager()->AlarmMessage("COMMUNITY_GUILD_FUNCTION_NOT_FOUND", TRUE);
		return;
	}

	if( !m_pNoticeGui )
	{
		CRectangle rtScreen = m_pParent->GetPosition();

		m_pNoticeGui = NTL_NEW CGuildNoticeGui("DojoNoticeGui");
		m_pNoticeGui->Create(CGuildNoticeGui::NOTICE_TYPE_DOJO);
		m_pNoticeGui->SetPosition(rtScreen.left + (rtScreen.GetWidth() - m_pNoticeGui->GetWidth())/2,
			rtScreen.top + 128 );

		m_pParent->Link( m_pNoticeGui->GetDialog() );
		CNtlPLGuiManager::GetInstance()->AddGui(m_pNoticeGui);

		m_pNoticeGui->SetInputFocus();
	}
}

VOID CDojoInfo_Info::OnMouseDown(const CKey& key)
{

}

VOID CDojoInfo_Info::OnMouseUp(const CKey& key)
{

}

VOID CDojoInfo_Info::OnMove(RwInt32 nX, RwInt32 nY)
{
	CRectangle rtScreen = m_pParent->GetPosition();

	m_srfDojoInfoBar	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfNotifyBar		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_NoticeBackground	.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CDojoInfo_Info::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{

}

VOID CDojoInfo_Info::OnMouseLeave(gui::CComponent* pComponent)
{

}

VOID CDojoInfo_Info::OnPaint()
{
	m_srfDojoInfoBar.Render();
	m_srfNotifyBar.Render();
	m_NoticeBackground.Render();
}

VOID CDojoInfo_Info::SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex /* = INVALID_TBLIDX */)
{
	if(bOpen)
	{
		m_uiDojoTableIndex = uiDojoTableIndex;
	}
	else
	{
		Clear();
		GetMsgBoxManager()->DeleteMsgBox("DST_DOJO_ADD_TENKAI_SEED");
	}
}

VOID CDojoInfo_Info::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventDojoNotify )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_INFO) )
			return;


		SNtlEventDojo* pEvent = reinterpret_cast<SNtlEventDojo*>( msg.pData );

		if( pEvent->uiParam != m_uiDojoTableIndex )
			return;

		switch(pEvent->byDojoEvent)
		{
		case DOJO_EVENT_LOST_DOJO:
			{
				Clear();
				break;
			}
		case DOJO_EVENT_CHANGE_LEVEL:
			{
				m_pDojoLevel->Format(L"%d / %d", *(RwUInt8*)pEvent->pExData + 1, DBO_MAX_LEVEL_DOJO_IN_WORLD);
				break;
			}
		case DOJO_EVENT_CHANGE_TENKAI_SEED_PLAYER:
			{
				m_pTenkaichiPlayer->SetText((const WCHAR*)pEvent->pExData);
				break;
			}
		case DOJO_EVENT_NPC_GUI_INFO:
			{
				SetDojoInfo((sDBO_DOJO_NPC_INFO*)pEvent->pExData);
				break;
			}
		case DOJO_EVENT_CHANGE_DOJO_NOTICE:
			{
				m_pNoticeCharName->SetText((WCHAR*)pEvent->pExData);

				m_pNoticeOutput->Clear();
				m_pNoticeOutput->AddText((WCHAR*)pEvent->pExData2);
				m_pNoticeOutput->FirstLine();				
				break;
			}
		}
	}
	else if( msg.Id == g_EventNotifyGuild )
	{
		// GUI를 열 때마다 정보를 갱신하기에 닫혀있는 상태에서는 갱신하지 않는다
		if( FALSE == GetDialogManager()->IsOpenDialog(DIALOG_DOJO_INFO) )
			return;


		// 현재 보고 있는 정보가 자신이 공격 유파에 속하지 않는다면 굳이 정보를 갱신할 필요없다
		SNtlEventNotifyGuild* pEvent = reinterpret_cast<SNtlEventNotifyGuild*>( msg.pData );

		if( SLGE_MEM_REPUTATION		== pEvent->iMessage ||
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


			m_pGuildName	->SetText( pGuild->GetGuildName() );
			m_pGuildPoint	->Format(L"%d / %d", pGuild->GetReputation(), pGuild->GetReputationEver());
		}
	}	
	else if( msg.Id == g_EventPostDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iType == DIALOGEVENT_CLOSE_GUILD_NOTICE )
			ClearNoticeGui();
	}
}