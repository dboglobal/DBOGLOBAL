#include "precomp_dboclient.h"
#include "PopupMenuGui.h"

// Shared and Table
#include "NtlStringUtil.h"
#include "WorldTable.h"
#include "ItemTable.h"
#include "TableContainer.h"

// Presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "InputActionMap.h"

// Dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "GUISoundDefine.h"
#include "DisplayStringManager.h"
#include "IconMoveManager.h"
#include "CursorManager.h"



#define ICONPOPUP_MENU_START_X		0
#define ICONPOPUP_MENU_START_Y		0
#define ICONPOPUP_ITEM_WIDTH		167
#define ICONPOPUP_ITEM_HEIGHT		22
#define ICONPOPUP_MARGIN_X			10
#define ICONPOPUP_MARGIN_Y			10
#define ICONPOPUP_SPACE				1

#define ICONPOPUP_POINT_MARGIN_X	5
#define ICONPOPUP_POINT_MARGIN_Y	5

////////////////////////////////////////////////////////////////////////////////
VOID CPopupGui::CaptureMouseDown( const CKey& key )
{
	if( !IsShow() )
		return;

	if( m_pThis->PosInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) != gui::CComponent::INRECT ) 
	{
		Show( false );
	}		
}

////////////////////////////////////////////////////////////////////////////////

CIconPopupItem::CIconPopupItem( CIconPopupGui* pIconPopup, gui::CButton* pButton, EIconPopupWork eWork )
{
	m_pIconPopup = pIconPopup;
	m_pButton	 = pButton;
	m_eWork		 = eWork;
	
	m_slotClick  = m_pButton->SigClicked().Connect( this, &CIconPopupItem::OnClick );
	m_pButton->Show( false );
}

CIconPopupItem::~CIconPopupItem(VOID)
{
	NTL_DELETE( m_pButton );
}

VOID CIconPopupItem::SetPosition( RwInt32 nX, RwInt32 nY )
{
	m_pButton->SetPosition( nX, nY );
}

VOID CIconPopupItem::SetColor( DWORD dwColorUp, DWORD dwColorDown, DWORD dwColorFoc, DWORD dwColorDis )
{
	m_pButton->SetTextUpColor( dwColorUp );
	m_pButton->SetTextDownColor( dwColorDown );
	m_pButton->SetTextFocusColor( dwColorFoc );
	m_pButton->SetTextDisableColor( dwColorDis );
	m_pButton->ApplyText();
}

VOID CIconPopupItem::Show( bool bShow )
{
	m_pButton->Show( bShow );
}

VOID CIconPopupItem::OnClick( gui::CComponent* pComponent )
{
	m_pIconPopup->Show( false );

	if( m_eWork == PMW_CANCEL )
	{
		return;
	}

	if( m_eWork == PMW_DELETE )
	{
		Logic_ItemDropConfirmProc();
	}
	else if( m_eWork == PMW_DIVIDE )
	{
		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_pIconPopup->GetSerial() ) );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		CRectangle rtScreen = m_pIconPopup->GetScreenRect();
		CDboEventGenerator::CalcPopupShow( TRUE, m_pIconPopup->GetSerial(), m_pIconPopup->GetPlace(), rtScreen.left, rtScreen.top, pItemAttr->GetStackNum() );
	}
	else if( m_eWork >= PMW_SET_SHARETARGET_1 && m_eWork <= PMW_UNSET_SHARETARGET_5 )
	{
		RwInt32 bySlot;

		if( m_eWork < PMW_UNSET_SHARETARGET_1 )
			bySlot = m_eWork - PMW_SET_SHARETARGET_1;
		else
			bySlot = m_eWork - PMW_UNSET_SHARETARGET_1;
		
		GetDboGlobal()->GetGamePacketGenerator()->SendPartyShareTargetReq( (RwUInt8)bySlot, m_pIconPopup->GetSerial() );
	}
	else if (m_eWork == PMW_ITEM_SORT)
	{
		//GetDboGlobal()->GetGamePacketGenerator()->SendSortItemReq();
	}
	else if (m_eWork == PMW_HELP)
	{
		CDboEventGenerator::OpenHelpContent(DIALOG_BAGSLOT);
	}
	else if (m_eWork == PMW_ITEM_DISASSEMBLE)
	{
		GetDialogManager()->OffMode();
		GetDialogManager()->OnMode(DIALOGMODE_ITEM_DISASSEMBLE);
	}
	else if (m_eWork == PMW_SCOUTER_CHECK_MENU_EQUIPMENT)
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPCInfoView(Logic_GetAvatarTargetHandle());
	}
	else
	{
		CDboEventGenerator::IconPopupResult( m_pIconPopup->GetSerial(), m_pIconPopup->GetPlace(), m_pIconPopup->GetSlotIdx(), m_eWork );
	}
}

////////////////////////////////////////////////////////////////////////////////

CIconPopupGui::CIconPopupGui(VOID)
{
	Init();
}

CIconPopupGui::CIconPopupGui( RwChar *szName )
: CPopupGui( szName )
{
	Init();
}

CIconPopupGui::~CIconPopupGui(VOID)
{

}

VOID CIconPopupGui::Init(VOID)
{
	memset( m_apMenu, 0, sizeof( CIconPopupItem* ) * NUM_PMW );
	m_ePlace = PLACE_NONE;
	m_hSerial = INVALID_SERIAL_ID;
}

RwBool CIconPopupGui::Create(VOID)
{
	NTL_FUNCTION( "CIconPopupGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\IconPopup.srf", "gui\\IconPopup.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	CRectangle rect;
	gui::CButton* pButton = NULL;
	gui::CSurfaceManager* pSurfaceMgr = GetNtlGuiManager()->GetSurfaceManager();
	rect.SetRectWH( 0, 0, ICONPOPUP_ITEM_WIDTH, ICONPOPUP_ITEM_HEIGHT );
	
	for( RwInt32 i = 0 ; i < NUM_PMW ; ++i )
	{
		pButton = NTL_NEW gui::CButton( rect, GetPopupMenuName( i ), m_pThis, pSurfaceMgr );
		pButton->AddSurfaceDown( pSurfaceMgr->GetSurface( "IconPopup.srf", "srfBtnDown" ) );
		pButton->AddSurfaceFocus( pSurfaceMgr->GetSurface( "IconPopup.srf", "srfBtnFocus" ) );
		pButton->SetTextUpColor( NTL_BUTTON_UP_COLOR );
		pButton->SetTextDownColor( NTL_BUTTON_DOWN_COLOR );
		pButton->SetTextFocusColor( NTL_BUTTON_FOCUS_COLOR );
		pButton->SetTextDisableColor( NTL_BUTTON_DISABLE_COLOR );
		pButton->SetTextDownCoordDiff( NTL_BUTTON_CLICK_DIFFX, NTL_BUTTON_CLICK_DIFFY );
		pButton->ApplyText();
		pButton->SetToolTip(GetPopupMenuTooltip(i));
		
		m_apMenu[i] = NTL_NEW CIconPopupItem( this, pButton, (EIconPopupWork)i );
	}

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CIconPopupGui::OnCaptureMouseDown );
	m_slotCaptureKeyUp = GetNtlGuiManager()->GetGuiManager()->SigCaptureKeyUp().Connect( this, &CIconPopupGui::OnCaptureKeyUp );

	Show( false );
	
	NTL_RETURN( TRUE );
}

VOID CIconPopupGui::Destroy(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PMW ; ++i )
	{
		if( m_apMenu[i] )
		{
			NTL_DELETE( m_apMenu[i] );
		}
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CIconPopupGui::ShowEvent( RWS::CMsg& msg )
{
	SDboEventIconPopupShow* pData = reinterpret_cast<SDboEventIconPopupShow*>( msg.pData );

	m_hSerial = pData->uiSerial;
	m_ePlace = (EPlace)pData->nSrcPlace;
	m_nSlotIdx = pData->nSrcSlotIdx;

	ResetItem();
	
	// Items in a bag
	if( pData->nSrcPlace == PLACE_BAG )
	{
		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hSerial) );
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
						
		if( pSobItem->IsStackable() )
		{
			// If you have items in the bag that overlap in the slots
			if( GetDialogManager()->IsOpenDialog(DIALOG_NPCSHOP) )
			{
				if( pSobItemAttr->IsNeedToIdentify() )
				{
					// 미확인 아이템					
					SelectItem( m_apMenu[PMW_PICKUP] );
				}
				else
				{
					if( pData->nSrcPlace == PLACE_BAG )
					{
						SelectItem( m_apMenu[PMW_SELL] );
					}

					SelectItem( m_apMenu[PMW_PICKUP] );
					SelectItem( m_apMenu[PMW_DIVIDE] );
				}				
			}
			else
			{
				if( pSobItemAttr->IsNeedToIdentify() )
				{
					// 미확인 아이템
					SelectItem( m_apMenu[PMW_PICKUP] );
					SelectItem( m_apMenu[PMW_DELETE] );
				}
				else
				{
					// 일반적인 경우
					if( reinterpret_cast<CNtlSobItemIcon*>( pSobItem->GetIcon() )->IsUsePossible() )
						SelectItem( m_apMenu[PMW_USE] );

					SelectItem( m_apMenu[PMW_PICKUP] );
					SelectItem( m_apMenu[PMW_DIVIDE] );
					SelectItem( m_apMenu[PMW_DELETE] );
				}				
			}			
		}
		else
		{
			// If the items in the bag can not be placed in the slot in duplicate
			if( GetDialogManager()->IsOpenDialog(DIALOG_NPCSHOP) )
			{
				// NPC 상점을 열었다면
				if( pSobItemAttr->IsNeedToIdentify() )
				{
					// 미확인 아이템					
					SelectItem( m_apMenu[PMW_PICKUP] );
				}
				else
				{
					if( pData->nSrcPlace == PLACE_BAG )
					{
						SelectItem( m_apMenu[PMW_SELL] );
					}

					SelectItem( m_apMenu[PMW_EQUIP] );
					SelectItem( m_apMenu[PMW_PICKUP] );
				}				
			}
			else
			{
				if( pSobItemAttr->IsNeedToIdentify() )
				{
					// 미확인 아이템					
					SelectItem( m_apMenu[PMW_PICKUP] );
				}
				else
				{
					// 일반적인 경우
					SelectItem( m_apMenu[PMW_EQUIP] );
					SelectItem( m_apMenu[PMW_PICKUP] );
					SelectItem( m_apMenu[PMW_DELETE] );
				}				
			}						
		}
	}
	else if( pData->nSrcPlace == PLACE_SKILL )
	{
		SelectItem( m_apMenu[PMW_USE] );
		SelectItem( m_apMenu[PMW_PICKUP] );
	}
	else if( pData->nSrcPlace == PLACE_QUICKSLOT )
	{
		SelectItem( m_apMenu[PMW_USE] );
		SelectItem( m_apMenu[PMW_PULLOUT] );
	}
	else if( pData->nSrcPlace == PLACE_EQUIP )
	{
		// 장비창인 아이템인 경우
		CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hSerial ) );
		
		if( pSobItem->IsScouterItem() )
		{
			// 장비창의 스카우터 슬롯인 경우
			SelectItem( m_apMenu[PMW_VIEW] );
		}

		SelectItem( m_apMenu[PMW_PICKUP] );
		SelectItem( m_apMenu[PMW_DELETE] );
	}
	else if( pData->nSrcPlace == PLACE_SCOUTER_SLOT )
	{
		// 스카우터 슬롯인 경우
		SelectItem( m_apMenu[PMW_VIEW] );
		SelectItem( m_apMenu[PMW_PICKUP] );
		SelectItem( m_apMenu[PMW_DELETE] );
	}
	else if( pData->nSrcPlace == PLACE_BAGSLOT )
	{
		if( pData->nSrcSlotIdx == 0 )
		{
			SelectItem( m_apMenu[PMW_OPEN] );
		}
		else
		{
			SelectItem( m_apMenu[PMW_OPEN] );
			SelectItem( m_apMenu[PMW_PICKUP] );
			//SelectItem( m_apMenu[PMW_DELETE], 2 );
		}
	}	
	else if( pData->nSrcPlace == PLACE_WAREHOUSE )
	{
		SelectItem( m_apMenu[PMW_PICKUP] );
		SelectItem( m_apMenu[PMW_DIVIDE] );
		SelectItem( m_apMenu[PMW_DELETE] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_TARGETUI )
	{		
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlParty* pParty = pAvatar->GetParty();
		CNtlGuild* pGuild = pAvatar->GetGuild();
		CNtlSob* pTarget = GetNtlSobManager()->GetSobObject( m_hSerial );

		if( pData->nSrcSlotIdx == 0 )
		{
			if( pTarget->GetClassID() == SLCLASS_PLAYER )
			{
				SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
				CWorldTable *pWorldTable = API_GetTableContainer()->GetWorldTable();	
				sWORLD_TBLDAT* pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData(pAvatarInfo->sWorldInfo.tblidx) );
				NTL_ASSERT(pWorldTblData, "CIconPopupGui::ShowEvent, invalid world table index : " << pAvatarInfo->sWorldInfo.tblidx );
				RwBool bIamPartyLearder = Logic_I_am_PartyLeader();
				RwBool bCanPartyInvite = Logic_CanPartyInvite();

				// 파티
				if( bIamPartyLearder )
				{
					if( pParty->IsMember(m_hSerial) )
					{
						SelectItem( m_apMenu[PMW_PARTY_LEADER_CHANGE] );

						if( bCanPartyInvite )
							SelectItem( m_apMenu[PMW_PARTY_KICK_OUT] );
					}
					else
					{
						if( bCanPartyInvite )
							SelectItem( m_apMenu[PMW_PARTY_INVITE] );
					}
				}
				else if( pParty->IsHaveGroup() == FALSE )
				{
					if( bCanPartyInvite )
						SelectItem( m_apMenu[PMW_PARTY_INVITE] );
				}


				// 길드장이거나 부길드장일 경우의 메뉴
				if( pGuild->IsGuildMaster( pAvatar->GetCharID() ) ||
					pGuild->IsSecondGuildMaster( pAvatar->GetCharID() ) )
				{
					SelectItem( m_apMenu[PMW_GUILD_INVITE] );
				}

				if (Logic_CanRequestTrade())
					SelectItem( m_apMenu[PMW_USER_TRADE] );

				SelectItem( m_apMenu[PMW_FRIEND_ADD] );

				// 일반 월드에서
				if (pWorldTblData->byWorldRuleType == GAMERULE_NORMAL)
				{
					if (Logic_CanRequestFreeBattle())
						SelectItem(m_apMenu[PMW_PVP_REQUEST_FIGHT]);
				}

				SelectItem( m_apMenu[PMW_FOLLOW] );

				SelectItem(m_apMenu[PMW_SEND_WHISPER]);
				SelectItem(m_apMenu[PMW_USER_BLOCK]);

				if (Logic_ExistScouterEffect(ACTIVE_SCOUTER_CHECK_EQUIPMENT))
					SelectItem(m_apMenu[PMW_SCOUTER_CHECK_MENU_EQUIPMENT]);
			}
		}
		else if( pData->nSrcSlotIdx == 1 )
		{
			//If you are a party leader, show shared targets.
			if( pAvatar->GetSerialID() == pParty->GetLeaderHandle() )
			{
				sSHARETARGET_INFO* pShareTargetInfos = pAvatar->GetParty()->GetShareTargetInfos();

				for( int i = 0 ; i < NTL_MAX_SHARETARGET_COUNT; ++i )
				{
					if(pShareTargetInfos[i].hTarget != INVALID_SERIAL_ID)
						SelectItem( m_apMenu[PMW_UNSET_SHARETARGET_1+i] );					
					else
						SelectItem( m_apMenu[PMW_SET_SHARETARGET_1+i] );
				}	
			}			
		}		
	}		
	else if( pData->nSrcPlace == PLACE_SUB_PARTYMEMBERUI )
	{
		CNtlSobAvatar* pAvater = GetNtlSLGlobal()->GetSobAvatar();
		SERIAL_HANDLE hLeaderHandle = pAvater->GetParty()->GetLeaderHandle();		

		// 파티장일 경우의 메뉴
		if( pAvater->GetSerialID() == hLeaderHandle )
		{
			SelectItem( m_apMenu[PMW_PARTY_LEADER_CHANGE] );

			if( Logic_CanPartyInvite() )
				SelectItem( m_apMenu[PMW_PARTY_KICK_OUT] );				
		}		
	}
	else if( pData->nSrcPlace == PLACE_QUESTBAG )
	{
		SelectItem( m_apMenu[PMW_DELETE] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_GUILD_DIALOG )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlGuild* pGuild = pAvatar->GetGuild();

		if( m_hSerial != pAvatar->GetCharID() )
		{
			if( Logic_I_am_GuildLeader() )
			{
				SelectItem( m_apMenu[PMW_GUILD_CHANGE_MASTER] );

				if( pGuild->IsSecondGuildMaster(m_hSerial) )
					SelectItem( m_apMenu[PMW_GUILD_DISAPPOINT_SECOND_MASTER] );
				else
					SelectItem( m_apMenu[PMW_GUILD_APPOINT_SECOND_MASTER] );

				SelectItem( m_apMenu[PMW_GUILD_KICK_OUT] );
			}		

			SelectItem( m_apMenu[PMW_GUILD_POST] );
		}		
	}
	else if( pData->nSrcPlace == PLACE_SUB_PARTY_MENU_MEMBER )
	{
		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
		RwBool bHaveParty = pParty->IsHaveGroup();
		RwBool bPartyLeader = Logic_I_am_PartyLeader();

		if( !bHaveParty )
			SelectItem( m_apMenu[PMW_PARTY_CREATE] );

		if( bPartyLeader )
			SelectItem( m_apMenu[PMW_PARTY_INVITE] );

		if( bHaveParty )
			SelectItem( m_apMenu[PMW_PARTY_LEAVE] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY )
	{
		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
		RwUInt8 byZennyLootingMethod = pParty->GetZennyLootingMethod();

		// 현재 선택된 제니 분배 방식은 다른 색으로 보여준다
		switch( byZennyLootingMethod )
		{
		case NTL_PARTY_ZENNY_LOOTING_GREEDILY:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_EQUAL], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				break;
			}
		case NTL_PARTY_ZENNY_LOOTING_SHARED:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_EQUAL], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				break;
			}
		}

		SelectItem( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON] );
		SelectItem( m_apMenu[PMW_PARTY_DIVIDE_EQUAL] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_PARTY_MENU_DIVIDE_ITEM )
	{
		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
		RwUInt8 byItemLootingMethod = pParty->GetItemLootingMethod();

		// 현재 선택된 아이템 분배 방식은 다른 색으로 보여준다
		switch( byItemLootingMethod )
		{
		case NTL_PARTY_ITEM_LOOTING_GREEDILY:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_ORDER], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_GRADE_ITEM], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				//SetColor( m_apMenu[PMW_PARTY_USE_INVEN], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				break;
			}
		case NTL_PARTY_ITEM_LOOTING_IN_ORDER:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_ORDER], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_GRADE_ITEM], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				//SetColor( m_apMenu[PMW_PARTY_USE_INVEN], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				break;
			}
		case NTL_PARTY_ITEM_LOOTING_IN_ORDER_SEPERATELY:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_ORDER], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_GRADE_ITEM], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				//SetColor( m_apMenu[PMW_PARTY_USE_INVEN], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				break;
			}
		case NTL_PARTY_ITEM_LOOTING_IN_LEADER:
		case NTL_PARTY_ITEM_LOOTING_BY_ZENNY:
		case NTL_PARTY_ITEM_LOOTING_DICE:
			{
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_DIVIDE_ORDER], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				SetColor( m_apMenu[PMW_PARTY_GRADE_ITEM], NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR );
				//SetColor( m_apMenu[PMW_PARTY_USE_INVEN], NTL_PARTY_BUTTON_UP_COLOR, NTL_PARTY_BUTTON_DOWN_COLOR, NTL_PARTY_BUTTON_FOCUS_COLOR, NTL_PARTY_BUTTON_DISABLE_COLOR );
				break;
			}
		}

		SelectItem( m_apMenu[PMW_PARTY_DIVIDE_PICKUP_PERSON] );
		SelectItem( m_apMenu[PMW_PARTY_DIVIDE_ORDER] );
		SelectItem( m_apMenu[PMW_PARTY_GRADE_ITEM] );
		//SelectItem( m_apMenu[PMW_PARTY_USE_INVEN] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_PARTY_MENU_SWITCH )
	{
		SelectItem( m_apMenu[PMW_PARTY_MENU_SWITCH_MEMBER] );
		SelectItem( m_apMenu[PMW_PARTY_MENU_SWITCH_MEMBER_BUFF] );
	}
	else if( pData->nSrcPlace == PLACE_SUB_PARTY_MENU_DUNGEON )
	{
		SelectItem( m_apMenu[PMW_PARTY_DUNGEON_INIT] );
		SelectItem( m_apMenu[PMW_PARTY_DUNGEON_NORMAL] );
		SelectItem( m_apMenu[PMW_PARTY_DUNGEON_HARD] );
	}
	else if (pData->nSrcPlace == PLACE_SUB_BAG)
	{
		SelectItem(m_apMenu[PMW_HELP]);
		SelectItem(m_apMenu[PMW_ITEM_SORT]);
		SelectItem(m_apMenu[PMW_ITEM_DISASSEMBLE]);
	 }
	else
	{
		// Oh my goodness!
		//NTL_ASSERTFAIL( "CIconPopupGui::CalcFormatEvent : Wrong Place" );
		return;
	}

	if( m_nSeletecItemCount == 0 )
	{
		Show(false);
		return;
	}

	// Cancel is in all windows
	SelectItem( m_apMenu[PMW_CANCEL] );

	RaiseTop();
	CalcRect( pData->nXPos, pData->nYPos );
}

VOID CIconPopupGui::SelectItem( CIconPopupItem* pItem )
{
	pItem->SetPosition( ICONPOPUP_MENU_START_X + ICONPOPUP_MARGIN_X,
					    ICONPOPUP_MENU_START_Y + ICONPOPUP_MARGIN_Y + ( ICONPOPUP_ITEM_HEIGHT + ICONPOPUP_SPACE ) * m_nSeletecItemCount );
	pItem->Show( TRUE );

	++m_nSeletecItemCount;
}

VOID CIconPopupGui::SetColor( CIconPopupItem* pItem, DWORD dwColorUp, DWORD dwColorDown, DWORD dwColorFoc, DWORD dwColorDis )
{
	pItem->SetColor( dwColorUp, dwColorDown, dwColorFoc, dwColorDis );
}

void CIconPopupGui::ResetItem(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_PMW ; ++i )
	{
		m_apMenu[i]->Show( FALSE );
	}

	m_nSeletecItemCount = 0;
}

const WCHAR * CIconPopupGui::GetPopupMenuName(RwInt32 num)
{
	switch (num)
	{
		case PMW_USE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_USE");
		case PMW_OPEN: return GetDisplayStringManager()->GetString("DST_POPUPMENU_OPEN");
		case PMW_EQUIP: return GetDisplayStringManager()->GetString("DST_POPUPMENU_EQUIP");
		case PMW_CANCEL_EQUIP: return GetDisplayStringManager()->GetString("DST_POPUPMENU_CANCEL_EQUIP");
		case PMW_VIEW: return GetDisplayStringManager()->GetString("DST_POPUPMENU_VIEW");
		case PMW_PICKUP: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PICKUP");
		case PMW_STOP_USE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_STOP_USE");
		case PMW_PULLOUT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PULLOUT");
		case PMW_SELL: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SELL");
		case PMW_DIVIDE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_DIVIDE");
		case PMW_CLEAR: return GetDisplayStringManager()->GetString("DST_POPUPMENU_CLEAR");
		case PMW_DELETE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_DELETE");
		case PMW_PARTY_CREATE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_CREATE");
		case PMW_PARTY_INVITE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_INVITE");
		case PMW_PARTY_LEADER_CHANGE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_LEADER_CHANGE");
		case PMW_PARTY_KICK_OUT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_KICK_OUT");
		case PMW_PARTY_LEAVE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_LEAVE");
		case PMW_PVP_REQUEST_FIGHT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PVP_REQUEST_FIGHT");
		case PMW_USER_TRADE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_USER_TRADE");
		case PMW_GUILD_INVITE: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_INVITE");
		case PMW_GUILD_CHANGE_MASTER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_CHANGE_MASTER");
		case PMW_GUILD_APPOINT_SECOND_MASTER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_APPOINT_SECOND_MASTER");
		case PMW_GUILD_DISAPPOINT_SECOND_MASTER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_DISAPPOINT_SECOND_MASTER");
		case PMW_GUILD_KICK_OUT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_KICK_OUT");
		case PMW_GUILD_POST: return GetDisplayStringManager()->GetString("DST_POPUPMENU_GUILD_POST");
		case PMW_FRIEND_ADD: return GetDisplayStringManager()->GetString("DST_POPUPMENU_FRIEND_ADD");
		case PMW_FOLLOW: return GetDisplayStringManager()->GetString("DST_POPUPMENU_FOLLOW");
		case PMW_SEND_WHISPER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SEND_WHISPER");
		case PMW_CANCEL: return GetDisplayStringManager()->GetString("DST_POPUPMENU_CANCEL");
		case PMW_SET_SHARETARGET_1: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SET_SHARETARGET_1");
		case PMW_SET_SHARETARGET_2: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SET_SHARETARGET_2");
		case PMW_SET_SHARETARGET_3: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SET_SHARETARGET_3");
		case PMW_SET_SHARETARGET_4: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SET_SHARETARGET_4");
		case PMW_SET_SHARETARGET_5: return GetDisplayStringManager()->GetString("DST_POPUPMENU_SET_SHARETARGET_5");
		case PMW_UNSET_SHARETARGET_1: return GetDisplayStringManager()->GetString("DST_POPUPMENU_UNSET_SHARETARGET_1");
		case PMW_UNSET_SHARETARGET_2: return GetDisplayStringManager()->GetString("DST_POPUPMENU_UNSET_SHARETARGET_2");
		case PMW_UNSET_SHARETARGET_3: return GetDisplayStringManager()->GetString("DST_POPUPMENU_UNSET_SHARETARGET_3");
		case PMW_UNSET_SHARETARGET_4: return GetDisplayStringManager()->GetString("DST_POPUPMENU_UNSET_SHARETARGET_4");
		case PMW_UNSET_SHARETARGET_5: return GetDisplayStringManager()->GetString("DST_POPUPMENU_UNSET_SHARETARGET_5");
		case PMW_PARTY_MENU_SWITCH_MEMBER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_MENU_SWITCH_MEMBER");
		case PMW_PARTY_MENU_SWITCH_MEMBER_BUFF: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_MENU_SWITCH_MEMBER_BUFF");
		case PMW_PARTY_DUNGEON_INIT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DUNGEON_INIT");
		case PMW_PARTY_DUNGEON_NORMAL: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DUNGEON_NORMAL");
		case PMW_PARTY_DUNGEON_HARD: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DUNGEON_HARD");
		case PMW_PARTY_DUNGEON_LEGENDARY: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DUNGEON_LEGENDARY");
		case PMW_PARTY_DIVIDE_PICKUP_PERSON: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DIVIDE_PICKUP_PERSON");
		case PMW_PARTY_DIVIDE_EQUAL: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DIVIDE_EQUAL");
		case PMW_PARTY_DIVIDE_ORDER: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_DIVIDE_ORDER");
		case PMW_PARTY_GRADE_ITEM: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_GRADE_ITEM");
		case PMW_PARTY_LOOTING_BY_DICE: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_BY_DICE");
		case PMW_PARTY_LOOTING_BY_DICE_BY_EQUIPPED: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_BY_DICE_BY_EQUIPPED");
		case PMW_PARTY_GRADE_NORMAL: return GetDisplayStringManager()->GetString("DST_ITEM_GRADE_NORMAL");
		case PMW_PARTY_GRADE_SUPERIOR: return GetDisplayStringManager()->GetString("DST_ITEM_GRADE_SUPERIOR");
		case PMW_PARTY_GRADE_EXCELLENT: return GetDisplayStringManager()->GetString("DST_ITEM_GRADE_EXCELLENT");
		case PMW_PARTY_GRADE_RARE: return GetDisplayStringManager()->GetString("DST_ITEM_GRADE_RARE");
		case PMW_PARTY_GRADE_LEGENDARY: return GetDisplayStringManager()->GetString("DST_ITEM_GRADE_LEGENDARY");
		case PMW_PARTY_USE_INVEN: return GetDisplayStringManager()->GetString("DST_POPUPMENU_PARTY_USE_INVEN");
		case PMW_USER_BLOCK: return GetDisplayStringManager()->GetString("DST_POPUPMENU_USER_BLOCK");
		case PMW_ITEM_SORT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_ITEM_SORT");
		case PMW_HELP: return GetDisplayStringManager()->GetString("DST_POPUPMENU_HELP");
		case PMW_SCOUTER_CHECK_MENU_EQUIPMENT: return GetDisplayStringManager()->GetString("DST_SCOUTER_CHECK_MENU_EQUIPMENT");
		case PMW_WAGU_ITEM_BUNCH: return GetDisplayStringManager()->GetString("DST_WAGU_ITEM_BUNCH");
		case PMW_WAGU_ITEM_BUNCH2: return GetDisplayStringManager()->GetString("DST_WAGU_ITEM_BUNCH");
		case PMW_ITEM_DISASSEMBLE: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SMITH_DISASSEMBLE");

		default: break;
	}


	DBO_ASSERT(true, "Not defined popup menu string : " << num);

	return L"";
}

const WCHAR * CIconPopupGui::GetPopupMenuTooltip(RwInt32 num)
{
	switch (num)
	{
		case PMW_ITEM_SORT: return GetDisplayStringManager()->GetString("DST_POPUPMENU_ITEM_SORT_TOOLTIP");

		default: break;
	}


	return L"";
}

VOID CIconPopupGui::CalcRect( RwInt32 nXPos, RwInt32 nYPos )
{
	RwInt32 nScreenWidth = m_pThis->GetGuiManager()->GetWidth();
	RwInt32 nScreenHeight = m_pThis->GetGuiManager()->GetHeight();

	RwInt32 nWidth = ICONPOPUP_ITEM_WIDTH + 2 * ICONPOPUP_MARGIN_X;
	RwInt32 nHeight = ICONPOPUP_ITEM_HEIGHT * m_nSeletecItemCount + ICONPOPUP_SPACE * ( m_nSeletecItemCount - 1 ) + 2 * ICONPOPUP_MARGIN_Y;

	// 기본위치는 Right Bottom
	RwInt32 nX = nXPos + NTL_ITEM_ICON_SIZE + ICONPOPUP_POINT_MARGIN_X;
	RwInt32 nY = nYPos + NTL_ITEM_ICON_SIZE + ICONPOPUP_POINT_MARGIN_Y;

	if( nX + nWidth > nScreenWidth )
	{
		nX = nXPos - nWidth - ICONPOPUP_POINT_MARGIN_X;
	}
	if( nY + nHeight > nScreenHeight )
	{
		nY = nYPos - nHeight - ICONPOPUP_POINT_MARGIN_Y;
	}

	SetPosition( CRectangle( nX, nY, nX + nWidth, nY + nHeight ) );	
}

VOID CIconPopupGui::OnCaptureMouseDown( const CKey& key )
{
	CaptureMouseDown( key );
}

VOID CIconPopupGui::OnCaptureKeyUp( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key )
{
	if( IsShow() )
	{
		if( key.m_dwVKey == VK_ESCAPE )
		{
			Show( false );

			Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CANCEL);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

CCalcPopupGui::CCalcPopupGui(VOID)
{
	Init();
}

CCalcPopupGui::CCalcPopupGui( RwChar *szName )
: CPopupGui( szName )
{
	Init();
}

CCalcPopupGui::~CCalcPopupGui(VOID)
{

}

VOID CCalcPopupGui::Init(VOID)
{
	memset( m_apButton, 0, sizeof( m_apButton ) );
	m_pStaticBox = NULL;	
	m_uiValue = 0;
	m_uiMaxValue = 0;
	m_bAllowNull = false;
}

RwBool CCalcPopupGui::Create(VOID)
{
	NTL_FUNCTION( "CPopupMenuGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\CalcPopup.srf", "gui\\CalcPopup.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pThis->SetPriority( dDIALOGPRIORITY_CALC_POPUP );	// 계산기 우선 순위 추가
	
	m_apButton[0] = (gui::CButton*)GetComponent( "btn0" );
	m_apButton[1] = (gui::CButton*)GetComponent( "btn1" );
	m_apButton[2] = (gui::CButton*)GetComponent( "btn2" );
	m_apButton[3] = (gui::CButton*)GetComponent( "btn3" );
	m_apButton[4] = (gui::CButton*)GetComponent( "btn4" );
	m_apButton[5] = (gui::CButton*)GetComponent( "btn5" );
	m_apButton[6] = (gui::CButton*)GetComponent( "btn6" );
	m_apButton[7] = (gui::CButton*)GetComponent( "btn7" );
	m_apButton[8] = (gui::CButton*)GetComponent( "btn8" );
	m_apButton[9] = (gui::CButton*)GetComponent( "btn9" );
	m_apButton[BUTTON_CLEAR] = (gui::CButton*)GetComponent( "btnClear" );
	m_apButton[BUTTON_OK] = (gui::CButton*)GetComponent( "btnOk" );
	m_pStaticBox = (gui::CStaticBox*)GetComponent( "stbPanel" );

	for( RwInt32 i = 0 ; i < 10 ; ++i )
	{
		m_slotButton[i] = m_apButton[i]->SigClicked().Connect( this, &CCalcPopupGui::OnButtonClick );
	}

	m_slotButton[BUTTON_CLEAR] = m_apButton[BUTTON_CLEAR]->SigClicked().Connect( this, &CCalcPopupGui::OnButtonClearClick );
	m_slotButton[BUTTON_OK] = m_apButton[BUTTON_OK]->SigClicked().Connect( this, &CCalcPopupGui::OnButtonOkClick );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CCalcPopupGui::OnCaptureMouseDown );
	m_slotCaptureKeyDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureKeyDown().Connect( this, &CCalcPopupGui::OnCaptureKeyDown );
	m_slotCaptureKeyUp = GetNtlGuiManager()->GetGuiManager()->SigCaptureKeyUp().Connect( this, &CCalcPopupGui::OnCaptureKeyUp );
	
	Show( false );

	NTL_RETURN( TRUE );
}

VOID CCalcPopupGui::Destroy(VOID)
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CCalcPopupGui::Show( bool bShow )
{
	CNtlPLGui::Show( bShow );

	if( bShow )
	{
		GetDboGlobal()->GetGameData()->bChatMode = TRUE;
		GetInputActionMap()->Reset();
		GetInputActionMap()->SetActive(FALSE);
		GetIconMoveManager()->IconMoveEnd();

		for( RwInt32 i = 0 ; i < NUM_BUTTON ; ++i )
			m_apButton[i]->SetDown( false );
	}
	else
	{
		GetDboGlobal()->GetGameData()->bChatMode = FALSE;
		GetInputActionMap()->SetActive(TRUE);
	}
}

VOID CCalcPopupGui::ShowEvent( RWS::CMsg& msg )
{
	SDboEventCalcPopupShow* pData = reinterpret_cast<SDboEventCalcPopupShow*>( msg.pData );

	m_uiValue = 0;
	m_uiMaxValue = pData->uiMaxValue;
	m_hSerial = pData->uiSerial;
	m_ePlace = (EPlace)pData->nSrcPlace;
	m_bAllowNull = pData->bAllowNull;

	RaiseTop();

	RwInt32 nScreenWidth = m_pThis->GetGuiManager()->GetWidth();
	RwInt32 nScreenHeight = m_pThis->GetGuiManager()->GetHeight();
	CRectangle rtBox = m_pThis->GetClientRect();

	// 기본위치는 Right Bottom
	RwInt32 nX = pData->nXPos + ICONPOPUP_MARGIN_X;
	RwInt32 nY = pData->nYPos + ICONPOPUP_MARGIN_Y;
	RwInt32 nWidth = rtBox.GetWidth(); 
	RwInt32 nHeight =rtBox.GetHeight();

	if( nX + nWidth > nScreenWidth )
	{
		nX = pData->nXPos - nWidth - ICONPOPUP_MARGIN_X;
	}
	if( nY + nHeight > nScreenHeight )
	{
		nY = pData->nYPos - nHeight - ICONPOPUP_MARGIN_Y;
	}

	SetPosition( nX, nY );

	m_pStaticBox->SetText( m_uiValue );
}

VOID CCalcPopupGui::OnButtonClick( gui::CComponent* pComponent )
{
	// RwUInt32 범위를 넘기는 것을 방지하기 위해
	RwUInt64 ui64Temp = m_uiValue;

	ui64Temp *= 10;

	if( pComponent == m_apButton[1] )		ui64Temp += 1;
	else if( pComponent == m_apButton[2] )	ui64Temp += 2; 
	else if( pComponent == m_apButton[3] )	ui64Temp += 3; 
	else if( pComponent == m_apButton[4] )	ui64Temp += 4; 
	else if( pComponent == m_apButton[5] )	ui64Temp += 5; 
	else if( pComponent == m_apButton[6] )	ui64Temp += 6; 
	else if( pComponent == m_apButton[7] )	ui64Temp += 7; 
	else if( pComponent == m_apButton[8] )	ui64Temp += 8; 
	else if( pComponent == m_apButton[9] )	ui64Temp += 9; 


	if( ui64Temp >= 0xffffffff || ui64Temp > (RwUInt64)m_uiMaxValue )
		m_uiValue = m_uiMaxValue;
	else
		m_uiValue = (RwUInt32)ui64Temp;


	m_pStaticBox->SetText(Logic_FormatZeni(m_uiValue));
}

VOID CCalcPopupGui::OnCaptureKeyDown( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key )
{
	if( IsShow() )
	{
		if( key.m_dwVKey >= '0' && key.m_dwVKey <= '9' )			
		{
			m_apButton[key.m_dwVKey-'0']->SetDown();
		}
		else if( key.m_dwVKey >= VK_NUMPAD0 && key.m_dwVKey <= VK_NUMPAD9 )
		{
			m_apButton[key.m_dwVKey-VK_NUMPAD0]->SetDown();
		}
		else if( key.m_dwVKey == VK_RETURN )
		{
			m_apButton[BUTTON_OK]->SetDown();
		}
		else if( key.m_dwVKey == VK_ESCAPE || key.m_dwVKey == VK_DELETE )
		{
			m_apButton[BUTTON_CLEAR]->SetDown();
		}
	}
}

VOID CCalcPopupGui::OnCaptureKeyUp( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key )
{
	if( IsShow() )
	{
		if( key.m_dwVKey >= '0' && key.m_dwVKey <= '9' )			
		{
			Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CLICK);

			m_apButton[key.m_dwVKey-'0']->SetDown( false );

			OnButtonClick( m_apButton[key.m_dwVKey-'0'] );
		}		
		else if( key.m_dwVKey >= VK_NUMPAD0 && key.m_dwVKey <= VK_NUMPAD9 )
		{
			Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CLICK);

			m_apButton[key.m_dwVKey-VK_NUMPAD0]->SetDown( false );

			OnButtonClick( m_apButton[key.m_dwVKey-VK_NUMPAD0] );
		}
		else if( key.m_dwVKey == VK_RETURN )
		{
			OnButtonOkClick( NULL );
			m_apButton[BUTTON_OK]->SetDown( false );
		}
		else if( key.m_dwVKey == VK_DELETE )
		{
			OnButtonClearClick( NULL );
			m_apButton[BUTTON_CLEAR]->SetDown( false );
		}
		else if( key.m_dwVKey == VK_ESCAPE )
		{
			Logic_PlayGUISound(GSD_SYSTEM_BUTTON_CANCEL);

			Show( false );
		}
	}	
}

VOID CCalcPopupGui::OnButtonClearClick( gui::CComponent* pComponent )
{
	m_uiValue = 0;

	m_pStaticBox->SetText( m_uiValue );
}

VOID CCalcPopupGui::OnButtonOkClick( gui::CComponent* pComponent )
{
	Show( false );

	if( m_uiValue == 0 && m_bAllowNull == false)
		return;

	// Message Event Send
	CDboEventGenerator::CalcPopupResult( m_hSerial, m_ePlace, m_uiValue );
}

VOID CCalcPopupGui::OnCaptureMouseDown( const CKey& key )
{
	CaptureMouseDown( key );
}
