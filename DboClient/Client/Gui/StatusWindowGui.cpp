#include "precomp_dboclient.h"

// shared
#include "NtlItem.h"
#include "NtlResultCode.h"
#include "MobTable.h"

// core
#include "ceventhandler.h"
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "gui_renderer.h"
#include "NtlPLCharacter.h"
#include "NtlPLEvent.h"

// Table
#include "ItemTable.h"
#include "ItemMixExpTable.h"
#include "CharTitleTable.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLDef.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobIcon.h"
#include "NtlSLEvent.h"
#include "NtlSobCharProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// DBO
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "SurfaceGui.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "ChatGui.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "GuiFunctor.h"

// Self
#include "StatusWindowGui.h"

////////////////////////////////////////////////////////////////////////////////
// ! Define
#define MAX_STACK_NUM_WIDTH		20
#define MAX_STACK_NUM_HEIGHT	20

#define YELLOW_TEXT_COLOR	RGB( 0xFF, 0xD1, 0x44 )
#define BLUE_TEXT_COLOR		RGB( 0x9B, 0xE2, 0xFF )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! AvatarTab
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ! Construction & Destruction

CStatusAvatarTab::CStatusAvatarTab(VOID)
{
	m_pSelf = NULL;

	m_nShowPickedUp = -1;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIndex = -1;
	m_nPushDownIndex = -1;	
	m_nDragRotX = 0;

	memset( m_arrFocusEffect, 0, sizeof( m_arrFocusEffect ) );
	
	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_INVALID;
	m_sBattleAttr.bySourceWeaponAttr = INVALID_BYTE;
	m_sBattleAttr.byTargetWeaponAttr = INVALID_BYTE;
}


CStatusAvatarTab::~CStatusAvatarTab(VOID)
{

}

////////////////////////////////////////////////////////////////////////////////
// ! Operation
RwBool CStatusAvatarTab::IsShow(VOID)
{
	if( !m_pSelf->IsVisible() )
		return FALSE;

	if( !m_pSelf->GetParent()->IsVisible() )
		return FALSE;

	return TRUE;
}


RwBool CStatusAvatarTab::Create( CNtlPLGui* pParent )
{
	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgAvatar" ) );

	m_pstbBattleAttrTitle = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "sttAttrTitle" ) );

	for( RwInt32 i = 0 ; i < NUM_BASIC_STAT ; ++i )
	{
		const char* szFilename[] = { "sttSTR", "sttDEX", "sttCON", "sttENG", "sttSOL", "sttFOC" };
		char buf[256]; 

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );
		
		m_pstbBasicStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbBasicStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}

	for( RwInt32 i = 0 ; i < NUM_PCINFO ; ++i )
	{
		const char* szFilename[] = { "sttName", "sttClass", "sttLevel", "sttAP" };
		char buf[256]; 

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );

		m_pstbPCInfo[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbPCInfoTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}

	for( RwInt32 i = 0 ; i < NUM_STAT ; ++i )
	{
		const char* szFilename[] = { "sttLP", "sttEP", "sttEXP_Cur", "sttEXP_Max" };
		
		m_pstbStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
	}

	for( RwInt32 i = 0 ; i < NUM_COMBAT_STAT ; ++i ) 
	{
		const char* szFilename[] = { "stt%sAttack", "stt%sDefence", "stt%sCritical" };
		char buf[256]; 
		char buftitle[256];

		sprintf_s( buf, 256, szFilename[i], "Physical" );
		sprintf_s( buftitle, 256, "%s%s", buf, "Title" );

		m_pstbPhysicalCombatStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
		m_pstbPhysicalCombatStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buftitle ) );

		sprintf_s( buf, 256, szFilename[i], "Energy" );
		sprintf_s( buftitle, 256, "%s%s", buf, "Title" );

		m_pstbEnergyCombatStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
		m_pstbEnergyCombatStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buftitle ) );
	}

	for( RwInt32 i = 0 ; i < NUM_ETC_STAT ; ++i )
	{
		const char* szFilename[] = { "sttAttackRate", "sttDodgeRate" };
		char buf[256];

		sprintf_s( buf, 256, "%s%s", szFilename[i], "Title" );

		m_pstbETCStat[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( szFilename[i] ) );
		m_pstbETCStatTitle[i] = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( buf ) );
	}


	m_pstbPCCharacterTitle = reinterpret_cast<gui::CStaticBox*>(pParent->GetComponent("sttCharTitle"));
	m_pstbPCActiveCharacterTitle = reinterpret_cast<gui::CStaticBox*>(pParent->GetComponent("sttCharActiveTitle"));

	m_pbtnLeftRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnLeftSpin" ) );
	m_pbtnRightRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnRightSpin" ) );
	m_pbtnDragRot = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnCharDrag" ) );

	m_btnCharTitle = reinterpret_cast<gui::CButton*>(pParent->GetComponent("btnCharTitle"));
	m_btnCharTitle->SetText(GetDisplayStringManager()->GetString("DST_CHAR_TITLE"));
	m_slotCharTitleButton = m_btnCharTitle->SigClicked().Connect(this, &CStatusAvatarTab::OnCharTitleBtnClicked);

	m_pnlBattleAttribute = reinterpret_cast<gui::CPanel*>( pParent->GetComponent( "pnlBattleAttr" ) );
	
	m_slotMouseDown = m_pSelf->SigMouseDown().Connect( this, &CStatusAvatarTab::OnMouseDown );
	m_slotMouseUp = m_pSelf->SigMouseUp().Connect( this, &CStatusAvatarTab::OnMouseUp );
	m_slotMouseMove = m_pSelf->SigMouseMove().Connect( this, &CStatusAvatarTab::OnMouseMove );
	m_slotMove = m_pSelf->SigMove().Connect( this, &CStatusAvatarTab::OnMove );
	m_slotMouseLeave = m_pSelf->SigMouseLeave().Connect( this, &CStatusAvatarTab::OnMouseLeave );
	m_slotPaint = m_pSelf->SigPaint().Connect( this, &CStatusAvatarTab::OnPaint );
	m_slotSlotPaint = m_pstbBattleAttrTitle->SigPaint().Connect( this, &CStatusAvatarTab::OnSlotPaint );

	m_slotLeftRotBtnPress = m_pbtnLeftRot->SigPressed().Connect( this, &CStatusAvatarTab::OnLeftRotBtnPress );
	m_slotLeftRotBtnRelease = m_pbtnLeftRot->SigReleased().Connect( this, &CStatusAvatarTab::OnLeftRotBtnRelease );
	m_slotRightRotBtnPress = m_pbtnRightRot->SigPressed().Connect( this, &CStatusAvatarTab::OnRightRotBtnPress );
	m_slotRightRotBtnRelease = m_pbtnRightRot->SigReleased().Connect( this, &CStatusAvatarTab::OnRightRotBtnRelease );
	m_slotDragRotBtnPress = m_pbtnDragRot->SigPressed().Connect( this, &CStatusAvatarTab::OnDragRotBtnPress );
	m_slotDragRotBtnRelease = m_pbtnDragRot->SigReleased().Connect( this, &CStatusAvatarTab::OnDragRotBtnRelease );
	m_slotDragRotMouseMove = m_pbtnDragRot->SigMouseMove().Connect( this, &CStatusAvatarTab::OnDragRotMouseMove );

	m_slotWeaponMouseEnter = m_pnlBattleAttribute->SigMouseEnter().Connect( this, &CStatusAvatarTab::OnMouseBattleAttrEnter);
	m_slotWeaponMouseLeave = m_pnlBattleAttribute->SigMouseLeave().Connect( this, &CStatusAvatarTab::OnMouseBattleAttrLeave);
	

	// Equip Slot CoordSet
	SetSlotRectHardCode();
	SetTextHardCode();

	// load title
	SetCharTitleText();

	// Tooltip
	SetAttributeToolTip();

	return TRUE;
}

VOID CStatusAvatarTab::Destroy(VOID)
{

}

VOID CStatusAvatarTab::UpdateBeforeCamera( RwReal fElapsed )
{
	if( !IsShow() )
		return;

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

	m_texCharacter.Load( pCharProxy->UIPcStatusWndRender() );
	m_surCharacter.SetTexture( &m_texCharacter );
}

VOID CStatusAvatarTab::HandleEvents( RWS::CMsg& msg )
{
	// Equip 정보
	if( msg.Id == g_EventSobInfoUpdate )
	{
		if( !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;
				
		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			UpdateEquipData();
		}
		if( pUpdate->uiUpdateType & EVENT_AIUT_ATTR )
		{
			UpdateStatData();
			
			// 아바타의 Attribute ( Battle Attribute bonus ) 가 업데이트 되면 다시 계산해준다.
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
	else if( msg.Id == g_EventSobConvertClass )
	{// 클래스 변경은 이것밖에 안날라온다.
		SNtlEventSobConvertClass* pData = reinterpret_cast<SNtlEventSobConvertClass*>( msg.pData );

		if( pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			m_pstbPCInfo[CLASS]->SetText( Logic_GetClassName( pData->byClass ) );
		}
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		ShowIconDestination();
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_EQUIP )
			return;

		ShowPickedUp( FALSE );
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace == PLACE_EQUIP )
			ShowDisableSlot( !pData->bEnable, pData->uiSlotIdx );
		else if( pData->ePlace == PLACE_SCOUTER_SLOT )
			ShowDisableSlot( !pData->bEnable, EQUIP_SLOT_TYPE_SCOUTER );
	}
	else if(msg.Id == g_EventSobEquipChange )
	{
		SNtlEventSobEquipChange* pEquipChange = reinterpret_cast<SNtlEventSobEquipChange*>( msg.pData );

		if( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() == pEquipChange->hSerialId ||
			Logic_GetAvatarTargetHandle() == pEquipChange->hSerialId )
		{
			CalcBattleAttribute();
			OnBattleAttributeRefresh();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// ! Implements

VOID CStatusAvatarTab::UpdateEquipData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( !pAvatar )
		return;				// 초기 아바타 생성전에 들어오는 경우.

	CNtlInventory* pInventory = pAvatar->GetInventory();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		SERIAL_HANDLE hEquipSerial = pInventory->GetEquipItem( i );

		if( hEquipSerial == INVALID_SERIAL_ID )
		{
			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				if (m_surIcon[i].GetTexture())
				{
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_UNREG);
					CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_STATUS, false);
				}
			}

			ShowDisableSlot( FALSE, i );
			m_surIcon[i].SetTexture( (gui::CTexture*)NULL );
			FritzEffect( FRITZ_NONE, i );			
		}
		else
		{
			if( i == EQUIP_SLOT_TYPE_SCOUTER )
			{
				if (!m_surIcon[i].GetTexture())
				{
					Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_SCOUTER_SLOT_REG);
					CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_STATUS, false);
				}
			}

			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipSerial ) );
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
			
			m_surIcon[i].SetTexture( (gui::CTexture*)pItem->GetIcon()->GetImage() );

			if( pItemAttr->IsExpired() || pItemAttr->GetDur() == 0 )
				FritzEffect( FRITZ_ABSOLUTE, i );
			else if( (RwReal)pItemAttr->GetDur() / (RwReal)pItemAttr->GetMaxDur() <= ITEM_DURATION_WARNING )
				FritzEffect( FRITZ_HALF, i );
			else
				FritzEffect( FRITZ_NONE, i );			
		}
	}

	if( m_nMouseOnIndex >= 0 && GetInfoWndManager()->GetRequestGui() == DIALOG_STATUS )
	{
		CRectangle rtScreen = m_pSelf->GetScreenRect();		

		if( pInventory->GetEquipItem( m_nMouseOnIndex ) == INVALID_SERIAL_ID )
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, m_rtEquipSlot[m_nMouseOnIndex].left + rtScreen.left, m_rtEquipSlot[m_nMouseOnIndex].top + rtScreen.top, (VOID*)GetEquipSlotText( m_nMouseOnIndex ), DIALOG_STATUS );						
		}
		else
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetEquipItem( m_nMouseOnIndex ) ) );
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, m_rtEquipSlot[m_nMouseOnIndex].left + rtScreen.left, m_rtEquipSlot[m_nMouseOnIndex].top + rtScreen.top, pItem, DIALOG_STATUS );
		}
	}
}

VOID CStatusAvatarTab::UpdateStatData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( !pAvatar )
		return;				// 초기 아바타 생성전에 들어오는 경우.

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
	RwUInt32 uiColor;

	// BasicStat
	uiColor = BASIC_STAT_COLOR;// GetGuiFuntor()->ColorDecision(pAvatarAttr->m_baseStr, pAvatarAttr->m_lastStr);
	m_pstbBasicStat[STR]->SetTextColor( uiColor );
	m_pstbBasicStat[STR]->SetText( pAvatarAttr->m_Str );
	m_pstbBasicStat[DEX]->SetTextColor( uiColor );
	m_pstbBasicStat[DEX]->SetText( pAvatarAttr->m_Dex );
	m_pstbBasicStat[CON]->SetTextColor( uiColor );
	m_pstbBasicStat[CON]->SetText( pAvatarAttr->m_Con );
	m_pstbBasicStat[ENG]->SetTextColor( uiColor );
	m_pstbBasicStat[ENG]->SetText( pAvatarAttr->m_Eng );
	m_pstbBasicStat[SOL]->SetTextColor( uiColor );
	m_pstbBasicStat[SOL]->SetText( pAvatarAttr->m_Sol );
	m_pstbBasicStat[FOC]->SetTextColor( uiColor );
	m_pstbBasicStat[FOC]->SetText( pAvatarAttr->m_Foc );
	
	// Physical Stat
	m_pstbPhysicalCombatStat[OFFENCE]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[OFFENCE]->SetText( pAvatarAttr->m_wPhysicalOffence );
	m_pstbPhysicalCombatStat[DEFENCE]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[DEFENCE]->SetText( pAvatarAttr->m_wPhysicalDefence );
	m_pstbPhysicalCombatStat[CRITICAL]->SetTextColor( uiColor );
	m_pstbPhysicalCombatStat[CRITICAL]->SetText( pAvatarAttr->m_wPhysicalCriticalRate );
	
	// Energy Stat
	m_pstbEnergyCombatStat[OFFENCE]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[OFFENCE]->SetText( pAvatarAttr->m_wEnergyOffence );
	m_pstbEnergyCombatStat[DEFENCE]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[DEFENCE]->SetText( pAvatarAttr->m_wEnergyDefence );
	m_pstbEnergyCombatStat[CRITICAL]->SetTextColor( uiColor );
	m_pstbEnergyCombatStat[CRITICAL]->SetText( pAvatarAttr->m_wEnergyCriticalRate );
	
	// Etc Stat
	m_pstbETCStat[ATTACKRATE]->SetTextColor( uiColor );
	m_pstbETCStat[ATTACKRATE]->SetText( pAvatarAttr->m_wAttackRate );
	m_pstbETCStat[DODGE]->SetTextColor( uiColor );
	m_pstbETCStat[DODGE]->SetText( pAvatarAttr->m_wDodgeRate );
	
	// Name
	m_pstbPCInfo[NAME]->SetText( pAvatarAttr->GetName() );
	m_pstbPCInfo[LEVEL]->SetText( pAvatarAttr->GetLevel() );
	m_pstbPCInfo[CLASS]->SetText( Logic_GetClassName( pAvatarAttr->GetClass() ) );
	m_pstbPCInfo[AP]->SetText( pAvatarAttr->GetAP() );

	m_pstbStat[LP]->Format( "%d / %d", pAvatarAttr->GetLp(), pAvatarAttr->GetMaxLp() );
	m_pstbStat[EP]->Format( "%d / %d", pAvatarAttr->GetEp(), pAvatarAttr->GetMaxEp() );
	
	if( pAvatarAttr->m_uiMaxExp )
	{
		m_pstbStat[EXP_CUR]->Format( L"%s : %d", GetDisplayStringManager()->GetString( "DST_STATUS_CURRENT_EXP" ), pAvatarAttr->m_uiExp );
		m_pstbStat[EXP_MAX]->Format( L"%s : %d", GetDisplayStringManager()->GetString( "DST_STATUS_GOAL_EXP" ), pAvatarAttr->m_uiMaxExp );

		if( m_pstbStat[EXP_CUR]->GetFitWidthToString() > m_pstbStat[EXP_CUR]->GetWidth() )
			m_pstbStat[EXP_CUR]->SetText( pAvatarAttr->m_uiExp );
		if( m_pstbStat[EXP_MAX]->GetFitWidthToString() > m_pstbStat[EXP_MAX]->GetWidth() )
			m_pstbStat[EXP_MAX]->SetText( pAvatarAttr->m_uiMaxExp );
	}	
	else
	{
		m_pstbStat[EXP_CUR]->SetText( "MAX EXP" );
		m_pstbStat[EXP_MAX]->Clear();
	}
}

RwInt32 CStatusAvatarTab::GetEquipSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_rtEquipSlot[i].PtInRect( CPos( nX, nY ) ) )
			return i;
	}

	return -1;
}

CRectangle CStatusAvatarTab::GetEquipSlotRect( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return CRectangle( 0, 0, 0, 0 );

	return m_rtEquipSlot[nSlotIdx];
}

const WCHAR* CStatusAvatarTab::GetEquipSlotText( RwInt32 nSlotIdx )
{
	if( nSlotIdx < EQUIP_SLOT_TYPE_FIRST || nSlotIdx > EQUIP_SLOT_TYPE_LAST )
		return NULL;


	switch( nSlotIdx )
	{
		case EQUIP_SLOT_TYPE_HAND: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_HAND"); break;
		case EQUIP_SLOT_TYPE_SUB_WEAPON: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_SUB_WEAPON"); break;
		case EQUIP_SLOT_TYPE_JACKET: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_JACKET"); break;
		case EQUIP_SLOT_TYPE_PANTS: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_PANTS"); break;
		case EQUIP_SLOT_TYPE_BOOTS: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_BOOTS"); break;
		case EQUIP_SLOT_TYPE_SCOUTER: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_SCOUTER"); break;
		case EQUIP_SLOT_TYPE_QUEST: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_QUEST"); break;
		case EQUIP_SLOT_TYPE_NECKLACE: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_NECKLACE"); break;
		case EQUIP_SLOT_TYPE_EARRING_1: 
		case EQUIP_SLOT_TYPE_EARRING_2: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_EARRING"); break;
		case EQUIP_SLOT_TYPE_RING_1:  
		case EQUIP_SLOT_TYPE_RING_2: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_RING"); break;
		case EQUIP_SLOT_TYPE_COSTUME_SET: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_DOGI"); break;
		case EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_HAIR"); break;
		case EQUIP_SLOT_TYPE_COSTUME_MASK: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_ACCESSORY1"); break;
		case EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_ACCESSORY2"); break;
		case EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY: return GetDisplayStringManager()->GetString("DST_EQUIP_SLOT_TYPE_ACCESSORY3"); break;
	}

	static WCHAR* text = L"Equip slot not";
	return text;
}

RwBool CStatusAvatarTab::IsEnableEquipPickUp( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	if( !GetIconMoveManager()->IsEnable() )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	SERIAL_HANDLE hSlotSerial = pInventory->GetEquipItem( nSlotIdx );
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSlotSerial ) );

	if( !pSobItem )
		return FALSE;

	CNtlSobIcon* pIcon = pSobItem->GetIcon();
	if( !pIcon->IsDragAndDropPossible() )
		return FALSE;

	return TRUE;
}

RwBool CStatusAvatarTab::IsEnableEquipPutDown( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	// 나머지 비교는 Logic에 맡긴다. 

	return TRUE;
}

RwBool CStatusAvatarTab::IsEnableUnequipItem( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 )
		return FALSE;

	if( IsDisableSlot( nSlotIdx ) )
		return FALSE;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	if( pInventory->GetEquipItem( nSlotIdx ) == INVALID_SERIAL_ID )
		return FALSE;
	
	return TRUE;
}

//RwBool CStatusAvatarTab::IsEnableEquipPopup( RwInt32 nSlotIdx )
//{
//	if( nSlotIdx < 0 )
//		return FALSE;
//
//	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
//	SERIAL_HANDLE hSlotSerial = pInventory->GetEquipItem( nSlotIdx );
//
//	if( hSlotSerial == INVALID_SERIAL_ID )
//		return FALSE;
//
//	return TRUE;
//}

VOID CStatusAvatarTab::ShowIconDestination(VOID)
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( GetIconMoveManager()->GetSrcPlace() != PLACE_BAG )
			return;

		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() ) );
		if( !pSobItem )
			return;

		CNtlSobItemAttr* pSobAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );

		if( !pSobItem->IsEquipItem() )
			return;


		for( RwUInt8 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
		{
			if( Logic_ConvertEquipSlotIdxToFlag( i ) & pSobAttr->GetItemTbl()->dwEquip_Slot_Type_Bit_Flag )
			{
				m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS_CAN_MOVE;
			}
			else
			{
				m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
			}
		}
	}
	else
	{
		for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS_CAN_MOVE;
		}
	}
}

VOID CStatusAvatarTab::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	m_rtEquipSlot[EQUIP_SLOT_TYPE_HAND].SetRectWH( 165, 231, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_SUB_WEAPON].SetRectWH( 165, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_JACKET].SetRectWH( 165, 105, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_PANTS].SetRectWH( 165, 147, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_BOOTS].SetRectWH( 165, 189, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_SCOUTER].SetRectWH( 66, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_QUEST].SetRectWH( 118, 273, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_NECKLACE].SetRectWH( 20, 105, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_EARRING_1].SetRectWH( 20, 147, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_EARRING_2].SetRectWH( 20, 189, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_RING_1].SetRectWH( 20, 231, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );
	m_rtEquipSlot[EQUIP_SLOT_TYPE_RING_2].SetRectWH( 20, 273, NTL_ITEM_ICON_SIZE , NTL_ITEM_ICON_SIZE );

	// set dogi inventory slot
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME_SET].SetRectWH(207, 105, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE].SetRectWH(207, 147, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME_MASK].SetRectWH(207, 189, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY].SetRectWH(207, 231, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);
	m_rtEquipSlot[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].SetRectWH(207, 273, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE);

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surIcon[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
							  rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );
		m_surFocusSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
		m_surFocusSlot[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
								   rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );

		m_surDisableSlot[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );
		m_surDisableSlot[i].SetRect( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top,
									 rtScreen.left + m_rtEquipSlot[i].right, rtScreen.top + m_rtEquipSlot[i].bottom );
		m_surDisableSlot[i].Show( false );
		
		// Default
		m_surFritzSlot[i].Show( false );
	}

	m_surPickedUp.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	m_surCharacter.SetRectWH( rtScreen.left + 43, rtScreen.top + 96, 132, 176 );
}

VOID CStatusAvatarTab::SetTextHardCode(VOID)
{
	m_pstbBasicStatTitle[0]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_STR"));
	m_pstbBasicStatTitle[1]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_DEX"));
	m_pstbBasicStatTitle[2]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_CON"));
	m_pstbBasicStatTitle[3]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_ENG"));
	m_pstbBasicStatTitle[4]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_SOL"));
	m_pstbBasicStatTitle[5]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_STAT_FOC"));
	
	m_pstbPhysicalCombatStatTitle[0]->SetText( GetDisplayStringManager()->GetString("DST_STATUS_PHYSICAL_ATTACK") );
	m_pstbPhysicalCombatStatTitle[1]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_PHYSICAL_DEFENCE"));
	m_pstbPhysicalCombatStatTitle[2]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_PHYSICAL_CRITICAL"));

	m_pstbEnergyCombatStatTitle[0]->SetText( GetDisplayStringManager()->GetString("DST_STATUS_ENERGY_ATTACK") );
	m_pstbEnergyCombatStatTitle[1]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_ENERGY_DEFENCE"));
	m_pstbEnergyCombatStatTitle[2]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_ENERGY_CRITICAL"));

	m_pstbETCStatTitle[0]->SetText( GetDisplayStringManager()->GetString("DST_STATUS_ATTACK_RATE") );
	m_pstbETCStatTitle[1]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_DODGE"));

	m_pstbPCInfoTitle[NAME]->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_NAME" ) );
	m_pstbPCInfoTitle[CLASS]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_JOB"));
	m_pstbPCInfoTitle[LEVEL]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_LEVEL"));
	m_pstbPCInfoTitle[AP]->SetText(GetDisplayStringManager()->GetString("DST_STATUS_AP"));

	m_pstbBattleAttrTitle->SetText( GetDisplayStringManager()->GetString("DST_STATUS_ATTR"));	

	m_pstbPCCharacterTitle->SetText(GetDisplayStringManager()->GetString("DST_CHAR_TITLE"));
}

VOID CStatusAvatarTab::CalcBattleAttribute(VOID)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar == NULL )
		return;

	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
	CNtlSobCharProxy* pAvatarProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobAvatar->GetSobProxy() );

	SetBattleAttribute(pSobAvatarAttr->GetMainBattleAttr());
	SetSourceAttr(pSobAvatarAttr->GetMainBattleAttr());
	

	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_HONEST] = pSobAvatarAttr->m_fCriticalBlockSuccessRate;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_STRANGE] = pSobAvatarAttr->m_fEnergyCriticalDefenceRate;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_WILD] = pSobAvatarAttr->m_fPhysicalArmorPenRate;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_ELEGANCE] = pSobAvatarAttr->m_fEnergyArmorPenRate;
	m_sBattleAttr.afSourceOffenceBonus[BATTLE_ATTRIBUTE_FUNNY] = pSobAvatarAttr->m_fPhysicalCriticalDefenceRate;
}

VOID CStatusAvatarTab::SetBattleAttribute( RwUInt8 byBattleAttribute )
{
	Logic_SetBattleAttributeMark( m_pnlBattleAttribute, byBattleAttribute, FALSE );	
}

VOID CStatusAvatarTab::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */ )
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	if( bPush )
	{
		m_surIcon[nSlotIdx].SetRect( rtScreen.left + m_rtEquipSlot[nSlotIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_rtEquipSlot[nSlotIdx].top + ICONPUSH_SIZEDIFF,
									 rtScreen.left + m_rtEquipSlot[nSlotIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_rtEquipSlot[nSlotIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		m_surIcon[m_nPushDownIndex].SetRect( rtScreen.left + m_rtEquipSlot[m_nPushDownIndex].left, rtScreen.top + m_rtEquipSlot[m_nPushDownIndex].top,
											 rtScreen.left + m_rtEquipSlot[m_nPushDownIndex].right, rtScreen.top + m_rtEquipSlot[m_nPushDownIndex].bottom );
	}	

	m_nPushDownIndex = nSlotIdx;
}

VOID CStatusAvatarTab::FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 || nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return;

	switch( eFritzState )
	{
	case FRITZ_NONE:
		m_surFritzSlot[nSlotIdx].Show( FALSE );
		return;
	case FRITZ_HALF:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfHalfFritzSlotEffect" ) );
		break;
	case FRITZ_ABSOLUTE:
		m_surFritzSlot[nSlotIdx].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfFritzSlotEffect" ) );
		break;
	}

	CRectangle rtScreen = m_pSelf->GetScreenRect();

	m_surFritzSlot[nSlotIdx].SetRect( rtScreen.left + m_rtEquipSlot[nSlotIdx].left, rtScreen.top + m_rtEquipSlot[nSlotIdx].top,
									  rtScreen.left + m_rtEquipSlot[nSlotIdx].right, rtScreen.top + m_rtEquipSlot[nSlotIdx].bottom );

	m_surFritzSlot[nSlotIdx].Show( TRUE );
}

VOID CStatusAvatarTab::ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx /* = 0  */ )
{
	if( bShow )
	{
		CRectangle rtScreen = m_pSelf->GetScreenRect();

		m_nShowPickedUp = nSlotIdx;
		m_surPickedUp.SetPosition( m_rtEquipSlot[nSlotIdx].left + rtScreen.left, m_rtEquipSlot[nSlotIdx].top + rtScreen.top );
	}
	else
	{
		m_nShowPickedUp = -1;
	}
}

VOID CStatusAvatarTab::ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx )
{
	m_surDisableSlot[nSlotIdx].Show( bShow );
}

RwBool CStatusAvatarTab::IsDisableSlot( RwInt32 nSlotIdx )
{
	if( nSlotIdx < 0 && nSlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT )
		return TRUE;

	return m_surDisableSlot[nSlotIdx].IsShow() ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// ! Callback

VOID CStatusAvatarTab::OnMouseDown( const CKey& key )
{
	RwInt32 nClickIdx = GetEquipSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	if( EQUIP_SLOT_TYPE_SCOUTER == nClickIdx )
	{
		if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_STATUS_WND_SCOUTER_SLOT ) )
		{
			m_nLSelectedSlotIdx = -1;
			m_nRSelectedSlotIdx = -1;
			ClickEffect( FALSE );
			m_pSelf->ReleaseMouse();
			return;
		}
	}	

	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		// 1. Src선택인가 Dest선택인가.
		if( GetIconMoveManager()->IsActive() )
		{
			if( IsEnableEquipPutDown( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}
		}
		else
		{
			eDialogMode eMode = GetDialogManager()->GetMode();

			if( eMode == DIALOGMODE_ITEM_REPAIR ||
				eMode == DIALOGMODE_ITEM_IDENTIFICATION ||
				eMode == DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}
			// 2. DragDrop이 가능한 상황인가.
			else if( IsEnableEquipPickUp( nClickIdx ) )
			{
				m_nLSelectedSlotIdx = nClickIdx;
			}			
		}		
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
		{
			if( !GetIconMoveManager()->IsActive() )
			{
				if( IsEnableUnequipItem( nClickIdx ) )
				{
					m_nRSelectedSlotIdx = nClickIdx;
				}
			}			
		}
		
		// 1. 아이콘 유효성 검사
		//if( IsEnableEquipPopup( nClickIdx ) )
		//{
		//	m_nRSelectedSlotIdx = nClickIdx;
		//}
	}

	m_pSelf->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pSelf->ReleaseMouse();
	}
}

VOID CStatusAvatarTab::OnMouseUp( const CKey& key )
{
	// 1. Left버튼인가 Right버튼인가
	// 2. Src선택인가 Dest선택인가.

	RwInt32 nSlotIdx = GetEquipSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );
	ClickEffect( FALSE );
	m_pSelf->ReleaseMouse();

	if( nSlotIdx < 0 || !m_pSelf->IsVisibleTruly() )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		return;
	}

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( nSlotIdx == m_nLSelectedSlotIdx )
		{
			if( GetIconMoveManager()->IsActive() )
			{
				if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
				{
					GetIconMoveManager()->IconMovePutDown( PLACE_EQUIP, INVALID_SERIAL_ID, nSlotIdx );
				}				
			}
			else
			{
				if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) )
				{
					Logic_ItemRepairProc( CONTAINER_TYPE_EQUIP, (RwUInt8)nSlotIdx );					
				}
				else
				{
					if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
					{
						CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
						SERIAL_HANDLE hSrcSerial = pInventory->GetEquipItem( nSlotIdx );
						CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
						if( pItem )
						{
							CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
							if( GetIconMoveManager()->IconMovePickUp( hSrcSerial, PLACE_EQUIP, nSlotIdx, pItemAttr->GetStackNum(), pItem->GetIcon()->GetImage() ) )
								ShowPickedUp( TRUE, nSlotIdx );
						}
					}					
				}
			}

		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( GetDialogManager()->GetMode() == DIALOGMODE_UNKNOWN )
		{
			if( nSlotIdx == m_nRSelectedSlotIdx )
			{
				RwInt32 nDestBagSlotIdx = 0, nDestSlotIdx = 0;
				if( Logic_FindEmptyItemSlot( &nDestBagSlotIdx, &nDestSlotIdx ) )
				{
					CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
					SERIAL_HANDLE hSrcSerial = pInventory->GetEquipItem( nSlotIdx );
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSrcSerial ) );
					if( pItem )
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
						Logic_ItemMoveProc( hSrcSerial, PLACE_EQUIP, (RwUInt8)nSlotIdx, PLACE_BAG, pInventory->GetBagItem( nDestBagSlotIdx ), (RwUInt8)nDestSlotIdx, pItemAttr->GetStackNum() );
					}					
				}
				else
				{
					GetAlarmManager()->AlarmMessage( "DST_ITEM_CAN_NOT_UNEQUIP_BAG_FULL" );
				}	
			}
		}
		
		m_nRSelectedSlotIdx = -1;
	}
}

VOID CStatusAvatarTab::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwBool bFlag = FALSE;

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_rtEquipSlot[i].PtInRect( nX, nY ) )
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] | SLOT_FOCUS;

			if( m_nMouseOnIndex != i )
			{
				SERIAL_HANDLE hSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( i );
				CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
				CRectangle rtScreen = m_pSelf->GetScreenRect();

				if( pItem )
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM, m_rtEquipSlot[i].left + rtScreen.left, m_rtEquipSlot[i].top + rtScreen.top, pItem, DIALOG_STATUS );					
				}
				else
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, m_rtEquipSlot[i].left + rtScreen.left, m_rtEquipSlot[i].top + rtScreen.top, (VOID*)GetEquipSlotText( i ), DIALOG_STATUS );										
				}

				if( i == m_nPushDownIndex )
					ClickEffect( TRUE, i );
				else if( m_nPushDownIndex >= 0 )
					ClickEffect( FALSE, m_nPushDownIndex );

				m_nMouseOnIndex = i;
			}

			bFlag = TRUE;
		}
		else
		{
			m_arrFocusEffect[i] = m_arrFocusEffect[i] & ~SLOT_FOCUS;
		}
	}

	if( !bFlag && m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CStatusAvatarTab::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex != -1 )
	{
		m_arrFocusEffect[m_nMouseOnIndex] = m_arrFocusEffect[m_nMouseOnIndex] & ~SLOT_FOCUS;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
	}
}

VOID CStatusAvatarTab::OnCharTitleBtnClicked(gui::CComponent * pComponent)
{
	if (GetDialogManager()->IsOpenDialog(DIALOG_PLAYER_TITLE))
		GetDialogManager()->CloseDialog(DIALOG_PLAYER_TITLE);
	else
		GetDialogManager()->OpenDialog(DIALOG_PLAYER_TITLE);
}

VOID CStatusAvatarTab::OnPaint(VOID)
{
	// Avatar출력
	m_surCharacter.Render();
}

VOID CStatusAvatarTab::OnSlotPaint(VOID)
{
	// Icon 출력
	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		if( m_surIcon[i].GetTexture() )
			m_surIcon[i].Render();
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.Render();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surFritzSlot[i].Render();
		m_surDisableSlot[i].Render();

		if( m_arrFocusEffect[i] )
			m_surFocusSlot[i].Render();
	}
}

VOID CStatusAvatarTab::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pSelf->GetScreenRect();

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		m_surIcon[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surFocusSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surFritzSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
		m_surDisableSlot[i].SetPosition( rtScreen.left + m_rtEquipSlot[i].left, rtScreen.top + m_rtEquipSlot[i].top );
	}

	if( m_nShowPickedUp >= 0 )
		m_surPickedUp.SetPosition( rtScreen.left + m_rtEquipSlot[m_nShowPickedUp].left, rtScreen.top + m_rtEquipSlot[m_nShowPickedUp].top );

	m_surCharacter.SetPosition( rtScreen.left + 46, rtScreen.top + 96 );
}

VOID CStatusAvatarTab::OnLeftRotBtnPress( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateLeft();
}

VOID CStatusAvatarTab::OnLeftRotBtnRelease( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateStop();
}

VOID CStatusAvatarTab::OnRightRotBtnPress( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateRight();
}

VOID CStatusAvatarTab::OnRightRotBtnRelease( gui::CComponent* pComponent )
{
	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );
	if( pCharProxy )
		pCharProxy->PcStatusRotateStop();	
}

VOID CStatusAvatarTab::OnDragRotBtnPress( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();
	m_nDragRotX = CMouse::GetX() - rtScreen.left;
}

VOID CStatusAvatarTab::OnDragRotBtnRelease( gui::CComponent* pComponent )
{

}

VOID CStatusAvatarTab::OnDragRotMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	if( nFlags & UD_MK_LBUTTON )
	{
		CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy() );

		RwReal fDeltaAngle = (RwReal)( nX - m_nDragRotX ) * 1.5f;
		pCharProxy->PcStatusRotate( fDeltaAngle );

		m_nDragRotX = nX;
	}
}

VOID CStatusAvatarTab::OnMouseBattleAttrEnter( gui::CComponent* pComponent )
{
	CRectangle rtScreen = pComponent->GetScreenRect();

	m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;

	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
		rtScreen.left, rtScreen.top,
		reinterpret_cast<void*>( &m_sBattleAttr ),
		DIALOG_STATUS );
}

VOID CStatusAvatarTab::OnMouseBattleAttrLeave( gui::CComponent* pComponent )
{
	if( DIALOG_STATUS == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CStatusAvatarTab::SetSourceAttr( RwUInt8 bySourceWeaponAttr)
{
	m_sBattleAttr.bySourceWeaponAttr = bySourceWeaponAttr;
}

VOID CStatusAvatarTab::SetTargetAttr( RwUInt8 byTargetWeaponAttr)
{
	m_sBattleAttr.byTargetWeaponAttr = byTargetWeaponAttr;
}

VOID CStatusAvatarTab::OnBattleAttributeRefresh()
{
	if( m_pnlBattleAttribute->GetScreenRect().PtInRect( CMouse::GetX(), CMouse::GetY() ) )
		m_sBattleAttr.eBattleAttrInfoType = stINFOWND_BATTLEATTR::TYPE_ATTR_WEAPON_INFO;

	if( DIALOG_STATUS == GetInfoWndManager()->GetRequestGui() &&
		CInfoWndManager::INFOWND_BATTLEATTRIBUTE == GetInfoWndManager()->GetInfoWndState() )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );

		CRectangle rtScreen = m_pnlBattleAttribute->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BATTLEATTRIBUTE,
			rtScreen.left, rtScreen.top,
			reinterpret_cast<void*>( &m_sBattleAttr ),
			DIALOG_STATUS );
	}
}


void CStatusAvatarTab::SetCharTitleText()
{
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	if (pAvatarInfo->sCharPf.charTitle == INVALID_TBLIDX)
	{
		m_pstbPCActiveCharacterTitle->SetText(GetDisplayStringManager()->GetString("DST_CHARTITLE_NOSELECT"));
	}
	else
	{
		CTextTable* pTitleTextTable = API_GetTableContainer()->GetTextAllTable()->GetCharTitleTbl();

		m_pstbPCActiveCharacterTitle->SetText(pTitleTextTable->GetText(pAvatarInfo->sCharPf.charTitle).c_str());
	}
}

void CStatusAvatarTab::SelectCharTitle(TBLIDX uiIndex)
{
	CNtlSobActor* pActor = Logic_GetAvatarActor();
	CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>(pActor);

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	pAvatarInfo->sCharPf.charTitle = uiIndex;

	SetCharTitleText();

	pSobPlayer->SetCharTitle(pAvatarInfo->sCharPf.bInvisibleTitle, uiIndex);

	Logic_SetHeadNameColor(pSobPlayer);
}

void CStatusAvatarTab::UpdateCharTitle(TBLIDX uiIndex, bool bDelete)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlOtherParam* pOtherParam = pAvatar->GetOtherParam();

	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	if (bDelete)
	{
		// remove the title
		pOtherParam->UnSetCharTitleValue(uiIndex - 1);

		// when we delete char title and player currently use it, then the server will send nfy packet to everyone that the title has changed.
		// So we can do the following code and dont have to send SendCharTitleSelectReq packet to the server
		if (pAvatarInfo->sCharPf.charTitle == uiIndex)
		{
			SelectCharTitle(INVALID_TBLIDX);
		}
	}
	else
	{
		//add title
		pOtherParam->SetCharTitleValue(uiIndex - 1);
	}
}

VOID CStatusAvatarTab::SetAttributeToolTip( VOID )
{
	m_pstbBattleAttrTitle->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ATTR" ) );
	
	m_pstbBasicStatTitle[STR]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_STR" ) );
	m_pstbBasicStatTitle[DEX]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_DEX" ) );
	m_pstbBasicStatTitle[CON]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_CON" ) );
	m_pstbBasicStatTitle[ENG]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ENG" ) );
	m_pstbBasicStatTitle[SOL]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_SOL" ) );
	m_pstbBasicStatTitle[FOC]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_FOC" ) );

	m_pstbPhysicalCombatStatTitle[OFFENCE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_PHYSICAL_ATTACK" ) );
	m_pstbPhysicalCombatStatTitle[DEFENCE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_PHYSICAL_DEFENCE" ) );
	m_pstbPhysicalCombatStatTitle[CRITICAL]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_PHYSICAL_CRITICAL" ) );

	m_pstbEnergyCombatStatTitle[OFFENCE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ENERGY_ATTACK" ) );
	m_pstbEnergyCombatStatTitle[DEFENCE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ENERGY_DEFENCE" ) );
	m_pstbEnergyCombatStatTitle[CRITICAL]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ENERGY_CRITICAL" ) );

	m_pstbETCStatTitle[ATTACKRATE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_ATTACK_RATING" ) );
	m_pstbETCStatTitle[DODGE]->SetToolTip( GetDisplayStringManager()->GetString( "DST_STATUS_TOOLTIP_DODGE_RATING" ) );
}
//VOID CStatusWindowGui::SetStackNum( RwInt32 nSlot, RwInt32 nStackNum )
//{
//	if( nSlot >= NTL_MAX_EQUIP_ITEM_SLOT || nSlot < 0 )
//		return;
//
//	if( m_pGuiStackNum[nSlot] )
//	{
//		m_pGuiStackNum[nSlot]->SetText( nStackNum );
//	}
//	else
//	{
//		CRectangle rtStackNum( m_rtEquipSlot[nSlot].right - MAX_STACK_NUM_WIDTH,
//							   m_rtEquipSlot[nSlot].bottom - MAX_STACK_NUM_HEIGHT,
//							   MAX_STACK_NUM_WIDTH, MAX_STACK_NUM_HEIGHT );
//		m_pGuiStackNum[nSlot] = NTL_NEW gui::CStaticBox( rtStackNum, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
//		m_pGuiStackNum[nSlot]->CreateFontStd( "Arial", 70, EZ_ATTR_BOLD );
//	}
//}
//
//VOID CStatusWindowGui::DeleteStackNum( RwInt32 nSlot )
//{
//	if( nSlot >= NTL_MAX_EQUIP_ITEM_SLOT || nSlot < 0 )
//		return;
//
//	if( m_pGuiStackNum[nSlot] )
//		NTL_DELETE( m_pGuiStackNum[nSlot] );
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorList
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorList::CStatusHonorList()
//{
//
//}
//
//CStatusHonorList::~CStatusHonorList()
//{
//
//}
//
//RwBool CStatusHonorList::Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth )
//{
//	if( !CGuiLineTree::Create( rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth ))
//		return FALSE;
//
//	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollLayout" ) );
//	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollMedium" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnUp" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnDown" ) );
//	m_pScroll->GetDecreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollPrevBtnFoc" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnDown" ) );
//	m_pScroll->GetIncreaseButton()->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfScrollNextBtnUp" ) );
//	m_pScroll->SetButtonSize(dSTATUSWINDOW_LIST_SCROLL_WIDTH, dSTATUSWINDOW_LIST_SCROLL_HEIGHT);
//	m_pScroll->SetSliderSize(dSTATUSWINDOW_LIST_SLIDER_WIDTH, dSTATUSWINDOW_LIST_SLIDER_HEIGHT);
//
//	return TRUE;
//}
//
//void CStatusHonorList::CreateTree()
//{
//
//	// 명예 점수 카테고리
//	CGuiLineTreeNode* pNode = NTL_NEW CStatusHonorCategoryNode(this,
//		L"명예점수", -2 );
//	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
//	pNode->Expand( true );
//
//	pNode = NTL_NEW CStatusHonorContent( this, L"명예점수 : 9999999999", 10 );
//	CGuiLineTree::AddNode( pNode, -2 );
//
//	pNode = NTL_NEW CStatusHonorCategoryNode( this,
//		L"랭크배틀", -3 );
//	CGuiLineTree::AddNode( pNode, GUILINETREE_ROOTNODE_ID );
//	pNode->Expand( true );
//
//	pNode = NTL_NEW CStatusHonorContent( this, L"랭크포인트 : 99999999", 11 );
//	CGuiLineTree::AddNode( pNode, -3 );
//
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorCategoryNode
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorCategoryNode::CStatusHonorCategoryNode( CGuiLineTree* pMgr, std::wstring strTitle, RwInt32 nID )
//: CGuiLineTreeNode( pMgr, nID )
//, m_pBtnExpand(NULL)
//, m_pBtnReduce(NULL)
//{
//	CRectangle rect;
//	rect.SetRectWH( dSTATUSWINDOW_CATEGORY_BUTTON_X, dSTATUSWINDOW_CATEGORY_BUTTON_Y,
//		dSTATUSWINDOW_CATEGORY_BUTTON_WIDTH, dSTATUSWINDOW_CATEGORY_BUTTON_HEIGHT );
//	m_pBtnExpand = NTL_NEW gui::CButton( rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
//	m_pBtnExpand->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnUp"));
//	m_pBtnExpand->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnFoc"));
//	m_pBtnExpand->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfExpandBtnDown"));
//	m_pBtnExpand->SetText( strTitle.c_str() );
//
//	// -버튼
//	m_pBtnReduce = NTL_NEW gui::CButton(rect, std::string(),pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager());
//	m_pBtnReduce->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnUp"));
//	m_pBtnReduce->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnFoc"));
//	m_pBtnReduce->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("QuestList.srf", "srfReduceBtnDown"));
//	m_pBtnReduce->SetText( strTitle.c_str() );
//
//	// Button의 Signal 연결
//	m_slotClickedBtnExpand = m_pBtnExpand->SigClicked().Connect(this, &CStatusHonorCategoryNode::OnClickBtnExpand);
//	m_slotClickedBtnReduce = m_pBtnReduce->SigClicked().Connect(this, &CStatusHonorCategoryNode::OnClickBtnReduce);
//}
//
//CStatusHonorCategoryNode::~CStatusHonorCategoryNode()
//{
//	NTL_DELETE( m_pBtnExpand );
//	NTL_DELETE( m_pBtnReduce );
//}
//
//void CStatusHonorCategoryNode::ShowProc()
//{
//	CGuiLineTreeNode::ShowProc();
//
//	if (IsExpand())
//	{
//		m_pBtnReduce->Show(true);		
//		m_pBtnExpand->Show(false);
//	}
//	else
//	{
//		m_pBtnExpand->Show(true);
//		m_pBtnReduce->Show(false);		
//	}
//
//	// 카테고리 노드들의 위치를 재 계산
//	m_pBtnExpand->SetPosition(m_nPosX + dSTATUSWINDOW_CATEGORY_BUTTON_X, m_nPosY + dSTATUSWINDOW_CATEGORY_BUTTON_Y);
//	m_pBtnReduce->SetPosition(m_nPosX + dSTATUSWINDOW_CATEGORY_BUTTON_X, m_nPosY + dSTATUSWINDOW_CATEGORY_BUTTON_Y);
//}
//
//void CStatusHonorCategoryNode::HideProc()
//{
//	CGuiLineTreeNode::HideProc();
//
//	m_pBtnExpand->Show(false);
//	m_pBtnReduce->Show(false);
//}
//
//void CStatusHonorCategoryNode::OnClickBtnExpand( gui::CComponent* pComponent )
//{
//	Expand(TRUE);
//}
//
//void CStatusHonorCategoryNode::OnClickBtnReduce( gui::CComponent* pComponent )
//{
//	Expand(FALSE);
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// ! CStatusHonorContent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//CStatusHonorContent::CStatusHonorContent( CGuiLineTree* pMgr, const WCHAR* pwcContent, RwInt32 nID )
//: CGuiLineTreeNode( pMgr, nID )
//, m_pHtmlContent(NULL)
//{
//	CRectangle rect;
//	rect.SetRectWH( dSTATUSWINDOW_CONTENTS_X, dSTATUSWINDOW_CONTENTS_Y,
//		dSTATUSWINDOW_CONTENTS_WIDTH, dSTATUSWINDOW_CONTENTS_HEIGHT );
//	
//	m_pHtmlContent = NTL_NEW gui::CHtmlBox( rect, pMgr->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
//	m_pHtmlContent->SetHtmlFromMemory( pwcContent, wcslen( pwcContent ) );
//}
//
//CStatusHonorContent::~CStatusHonorContent()
//{
//	NTL_DELETE( m_pHtmlContent );
//}
//
//void CStatusHonorContent::ShowProc()
//{
//	CGuiLineTreeNode::ShowProc();
//
//	m_pHtmlContent->Show(true);
//
//	m_pHtmlContent->SetPosition( m_nPosX, m_nPosY );
//}
//
//void CStatusHonorContent::HideProc()
//{
//	CGuiLineTreeNode::HideProc();
//
//	m_pHtmlContent->Show(false);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CStatusHonorTab
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CStatusHonorTab::CStatusHonorTab()
{
	m_pSelf = NULL;
}

CStatusHonorTab::~CStatusHonorTab()
{
	
}

RwBool CStatusHonorTab::IsShow( VOID )
{
	if( !m_pSelf->IsVisible() )
		return FALSE;

	if( !m_pSelf->GetParent()->IsVisible() )
		return FALSE;

	return TRUE;
}

VOID CStatusHonorTab::Init()
{
	m_bExpandHonor = FALSE;
	m_bExpandRankBattle = FALSE;
}

RwBool CStatusHonorTab::Create( CNtlPLGui* pParent )
{
	NTL_FUNCTION( "CStatusHonorTab::Create" );

	Init();

	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgHonor" ) );

	m_pBtnExpandHonor = (gui::CButton*)pParent->GetComponent("btnHonorExpand");
	m_pBtnReduceHonor = (gui::CButton*)pParent->GetComponent("btnHonorReduce");
	m_pBtnExpandRankBattle = (gui::CButton*)pParent->GetComponent("btnRankBattleExpand");
	m_pBtnReduceRankBattle = (gui::CButton*)pParent->GetComponent("btnRankBattleReduce");

	m_pHtmlHonor = (gui::CHtmlBox*)pParent->GetComponent("htmlHonor");
	m_pHtmlRankBattle = (gui::CHtmlBox*)pParent->GetComponent("htmlRankBattle");

	m_slotClickedBtnExpandHonor = m_pBtnExpandHonor->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnExpandHonor );
	m_slotClickedBtnReduceHonor = m_pBtnReduceHonor->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnReduceHonor );
	m_slotClickedBtnExpandRankBattle = m_pBtnExpandRankBattle->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnExpandRankBattle );
	m_slotClickedBtnReduceRankBattle = m_pBtnReduceRankBattle->SigClicked().Connect( this, &CStatusHonorTab::OnClickedBtnReduceRankBattle );

	m_pBtnExpandHonor->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_TITLE_HONOR" ) );
	m_pBtnReduceHonor->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_TITLE_HONOR" ) );
	m_pBtnExpandRankBattle->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_TITLE_RANKBATTLE" ) );
	m_pBtnReduceRankBattle->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_TITLE_RANKBATTLE" ) );

	UpdateUI();
	Refresh();

	NTL_RETURN( TRUE );
}

VOID CStatusHonorTab::Destroy( VOID )
{
	NTL_FUNCTION("CStatusHonorTab::Destroy");

	NTL_RETURNVOID();
}

VOID CStatusHonorTab::UpdateUI()
{
	m_pBtnExpandRankBattle->Show( !m_bExpandRankBattle );
	m_pBtnReduceRankBattle->Show( B2b(m_bExpandRankBattle) );

	m_pBtnExpandRankBattle->SetPosition( 20, m_pBtnExpandHonor->GetPosition().top ); 
	m_pBtnReduceRankBattle->SetPosition( 20, m_pBtnExpandHonor->GetPosition().top ); 

	if( m_bExpandRankBattle )
	{
		m_pHtmlRankBattle->SetPosition( 50, m_pBtnExpandRankBattle->GetPosition().bottom + 5 );
		m_pHtmlRankBattle->Show( true );
	}
	else
	{
		m_pHtmlRankBattle->Show( false );
	}
}

VOID CStatusHonorTab::Refresh( VOID )
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( pSobAvatar )
	{
		CNtlRankBattle* pRankBattle = pSobAvatar->GetRankBattle();

		if( pRankBattle )
		{
			sRANKBATTLE_SCORE_INFO* pScore = pRankBattle->GetRankBattleScore();

			RwUInt32 uiTotal = pScore->dwWin
				+ pScore->dwDraw + pScore->dwLose;

			WCHAR awcData[256];
			swprintf_s( awcData, 256, GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_PRIVATE_SCORE" ),
				uiTotal, 
				pScore->dwWin,
				pScore->dwLose );

			WCHAR awcRankPoint[256];
			swprintf_s( awcRankPoint, 256,
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_RANK_SCORE" ),
				(RwInt32)pScore->fPoint );

			WCHAR awcMudosaPoint[256];
			swprintf_s( awcMudosaPoint, 
				256, 
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_MUDOSA_SCORE" ),
				Logic_GetMudosaPoint() );

			WCHAR awcRankBattle[1024];
			swprintf_s( awcRankBattle, 1024, 
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_HTMLTAG_RANKBATTLE" ), 
				awcData, 
				awcRankPoint, 
				awcMudosaPoint );

			m_pHtmlRankBattle->SetHtmlFromMemory( awcRankBattle, 1024*sizeof(WCHAR) );
		}
	}
	else
	{
		SAvatarInfo* pInfo = GetNtlSLGlobal()->GetAvatarInfo();

		if( pInfo )
		{
			RwUInt32 uiTotal = pInfo->sRankBattleScoreInfo.dwWin
				+ pInfo->sRankBattleScoreInfo.dwDraw + pInfo->sRankBattleScoreInfo.dwLose;

			WCHAR awcData[256];
			swprintf_s( awcData, 256, GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_PRIVATE_SCORE" ),
				uiTotal, 
				pInfo->sRankBattleScoreInfo.dwWin,
				pInfo->sRankBattleScoreInfo.dwLose );

			WCHAR awcRankPoint[256];
			swprintf_s( awcRankPoint, 256,
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_RANK_SCORE" ),
				(RwInt32)pInfo->sRankBattleScoreInfo.fPoint );

			WCHAR awcMudosaPoint[256];
			swprintf_s( awcMudosaPoint, 
				256, 
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_MUDOSA_SCORE" ),
				Logic_GetMudosaPoint() );

			WCHAR awcRankBattle[1024];
			swprintf_s( awcRankBattle, 1024, 
				GetDisplayStringManager()->GetString( "DST_STATUS_HONOR_HTMLTAG_RANKBATTLE" ), 
				awcData, 
				awcRankPoint, 
				awcMudosaPoint );

			m_pHtmlRankBattle->SetHtmlFromMemory( awcRankBattle, 1024*sizeof(WCHAR) );
		}
	}
}

VOID CStatusHonorTab::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventRBTotalScoreUpdate ||
		msg.Id == g_EventCharRankPointResetNfy )
	{
		Refresh();
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( !IsShow() )
			return;

		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_POINT_MUDOSA ||
			pUpdate->uiUpdateType & EVENT_AIUT_POINT_HONOR )
		{
			Refresh();
		}
	}
}

VOID CStatusHonorTab::OnClickedBtnExpandHonor( gui::CComponent* pComponent )
{
	m_bExpandHonor = TRUE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnReduceHonor( gui::CComponent* pComponent )
{
	m_bExpandHonor = FALSE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnExpandRankBattle( gui::CComponent* pComponent )
{
	m_bExpandRankBattle = TRUE;
	UpdateUI();
}

VOID CStatusHonorTab::OnClickedBtnReduceRankBattle( gui::CComponent* pComponent )
{
	m_bExpandRankBattle = FALSE;
	UpdateUI();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! StatusTechnicTab
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStatusTechnicTab::CStatusTechnicTab()
{

}

CStatusTechnicTab::~CStatusTechnicTab()
{

}

RwBool CStatusTechnicTab::IsShow( VOID )
{
	return m_pSelf->IsVisible();
}

VOID CStatusTechnicTab::Init()
{
	m_bHoipoiMixExpand = true;
}

RwBool CStatusTechnicTab::Create( CNtlPLGui* pParent )
{
	Init();

	m_pSelf = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgTechnic" ) );

	m_pBtnExpandHoipoiMix = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnHoipoiExpand") );
	m_pBtnExpandHoipoiMix->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_TITLE" ) );
	m_pBtnReduceHoipoiMix = reinterpret_cast<gui::CButton*>( pParent->GetComponent( "btnHoipoiReduce") );
	m_pBtnReduceHoipoiMix->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_TITLE" ) );

	m_slotClickedBtnExpandHoipoiMix = m_pBtnExpandHoipoiMix->SigClicked().Connect( this, &CStatusTechnicTab::OnClickedBtnExpandHoipoiMix );
	m_slotClickedBtnReduceHoipoiMix = m_pBtnReduceHoipoiMix->SigClicked().Connect( this, &CStatusTechnicTab::OnClickedBtnReduceHoipoiMix );

	m_pHoipoiMixDlg = reinterpret_cast<gui::CDialog*>( pParent->GetComponent( "dlgHoipoiMix" ) );
	
	m_pStbMixLevel = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixlevel" ) );
	m_pStbMixLevel->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_MIXLEVEL" ) );
	m_pStbMixLevelData = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixLevelData" ) );
	m_pStbMixExp = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixExp" ) );
	m_pStbMixExp->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_MIXEXPDATA" ) );
	m_pStbMixExpData = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbMixExpData" ) );
	m_pPgbExpData = reinterpret_cast<gui::CProgressBar*>( pParent->GetComponent( "pgbMixExpData" ) );
	m_pPgbExpData->SetRange( 0, 100 );

	m_pStbNormalMix = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbNormalMix" ) );
	m_pStbNormalMix->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_INFO" ) );
	m_pStbNormalRequireGuide = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbNormalRequireGuide" ) );
	m_pStbNormalRequireGuide->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_INFO_GUIDE" ) );
	m_pHtmlNormalGuide = reinterpret_cast<gui::CHtmlBox*>( pParent->GetComponent( "htmlNormalGuide" ) );
	m_pHtmlNormalGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_INFO_GUIDE_HTML" ),
		wcslen( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_INFO_GUIDE_HTML" ) ) );

	m_pStbSpecialMix = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbSpecialMix" ) );
	m_pStbSpecialMix->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL" ) );
	m_pStbSpecialRequireGuide = reinterpret_cast<gui::CStaticBox*>( pParent->GetComponent( "stbSpecialRequireGuide" ) );
	m_pStbSpecialRequireGuide->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE" ) );
	m_pHtmlSpecialGuide = reinterpret_cast<gui::CHtmlBox*>( pParent->GetComponent( "htmlSpecialGuide" ) );
	m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE_HTML" ),
		wcslen( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE_HTML" ) ) );

	m_pStbSpecialMix->Show(false);
	m_pStbSpecialRequireGuide->Show(false);
	m_pHtmlSpecialGuide->Show(false);


	UpdateUI();
	Refresh(NULL);

	return TRUE;
}

VOID CStatusTechnicTab::Destroy( VOID )
{

}

VOID CStatusTechnicTab::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventHoipoiMixItemMakeExpNfy)
	{
		Refresh(msg.pData);
	}
}

VOID CStatusTechnicTab::UpdateUI( VOID )
{
	m_pBtnExpandHoipoiMix->Show( !m_bHoipoiMixExpand );
	m_pBtnReduceHoipoiMix->Show( m_bHoipoiMixExpand );
	m_pHoipoiMixDlg->Show( m_bHoipoiMixExpand );
}

VOID CStatusTechnicTab::Refresh(void* pNotify)
{
	CNtlSobAvatar* pSobAvatar =  GetNtlSLGlobal()->GetSobAvatar();
	if( pSobAvatar )
	{
		CNtlSLHoipoiMix* pHoipoiMix = pSobAvatar->GetHoipoiMix();
		if( pHoipoiMix )
		{
			RwUInt8 byLevel = pHoipoiMix->GetMixLevel();

			sITEM_MIX_EXP_TBLDAT* pTblDat = (sITEM_MIX_EXP_TBLDAT*)API_GetTableContainer()->GetItemMixExpTable()->FindData(byLevel);
			if (!pTblDat)
			{
				NTL_ASSERT(false, "Not Exist sITEM_MIX_EXP_TBLDAT of index : " << (int)byLevel);
				return;
			}

			// set level
			m_pStbMixLevelData->SetText( byLevel );

			// set exp
			RwUInt32 uiExp = pHoipoiMix->GetMixExp();
			RwUInt32 uiMaxExp = pTblDat->dwNeedEXP;

			WCHAR awcBuffer[128];
			swprintf_s( awcBuffer, 128, L"%u / %u", uiExp, uiMaxExp );

			m_pStbMixExpData->SetText( awcBuffer );
			RwInt32 nPercent = 0;

			if( uiExp != 0 )
			{
				nPercent = (RwInt32)(( (RwReal)uiExp / (RwReal)uiMaxExp ) * 100.f );
				if( nPercent > 100 )
					nPercent = 100;
			}

			if (pNotify)
			{
				SNtlEventHoipoiMixItemMakeExpNfy* pInfo = (SNtlEventHoipoiMixItemMakeExpNfy*)pNotify;

				if (pInfo->dwExpGained > 0)
				{
					GetAlarmManager()->FormattedAlarmMessage("DST_NOTIFY_GAIN_MIX_EXP", FALSE, NULL, pInfo->dwExpGained);

					if (nPercent == 100)
						GetAlarmManager()->AlarmMessage("DST_NOTIFY_GAIN_MIX_EXP_MAX");
				}
			}
		
			m_pPgbExpData->SetPos( nPercent );

			m_pHtmlNormalGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML" ),
					wcslen( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML" ) ) );

			m_pHtmlSpecialGuide->SetHtmlFromMemory( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML" ),
				wcslen( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML" ) ) );
		}
	}
}

VOID CStatusTechnicTab::OnClickedBtnExpandHoipoiMix( gui::CComponent* pComponent )
{
	m_bHoipoiMixExpand = true;
	UpdateUI();
}

VOID CStatusTechnicTab::OnClickedBtnReduceHoipoiMix( gui::CComponent* pComponent )
{
	m_bHoipoiMixExpand = false;
	UpdateUI();
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! StatusStatsTab
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStatusStatsTab::CStatusStatsTab()
{

}

CStatusStatsTab::~CStatusStatsTab()
{
	Destroy();
}

RwBool CStatusStatsTab::IsShow(VOID)
{
	return m_pStatsDlg->IsVisible();
}

VOID CStatusStatsTab::Init()
{

}

RwBool CStatusStatsTab::Create(CNtlPLGui* pParent)
{
	Init();

	m_pStatsDlg = reinterpret_cast<gui::CDialog*>(pParent->GetComponent("dlgStats"));

	m_pOutDisplayStatsName = reinterpret_cast<gui::COutputBox*>(pParent->GetComponent("outputDisplayStatsName"));
	m_pOutDisplayStatsName->SetLineSpace(3);
	m_pOutDisplayStatsName->SetMaxLine(ATTRIBUTE_TO_UPDATE_COUNT);
	m_pOutDisplayStatsName->ShowScrollBar(false);

	m_pOutDisplayStatsValue = reinterpret_cast<gui::COutputBox*>(pParent->GetComponent("outputDisplayStatsValue"));
	m_pOutDisplayStatsValue->SetLineSpace(3);
	m_pOutDisplayStatsValue->SetMaxLine(ATTRIBUTE_TO_UPDATE_COUNT);

	m_pOutDisplayStatsName->Clear(); // if we dont do this, then it will crash after ~80 lines

	m_pOutDisplayStatsName->AddText("STR:");
	m_pOutDisplayStatsName->AddText("CON:");
	m_pOutDisplayStatsName->AddText("FOC:");
	m_pOutDisplayStatsName->AddText("DEX:");
	m_pOutDisplayStatsName->AddText("SOL:");
	m_pOutDisplayStatsName->AddText("ENG:");
	m_pOutDisplayStatsName->AddText("LP Regen:");
	m_pOutDisplayStatsName->AddText("LP Sitdown Regen:");
	m_pOutDisplayStatsName->AddText("LP Battle Regen:");
	m_pOutDisplayStatsName->AddText("EP Regen:");
	m_pOutDisplayStatsName->AddText("EP Sitdown Regen:");
	m_pOutDisplayStatsName->AddText("EP Battle Regen:");
	m_pOutDisplayStatsName->AddText("AP Regen:");
	m_pOutDisplayStatsName->AddText("AP Sitdown Regen:");
	m_pOutDisplayStatsName->AddText("AP Diminution:");
	m_pOutDisplayStatsName->AddText("RP Charge Speed:");
	m_pOutDisplayStatsName->AddText("RP Diminution:");
	m_pOutDisplayStatsName->AddText("Physical Offence:");
	m_pOutDisplayStatsName->AddText("Physical Defence:");
	m_pOutDisplayStatsName->AddText("Energy Offence:");
	m_pOutDisplayStatsName->AddText("Energy Defence:");
	m_pOutDisplayStatsName->AddText("Physical Armor Pen. Rate:");
	m_pOutDisplayStatsName->AddText("Energy Armor Pen. Rate:");
	m_pOutDisplayStatsName->AddText("Hit Rate:");
	m_pOutDisplayStatsName->AddText("Dodge Rate:");
	m_pOutDisplayStatsName->AddText("Block Rate:");
	m_pOutDisplayStatsName->AddText("Block Damage Rate:");
	m_pOutDisplayStatsName->AddText("Curse Success Rate:");
	m_pOutDisplayStatsName->AddText("Curse Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Physical Critical Rate:");
	m_pOutDisplayStatsName->AddText("Energy Critical Rate:");
	m_pOutDisplayStatsName->AddText("Physical Critical Damage Rate:");
	m_pOutDisplayStatsName->AddText("Energy Critical Damage Rate:");
	m_pOutDisplayStatsName->AddText("Walk Speed:");
	m_pOutDisplayStatsName->AddText("Run Speed:");
	m_pOutDisplayStatsName->AddText("Fly Speed:");
	m_pOutDisplayStatsName->AddText("Flying Dash Speed:");
	m_pOutDisplayStatsName->AddText("Flying Accel Speed:");
	m_pOutDisplayStatsName->AddText("Attack Speed:");
	m_pOutDisplayStatsName->AddText("Attack Range:");
	m_pOutDisplayStatsName->AddText("Change Casting Time ");
	m_pOutDisplayStatsName->AddText("Change Cool Time ");
	m_pOutDisplayStatsName->AddText("Keep Time: ");
	m_pOutDisplayStatsName->AddText("DOT Time Change:");
	m_pOutDisplayStatsName->AddText("Change Required EP: ");
	m_pOutDisplayStatsName->AddText("Physical Reflection:");
	m_pOutDisplayStatsName->AddText("Energy Diminution:");
	m_pOutDisplayStatsName->AddText("Paralyze Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Terror Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Confusion Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Stone Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Candy Tolerance Rate:");
	m_pOutDisplayStatsName->AddText("Paralyze Time Down:");
	m_pOutDisplayStatsName->AddText("Terror Time Down:");
	m_pOutDisplayStatsName->AddText("Confusion Time Down:");
	m_pOutDisplayStatsName->AddText("Stone Time Down:");
	m_pOutDisplayStatsName->AddText("Candy Time Down:");
	m_pOutDisplayStatsName->AddText("Bleed Time Down:");
	m_pOutDisplayStatsName->AddText("Poison Time Down:");
	m_pOutDisplayStatsName->AddText("Stomachache Time Down:");
	m_pOutDisplayStatsName->AddText("Block Critical Rate %:");
	m_pOutDisplayStatsName->AddText("LP Recover On Hit:");
	m_pOutDisplayStatsName->AddText("LP Recover On Hit %:");
	m_pOutDisplayStatsName->AddText("EP Recover On Hit:");
	m_pOutDisplayStatsName->AddText("EP Recover On Hit %:");
	m_pOutDisplayStatsName->AddText("Stomachache Defence:");
	m_pOutDisplayStatsName->AddText("Poison Defence:");
	m_pOutDisplayStatsName->AddText("Bleed Defence:");
	m_pOutDisplayStatsName->AddText("Burn Defence:");
	m_pOutDisplayStatsName->AddText("Mind Curse Immunity %:");
	m_pOutDisplayStatsName->AddText("Body Curse Immunity %:");
	m_pOutDisplayStatsName->AddText("Change Curse Immunity %:");
	m_pOutDisplayStatsName->AddText("Skill Animation Speed %:");
	m_pOutDisplayStatsName->AddText("Weight Limit:");
	m_pOutDisplayStatsName->AddText("Aggro Bonus:");
	m_pOutDisplayStatsName->AddText("Aggro Bonus %:");
	m_pOutDisplayStatsName->AddText("Heal Bonus:");
	m_pOutDisplayStatsName->AddText("Heal Bonus %:");
	m_pOutDisplayStatsName->AddText("Heal over Time Bonus:");
	m_pOutDisplayStatsName->AddText("Heal over Time Bonus %:");
	m_pOutDisplayStatsName->AddText("Damage over Time Bonus %:");
	m_pOutDisplayStatsName->AddText("Guard Rate:");
	m_pOutDisplayStatsName->AddText("Block Skill Damage %:");
	m_pOutDisplayStatsName->AddText("Block Curse Success %:");
	m_pOutDisplayStatsName->AddText("Block Knockdown Success %:");
	m_pOutDisplayStatsName->AddText("Block HTB Success %:");
	m_pOutDisplayStatsName->AddText("Item Upgrade Rate %:");
	m_pOutDisplayStatsName->AddText("EXP Boost %:");
	m_pOutDisplayStatsName->AddText("Quest Drop Rate %:");

	m_slotWheelMoveStats = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect(this, &CStatusStatsTab::OnCaptureWheelMove); 
	m_slotSliderValueChanged = m_pOutDisplayStatsValue->GetVerticalScrollBar()->SigValueChanged().Connect(this, &CStatusStatsTab::OnScrollChanged);
	m_slotSliderMoved = m_pOutDisplayStatsValue->GetVerticalScrollBar()->SigSliderMoved().Connect(this, &CStatusStatsTab::OnScrollChanged);

	return TRUE;
}

VOID CStatusStatsTab::Destroy(VOID)
{
	GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Disconnect(m_slotWheelMoveStats);
}

VOID CStatusStatsTab::HandleEvents(RWS::CMsg& msg)
{
	
}

VOID CStatusStatsTab::Refresh(VOID)
{
	UpdateStatData();

	m_pOutDisplayStatsName->FirstLine();
	m_pOutDisplayStatsValue->FirstLine();
}

VOID CStatusStatsTab::UpdateStatData(VOID)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if (!pAvatar)
		return;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());

	m_pOutDisplayStatsValue->Clear();

	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Str);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Con);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Foc);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Dex);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Sol);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_Eng);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wLpRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wLpSitdownRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wLpBattleRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEpRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEpSitdownRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEpBattleRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wApRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wApSitdownRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wApDegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wRpRegen);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wRpDimimutionRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wPhysicalOffence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wPhysicalDefence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEnergyOffence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEnergyDefence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_fPhysicalArmorPenRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_fEnergyArmorPenRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wAttackRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wDodgeRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wBlockRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wBlockDamageRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wCurseSuccessRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wCurseToleranceRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wPhysicalCriticalRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wEnergyCriticalRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fPhysicalCriticalDamageRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fEnergyCriticalDamageRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetWalkSpeed());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetRunSpeed());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetFlySpeed());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetFlyDashSpeed());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetFlyAccelSpeed());
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wAttackSpeedRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetAttackRange());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetCastingTimeModifier());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetCoolingTimeModifier());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetKeepingTimeModifier());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetDOTTimeModifier());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetRequiredEPModifier());
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fPhysicalReflection);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fEnergyReflection);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wParalyzeToleranceRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wTerrorToleranceRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wConfuseToleranceRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wStoneToleranceRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wCandyToleranceRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fParalyzeKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fTerrorKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fConfuseKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fStoneKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fCandyKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fBleedingKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fPoisonKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fStomachacheKeepTimeDown);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fCriticalBlockSuccessRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_dwLpRecoveryWhenHit);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fLpRecoveryWhenHitInPercent);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_dwEpRecoveryWhenHit);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fEpRecoveryWhenHitInPercent);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wStomachacheDefence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wPoisonDefence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wBleedDefence);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wBurnDefence);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fMindCurseImmunity);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fBodyCurseImmunity);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fChangeCurseImmunity);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetSkillAnimationSpeedModifier());
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_dwWeightLimit);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fSkillAggroBonus);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fSkillAggroBonusInPercent);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fDirectHealPowerBonus);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fDirectHealPowerBonusInPercent);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fHotPowerBonus);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fHotPowerBonusInPercent);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->GetDOTValueModifier());
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_wGuardRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fSkillDamageBlockModeSuccessRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fCurseBlockModeSuccessRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fKnockdownBlockModeSuccessRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fHtbBlockModeSuccessRate);
	m_pOutDisplayStatsValue->Format("%.2f", pAvatarAttr->m_fItemUpgradeBonusRate);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_byExpBooster);
	m_pOutDisplayStatsValue->Format("%u", pAvatarAttr->m_byQuestDropRate);
}

VOID CStatusStatsTab::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if (!m_pStatsDlg || !IsShow())
		return;

	//if (m_pStatsDlg->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pStatsDlg)
	//	return;

	if (m_pStatsDlg->PosInRect(pos.x, pos.y) != gui::CComponent::INRECT)
		return;

	if (sDelta < 0)
	{
		m_pOutDisplayStatsName->NextLine();
		m_pOutDisplayStatsValue->NextLine();
	}
	else
	{
		m_pOutDisplayStatsName->PrevLine();
		m_pOutDisplayStatsValue->PrevLine();
	}
}

VOID CStatusStatsTab::OnScrollChanged(RwInt32 nNewOffset)
{
	m_pOutDisplayStatsName->SetLine(nNewOffset);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! StatusWnd
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// ! Construction & Destruction

CStatusWindowGui::CStatusWindowGui(VOID)
{
	m_pstbTitle = NULL;
	m_ptabStatus = NULL;
	m_pbtnExit = NULL;

	m_pAvatarTab = NULL;
	m_pHonorTab = NULL;
	m_pTechnicTab = NULL;
}

CStatusWindowGui::CStatusWindowGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_pstbTitle = NULL;
	m_ptabStatus = NULL;
	m_pbtnExit = NULL;

	m_pAvatarTab = NULL;
	m_pHonorTab = NULL;
	m_pTechnicTab = NULL;

	m_nCurrentTab = -1;
}

CStatusWindowGui::~CStatusWindowGui(VOID)
{
}

////////////////////////////////////////////////////////////////////////////////
// ! Operation
RwBool CStatusWindowGui::Create(VOID)
{
	NTL_FUNCTION( "CStatusWindowGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\StatusWnd.srf", "gui\\StatusWnd.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	
	m_pstbTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbTitle" ) );
	m_ptabStatus = reinterpret_cast<gui::CTabButton*>( GetComponent( "tabStatus" ) );
	m_pbtnExit = reinterpret_cast<gui::CButton*>( GetComponent( "btnClose" ) );
	m_pBtnHelp = reinterpret_cast<gui::CButton*>( GetComponent( "btnHelp" ) );

	m_slotHelp = m_pBtnHelp->SigClicked().Connect( this, &CStatusWindowGui::OnClickedHelp );

	m_slotSelectTab = m_ptabStatus->SigSelectChanged().Connect( this, &CStatusWindowGui::OnSelectedTab );
	m_slotExit = m_pbtnExit->SigClicked().Connect( this, &CStatusWindowGui::OnClickedExit );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CStatusWindowGui::OnAbsolulteClick );
	
	m_pAvatarTab = NTL_NEW CStatusAvatarTab;
	NTL_ASSERT( m_pAvatarTab, "CStatusWindowGui::Create() : Alloc Fail" );
	if( !m_pAvatarTab->Create( this ) )
	{
		m_pAvatarTab->Destroy();

		NTL_DELETE( m_pAvatarTab );

		NTL_RETURN( FALSE );
	}

	m_pHonorTab = NTL_NEW CStatusHonorTab;
	NTL_ASSERT( m_pHonorTab, "CStatusWindowGui::Create() : Alloc Failed" );
	if( !m_pHonorTab->Create( this ) )
	{
		m_pHonorTab->Destroy();

		NTL_DELETE( m_pHonorTab );

		NTL_RETURN( FALSE );
	}

	m_pTechnicTab = NTL_NEW CStatusTechnicTab;
	NTL_ASSERT( m_pTechnicTab, "CStatusWindowGui::Create() : Alloc Failed" );
	if( !m_pTechnicTab->Create( this ) )
	{
		m_pTechnicTab->Destroy();

		NTL_DELETE( m_pTechnicTab );

		NTL_RETURN( FALSE );
	}

	m_pStatsTab = NTL_NEW CStatusStatsTab;
	NTL_ASSERT(m_pStatsTab, "CStatusWindowGui::Create() : Alloc Failed");
	if (!m_pStatsTab->Create(this))
	{
		m_pStatsTab->Destroy();

		NTL_DELETE(m_pStatsTab);

		NTL_RETURN(FALSE);
	}

	// Initial Setting
	SetBasicUISetting();
	m_pThis->Show( false );

	// serialize setting
	EnableSerialize(TRUE);

	// Update 연결
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );
	
	// Event Link
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventPickedUpHide, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg(g_EventSobEquipChange, (const char*)0, 0x7000);
	LinkMsg(g_EventTLNotifyLockUnlock, 0);
	LinkMsg(g_EventChangeWorldConceptState, 0);
	LinkMsg(g_EventSobConvertClass);
	LinkMsg(g_EventChangeClassAuthorityChangedNfy);
	LinkMsg(g_EventRBTotalScoreUpdate);
	LinkMsg(g_EventCharRankPointResetNfy);
	LinkMsg(g_EventHoipoiMixItemMakeExpNfy);
	LinkMsg(g_EventCharTitleSelectRes);
	LinkMsg(g_EventCharTitleUpdate);

	NTL_RETURN( TRUE );
}

VOID CStatusWindowGui::Destroy(VOID)
{
	NTL_FUNCTION( "CStatusWindowGui::Destroy" );

	// Event Unlink
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventPickedUpHide );
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg(g_EventSobEquipChange);
	UnLinkMsg(g_EventTLNotifyLockUnlock);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventSobConvertClass);
	UnLinkMsg(g_EventChangeClassAuthorityChangedNfy);
	UnLinkMsg(g_EventRBTotalScoreUpdate);
	UnLinkMsg(g_EventCharRankPointResetNfy);
	UnLinkMsg(g_EventHoipoiMixItemMakeExpNfy);
	UnLinkMsg(g_EventCharTitleSelectRes);
	UnLinkMsg(g_EventCharTitleUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	// Update 해제
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );

	NTL_DELETE(m_pAvatarTab);
	NTL_DELETE(m_pHonorTab);
	NTL_DELETE(m_pTechnicTab);
	NTL_DELETE(m_pStatsTab);

	NTL_RETURNVOID();
}

VOID CStatusWindowGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( m_nCurrentTab == TAB_AVATAR )
		m_pAvatarTab->UpdateBeforeCamera( fElapsed );	
}

VOID CStatusWindowGui::HandleEvents( RWS::CMsg& msg )
{
	if( m_nCurrentTab == TAB_AVATAR )
		m_pAvatarTab->HandleEvents( msg );
	else if( m_nCurrentTab == TAB_HONOR )
		m_pHonorTab->HandleEvents( msg );
	else if( m_nCurrentTab == TAB_TECHNIC )
		m_pTechnicTab->HandleEvents( msg );
	else if (m_nCurrentTab == TAB_STATS)
		m_pStatsTab->HandleEvents(msg);

	if( msg.Id == g_EventTLNotifyLockUnlock )
	{
		SNtlEventTLLockUnlcok* pEvent = reinterpret_cast<SNtlEventTLLockUnlcok*>( msg.pData );

		if( pEvent->byLockType != TLLOCK_MOUSE )
			return;

		if( pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_STATUS_WND_TAB && 
			pEvent->byIndex != ETL_MOUSE_INPUT_TYPE_ALL )
			return;

		for( RwUInt8 i = 0 ; i < m_ptabStatus->GetButtonCount() ; ++i )
			m_ptabStatus->EnableTab(i, !pEvent->bLock);
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept == WORLD_PLAY_TUTORIAL )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				for( RwUInt8 i = 0 ; i < m_ptabStatus->GetButtonCount() ; ++i )
					m_ptabStatus->EnableTab(i, true);
			}
		}
	}
	else if( msg.Id == g_EventChangeClassAuthorityChangedNfy )
	{
		SNtlEventChangeClassAuthorityChanged* pData = reinterpret_cast<SNtlEventChangeClassAuthorityChanged*>( msg.pData );

		if( pData->bCanChangeClass )
			GetAlarmManager()->AlarmMessage( "DST_CHAR_CAN_CHANGE_CLASS" );
	}
	else if (msg.Id == g_EventCharTitleSelectRes)
	{
		SDboEventCharTitleSelectRes* pData = reinterpret_cast<SDboEventCharTitleSelectRes*>(msg.pData);

		m_pAvatarTab->SelectCharTitle(pData->uiTitle);
	}
	else if (msg.Id == g_EventCharTitleUpdate)
	{
		SDboEventCharTitleUpdate* pData = reinterpret_cast<SDboEventCharTitleUpdate*>(msg.pData);

		m_pAvatarTab->UpdateCharTitle(pData->uiTitle, pData->bDelete);
	}
}

RwInt32 CStatusWindowGui::SwitchDialog( bool bOpen )
{
	if( bOpen )
	{
		RaiseTop();
		Show( true );
		m_ptabStatus->SelectTab( (RwInt32)TAB_AVATAR );
		SelectTab( TAB_AVATAR );
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_OPEN);
	}
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == DIALOG_STATUS )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		Show( false );		
		Logic_SendTutorialCondition(ETL_CONDITION_TYPE_STATUS_CLOSE);
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// ! Implementation

VOID CStatusWindowGui::SetBasicUISetting(VOID)
{
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_STATUS_WINDOW_TITLE" ) );

	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_STATUS_TAB_AVATAR" ) ) );
	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_STATUS_TAB_FAMILY" ) ) );
	m_ptabStatus->AddTab( std::wstring( GetDisplayStringManager()->GetString( "DST_STATUS_TECHNIC_TITLE" ) ) );

	// temporary
	std::wstring wstrStatsTitle = L"Stats";
	m_ptabStatus->AddTab(wstrStatsTitle);

	m_ptabStatus->SelectTab( (RwInt32)TAB_AVATAR );		
	SelectTab( TAB_AVATAR );
}

VOID CStatusWindowGui::SelectTab( RwInt32 nIndex )
{
	switch( nIndex )
	{
	case TAB_AVATAR:
		m_pAvatarTab->Show(true);
		m_pAvatarTab->Refresh();
		m_pHonorTab->Show(false);
		m_pTechnicTab->Show( false );
		m_pStatsTab->Show(false);
		break;
	case TAB_HONOR:
		m_pAvatarTab->Show( false );
		m_pHonorTab->Show(true);
		m_pHonorTab->Refresh();
		m_pTechnicTab->Show( false );
		m_pStatsTab->Show(false);
		break;
	case TAB_TECHNIC:
		m_pAvatarTab->Show( false );
		m_pHonorTab->Show( false );
		m_pTechnicTab->Show(true);
		m_pTechnicTab->Refresh(NULL);
		m_pStatsTab->Show(false);
		break;
	case TAB_STATS:
		m_pAvatarTab->Show(false);
		m_pHonorTab->Show(false);
		m_pTechnicTab->Show(false);
		m_pStatsTab->Show(true);
		m_pStatsTab->Refresh();
		break;
	}

	m_nCurrentTab = nIndex;
}

////////////////////////////////////////////////////////////////////////////////
// ! Callbacks
VOID CStatusWindowGui::OnClickedExit( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_STATUS_WND_CLOSE_BTN ) )
		return;

	GetDialogManager()->SwitchDialog( DIALOG_STATUS );
}

VOID CStatusWindowGui::OnSelectedTab( RwInt32 nSelectIndex, RwInt32 nPreviousIndex )
{
	SelectTab( nSelectIndex );
}

VOID CStatusWindowGui::OnAbsolulteClick( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_STATUS, key.m_fX, key.m_fY );
}

VOID CStatusWindowGui::OnClickedHelp( gui::CComponent* pComponent )
{
	CDboEventGenerator::OpenHelpContent( DIALOG_STATUS );
}

