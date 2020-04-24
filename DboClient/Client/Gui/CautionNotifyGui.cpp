#include "precomp_dboclient.h"
#include "CautionNotifyGui.h"

// Sound
#include "GUISoundDefine.h"

// shared
#include "NtlBattle.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "DialogPriority.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNotifyTimer
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CNotifyTimer::CNotifyTimer()
: m_pCallTimer(0)
, m_fCurrentTime(0.0f)
, m_fIntervalTime(dDEFAULT_NOTIFY_TIMER_INVERVAL_TIME)
{

}

/**
* \brief Constructions
*/
CNotifyTimer::CNotifyTimer(RwReal fIntervalTime) 
: m_pCallTimer(0)
, m_fCurrentTime(0.0f)
{
	m_fIntervalTime = fIntervalTime;
}

/**
* \brief Destruction
*/
CNotifyTimer::~CNotifyTimer() 
{
	UnLinkIntervalTimer();
}

/**
* \brief 타이머가 발동될 간격을 설정해준다.
* \param fIntervalTIme	(RwReal) 타이머가 발동될 간격
*/
void CNotifyTimer::SetIntervalTime( RwReal fIntervalTime ) 
{
	m_fIntervalTime = fIntervalTime;
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
void CNotifyTimer::Update( RwReal fElapsed ) 
{
	m_fCurrentTime += fElapsed;

	if( m_fCurrentTime > m_fIntervalTime )
	{
		m_fCurrentTime = 0.0f;

		if( m_pCallTimer )
			m_pCallTimer->Call( m_fIntervalTime );
	}
}


/**
* \brief CNtlCallbackParam1float 에 Callback이 등록되어 있다면 해제해준다.
*/
void CNotifyTimer::UnLinkIntervalTimer( void ) 
{
	if( m_pCallTimer )
		NTL_DELETE( m_pCallTimer );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCautionNotifyContinuance
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CCautionNotifyContinuance::CCautionNotifyContinuance()
: m_pNotifyGui(NULL)
, m_strStringID("")
, m_fRemainTime(0.0f)
{

}

/**
* \brief Constructions
*/
CCautionNotifyContinuance::CCautionNotifyContinuance( CCautionNotifyGui* pNotify, std::string& uiStringID, RwReal fLimitTime, RwReal fIntervalTime ) 
{
	m_pNotifyGui = pNotify;
	m_strStringID = uiStringID;
	m_fRemainTime = fLimitTime;
	
	m_Timer.SetIntervalTime( fIntervalTime );
	m_Timer.LinkIntervalTimer( this, &CCautionNotifyContinuance::OnIntervalTimer );
}

/**
* \brief Destruction
*/
CCautionNotifyContinuance::~CCautionNotifyContinuance() 
{
	m_Timer.UnLinkIntervalTimer();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
void CCautionNotifyContinuance::Update( RwReal fElapsed ) 
{
	m_Timer.Update( fElapsed );
}

/**
* \brief 남은 시간을 찾는다.
* \return 남은 시간
*/
RwReal CCautionNotifyContinuance::IsRemain() 
{
	return m_fRemainTime;
}

/**
* \brief CNotifyTimer에 등록된 Callback
* \param fElapsed	(RwReal) IntervalTime
*/
int CCautionNotifyContinuance::OnIntervalTimer( RwReal fIntervalTime ) 
{
	m_fRemainTime -= fIntervalTime;

	WCHAR awcBuffer[256];
	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( m_strStringID ), (RwInt32)m_fRemainTime );
	m_pNotifyGui->ShowCautionNotify( TRUE, m_strStringID, awcBuffer );

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCautionNotifyGui
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Constructions
*/
CCautionNotifyGui::CCautionNotifyGui() 
{
	m_pStbOut = NULL;

	m_bFinish = TRUE;
	m_fLifeTime = 0.0f;
	m_fCurrentTime = 0.0f;

	m_bFadeOut = FALSE;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";

}

/**
* \brief Constructions
*/
CCautionNotifyGui::CCautionNotifyGui( const RwChar* pName )
: CNtlPLGui(pName)
{
	m_pStbOut = NULL;

	m_bFinish = TRUE;
	m_fLifeTime = 0.0f;
	m_fCurrentTime = 0.0f;

	m_bFadeOut = FALSE;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";
}

/**
* \brief Destruction
*/
CCautionNotifyGui::~CCautionNotifyGui() 
{
	for( CONTINUANCELIST::iterator it = m_listContinuance.begin(); it != m_listContinuance.end(); ++it )
	{
		NTL_DELETE( (*it) );
	}
	m_listContinuance.clear();

}

/**
* \brief Create
* \return 성공여부
*/
RwBool CCautionNotifyGui::Create( VOID ) 
{
	NTL_FUNCTION("CCautionNotifyGui::Create");

	// Create Components
	if(!CNtlPLGui::Create("", "", "gui\\CautionNotify.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_NOTIFY );

	m_pStbOut = (gui::CStaticBox*)GetComponent("sttOut");


	Enable(FALSE);

	// Add Update
	GetNtlGuiManager()->AddUpdateFunc(this);

	// Link Message
	LinkMsg( g_EventTimeFixedNotify, 0 );
	LinkMsg( g_EventResize, 0 );
	
	// Event를 등록한다.
	RegisterHandler();

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CCautionNotifyGui::Destroy( VOID ) 
{
	NTL_FUNCTION( "CCautionNotifyGui::Destroy");

	UnLinkMsg( g_EventTimeFixedNotify );
	UnLinkMsg( g_EventResize );

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief Update
* \param fElapsed	(RwReal) 이전 업데이트에서 경과된 시간
*/
VOID CCautionNotifyGui::Update( RwReal fElapsed ) 
{
	// 만약 Continuance 의 리스트가 비어 있지 않다면 업데이트 해준다.
	if( !m_listContinuance.empty() )
	{
		CONTINUANCELIST::iterator it = m_listContinuance.begin();

		while( it != m_listContinuance.end() )
		{
			// 업데이트하고 RemainTime이 1.0 초보다 낮다면 삭제해준다. ( 그전에 이미 Callback은 호출된 상태 )
			(*it)->Update( fElapsed );
			if( (*it)->IsRemain() <= 1.0f )
			{
				// 삭제
				CCautionNotifyContinuance *pContinuance = (*it);
				it = m_listContinuance.erase(it);
				NTL_DELETE( pContinuance );

				continue;
			}

			it++;
		}	
	}

	if( m_bFinish )
		return;

	m_fCurrentTime += fElapsed;
	
	// 지속시간보다 현재 시간이 더 지났으면 Fade Out시킨다.
	if( m_fCurrentTime > m_fLifeTime )
	{
		m_bFadeOut = TRUE;
	}

	// FadeOut 되고, FadeOut이 완료가 되면 지워진다.
	if(m_bFadeOut)
	{
		m_fFadeOutTime += fElapsed;
		if(m_fFadeOutTime >= CAUTION_NOTIFY_FADEOUT_TIME)
		{
			m_bFinish = TRUE;
			m_fCurrentTime = 0.0f;
			Show(false);

			return;
		}

		RwUInt8 byAlpha = (RwUInt8)((CAUTION_NOTIFY_FADEOUT_TIME - m_fFadeOutTime)*255.0f/CAUTION_NOTIFY_FADEOUT_TIME);
		m_pStbOut->SetAlpha(byAlpha);
	}
}

/**
* \brief HandleEvents
* \param msg	(RWS::CMsg*) 이벤트 메시지 구조체
*/
VOID CCautionNotifyGui::HandleEvents( RWS::CMsg& msg ) 
{
	if( msg.Id == g_EventTimeFixedNotify )
	{
		SDboEventTimeFixedNotify* pNotify = reinterpret_cast<SDboEventTimeFixedNotify*>( msg.pData );

		// 핸들러에 등록되어 있다면 관련 함수를 호출하고 Return한다.
		HANDLEMAP::iterator it = m_mapNotifyHandler.find( pNotify->strStringID );
		if( it != m_mapNotifyHandler.end() )
		{
			(this->*((*it).second))(pNotify->bActive, pNotify->strStringID);
			return;
		}

		ShowCautionNotify( pNotify->bActive, pNotify->strStringID, pNotify->pString );

		// 같은 메세지 중복시 현재는 메세지 들어올때마다 출력, 중복을 시킬라면  ShowCautionNotify 내에서 처리할것,
		Logic_PlayGUISound(GSD_SYSTEM_WARNING_NOTIFY);		
	}
	// 해상도가 변경되었을 때 날라오는 이벤트
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize *pData = reinterpret_cast<SNtlPLEventResize*>( msg.pData );

		// 리사이즈
		PositionAlign( pData->iWidht, pData->iHeight );
	}
}

/**
* \brief 출력될 텍스트의 위치를 정렬한다.
*/
VOID CCautionNotifyGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight ) 
{
	m_pThis->SetPosition( (RwInt32)( (nScreenWidth - m_pThis->GetPosition().GetWidth() ) /2 ),
		(RwInt32)( (nScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fCautionNotiry_YPosRate ) ) );
}

/**
* \brief 현재의 Caution 에 Text를 세팅하고 초기화한다.
*/
VOID CCautionNotifyGui::SetCurrentCautionNotify(std::string& strStringID, const WCHAR* pwcString )
{
	m_fCurrentTime = 0.0f;
	m_fFadeOutTime = 0.0f;
	m_fLifeTime = CAUTION_NOTIFY_LIFE_TIME;
	
	m_strStringID = strStringID;
	m_pStbOut->SetText( pwcString );
	m_pStbOut->SetAlpha(CAUTION_NOTIFY_DEFAULT_ALPHA);
	
	PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
	
	m_bFinish = FALSE;
	m_bFadeOut = FALSE;

	Show(true);
}

/**
* \brief 경고메시지를 출력하는 함수 ( 이미 띄우고 있으면 기존에 있던 것을 SideIcon으로 보내버린다. )
*/
VOID CCautionNotifyGui::ShowCautionNotify( RwBool bActive, std::string& strStringID, const WCHAR* pwcString )
{
	if( bActive )
	{
		// 이미 출력하고 있는 CautionNotify가 없다면 바로 출력한다.
		if( m_bFinish )
			SetCurrentCautionNotify( strStringID, pwcString );
		else
		{
			// 출력하고 있는 CautionNotify가 있다면 SideIcon으로 현재의 출력되고 있는 CautionNotify를 보내버리고
			// 새로운 CautionNotify를 등록한다.
			CDboEventGenerator::CautionSideNotify( TRUE, strStringID, m_pStbOut->GetText().c_str(), m_fLifeTime-m_fCurrentTime);

			SetCurrentCautionNotify( strStringID, pwcString );
		}
	}
	else
	{
		// 현재 출력되고 있는 ID와 삭제하라고 날라온 ID가 동일한 경우 지워준다.
		if( m_bFinish )
		{
			if( m_strStringID == strStringID )
				ClearCurrentCautionNotify();
		}
	}
}

/**
* \brief 현재의 CautionNotify를 초기화한다.
*/
VOID CCautionNotifyGui::ClearCurrentCautionNotify() 
{
	m_fCurrentTime = 0.0f;
	m_fLifeTime = 0.0f;
	m_fFadeOutTime = 0.0f;

	m_strStringID = "";

	m_pStbOut->SetAlpha(CAUTION_NOTIFY_DEFAULT_ALPHA);
	m_pStbOut->Clear();

	m_bFinish = TRUE;

	Show(false);
}

/**
* \brief Free PVP 장외 처리 아웃 메시지의 핸들러
* \param bActive	(RwBool) 등록인지 삭제인지
* \param uiStringID	(RwUInt32) 문자열 인덱스
*/
VOID CCautionNotifyGui::FreePvpOutSideHandler( RwBool bActive, std::string strStringID)
{
	// bActive 가 FALSE면 제거한다.
	if( bActive )
	{
		if( !m_listContinuance.empty() )
			DeleteContinuance( strStringID );
	
		// 등록
		AddContinuance( strStringID, dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME, dDEFAULT_NOTIFY_TIMER_INVERVAL_TIME );

		// 한번은 출력해준다.
		WCHAR awcBuffer[256];
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_FREEPVP_OUTSIDE_STAGE" ), (RwInt32)dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME );
		ShowCautionNotify( TRUE, strStringID, awcBuffer );
		
	}
	else
	{
		// 리스트에 아무것도 안 들어 있다면 그냥 리턴시킨다.
		if( !m_listContinuance.empty() )
			DeleteContinuance( strStringID );
	}
}

/**
* \brief 핸들러에 등록될 것들을 등록한다.
*/
VOID CCautionNotifyGui::RegisterHandler() 
{
	m_mapNotifyHandler["DST_FREEPVP_OUTSIDE_STAGE"] = &CCautionNotifyGui::FreePvpOutSideHandler;
}

/**
* \brief StringID를 Continuance로 등록한다.
* \param uiStringID		(RwUInt32) 문자열 인덱스
* \param fLimitTime		(RwReal) 제한시간
* \param fIntervalTime	(RwReal) 간격
*/
VOID CCautionNotifyGui::AddContinuance(std::string& strStringID, RwReal fLimitTime, RwReal fIntervalTime )
{
	CCautionNotifyContinuance* pContinuance = NTL_NEW CCautionNotifyContinuance( this, strStringID, fLimitTime, fIntervalTime );
	m_listContinuance.push_back( pContinuance );
}

/**
* \brief Continuance List에 등록되어 있는 동일한 StringID를 가진 Continuance를 삭제한다.
* \param uiStringID		(RwUInt32) 문자열 인덱스
*/
VOID CCautionNotifyGui::DeleteContinuance(std::string& strStringID)
{	
	CONTINUANCELIST::iterator it = m_listContinuance.begin();
	while( it != m_listContinuance.end() )
	{
		if( (*it)->GetStringID() == strStringID )
		{
			// 삭제
			CCautionNotifyContinuance *pContinuance = (*it);
			it = m_listContinuance.erase(it);
			NTL_DELETE( pContinuance );

			continue;
		}

		it++;
	}	
}