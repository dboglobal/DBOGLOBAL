#include "precomp_dboclient.h"
#include "DeliberationRankGui.h"

// core
#include "NtlDebug.h"

// framework
#include "NtlStage.h"
#include "StageManager.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// client
#include "DboGlobal.h"
#include "DialogManager.h"
#include "AlarmManager.h"


#define dREST_TIME				(3600.f)
#define dSHOW_TIME				(3.f)


//////////////////////////////////////////////////////////////////////////
//	CDeliberationRankGui
//////////////////////////////////////////////////////////////////////////

CDeliberationRankGui::CDeliberationRankGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CDeliberationRankGui::~CDeliberationRankGui()
{
}

RwBool CDeliberationRankGui::Create(RwBool bGameStage)
{
	NTL_FUNCTION( "CDeliberationRankGui::Create" );

	if(!CNtlPLGui::Create("gui\\DeliberationRank.rsr", "gui\\DeliberationRank.srf", "gui\\DeliberationRank.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_bGameStage = bGameStage;

	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDeliberationRankGui::Destroy()
{
	NTL_FUNCTION("CDeliberationRankGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDeliberationRankGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	if( m_bGameStage )
	{
		CNtlPLGui* pGui = GetDialogManager()->GetDialog(DIALOG_MINIMAP);
		if( !pGui )
			return;

		CRectangle rtMiniMap = pGui->GetPosition();		
		SetPosition(rtMiniMap.left - (10 + GetWidth()), 7);
	}
	else
	{
		SetPosition(iWidth - (8 + GetWidth()), 7);		
	}
}

VOID CDeliberationRankGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDeliberationRankGui::HandleEvents");

	if( msg.Id == g_EventLoginMessage )
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( msg.pData );
		
		if( LOGIN_EVENT_FINISH_LOADING_AND_MOVIE == pEvent->byMessage )
		{
			LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
			Show(true);
		}
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocateComponent(pPacket->iWidht, pPacket->iHeight);
	}

	NTL_RETURNVOID();
}



//////////////////////////////////////////////////////////////////////////
//	CDeliberationRankContainer
//////////////////////////////////////////////////////////////////////////

CDeliberationRankContainer::CDeliberationRankContainer()
:m_pDeliberationRank(NULL)
{

}

CDeliberationRankContainer::~CDeliberationRankContainer()
{

}

RwBool CDeliberationRankContainer::Create()
{
	NTL_FUNCTION("CDeliberationRankContainer::HandleEvents");

	// default value
	m_tALARM_INFO.bGameStage	= FALSE;
	m_tALARM_INFO.fElapsed		= dREST_TIME;
	m_tALARM_INFO.fShowTime		= dSHOW_TIME;

	CNtlStage* pStage = CStageManager::GetInstance()->GetActiveStage();
	if( pStage )
	{
		std::string strStageName = pStage->GetName();
		if( GAME_STAGE_NAME == strStageName )
			m_tALARM_INFO.bGameStage = TRUE;
	}

	if( !m_tALARM_INFO.bGameStage )
		CreateGui();
	

	LinkMsg(g_EventUpdateTick);
	LinkMsg(g_EventLoginMessage);
	LinkMsg(g_EventResize, NULL, 0);

	NTL_RETURN(TRUE);
}

VOID CDeliberationRankContainer::Destroy()
{
	DestroyGui();

	UnLinkMsg(g_EventUpdateTick);
	UnLinkMsg(g_EventLoginMessage);
	UnLinkMsg(g_EventResize);
}

VOID CDeliberationRankContainer::Update(RwReal fElapsed)
{
	// 일정 시간이 지나면 잠시동안 등급표시를 한다
	if( !m_tALARM_INFO.bGameStage )
		return;

	m_tALARM_INFO.fElapsed -= fElapsed;
	if( m_tALARM_INFO.fElapsed > 0.f )
		return;

	if( !m_pDeliberationRank )
	{
		CreateGui();
		
		if( m_pDeliberationRank )
			m_pDeliberationRank->Show(true);

		GetAlarmManager()->AlarmMessage("DST_DELIBERATION_RANK_MESSAGE");
	}

	m_tALARM_INFO.fShowTime -= fElapsed;
	if( m_tALARM_INFO.fShowTime > 0.f )
		return;


	m_tALARM_INFO.fElapsed		= dREST_TIME;
	m_tALARM_INFO.fShowTime		= dSHOW_TIME;

	DestroyGui();
}

VOID CDeliberationRankContainer::CreateGui()
{
	if( m_pDeliberationRank )
		return;

	m_pDeliberationRank = NTL_NEW CDeliberationRankGui("CDeliberationRankGui");
	if(!m_pDeliberationRank->Create(m_tALARM_INFO.bGameStage))
	{
		m_pDeliberationRank->Destroy();
		NTL_DELETE(m_pDeliberationRank);
		return;
	}
}

VOID CDeliberationRankContainer::DestroyGui()
{
	if( m_pDeliberationRank )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pDeliberationRank);
		m_pDeliberationRank->Destroy();
		NTL_DELETE(m_pDeliberationRank);
	}
}

VOID CDeliberationRankContainer::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDeliberationRankContainer::HandleEvents");

	if( msg.Id == g_EventUpdateTick )
	{
		RwReal fElapsed = *reinterpret_cast<RwReal*>(msg.pData);
		Update(fElapsed);
	}
	else
	{
		if( m_pDeliberationRank )
			m_pDeliberationRank->HandleEvents(msg);
	}	

	NTL_RETURNVOID();
}