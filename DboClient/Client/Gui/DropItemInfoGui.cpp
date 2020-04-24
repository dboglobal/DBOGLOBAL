#include "precomp_dboclient.h"
#include "DropItemInfoGui.h"

// core
#include "NtlDebug.h"
#include "NtlPLDef.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobWorldItem.h"
#include "NtlSobWorldItemAttr.h"
#include "NtlSobWorldItemProxy.h"
#include "NtlSobManager.h"

// dbo
#include "DboDef.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "InfoWndManager.h"
#include "DboLogic.h"
#include "GuiFunctor.h"
#include "QuestGuiDef.h"
#include "DisplayStringManager.h"

#define DROPITEM_OFFSET_X	-10
#define DROPITEM_OFFSET_Y	-60
#define DROPITEM_SHOW_TIME	5.0f
#define DROPITEM_FADE_TIME	1.0f

CDropItemInfoGui::CDropItemInfoGui(VOID)
{
	Init();
}

CDropItemInfoGui::CDropItemInfoGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	Init();
}

CDropItemInfoGui::~CDropItemInfoGui(VOID)
{

}

VOID CDropItemInfoGui::Init(VOID)
{
	m_fTime = 0.0f;
	m_eState = HIDE;
}

RwBool CDropItemInfoGui::Create(VOID)
{
	NTL_FUNCTION( "CDropItemInfoGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\DropItemInfo.srf", "gui\\DropItemInfo.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	
	m_slotMouseDown = m_pThis->SigMouseDown().Connect( this, &CDropItemInfoGui::OnMouseDown );
	m_slotMouseEnter = m_pThis->SigMouseEnter().Connect( this, &CDropItemInfoGui::OnMouseEnter );
	m_slotMouseLeave = m_pThis->SigMouseLeave().Connect( this, &CDropItemInfoGui::OnMouseLeave );
	m_slotPaint	= m_pThis->SigPaint().Connect( this, &CDropItemInfoGui::OnPaint );
	m_slotMove	= m_pThis->SigMove().Connect( this, &CDropItemInfoGui::OnMove );
	
	m_surIcon.SetRectWH( 0, 0, 32, 32 );	

	GetNtlGuiManager()->AddUpdateFunc( this );
	SetState( HIDE );

	LinkMsg( g_EventSobGotFocus );
	LinkMsg( g_EventSobLostFocus );
	LinkMsg( g_EventCharObjDelete );

	CDboEventGenerator::OpenHelpContent(DIALOG_DROPITEM_INFO);

	NTL_RETURN( TRUE );
}

VOID CDropItemInfoGui::Destroy(VOID)
{
	NTL_FUNCTION( "CDropItemInfoGui::Destroy" );

	UnLinkMsg( g_EventSobGotFocus );
	UnLinkMsg( g_EventSobLostFocus );
	UnLinkMsg( g_EventCharObjDelete );

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	SetState( HIDE );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDropItemInfoGui::Update( RwReal fElapsedTime )
{
	if( m_eState != HIDE )
	{
		m_fTime += fElapsedTime;

		PositionUpdate();		
		FadeEffect();
	}
}

RwInt32 CDropItemInfoGui::SwitchDialog( bool bOpen )
{
	return 1;
}

VOID CDropItemInfoGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobGotFocus )
	{
		SNtlEventSobGotFocus* pData = reinterpret_cast<SNtlEventSobGotFocus*>( msg.pData );
		CNtlSob* pObj = GetNtlSobManager()->GetSobObject( pData->hSerialId );

		if( !pObj )
			return;

		if( pObj->GetClassID() != SLCLASS_WORLD_ITEM )
			return;

		CNtlSobWorldItem* pDropObject = reinterpret_cast<CNtlSobWorldItem*>( pObj );

		if( m_uiDropItemSerial == pDropObject->GetSerialID() )
		{
			switch( m_eState )
			{			
				case SHOW_WAIT_FADE: SetState( SHOW ); break;
				case FADE_OUT: SetState( FADE_IN ); break;
			}
		}
		else
		{
			m_pDropItem = pDropObject;
			m_uiDropItemSerial = pDropObject->GetSerialID();
			Logic_DeleteTexture( m_surIcon.GetTexture() );
			m_surIcon.UnsetTexture();

			// icon surface setting.
			CNtlSobWorldItemAttr* pAttr = reinterpret_cast<CNtlSobWorldItemAttr*>( m_pDropItem->GetSobAttr() );
			if( pAttr->IsItem() )
			{
				if( pAttr->IsIdentified() )
					m_surIcon.SetTexture( Logic_CreateTexture( pAttr->GetItemTbl()->szIcon_Name ) );
				else
					m_surIcon.SetTexture( Logic_CreateTexture( UNIDENTIFIED_ICON_NAME ) );					
			}
			else if( pAttr->IsMoney() )
			{
				m_surIcon.SetTexture( Logic_CreateTexture(  QUEST_REWARD_ICONNAME_ZENY ) ) ;				
			}
			else
				return;			

			SetState( FADE_IN );				
		}
		
	}
	else if( msg.Id == g_EventSobLostFocus )
	{
		SNtlEventSobLostFocus* pData = reinterpret_cast<SNtlEventSobLostFocus*>( msg.pData );

		if( pData->hSerialId != m_uiDropItemSerial )
			return;		

		switch( m_eState )
		{
		case FADE_IN: m_bDirectToShowWaitFade = TRUE; break;
		case SHOW: SetState( SHOW_WAIT_FADE ); break;				
		}		
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_uiDropItemSerial == *pDeleteSerial )
		{
			SetState( HIDE );
		}
	}
}

VOID CDropItemInfoGui::SetState( RwInt32 eState )
{
	switch( eState )
	{
	case HIDE:
		m_uiDropItemSerial = INVALID_SERIAL_ID;
		m_pDropItem = NULL;
		OnMouseLeave( NULL );	// 인포윈도우 닫기 
		Logic_DeleteTexture( m_surIcon.GetTexture() );
		m_surIcon.UnsetTexture();
		Show( false );
		break;
	case FADE_IN:
		m_fTime = 0.0f;
		Show( true );
		break;
	case SHOW_WAIT_FADE:				
	case FADE_OUT:
		m_fTime = 0.0f;
		break;
	}

	m_eState = eState;
}

VOID CDropItemInfoGui::PositionUpdate(VOID)
{
	if( !m_pDropItem )
		return;

	CPos posRes;
	CPos posOffset( DROPITEM_OFFSET_X, DROPITEM_OFFSET_Y );

    RwV3d vDropItemPos = m_pDropItem->GetPosition();
	if( !GetGuiFuntor()->Calc3DPosTo2D(vDropItemPos, 1.0f, &posOffset, &posRes ) )
	{
		Show( false );
		return;
	}

	SetPosition( posRes.x, posRes.y );	
	Show( true );	
}

VOID CDropItemInfoGui::FadeEffect(VOID)
{
	if( m_eState == SHOW_WAIT_FADE )
	{
		if( m_fTime > DROPITEM_SHOW_TIME )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fTime < DROPITEM_FADE_TIME )
		{
			RwReal fResult = gui::GetResultLineInterpolation( m_fTime, DROPITEM_FADE_TIME, 0.0f, 255.0f );
			SetAlpha( (RwUInt8)fResult );
		}
		else
		{
			SetAlpha( 255 );
			if( m_bDirectToShowWaitFade )
				SetState( SHOW_WAIT_FADE );
			else
				SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fTime < DROPITEM_FADE_TIME )
		{
			RwReal fResult = gui::GetResultLineInterpolation( m_fTime, DROPITEM_FADE_TIME, 255.0f, 0.0f );
			SetAlpha( (RwUInt8)fResult );
		}
		else
		{
			SetState( HIDE );
		}
	}
}

VOID CDropItemInfoGui::SetAlpha( RwUInt8 ucAlpha )
{
	m_pThis->SetAlpha( ucAlpha );
	m_surIcon.SetAlpha( ucAlpha );
}

VOID CDropItemInfoGui::OnMouseDown( const CKey& key )
{
	gui::CGUIManager* pGuiManager = GetNtlGuiManager()->GetGuiManager();
	pGuiManager->SetFocus( pGuiManager );

	Logic_WorldItemPick( m_uiDropItemSerial );
}

VOID CDropItemInfoGui::OnMouseEnter( gui::CComponent* pComponent )
{
	CNtlSobWorldItemAttr* pAttr = reinterpret_cast<CNtlSobWorldItemAttr*>( m_pDropItem->GetSobAttr() );
	CRectangle rtScreen = m_pThis->GetScreenRect();

	if( pAttr->IsItem() )
	{
		if( pAttr->IsIdentified() )
		{
			sITEM_PROFILE	sItemProfile;
			sItemProfile.handle = m_pDropItem->GetSerialID();
			sItemProfile.tblidx = pAttr->GetItemTbl()->tblidx;
			sItemProfile.byPlace = CONTAINER_TYPE_NONE;
			sItemProfile.byPos = INVALID_BYTE;
			sItemProfile.byStackcount = 1;
			sItemProfile.byRank = (BYTE)pAttr->GetRank();
			sItemProfile.byCurDur = pAttr->GetItemTbl()->byDurability;
			sItemProfile.bNeedToIdentify = !pAttr->IsIdentified();
			sItemProfile.byGrade = (BYTE)pAttr->GetGrade();			
			sItemProfile.byBattleAttribute = pAttr->GetBattleAttribute();

			sItemProfile.byRestrictState = ITEM_RESTRICT_STATE_TYPE_NONE;
			memset( sItemProfile.awchMaker, 0, NTL_MAX_SIZE_CHAR_NAME + 1 );
			
			sItemProfile.byDurationType = eDURATIONTYPE_NORMAL;
			sItemProfile.nUseStartTime = 0;
			sItemProfile.nUseEndTime = 0;

			sItemProfile.sOptionSet.Init();
			memcpy(&sItemProfile.sOptionSet, pAttr->GetItemOptionSet(), sizeof(sITEM_OPTION_SET));
			
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM, rtScreen.left, rtScreen.top, &sItemProfile, DIALOG_DROPITEM_INFO );
		}
		else
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rtScreen.left, rtScreen.top, NULL, DIALOG_DROPITEM_INFO );		
	}
	else if( pAttr->IsMoney() )
	{
		WCHAR wBuf[256];
		swprintf_s( wBuf, 256, GetDisplayStringManager()->GetString( "DST_DROPITEM_ZENNY" ), Logic_FormatZeni((DWORD)pAttr->GetMoney()) );
		GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT, rtScreen.left, rtScreen.top, wBuf, DIALOG_DROPITEM_INFO );
	}
}

VOID CDropItemInfoGui::OnMouseLeave( gui::CComponent* pComponent )
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_DROPITEM_INFO )
		GetInfoWndManager()->ShowInfoWindow( FALSE );	
}

VOID CDropItemInfoGui::OnPaint(VOID)
{
	m_surIcon.Render();
}

VOID CDropItemInfoGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rect = m_pThis->GetScreenRect();
	m_surIcon.SetPosition( rect.left, rect.top );
}