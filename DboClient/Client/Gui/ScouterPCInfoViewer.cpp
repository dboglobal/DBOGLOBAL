#include "precomp_dboclient.h"
#include "ScouterPCInfoViewer.h"

// core
#include "NtlDebug.h"

#include "NtlSLEvent.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "SideDialogManager.h"
#include "DisplayStringManager.h"

namespace
{
	#define dSHOWTIME		60.f
};


CScouterPCInfoViewerGui::CScouterPCInfoViewerGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fElapsed(0.f)
,m_byInfoWindowIndex(INVALID_BYTE)
{
}

CScouterPCInfoViewerGui::~CScouterPCInfoViewerGui()
{

}

RwBool CScouterPCInfoViewerGui::Create()
{
	NTL_FUNCTION( "CScouterPCInfoViewerGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ScouterPCInfoViewer.srf", "gui\\ScouterPCInfoViewer.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pCloseButton = (gui::CButton*)GetComponent("btnClose");
	m_slotCloseButton = m_pCloseButton->SigClicked().Connect(this, &CScouterPCInfoViewerGui::OnClicked_CloseButton);
	
	m_srfWeaponBack		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterPCInfoViewer.srf", "srfWeaponBack") );
	m_srfDefneceBack	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterPCInfoViewer.srf", "srfDefenceBack") );
	m_srfAccessaryBack	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterPCInfoViewer.srf", "srfAccessaryBack") );
	m_srfSpecialBack	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterPCInfoViewer.srf", "srfSpecialBack") );
	m_srfCostumeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterPCInfoViewer.srf", "srfCostumeBack"));

	m_srfWeaponBack		.SetPositionfromParent(42, 56);
	m_srfDefneceBack	.SetPositionfromParent(20, 119);
	m_srfAccessaryBack	.SetPositionfromParent(20, 182);
	m_srfSpecialBack	.SetPositionfromParent(42, 283);
	m_srfCostumeBack.SetPositionfromParent(20, 346);

	m_srfLine	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSideContourLine") );
	m_srfLine2	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSideContourLine") );
	m_srfLine3	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSideContourLine") );
	m_srfLine4.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSideContourLine"));
	m_srfLine5.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSideContourLine"));

	m_srfLine	.SetPositionfromParent(0, 97);
	m_srfLine2	.SetPositionfromParent(0, 159);
	m_srfLine3	.SetPositionfromParent(0, 262);
	m_srfLine4.SetPositionfromParent(0, 325);
	m_srfLine5.SetPositionfromParent(0, 426);

	m_pStaticTitle		= (gui::CStaticBox*)GetComponent("stbDialogName");
	m_pStaticWeapon		= (gui::CStaticBox*)GetComponent("stbWeapon");
	m_pStaticDefence	= (gui::CStaticBox*)GetComponent("stbDefence");
	m_pStaticAccesary	= (gui::CStaticBox*)GetComponent("stbAccesary");
	m_pStaticSpecial	= (gui::CStaticBox*)GetComponent("stbSpecial");
	m_pstbCostume = (gui::CStaticBox*)GetComponent("stbCostume");

	m_pStaticTitle		->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pStaticTitle		->SetText(GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_TARGETITEMINFO"));
	m_pStaticWeapon		->SetText(GetDisplayStringManager()->GetString("DST_SCOUTER_PC_VIEWER_WEAPON"));
	m_pStaticDefence	->SetText(GetDisplayStringManager()->GetString("DST_SCOUTER_PC_VIEWER_DEFENCE"));
	m_pStaticAccesary	->SetText(GetDisplayStringManager()->GetString("DST_SCOUTER_PC_VIEWER_ACCESARY"));
	m_pStaticSpecial	->SetText(GetDisplayStringManager()->GetString("DST_SCOUTER_PC_VIEWER_SPECIAL"));
	m_pstbCostume->SetText(GetDisplayStringManager()->GetString("DST_SCOUTER_PC_VIEWER_COSTUME"));

	// 메인 무기
	m_Item[EQUIP_SLOT_TYPE_HAND].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_HAND].SlotItem.SetPosition_fromParent(44, 58);

	// 보조 무기
	m_Item[EQUIP_SLOT_TYPE_SUB_WEAPON].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_SUB_WEAPON].SlotItem.SetPosition_fromParent(88, 58);

	// 자켓
	m_Item[EQUIP_SLOT_TYPE_JACKET].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_JACKET].SlotItem.SetPosition_fromParent(22, 121);

	// 팬츠
	m_Item[EQUIP_SLOT_TYPE_PANTS].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_PANTS].SlotItem.SetPosition_fromParent(65, 121);

	// 부츠
	m_Item[EQUIP_SLOT_TYPE_BOOTS].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_BOOTS].SlotItem.SetPosition_fromParent(110, 121);

	// 목걸이
	m_Item[EQUIP_SLOT_TYPE_NECKLACE].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_NECKLACE].SlotItem.SetPosition_fromParent(22, 184);

	// 귀거리
	m_Item[EQUIP_SLOT_TYPE_EARRING_1].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_EARRING_1].SlotItem.SetPosition_fromParent(66, 184);

	// 귀거리 2
	m_Item[EQUIP_SLOT_TYPE_EARRING_2].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_EARRING_2].SlotItem.SetPosition_fromParent(110, 184);

	// 반지
	m_Item[EQUIP_SLOT_TYPE_RING_1].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_RING_1].SlotItem.SetPosition_fromParent(44, 223);

	// 반지 2
	m_Item[EQUIP_SLOT_TYPE_RING_2].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_RING_2].SlotItem.SetPosition_fromParent(88, 223);

	// scouter
	m_Item[EQUIP_SLOT_TYPE_SCOUTER].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_SCOUTER].SlotItem.SetPosition_fromParent(44, 285);

	// quest
	m_Item[EQUIP_SLOT_TYPE_QUEST].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_QUEST].SlotItem.SetPosition_fromParent(88, 285);


	m_Item[EQUIP_SLOT_TYPE_COSTUME_SET].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_COSTUME_SET].SlotItem.SetPosition_fromParent(22, 347);

	m_Item[EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_COSTUME_HAIR_STYLE].SlotItem.SetPosition_fromParent(65, 347);

	m_Item[EQUIP_SLOT_TYPE_COSTUME_MASK].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_COSTUME_MASK].SlotItem.SetPosition_fromParent(110, 347);

	m_Item[EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_COSTUME_HAIR_ACCESSORY].SlotItem.SetPosition_fromParent(44, 386);

	m_Item[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].SlotItem.Create(m_pThis, SDIALOG_SCOUTER_PC_VIEWER, REGULAR_SLOT_ITEM_TABLE);
	m_Item[EQUIP_SLOT_TYPE_COSTUME_BACK_ACCESSORY].SlotItem.SetPosition_fromParent(88, 386);

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CScouterPCInfoViewerGui::OnPaint );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CScouterPCInfoViewerGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CScouterPCInfoViewerGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CScouterPCInfoViewerGui::OnMouseLeave );

	LinkMsg(g_EventScouter);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterPCInfoViewerGui::Destroy()
{
	NTL_FUNCTION("CScouterPCInfoViewerGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	for(RwUInt8 i = 0 ; i < EQUIP_SLOT_TYPE_COUNT ; ++i)
		m_Item[i].SlotItem.Destroy();

	UnLinkMsg(g_EventScouter);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterPCInfoViewerGui::Update( RwReal fElapsed )
{
	if( IsShow() == FALSE )
		return;

	m_fElapsed += fElapsed;

	if( m_fElapsed >= dSHOWTIME )
	{
		m_fElapsed = 0.f;

		GetSideDialogManager()->CloseDialog(SDIALOG_SCOUTER_PC_VIEWER);
	}
}

VOID CScouterPCInfoViewerGui::ShowPCInfo(SERIAL_HANDLE hTarger, RwUInt8 byCount, sITEM_PROFILE* pItemProfile)
{
	GetSideDialogManager()->OpenDialog(SDIALOG_SCOUTER_PC_VIEWER);

	CRectangle rtScreen = m_pThis->GetScreenRect();

	for(RwUInt8 i = 0 ; i < byCount ; ++i )
	{
		if( EQUIP_SLOT_TYPE_COUNT <= pItemProfile[i].byPos )
		{
			DBO_FAIL("Invalid equip slot index : " << pItemProfile[i].byPos);
			continue;
		}

		m_Item[pItemProfile[i].byPos].SlotItem.SetParentPosition(rtScreen.left, rtScreen.top);
		m_Item[pItemProfile[i].byPos].SlotItem.SetIcon(pItemProfile[i].tblidx);
		m_Item[pItemProfile[i].byPos].ItemProfile = pItemProfile[i];
	}

	m_fElapsed = 0.f;
}

VOID CScouterPCInfoViewerGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == SDIALOG_SCOUTER_PC_VIEWER )
	{
        m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CScouterPCInfoViewerGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	GetSideDialogManager()->CloseDialog(SDIALOG_SCOUTER_PC_VIEWER);
}

VOID CScouterPCInfoViewerGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfWeaponBack		.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfDefneceBack	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfAccessaryBack	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSpecialBack	.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCostumeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfLine			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfLine2			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfLine3			.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfLine4.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfLine5.SetPositionbyParent(rtScreen.left, rtScreen.top);

	for( RwInt32 i = 0 ; i < EQUIP_SLOT_TYPE_COUNT ; ++i )
		m_Item[i].SlotItem.SetParentPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CScouterPCInfoViewerGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( GetIconMoveManager()->IsActive() )
		return;

	for(RwUInt8 i = 0 ; i < EQUIP_SLOT_TYPE_COUNT ; ++i)
	{
		if( m_Item[i].SlotItem.PtInRect(nX, nY) )
		{
			if( m_Item[i].SlotItem.GetSerial() != INVALID_SERIAL_ID )
			{
				if( m_byInfoWindowIndex != i )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM,
														rtScreen.left + m_Item[i].SlotItem.GetX_fromParent(),
														rtScreen.top + m_Item[i].SlotItem.GetY_fromParent(),
														(VOID*)&m_Item[i].ItemProfile, SDIALOG_SCOUTER_PC_VIEWER );
					m_byInfoWindowIndex = i;
				}
			}

			return;
		}
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterPCInfoViewerGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterPCInfoViewerGui::OnPaint()
{
	m_srfWeaponBack		.Render();
	m_srfDefneceBack	.Render();
	m_srfAccessaryBack	.Render();
	m_srfSpecialBack	.Render();
	m_srfCostumeBack.Render();

	m_srfLine			.Render();
	m_srfLine2			.Render();
	m_srfLine3			.Render();
	m_srfLine4.Render();
	m_srfLine5.Render();

	for( RwUInt8 i = 0 ; i < EQUIP_SLOT_TYPE_COUNT ; ++i )
		m_Item[i].SlotItem.Paint();
}

RwInt32 CScouterPCInfoViewerGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		for (RwUInt8 i = 0; i < EQUIP_SLOT_TYPE_COUNT; ++i)
			m_Item[i].SlotItem.Clear();

		Show(false);
		CheckInfoWindow();
	}

	return 1;
}

VOID CScouterPCInfoViewerGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScouterPCInfoViewerGui::HandleEvents");

	if(msg.Id == g_EventScouter)
	{
		SDboEventScouter* pPacket = reinterpret_cast<SDboEventScouter*>(msg.pData);
		switch(pPacket->iType)
		{
		case SCOUTER_EVENT_PC_INFO_VIEW:
			{
				ShowPCInfo(pPacket->uiUserData, (RwUInt8)pPacket->iUserData2, reinterpret_cast<sITEM_PROFILE*>(pPacket->iUserData3));			
				break;
			}
		case SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER:
			{
				GetSideDialogManager()->CloseDialog(SDIALOG_SCOUTER_PC_VIEWER);
				break;
			}

		default: break;
		}
	}

	NTL_RETURNVOID();
}