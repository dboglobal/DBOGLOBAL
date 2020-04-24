#include "precomp_dboclient.h"
#include "SideDialogManager.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEventGenerator.h"


#define dVELOCITY_MOVE			1500.f			///< Side dialog가 open/close 되는 속도

CSideDialogManager* CSideDialogManager::m_pInstance = NULL;


CSideDialogManager::CSideDialogManager(const RwChar* pName)
:CNtlPLGui(pName)
{
}

CSideDialogManager::~CSideDialogManager()
{

}

CSideDialogManager* CSideDialogManager::GetInstance()
{
	NTL_ASSERT(m_pInstance, "CSideDialogManager::GetInstance, Not exist instance");
	return m_pInstance;
}

VOID CSideDialogManager::CreateInstance()
{
	NTL_FUNCTION("CSideDialogManager::CreateInstance");

	m_pInstance = NTL_NEW CSideDialogManager("SideDialogManager");
	NTL_ASSERT(m_pInstance, "CSideDialogManager::CreateInstance : Instance Allocate Fail" );

	m_pInstance->m_pThis = NTL_NEW gui::CDialog(CRectangle(0, 0, 0, 0), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager());
	NTL_ASSERT(m_pInstance->m_pThis, "CSideDialogManager::CreateInstance : dialog Allocate Fail" );

	m_pInstance->m_slotMove = m_pInstance->m_pThis->SigMove().Connect(m_pInstance, &CSideDialogManager::OnMove );

	m_pInstance->RegisterAttribute();

	m_pInstance->Show(false);

	m_pInstance->LinkMsg(g_EventSideDialog);

	NTL_RETURNVOID();
}

VOID CSideDialogManager::DestoryInstance()
{
	NTL_FUNCTION("CSideDialogManager::Destroy");

	MAP_SDIALOG_ITER it = m_pInstance->m_mapDialog.begin();
	for( ; it != m_pInstance->m_mapDialog.end() ; ++it )
	{
		NTL_DELETE(it->second.pCallSwitch);
	}
	m_pInstance->m_mapDialog.clear();

	m_pInstance->UnLinkMsg(g_EventSideDialog);

	NTL_DELETE(m_pInstance->m_pThis);
	NTL_DELETE(m_pInstance);	

	NTL_RETURNVOID();
}

VOID CSideDialogManager::Update(RwReal fElapsed)
{
}

VOID CSideDialogManager::UnRegistDialog(int iSideDialog)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iSideDialog);
	if(!pDialogInfo)
		return;


	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.find(iSideDialog);
	if( it_presentation != m_mapPresentDialog.end() )
		m_mapPresentDialog.erase(it_presentation);

	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_CONTROLLER_DEL_BUTTON, PLACE_NONE, PLACE_NONE, iSideDialog);

	pDialogInfo->pDialog->Show(false);

	NTL_DELETE(pDialogInfo->pCallSwitch);
	m_mapDialog.erase(iSideDialog);
}

RwInt32 CSideDialogManager::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

RwBool CSideDialogManager::SwitchDialog(int iDialog)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if(pDialogInfo)
	{
		if( pDialogInfo->pDialog->IsShow() )
			return CloseDialog(iDialog);
		else
			return OpenDialog(iDialog);
	}

	return FALSE;
}

VOID CSideDialogManager::OpenDefaultDialog()
{
	if( GetDialogManager()->GetMode() == DIALOGMODE_NARRATION )
		return;
	
	for( RwUInt32 i = SDIALOG_DEFAULT_FIRST ; i <= SDIALOG_DEFAULT_LAST ; ++i )
		OpenDialog(i, FALSE);
}

RwBool CSideDialogManager::OpenDialog(RwInt32 iSideDialog, RwBool bPlaySound /* = TRUE */)
{
	// 퀘스트 나레이션이 흐르면 리턴
	if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		return FALSE;

	DialogInfo* pDialogInfo = GetDialogInfo(iSideDialog);
	if( !pDialogInfo )
		return FALSE;

	MAP_PRESENTATION_ITER it_presentation_find = m_mapPresentDialog.begin();
	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.end();
	MAP_PRESENTATION_ITER it_presentation_Open = m_mapPresentDialog.end();
	for( ; it_presentation_find != m_mapPresentDialog.end() ; ++it_presentation_find )
	{		
		if( it_presentation_find->second.eSDialog == (eSideDialogType)iSideDialog )
		{
			// 현재 요청한 사이드 다이얼로그가 활성화된 목록에 있는지 찾아본다
			it_presentation = it_presentation_find;
		}
		else
		{
			// 기존에 OPEN 상태인 사이드 다이얼로그를 찾는다
			if( it_presentation_find->second.openType == OPENTYPE_OPEN ||
				it_presentation_find->second.openType == OPENTYPE_MINIMAM )
			{
				// 다른 사이드 다이얼로그와 겹칠 수 없는 것이라면..
				if( IsAttribute(it_presentation_find->second.eSDialog, dSDA_INDEPENDENTLY) == FALSE )
					it_presentation_Open = it_presentation_find;
			}
		}

		if( it_presentation_Open != m_mapPresentDialog.end() &&
			it_presentation != m_mapPresentDialog.end() )
			break;
	}

	// 기존 활성화된 목록에 있는 것이라면...
	if( it_presentation != m_mapPresentDialog.end() )
	{
		sPresentationDialog& rPresentaionDialog = it_presentation->second;

		switch( rPresentaionDialog.openType )
		{
		case OPENTYPE_OPEN:
			{
				return FALSE;
			}			
		case OPENTYPE_MINIMAM:
		case OPENTYPE_CONCEAL:
			{
				rPresentaionDialog.openType = OPENTYPE_OPEN;
				rPresentaionDialog.pDialogInfo->pDialog->Show(true);

				// 기존에 Open 상태인 것을 감춘다
				if( it_presentation_Open != m_mapPresentDialog.end() )
				{
					it_presentation_Open->second.openType = OPENTYPE_CONCEAL;
					it_presentation_Open->second.pDialogInfo->pDialog->Show(false);
				}

				CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_INACTIVE, PLACE_NONE, PLACE_NONE);
				return TRUE;
			}
		}
	}

	// 새로이 열리는 각 클래스의 SwitcDialog()를 호출한다.
	if( pDialogInfo->pCallSwitch->Call(true) < 0 )
		return FALSE;

	// current presentation PLGui
	sPresentationDialog newPresentaionDialog;

	newPresentaionDialog.eSDialog			= (eSideDialogType)iSideDialog;
	newPresentaionDialog.pDialogInfo		= pDialogInfo;
	newPresentaionDialog.openType			= OPENTYPE_OPEN;

	// 다른 사이드 다이얼로그와 동시에 뜰 수 없는 것이라면
	if( IsAttribute(iSideDialog, dSDA_INDEPENDENTLY) == FALSE )
	{
		// 기존에 Open 상태인 것을 감춘다
		if( it_presentation_Open != m_mapPresentDialog.end() )
		{
			it_presentation_Open->second.openType = OPENTYPE_CONCEAL;
			it_presentation_Open->second.pDialogInfo->pDialog->Show(false);
		}
	}

	// 사이드 다이얼로그의 좌표 설정
	RwInt32 iXPos = GetDboGlobal()->GetScreenWidth() - newPresentaionDialog.pDialogInfo->pDialog->GetWidth() - dDIALOG_CLEINT_EDGE_GAP;
	RwInt32 iYPos;
	if( IsAttribute(iSideDialog, dSDA_FREE_Y_POSITION) )
	{
		iYPos = newPresentaionDialog.pDialogInfo->pDialog->GetHeight();
	}
	else
	{
		CRectangle rtSideDialogManager = m_pThis->GetPosition();
		iYPos = rtSideDialogManager.bottom;
	}
	newPresentaionDialog.pDialogInfo->pDialog->SetPosition(iXPos, iYPos);

	m_mapPresentDialog[iSideDialog]			= newPresentaionDialog;

	if( bPlaySound )
		PlayOpenSound(iSideDialog);

	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_CONTROLLER_ADD_BUTTON, PLACE_NONE, PLACE_NONE, iSideDialog);

	return TRUE;
}

RwBool CSideDialogManager::CloseDialog(RwInt32 iSideDialog, RwBool bPlaySound /* = TRUE */)
{
	// 이미 닫혀 있다면 리턴
	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.find(iSideDialog);
	if( it_presentation == m_mapPresentDialog.end() )
		return FALSE;

	DialogInfo* pDialogInfo = GetDialogInfo(iSideDialog);
	if(!pDialogInfo)
		return FALSE;

	// 각 클래스의 SwithDialog()를 호출한다. 이 함수내에서 Show(false)를 호출해서는 안된다
	// Show(false)는 CSideDialogManager::Update()에서 Close effect가 마무리 되면 자동 호출된다
	if( pDialogInfo->pCallSwitch->Call(false) < 0 )
		return FALSE;

	m_mapPresentDialog.erase(it_presentation);

	if(bPlaySound)
		PlayCloseSound(iSideDialog);

	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_CONTROLLER_DEL_BUTTON, PLACE_NONE, PLACE_NONE, iSideDialog);

	return TRUE;
}

VOID CSideDialogManager::CloseAll()
{
	for(RwInt32 i = SDIALOG_FIRST ; i <= SDIALOG_LAST ; ++i )
		CloseDialog(i, FALSE);
}

CSideDialogManager::DialogInfo* CSideDialogManager::GetDialogInfo(int iSideDialog)
{	
	MAP_SDIALOG_ITER it = m_mapDialog.find(iSideDialog);
	if( it != m_mapDialog.end() )
		return &( it->second );

	return NULL;
}

CNtlPLGui* CSideDialogManager::GetDialog(int iSideDialog)
{	
	MAP_SDIALOG_ITER it = m_mapDialog.find(iSideDialog);
	if( it != m_mapDialog.end() )
		return it->second.pDialog ;

	return NULL;
}

RwBool CSideDialogManager::IsOpenDialog(int iSideDialog)
{
	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.find(iSideDialog);
	if( it_presentation == m_mapPresentDialog.end() )
		return FALSE;

	if( it_presentation->second.openType == OPENTYPE_OPEN )
		return TRUE;

	return FALSE;
}

RwBool CSideDialogManager::IsConcealDialg(int iSideDialog)
{
	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.find(iSideDialog);
	if( it_presentation == m_mapPresentDialog.end() )
		return FALSE;

	if( it_presentation->second.openType == OPENTYPE_CONCEAL )
		return TRUE;

	return FALSE;
}

RwBool CSideDialogManager::IsDefaultDialog(int iSideDialog)
{
	if( SDIALOG_DEFAULT_FIRST <= iSideDialog && iSideDialog <= SDIALOG_DEFAULT_LAST )
		return TRUE;

	return FALSE;
}

RwBool CSideDialogManager::IsEventDialog(int iSideDialog)
{
	if( SDIALOG_EVENT_FIRST <= iSideDialog && iSideDialog <= SDIALOG_EVENT_LAST )
		return TRUE;

	return FALSE;
}

RwBool CSideDialogManager::IsNormalDialog(int iSideDialog)
{
	if( SDIALOG_NORMAL_FIRST <= iSideDialog && iSideDialog <= SDIALOG_NORMAL_LAST )
		return TRUE;

	return FALSE;
}

RwBool CSideDialogManager::IsAttribute(RwInt32 iSideDialog, RwUInt32 uiFlag)
{
	MAP_ATTRIBUTE_ITER it_Attribute = m_mapAttribute.find(iSideDialog);
	if( it_Attribute == m_mapAttribute.end() )
		return FALSE;

	if( BIT_FLAG_TEST(it_Attribute->second, uiFlag) )
		return TRUE;

	return FALSE;
}

VOID CSideDialogManager::PlayOpenSound(RwInt32 iSideDialog)
{
	Logic_PlayGUISound(GSD_SYSTEM_WINDOW_OPEN);
}

VOID CSideDialogManager::PlayCloseSound(RwInt32 iSideDialog)
{
	Logic_PlayGUISound(GSD_SYSTEM_WINDOW_OPEN);
}

VOID CSideDialogManager::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.begin();
	for( ; it_presentation != m_mapPresentDialog.end() ; ++it_presentation )
	{
		sPresentationDialog& rPresentaionDialog = it_presentation->second;
		CRectangle rtRect = rPresentaionDialog.pDialogInfo->pDialog->GetPosition();

		if( IsAttribute(rPresentaionDialog.eSDialog, dSDA_FREE_Y_POSITION) )
		{
			rPresentaionDialog.pDialogInfo->pDialog->SetPosition(rtScreen.right - rtRect.GetWidth(), rtRect.top);
		}
		else
		{
			rPresentaionDialog.pDialogInfo->pDialog->SetPosition(rtScreen.right - rtRect.GetWidth(), rtScreen.bottom);
		}
		
	}
}

VOID CSideDialogManager::RegisterAttribute()
{
	for( RwInt32 i = SDIALOG_FIRST ; i <= SDIALOG_LAST ; ++i )
		m_mapAttribute[i] = dSDA_NONE;

	// 특별한 경우의 사이드 다이얼로그의 속성을 정의한다
}

VOID CSideDialogManager::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CSideDialogManager::HandleEvents");

	if( msg.Id == g_EventSideDialog )
	{
		SDBoEventSideDialog* pEvent = reinterpret_cast<SDBoEventSideDialog*>( msg.pData );

		if( pEvent->byMessage == SDIALOG_MESSAGE_MINIMIZE )
		{
			MAP_PRESENTATION_ITER it_presentation = m_mapPresentDialog.begin();
			for( ; it_presentation != m_mapPresentDialog.end() ; ++it_presentation )
			{
				sPresentationDialog& rPresentaionDialog = it_presentation->second;
				if( rPresentaionDialog.eSDialog == (eSideDialogType)pEvent->uiSrcSDialog )
				{
					rPresentaionDialog.openType = OPENTYPE_MINIMAM;
					rPresentaionDialog.pDialogInfo->pDialog->Show(false);
					break;
				}
			}
		}
	}

	NTL_RETURNVOID();
}
///////////////////////////////////////////////////////////// Test Func
/// woosungs_test
CNtlPLGui* const CSideDialogManager::GetpDialogTEST(const char* szFrmFileName)
{
	CNtlPLGui* pDialog;
	MAP_SDIALOG::iterator it = m_mapDialog.begin();	/// 등록된 dialog 검색
	for( ; it != m_mapDialog.end(); ++it)
	{
		pDialog = it->second.pDialog;
		if(pDialog)
		{								// ex) "gui\PcStatus.frm"
			if(!strncmp(szFrmFileName, pDialog->GetCompPageName(), 150))
				return pDialog;
		}
	}

	return NULL;
}

RwBool CSideDialogManager::ShowDialogTEST(RwInt32 iDialog, bool bOpen)
{
	/// OpenDialog 아닌 단순 Show로.....
	CNtlPLGui* pDialog;
	MAP_SDIALOG::iterator it = m_mapDialog.find(iDialog);	/// 등록된 dialog 검색
	if( it != m_mapDialog.end() )
	{
		pDialog = it->second.pDialog;
		if(pDialog)
		{
			pDialog->Show(bOpen);
			return TRUE;
		}
	}

	return FALSE;
}

VOID CSideDialogManager::ShowAllDialogTEST(bool bOpen)	///< SideDialogManager에 등록된 모든 dialog open/close
{
	CNtlPLGui* pDialog;
	for(RwInt32 i = SDIALOG_FIRST + 1 ; i < SDIALOG_UNKNOWN ; ++i )
	{	/// OpenDialog 아닌 단순 Show로.....
		MAP_SDIALOG::iterator it = m_mapDialog.find(i);	/// 등록된 dialog 검색
		if( it != m_mapDialog.end() )
		{
			pDialog = it->second.pDialog;
			if(pDialog)
			{
				pDialog->Show(bOpen);
			}
		}
	}
}