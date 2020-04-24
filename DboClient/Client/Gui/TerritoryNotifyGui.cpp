#include "precomp_dboclient.h"
#include "TerritoryNotifyGui.h"

// core
#include "NtlDebug.h"

// table
#include "TableContainer.h"
#include "MobTable.h"
#include "TextAllTable.h"

// sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobMonsterAttr.h"

// Dbo
#include "GuiFunctor.h"
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogPriority.h"


#define TERRITORY_NOTIFY_MAX_ALPHA		255
#define TERRITORY_NOTIFY_MIN_ALPHA		0
#define TERRITORY_NOTIFY_TIME_FADEIN	1.0f
#define TERRITORY_NOTIFY_TIME_SHOW		4.0f
#define TERRITORY_NOTIFY_TIME_FADEOUT	1.0f
#define TERRITORY_NOTIFY_COLOR_ZONE		RGB( 180, 238, 255 )
#define TERRITORY_NOTIFY_COLOR_SECTOR	RGB( 180, 238, 255 )
#define TERRITORY_NOTIFY_COLOR_FINISH	RGB( 243, 106, 124 )
#define TERRITORY_NOTIFY_ZONE_FONT		0
#define TERRITORY_NOTIFY_SECTOR_FONT	1
#define TERRITORY_NOTIFY_SHADOW_COLOR	RGB( 0, 0, 0 )
#define TERRITORY_NOTIFY_SHADOW_OFFSET	1
//#define TERRITORY_NOTIFY_YPOS_RATE		0.319f
#define TERRITORY_NOTIFY_YPOS_RATE		0.345f

CTerritoryNotifyGui::CTerritoryNotifyGui(VOID)
{
	Init();
}

CTerritoryNotifyGui::CTerritoryNotifyGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CTerritoryNotifyGui::~CTerritoryNotifyGui(VOID)
{

}

VOID CTerritoryNotifyGui::Init(VOID)
{
	m_pmdbNotifyText = NULL;
	m_fCurrentTime = 0.0f;
	m_uiNextMapIndex = INVALID_TBLIDX;

	SetUpdateState( NONE );
}

RwBool CTerritoryNotifyGui::Create(VOID)
{
	NTL_FUNCTION( "CTerritoryNotifyGui::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\TerritoryNotify.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( CNtlPLGuiManager::GetInstance()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	m_pmdbNotifyText = (gui::CMDStaticBox*)GetComponent( "mdbText" );

	Show( false );
	
	LinkMsg( NPEI_IS_MAPNAME_CHANGE, 0 );
	LinkMsg( g_EventSobServerUpdateState, 0 );
	LinkMsg( g_EventResize, 0 );

	NTL_RETURN(TRUE);
}

VOID CTerritoryNotifyGui::Destroy(VOID)
{
	NTL_FUNCTION( "TerritoryNotifyGui::Destroy" );
	
	UnLinkMsg( NPEI_IS_MAPNAME_CHANGE );
	UnLinkMsg( g_EventSobServerUpdateState );
	UnLinkMsg( g_EventResize );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTerritoryNotifyGui::Update( RwReal fElapsed )
{
	m_fCurrentTime += fElapsed;

	(this->*StateUpdate)();
}

VOID CTerritoryNotifyGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == NPEI_IS_MAPNAME_CHANGE )
	{
		sNTL_EVENT_MAPNAME* pData = reinterpret_cast<sNTL_EVENT_MAPNAME*>( msg.pData );
		RwUInt32 tblidx = *(RwUInt32*)( pData );
	
		CMapNameTextTable* pText = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();

		std::wstring strZoneName, strSectorName;
		RwBool bShowZoneName, bShowSectorName;

		if( pText->GetText( tblidx, &strZoneName ) )
		{
			bShowZoneName = pText->GetZoneName( tblidx, &strZoneName );
			bShowSectorName = pText->GetSectorName( tblidx, &strSectorName );

			if( strZoneName == m_strOldZoneName && strSectorName == m_strOldSectorName )
				return;

			if( !bShowZoneName )
				return;

			m_strOldZoneName = strZoneName;
			m_strOldSectorName = strSectorName;
		}
		else
		{
			bShowSectorName = FALSE;			
		}
		
		m_pmdbNotifyText->SetAlpha( TERRITORY_NOTIFY_MIN_ALPHA );
		m_pmdbNotifyText->Clear();
		m_pmdbNotifyText->SetItem( strZoneName.c_str(), "ZoneName", TERRITORY_NOTIFY_ZONE_FONT, COMP_TEXT_CENTER, TERRITORY_NOTIFY_COLOR_ZONE, TERRITORY_NOTIFY_SHADOW_COLOR, TE_OUTLINE, TERRITORY_NOTIFY_SHADOW_OFFSET );
		if( bShowSectorName )
			m_pmdbNotifyText->SetItem( strSectorName.c_str(), "SectorName", TERRITORY_NOTIFY_SECTOR_FONT, COMP_TEXT_CENTER, TERRITORY_NOTIFY_COLOR_SECTOR, TERRITORY_NOTIFY_SHADOW_COLOR, TE_OUTLINE, TERRITORY_NOTIFY_SHADOW_OFFSET );
		m_pmdbNotifyText->DrawItem();

		GetNtlGuiManager()->AddUpdateFunc( this );
		Show( true );
		SetUpdateState( FADE_IN );

		Logic_PlayGUISound(GSD_SYSTEM_TERRAIN_NOTIFY);		

		// 위치는 다시 지정해야한다.
		SetPosition( CRectangle( 0, 0, m_pmdbNotifyText->GetWidth(), m_pmdbNotifyText->GetHeight() ) );
		PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );		
	}
	else if( msg.Id == g_EventSobServerUpdateState )
	{
		// maybe change the following to CBossNotifyGui::HandleEvents

		SNtlEventSobServerUpdateState* pData = reinterpret_cast<SNtlEventSobServerUpdateState*>( msg.pData );

		if( !pData->pState )
			return;

		if( pData->pState->sCharStateBase.byStateID != CHARSTATE_FAINTING )
			return;

		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pData->hSerialId);
		if( !pSob )
		{
			NTL_ASSERT(false, "CTerritoryNotifyGui::HandleEvents, Not exist sob object of handle : " << pData->hSerialId);
			return;
		}

		if( pSob->GetClassID() != SLCLASS_MONSTER )
			return;

		CNtlSobMonsterAttr* pSobMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pSob->GetSobAttr() );
		sMOB_TBLDAT* pMOB_TBLDAT = pSobMonsterAttr->GetMobTbl();

		// disable by daneos.. might need to do
		if( pMOB_TBLDAT->byGrade != MOB_GRADE_HERO)
			return;

		CTextTable* pMobTextTable = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
		sTEXT_TBLDAT* pNameData = reinterpret_cast<sTEXT_TBLDAT*>( pMobTextTable->FindData(pMOB_TBLDAT->Name) );
		if( !pNameData )
			return;

		Logic_PlayGUISound(GSD_FINISH_HERO_MOB);

		WCHAR awcBuffer[256] = L"";
		swprintf_s(awcBuffer, 256 , GetDisplayStringManager()->GetString("DST_FINISHED_MONSTER"), pNameData->wstrText.c_str());

		m_pmdbNotifyText->SetAlpha( TERRITORY_NOTIFY_MIN_ALPHA );
		m_pmdbNotifyText->Clear();
		m_pmdbNotifyText->SetItem( awcBuffer, "text", TERRITORY_NOTIFY_ZONE_FONT, COMP_TEXT_CENTER, TERRITORY_NOTIFY_COLOR_FINISH, TERRITORY_NOTIFY_SHADOW_COLOR, TE_SHADOW, TERRITORY_NOTIFY_SHADOW_OFFSET );
		m_pmdbNotifyText->DrawItem();

		GetNtlGuiManager()->AddUpdateFunc( this );
		Show( true );
		SetUpdateState( FADE_IN );

		// 위치는 다시 지정해야한다.
		SetPosition( CRectangle( 0, 0, m_pmdbNotifyText->GetWidth(), m_pmdbNotifyText->GetHeight() ) );
		PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );		
	}
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign( pPacket->iWidht, pPacket->iHeight );
	}
}

VOID CTerritoryNotifyGui::Update_None(VOID)
{

}

VOID CTerritoryNotifyGui::Update_FadeIn(VOID)
{
	if( m_fCurrentTime >= TERRITORY_NOTIFY_TIME_FADEIN )
	{
		m_pmdbNotifyText->SetAlpha( TERRITORY_NOTIFY_MAX_ALPHA );
		SetUpdateState( SHOW );
		return;
	}

	RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fCurrentTime, TERRITORY_NOTIFY_TIME_FADEIN, TERRITORY_NOTIFY_MIN_ALPHA, TERRITORY_NOTIFY_MAX_ALPHA ) );
	m_pmdbNotifyText->SetAlpha( byAlpha );
}

VOID CTerritoryNotifyGui::Update_Show(VOID)
{
	if( m_fCurrentTime >= TERRITORY_NOTIFY_TIME_SHOW )
	{
		SetUpdateState( FADE_OUT );
	}
}

VOID CTerritoryNotifyGui::Update_FadeOut(VOID)
{
	if( m_fCurrentTime >= TERRITORY_NOTIFY_TIME_FADEOUT )
	{
		m_pmdbNotifyText->SetAlpha( TERRITORY_NOTIFY_MIN_ALPHA );
		SetUpdateState( NONE );
		Show( false );
		GetNtlGuiManager()->RemoveUpdateFunc( this );
		return;
	}

	RwUInt8 byAlpha = (RwUInt8)( gui::GetResultLineInterpolation( m_fCurrentTime, TERRITORY_NOTIFY_TIME_FADEOUT, TERRITORY_NOTIFY_MAX_ALPHA, TERRITORY_NOTIFY_MIN_ALPHA ) );
	m_pmdbNotifyText->SetAlpha( byAlpha );
}

VOID CTerritoryNotifyGui::SetUpdateState( STATE eState )
{
	m_eState = eState;
	m_fCurrentTime = 0.0f;

	switch( eState )
	{
	case NONE: 
		StateUpdate = &CTerritoryNotifyGui::Update_None; break;
	case FADE_IN:
		StateUpdate = &CTerritoryNotifyGui::Update_FadeIn; break;
	case SHOW:
		StateUpdate = &CTerritoryNotifyGui::Update_Show; break;
	case FADE_OUT:
		StateUpdate = &CTerritoryNotifyGui::Update_FadeOut; break;		
	}
}

VOID CTerritoryNotifyGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	m_pThis->SetPosition( (RwInt32)( ( nScreenWidth - m_pThis->GetWidth() ) / 2 ), (RwInt32)( nScreenHeight * GetDboGlobal()->GetDBOUIConfig()->GetNotifyConfig()->fTerritoryNotify_YPosRate ) );		
}