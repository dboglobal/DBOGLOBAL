#include "precomp_dboclient.h"
#include "LogoGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboGlobal.h"
#include "LoginStageState.h"
#include "MoviePlayer.h"
#include "DboApplication.h"


CLogoGui::CLogoGui(const RwChar* pName)
:CNtlPLGui(pName)
{
}

CLogoGui::~CLogoGui()
{
}

RwBool CLogoGui::Create()
{
	NTL_FUNCTION( "CLogoGui::Create" );

	CRectangle rect(0, 0, 0, 0);
	
	m_pThis = NTL_NEW gui::CDialog( &rect, CNtlPLGuiManager::GetInstance()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );	

	LinkMsg(g_EventLogInStageStateEnter);

	NTL_RETURN(TRUE);
}

VOID CLogoGui::Destroy()
{
	NTL_FUNCTION("CLogoGui::Destroy");

	UnLinkMsg(g_EventLogInStageStateEnter);

	NTL_RETURNVOID();
}

int CLogoGui::CallBackMoviePlayStop()
{
	CDboEventGenerator::LoginEvent(LOGIN_EVENT_FINISH_LOGO_PLAY);
	return TRUE;
}

VOID CLogoGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CLogoGui::HandleEvents");

	if( msg.Id == g_EventLogInStageStateEnter )
	{
		SDboEventLogInStageStateEnter* pEvent = reinterpret_cast<SDboEventLogInStageStateEnter*>(msg.pData);

		if( pEvent->byState == LOGIN_STATE_LOGO )
		{
			if( GetMoviePlayer()->Play(dLOGO_MOVIE, MOVIE_SIZE_RATIO_FULL) )
			{
				Logic_SetOptionMoviePlay();
				GetMoviePlayer()->SetCallBack(this, &CLogoGui::CallBackMoviePlayStop);				
			}
			else
			{
				CallBackMoviePlayStop();
			}
		}
	}

	NTL_RETURNVOID();
}