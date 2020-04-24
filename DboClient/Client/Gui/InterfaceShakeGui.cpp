#include "precomp_dboclient.h"
#include "InterfaceShakeGui.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// gui
#include "gui_guimanager.h"
#include "mouse.h"

// simulation
#include "InputActionMap.h"
#include "NtlSLEvent.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Dbo
#include "DialogManager.h"
#include "DboGlobal.h"
 
/**
* \brief 생성자
*/
CInterfaceShakeGui::CInterfaceShakeGui( const RwChar* pName ) 
: CNtlPLGui(pName)
{
	m_nOldMouseX = 0;
	m_nMouseX = 0;
	m_nTrackValue = 0;
	m_nOffsetX = 0;
	m_nTrackMin = 0;
	m_nTrackMax = 0;
	m_dwLeftKey = 0;
	m_dwRightKey = 0;

	m_bMouseToggle = FALSE;
	m_bKeyToggle = FALSE;
	m_bComplete = FALSE;
	
	m_pPgbTrack = NULL;
	m_slotKeyDown = NULL;

	m_fElapsedTime = 0.0f;
	m_fShakeElapsedTime = 0.0f;
	m_fKeyDownElapsedTime = 0.0f;
}

/**
* \brief 소멸자
*/
CInterfaceShakeGui::~CInterfaceShakeGui() 
{

}

/**
* \brief Create
*/
RwBool CInterfaceShakeGui::Create() 
{
	NTL_FUNCTION( "CInterfaceShakeGui::Create" );

	if( !CNtlPLGui::Create("", dSHAKE_FILE_SURFACE , dSHAKE_FILE_FRAME ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pPgbTrack = (gui::CProgressBar*)GetComponent( "pgbTrack" );
	m_pPanTrack = (gui::CPanel*)GetComponent( "pnlTrackBack" );
	m_pPgbTrack->GetRange( m_nTrackMin, m_nTrackMax );

	// Priority
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );

	//m_pFlashLeftArrow = (gui::CFlash*)GetComponent( "flaLeftKey" );
	//m_pFlashRightArrow = (gui::CFlash*)GetComponent( "flaRightKey" );
	m_pFlashMouseBack = (gui::CFlash*)GetComponent( "flaMouseBack" );

	//m_pBtnLeftKey = (gui::CButton*)GetComponent( "btnLeftKey" );
	//m_pBtnRightKey = (gui::CButton*)GetComponent( "btnRightKey" );
	m_pStbMouse = (gui::CStaticBox*)GetComponent( "stbMouse" );

	Show( false );

	// Link
	LinkMsg( g_EventKnockDownNfy, 0 );
	LinkMsg( g_EventKnockDownWakeUpNfy, 0 );

	NTL_RETURN( TRUE );
}

/**
* \brief Destroy
*/
VOID CInterfaceShakeGui::Destroy() 
{
	NTL_FUNCTION( "CInterfaceShakeGui::Destroy" );

	// Unlink
	UnLinkMsg( g_EventKnockDownWakeUpNfy );
	UnLinkMsg( g_EventKnockDownNfy );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

/**
* \brief HandleEvents
*/
VOID CInterfaceShakeGui::HandleEvents( RWS::CMsg &msg ) 
{
	NTL_FUNCTION("CInterfaceShakeGui::HandleEvents");
	
	// 넉다운이 되었다는 Notify
	if( msg.Id == g_EventKnockDownNfy )
	{
		// 인터페이스를 띄운다.
		GetDialogManager()->OpenDialog( DIALOG_INTERFACE_SHAKE );
	}
	// 넉다운이 풀렸다는 Notify ( 인터페이스 UI가 열려 있는 상황에서만 받아서 처리 )
	else if ( msg.Id == g_EventKnockDownWakeUpNfy 
		&& GetDialogManager()->IsOpenDialog( DIALOG_INTERFACE_SHAKE ) )
	{
		// 인터페이스를 닫는다.
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}

	NTL_RETURNVOID();
}

/**
* \brief SwitchDialog
* DialogManager에서 OpenDialog나 CloseDialog를 해줬을 경우 실행 된다.
* \param bOpen Open = TRUE / Close = FALSE
* \return 성공여부
*/
RwInt32 CInterfaceShakeGui::SwitchDialog( bool bOpen ) 
{
	if( bOpen == TRUE )
		ShowInterface();
	else if( bOpen == FALSE )
		CloseInterface();	

	return 1;
}

/**
* \brief Update
* \param fElapsed 경과시간
*/
VOID CInterfaceShakeGui::Update( RwReal fElapsed ) 
{
	// 시간 증가
	m_fElapsedTime += fElapsed;
	m_fShakeElapsedTime += fElapsed;
	m_fKeyDownElapsedTime += fElapsed;
	m_fElapsedShakeTime += fElapsed;

	// 플래쉬 업데이트
	m_pFlashMouseBack->Update( fElapsed );

	// 마우스 위치 갱신
	m_nMouseX = CMouse::GetX();					// 현재 마우스
	m_nOffsetX = (m_nMouseX - m_nOldMouseX);		// 이전 마우스에서 증가값 구하기
	m_nOldMouseX = m_nMouseX;					// 이전 마우스는 현재의 마우스가 된다.

	// True 우측, False 좌측
	if( m_bMouseToggle )
	{
		if( m_nOffsetX > dSHAKE_MOVE_DISTANCE )
		{
			ProgressUpdateFromMouse();
			m_bMouseToggle = FALSE;
			m_pStbMouse->SetPosition( dSHAKE_MOUSE_LEFT_X, dSHAKE_MOUSE_Y );
		}
	}
	else
	{
		if( m_nOffsetX < -dSHAKE_MOVE_DISTANCE )
		{
			ProgressUpdateFromMouse();
			m_bMouseToggle = TRUE;
			m_pStbMouse->SetPosition( dSHAKE_MOUSE_RIGHT_X, dSHAKE_MOUSE_Y );
		}
	}

	// ProgressBar Set
	m_pPgbTrack->SetPos( m_nTrackValue );

	// Todo : Time Over
	if( m_fElapsedTime > dSHAKE_MAX_WAKEUP )
	{
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}

	// ProgressBar의 Max에 다다르면 성공
	if( m_nTrackValue >= m_nTrackMax && m_bComplete == FALSE )
		m_bComplete = TRUE;

	// 성공 하면 패킷을 날려주고 Dialog를 닫아야 한다.
	if( m_bComplete )
	{
		// 성공했더라도 최소 시간이 안되면 기다린다.
		if( m_fElapsedTime < dSHAKE_MIN_WAKEUP )
			return;
		
		// KnockDown의 회복 패킷
		GetDboGlobal()->GetGamePacketGenerator()->SendCharKnockDownReleaseNfy();
		
		// Dialog를 닫아준다.
		GetDialogManager()->CloseDialog( DIALOG_INTERFACE_SHAKE );
	}
}

/**
* \brief 마우스로 인한 ProgressBar 증가
*/
VOID CInterfaceShakeGui::ProgressUpdateFromMouse() 
{
	// 이득
	if( m_fShakeElapsedTime < dSHAKE_MOUSE_FIRSTTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_FIRST;
	}
	else if( m_fShakeElapsedTime < dSHAKE_MOUSE_SECONDTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_SECOND;
	}
	else
		m_nTrackValue += dSHAKE_ADVANTAGE_THIRD;

	// 마우스 흔들기 경과시간 초기화 & 마우스 Offset 초기화
	m_fShakeElapsedTime = 0.0f;
	m_nOffsetX = 0;

	// 제한
	if( m_nTrackValue > m_nTrackMax )
		m_nTrackValue = m_nTrackMax;
}

/**
* \brief 키보드로 인한 ProgressBar 증가
*/
VOID CInterfaceShakeGui::ProgressUpdateFromKeyboard() 
{
	// 이득
	if( m_fKeyDownElapsedTime < dSHAKE_KEYBOARD_FIRSTTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_FIRST;
	}
	else if ( m_fKeyDownElapsedTime < dSHAKE_KEYBOARD_SECONDTIME )
	{
		m_nTrackValue += dSHAKE_ADVANTAGE_SECOND;
	}
	else
		m_nTrackValue += dSHAKE_ADVANTAGE_THIRD;

	// KeyDown 경과시간 초기화
	m_fKeyDownElapsedTime = 0.0f;

	// Track 값 제한
	if( m_nTrackValue > m_nTrackMax )
		m_nTrackValue = m_nTrackMax;
}

/**
* \brief UI를 열고 필요한 정보를 초기화한다.
*/
VOID CInterfaceShakeGui::ShowInterface( VOID ) 
{
	m_nMouseX = CMouse::GetX();
	m_nOldMouseX = m_nMouseX ;
	m_nTrackValue = 0;
	m_nOffsetX = 0;
	m_bMouseToggle = FALSE;
	m_bKeyToggle = FALSE;
	m_bComplete = FALSE;
	m_fElapsedTime = 0.0f;
	m_fShakeElapsedTime = 0.0f;
	m_fKeyDownElapsedTime = 0.0f;
	m_dwLeftKey = GetInputActionMap()->FindKey( ACTION_AVATAR_LEFTTURN ) & 0xFF;
	m_dwRightKey = GetInputActionMap()->FindKey( ACTION_AVATAR_RIGHTTURN ) & 0xFF;
	
	GetNtlGuiManager()->AddUpdateFunc( this );

	m_pFlashMouseBack->RestartMovie();

	Show( TRUE );
}

/**
* \brief UI를 닫아주며 해제해줘야 하는 일들을 수행한다.
*/
VOID CInterfaceShakeGui::CloseInterface( VOID ) 
{
	m_slotKeyDown = 0;
	m_slotKeyUp = 0;
	
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	Show( FALSE );
}

/**
* \brief OnKeyDown
* 콜백으로 다이얼로그 매니저의 SigCaptureKeyDown에 연결된 함수
*/
VOID CInterfaceShakeGui::OnKeyDown( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key ) 
{
	if( key.m_dwVKey == m_dwLeftKey )
	{
		m_pBtnLeftKey->Enable( FALSE );
		if( m_bKeyToggle )
		{
			ProgressUpdateFromKeyboard();
			m_bKeyToggle = FALSE;
		}
	}
	else if( key.m_dwVKey == m_dwRightKey )
	{
		m_pBtnRightKey->Enable( FALSE );
		if( !m_bKeyToggle )
		{
			ProgressUpdateFromKeyboard();
			m_bKeyToggle = TRUE;
		}
	}
}

/**
* \brief OnKeyUp
* 콜백으로 다이얼로그 매니저의 SigCaptureKeyUp에 연결된 함수
*/
VOID CInterfaceShakeGui::OnKeyUp( gui::CComponent* pComponent, CInputDevice* pDevice, const CKey& key ) 
{
	if( key.m_dwVKey == m_dwLeftKey )
		m_pBtnLeftKey->Enable( TRUE );
	else if( key.m_dwVKey == m_dwRightKey )
		m_pBtnRightKey->Enable( TRUE );
}

