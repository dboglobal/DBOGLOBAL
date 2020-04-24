#include "precomp_dboclient.h"
#include "PopupManager.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLEvent.h"

// Dbo
#include "PopupMenuGui.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "InfoWndManager.h"

CPopupManager* CPopupManager::m_pInstance = NULL;


CPopupManager::CPopupManager(VOID)
{
	memset( m_apPopup, 0, sizeof( m_apPopup ) );
	m_eShowedPopupPlace = PLACE_NONE;	
}

CPopupManager::~CPopupManager(VOID)
{
}

RwBool CPopupManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CPopupManager::CreateInstance" );

	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CPopupManager;
		NTL_ASSERT( m_pInstance, "CPopupManager::CreateInstance : Instance Allocate Fail" );

		m_pInstance->m_apPopup[ICON_POPUP] = NTL_NEW CIconPopupGui( "PopupMenu" );
		NTL_ASSERT( m_pInstance->m_apPopup[ICON_POPUP], "CPopupManager::CreateInstance : Popup Allocate Fail" );
		if( !m_pInstance->m_apPopup[ICON_POPUP]->Create() )
		{
			m_pInstance->m_apPopup[ICON_POPUP]->Destroy();
			NTL_DELETE( m_pInstance->m_apPopup[ICON_POPUP] );
			NTL_RETURN( FALSE );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance->m_apPopup[ICON_POPUP] );

		m_pInstance->m_apPopup[CALC_POPUP] = NTL_NEW CCalcPopupGui( "CalcPopup" );
		NTL_ASSERT( m_pInstance->m_apPopup[CALC_POPUP], "CPopupManager::CreateInstance : Popup Allocate Fail" );
		if( !m_pInstance->m_apPopup[CALC_POPUP]->Create() )
		{
			m_pInstance->m_apPopup[CALC_POPUP]->Destroy();
			NTL_DELETE( m_pInstance->m_apPopup[CALC_POPUP] );
			NTL_RETURN( FALSE );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance->m_apPopup[CALC_POPUP] );

		m_pInstance->LinkEvent();
	}

	NTL_RETURN( TRUE );
}

VOID CPopupManager::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkEvent();

		if( m_pInstance->m_apPopup[ICON_POPUP] )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance->m_apPopup[ICON_POPUP] );
			m_pInstance->m_apPopup[ICON_POPUP]->Destroy();
			NTL_DELETE( m_pInstance->m_apPopup[ICON_POPUP] );
		}

		if( m_pInstance->m_apPopup[CALC_POPUP] )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance->m_apPopup[CALC_POPUP] );
			m_pInstance->m_apPopup[CALC_POPUP]->Destroy();
			NTL_DELETE( m_pInstance->m_apPopup[CALC_POPUP] );
		}

		NTL_DELETE( m_pInstance );
	}
}

CPopupManager* CPopupManager::GetInstance(VOID)
{
	return m_pInstance;
}

RwBool CPopupManager::IsVisiblePopupGui(VOID)
{
	for( RwInt16 i = 0 ; i < NUM_POPUPGUI ; ++i )
	{
		if( m_apPopup[i]->IsShow() )
			return TRUE;
	}

	return FALSE;
}

RwBool CPopupManager::IsVisiblePopupGui( RwInt32 ePlace )
{
	if( ePlace == m_eShowedPopupPlace )
	{
		for( RwInt32 i = 0 ; i < NUM_POPUPGUI ; ++i )
		{
			if( m_apPopup[i]->IsShow() )
				return TRUE;
		}
	}

	return FALSE;
}

VOID CPopupManager::AllPopupClose(VOID)
{
	for( RwInt16 i = 0 ; i < NUM_POPUPGUI ; ++i )
	{
		if( m_apPopup[i] )
		{
			m_apPopup[i]->Show( false );
		}
	}
}

VOID CPopupManager::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventIconPopupShow )
	{
		SDboEventIconPopupShow* pData = reinterpret_cast<SDboEventIconPopupShow*>( msg.pData );
		if( pData->bShow )
		{
			m_eShowedPopupPlace = pData->nSrcPlace;
			m_Pos.SetPos( pData->nXPos, pData->nYPos );
			GetInfoWndManager()->ShowInfoWindow( FALSE );

			m_apPopup[ICON_POPUP]->Show( true );
			m_apPopup[CALC_POPUP]->Show( false );

			m_apPopup[ICON_POPUP]->ShowEvent( msg );
		}
		else
		{
			if( pData->nSrcPlace == m_eShowedPopupPlace ||
				pData->nSrcPlace == PLACE_NONE )
			{
				m_apPopup[ICON_POPUP]->Show( false );
				m_eShowedPopupPlace = PLACE_NONE;
			}
		}
	}
	else if( msg.Id == g_EventCalcPopupShow )
	{
		SDboEventCalcPopupShow* pData = reinterpret_cast<SDboEventCalcPopupShow*>( msg.pData );
		if( pData->bShow )
		{
			m_eShowedPopupPlace = pData->nSrcPlace;
			m_Pos.SetPos( pData->nXPos, pData->nYPos );
			GetInfoWndManager()->ShowInfoWindow( FALSE );

			m_apPopup[ICON_POPUP]->Show( false );
			m_apPopup[CALC_POPUP]->Show( true );

			m_apPopup[CALC_POPUP]->ShowEvent( msg );
		}
		else
		{
			if( pData->nSrcPlace == m_eShowedPopupPlace ||
				pData->nSrcPlace == PLACE_NONE )
			{
				m_apPopup[CALC_POPUP]->Show( false );
				m_eShowedPopupPlace = PLACE_NONE;
			}
		}
	}
	else if( msg.Id == g_EventPartyShareTargetNfy )
	{
		if( m_apPopup[ICON_POPUP]->IsShow() &&
			m_eShowedPopupPlace == PLACE_SUB_TARGETUI )
		{
			SDboEventIconPopupShow Data;
			Data.bShow = TRUE;
			Data.nSrcPlace = PLACE_SUB_TARGETUI;
			Data.uiSerial = m_apPopup[ICON_POPUP]->GetSerial();
			Data.nXPos = m_Pos.x;
			Data.nYPos = m_Pos.y;
			Data.nSrcSlotIdx = -1;

			RWS::CMsg msgShow;
			msgShow.Id = g_EventIconPopupShow;
			msgShow.pData = &Data;

			m_apPopup[ICON_POPUP]->ShowEvent( msgShow );
		}
	}
}

VOID CPopupManager::LinkEvent(VOID)
{
	// Event Link
	LinkMsg( g_EventIconPopupShow );
	LinkMsg( g_EventCalcPopupShow );
	LinkMsg( g_EventPartyShareTargetNfy, 0, 0x6000 );
}

VOID CPopupManager::UnLinkEvent(VOID)
{
	// Event Unlink
	UnLinkMsg( g_EventIconPopupShow );
	UnLinkMsg( g_EventCalcPopupShow );
	UnLinkMsg( g_EventPartyShareTargetNfy );
}