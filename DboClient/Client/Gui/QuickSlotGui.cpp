#include "precomp_dboclient.h"
#include "QuickSlotGui.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLDef.h"

// sharde
#include "NtlSkill.h"
#include "NtlResultCode.h"

// Simulation
#include "NtlSLDef.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"
#include "NtlSkillContainer.h"
#include "NtlSobAvatar.h"
#include "NtlSob.h"
#include "NtlSobIcon.h"
#include "NtlSobItem.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobActionSkill.h"
#include "NtlSobItemAttr.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobActionSkillAttr.h"
#include "InputActionMap.h"
#include "ItemTable.h"
#include "NtlSobProxy.h"

// Table
#include "ItemTable.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ActionTable.h"
#include "UseItemTable.h"

// Dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "PopupManager.h"
#include "InfoWndManager.h"
#include "ChatGui.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"


////////////////////////////////////////////////
// Info:
/*
	State: CHECK_FLAG_TARGETAPPLY disabled, so icons are shown as active. Since we added the "Select target" option when click on skills, it is no longer required to have this flag.

*/

////////////////////////////////////////////////////////////////////////////////
// ! CQuickslotItem

CQuickSlotItem::CQuickSlotItem(VOID)
{
	m_eEnableState = ENABLE;
	m_flagUseableCheck = 0;
	m_uiSLClassID = INVALID_BYTE;
	m_byQuickSlotID = INVALID_BYTE;
}

CQuickSlotItem::~CQuickSlotItem(VOID)
{

}

CNtlSobIcon* CQuickSlotItem::GetFirstIcon(VOID)
{
	CNtlSobIcon* pIcon = NULL;
	LIST_QUICKSLOTOBJ_ITER iter;

	for( iter = m_listObj.begin() ; iter != m_listObj.end() ; ++iter )
	{
		stQUICKSLOTOBJ* pObj = &(*iter);
		
		if( IsItem() )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pObj->pIcon->GetSobObj()->GetSobAttr() );

			if( pItemAttr->IsNeedToIdentify() )
				continue;
			else
			{
				pIcon = pObj->pIcon;
			}
		}
		else
		{
			pIcon = pObj->pIcon;
		}
		
	}
	
	return pIcon;
}

RwUInt32 CQuickSlotItem::GetFirstSerialID(VOID)
{
	RwUInt32 hSerial = INVALID_SERIAL_ID;
	LIST_QUICKSLOTOBJ_ITER iter;

	for( iter = m_listObj.begin() ; iter != m_listObj.end() ; ++iter )
	{
		stQUICKSLOTOBJ* pObj = &(*iter);

		if( IsItem() )
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pObj->pIcon->GetSobObj()->GetSobAttr() );

			if( pItemAttr->IsNeedToIdentify() )
				continue;
			else
			{
				hSerial = pObj->hSerial;
			}
		}
		else
		{
			hSerial = pObj->hSerial;
		}
	}

	return hSerial;
}

RwInt32 CQuickSlotItem::GetStackCount(VOID)
{
	if( !IsItem() )
		return 0;

	LIST_QUICKSLOTOBJ_ITER iter;
	RwInt32 nStackCount = 0;

	for( iter = m_listObj.begin() ; iter != m_listObj.end() ; ++iter )
	{
		stQUICKSLOTOBJ* pObj = &(*iter);
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pObj->pIcon->GetSobObj()->GetSobAttr() );

		if( pItemAttr->IsNeedToIdentify() )
			continue;

		nStackCount += (int)pItemAttr->GetStackNum();
	}

	return nStackCount;
}

RwBool CQuickSlotItem::IsValid(VOID)
{
	if( m_listObj.size() )
		return TRUE;
	else
		return FALSE;
}

RwBool CQuickSlotItem::IsItem(VOID)
{
	if( m_uiSLClassID == SLCLASS_ITEM_ICON )
		return TRUE;
	else
		return FALSE;	
}

CQuickSlotItem::stQUICKSLOTOBJ* CQuickSlotItem::HasObj( RwUInt32 hSerialID )
{
	LIST_QUICKSLOTOBJ_ITER iter;
	
	for( iter = m_listObj.begin() ; iter != m_listObj.end() ; ++iter )
	{
		stQUICKSLOTOBJ* pObj = &(*iter);
		if( pObj->hSerial == hSerialID )
			return pObj;
	}

	return NULL;
}

VOID CQuickSlotItem::SetItem( RwUInt32 hSerialID, CNtlSobIcon* pIcon, RwUInt8 byQuickSlotID )
{
	stQUICKSLOTOBJ stOBJ;
	stOBJ.hSerial = hSerialID;
	stOBJ.pIcon = pIcon;

	m_listObj.push_back( stOBJ );

	m_uiSLClassID = pIcon->GetClassID();
	m_byQuickSlotID = byQuickSlotID;

	NTL_ASSERT( byQuickSlotID >= 0 && byQuickSlotID < 48, "CQuickSlotItem::SetItem : SlotID Error!(" << byQuickSlotID << ")" );	
}

RwBool CQuickSlotItem::UnSetItem( RwUInt32 hSerialID )
{
	LIST_QUICKSLOTOBJ_ITER iter;

	for( iter = m_listObj.begin() ; iter != m_listObj.end() ; ++iter )
	{
		stQUICKSLOTOBJ* pObj = &(*iter);
		
		if( pObj->hSerial == hSerialID )
		{
			m_listObj.erase( iter );			
			
			if( IsValid() )
			{
				stQUICKSLOTOBJ* pNextObj = &(*m_listObj.begin());

				if( IsItem() )
				{
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( pNextObj->pIcon->GetSobObj() );
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

					GetDboGlobal()->GetGamePacketGenerator()->SendSetIconToQuickSlot( pItemAttr->GetTblIdx(), m_byQuickSlotID, QUICK_SLOT_TYPE_ITEM, pItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1, pItem->GetItemSlotIdx() );
				}				
				return FALSE;
			}
			else
			{
				m_eEnableState = ENABLE;
				m_flagUseableCheck = 0;
				m_uiSLClassID = INVALID_BYTE;
				m_byQuickSlotID = INVALID_BYTE;
				return TRUE;
			}

		}	
	}

	if( IsValid() )
		return FALSE;
	else
		return TRUE;
}

VOID CQuickSlotItem::UnSetItem(VOID)
{
	LIST_QUICKSLOTOBJ_ITER iter;

	m_listObj.clear();
	m_eEnableState = ENABLE;
	m_flagUseableCheck = 0;
	m_uiSLClassID = INVALID_BYTE;
	m_byQuickSlotID = INVALID_BYTE;
}

VOID CQuickSlotItem::operator = ( CQuickSlotItem &QuickSlotItem )
{
	m_listObj = QuickSlotItem.m_listObj;
	m_eEnableState = QuickSlotItem.m_eEnableState;
	m_flagUseableCheck = QuickSlotItem.m_flagUseableCheck;
	m_uiSLClassID = QuickSlotItem.m_uiSLClassID;
	m_byQuickSlotID = QuickSlotItem.m_byQuickSlotID;
}

////////////////////////////////////////////////////////////////////////////////
// ! Define

#define QUICK_STACK_WIDTH	10
#define QUICK_STACK_HEIGHT	10
#define QUICK_RPTYPE_OFFSET	21

#define SLOTID_TO_ROW(id)	(id/QUICKSLOT_MAXCOL)
#define SLOTID_TO_COL(id)	(id%QUICKSLOT_MAXCOL)
#define ROWCOL_TO_SLOTID(row,col)	(row*QUICKSLOT_MAXCOL+col)

////////////////////////////////////////////////////////////////////////////////
// ! Constructor & Destructor

CQuickSlotGui::CQuickSlotGui(VOID)
{
	Init();
}

CQuickSlotGui::CQuickSlotGui( const RwChar *pName )
: CNtlPLGui( pName )
{
	Init();
}

CQuickSlotGui::~CQuickSlotGui(VOID)
{
	
}

////////////////////////////////////////////////////////////////////////////////
// ! Operation

VOID CQuickSlotGui::Init(VOID)
{
	pEffectRange = NULL;
	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			m_anFocusEffect[j][i] = SLOT_FOCUS_NONE;
			m_apstbStackNum[j][i] = NULL;
			m_apflaEffect[j][i] = NULL;
			m_abPushDownKey[j][i] = FALSE;
			m_abIsProcDownKey[j][i] = FALSE;

			// 퀵슬롯의 상태들
			m_afPushDownKeyElapsed[j][i] = 0.0f;
			m_abEnableIgnoreUp[j][i] = FALSE;
		}
	}	
	
	m_nCurrentRow = 0;
	m_nVisibleRowCount = 1;

	m_nLSelectedSlotIdx = -1;
	m_nRSelectedSlotIdx = -1;
	m_nMouseOnIndex = -1;
	m_nPushDownIndex = -1;
	
	m_pUpRowBtn = NULL;
	m_pDownRowBtn = NULL;
	/*m_ppnlAlt = NULL;
	m_ppnlCtrl = NULL;*/

	m_bAttackMode = FALSE;
	m_uiEffectFrame = 0;

	m_bLock = TRUE;
	m_bIconMoveLock = FALSE;

	m_nEnableIgnoreIndex = -1;					
	m_fPushDownMouseElapsed = 0.0f;				
	m_bIsProcMouseSelected = NULL;
}

RwBool CQuickSlotGui::Create(VOID)
{
	NTL_FUNCTION("CQuickSlotGui::Create");

	// 퀵슬롱세 관련된 Component 생성
	if( !CNtlPLGui::Create( "", "gui\\QuickSlot.srf", "gui\\QuickSlot.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	// 퀵슬롯을 구성할 때 콜백이 필요한 컴포넌트들을 가져오고
	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pUpRowBtn = (gui::CButton*)GetComponent( "btnSlotPrev" );
	m_pDownRowBtn = (gui::CButton*)GetComponent( "btnSlotNext" );
	m_pRowNum = (gui::CStaticBox*)GetComponent( "stbNum" );

	// 연결해준다.
	m_slotUpRowClick = m_pUpRowBtn->SigClicked().Connect( this, &CQuickSlotGui::OnClickedUpRow );
	m_slotDownRowClick = m_pDownRowBtn->SigClicked().Connect( this, &CQuickSlotGui::OnClickedDownRow );
	m_slotMouseUp = m_pThis->SigMouseUp().Connect( this, &CQuickSlotGui::OnMouseUp );
	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CQuickSlotGui::OnMouseDown );
	m_slotMouseMove = m_pThis->SigMouseMove().Connect( this, &CQuickSlotGui::OnMouseMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CQuickSlotGui::OnPaint );
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect( this, &CQuickSlotGui::OnMouseLeave );
	m_slotMove = m_pThis->SigMove().Connect( this, &CQuickSlotGui::OnMove );

	// QuickSlot CoordSet
	SetSlotRectHardCode();
	m_pRowNum->SetText( m_nCurrentRow + 1 );

	// 퀵슬롯 단축키의 이름을 생성하고 등록한다.
	// 퀵슬롯 Index를 초기화한다.
	m_anQuickSlotRow[ROW_DEFAULT] = 2;
	m_anQuickSlotRow[ROW_EX] = 1;
	m_anQuickSlotRow[ROW_EX2] = 0;

	CreateShortCutKeyName();
	RegisterShortCutkeyName();

	// InputMap Setting
	LinkActionMap();

	// Update 연결
	GetNtlGuiManager()->AddUpdateFunc( this );

	// Event
//	LinkMsg( g_EventIconPopupResult, 0 );
	LinkMsg( g_EventIconMoveClick, 0 );
	LinkMsg( g_EventSobInfoUpdate, 0 );
	LinkMsg( g_EventSobCooling, 0 );
	LinkMsg( g_EventSobItemAdd, 0, 0x7000 );	// 아이템이 생성된후 이벤트를 받는다.
	LinkMsg( g_EventSobItemDelete, 0 );
	LinkMsg( g_EventQuickSlotInfo, 0 );
	LinkMsg( g_EventSobDeleteQuickSlotIcon, 0 );
	LinkMsg( g_EventEnableItemIcon, 0 );
	LinkMsg( g_EventAvatarTargetStateChange, 0 );
	LinkMsg( g_EventSobTargetInfoUpdate, 0 );
	LinkMsg( g_EventChangeWorldConceptState );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventCapsuleLockItemWithoutBag, 0 );
	LinkMsg( g_EventQuickSlotGuiMode );
	LinkMsg( g_EventQuickSlotLockMode );
	LinkMsg( g_EventActionMapLoadInfo );
	LinkMsg( g_EventActionMapUpdateRes );
	LinkMsg( g_EventQuickSlotDelNfy );
	LinkMsg( g_EventSobAttackMode );
	LinkMsg( g_EventTransform );
	LinkMsg(g_EventSobServerUpdateState, 0, 0x7900);
	LinkMsg(g_EventSobServerUpdateCondition, 0, 0x7900);
	LinkMsg(g_EventSkillSelectMode);
	
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );

	NTL_RETURN(TRUE);
}

void CQuickSlotGui::Destroy(void)
{
	NTL_FUNCTION("CQuickSlotGui::Destroy");

	ClearQuickSlot();

	DestroyShortCutKeyName();

//	UnLinkMsg( g_EventIconPopupResult );
	UnLinkMsg( g_EventIconMoveClick );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventSobCooling );
	UnLinkMsg( g_EventSobItemAdd );
	UnLinkMsg( g_EventSobItemDelete );
	UnLinkMsg( g_EventQuickSlotInfo );
	UnLinkMsg( g_EventSobDeleteQuickSlotIcon );
	UnLinkMsg( g_EventEnableItemIcon );
	UnLinkMsg( g_EventAvatarTargetStateChange );
	UnLinkMsg( g_EventSobTargetInfoUpdate );
	UnLinkMsg( g_EventChangeWorldConceptState );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventCapsuleLockItemWithoutBag );
	UnLinkMsg( g_EventQuickSlotGuiMode );
	UnLinkMsg( g_EventQuickSlotLockMode );
	UnLinkMsg( g_EventActionMapLoadInfo );
	UnLinkMsg( g_EventActionMapUpdateRes );
	UnLinkMsg( g_EventQuickSlotDelNfy );
	UnLinkMsg( g_EventSobAttackMode );
	UnLinkMsg( g_EventTransform );
	UnLinkMsg(g_EventSobServerUpdateState);
	UnLinkMsg(g_EventSobServerUpdateCondition);
	UnLinkMsg(g_EventSkillSelectMode);

	UnLinkActionMap();

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CQuickSlotGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CQuickSlotGui::Update( RwReal fElapsed )
{
	if (pEffectRange)
		pEffectRange->Update(fElapsed);

	for( int i = 0 ; i < m_nVisibleRowCount ; ++i )
	{
		for( int j = 0 ; j < QUICKSLOT_MAXCOL; ++j )
		{
			int nDataRow = GetDataRowIndex( i );				

			if( m_CoolTimeEffect[i][j].IsWork())
			{
				RwReal fCurrentTime = 0.0f;				
				fCurrentTime = m_aQuickSlotItem[nDataRow][j].GetFirstIcon()->GetCoolingTime();
				m_CoolTimeEffect[i][j].Update( fCurrentTime );
			}

			if( m_abPushDownKey[i][j] )
			{
				m_afPushDownKeyElapsed[i][j] += fElapsed;
				if( m_afPushDownKeyElapsed[i][j] > 0.5f )
				{
					// 시간 초기화
					m_afPushDownKeyElapsed[i][j] = 0.0f;

					SERIAL_HANDLE hSerial = m_aQuickSlotItem[nDataRow][j].GetFirstSerialID();
					RwInt32 nEnableState = m_aQuickSlotItem[nDataRow][j].GetEnableState();

					if( hSerial == INVALID_SERIAL_ID || nEnableState != CQuickSlotItem::ENABLE )
						continue;

					// 다운 되고 있을 때 한번 실행됐다면 해당 슬롯의 KeyDown, KeyUp이 일어나기 전까지는 Flag로 체크하여
					// 스킬 중복 사용을 막는다.
					if( !m_abIsProcDownKey[i][j] )
					{
						m_abEnableIgnoreUp[i][j] = Logic_UseProcRpBonusSkill( hSerial );
						m_abIsProcDownKey[i][j] = TRUE;
					}
				}
			}

			if( m_apflaEffect[i][j]->IsVisible() )
			{
				m_apflaEffect[i][j]->Update( fElapsed );	
				m_uiEffectFrame = m_apflaEffect[i][j]->GetCurrentFrame();
			}
		}
	}
	
	// 현재 마우스로 다운한 인덱스가 0보다 크다면 ( 유효하다면 )
	// 마우스 시간을 업데이트한다.
	if( m_nRSelectedSlotIdx >= 0 && m_nMouseOnIndex == m_nRSelectedSlotIdx )
	{
		m_fPushDownMouseElapsed += fElapsed;
		if( m_fPushDownMouseElapsed > 0.5f )
		{
			// 시간 초기화
			m_fPushDownMouseElapsed = 0.0f;

			RwInt32 nVisibleRow = SLOTID_TO_ROW( m_nRSelectedSlotIdx );
			RwInt32 nCol = SLOTID_TO_COL( m_nRSelectedSlotIdx );
			RwInt32 nDataRow = GetDataRowIndex( nVisibleRow );

			if( !IsValidRowIndex( nDataRow ) )
				return;

			SERIAL_HANDLE hSerial = m_aQuickSlotItem[nDataRow][nCol].GetFirstSerialID();
			RwInt32 nEnableState = m_aQuickSlotItem[nDataRow][nCol].GetEnableState();

			if( hSerial == INVALID_SERIAL_ID || nEnableState != CQuickSlotItem::ENABLE )
				return;

			// 다운 되고 있을 때 한번 실행됐다면 해당 슬롯에 다시 MosueSlot 이 일어나기 전까지는 Flag로 체크하여
			// 스킬 중복 사용을 막는다.
			if( !m_bIsProcMouseSelected )
			{
				m_bIsProcMouseSelected = TRUE;

				if( Logic_UseProcRpBonusSkill( hSerial ) )
				{
					m_nEnableIgnoreIndex = m_nRSelectedSlotIdx;
				}
				else
					m_nEnableIgnoreIndex = -1;
			}
		}
	}
}

VOID CQuickSlotGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventIconMoveClick )
	{
		RwBool bPickUpState = (RwBool)msg.pData;

		ShowIconDestination();
		RwInt32 nEnableState = bPickUpState ? CQuickSlotItem::ICONPICK_DISABLE : CQuickSlotItem::ICONPICK_ENABLE;
		EnableStateIcon( GetIconMoveManager()->GetSrcSerial(), nEnableState );
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pData = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		
		if( pData->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		// 아이템 갯수, 삭제 때문.
		if( pData->uiUpdateType & ( EVENT_AIUT_ITEM | EVENT_AIUT_SKILL_RPBONUS ) )
			UpdateAllSlot( CHECK_FLAG_ITEMNEED );

		if( pData->uiUpdateType & EVENT_AIUT_SKILL_LEVELUP )
		{
			CNtlSobSkill* pLevelUpSkill = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetSkill( pData->uiParam1 );
			RwUInt32 hSerial = pLevelUpSkill->GetSerialID();
			
			for( RwInt32 i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
			{
				for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
				{
					if( !m_aQuickSlotItem[i][j].IsItem() )
					{
						if( m_aQuickSlotItem[i][j].GetFirstSerialID() == hSerial )
						{
							// 객체는 업그레이드의 영향을 받지 않는다.
							if( IsVisibleRow( i ) )
							{
								if( ROWCOL_TO_SLOTID( i, j ) == m_nMouseOnIndex && GetInfoWndManager()->GetRequestGui() == DIALOG_QUICKSLOT )
								{
									RwInt32 nVisibleRowIndex = GetVisibleRowIndex( i );

									CRectangle rtScreen = m_pThis->GetScreenRect();
									GetInfoWndManager()->ShowInfoWindow( TRUE, GetInfoWndType( SLCLASS_SKILL_ICON ), m_artQuickSlot[i][j].left + rtScreen.left, m_artQuickSlot[nVisibleRowIndex][j].top + rtScreen.top, m_aQuickSlotItem[i][j].GetFirstIcon()->GetSobObj(), DIALOG_QUICKSLOT );					
									return;
								}						
							}				

							SendPacketToSetIcon( i, j, pLevelUpSkill->GetIcon() );
						}
					}					
				}											
			}			
		}

		if( pData->uiUpdateType & EVENT_AIUT_ATTR )
			UpdateAllSlot( CHECK_FLAG_LPEPRP );		

		if( pData->uiUpdateType & EVENT_AIUT_SKILL_INIT )
		{
			for( RwInt32 i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
			{
				for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
				{
					if( m_aQuickSlotItem[i][j].GetSLClassID() == SLCLASS_SKILL_ICON || 
						m_aQuickSlotItem[i][j].GetSLClassID() == SLCLASS_HTBSKILL_ICON )
					{
						CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( m_aQuickSlotItem[i][j].GetFirstSerialID() );

						if (!pSobObj)
						{
							if (i < QUICKSLOT_MAX_VISIBLE_ROW)
								m_CoolTimeEffect[i][j].EndProc();

							UnsetIconFromQuickSlot(i, j);
						}
					}					
				}
			}

			UpdateAllSlot();
		}
	}
	else if( msg.Id == g_EventSobCooling )
	{
		SNtlEventSobCooling* pData = reinterpret_cast<SNtlEventSobCooling*>( msg.pData );
		
		CoolingEffectProc( pData->hSerialId, pData->bStartOrEnd );
	}

	else if( msg.Id == g_EventSobItemAdd )
	{
		SNtlEventSobItemAdd* pData = reinterpret_cast<SNtlEventSobItemAdd*>( msg.pData );

		AddItemIcon( pData->hItemSerialId );
	}
	else if( msg.Id == g_EventSobItemDelete )
	{
		SNtlEventSobItemDelete* pData = reinterpret_cast<SNtlEventSobItemDelete*>( msg.pData );

		EnableStateIcon( pData->hItemSerialId, CQuickSlotItem::ENABLE );
		DeleteItemIcon( pData->hItemSerialId );
	}
	else if( msg.Id == g_EventQuickSlotInfo )
	{
		LoadQuickSlot();
	}
	// peessi : 이 이벤트는 퀵슬롯을 지울때가 아니라, 아이템이 창고로 날라갈때 등에 호출, 이름을 바꿔야 한다.
	else if( msg.Id == g_EventSobDeleteQuickSlotIcon )
	{
		SNtlEventSobDeleteQuickSlotIcon* pData = reinterpret_cast<SNtlEventSobDeleteQuickSlotIcon*>( msg.pData );

		DeleteItemIcon( pData->hSerialId );		
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace == PLACE_BAG )
		{
			CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
			
			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem( pData->nPlaceIdx ) ) );
			if( !pBagItem )
				return;

			SERIAL_HANDLE hSerial = pBagItem->GetChildSerial( pData->uiSlotIdx );
			if( hSerial == INVALID_SERIAL_ID )
				return;

			RwInt32 nEnableState = pData->bEnable ? CQuickSlotItem::ENABLE : CQuickSlotItem::DISABLE;
			EnableStateIcon( hSerial, nEnableState );
		}		
	}
	else if( msg.Id == g_EventCapsuleLockItemWithoutBag )
	{
		// 후에 Capsule Lock/Unlock 등 퀵슬롯에 관련된 룰이 정해지면 그때 작업한다.
		/*SDboEventCapsuleLockItemWithoutBag* pData = reinterpret_cast<SDboEventCapsuleLockItemWithoutBag*>( msg.pData );

		RwInt32 nEnableState = pData->bEnable ? CQuickSlotItem::CAPSULE_LOCK : CQuickSlotItem::ENABLE;
		EnableStateIcon( pData->hItem, nEnableState );*/
	}
	else if( msg.Id == g_EventAvatarTargetStateChange )
	{
		UpdateAllSlot( CHECK_FLAG_TARGETSTATE );
	}
	else if( msg.Id == g_EventSobTargetInfoUpdate )
	{
		SNtlEventSobTargetInfoUpdate* pTartgetUpdate = reinterpret_cast<SNtlEventSobTargetInfoUpdate*>( msg.pData ); 

		if( pTartgetUpdate->bShow )
		{
			UpdateAllSlot( /*CHECK_FLAG_TARGETAPPLY |*/ CHECK_FLAG_TARGETSTATE );
		}
		else
		{
			if(Logic_GetAvatarTargetHandle() == INVALID_SERIAL_ID )
				UpdateAllSlot( /*CHECK_FLAG_TARGETAPPLY |*/ CHECK_FLAG_TARGETSTATE );
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pWorldConceptState = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pWorldConceptState->eWorldConcept == WORLD_PLAY_FREEPVP )
		{
			if( pWorldConceptState->uiState == WORLD_STATE_IDLE ||
				pWorldConceptState->uiState == WORLD_STATE_EXIT )
				UpdateAllSlot( /*CHECK_FLAG_TARGETAPPLY |*/ CHECK_FLAG_TARGETSTATE );
		}					
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		ClearQuickSlot();
	}
	else if( msg.Id == g_EventQuickSlotGuiMode )
	{
		SNtlEventQuickSlotGuiMode* pMode = reinterpret_cast<SNtlEventQuickSlotGuiMode*>( msg.pData );

		if( pMode->bExpand2 )
		{
			SetVisibleRowCount( 3 );
		}
		else
		{
			if( pMode->bExpand1 )
			{
				SetVisibleRowCount( 2 );
			}
			else
			{
				SetVisibleRowCount( 1 );
			}
		}
	}
	else if( msg.Id == g_EventQuickSlotLockMode )
	{
		SNtlEventQuickSlotLockMode* pMode = reinterpret_cast<SNtlEventQuickSlotLockMode*>( msg.pData );

		m_bIconMoveLock = pMode->bLock;
	}
	else if( msg.Id == g_EventActionMapLoadInfo )
	{
		SNtlEventActionMapLoadInfo* pLoadInfo = reinterpret_cast<SNtlEventActionMapLoadInfo*>( msg.pData );

		if( pLoadInfo->byCount != 0 )
		{
			RegisterShortCutkeyName();
		}
	}
	else if( msg.Id == g_EventActionMapUpdateRes )
	{
		SNtlEventActionMapUpdateRes* pResult = reinterpret_cast<SNtlEventActionMapUpdateRes*>( msg.pData );

		if( pResult->bSuccess )
		{
			RegisterShortCutkeyName();
		}
	}
	else if( msg.Id == g_EventQuickSlotDelNfy )
	{
		SDboEventQuickSlotDelNfy* pNotify = reinterpret_cast<SDboEventQuickSlotDelNfy*>( msg.pData );

		if( !m_bLock )
		{
			if (pNotify->bSendPacket)
			{
				RwBool bUnreg = TRUE;
				if (pNotify->bySlotIdx == ROWCOL_TO_SLOTID(GetVisibleRowIndex(0), 0))
				{
					bUnreg = Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_UNREG);
				}
				else
				{
					bUnreg = Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_UNREG);
				}

				if (bUnreg)
				{
					RwInt32 nVisibleRowIndex = SLOTID_TO_ROW(pNotify->bySlotIdx);
					RwInt32 nColIndex = SLOTID_TO_COL(pNotify->bySlotIdx);
					RwInt32 nDataRowIndex = GetDataRowIndex(nVisibleRowIndex);

					if (nDataRowIndex >= 0)
					{
						if (!CheckIconMoveLock())
						{
							if (nVisibleRowIndex < QUICKSLOT_MAX_VISIBLE_ROW && nColIndex < QUICKSLOT_MAXCOL)
								m_CoolTimeEffect[nVisibleRowIndex][nColIndex].EndProc();

							UnsetIconFromQuickSlot(nDataRowIndex, nColIndex, -1); // dont use hHandle
						}
					}
				}
			}
		}

		// do this outside the lock check.. So if we dont get issues when locking quick slot while waiting for sGU_QUICK_SLOT_DEL_NFY
		if (pNotify->bSendPacket == FALSE)
		{
			UpdateAllSlot();
		}
		else
		{
			GetIconMoveManager()->IconMoveEnd(); // stop icon move
		}
	}
	else if( msg.Id == g_EventSobAttackMode )
	{
		SNtlEventSobAttackMode* pMode = reinterpret_cast<SNtlEventSobAttackMode*>( msg.pData );

		if( pMode->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
		{
			m_bAttackMode = pMode->bAttackMode;
			UpdateAllSlot();
		}
	}
	else if( msg.Id == g_EventTransform )
	{
		SNtlEventTransform* pData = reinterpret_cast<SNtlEventTransform*>( msg.pData );
		if( pData->hSerialId == Logic_GetAvatarHandle() )
		{
			UpdateAllSlot( CHECK_FLAG_AVATARSTATE );
		}
	}
	else if (msg.Id == g_EventSobServerUpdateState)
	{
		SNtlEventSobServerUpdateState* pData = reinterpret_cast<SNtlEventSobServerUpdateState*>(msg.pData);
		if (pData->hSerialId == Logic_GetAvatarHandle())
		{
			UpdateAllSlot(CHECK_FLAG_AVATARSTATE);
		}
	}
	else if (msg.Id == g_EventSobServerUpdateCondition)
	{
		SNtlEventSobServerUpdateCondition* pData = reinterpret_cast<SNtlEventSobServerUpdateCondition*>(msg.pData);
		if (pData->hSerialId == Logic_GetAvatarHandle())
		{
			UpdateAllSlot(CHECK_FLAG_AVATARSTATE);
		}
	}
	else if (msg.Id == g_EventSkillSelectMode)
	{
		SNtlEventSkillSelectMode* pData = reinterpret_cast<SNtlEventSkillSelectMode*>(msg.pData);

		if (pData->bSelect)
		{
			for (RwInt32 j = 0; j < QUICKSLOT_MAXROW; ++j)
			{
				for (RwInt32 i = 0; i < QUICKSLOT_MAXCOL; ++i)
				{
					RwInt32 nDataRowIndex = GetDataRowIndex(j);

					if (nDataRowIndex >= 0)
					{
						if (m_aQuickSlotItem[nDataRowIndex][i].IsValid())
						{
							CNtlSobIcon* pCurrentIcon = m_aQuickSlotItem[nDataRowIndex][i].GetFirstIcon();
							if (pCurrentIcon)
							{
								if (pCurrentIcon->GetSobObj()->GetSerialID() == pData->hSerialId)
								{
									CreateFlashEffect(ROWCOL_TO_SLOTID(j, i));
									break;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			for (RwInt32 j = 0; j < QUICKSLOT_MAXROW; ++j)
			{
				for (RwInt32 i = 0; i < QUICKSLOT_MAXCOL; ++i)
				{
					RwInt32 nDataRowIndex = GetDataRowIndex(j);

					if (nDataRowIndex >= 0)
					{
						if (m_aQuickSlotItem[nDataRowIndex][i].IsValid())
						{
							CNtlSobIcon* pCurrentIcon = m_aQuickSlotItem[nDataRowIndex][i].GetFirstIcon();
							if (pCurrentIcon)
							{
								if (pCurrentIcon->GetSobObj()->GetSerialID() == pData->hSerialId)
								{
									DestroyFlashEffect(ROWCOL_TO_SLOTID(j, i));
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	//else  if( msg.Id == g_EventIconPopupResult )
	//{
	//	SDboEventIconPopupResult* pData = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

	//	if( pData->nSrcPlace == PLACE_QUICKSLOT )
	//	{
	//		if( pData->nWorkId == PMW_USE )
	//		{
	//			// 사용
	//			IconUseProc( pData->nSrcSlotIdx );
	//		}
	//		else if( pData->nWorkId == PMW_PULLOUT )
	//		{
	//			// 퀵슬롯에서 삭제
	//			if( m_aQuickSlotItem[m_nCurrentRow][pData->nSrcSlotIdx].hSerial == pData->uiSerial )
	//			{
	//				UnsetIconFromQuickSlot( m_nCurrentRow, pData->nSrcSlotIdx );
	//				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( ROWCOL_TO_SLOTID( m_nCurrentRow, pData->nSrcSlotIdx ) );
	//				UpdateAllSlot();
	//			}
	//		}
	//	}
	//}
}

VOID CQuickSlotGui::LinkActionMap()
{
	GetInputActionMap()->LinkSkillQuickSlotChange( this, &CQuickSlotGui::ActionMapQuickSlotChange );
	GetInputActionMap()->LinkSkillQuickSlotUp( this, &CQuickSlotGui::ActionMapQuickSlotUp );
	GetInputActionMap()->LinkSkillQuickSlotDown( this, &CQuickSlotGui::ActionMapQuickSlotDown );
	GetInputActionMap()->LinkSkillQuickSlotExUp( this, &CQuickSlotGui::ActionMapQuickSlotExUp );
	GetInputActionMap()->LinkSkillQuickSlotExDown( this, &CQuickSlotGui::ActionMapQuickSlotExDown );
	GetInputActionMap()->LinkSkillQuickSlotEx2Up( this, &CQuickSlotGui::ActionMapQuickSlotEx2Up );
	GetInputActionMap()->LinkSkillQuickSlotEx2Down( this, &CQuickSlotGui::ActionMapQuickSlotEx2Down );
}

VOID CQuickSlotGui::UnLinkActionMap()
{
	GetInputActionMap()->UnLinkSkillQuickSlotChange();
	GetInputActionMap()->UnLinkSkillQuickSlotDown();
	GetInputActionMap()->UnLinkSkillQuickSlotUp();
	GetInputActionMap()->UnLinkSkillQuickSlotExUp();
	GetInputActionMap()->UnLinkSkillQuickSlotExDown();
	GetInputActionMap()->UnLinkSkillQuickSlotEx2Up();
	GetInputActionMap()->UnLinkSkillQuickSlotEx2Down();
}

CPos CQuickSlotGui::GetInitialRowPosOffset(VOID)
{
	CPos pos;
	RwInt32 nRowIndex = GetVisibleRowIndex( 0 );
	pos.x = m_artQuickSlot[nRowIndex][0].left - m_artQuickSlot[0][0].left;
	pos.y = m_artQuickSlot[nRowIndex][0].top - m_artQuickSlot[0][0].top;

	return pos;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotChange( RwUInt32 uiParam )
{
	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_CHANGE ) )
		return 1;

	if( uiParam == 0 )
	{
		if( --m_nCurrentRow < 0 )
			m_nCurrentRow = QUICKSLOT_MAXROW - 1;
	}
	else
	{
		if( ++m_nCurrentRow >= QUICKSLOT_MAXROW )
			m_nCurrentRow = 0;	
	}

	m_pRowNum->SetText( m_nCurrentRow + 1 );
	UpdateAllSlot( CHECK_FLAG_ALL );		

	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotDown( RwUInt32 uiKey )
{
	if( uiKey == 0 )
	{
		if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_1 ) )
			return 1;
	}
	else
	{
		if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
			return 1;
	}

	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 1 : nVisibleRowIndex = 0; break;
	case 2 : nVisibleRowIndex = 1; break;
	case 3 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	// 처음 눌렸다면 그에 맞는 정보를 초기화
	if( KeyClickEffect( TRUE, nSlotIdx ) )
	{
		/*GetAlarmManager()->AlarmMessage( L"Key Down" );*/
		m_afPushDownKeyElapsed[nVisibleRowIndex][uiKey] = 0.0f;
		m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] = FALSE;
		m_abIsProcDownKey[nVisibleRowIndex][uiKey] = FALSE;
	}
	
	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotUp( RwUInt32 uiKey )
{
	if( uiKey == 1 )
	{
		if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_1 ) )
			return 1;
	}
	else
	{
		if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
			return 1;
	}
	
	/*GetAlarmManager()->AlarmMessage( L"Key Up" );*/

	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 1 : nVisibleRowIndex = 0; break;
	case 2 : nVisibleRowIndex = 1; break;
	case 3 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	// Key 를 Up한 상태로 이미지를 만들어주고, m_abEnableignoreUp 결과에 따라서
	// 퀵슬롯의 Icon을 사용할지 안할지 정해준다.
	if( KeyClickEffect( FALSE, nSlotIdx ) )
	{
		// peessi : 키조합에 맞게 Row값이 바뀌어야 함.
		if( m_abIsProcDownKey[nVisibleRowIndex][uiKey] == FALSE )
			m_abIsProcDownKey[nVisibleRowIndex][uiKey] = TRUE;

		if( m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] == FALSE )
			IconUseProc( nSlotIdx );
	}

	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotExDown( RwUInt32 uiKey )
{
	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 2 : nVisibleRowIndex = 0; break;
	case 3 : nVisibleRowIndex = 1; break;
	case 1 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
		return 1;
	
	// 처음 눌렸다면 그에 맞는 정보를 초기화
	if( KeyClickEffect( TRUE, nSlotIdx ) )
	{
		// peessi : 키조합에 맞게 Row값이 바뀌어야 함.
		/*GetAlarmManager()->AlarmMessage( L"Key Down" );*/
		m_afPushDownKeyElapsed[nVisibleRowIndex][uiKey] = 0.0f;
		m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] = FALSE;
		m_abIsProcDownKey[nVisibleRowIndex][uiKey] = FALSE;
	}

	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotExUp( RwUInt32 uiKey )
{
	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 2 : nVisibleRowIndex = 0; break;
	case 3 : nVisibleRowIndex = 1; break;
	case 1 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
		return 1;
	
	/*GetAlarmManager()->AlarmMessage( L"Key Up" );*/

	// Key 를 Up한 상태로 이미지를 만들어주고, m_abEnableignoreUp 결과에 따라서
	// 퀵슬롯의 Icon을 사용할지 안할지 정해준다.
	if( KeyClickEffect( FALSE, nSlotIdx ) )
	{
		// peessi : 키조합에 맞게 Row값이 바뀌어야 함.
		if( m_abIsProcDownKey[nVisibleRowIndex][uiKey] == FALSE )
			m_abIsProcDownKey[nVisibleRowIndex][uiKey] = TRUE;

		if( m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] == FALSE )
			IconUseProc( nSlotIdx );
	}

	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotEx2Down( RwUInt32 uiKey )
{
	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 3 : nVisibleRowIndex = 0; break;
	case 1 : nVisibleRowIndex = 1; break;
	case 2 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
		return 1;
	
	// 처음 눌렸다면 그에 맞는 정보를 초기화
	if( KeyClickEffect( TRUE, nSlotIdx ) )
	{
		// peessi : 키조합에 맞게 Row값이 바뀌어야 함.
		/*GetAlarmManager()->AlarmMessage( L"Key Down" );*/
		m_afPushDownKeyElapsed[nVisibleRowIndex][uiKey] = 0.0f;
		m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] = FALSE;
		m_abIsProcDownKey[nVisibleRowIndex][uiKey] = FALSE;
	}

	return 1;
}

RwInt32 CQuickSlotGui::ActionMapQuickSlotEx2Up( RwUInt32 uiKey )
{
	RwInt32 nSlotIdx = -1;
	RwInt32 nVisibleRowIndex = -1;

	switch( m_nVisibleRowCount )
	{
	case 3 : nVisibleRowIndex = 0; break;
	case 1 : nVisibleRowIndex = 1; break;
	case 2 : nVisibleRowIndex = 2; break;
	default : return 1;
	}

	nSlotIdx = ROWCOL_TO_SLOTID( nVisibleRowIndex, uiKey );

	if( !Logic_CanKeybaordInput_in_Tutorial( ETL_KEYBOARD_INPUT_TYPE_QUICK_SLOT_2_LAST ) )
		return 1;
	
	/*GetAlarmManager()->AlarmMessage( L"Key Up" );*/

	// Key 를 Up한 상태로 이미지를 만들어주고, m_abEnableignoreUp 결과에 따라서
	// 퀵슬롯의 Icon을 사용할지 안할지 정해준다.
	if( KeyClickEffect( FALSE, nSlotIdx ) )
	{
		// peessi : 키조합에 맞게 Row값이 바뀌어야 함.
		if( m_abIsProcDownKey[nVisibleRowIndex][uiKey] == FALSE )
			m_abIsProcDownKey[nVisibleRowIndex][uiKey] = TRUE;

		if( m_abEnableIgnoreUp[nVisibleRowIndex][uiKey] == FALSE )
			IconUseProc( nSlotIdx );
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// ! Implementation

VOID CQuickSlotGui::LoadQuickSlot(VOID)
{
	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	m_bLock = FALSE;

	if( !pAvatarInfo->bySetQuickSlotInfo )
		return;

	for( RwInt32 i = 0 ; i < pAvatarInfo->byQuickSlotInfoCount ; ++i )
	{
		sQUICK_SLOT_PROFILE* pQuickSlotData = &pAvatarInfo->sQuickSlotInfo[i];

		// peessi: In the tutorial, there are times when a packet is being sent to the server before the avatar is created to clear the skill.
		// At that time, the slot information of the quick slot information of the avatar information is modified to INVALID_BYTE and processed.
		// Because the DB information is also deleted when the packet arrives, you do not have to ask the server to delete it.
		if( pQuickSlotData->bySlot == INVALID_BYTE )
			continue;

		CNtlSobIcon* pIcon = NULL;
		CNtlSob* pSob = NULL;
		
		if( pQuickSlotData->byType == QUICK_SLOT_TYPE_SKILL )
		{
			pSob = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetSkillFromTableId( pQuickSlotData->tblidx );
			if( !pSob )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( pQuickSlotData->bySlot );
				continue;
			}

			pIcon = pSob->GetIcon();
			pQuickSlotData->hItem = pIcon->GetSobObj()->GetSerialID();
		}
		else if( pQuickSlotData->byType == QUICK_SLOT_TYPE_HTB_SKILL )
		{
			pSob = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetHTBSkillFromTableId( pQuickSlotData->tblidx );
			if( !pSob )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( pQuickSlotData->bySlot );
				continue;
			}

			pIcon = pSob->GetIcon();
			pQuickSlotData->hItem = pIcon->GetSobObj()->GetSerialID();
		}
		else if( pQuickSlotData->byType == QUICK_SLOT_TYPE_SOCIALACTION )
		{
			pSob = GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetActionSkillFromTableId( pQuickSlotData->tblidx );
			if( !pSob )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( pQuickSlotData->bySlot );
				continue;
			}

			pIcon = pSob->GetIcon();
			pQuickSlotData->hItem = pIcon->GetSobObj()->GetSerialID();
		}
		else if( pQuickSlotData->byType == QUICK_SLOT_TYPE_ITEM )
		{
			pSob = GetNtlSobManager()->GetSobObject( pQuickSlotData->hItem );
			if( !pSob )
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( pQuickSlotData->bySlot );
				continue;
			}
			
			pIcon = pSob->GetIcon();
		}
		else
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( pQuickSlotData->bySlot );
			continue;
		}

		SetIconToQuickSlot( SLOTID_TO_ROW( pQuickSlotData->bySlot ), SLOTID_TO_COL( pQuickSlotData->bySlot ), pQuickSlotData->hItem, pIcon );
	}

	UpdateAllSlot( CHECK_FLAG_ALL );
}

VOID CQuickSlotGui::ClearQuickSlot(VOID)
{
	for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
		{
			m_aQuickSlotItem[i][j].UnSetItem();

			if (i < QUICKSLOT_MAX_VISIBLE_ROW)
				m_CoolTimeEffect[i][j].EndProc();
		}
	}

	UpdateAllSlot();
}

VOID CQuickSlotGui::SetIconToQuickSlot( RwInt32 nRow, RwInt32 nCol, RwUInt32 hSerialID, CNtlSobIcon* pSetIcon )
{
	if( m_bLock )
		return;

	if( !IsValidSlotIdx( ROWCOL_TO_SLOTID( nRow, nCol ) ) )
		return;

	if( nRow == 0 && nCol == 0 )
	{
		if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_REG ) )
		{
			if( pSetIcon->GetClassID() == SLCLASS_SKILL_ICON )
				Logic_SendTutorialCondition(ETL_CONDITION_TYPE_QUICK_SLOT_1_REG_SKILL);
		}
	}

	// Delete previous content
	m_aQuickSlotItem[nRow][nCol].UnSetItem();

	if( pSetIcon->GetClassID() == SLCLASS_ITEM_ICON )
	{
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		CNtlSobItemAttr* pSetItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSetIcon->GetSobObj()->GetSobAttr() );
		m_aQuickSlotItem[nRow][nCol].SetItem( pSetIcon->GetSobObj()->GetSerialID(), pSetIcon, ROWCOL_TO_SLOTID( nRow, nCol ) );					
		
		for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
		{
			RwUInt32 hBagSerial = pInventory->GetBagItem( i );
			if( hBagSerial == INVALID_SERIAL_ID )
				continue;

			CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagSerial ) );
			
			for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
			{
				CNtlSobItem* pItem = pBagItem->GetChildItem( j );
				
				if( !pItem )
					continue;

				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );				

				if( pSetItemAttr->GetTblIdx() == pItemAttr->GetTblIdx() && hSerialID != pItem->GetSerialID() )
				{
					m_aQuickSlotItem[nRow][nCol].SetItem( pItem->GetSerialID(), pItem->GetIcon(), ROWCOL_TO_SLOTID( nRow, nCol ) );					
				}
			}
		}

		m_aQuickSlotItem[nRow][nCol].SetEnableState( CQuickSlotItem::ENABLE );
	}
	else
	{
		m_aQuickSlotItem[nRow][nCol].SetItem( hSerialID, pSetIcon, ROWCOL_TO_SLOTID( nRow, nCol ) );
		m_aQuickSlotItem[nRow][nCol].SetEnableState( CQuickSlotItem::ENABLE );
	}
}

VOID CQuickSlotGui::UnsetIconFromQuickSlot( RwInt32 nRow, RwInt32 nCol, RwUInt32 hSerialID /* = INVALID_SERIAL_ID */, RwBool bSendPacket /* = TRUE  */ )
{
	if (m_bLock)
	{
		return;
	}

	if (!IsValidSlotIdx(ROWCOL_TO_SLOTID(nRow, nCol)))
	{
		return;
	}

	if( nRow == 0 && nCol == 0 )
	{
		if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_REG ) )
		{
			if( m_aQuickSlotItem[nRow][nCol].GetSLClassID() == SLCLASS_SKILL_ICON )
				Logic_SendTutorialCondition(ETL_CONDITION_TYPE_QUICK_SLOT_1_REG_SKILL);
		}			
	}

	if( hSerialID == INVALID_SERIAL_ID )
	{
		m_aQuickSlotItem[nRow][nCol].UnSetItem();

		if( bSendPacket )
			GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( ROWCOL_TO_SLOTID( nRow, nCol ) );
	}
	else
	{
		if( m_aQuickSlotItem[nRow][nCol].UnSetItem( hSerialID ) )
		{
			if( bSendPacket )
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( ROWCOL_TO_SLOTID( nRow, nCol ) );
		}
	}
}

VOID CQuickSlotGui::SendPacketToSetIcon( RwInt32 nRow, RwInt32 nCol, CNtlSobIcon* pIcon )
{
	RwUInt32	uiTblIdx = INVALID_DWORD;
	RwUInt8		ucType = INVALID_BYTE;
	RwUInt8		ucPlace = INVALID_BYTE;
	RwUInt8		ucPos = INVALID_BYTE;

	switch( pIcon->GetClassID() )
	{
	case SLCLASS_ITEM_ICON:
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( pIcon->GetSobObj() );
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pIcon->GetSobObj()->GetSobAttr() );

			uiTblIdx = pItemAttr->GetTblIdx();
			ucType = QUICK_SLOT_TYPE_ITEM;
			ucPlace = pItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1;
			ucPos = pItem->GetItemSlotIdx();
		}
		break;
	case SLCLASS_SKILL_ICON:
		{
			CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pIcon->GetSobObj()->GetSobAttr() );

			uiTblIdx = pSkillAttr->GetSkillTbl()->tblidx;
			ucType = QUICK_SLOT_TYPE_SKILL;
		}
		break;
	case SLCLASS_HTBSKILL_ICON:
		{
			CNtlSobHTBSkillAttr* pHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pIcon->GetSobObj()->GetSobAttr() );

			uiTblIdx = pHTBSkillAttr->GetHTBSkillTbl()->tblidx;
			ucType = QUICK_SLOT_TYPE_HTB_SKILL;
		}
		break;
	case SLCLASS_ACTIONSKILL_ICON:
		{
			CNtlSobActionSkillAttr* pActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pIcon->GetSobObj()->GetSobAttr() );

			uiTblIdx = pActionSkillAttr->GetActionSkillTbl()->tblidx;
			ucType = QUICK_SLOT_TYPE_SOCIALACTION;;
		}
		break;
	default:
		NTL_ASSERTFAIL( "CQuickSlotGui::SendPacketToSetIcon : Invalid SobObject Type" );
		break;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendSetIconToQuickSlot( uiTblIdx, ROWCOL_TO_SLOTID( nRow, nCol ), ucType, ucPlace, ucPos );	
}

/**
* \brief 좌표상에 위치한 퀵슬롯의 인덱스를 가져온다.
* \param	nX				X 좌표
* \param	nY				Y 좌표
* \return	퀵슬롯의 인덱스(-1 이라면 좌표가 인덱스에서 벗어났다는 것)
*/
RwInt32 CQuickSlotGui::GetQuickSlotIdx( RwInt32 nX, RwInt32 nY )
{
	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			if( m_artQuickSlot[j][i].PtInRect( CPos( nX, nY ) ) )
				return ROWCOL_TO_SLOTID( j, i );
		}
	}
	return -1;
}

RwBool CQuickSlotGui::IsEnableClick( RwInt32 nSlotIdx )
{
	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return FALSE;

	RwInt32 nVisiblwRow = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nDataRow = GetDataRowIndex( nVisiblwRow );
	RwInt32 nCol = SLOTID_TO_COL( nSlotIdx );

	if( !IsValidRowIndex( nDataRow ) )
		return FALSE;

	if( !m_aQuickSlotItem[nDataRow][nCol].IsValid() )
		return FALSE;

	if( m_aQuickSlotItem[nDataRow][nCol].GetEnableState() != CQuickSlotItem::ENABLE )
		return FALSE;

	return TRUE;
}

RwBool CQuickSlotGui::IsEnablePutDown( RwInt32 nSlotIdx )
{
	if( CheckIconMoveLock() )
		return FALSE;	

	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return FALSE;

	RwInt32 nVisiblwRow = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nDataRow = GetDataRowIndex( nVisiblwRow );
	RwInt32 nCol = SLOTID_TO_COL( nSlotIdx );

	if( !IsValidRowIndex( nDataRow ) )
		return FALSE;

	if( m_aQuickSlotItem[nDataRow][nCol].GetEnableState() != CQuickSlotItem::ENABLE )
		return FALSE;

	return TRUE;
}

VOID CQuickSlotGui::ShowIconDestination(VOID)
{
	if( GetIconMoveManager()->IsActive() )
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject( GetIconMoveManager()->GetSrcSerial() );
		
		if( !pSob )
			return;
		
		if( GetIconMoveManager()->GetSrcPlace() != PLACE_SKILL && 
			GetIconMoveManager()->GetSrcPlace() != PLACE_BAG &&
			GetIconMoveManager()->GetSrcPlace() != PLACE_QUICKSLOT )
			return;

		if (GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
		{
			if (CanPlaceItem(pSob->GetIcon()) == FALSE)
				return;
		}

		for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
		{
			for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
				CreateFlashEffect(ROWCOL_TO_SLOTID(j, i));
		}
	}
	else
	{
		for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
		{
			for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
				DestroyFlashEffect(ROWCOL_TO_SLOTID(j, i));
		}		
	}
}

/**
* \brief 퀵슬롯의 박스의 구성을 하드코딩
*/
VOID CQuickSlotGui::SetSlotRectHardCode(VOID)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		/*m_asurKeyImage[j][0].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey1" ) );
		m_asurKeyImage[j][1].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey2" ) );
		m_asurKeyImage[j][2].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey3" ) );
		m_asurKeyImage[j][3].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey4" ) );
		m_asurKeyImage[j][4].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey5" ) );
		m_asurKeyImage[j][5].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey6" ) );
		m_asurKeyImage[j][6].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey7" ) );
		m_asurKeyImage[j][7].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey8" ) );
		m_asurKeyImage[j][8].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey9" ) );
		m_asurKeyImage[j][9].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey0" ) );
		m_asurKeyImage[j][10].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey-" ) );
		m_asurKeyImage[j][11].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfKey=" ) );*/

		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			m_artQuickSlot[j][i].SetRectWH( i * 35 + 1, j * 36 + 3, NTL_ITEM_ICON_SIZE, NTL_ITEM_ICON_SIZE );

			RwInt32 nLeft	= rtScreen.left + m_artQuickSlot[j][i].left;
			RwInt32 nTop	= rtScreen.top + m_artQuickSlot[j][i].top;
			RwInt32 nRight	= rtScreen.left + m_artQuickSlot[j][i].right;
			RwInt32 nBottom = rtScreen.top + m_artQuickSlot[j][i].bottom;
			m_asurIcon[j][i].SetRect( nLeft, nTop, nRight, nBottom  );

			m_asurFocusSlot[j][i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
			m_asurFocusSlot[j][i].SetRect( nLeft, nTop, nRight, nBottom );

			m_asurSlot[j][i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "QuickSlot.srf", "srfQuickSlotBack" ) );
			m_asurSlot[j][i].SetRect( nLeft, nTop, nRight + 2, nBottom + 4 );

			//m_asurKeyImage[j][i].SetPosition( nLeft, nTop );

			m_CoolTimeEffect[j][i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
			m_CoolTimeEffect[j][i].SetPosition( nLeft, nTop );		

			m_asurDisableSlot[j][i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotDisableEffect" ) );

			m_asurDisableSkill[j][i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

			m_asurRPType[j][i].SetPositionfromParent( m_artQuickSlot[j][i].left + QUICK_RPTYPE_OFFSET, m_artQuickSlot[j][i].top + QUICK_RPTYPE_OFFSET );		

			// peessi : Flash는 로딩시 부하가 심해, 미리 로딩해서 사용한다. 
			CRectangle rect;
			rect.SetRectWH( m_artQuickSlot[j][i].left - 5, m_artQuickSlot[j][i].top - 5, 42, 42 );				  
			m_apflaEffect[j][i] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf" );
			m_apflaEffect[j][i]->Show( false );
			m_apflaEffect[j][i]->Enable( false );
			//
		}	
	}	
}

/**
* \brief 반드시 기존의 CQuickSlotGui::SetSlotHardCodeRect 함수 실행 후에 실행되어야 한다.
*/
VOID CQuickSlotGui::CreateShortCutKeyName()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		for( RwInt32 i = 0; i < QUICKSLOT_MAXCOL; ++i )
		{
			RwInt32 nLeft	= m_artQuickSlot[j][i].left;
			RwInt32 nTop	= m_artQuickSlot[j][i].top;
			RwInt32 nRight	= m_artQuickSlot[j][i].right;
			RwInt32 nBottom = m_artQuickSlot[j][i].bottom;

			// Static Box의 위치
			rtRect.SetRect( nLeft, nTop, nRight, nBottom );

			m_apStbShortCutName[j][i] = NTL_NEW gui::CStaticBox( &rtRect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_SLOT_KEYNAME_ALIGN );
			m_apStbShortCutName[j][i]->CreateFontStd( DBOGUI_SLOT_KEYNAME_FONT, DBOGUI_SLOT_KEYNAME_FONTHEIGHT, DBOGUI_SLOT_KEYNAME_FONTATTR );
			m_apStbShortCutName[j][i]->SetEffectMode( TE_OUTLINE );
			m_apStbShortCutName[j][i]->SetLineSpace( 0 );
			m_apStbShortCutName[j][i]->Enable( false );
		}
	}
}

VOID CQuickSlotGui::DestroyShortCutKeyName()
{
	for( int j = 0; j < QUICKSLOT_MAX_VISIBLE_ROW; ++j )
	{
		for( int i = 0; i < QUICKSLOT_MAXCOL; ++i )
		{
			NTL_DELETE( m_apStbShortCutName[j][i] );
		}
	}
}

/**
* \brief 최초에 퀵슬롯에 단축키 이름을 SetText 한다.
*/
VOID CQuickSlotGui::RegisterShortCutkeyName()
{
	for( RwInt32 i = ACTION_QUICK_1; i <= ACTION_QUICK_PLUS; ++i )
	{
		m_apStbShortCutName[m_anQuickSlotRow[ROW_DEFAULT]][i-ACTION_QUICK_1]->SetText( GetInputActionMap()->GetKeyName( i ).c_str() );
	}

	for( RwInt32 i = ACTION_QUICK_1_EX; i <= ACTION_QUICK_PLUS_EX; ++i )
	{
		m_apStbShortCutName[m_anQuickSlotRow[ROW_EX]][i-ACTION_QUICK_1_EX]->SetText( GetInputActionMap()->GetKeyName( i ).c_str() );
	}

	for( RwInt32 i = ACTION_QUICK_1_EX2; i <= ACTION_QUICK_PLUS_EX2; ++i )
	{
		m_apStbShortCutName[m_anQuickSlotRow[ROW_EX2]][i-ACTION_QUICK_1_EX2]->SetText( GetInputActionMap()->GetKeyName( i ).c_str() );
	}
}

VOID CQuickSlotGui::IconPutDownProc(RwUInt32 hSerial, RwInt32 ePlace, RwInt32 nSlotIdx )
{
	if( CheckIconMoveLock() )
		return;	

	// 예외 처리
	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return;

	if( hSerial == INVALID_SERIAL_ID )
		return;

	RwInt32 nVisibleRow = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nCol = SLOTID_TO_COL( nSlotIdx );
	RwInt32 nDataRow = GetDataRowIndex( nVisibleRow );

	if( !IsValidRowIndex( nDataRow ) )
		return;

	if( ePlace == PLACE_SKILL || ePlace == PLACE_BAG )
	{
		CNtlSobIcon* pIcon = (GetNtlSobManager()->GetSobObject(hSerial))->GetIcon();

		if (ePlace == PLACE_BAG)
		{
			if (CanPlaceItem(pIcon) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_THERE");
				return;
			}
		}

		// Duplicate check missing.
		//DeleteOverlapIcon( hSerial );

		// Data Set
		SetIconToQuickSlot( nDataRow, nCol, hSerial, pIcon );

		// 패킷 보내기 
		RwInt32 nSlotID = ROWCOL_TO_SLOTID( nDataRow, nCol );
		NTL_ASSERT( nSlotID >= 0 && nSlotID < 48, "CQuickSlotItem::IconPutDownProc : SlotID Error!(" << nSlotID << ")" );	
		SendPacketToSetIcon( nDataRow, nCol, pIcon );
	}
	else if( ePlace == PLACE_QUICKSLOT )
	{
		RwInt32 nSrcVisibleRow = SLOTID_TO_ROW( GetIconMoveManager()->GetSrcSlotIdx() );
		RwInt32 nSrcCol = SLOTID_TO_COL( GetIconMoveManager()->GetSrcSlotIdx() );
		RwInt32 nSrcDataRow = GetDataRowIndex( nSrcVisibleRow );

		if( IsValidRowIndex( nSrcDataRow ) )
		{
			// Data Set
			CQuickSlotItem TempQuickSlot = m_aQuickSlotItem[nDataRow][nCol];
			m_aQuickSlotItem[nDataRow][nCol] = m_aQuickSlotItem[nSrcDataRow][nSrcCol];
			m_aQuickSlotItem[nSrcDataRow][nSrcCol] = TempQuickSlot;

			// 패킷 보내기
			if( m_aQuickSlotItem[nDataRow][nCol].IsValid() )
			{
				RwInt32 nSlotID = ROWCOL_TO_SLOTID( nDataRow, nCol );
				NTL_ASSERT( nSlotID >= 0 && nSlotID < 48, "CQuickSlotItem::IconPutDownProc by PLACE_QUICKSLOT New : SlotID Error!(" << nSlotID << ")" );	
				SendPacketToSetIcon( nDataRow, nCol, m_aQuickSlotItem[nDataRow][nCol].GetFirstIcon() );
			}
			else
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( ROWCOL_TO_SLOTID( nDataRow, nCol ) );

			if( m_aQuickSlotItem[nSrcDataRow][nSrcCol].IsValid() )
			{
				RwInt32 nSlotID = ROWCOL_TO_SLOTID( nSrcDataRow, nSrcCol );
				NTL_ASSERT( nSlotID >= 0 && nSlotID < 48, "CQuickSlotItem::IconPutDownProc by PLACE_QUICKSLOT Old : SlotID Error!(" << nSlotID << ")" );	
				SendPacketToSetIcon( nSrcDataRow, nSrcCol, m_aQuickSlotItem[nSrcDataRow][nSrcCol].GetFirstIcon() );
			}
			else
				GetDboGlobal()->GetGamePacketGenerator()->SendUnsetIconFromQuickSlot( ROWCOL_TO_SLOTID( nSrcDataRow, nSrcCol ) );
		}		
	}
	else
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEM_CAN_NOT_MOVE_THERE" );
		return;
	}
		
	// Display Icon Update
	UpdateAllSlot( CHECK_FLAG_ALL );

	// IconMoveManager Reset
	GetIconMoveManager()->IconMoveEnd();

	// Sound
	Logic_PlayGUISound(GSD_SYSTEM_ICON_SET);
}

VOID CQuickSlotGui::IconUseProc( RwInt32 nSlotIdx )
{
	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return;

	RwInt32 nVisibleRowIndex = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIndex = SLOTID_TO_COL( nSlotIdx );
	RwInt32 nDataRowIndex = GetDataRowIndex( nVisibleRowIndex );

	if( !IsValidRowIndex( nDataRowIndex ) )
		return;

	SERIAL_HANDLE hSerial = m_aQuickSlotItem[nDataRowIndex][nColIndex].GetFirstSerialID();
	RwInt32 nEnableState = m_aQuickSlotItem[nDataRowIndex][nColIndex].GetEnableState();
	
	if( hSerial == INVALID_SERIAL_ID || nEnableState != CQuickSlotItem::ENABLE )
		return;

	// Icon이 스킬일 경우 현재 RpBonus Dialog가 떠 있으면 사용안되게 해야 한다.
	if( m_aQuickSlotItem[nDataRowIndex][nColIndex].GetSLClassID() == SLCLASS_SKILL_ICON )
		if( GetDialogManager()->IsOpenDialog( DIALOG_SKILL_RPBONUS ) )
			return;

	Logic_UseProc( hSerial ); 
}

VOID CQuickSlotGui::AddItemIcon( RwUInt32 hSerial )
{
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
	if (!pItem)
		return;

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		 
	for( RwInt32 i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
	{
		for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			CQuickSlotItem* pQuickSlotItem = &m_aQuickSlotItem[i][j];

			if( pQuickSlotItem->IsItem() )
			{
				if( pItemAttr->GetTblIdx() == reinterpret_cast<CNtlSobItemAttr*>( pQuickSlotItem->GetFirstIcon()->GetSobObj()->GetSobAttr() )->GetTblIdx() )
				{
					pQuickSlotItem->SetItem( pItem->GetSerialID(), pItem->GetIcon(), ROWCOL_TO_SLOTID( i, j ) );

					if( i == m_nCurrentRow )
						UpdateAllSlot();					
				}
			}
		}
	}
}

VOID CQuickSlotGui::DeleteItemIcon(RwUInt32 hSerial )
{
	for( int i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
	{
		for( int j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			if( m_aQuickSlotItem[i][j].HasObj( hSerial ) )
			{
				UnsetIconFromQuickSlot( i, j, hSerial );	

				if(i < QUICKSLOT_MAX_VISIBLE_ROW)
					m_CoolTimeEffect[i][j].EndProc();

				Logic_PlayGUISound(GSD_SYSTEM_ICON_WASTE);

				if( i == m_nCurrentRow )
					UpdateAllSlot();
			}
		}
	}
}

VOID CQuickSlotGui::EnableStateIcon( RwUInt32 hSerial, RwInt32 nEnableState )
{
	RwBool bUpdate = FALSE;

	for( RwInt32 i = 0 ; i < QUICKSLOT_MAXROW ; ++i )
	{
		for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			if( m_aQuickSlotItem[i][j].HasObj( hSerial ) )
			{
				if( nEnableState == CQuickSlotItem::ICONPICK_ENABLE )
				{
					if( m_aQuickSlotItem[i][j].GetEnableState() == CQuickSlotItem::DISABLE )
						continue;
					else
						m_aQuickSlotItem[i][j].SetEnableState( CQuickSlotItem::ENABLE );
				}
				else
				{
					m_aQuickSlotItem[i][j].SetEnableState( nEnableState );				
				}		

				if( IsVisibleRow( i ) )
					bUpdate = TRUE;
			}
		}
	}	

	if( bUpdate )
		UpdateAllSlot();
}

bool CQuickSlotGui::CanPlaceItem(CNtlSobIcon* pIcon)
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(pIcon->GetSobObj());
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

	if (pSobItemAttr->IsExpired())
		return false;

	if (pSobItemAttr->GetRestrictState() == ITEM_RESTRICT_STATE_TYPE_SEAL)
		return false;

	if (pSobItemAttr->IsNeedToIdentify())
		return false;

	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

	if (pITEM_TBLDAT->byItem_Type == ITEM_TYPE_CAPSULE) // return true if vehicle
		return true;

	if (pITEM_TBLDAT->Use_Item_Tblidx == INVALID_TBLIDX)
		return false;

	CUseItemTable* pUseItemTable = API_GetTableContainer()->GetUseItemTable();
	sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>(pUseItemTable->FindData(pITEM_TBLDAT->Use_Item_Tblidx));
	if (!pUseItemData)
		return false;

	//DBO_WARNING_MESSAGE("tblidx: " << (int)pUseItemData->tblidx << ", wFunction_Bit_Flag: " << (int)pUseItemData->wFunction_Bit_Flag << ", byItem_Type: " << (int)pITEM_TBLDAT->byItem_Type);
	//DBO_WARNING_MESSAGE("byBuff_Group: " << (int)pUseItemData->byBuff_Group << ", byBuffKeepType: " << (int)pUseItemData->byBuffKeepType << ", byUse_Item_Active_Type: " << (int)pUseItemData->byUse_Item_Active_Type);

	if (pUseItemData->byUse_Item_Active_Type != SKILL_ACTIVE_TYPE_DH
		&& pUseItemData->byUse_Item_Active_Type != SKILL_ACTIVE_TYPE_HOT
		&& pUseItemData->byUse_Item_Active_Type != SKILL_ACTIVE_TYPE_BB
		&& pUseItemData->byUse_Item_Active_Type != SKILL_ACTIVE_TYPE_DB)
		return false;

	switch (pITEM_TBLDAT->byItem_Type)
	{
		case ITEM_TYPE_GAMBLE:// return false if gamble like box
		case ITEM_TYPE_QUICK_TELEPORT:
		case ITEM_TYPE_CRESCENT_POPO:
		case ITEM_TYPE_CHARACTER_NAME_CHANGE:
		case ITEM_TYPE_GUILD_NAME_CHANGE:
		case ITEM_TYPE_SKILL_POINT_RESET:
		case ITEM_TYPE_SKILL_POINT_RESET_PLUS:
		case ITEM_TYPE_SKILL_ONE_POINT_RESET:

			return false;
		break;


		default: break;
	}

	return true;
}

// Desc : UpdateAllSlot에서는 실제 QuickSlot데이터가 아닌 Cool-time, Stack, QuickSlotRow의 변화만 조정한다.
VOID CQuickSlotGui::UpdateAllSlot( RwUInt32 flagUseableCheck /* = 0  */ )
{
	for( RwInt32 j = 0 ; j < m_nVisibleRowCount ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			RwInt32 nDataRowIndex = GetDataRowIndex( j );

			if( nDataRowIndex >= 0 )
			{
				if( m_aQuickSlotItem[nDataRowIndex][i].IsValid() )
				{
					CNtlSobIcon* pCurrentIcon = m_aQuickSlotItem[nDataRowIndex][i].GetFirstIcon();

					m_asurIcon[j][i].SetTexture( reinterpret_cast<gui::CTexture*>( pCurrentIcon->GetImage() ) );

					if( pCurrentIcon->GetClassID() == SLCLASS_ITEM_ICON )
					{
						CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( pCurrentIcon->GetSobObj() );
						RwInt32 nStackNumber = m_aQuickSlotItem[nDataRowIndex][i].GetStackCount();

						if( pItem->IsStackable() && nStackNumber > 0 )
						{
							CreateStackNumber( ROWCOL_TO_SLOTID( j, i ), nStackNumber );
						}
						else
						{
							DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );
						}

						m_asurRPType[j][i].Show( FALSE );
						m_asurDisableSkill[j][i].Show( FALSE );
					//	DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
					}
					else if( pCurrentIcon->GetClassID() == SLCLASS_SKILL_ICON )
					{
						CNtlSobSkill* pSkill = reinterpret_cast<CNtlSobSkill*>( pCurrentIcon->GetSobObj() );
						CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );

						if( pSkillAttr->m_bRPBonusAutoMode )
						{
							m_asurRPType[j][i].SetSurface( Logic_GetSmallRPTypeMark( pSkillAttr->m_byRPBonusType ) );
							m_asurRPType[j][i].Show( TRUE );

							CRectangle rect = m_pThis->GetScreenRect();
							m_asurRPType[j][i].SetPositionbyParent( rect.left, rect.top );
						}
						else 
						{
							m_asurRPType[j][i].Show( FALSE );
						}

					//	DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
						DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );
						m_asurDisableSkill[j][i].Show( FALSE );	// 초기화

						if( flagUseableCheck & CHECK_FLAG_LPEPRP )
						{
							if( !UseableCheck_LPEPRP( pSkillAttr ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_LPEPRP );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_LPEPRP );
						}

						if( flagUseableCheck & CHECK_FLAG_TARGETSTATE )
						{
							if( !UseableCheck_TargetState( pSkillAttr->GetSkillTbl() ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_TARGETSTATE );					
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_TARGETSTATE );
						}

						if ( flagUseableCheck & CHECK_FLAG_ITEMNEED )
						{
							if( !UseableCheck_ItemNeed( pSkillAttr->GetSkillTbl() ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_ITEMNEED );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_ITEMNEED );
						}

						/*if ( flagUseableCheck & CHECK_FLAG_TARGETAPPLY )
						{
							if( !UseableCheck_TargetApply( pSkillAttr->GetSkillTbl() ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_TARGETAPPLY );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_TARGETAPPLY );
						}*/

						if( flagUseableCheck & CHECK_FLAG_AVATARSTATE )
						{	
							if( !UseableCheck_AvatarState( pSkillAttr->GetSkillTbl() ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_AVATARSTATE );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_AVATARSTATE );
						}

						if( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() )
							m_asurDisableSkill[j][i].Show( TRUE );
					}		
					else if( pCurrentIcon->GetClassID() == SLCLASS_HTBSKILL_ICON )
					{
						CNtlSobHTBSkill* pHTBSkill = reinterpret_cast<CNtlSobHTBSkill*>( pCurrentIcon->GetSobObj() );
						CNtlSobHTBSkillAttr* pHTBSkillAttr = reinterpret_cast<CNtlSobHTBSkillAttr*>( pHTBSkill->GetSobAttr() );
						CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
						sSKILL_TBLDAT* pSkillData = NULL;

						for( RwInt32 k = 0 ; k < NTL_HTB_MAX_SKILL_COUNT_IN_SET ; ++k )
						{
							if( pHTBSkillAttr->GetHTBSkillTbl()->aHTBAction[k].skillTblidx != INVALID_TBLIDX )
							{
								pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pHTBSkillAttr->GetHTBSkillTbl()->aHTBAction[k].skillTblidx ) );
								break;
							}
						}

						m_asurRPType[j][i].Show( FALSE );
					//	DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
						DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );
						m_asurDisableSkill[j][i].Show( FALSE );

						if( flagUseableCheck & CHECK_FLAG_LPEPRP )
						{
							if( !UseableCheck_LPEPRP( pHTBSkillAttr ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_LPEPRP );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_LPEPRP );
						}

						if( flagUseableCheck & CHECK_FLAG_TARGETSTATE )
						{
							if( !UseableCheck_TargetState( pSkillData ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_TARGETSTATE );					
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_TARGETSTATE );
						}

						if ( flagUseableCheck & CHECK_FLAG_ITEMNEED )
						{
							if( !UseableCheck_ItemNeed( pSkillData ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_ITEMNEED );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_ITEMNEED );
						}

						/*if ( flagUseableCheck & CHECK_FLAG_TARGETAPPLY )
						{
							if( !UseableCheck_TargetApply( pSkillData ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_TARGETAPPLY );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_TARGETAPPLY );
						}*/

						if( flagUseableCheck & CHECK_FLAG_AVATARSTATE )
						{	
							if( !UseableCheck_AvatarState( pSkillData ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_AVATARSTATE );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_AVATARSTATE );
						}

						if( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() )
							m_asurDisableSkill[j][i].Show( TRUE );
					}
					else if( pCurrentIcon->GetClassID() == SLCLASS_ACTIONSKILL_ICON )
					{
						CNtlSobActionSkill* pActionSkill = reinterpret_cast<CNtlSobActionSkill*>( pCurrentIcon->GetSobObj() );
						CNtlSobActionSkillAttr* pActionSkillAttr = reinterpret_cast<CNtlSobActionSkillAttr*>( pActionSkill->GetSobAttr() );
						sACTION_TBLDAT* pActionTblDat = pActionSkillAttr->GetActionSkillTbl();

						if( pActionTblDat->byAction_Type == ACTION_TYPE_NORMAL_ACTION )
						{
							if( pActionTblDat->byETC_Action_Type == ACTION_FUNC_TYPE_BASIC_ATTACK && m_bAttackMode )
							{
								CreateFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
							}
							else
							{
								DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
							}
						}
					//	else
					//		DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );

						m_asurDisableSkill[j][i].Show( FALSE );	// 초기화
						
						/*if( flagUseableCheck & CHECK_FLAG_TARGETAPPLY )
						{
							if( !UseableCheck_TargetApply( pActionTblDat ) )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_TARGETAPPLY );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_TARGETAPPLY );
						}*/

						if( flagUseableCheck & CHECK_FLAG_AVATARSTATE )
						{
							if( !UseableCheck_AvatarState() )
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() | CHECK_FLAG_AVATARSTATE );
							else
								m_aQuickSlotItem[nDataRowIndex][i].SetUseableCheckFlag( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() & ~CHECK_FLAG_AVATARSTATE );
						}

						if( m_aQuickSlotItem[nDataRowIndex][i].GetUseableCheckFlag() )
							m_asurDisableSkill[j][i].Show( TRUE );

						DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );						
						m_asurRPType[j][i].Show( FALSE );
					}
					else
					{
					//	DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
						DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );
						m_asurDisableSkill[j][i].Show( FALSE );	// 초기화
						m_asurRPType[j][i].Show( FALSE );
					}

					if( pCurrentIcon->GetIconState() == CNtlSobIcon::ICON_STATE_COOLING )
						m_CoolTimeEffect[j][i].StartProc( pCurrentIcon->GetMaxCoolingTime() );
					else
						m_CoolTimeEffect[j][i].EndProc();

					if( m_aQuickSlotItem[nDataRowIndex][i].GetEnableState() == CQuickSlotItem::ENABLE )
						m_asurDisableSlot[j][i].Show( FALSE );
					else
						m_asurDisableSlot[j][i].Show( TRUE );
				}
				else
				{
					m_asurIcon[j][i].UnsetTexture();
				//	DestroyFlashEffect( ROWCOL_TO_SLOTID( j, i ) );
					DestroyStackNumber( ROWCOL_TO_SLOTID( j, i ) );
					m_CoolTimeEffect[j][i].EndProc();
					m_asurDisableSlot[j][i].Show( FALSE );
					m_asurDisableSkill[j][i].Show( FALSE );
					m_asurRPType[j][i].Show( FALSE );
				}
			}			
		}
	}		
}

RwBool CQuickSlotGui::CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue )
{
	NTL_FUNCTION( "CQuickSlotGui::CreateStackNumber" );

	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		NTL_RETURN( FALSE );

	RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

	if( m_apstbStackNum[nRowIdx][nColIdx] )
	{
		m_apstbStackNum[nRowIdx][nColIdx]->SetText( nValue );
		NTL_RETURN( TRUE );
	}

	CRectangle rect;
	rect.SetRect( m_artQuickSlot[nRowIdx][nColIdx].left, m_artQuickSlot[nRowIdx][nColIdx].bottom - QUICK_STACK_HEIGHT,
				  m_artQuickSlot[nRowIdx][nColIdx].right + QUICK_STACK_WIDTH, m_artQuickSlot[nRowIdx][nColIdx].bottom );

	m_apstbStackNum[nRowIdx][nColIdx] = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), DBOGUI_STACKNUM_ALIGN );
	if( !m_apstbStackNum[nRowIdx][nColIdx] )
		NTL_RETURN( FALSE );

	m_apstbStackNum[nRowIdx][nColIdx]->CreateFontStd(DBOGUI_STACKNUM_FONT, DBOGUI_STACKNUM_FONTHEIGHT, DBOGUI_STACKNUM_FONTATTR);
	m_apstbStackNum[nRowIdx][nColIdx]->SetTextStyle(COMP_TEXT_LEFT); // < for some reason, the number is not shown without this.
	m_apstbStackNum[nRowIdx][nColIdx]->SetEffectMode(DBOGUI_STACKNUM_FONTEFFECTMODE);
	m_apstbStackNum[nRowIdx][nColIdx]->SetText( nValue );
	m_apstbStackNum[nRowIdx][nColIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CQuickSlotGui::DestroyStackNumber( RwInt32 nSlotIdx )
{
	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return;

	RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

	NTL_DELETE( m_apstbStackNum[nRowIdx][nColIdx] );
}

RwBool CQuickSlotGui::CreateFlashEffect( RwInt32 nSlotIdx )
{
	NTL_FUNCTION( "CQuickSlotGui::CreateFlashEffect" );

	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		NTL_RETURN( FALSE );

	RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

	if( m_apflaEffect[nRowIdx][nColIdx]->IsVisible() )
		NTL_RETURN( TRUE );
	
	// 
	m_apflaEffect[nRowIdx][nColIdx]->GotoFrame( m_uiEffectFrame );
	m_apflaEffect[nRowIdx][nColIdx]->PlayMovie( TRUE );
	m_apflaEffect[nRowIdx][nColIdx]->Show( true );
	//

	// peessi : Flash 로딩시 부하가 심해 미리 로딩해서 사용함. 
	//if( m_apflaEffect[nRowIdx][nColIdx] )
	//	NTL_RETURN( TRUE );

	//CRectangle rect;
	//rect.SetRectWH( m_artQuickSlot[nRowIdx][nColIdx].left - 5, m_artQuickSlot[nRowIdx][nColIdx].top - 5, 42, 42 );				  

	//m_apflaEffect[nRowIdx][nColIdx] = NTL_NEW gui::CFlash( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), "Skill_action.swf" );
	//if( !m_apflaEffect[nRowIdx][nColIdx] )
	//	NTL_RETURN( FALSE );

	//m_apflaEffect[nRowIdx][nColIdx]->PlayMovie( TRUE );
	//m_apflaEffect[nRowIdx][nColIdx]->Enable( false );

	NTL_RETURN( TRUE );
}

VOID CQuickSlotGui::DestroyFlashEffect( RwInt32 nSlotIdx )
{
	if( !IsValidVisibleSlotIdx( nSlotIdx ) )
		return;

	RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

	if( !m_apflaEffect[nRowIdx][nColIdx]->IsVisible() )
		return;

	// 
	m_apflaEffect[nRowIdx][nColIdx]->PlayMovie( FALSE );
	m_apflaEffect[nRowIdx][nColIdx]->Show( false );
	//

	// peessi : Flash 로딩시 부하가 심해 미리 로딩해서 사용함. 
	//NTL_DELETE( m_apflaEffect[nRowIdx][nColIdx] );
}

VOID CQuickSlotGui::CoolingEffectProc(RwUInt32 hSerial, BOOL bStart )
{
	for( RwInt32 i = 0 ; i < m_nVisibleRowCount ; ++i )
	{
		for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			RwInt32 nDataRowIndex = GetDataRowIndex( i );

			if( nDataRowIndex >= 0 )
			{
				CQuickSlotItem::stQUICKSLOTOBJ* pQuickSlotObj = m_aQuickSlotItem[nDataRowIndex][j].HasObj( hSerial );
				if( pQuickSlotObj )
				{
					if( bStart )
					{
						m_CoolTimeEffect[i][j].StartProc( pQuickSlotObj->pIcon->GetMaxCoolingTime() );
					}
					else
					{
						m_CoolTimeEffect[i][j].EndProc();
					}			
				}
			}
		}
	}		
}

RwBool CQuickSlotGui::ClickEffect( RwBool bPush, RwInt32 nSlotIdx /* = -1  */)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	// TRUE = 다운, FALSE = 업
	if( bPush )
	{
		if( !IsValidVisibleSlotIdx( nSlotIdx ) )
			return FALSE;

		RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
		RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

		// 이미 키보드로 눌려져 있는 키라면 막아준다.
		if( m_abPushDownKey[nRowIdx][nColIdx] )
			return FALSE;

		m_asurIcon[nRowIdx][nColIdx].SetRect( rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].top + ICONPUSH_SIZEDIFF,
											  rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].bottom - ICONPUSH_SIZEDIFF );
	}
	else if( m_nPushDownIndex >= 0 )
	{
		RwInt32 nRowIdx = SLOTID_TO_ROW( m_nPushDownIndex );
		RwInt32 nColIdx = SLOTID_TO_COL( m_nPushDownIndex );

		m_asurIcon[nRowIdx][nColIdx].SetRect( rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].left, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].top,
											  rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].right, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].bottom );
	}	
	else
	{
		return FALSE;	
	}

	// 마우스로 누른 Index를 현재의 Index
	m_nPushDownIndex = nSlotIdx;

	return TRUE;
}

RwBool CQuickSlotGui::KeyClickEffect( RwBool bPush, RwInt32 nSlotIdx )
{
	if( m_nPushDownIndex == nSlotIdx )
		return FALSE;

	if( !IsValidVisibleSlotIdx( nSlotIdx ) )    
        return FALSE;

	CRectangle rtScreen = m_pThis->GetScreenRect();

	RwInt32 nRowIdx = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIdx = SLOTID_TO_COL( nSlotIdx );

	if( bPush )
	{
		m_asurIcon[nRowIdx][nColIdx].SetRect( rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].top + ICONPUSH_SIZEDIFF,
											  rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].bottom - ICONPUSH_SIZEDIFF );

		// 이미 눌려져 있다면 FALSE를 리턴
		if( m_abPushDownKey[nRowIdx][nColIdx] )
			return FALSE;
		else 
			m_abPushDownKey[nRowIdx][nColIdx] = TRUE;
	}
	else
	{
		m_asurIcon[nRowIdx][nColIdx].SetRect( rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].left, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].top,
											  rtScreen.left + m_artQuickSlot[nRowIdx][nColIdx].right, rtScreen.top + m_artQuickSlot[nRowIdx][nColIdx].bottom );
		
		m_abPushDownKey[nRowIdx][nColIdx] = FALSE;
	}	

	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_LPEPRP( CNtlSobAttr* pAttr )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	if( pAttr->GetClassID() == SLCLASS_SKILL )
	{
		CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pAttr );

		if( pSkillAttr->GetSkillTbl()->dwRequire_LP > (DWORD)pAvatarAttr->GetLp() )
			return FALSE;

		if( pSkillAttr->GetSkillTbl()->wRequire_EP > pAvatarAttr->GetEp() )
			return FALSE;

		if( pSkillAttr->GetSkillTbl()->dwFunction_Bit_Flag & SKILL_FUNCTION_ONLY_LOW_LP)		// Enum Definition Required
		{
			if( pAvatarAttr->GetLp() > pAvatarAttr->GetMaxLp() / 2 )
				return FALSE;
		}
	}	
	else if( pAttr->GetClassID() == SLCLASS_HTBSKILL )
	{
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

		if(pAvatarInfo->sCharPf.byCurRPBall <= 0 )
			return FALSE;
	}

	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_TargetState( sSKILL_TBLDAT* pData )
{
	if( !pData )
		return TRUE;
	
	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_ItemNeed( sSKILL_TBLDAT* pData )
{
	if( !pData )
		return TRUE;

	RwUInt8 byRequireItemType = pData->byRequire_Item_Type;

	if( byRequireItemType != INVALID_BYTE )
	{
		// 현재 필요 아이템 타입은 무기, 보조무기만으로 지정되어 있다. 			
		if( Check_EquippedSlot( EQUIP_SLOT_TYPE_HAND, byRequireItemType ) )
			return TRUE;

		if( Check_EquippedSlot( EQUIP_SLOT_TYPE_SUB_WEAPON, byRequireItemType ) )
			return TRUE;

		return FALSE;
	}	

	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_TargetApply( sSKILL_TBLDAT* pData )
{
	if( !pData )
		return FALSE;

	if( pData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_SELF )
	{
		return TRUE;
	}
	else if( pData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET )
	{
		RwUInt32 hTargetSerial = Logic_GetAvatarTargetHandle();

		if( !Dbo_IsHarmfulEffectType( pData->bySkill_Active_Type ) && !Logic_IsSkillSystemEffectCode( pData->tblidx, ACTIVE_WARP ) )
			return TRUE;

		if( hTargetSerial == INVALID_SERIAL_ID )
		{
			return FALSE;			
		}
		else
		{
			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hTargetSerial );
			
			if( !pSobObj )
			{
				return FALSE;
			}

			if( pData->byApply_Range == DBO_SKILL_APPLY_RANGE_SINGLE )
			{
				if( pSobObj->GetClassID() == SLCLASS_AVATAR )
				{
					if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_SELF ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_PARTY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
						return TRUE;
					else
						return FALSE;
				}
				else if( pSobObj->GetClassID() == SLCLASS_NPC )							 
				{
					if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_NPC ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
						return TRUE;
					else
						return FALSE;
				}
				else if( pSobObj->GetClassID() == SLCLASS_MONSTER )
				{
					if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ENEMY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
						pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_MOB)
						return TRUE;
					else
						return FALSE;
				}
				else if( pSobObj->GetClassID() == SLCLASS_PLAYER || pSobObj->GetClassID() == SLCLASS_PET )
				{
					if( GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor( NULL, hTargetSerial ) )
					{
						if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ENEMY ||
							pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
							pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
							return TRUE;
						else
							return FALSE;
					}
					else
					{
						if( pSobObj->GetClassID() == SLCLASS_PET )
						{
							if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
								pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
								pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
								return TRUE;
							else
								return FALSE;		
						}
						else
						{
							if( Logic_IsMyPartyMember( hTargetSerial ) )
							{
								if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
									pData->byApply_Target == DBO_SKILL_APPLY_TARGET_PARTY ||
									pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
									pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
									return TRUE;
								else 
									return FALSE;
							}
							else
							{
								if( pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ALLIANCE ||
									pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY ||
									pData->byApply_Target == DBO_SKILL_APPLY_TARGET_ANY_ALLIANCE)
									return TRUE;
								else 
									return FALSE;
							}								
						}
					}
				}
			}
			else
			{
				return TRUE;
			}
		}
	}
	else
	{
		DBO_WARNING_MESSAGE("byAppoint_Target not set: " << pData->byAppoint_Target);
	}
	
	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_TargetApply( sACTION_TBLDAT* pData )
{
	if( !pData )
		return FALSE;

	if( pData->byAction_Type == ACTION_TYPE_NORMAL_ACTION )
	{
		RwUInt32 hTargetSerial = Logic_GetAvatarTargetHandle();

		if( pData->byETC_Action_Type == ACTION_FUNC_TYPE_BASIC_ATTACK )
		{
			if( hTargetSerial == INVALID_SERIAL_ID )
				return FALSE;

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hTargetSerial );

			if( pSobObj->GetClassID() == SLCLASS_MONSTER )
			{
				return TRUE;
			}
			else if( pSobObj->GetClassID() == SLCLASS_PLAYER ||
					 pSobObj->GetClassID() == SLCLASS_PET )
			{
				if( GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor( NULL, hTargetSerial ) )
					return TRUE;
			}

			return FALSE;			
		}
		else if( pData->byETC_Action_Type == ACTION_FUNC_TYPE_ASSIST )
		{
			if( hTargetSerial == INVALID_SERIAL_ID )
				return FALSE;

			CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hTargetSerial );

			if( pSobObj->GetClassID() == SLCLASS_PLAYER )			
			{
				if( GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor( NULL, hTargetSerial ) )
					return FALSE;
				
				return TRUE;
			}
			
			return FALSE;
		}
	}

	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_AvatarState( sSKILL_TBLDAT* pData )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sCHARSTATE* pState = pAvatar->GetServerFullState();
	if (!pState)
	{
		NTL_RETURN(FALSE);
	}

	WORD wTemp;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pAvatar->GetBehaviorData());
	
	// only check aspectstate
	BYTE byCharState = CHARSTATE_STANDING;
	BYTE stunType = INVALID_BYTE;// pBeData->GetCtrlStuff()->uExtra.sStun.byStunType;
	BYTE aspectStateID = pAvatar->GetAspectState()->sAspectStateBase.byAspectStateId;
	QWORD qwCurCharCondition = pState->sCharStateBase.dwConditionFlag;
	bool bIsSwimming = false;
	bool bIsJumping = false;
	BYTE airState = AIR_STATE_OFF;
	
	bool bValidState = Dbo_IsValidStateToUseSkill(pData->dwTransform_Use_Info_Bit_Flag, byCharState, stunType,
		aspectStateID, qwCurCharCondition, bIsSwimming, bIsJumping,
		airState, &wTemp);

	if(!bValidState)
	{
		NTL_RETURN(FALSE);
	}

	return TRUE;
}

RwBool CQuickSlotGui::UseableCheck_AvatarState(VOID)
{
	// 변신중일때는 일반 스킬은 사용하지 못한다	
	if(Logic_IsTransform(GetNtlSLGlobal()->GetSobAvatar()))
	{
		BYTE byAspectStateID = GetNtlSLGlobal()->GetSobAvatar()->GetAspectState()->sAspectStateBase.byAspectStateId;
		switch(byAspectStateID)
		{
			case ASPECTSTATE_SPINNING_ATTACK:
				return FALSE;
			
			case ASPECTSTATE_VEHICLE:
				return FALSE;		

			case ASPECTSTATE_ROLLING_ATTACK:
				return FALSE;
		}
	}

	return TRUE;
}

RwBool CQuickSlotGui::Check_EquippedSlot( RwInt32 nEquipSlot, RwUInt8 byRequireItemType )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwUInt32 hEquipSerial = pAvatar->GetInventory()->GetEquipItem( nEquipSlot );
	CNtlSobItem* pEquipItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipSerial ) );
	CNtlSobItemAttr* pEquipItemAttr = NULL;

	if( pEquipItem )
	{
		pEquipItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pEquipItem->GetSobAttr() );
		if( pEquipItemAttr->GetItemTbl()->byItem_Type == byRequireItemType )
			return TRUE;
	}	

	return FALSE;
}

RwInt32 CQuickSlotGui::GetInfoWndType( RwUInt8 uiSLClassID )
{
	RwInt32 eType = CInfoWndManager::INFOWND_NOTSHOW;

	switch( uiSLClassID )
	{
	case SLCLASS_ITEM_ICON:	eType = CInfoWndManager::INFOWND_ITEM; break;
	case SLCLASS_SKILL_ICON: eType = CInfoWndManager::INFOWND_SKILL; break;
	case SLCLASS_HTBSKILL_ICON: eType = CInfoWndManager::INFOWND_HTBSKILL; break;
	case SLCLASS_ACTIONSKILL_ICON: eType = CInfoWndManager::INFOWND_ACTIONSKILL; break;
	}

	return eType;
}

RwInt32 CQuickSlotGui::GetDataRowIndex( RwInt32 nVisibleRowIndex )
{
	RwInt32 nResult = -1;

	if( nVisibleRowIndex < 0 || nVisibleRowIndex >= m_nVisibleRowCount )
	{
		DBO_ASSERTE( "CQuickSlotGui::GetDataRowIndex : Wrong Parameter, nVisibleRow" );
		return nResult;
	}

	RwInt32 nVisibleCurrentIndex = m_nVisibleRowCount - 1;			
	RwInt32 nIndexOffset = nVisibleCurrentIndex - nVisibleRowIndex;	// 항상 CurrentIndex가 크다.
	nResult = ( m_nCurrentRow + nIndexOffset ) % QUICKSLOT_MAXROW;

	return nResult;
}

RwInt32 CQuickSlotGui::GetVisibleRowIndex( RwInt32 nDataRowIndex )
{
	if( nDataRowIndex < 0 || nDataRowIndex >= QUICKSLOT_MAXROW )
	{
		DBO_ASSERTE( "CQuickSlotGui::GetVisibleRowIndex : Wrong Parameter, nDataRow" );
		return -1;
	}

	RwInt32 nVisibleCurrentIndex = m_nVisibleRowCount - 1;
	RwInt32 nTempDataIndex = m_nCurrentRow;

	for( RwInt32 i = nVisibleCurrentIndex ; i >= 0 ; --i )
	{
		if( nDataRowIndex == nTempDataIndex )
			return i;

		nTempDataIndex = ( nTempDataIndex + 1 ) % QUICKSLOT_MAXROW;
	}

	return -1;
}

RwBool CQuickSlotGui::IsValidRowIndex( RwInt32 nRowIndex )
{
	if( nRowIndex < 0 || nRowIndex >= QUICKSLOT_MAXROW )
	{
		DBO_ASSERTE( "CQuickSlotGui::IsValidRowIndex : Wrong RowIndex" );
		return FALSE;
	}

	return TRUE;
}

RwBool CQuickSlotGui::IsValidColIndex( RwInt32 nColIndex )
{
	if( nColIndex < 0 || nColIndex >= QUICKSLOT_MAXCOL )
	{
		DBO_ASSERTE( "CQuickSlotGui::IsValidColIndex : Wrong ColIndex" );
		return FALSE;
	}

	return TRUE;
}

RwBool CQuickSlotGui::IsValidVisibleRowIndex( RwInt32 nVisibleRowIndex )
{
	if( nVisibleRowIndex < 0 || nVisibleRowIndex >= m_nVisibleRowCount )
	{
		DBO_ASSERTE( "CQuickSlotGui::IsValidVisibleRowIndex : Wrong VisibleRowIndex" );			
		return FALSE;
	}

	return TRUE;
}

RwBool CQuickSlotGui::IsValidSlotIdx( RwInt32 nSlotIdx )
{
	RwInt32 nRowIndex = SLOTID_TO_ROW( nSlotIdx );
	RwInt32 nColIndex = SLOTID_TO_COL( nSlotIdx );

	if( !IsValidRowIndex( nRowIndex ) )
		return FALSE;

	if( !IsValidColIndex( nColIndex ) )
		return FALSE;

	return TRUE;
}

RwBool CQuickSlotGui::IsValidVisibleSlotIdx( RwInt32 nVisibleSlotIdx )
{
	RwInt32 nVisibleRowIndex = SLOTID_TO_ROW( nVisibleSlotIdx );
	RwInt32 nColIndex = SLOTID_TO_COL( nVisibleSlotIdx );

	if( !IsValidVisibleRowIndex( nVisibleRowIndex ) )
		return FALSE;

	if( !IsValidColIndex( nColIndex ) )
		return FALSE;

	return TRUE;
}

RwBool CQuickSlotGui::IsVisibleRow( RwInt32 nDataRow )
{
	RwInt32 nVisibleIndex = GetVisibleRowIndex( nDataRow );

	if( nVisibleIndex < 0 || nVisibleIndex >= m_nVisibleRowCount )
		return FALSE;

	return TRUE;
}

VOID CQuickSlotGui::SetVisibleRowCount( RwInt32 nVisibleRowCount )
{
	DBO_ASSERT( nVisibleRowCount > 0 && nVisibleRowCount < 4, "Quickslot VisibleCount Error!!!" );

	m_pThis->SetHeight( ( nVisibleRowCount * 36 ) + 3 );
	m_pRowNum->SetPosition( 422, ( nVisibleRowCount - 1 ) * 36 + 12 );
	m_pUpRowBtn->SetPosition( 421, ( nVisibleRowCount - 1 ) * 36 + 2 );
	m_pDownRowBtn->SetPosition( 421, ( nVisibleRowCount - 1 ) * 36 + 25 );

	switch( nVisibleRowCount )
	{
	case 1:
		for( RwInt32 i =0; i < QUICKSLOT_MAXCOL; ++i )
		{
			m_apStbShortCutName[0][i]->Show( true );
			m_apStbShortCutName[1][i]->Show( false );
			m_apStbShortCutName[2][i]->Show( false );		
		}

		m_anQuickSlotRow[ROW_DEFAULT] = 0;
		m_anQuickSlotRow[ROW_EX] = 1;
		m_anQuickSlotRow[ROW_EX2] = 2;

		break;
	case 2:
		for( RwInt32 i =0; i < QUICKSLOT_MAXCOL; ++i )
		{
			m_apStbShortCutName[0][i]->Show( true );
			m_apStbShortCutName[1][i]->Show( true );
			m_apStbShortCutName[2][i]->Show( false );
		}
		
		m_anQuickSlotRow[ROW_DEFAULT] = 1;
		m_anQuickSlotRow[ROW_EX] = 0;
		m_anQuickSlotRow[ROW_EX2] = 2;

		break;
	case 3:
		for( RwInt32 i =0; i < QUICKSLOT_MAXCOL; ++i )
		{
			m_apStbShortCutName[0][i]->Show( true );
			m_apStbShortCutName[1][i]->Show( true );
			m_apStbShortCutName[2][i]->Show( true );
		}

		m_anQuickSlotRow[ROW_DEFAULT] = 2;
		m_anQuickSlotRow[ROW_EX] = 1;
		m_anQuickSlotRow[ROW_EX2] = 0;

		break;
	}

	// peessi : GUIComponent는 삭제해줘야 한다. 
	for( RwInt32 i = nVisibleRowCount ; i < QUICKSLOT_MAX_VISIBLE_ROW ; ++i )
	{
		for( RwInt32 j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
		//	DestroyFlashEffect( ROWCOL_TO_SLOTID( i, j ) );
			DestroyStackNumber( ROWCOL_TO_SLOTID( i, j ) );
		}
	}

	SwapKeyInputData( m_nVisibleRowCount, nVisibleRowCount );

	m_nVisibleRowCount = nVisibleRowCount;

	UpdateAllSlot( CHECK_FLAG_ALL );	

	// 변경된 사항에 따라 단축키 이름을 다시 세팅한다.
	RegisterShortCutkeyName();

	GetDialogManager()->LocationDialogs( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
}

RwBool CQuickSlotGui::CheckIconMoveLock(VOID)
{
	if( m_bIconMoveLock )
	{
		GetAlarmManager()->AlarmMessage( "DST_QUICKSLOT_IS_LOCK" );
	}

	return m_bIconMoveLock;
}

VOID CQuickSlotGui::SwapKeyInputData( RwInt32 nCurrentVisibleCount, RwInt32 nChangeVisibleCount )
{
	RwInt32		abTempPushDownKey[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL] = { 0, };			
	RwReal		afTempPushDownKeyElapsed[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL] = { 0.0f, };	
	RwBool		abTempIsProcDownKey[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL] = { FALSE, };		
	RwInt32		abTempEnableIgnoreUp[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL] = { 0, };	

	RwInt32 i = 0, j = 0;
	RwInt32 nOffset = nChangeVisibleCount - nCurrentVisibleCount;

	if( nOffset < 0 )
	{
		RwInt32 nValue = 0;
		for( i = 0 ; i > nOffset ; --i )
		{
			if( --nValue < 0 )
				nValue = QUICKSLOT_MAX_VISIBLE_ROW - 1;			
		}

		nOffset = nValue;
	}

	if( nOffset == 0 )
		return;

	for( i = 0 ; i < QUICKSLOT_MAX_VISIBLE_ROW ; ++i )
	{
		RwInt32 nDestRowIndex = ( i + nOffset ) % QUICKSLOT_MAX_VISIBLE_ROW;

		for( j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			abTempPushDownKey[nDestRowIndex][j] = m_abPushDownKey[i][j];
			afTempPushDownKeyElapsed[nDestRowIndex][j] = m_afPushDownKeyElapsed[i][j];
			abTempIsProcDownKey[nDestRowIndex][j] = m_abIsProcDownKey[i][j];
			abTempEnableIgnoreUp[nDestRowIndex][j] = m_abEnableIgnoreUp[i][j];			

			if( m_abPushDownKey[i][j] )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();				

				m_asurIcon[i][j].SetRect( rtScreen.left + m_artQuickSlot[i][j].left, rtScreen.top + m_artQuickSlot[i][j].top,
										  rtScreen.left + m_artQuickSlot[i][j].right, rtScreen.top + m_artQuickSlot[i][j].bottom );				
			}
		}
	}	

	for( i = 0 ; i < QUICKSLOT_MAX_VISIBLE_ROW ; ++i )
	{
		for( j = 0 ; j < QUICKSLOT_MAXCOL ; ++j )
		{
			m_abPushDownKey[i][j] = abTempPushDownKey[i][j];
			m_afPushDownKeyElapsed[i][j] = afTempPushDownKeyElapsed[i][j];
			m_abIsProcDownKey[i][j] = abTempIsProcDownKey[i][j];
			m_abEnableIgnoreUp[i][j] = abTempEnableIgnoreUp[i][j];

			if( m_abPushDownKey[i][j] )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();

				m_asurIcon[i][j].SetRect( rtScreen.left + m_artQuickSlot[i][j].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[i][j].top + ICONPUSH_SIZEDIFF,
										  rtScreen.left + m_artQuickSlot[i][j].right - ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[i][j].bottom - ICONPUSH_SIZEDIFF );				
			}
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////
// ! CallBack

VOID CQuickSlotGui::OnClickedUpRow( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_CHANGE ) )
		return;

	if( --m_nCurrentRow < 0 )
		m_nCurrentRow = QUICKSLOT_MAXROW - 1;

	m_pRowNum->SetText( m_nCurrentRow + 1 );
	UpdateAllSlot( CHECK_FLAG_ALL );	
}

VOID CQuickSlotGui::OnClickedDownRow( gui::CComponent* pComponent )
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_CHANGE ) )
		return;

	if( ++m_nCurrentRow >= QUICKSLOT_MAXROW )
		m_nCurrentRow = 0;

	m_pRowNum->SetText( m_nCurrentRow + 1 );
	UpdateAllSlot( CHECK_FLAG_ALL );	
}

VOID CQuickSlotGui::OnMouseDown( const CKey& key )
{
	// When OnMouseDown is called, take the index of the quick slot with the current mouse position
	// If the mouse is included in each index of the quick slot, it gives the pressed effect on the index.

	RwInt32 nClickIdx = GetQuickSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY );

	if( nClickIdx >= 0 && !GetIconMoveManager()->IsActive() )
		ClickEffect( TRUE, nClickIdx );
	
	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( GetIconMoveManager()->IsActive() )
		{
			if (IsEnablePutDown(nClickIdx))
				m_nLSelectedSlotIdx = nClickIdx;
		}
		else
		{
			if( !CheckIconMoveLock() )
			{
				if( IsEnableClick( nClickIdx ) )
					m_nLSelectedSlotIdx = nClickIdx;			
			}
		}		
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( !GetIconMoveManager()->IsActive() )
		{
			if( IsEnableClick( nClickIdx ) )
			{
				m_nRSelectedSlotIdx = nClickIdx;
				m_fPushDownMouseElapsed = 0.0f;
				m_nEnableIgnoreIndex = -1;
				m_bIsProcMouseSelected = FALSE;
			}
		}		
	}

	m_pThis->CaptureMouse();

	if( m_nLSelectedSlotIdx >= 0 && m_nRSelectedSlotIdx >= 0 )
	{
		m_nLSelectedSlotIdx = -1;
		m_nRSelectedSlotIdx = -1;
		ClickEffect( FALSE );
		m_pThis->ReleaseMouse();
	}
}

VOID CQuickSlotGui::OnMouseUp( const CKey& key )
{
	// 1. Left button or right button
	// 2. Select Src or Dest.

	RwInt32 nSlotIdx = GetQuickSlotIdx( (RwInt32)key.m_fX, (RwInt32)key.m_fY ); 
	RwBool bClick = ClickEffect( FALSE );

	m_pThis->ReleaseMouse();
	
	if( nSlotIdx < 0 || !IsShow() )
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
				RwBool bReg;
				if( m_nLSelectedSlotIdx == ROWCOL_TO_SLOTID( GetVisibleRowIndex( 0 ), 0 ) )
				{
					bReg = Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_REG );
				}
				else
				{
					bReg = Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_REG );
				}

				// 아이콘 PutDown
				if( bReg )
					IconPutDownProc( GetIconMoveManager()->GetSrcSerial(), GetIconMoveManager()->GetSrcPlace(), nSlotIdx );
			}
			else
			{
				RwBool bUnreg = TRUE;
				if( m_nLSelectedSlotIdx == ROWCOL_TO_SLOTID( GetVisibleRowIndex( 0 ), 0 ) )
				{
					bUnreg = Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_UNREG );
				}
				else
				{
					bUnreg = Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_UNREG );
				}

				if( bUnreg )
				{
					RwInt32 nVisibleRowIndex = SLOTID_TO_ROW( nSlotIdx );
					RwInt32 nColIndex = SLOTID_TO_COL( nSlotIdx );
					RwInt32 nDataRowIndex = GetDataRowIndex( nVisibleRowIndex );

					if( nDataRowIndex >= 0 )
					{
						if( key.m_dwVKey & UD_MK_CONTROL )
						{
							if( !CheckIconMoveLock() )
							{
								if (nVisibleRowIndex < QUICKSLOT_MAX_VISIBLE_ROW && nColIndex < QUICKSLOT_MAXCOL)
									m_CoolTimeEffect[nVisibleRowIndex][nColIndex].EndProc();
								UnsetIconFromQuickSlot( nDataRowIndex, nColIndex );
								UpdateAllSlot();
							}							
						}
						else
						{
							if( !CheckIconMoveLock() )
								GetIconMoveManager()->IconMovePickUp( m_aQuickSlotItem[nDataRowIndex][nColIndex].GetFirstSerialID(), PLACE_QUICKSLOT, ROWCOL_TO_SLOTID( nVisibleRowIndex, nColIndex ), 0, m_aQuickSlotItem[nDataRowIndex][nColIndex].GetFirstIcon()->GetImage() );					
						}
					}									
				}
			}
		}
		else if (m_nLSelectedSlotIdx == -1)
		{
			RwInt32 nVisiblwRow = SLOTID_TO_ROW(nSlotIdx);
			RwInt32 nDataRow = GetDataRowIndex(nVisiblwRow);
			RwInt32 nCol = SLOTID_TO_COL(nSlotIdx);

			if (!IsValidRowIndex(nDataRow))
				return;

			if (m_aQuickSlotItem[nDataRow][nCol].GetFirstSerialID() == GetIconMoveManager()->GetSrcSerial()) // if we move same icon over same icon..
			{
				GetIconMoveManager()->IconMoveEnd();
				return;
			}
		}

		m_nLSelectedSlotIdx = -1;
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( nSlotIdx == m_nRSelectedSlotIdx )
		{
			m_bIsProcMouseSelected = FALSE;

			if( bClick )
			{
				// 현재 인덱스가 무시하기로 한 인덱스가 아니라면 실행
				if( m_nEnableIgnoreIndex != nSlotIdx )
				{
					RwBool bExcute = false;

					if( m_nRSelectedSlotIdx == ROWCOL_TO_SLOTID( 0, 0 ) )
					{
						if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_1_EXCUTE ) )
							bExcute = true;
					}
					else
					{
						if( Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_QUICK_SLOT_2_LAST_EXCUTE ) )
							bExcute = true;
					}		

					if( bExcute )
						IconUseProc( nSlotIdx );
				}
			}
		}

		m_nRSelectedSlotIdx = -1;
	}
}

VOID CQuickSlotGui::OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY )
{
	RwBool bFlag = FALSE;

	for( RwInt32 j = 0 ; j < m_nVisibleRowCount ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			if( m_artQuickSlot[j][i].PtInRect( nX, nY ) )
			{
				bFlag = TRUE;
				m_anFocusEffect[j][i] = m_anFocusEffect[j][i] | SLOT_FOCUS;
				RwInt32 nCurrentMouseIndex = ROWCOL_TO_SLOTID( j, i );
				
				if( m_nMouseOnIndex != nCurrentMouseIndex )
				{
					RwInt32 nDataRowIndex = GetDataRowIndex( j );

					if( nDataRowIndex >= 0 )
					{
						CQuickSlotItem* pQuickSlotItem = &m_aQuickSlotItem[nDataRowIndex][i];
						if( pQuickSlotItem->IsValid() )
						{
							CRectangle rtScreen = m_pThis->GetScreenRect();										
							m_nMouseOnIndex = nCurrentMouseIndex;													
							GetInfoWndManager()->ShowInfoWindow( TRUE, GetInfoWndType( pQuickSlotItem->GetSLClassID() ), m_artQuickSlot[j][i].left + rtScreen.left, m_artQuickSlot[j][i].top + rtScreen.top, pQuickSlotItem->GetFirstIcon()->GetSobObj(), DIALOG_QUICKSLOT );


							////////////////////////////////////////////////////////////////////////////
							//// Range Skill Effect
							//CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>(pQuickSlotItem->GetFirstIcon()->GetSobObj()->GetSobAttr());
							//sSKILL_TBLDAT *pSkillTblData = pSkillAttr->GetSkillTbl();

							//CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

							//if (pSkillTblData->byApply_Range == DBO_SKILL_APPLY_RANGE_CIRCLE)
							//{
							//	std::string strEffectName;
							//	RwV3d vPos = ZeroAxis;
							//	CNtlSob* pSobTarget = pAvatar;

							//	if (Logic_IsSkillSystemEffectCode(pSkillTblData->tblidx, SKILL_ACTIVE_TYPE_DH) ||
							//		Logic_IsSkillSystemEffectCode(pSkillTblData->tblidx, SKILL_ACTIVE_TYPE_HOT))
							//	{
							//		strEffectName = "GME_HEAL_RANGE";
							//	}
							//	else
							//	{
							//		strEffectName = "GME_ENABLE_SPAWN_DECAL";
							//	}

							//	if (pSkillTblData->byAppoint_Target == DBO_SKILL_APPOINT_TARGET_TARGET)
							//	{
							//		pSobTarget = GetNtlSobManager()->GetSobObject(pAvatar->GetTargetID());
							//	}

							//	if (pSobTarget)
							//	{
							//		pEffectRange = pSobTarget->GetSobProxy()->CreatePLEffect(strEffectName.c_str(), &pSobTarget->GetPosition());
							//		pEffectRange->SetScale((float)pSkillTblData->byApply_Area_Size_1);

							//		CNtlInstanceEffect *pPLEffect = reinterpret_cast<CNtlInstanceEffect*>(pEffectRange);
							//		DBO_WARNING_MESSAGE("NO AUTO DELETE: " << pPLEffect->IsVisible());
							//	}
							//}
						}
						else
							bFlag = FALSE;
					}
					else
					{
						bFlag = FALSE;
					}					

					if( nCurrentMouseIndex == m_nPushDownIndex )
						ClickEffect( TRUE, nCurrentMouseIndex );
					else if( m_nPushDownIndex >= 0 )
						ClickEffect( FALSE, m_nPushDownIndex );
				}			
			}
			else
			{
				m_anFocusEffect[j][i] = m_anFocusEffect[j][i] & ~SLOT_FOCUS;
			}
		}	
	}	

	if( !bFlag && m_nMouseOnIndex != -1 )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
		if( m_nPushDownIndex >= 0 )
			ClickEffect( FALSE, m_nPushDownIndex );
		m_nMouseOnIndex = -1;
		m_nRSelectedSlotIdx = -1;
	}
}

VOID CQuickSlotGui::OnMouseLeave( gui::CComponent* pComponent )
{
	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
			m_anFocusEffect[j][i] = m_anFocusEffect[j][i] & ~SLOT_FOCUS;

		if( m_nMouseOnIndex != -1 )
		{
			GetInfoWndManager()->ShowInfoWindow( FALSE );
			if( m_nPushDownIndex >= 0 )
				ClickEffect( FALSE, m_nPushDownIndex );
			m_nMouseOnIndex= -1;
			m_nRSelectedSlotIdx = -1;
		}
	}		
}

VOID CQuickSlotGui::OnPaint(VOID)
{
	for( RwInt32 j = 0 ; j < m_nVisibleRowCount ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			m_asurSlot[j][i].Render();

			if( m_asurIcon[j][i].GetTexture() )
				m_asurIcon[j][i].Render();

			m_asurRPType[j][i].Render();

			if( m_anFocusEffect[j][i] )
				m_asurFocusSlot[j][i].Render();

			m_CoolTimeEffect[j][i].Render();

			m_asurDisableSkill[j][i].Render();
			m_asurDisableSlot[j][i].Render();
			/*m_asurKeyImage[j][i].Render();			*/
		}	
	}	
}

VOID CQuickSlotGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwInt32 j = 0 ; j < QUICKSLOT_MAX_VISIBLE_ROW ; ++j )
	{
		for( RwInt32 i = 0 ; i < QUICKSLOT_MAXCOL ; ++i )
		{
			m_asurSlot[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );

			if( m_nPushDownIndex == ROWCOL_TO_SLOTID( j, i ) ||	m_abPushDownKey[j][i] )
				m_asurIcon[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left + ICONPUSH_SIZEDIFF, rtScreen.top + m_artQuickSlot[j][i].top + ICONPUSH_SIZEDIFF );				
			else
				m_asurIcon[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );
					
			m_asurFocusSlot[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );
			/*m_asurKeyImage[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );*/
			m_CoolTimeEffect[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );
			m_asurDisableSlot[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );
			m_asurDisableSkill[j][i].SetPosition( rtScreen.left + m_artQuickSlot[j][i].left, rtScreen.top + m_artQuickSlot[j][i].top );
			m_asurRPType[j][i].SetPositionbyParent( rtScreen.left, rtScreen.top );
		}
	}	
}