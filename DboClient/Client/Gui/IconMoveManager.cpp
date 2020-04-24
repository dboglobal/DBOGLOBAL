#include "precomp_dboclient.h"
#include "IconMoveManager.h"

// Presentation
#include "NtlPLDef.h"

// Simulation
#include "NtlSobItem.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"
#include "NtlPacketLockManager.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlSkillContainer.h"

// Gui
#include "gui_texture.h"
#include "mouse.h"

// Dbo
#include "DboEventGenerator.h"
#include "PopupManager.h"
#include "InfoWndManager.h"
#include "DboLogic.h"
#include "GUISoundDefine.h"


CIconMoveManager* CIconMoveManager::m_pInstance = NULL;

CIconMoveManager::CIconMoveManager(VOID)
{
	m_bActive = FALSE;
	m_hSrcSerial = INVALID_SERIAL_ID;
	m_eSrcPlace = PLACE_NONE;
	m_uiSrcSlotIdx = 0;
	m_uiStackCount = 0;
	m_surIcon.SetSize( ICONPICKUP_SIZE, ICONPICKUP_SIZE ); 
	m_surIcon.GetSurface()->m_SnapShot.UVs[0] = 0.01f;
	m_surIcon.GetSurface()->m_SnapShot.UVs[1] = 0.01f;
	m_surIcon.GetSurface()->m_SnapShot.UVs[2] = 0.99f;
	m_surIcon.GetSurface()->m_SnapShot.UVs[3] = 0.99f;
	m_ExData1 = 0;
	m_ExData2 = 0;
	m_ExData3 = 0;
	m_ExData4 = 0;
	m_ExData5 = 0;
}

CIconMoveManager::~CIconMoveManager(VOID)
{
}

VOID CIconMoveManager::CreateInstance( gui::CGUIManager* pGuiManager )
{
	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CIconMoveManager;
		NTL_ASSERT( m_pInstance, "CIconMoveManager::CreateInstance : Instance Allocate Fail" );

		m_pInstance->m_slotCaptureEndMouseDown = pGuiManager->SigCaptureEndMouseDown().Connect( m_pInstance, &CIconMoveManager::OnCaptureEndMouseDown );

		m_pInstance->LinkMsg( g_EventSkillInit, NULL, 0x9000 );
		m_pInstance->LinkMsg( g_EventSobItemDelete );
		m_pInstance->LinkMsg( g_EventGameServerChangeOut );
	}
}

VOID CIconMoveManager::DeleteInstance(VOID)
{	
	if( m_pInstance )
	{
		m_pInstance->UnLinkMsg( g_EventSkillInit );
		m_pInstance->UnLinkMsg( g_EventSobItemDelete );
		m_pInstance->UnLinkMsg( g_EventGameServerChangeOut );

		NTL_DELETE( m_pInstance );
	}
}

CIconMoveManager* CIconMoveManager::GetInstance(VOID)
{
	return m_pInstance;
}

RwBool CIconMoveManager::IsEnable(VOID)
{
	if( API_GetSLPacketLockManager()->IsLockType( CNtlPacketLockManager::LT_ITEM | CNtlPacketLockManager::LT_ZENNY ) )
		return FALSE;

	return TRUE;
}

RwBool CIconMoveManager::IsActiveZenny(VOID)
{
	if( IsActive() )
	{
		if( m_eSrcPlace >= PLACE_ZENNY_FIRST && m_eSrcPlace <= PLACE_ZENNY_LAST )
			return TRUE;
	}

	return FALSE;
}

RwBool CIconMoveManager::IconMovePickUp( RwUInt32 hSrcSerial, RwUInt32 eSrcPlace, RwUInt32 uiSrcSlotIdx, RwUInt32 uiStackCount, VOID* pTexture, 
										 RwUInt32 EXData1 /* = 0 */, RwUInt32 EXData2 /* = 0 */, RwUInt32 EXData3 /* = 0 */, RwUInt32 EXData4 /* = 0 */, RwUInt32 EXData5 /* = 0  */)
{
	if( !IsEnable() )
		return FALSE;

	if( m_bActive )
		IconMoveEnd();

	GetInfoWndManager()->ShowInfoWindow( FALSE );
	GetPopupManager()->AllPopupClose();

	m_bActive = TRUE;
	m_hSrcSerial = hSrcSerial;
	m_eSrcPlace = eSrcPlace;
	m_uiSrcSlotIdx = uiSrcSlotIdx;
	m_uiStackCount = uiStackCount;
	m_surIcon.SetTexture( reinterpret_cast<gui::CTexture*>( pTexture ) );
	m_ExData1 = EXData1;
	m_ExData2 = EXData2;
	m_ExData3 = EXData3;
	m_ExData4 = EXData4;
	m_ExData5 = EXData5;		

	CDboEventGenerator::IconMoveClick( TRUE );

	Logic_PlayGUISound(GSD_SYSTEM_ICON_SELECT);

	return TRUE;
}

RwBool CIconMoveManager::IconMovePutDown( RwUInt32 eDestPlace, RwUInt32 hDestParentSerial, RwUInt32 uiDestSlotIdx)
{
	if( !IsEnable() )
		return FALSE;

	GetInfoWndManager()->ShowInfoWindow( FALSE );
	return Logic_ItemMoveProc( m_hSrcSerial, (EPlace)m_eSrcPlace, (RwUInt8)m_uiSrcSlotIdx, (EPlace)eDestPlace, hDestParentSerial, (RwUInt8)uiDestSlotIdx, m_uiStackCount);
}

VOID CIconMoveManager::IconMoveEnd(VOID)
{
	m_bActive = FALSE;

	CDboEventGenerator::IconMoveClick( FALSE );
	CDboEventGenerator::PickedUpHide( m_eSrcPlace );
	
	m_surIcon.UnsetTexture();
	m_hSrcSerial = INVALID_SERIAL_ID;
	m_eSrcPlace = PLACE_NONE;
	m_uiSrcSlotIdx = 0;
	m_uiStackCount = 0;	
	m_ExData1 = 0;
	m_ExData2 = 0;
	m_ExData3 = 0;
	m_ExData4 = 0;
	m_ExData5 = 0;			
}

VOID CIconMoveManager::Render(VOID)
{
	if( m_bActive )
	{
		m_surIcon.SetCenterPosition(CMouse::GetX(), CMouse::GetY());
		m_surIcon.Render( true );
	}
}

VOID CIconMoveManager::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobItemDelete )
	{
		SNtlEventSobItemDelete* pData = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		if( m_bActive && m_hSrcSerial == pData->hItemSerialId )
		{
			IconMoveEnd();
		}
	}
	else if( msg.Id == g_EventSkillInit )
	{
		if( GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetSkillSlotIdx( m_hSrcSerial ) != -1 &&
			m_bActive )
		{
			IconMoveEnd();
		}			
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		if( m_bActive )
		{
			IconMoveEnd();
		}
	}
}

VOID CIconMoveManager::OnCaptureEndMouseDown( const CKey& key )
{
	if( m_bActive )
	{
		if( key.m_nID == UD_RIGHT_BUTTON )
		{
			IconMoveEnd();
		}
	}
}