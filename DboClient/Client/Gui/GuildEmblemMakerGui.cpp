#include "precomp_dboclient.h"
#include "GuildEmblemMakerGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEmblemMaker.h"

// simulation
#include "NtlSLGuild.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// dbo
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "DialogManager.h"
#include "DboGlobal.h"


#define dEMBLEM_STUFF_START_X		(52)
#define dEMBLEM_STUFF_START_Y		(77)
#define dEMBLEM_STUFF_SIZE			(32)
#define dEMBLEM_STUFF_COLUMN_GAP	(9)
#define dEMBLEM_STUFF_ROW_GAP		(8)

#define dPALETTE_START_X			(98)
#define dPALETTE_START_Y			(204)
#define dPALETTE_SIZE				(16)
#define dPALETTE_GAP				(3)

#define dSLOTSTUFF_TYPE_PALETTE		NUM_EMBLEMTYPE
#define dSLOTSTUFF_TYPE_INAVLID		0xff



CGuildEmblemMakerGui::CGuildEmblemMakerGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bUpdateEmblem(TRUE)
,m_bStuffFocus(FALSE)
,m_byCurStuffRow(INVALID_BYTE)
,m_byPaletteFocus(INVALID_BYTE)
{
}

CGuildEmblemMakerGui::~CGuildEmblemMakerGui()
{
}

RwBool CGuildEmblemMakerGui::Create()
{
	NTL_FUNCTION( "CGuildEmblemMakerGui::Create" );

	if(!CNtlPLGui::Create("gui\\GuildEmblemMaker.rsr", "gui\\GuildEmblemMaker.srf", "gui\\GuildEmblemMaker.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pTitle		= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pExplain		= (gui::CStaticBox*)GetComponent("stbExplain");
	m_pExplain2		= (gui::CStaticBox*)GetComponent("stbExplain2");

	m_pstbColorExplain = (gui::CStaticBox*)GetComponent("stbColorExplain");

	m_pTitle->SetText(GetDisplayStringManager()->GetString("DST_GUILD_EMBLEM_TITLE"));
	m_pExplain->SetText(GetDisplayStringManager()->GetString("DST_GUILD_EMBLEM_EXPLAIN_1"));
	m_pExplain2->SetText(GetDisplayStringManager()->GetString("DST_GUILD_EMBLEM_EXPLAIN_2"));
	m_pstbColorExplain->SetText(GetDisplayStringManager()->GetString("DST_GUILD_SETUP_EMBLEM_COLOR"));

	m_pLeftAButton	= (gui::CButton*)GetComponent("btnLeftAType");
	m_pLeftBButton	= (gui::CButton*)GetComponent("btnLeftBType");
	m_pRightAButton	= (gui::CButton*)GetComponent("btnRightAType");
	m_pRightBButton	= (gui::CButton*)GetComponent("btnRightBType");

	m_pbtnMarkColor = (gui::CButton*)GetComponent("btnMarkColor");
	m_pbtnFrameColor = (gui::CButton*)GetComponent("btnFrameColor");
	m_pbtnBackgroundColor = (gui::CButton*)GetComponent("btnBackgroundColor");

	m_pExitButton	= (gui::CButton*)GetComponent("btnExit");
	m_pOKButton		= (gui::CButton*)GetComponent("btnOK");
	m_pCancelButton	= (gui::CButton*)GetComponent("btnCancel");

	m_pOKButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));

	// 재료 텍스처의 서페이스
	RwInt32 iStuffX = dEMBLEM_STUFF_START_X;
	RwInt32 iStuffY = dEMBLEM_STUFF_START_Y;
	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
	{
		for(RwUInt8 k = 0 ; k < dSTUFF_TEXTURE_COLUMN ; ++k)
		{
			m_StuffSlot[i][k].srfStuff.SetPositionfromParent(iStuffX, iStuffY);
			m_StuffSlot[i][k].srfStuff.SetSize(dEMBLEM_STUFF_SIZE, dEMBLEM_STUFF_SIZE);
			iStuffX += dEMBLEM_STUFF_COLUMN_GAP + dEMBLEM_STUFF_SIZE;
		}

		iStuffX = dEMBLEM_STUFF_START_X;
		iStuffY += dEMBLEM_STUFF_ROW_GAP + dEMBLEM_STUFF_SIZE;
	}

	// 팔레트 영역
	RwInt32 iPaletteX = dPALETTE_START_X;
	RwInt32 iPaletteY = dPALETTE_START_Y;
	for(RwUInt8 i = 0 ; i < dPALETTE_ROW ; ++i )
	{
		for(RwUInt8 k = 0 ; k < dPALETTE_COLUMN ; ++k)
		{
			m_rtPalette[i*dPALETTE_COLUMN + k].SetRectWH(iPaletteX, iPaletteY, dPALETTE_SIZE, dPALETTE_SIZE);
			iPaletteX += dPALETTE_GAP + dPALETTE_SIZE;;
		}

		iPaletteX = dPALETTE_START_X;
		iPaletteY += dPALETTE_GAP + dPALETTE_SIZE;
	}

	// 셀렉트 & 포커스 서페이스
	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
		m_srfEmblemSelect[i].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildEmblemMaker.srf", "srfSelectEmblem" ) );

	m_srfEmblemFocus.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildEmblemMaker.srf", "srfSelectEmblem" ) );
	m_srfPaletteFocus.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GuildEmblemMaker.srf", "srfSelectColor" ) );

	// Emblem 구성 요소
	m_pEmblemFactor = NTL_NEW sEmblemFactor;

	// 재료 텍스처 리스트 초기화
	m_mapStuffTypeA = (MAP_STUFF*)GetEmblemMaker()->GetStuffList(EMBLEMTYPE_A);
	m_mapStuffTypeB = (MAP_STUFF*)GetEmblemMaker()->GetStuffList(EMBLEMTYPE_B);

	if( m_mapStuffTypeA->size() > 0 )
		SetTextureStuff(EMBLEMTYPE_A, m_mapStuffTypeA->begin()->second);

	if( m_mapStuffTypeB->size() > 0 )
		SetTextureStuff(EMBLEMTYPE_B, m_mapStuffTypeB->begin()->second);

	m_MouseInfo.bySlotType	= dSLOTSTUFF_TYPE_INAVLID;
	m_MouseInfo.bySlotIndex	= INVALID_BYTE;

	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW; ++i)
	{
		m_StuffSelect[i].bySelectStuffIndex	= INVALID_INDEX;
	}
	

	m_slotLeftAButton	= m_pLeftAButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_LeftAButton);
	m_slotLeftBButton	= m_pLeftBButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_LeftBButton);
	m_slotRightAButton	= m_pRightAButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_RightAButton);
	m_slotRightBButton	= m_pRightBButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_RightBButton);

	m_slotMarkColorButton = m_pbtnMarkColor->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_MarkColorButton);
	m_slotFrameColorButton = m_pbtnFrameColor->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_FrameColorButton);
	m_slotBackgroundColorButton = m_pbtnBackgroundColor->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_BackgroundColorButton);

	m_slotExitButton	= m_pExitButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_CancelButton);
	m_slotOKButton		= m_pOKButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_OKButton);
	m_slotCancelButton	= m_pCancelButton->SigClicked().Connect(this, &CGuildEmblemMakerGui::OnClicked_CancelButton);

	// Link updatebefore
	GetNtlGuiManager()->AddUpdateBeforeFunc( this );

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CGuildEmblemMakerGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CGuildEmblemMakerGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CGuildEmblemMakerGui::OnMove );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CGuildEmblemMakerGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CGuildEmblemMakerGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CGuildEmblemMakerGui::OnPaint );	
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CGuildEmblemMakerGui::OnCaptureMouseDown );

	LinkMsg(g_EventPostDialog);

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CGuildEmblemMakerGui::Destroy()
{
	NTL_FUNCTION("CGuildEmblemMakerGui::Destroy");

	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
	{
		for(RwUInt8 k = 0 ; k < dSTUFF_TEXTURE_COLUMN ; ++k)
		{
			Logic_DeleteTexture( m_StuffSlot[i][k].srfStuff.GetTexture() );
		}
	}

	if( !m_strEmblemTextureName.empty() )
	{
		GetEmblemMaker()->DestoyEmblem( m_strEmblemTextureName.c_str() );
		m_strEmblemTextureName.clear();
	}

	NTL_DELETE(m_pEmblemFactor);

	// Unlink updatebefore
	GetNtlGuiManager()->RemoveUpdateBeforeFunc( this );

	UnLinkMsg(g_EventPostDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CGuildEmblemMakerGui::UpdateBeforeCamera(RwReal fElapsed)
{
	if( !m_bUpdateEmblem )
		return;

	if( !m_strEmblemTextureName.empty() )
	{
		GetEmblemMaker()->DestoyEmblem( m_strEmblemTextureName.c_str() );
		m_strEmblemTextureName.clear();
	}
	
	m_srfEmblem.UnsetTexture();

	RwTexture* pEmblemTexture = GetEmblemMaker()->CreateEmblem(m_pEmblemFactor);
	if( pEmblemTexture )
	{
		m_strEmblemTextureName = RwTextureGetNameMacro(pEmblemTexture);
		m_EmblemTexture.Load(pEmblemTexture);
		m_srfEmblem.SetTexture(&m_EmblemTexture);
	}

	m_bUpdateEmblem = FALSE;
}

VOID CGuildEmblemMakerGui::Active(bool bOpen)
{
	RaiseTop();
	Show(bOpen);
}

VOID CGuildEmblemMakerGui::SetTextureStuff(RwInt8 byStuffType, RwUInt32 uiStuffTextureIndex)
{
	typedef std::map<RwUInt32, gui::CTexture*>				TEMP_STUFF_TEXTURE;

	char acBuffer[256] = "";
	MAP_STUFF* pmapStuffType = NULL;
	MAP_STUFF_ITER it;	
	TEMP_STUFF_TEXTURE mapTempTexture;

	switch(byStuffType)
	{
		case EMBLEMTYPE_A:		pmapStuffType = m_mapStuffTypeA;		break;
		case EMBLEMTYPE_B:		pmapStuffType = m_mapStuffTypeB;		break;
		default:
			{
				DBO_FAIL("Invalid stuff type : " << byStuffType);
				return;
			}
	}

	// 기존에 존재하는 텍스처를 재활용하기 위해 로딩된 텍스처 보관
	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_COLUMN ; ++i)
	{
		if( m_StuffSlot[byStuffType][i].srfStuff.GetTexture() )
			mapTempTexture[ m_StuffSlot[byStuffType][i].byStuffTextureIndex ] = m_StuffSlot[byStuffType][i].srfStuff.GetTexture();

		m_StuffSlot[byStuffType][i].srfStuff.UnsetTexture();
	}

	// 화면에 보여지는 재료 텍스처의 재구성
	it = pmapStuffType->find(uiStuffTextureIndex);
	if( it != pmapStuffType->end() )
	{
		// 화면에 보여줄 연속된 4개의 재료 텍스처를 찾는다
		RwUInt32 uiMaxStuff = pmapStuffType->size();

		if( uiMaxStuff > dSTUFF_TEXTURE_COLUMN )
			uiMaxStuff = dSTUFF_TEXTURE_COLUMN;

		for(RwUInt32 i = 0 ; i <  uiMaxStuff ; ++i )
		{			
			TEMP_STUFF_TEXTURE::iterator it_tempFind = mapTempTexture.find(it->second);
			if( it_tempFind != mapTempTexture.end() )
			{
				// 재료 텍스처의 인덱스가 메모리에 존재하면
				m_StuffSlot[byStuffType][i].srfStuff.SetTexture( it_tempFind->second );
				m_StuffSlot[byStuffType][i].byStuffTextureIndex = (RwUInt8)it->second;
				mapTempTexture.erase(it_tempFind);
			}
			else
			{
				// 메모리로 새로운 재료 텍스처를 읽어들인다				
                ZeroMemory(acBuffer, sizeof(acBuffer));
				GetEmblemMaker()->MakeEmblemResourceName((eEmblemType)byStuffType, (RwUInt8)it->second, acBuffer, 64);
				m_StuffSlot[byStuffType][i].srfStuff.SetTexture( Logic_CreateTexture(acBuffer, TEXTTYPE_EMBLEM) );
				m_StuffSlot[byStuffType][i].byStuffTextureIndex = (RwUInt8)it->second;
			}

			++it;

			if( it == pmapStuffType->end() )
				it = pmapStuffType->begin();
		}
	}

	// 재사용되지 않은 텍스처는 메모리에서 삭제한다
	std::map<RwUInt32, gui::CTexture*>::iterator it_TempTexture = mapTempTexture.begin();
	for( ; it_TempTexture != mapTempTexture.end() ; ++it_TempTexture )
	{
		Logic_DeleteTexture(it_TempTexture->second);
	}
}

RwUInt8 CGuildEmblemMakerGui::PtInRectStuffEmblem(RwInt8 byRow, RwInt32 nX, RwInt32 nY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_COLUMN ; ++i)
	{
		if( m_StuffSlot[byRow][i].srfStuff.PtInRect(rtScreen.left + nX, rtScreen.top + nY) )
		{
			if( m_StuffSlot[byRow][i].byStuffTextureIndex != dPLEMBLEM_INVALID_FACTOR )
				return i;
			else
                return INVALID_BYTE;
		}
	}	

	return INVALID_BYTE;
}

RwUInt8 CGuildEmblemMakerGui::PtInRectPalette(RwInt32 nX, RwInt32 nY)
{
	for(RwUInt8 i = 0 ; i < dPALETTE_ROW ; ++i)
	{
		for(RwUInt8 k = 0 ; k < dPALETTE_COLUMN ; ++k)
		{
			if( m_rtPalette[i*dPALETTE_COLUMN + k].PtInRect(nX, nY) )
			{
				return i*dPALETTE_COLUMN + k;
			}
		}
	}

	return INVALID_BYTE;
}

VOID CGuildEmblemMakerGui::OnClicked_LeftAButton(gui::CComponent* pComponent)
{
	MAP_STUFF_ITER it = m_mapStuffTypeA->find( m_StuffSlot[EMBLEMTYPE_A][0].byStuffTextureIndex );

	if( it == m_mapStuffTypeA->end() )
		return;

	if( it != m_mapStuffTypeA->begin() )
	{
		--it;
	}
	else if( it == m_mapStuffTypeA->begin() )
	{
		it = m_mapStuffTypeA->end();
		--it;
	}

	SetTextureStuff(EMBLEMTYPE_A, it->second);
}

VOID CGuildEmblemMakerGui::OnClicked_LeftBButton(gui::CComponent* pComponent)
{
	MAP_STUFF_ITER it = m_mapStuffTypeB->find( m_StuffSlot[EMBLEMTYPE_B][0].byStuffTextureIndex );

	if( it == m_mapStuffTypeB->end() )
		return;

	if( it != m_mapStuffTypeB->begin() )
	{
		--it;
	}
	else if( it == m_mapStuffTypeB->begin() )
	{
		it = m_mapStuffTypeB->end();
		--it;
	}

	SetTextureStuff(EMBLEMTYPE_B, it->second);
}

VOID CGuildEmblemMakerGui::OnClicked_RightAButton(gui::CComponent* pComponent)
{
	MAP_STUFF_ITER it = m_mapStuffTypeA->find( m_StuffSlot[EMBLEMTYPE_A][1].byStuffTextureIndex );

	if( it == m_mapStuffTypeA->end() )
		return;

	SetTextureStuff(EMBLEMTYPE_A, it->second);
}

VOID CGuildEmblemMakerGui::OnClicked_RightBButton(gui::CComponent* pComponent)
{
	MAP_STUFF_ITER it = m_mapStuffTypeB->find( m_StuffSlot[EMBLEMTYPE_B][1].byStuffTextureIndex );

	if( it == m_mapStuffTypeB->end() )
		return;

	SetTextureStuff(EMBLEMTYPE_B, it->second);
}

void CGuildEmblemMakerGui::OnClicked_MarkColorButton(gui::CComponent * pComponent)
{
	m_byCurStuffRow = EMBLEMTYPE_A;

	m_pbtnMarkColor->Enable(false);
	m_pbtnFrameColor->Enable(true);
	m_pbtnBackgroundColor->Enable(true);
}

void CGuildEmblemMakerGui::OnClicked_FrameColorButton(gui::CComponent * pComponent)
{
	m_byCurStuffRow = EMBLEMTYPE_B;

	m_pbtnMarkColor->Enable(true);
	m_pbtnFrameColor->Enable(false);
	m_pbtnBackgroundColor->Enable(true);
}

void CGuildEmblemMakerGui::OnClicked_BackgroundColorButton(gui::CComponent * pComponent)
{
	m_byCurStuffRow = EMBLEMTYPE_C;

	m_pbtnMarkColor->Enable(true);
	m_pbtnFrameColor->Enable(true);
	m_pbtnBackgroundColor->Enable(false);
}

VOID CGuildEmblemMakerGui::OnClicked_OKButton(gui::CComponent* pComponent)
{
	if( m_pEmblemFactor->byTypeA == dPLEMBLEM_INVALID_FACTOR ||
		m_pEmblemFactor->byTypeB == dPLEMBLEM_INVALID_FACTOR ||
		m_pEmblemFactor->byTypeC == dPLEMBLEM_INVALID_FACTOR )
	{
		GetAlarmManager()->AlarmMessage( "DST_GUILD_NOT_SELECT_EMBLEM" );
		return;
	}

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();

	if( pGuild->IsHaveEmblem() )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendGuildChangeEmblemReq(
													m_pEmblemFactor->byTypeA, m_pEmblemFactor->byTypeAColor,
													m_pEmblemFactor->byTypeB, m_pEmblemFactor->byTypeBColor,
													m_pEmblemFactor->byTypeC, m_pEmblemFactor->byTypeCColor);
	}
	else
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendGuildCreateEmblemReq(
													m_pEmblemFactor->byTypeA, m_pEmblemFactor->byTypeAColor,
													m_pEmblemFactor->byTypeB, m_pEmblemFactor->byTypeBColor,
													m_pEmblemFactor->byTypeC, m_pEmblemFactor->byTypeCColor);
	}
}

VOID CGuildEmblemMakerGui::OnClicked_CancelButton(gui::CComponent* pComponent)
{
	Enable(false);
	CDboEventGenerator::DialogPostEvent(DIALOGEVENT_CLOSE_EMBLEM_MAKER);
}

VOID CGuildEmblemMakerGui::OnMouseDown(const CKey& key)
{
	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
	{
		m_MouseInfo.bySlotIndex = PtInRectStuffEmblem(i, (RwInt32)key.m_fX, (RwInt32)key.m_fY);
		if( m_MouseInfo.bySlotIndex != INVALID_BYTE )
		{
			m_MouseInfo.bySlotType = i;
			return;
		}
	}
	
	m_MouseInfo.bySlotIndex = PtInRectPalette((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_MouseInfo.bySlotIndex != INVALID_BYTE )
		m_MouseInfo.bySlotType = dSLOTSTUFF_TYPE_PALETTE;
}

VOID CGuildEmblemMakerGui::OnMouseUp(const CKey& key)
{
	if( m_MouseInfo.bySlotIndex == INVALID_BYTE )
		return;

	if( m_MouseInfo.bySlotIndex == PtInRectStuffEmblem(m_MouseInfo.bySlotType, (RwInt32)key.m_fX, (RwInt32)key.m_fY) )
	{
		m_bUpdateEmblem = TRUE;

		switch (m_MouseInfo.bySlotType)
		{
			case EMBLEMTYPE_A:
			{
				m_pEmblemFactor->byTypeA = m_StuffSlot[EMBLEMTYPE_A][m_MouseInfo.bySlotIndex].byStuffTextureIndex;
				m_StuffSelect[EMBLEMTYPE_A].bySelectStuffIndex = m_pEmblemFactor->byTypeA;
			}
			break;
			case EMBLEMTYPE_B:
			{
				m_pEmblemFactor->byTypeB = m_StuffSlot[EMBLEMTYPE_B][m_MouseInfo.bySlotIndex].byStuffTextureIndex;
				m_StuffSelect[EMBLEMTYPE_B].bySelectStuffIndex = m_pEmblemFactor->byTypeB;

				m_pEmblemFactor->byTypeC = m_StuffSlot[EMBLEMTYPE_B][m_MouseInfo.bySlotIndex].byStuffTextureIndex;
			}
			break;
			/*case EMBLEMTYPE_C:
			{
				m_pEmblemFactor->byTypeC = m_StuffSlot[EMBLEMTYPE_C][m_MouseInfo.bySlotIndex].byStuffTextureIndex;
				m_StuffSelect[EMBLEMTYPE_C].bySelectStuffIndex = m_pEmblemFactor->byTypeC;
				break;
			}*/
			default: DBO_FAIL("Ivalid emblem stuff type : " << (int)m_MouseInfo.bySlotType);
		}
	}
	else if( m_MouseInfo.bySlotType == dSLOTSTUFF_TYPE_PALETTE )
	{
		if( m_MouseInfo.bySlotIndex == PtInRectPalette((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
		{
			if( m_byCurStuffRow != INVALID_BYTE )
			{
				// 팔레트를 선택했다
				switch(m_byCurStuffRow)
				{
					case 0:	m_pEmblemFactor->byTypeAColor = m_MouseInfo.bySlotIndex; break;
					case 1:	m_pEmblemFactor->byTypeBColor = m_MouseInfo.bySlotIndex; break;
					case 2:	m_pEmblemFactor->byTypeCColor = m_MouseInfo.bySlotIndex; break;
					default: DBO_FAIL("dSLOTSTUFF_TYPE_PALETTE == m_MouseInfo.bySlotType, Invalid stuff row : " << (int)m_byCurStuffRow);
				}

				m_bUpdateEmblem = TRUE;
			}
		}
	}

	m_MouseInfo.bySlotIndex	= INVALID_BYTE;
	m_MouseInfo.bySlotType	= dSLOTSTUFF_TYPE_INAVLID;
}

VOID CGuildEmblemMakerGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
	{
		for(RwUInt8 k = 0 ; k < dSTUFF_TEXTURE_COLUMN ; ++k)
		{
			m_StuffSlot[i][k].srfStuff.SetPositionbyParent(rtScreen.left, rtScreen.top);
		}
	}

	m_srfEmblem.SetRectWH(rtScreen.left + 15, rtScreen.top + 211, dEMBLEM_TEXTURE_DEFAULT_SIZE, dEMBLEM_TEXTURE_DEFAULT_SIZE);
}

VOID CGuildEmblemMakerGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	// 재료 텍스처 포커스
	RwUInt8 byStuffFocus = INVALID_BYTE;
	for(RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i)
	{
		byStuffFocus = PtInRectStuffEmblem(i, nX, nY);
		if( byStuffFocus != INVALID_BYTE )
		{
			CRectangle rtRect;
			m_StuffSlot[i][byStuffFocus].srfStuff.GetRect(rtRect);
			m_srfEmblemFocus.SetRect(rtRect);
			m_bStuffFocus = TRUE;
			return;
		}
	}

	m_bStuffFocus = FALSE;

	// 팔레트 포커스
	m_byPaletteFocus = PtInRectPalette(nX, nY);
	if( m_byPaletteFocus != INVALID_BYTE )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_srfPaletteFocus.SetRectWH(rtScreen.left + m_rtPalette[m_byPaletteFocus].left,
								  rtScreen.top + m_rtPalette[m_byPaletteFocus].top,
								  m_rtPalette[m_byPaletteFocus].GetWidth(),
								  m_rtPalette[m_byPaletteFocus].GetHeight());
	}
}

VOID CGuildEmblemMakerGui::OnMouseLeave(gui::CComponent* pComponent)
{	
	m_bStuffFocus			= FALSE;
	m_byPaletteFocus		= INVALID_BYTE;
}

VOID CGuildEmblemMakerGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED( DIALOG_COMMUNITY, (RwInt32)key.m_fX, (RwInt32)key.m_fY );
}

VOID CGuildEmblemMakerGui::OnPaint()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	for( RwUInt8 i = 0 ; i < dSTUFF_TEXTURE_ROW ; ++i )
	{
		for( RwUInt8 k = 0 ; k < dSTUFF_TEXTURE_COLUMN ; ++k )
		{
			if( m_StuffSlot[i][k].srfStuff.GetTexture() )
				m_StuffSlot[i][k].srfStuff.Render();
		}
	}

	for( RwUInt8 k = 0 ; k < dSTUFF_TEXTURE_COLUMN ; ++k )
	{
		if( m_StuffSelect[0].bySelectStuffIndex != INVALID_INDEX )
		{
			if( m_pEmblemFactor->byTypeA == m_StuffSlot[0][k].byStuffTextureIndex )
			{
				RwInt32 iXPos = dEMBLEM_STUFF_START_X + (dEMBLEM_STUFF_SIZE + dEMBLEM_STUFF_COLUMN_GAP)*k;
				RwInt32 iYPos = dEMBLEM_STUFF_START_Y;

				m_srfEmblemSelect[0].SetRectWH(rtScreen.left + iXPos, rtScreen.top + iYPos, dEMBLEM_STUFF_SIZE, dEMBLEM_STUFF_SIZE);
				m_srfEmblemSelect[0].Render();
			}
		}

		if( m_StuffSelect[1].bySelectStuffIndex != INVALID_INDEX )
		{
			if( m_pEmblemFactor->byTypeB == m_StuffSlot[1][k].byStuffTextureIndex )
			{
				RwInt32 iXPos = dEMBLEM_STUFF_START_X + (dEMBLEM_STUFF_SIZE + dEMBLEM_STUFF_COLUMN_GAP)*k;
				RwInt32 iYPos = dEMBLEM_STUFF_START_Y + (dEMBLEM_STUFF_SIZE + dEMBLEM_STUFF_ROW_GAP);

				m_srfEmblemSelect[1].SetRectWH(rtScreen.left + iXPos, rtScreen.top + iYPos, dEMBLEM_STUFF_SIZE, dEMBLEM_STUFF_SIZE);
				m_srfEmblemSelect[1].Render();
			}
		}

		/*if( m_StuffSelect[2].bySelectStuffIndex != INVALID_INDEX )
		{
			if( m_pEmblemFactor->byTypeC == m_StuffSlot[2][k].byStuffTextureIndex )
			{
				RwInt32 iXPos = dEMBLEM_STUFF_START_X + (dEMBLEM_STUFF_SIZE + dEMBLEM_STUFF_COLUMN_GAP)*k;
				RwInt32 iYPos = dEMBLEM_STUFF_START_Y + (dEMBLEM_STUFF_SIZE + dEMBLEM_STUFF_ROW_GAP)*2;

				m_srfEmblemSelect[2].SetRectWH(rtScreen.left + iXPos, rtScreen.top + iYPos, dEMBLEM_STUFF_SIZE, dEMBLEM_STUFF_SIZE);
				m_srfEmblemSelect[2].Render();
			}
		}*/
	}

	if( m_bStuffFocus )
		m_srfEmblemFocus.Render();

	if( m_byPaletteFocus != INVALID_BYTE )
		m_srfPaletteFocus.Render();

	if( m_srfEmblem.GetTexture() )
		m_srfEmblem.Render();
}

VOID CGuildEmblemMakerGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGuildEmblemMakerGui::HandleEvents");

	if( msg.Id == g_EventPostDialog )
	{
		SDboEventDialog* pEvent = (SDboEventDialog*)msg.pData;

		if( pEvent->iType == DIALOGEVENT_FAIL_CLOSE_EMBLEM_MAKER )
			Enable(true);
	}

	NTL_RETURNVOID();
}