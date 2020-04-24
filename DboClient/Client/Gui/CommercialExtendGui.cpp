#include "precomp_dboclient.h"
#include "CommercialExtendGui.h"

#include "NtlDebug.h"
#include "NtlPLGuiManager.h"
#include "StageManager.h"
#include "NtlStage.h"
#include "DboGlobal.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"
#include "MsgBoxManager.h"

#include "NtlSobItemAttr.h"



void CCommercialExtendMode::CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet)
{
	m_AttributeControlSet = *pAttributeControlSet;
	m_pTargetNtlSobItem = NULL;
}

void CCommercialExtendMode::InitShowComponent()
{
	if(SIZE_DEFAULT == m_AttributeControlSet.eSizeMode)
	{
		m_AttributeControlSet.pPanel_pnlPanel2->Show(true);
	}
	else
	{
		m_AttributeControlSet.pPanel_pnlPanel2->Show(false);
	}

	if(DEFAULT_BUTTONS == m_AttributeControlSet.eArrangeMode)
	{
		m_AttributeControlSet.pButt_btnConfirm->Show(true);
		m_AttributeControlSet.pButt_btnBuyDuration->Show(false);
		m_AttributeControlSet.pButt_btnRefillCash->Show(false);
	}
	else
	{
		m_AttributeControlSet.pButt_btnConfirm->Show(false);
		m_AttributeControlSet.pButt_btnBuyDuration->Show(true);
		m_AttributeControlSet.pButt_btnRefillCash->Show(true);
	}

	/// 크기 위치 조정
	CRectangle rect = m_AttributeControlSet.pDialog->GetPosition();
	rect.bottom = rect.top + m_AttributeControlSet.iDefaultWndHeight;
	m_AttributeControlSet.pDialog->SetPosition(rect);

		/// 크기 변경에 따른 하부 component 위치 조정
	int iDiff1 = m_AttributeControlSet.pStatic_stbDescript2->GetPosition().top - 
		m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top;

	int iDiff2 = m_AttributeControlSet.pButt_btnConfirm->GetPosition().top -
		m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top;

	rect = m_AttributeControlSet.pStatic_stbDescript1->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top - rect.top);
	m_AttributeControlSet.pStatic_stbDescript1->SetPosition(rect);

	rect = m_AttributeControlSet.pStatic_stbDescript2->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top + iDiff1 - rect.top);
	m_AttributeControlSet.pStatic_stbDescript2->SetPosition(rect);


	rect = m_AttributeControlSet.pButt_btnConfirm->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top + iDiff2 - rect.top);
	m_AttributeControlSet.pButt_btnConfirm->SetPosition(rect);

	rect = m_AttributeControlSet.pButt_btnBuyDuration->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top + iDiff2 - rect.top);
	m_AttributeControlSet.pButt_btnBuyDuration->SetPosition(rect);

	rect = m_AttributeControlSet.pButt_btnRefillCash->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top + iDiff2 - rect.top);
	m_AttributeControlSet.pButt_btnRefillCash->SetPosition(rect);

	rect = m_AttributeControlSet.pButt_btnCancel->GetPosition();
	rect.Offset(0, m_AttributeControlSet.iDefaultDescript1Top + iDiff2 - rect.top);
	m_AttributeControlSet.pButt_btnCancel->SetPosition(rect);

	/// 하부 버튼 횡 위치 조정
	rect = m_AttributeControlSet.pButt_btnCancel->GetPosition();
	int iWidth = rect.right - rect.left;

	rect.left = m_AttributeControlSet.iDefaultCancelButtLeft;
	rect.right = rect.left + iWidth;
	m_AttributeControlSet.pButt_btnCancel->SetPosition(rect);
}

void CCommercialExtendMode::SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT)
{
	CNtlSobItemAttr* pItemAttr = NULL;
	CRetCaculDayHourMinSecond TimeRes;
	WCHAR text[GUI_TEXT_BUFFER_SIZE];

	/// 남은 사용기간 설정
	CNtlSobItem* pNtlSobItem = Logic_FindInventoryItemMinDurByDurationGroup(pITEM_TBLDAT->dwDurationGroup);

	m_pTargetNtlSobItem = pNtlSobItem;

	CDboEventGenerator::SetDurExtendTargetItemOfCommercialExtend( pNtlSobItem );

	if(pNtlSobItem)
	{
		pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pNtlSobItem->GetSobAttr() );
		Logic_CaculDayHourMinSecond( (RwUInt32)pItemAttr->GetRemainTime(), &TimeRes );

		if( TimeRes.uiDay > 0 )
		{
			swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s %d%s", TimeRes.uiDay, GetDisplayStringManager()->GetString( "DST_DAY" ),
				TimeRes.uiHour, GetDisplayStringManager()->GetString( "DST_TIME" ) );
		}
		else
		{
			swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s", TimeRes.uiHour, GetDisplayStringManager()->GetString( "DST_TIME" ) );
		}
		text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

		m_AttributeControlSet.pStatic_stbRemainTime->SetText( text );
	}

	/// 추가 사용기간 설정
	Logic_CaculDayHourMinSecond( pITEM_TBLDAT->dwUseDurationMax, &TimeRes );

	if( TimeRes.uiDay > 0 )
	{
		swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s %d%s", TimeRes.uiDay, GetDisplayStringManager()->GetString( "DST_DAY" ),
			TimeRes.uiHour, GetDisplayStringManager()->GetString( "DST_TIME" ) );
	}
	else
	{
		swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s", TimeRes.uiHour, GetDisplayStringManager()->GetString( "DST_TIME" ) );
	}
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;
	m_AttributeControlSet.pStatic_stbAddableTime->SetText( text );

}

void CCommercialExtendZennyMode::Init()
{
	InitShowComponent();

	m_AttributeControlSet.pStatic_stbTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_TITLE" ) );
	m_AttributeControlSet.pStatic_stbConsumeGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_JENNY" ) );
	m_AttributeControlSet.pStatic_stbMyGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_JENNY" ) );
	m_AttributeControlSet.pStatic_stbDescript1->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_EXTEND_Q" ) );
	m_AttributeControlSet.pStatic_stbDescript2->SetText(" ");
}

void CCommercialExtendZennyMode::SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT)
{
	CCommercialExtendMode::SetCurrItemInfo( pITEM_TBLDAT );

	/// 소모 제니
	WCHAR text[GUI_TEXT_BUFFER_SIZE];
	swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%s%s", Logic_FormatZeni(pITEM_TBLDAT->dwCost), GetDisplayStringManager()->GetString( "DST_ZENNY" ) );
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	m_AttributeControlSet.pStatic_stbConsumeGold->SetText( text );

	/// 보유 제니
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );

	swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%s%s", Logic_FormatZeni((DWORD)pAvatarAttr->m_uiZenny), GetDisplayStringManager()->GetString( "DST_ZENNY" ) );
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	m_AttributeControlSet.pStatic_stbMyGold->SetText( text );

	/// icon skin 변경
	m_AttributeControlSet.pSurConsumeGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfImageZenny" ) );
	/// icon skin 변경
	m_AttributeControlSet.pSurMyGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfImageZenny" ) );

	m_uiConsumeGold = pITEM_TBLDAT->dwCost;
	m_uiMyGold = pAvatarAttr->m_uiZenny;
}

RwBool CCommercialExtendZennyMode::ConFirm()
{
	/// 기간 연장  m_pTargetNtlSobItem
	if(m_pTargetNtlSobItem)
	{
		if( m_uiMyGold < m_uiConsumeGold )
		{
			GetAlarmManager()->AlarmMessage( "DST_LACK_OF_ZENNY" );
			return FALSE;
		}
		else
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pTargetNtlSobItem->GetSobAttr());

			GetDboGlobal()->GetGamePacketGenerator()->SendDurationRenewReq(m_pTargetNtlSobItem->GetSerialID(), pItemAttr->GetItemTbl()->tblidx);

			return TRUE;
		}
	}

	return FALSE;
}

void CCommercialExtendZennyMode::Cancel()
{
	m_pTargetNtlSobItem = NULL;
}

void CCommercialExtendZennyMode::CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet)
{
	CCommercialExtendMode::CreateDefaultSetting(pAttributeControlSet);

	m_AttributeControlSet.eSizeMode = SIZE_DETAIL_DESC_DELETED;
	m_AttributeControlSet.eArrangeMode = DEFAULT_BUTTONS;
	m_AttributeControlSet.iDefaultWndHeight = m_AttributeControlSet.pDialog->GetHeight() - 
		m_AttributeControlSet.pInput_opbDescriptDetail->GetHeight();
	m_AttributeControlSet.iDefaultDescript1Top = m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top - 
		m_AttributeControlSet.pInput_opbDescriptDetail->GetHeight();
	m_AttributeControlSet.iDefaultCancelButtLeft = m_AttributeControlSet.pButt_btnCancel->GetPosition().left;
}

void CCommercialExtendNetphyMode::Init()
{
	InitShowComponent();

	m_AttributeControlSet.pStatic_stbTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_TITLE" ) );
	m_AttributeControlSet.pStatic_stbConsumeGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_NETPHY" ) );
	m_AttributeControlSet.pStatic_stbMyGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_NETPHY" ) );
	m_AttributeControlSet.pStatic_stbDescript1->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_EXTEND_Q" ) );
	m_AttributeControlSet.pStatic_stbDescript2->SetText(" ");
}

void CCommercialExtendNetphyMode::SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT)
{
	CCommercialExtendMode::SetCurrItemInfo( pITEM_TBLDAT );

	/// 소모 넷피
	WCHAR text[GUI_TEXT_BUFFER_SIZE];
	swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%s%s", Logic_FormatZeni(pITEM_TBLDAT->CommonPoint), GetDisplayStringManager()->GetString( "DST_NETPY" ) );
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	m_AttributeControlSet.pStatic_stbConsumeGold->SetText( text );

	/// 보유 넷피
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );

	swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%s%s", Logic_FormatZeni(pAvatarAttr->m_dwNetPy), GetDisplayStringManager()->GetString( "DST_NETPY" ) );
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	m_AttributeControlSet.pStatic_stbMyGold->SetText( text );

	/// icon skin 변경
	m_AttributeControlSet.pSurConsumeGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfPannelNetpy" ) );
	/// icon skin 변경
	m_AttributeControlSet.pSurMyGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfPannelNetpy" ) );

	m_uiConsumeGold = pITEM_TBLDAT->CommonPoint;
	m_uiMyGold = pAvatarAttr->m_dwNetPy;

}

RwBool CCommercialExtendNetphyMode::ConFirm()
{
	/// 기간 연장  m_pTargetNtlSobItem
	if(m_pTargetNtlSobItem)
	{
		if( m_uiMyGold < m_uiConsumeGold )
		{
			GetAlarmManager()->AlarmMessage( "DST_LACK_OF_NETPY" );
			return FALSE;
		}
		else
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pTargetNtlSobItem->GetSobAttr());

			GetDboGlobal()->GetGamePacketGenerator()->SendDurationRenewReq(m_pTargetNtlSobItem->GetSerialID(), pItemAttr->GetItemTbl()->tblidx);

			return TRUE;
		}
	}

	return FALSE;
}

void CCommercialExtendNetphyMode::Cancel()
{
	m_pTargetNtlSobItem = NULL;
}

void CCommercialExtendNetphyMode::CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet)
{
	CCommercialExtendMode::CreateDefaultSetting(pAttributeControlSet);

	m_AttributeControlSet.eSizeMode = SIZE_DETAIL_DESC_DELETED;
	m_AttributeControlSet.eArrangeMode = DEFAULT_BUTTONS;
	m_AttributeControlSet.iDefaultWndHeight = m_AttributeControlSet.pDialog->GetHeight() - 
		m_AttributeControlSet.pInput_opbDescriptDetail->GetHeight();
	m_AttributeControlSet.iDefaultDescript1Top = m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top - 
		m_AttributeControlSet.pInput_opbDescriptDetail->GetHeight();
	m_AttributeControlSet.iDefaultCancelButtLeft = m_AttributeControlSet.pButt_btnCancel->GetPosition().left;
}

void CCommercialExtendCashMode::Init()
{
	InitShowComponent();

	m_AttributeControlSet.pStatic_stbTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_TITLE" ) );
	m_AttributeControlSet.pStatic_stbConsumeGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_CASH" ) );
	m_AttributeControlSet.pStatic_stbMyGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_CASH" ) );

	const WCHAR* pStr = GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_EXTEND_Q ");
	/// Consumption
	WCHAR text[41];
	swprintf_s( text, 41, L"%s", pStr );
	m_AttributeControlSet.pStatic_stbDescript1->SetText( text );

	m_AttributeControlSet.pInput_opbDescriptDetail->Clear();
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC1" ) );
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC2" ) );
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC3" ) );
}

void CCommercialExtendCashMode::SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT)
{
	CCommercialExtendMode::SetCurrItemInfo( pITEM_TBLDAT );
	/// Cash는 아직 미완성	///미구현

	/// 소모 Cash
	WCHAR text[GUI_TEXT_BUFFER_SIZE];
	swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s", pITEM_TBLDAT->CommonPoint, GetDisplayStringManager()->GetString( "DST_CASH" ) );
	text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	m_AttributeControlSet.pStatic_stbConsumeGold->SetText( text );

	/// 보유 넷피
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pSobAvatar->GetSobAttr() );
	/// Cash는 아직 미완성
	//swprintf_s( text, GUI_TEXT_BUFFER_SIZE, L"%d%s", pAvatarAttr->m_dwNetPy, GetDisplayStringManager()->GetString( DST_CASH ) );
	//text[GUI_TEXT_BUFFER_SIZE - 1] = NULL;

	//m_AttributeControlSet.pStatic_stbMyGold->SetText( text );
	m_AttributeControlSet.pStatic_stbMyGold->SetText( " " );

	/// icon skin 변경
	m_AttributeControlSet.pSurConsumeGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfPannelCash" ) );
	/// icon skin 변경
	m_AttributeControlSet.pSurMyGoldIcon->SetSurface( 
		GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfPannelCash" ) );

}

RwBool CCommercialExtendCashMode::ConFirm()
{	/// no op
	return FALSE;
}

void CCommercialExtendCashMode::Cancel()
{
}

void CCommercialExtendCashMode::Buy()
{
	/// CCommercialExtendCashBuyMode로
	CDboEventGenerator::ChangeCommercialExtendmode( CCommercialExtendGui::CASH_BUY );
}

void CCommercialExtendCashMode::Refill()
{
	/// 캐쉬충전창............  web연동	///미구현

}

void CCommercialExtendCashMode::CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet)
{
	CCommercialExtendMode::CreateDefaultSetting(pAttributeControlSet);

	m_AttributeControlSet.eSizeMode = SIZE_DEFAULT;
	m_AttributeControlSet.eArrangeMode = CASH_EXTEND_BUTTONS;
	m_AttributeControlSet.iDefaultWndHeight = m_AttributeControlSet.pDialog->GetHeight();
	m_AttributeControlSet.iDefaultDescript1Top = m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top;

	int iDiff = m_AttributeControlSet.pButt_btnRefillCash->GetPosition().left - 
		m_AttributeControlSet.pButt_btnBuyDuration->GetPosition().right;

	m_AttributeControlSet.iDefaultCancelButtLeft = m_AttributeControlSet.pButt_btnRefillCash->GetPosition().right
		+ iDiff;
}

void CCommercialExtendCashBuyMode::Init()
{
	InitShowComponent();

	m_AttributeControlSet.pStatic_stbTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_BUY_TITLE" ) );
	m_AttributeControlSet.pStatic_stbConsumeGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_CASH" ) );
	m_AttributeControlSet.pStatic_stbMyGoldTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_CASH" ) );

	m_AttributeControlSet.pStatic_stbDescript1->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_BUY_Q1" ) );
	m_AttributeControlSet.pStatic_stbDescript2->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_BUY_Q2" ) );

	m_AttributeControlSet.pInput_opbDescriptDetail->Clear();
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC1" ) );
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC2" ) );
	m_AttributeControlSet.pInput_opbDescriptDetail->AddText( 
		GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC3" ) );
}

void CCommercialExtendCashBuyMode::SetCurrItemInfo(sITEM_TBLDAT* pITEM_TBLDAT)
{
	CCommercialExtendMode::SetCurrItemInfo( pITEM_TBLDAT );

	/// Cash는 아직 미완성	///미구현
}

RwBool CCommercialExtendCashBuyMode::ConFirm()
{
	/// 기간 연장  m_pTargetNtlSobItem
	if(m_pTargetNtlSobItem)
	{
		if( m_uiMyGold < m_uiConsumeGold )
		{
			GetAlarmManager()->AlarmMessage( "DST_LACK_OF_CASH" );
			return FALSE;
		}
		else
		{
			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(m_pTargetNtlSobItem->GetSobAttr());

			GetDboGlobal()->GetGamePacketGenerator()->SendDurationRenewReq( m_pTargetNtlSobItem->GetSerialID(), pItemAttr->GetItemTbl()->tblidx);

			return TRUE;
		}
	}

	return FALSE;
}

void CCommercialExtendCashBuyMode::Cancel()
{
	m_pTargetNtlSobItem = NULL;
}

void CCommercialExtendCashBuyMode::CreateDefaultSetting(const CAttributeControlSet* pAttributeControlSet)
{
	CCommercialExtendMode::CreateDefaultSetting(pAttributeControlSet);

	m_AttributeControlSet.eSizeMode = SIZE_DEFAULT;
	m_AttributeControlSet.eArrangeMode = DEFAULT_BUTTONS;
	m_AttributeControlSet.iDefaultWndHeight = m_AttributeControlSet.pDialog->GetHeight();
	m_AttributeControlSet.iDefaultDescript1Top = m_AttributeControlSet.pStatic_stbDescript1->GetPosition().top;
	m_AttributeControlSet.iDefaultCancelButtLeft = m_AttributeControlSet.pButt_btnCancel->GetPosition().left;
}

///////////////////////////////////////// CCommercialExtendGui !!!!!!
CCommercialExtendGui::CCommercialExtendGui( const RwChar* pName )
:CNtlPLGui(pName)
{
	Init();
}

CCommercialExtendGui::~CCommercialExtendGui()
{

}

VOID CCommercialExtendGui::Init(VOID)
{
	m_pCurrITEM_TBLDA = NULL;
	m_pTargetNtlSobItem = NULL;			/// 기간연장을 위한 item의 정보 ref
}

RwBool CCommercialExtendGui::Create(VOID)
{
	NTL_FUNCTION( "CCommercialExtendGui::Create" );
//return FALSE;
	// Load window
	if (!CNtlPLGui::Create("", "gui\\CommercialExtend.srf", "gui\\CommercialExtend.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");

	m_pStatic_stbTitle			= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pPanel_pnlItemButt		= (gui::CPanel*)GetComponent("pnlItemButt");
	m_pStatic_stbRemainTimeTitle= (gui::CStaticBox*)GetComponent("stbRemainTimeTitle");
	m_pStatic_stbAddableTimeTitle= (gui::CStaticBox*)GetComponent("stbAddableTimeTitle");
	m_pStatic_stbConsumeGoldTitle= (gui::CStaticBox*)GetComponent("stbConsumeGoldTitle");
	m_pStatic_stbMyGoldTitle	= (gui::CStaticBox*)GetComponent("stbMyGoldTitle");
	m_pStatic_stbRemainTime		= (gui::CStaticBox*)GetComponent("stbRemainTime");
	m_pStatic_stbAddableTime	= (gui::CStaticBox*)GetComponent("stbAddableTime");
	m_pStatic_stbConsumeGold	= (gui::CStaticBox*)GetComponent("stbConsumeGold");
	m_pPanel_pnlPanelConsumeGoldIcon = (gui::CPanel*)GetComponent("pnlPanelConsumeGoldIcon");
	m_pStatic_stbMyGold			= (gui::CStaticBox*)GetComponent("stbMyGold");
	m_pPanel_pnlPanelMyGoldIcon	= (gui::CPanel*)GetComponent("pnlPanelMyGoldIcon");

	m_pPanel_pnlPanel2			= (gui::CPanel*)GetComponent("pnlPanel2");
	m_pInput_opbDescriptDetail	= (gui::COutputBox*)GetComponent("opbDescriptDetail");

	m_pPanel_pnlPanelName		= (gui::CPanel*)GetComponent("pnlPanelName");
	m_pStatic_stbItemName		= (gui::CStaticBox*)GetComponent("stbItemName");
	m_pStatic_stbDescript1		= (gui::CStaticBox*)GetComponent("stbDescript1");
	m_pStatic_stbDescript2		= (gui::CStaticBox*)GetComponent("stbDescript2");
	m_pButt_btnConfirm			= (gui::CButton*)GetComponent("btnConfirm");
	m_pButt_btnCancel			= (gui::CButton*)GetComponent("btnCancel");
	m_pButt_btnBuyDuration		= (gui::CButton*)GetComponent("btnBuyDuration");
	m_pButt_btnRefillCash		= (gui::CButton*)GetComponent("btnRefillCash");
	m_pButt_btnClose			= (gui::CButton*)GetComponent("btnClose");

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	// Signal
	m_slotGotFocus_pnlItemButt	= m_pPanel_pnlItemButt->SigMouseEnter().Connect(this, &CCommercialExtendGui::OnMouseEnter_pnlItemButt);
	m_slotLostFocus_pnlItemButt	= m_pPanel_pnlItemButt->SigMouseLeave().Connect(this, &CCommercialExtendGui::OnMouseLeave_pnlItemButt);
	m_slotClicked_btnConfirm	= m_pButt_btnConfirm->SigClicked().Connect(this, &CCommercialExtendGui::OnClicked_btnConfirm);
	m_slotClicked_btnCancel		= m_pButt_btnCancel->SigClicked().Connect(this, &CCommercialExtendGui::OnClicked_btnCancel);
	m_slotClicked_btnBuyDuration= m_pButt_btnBuyDuration->SigClicked().Connect(this, &CCommercialExtendGui::OnClicked_btnBuyDuration);
	m_slotClicked_btnRefillCash	= m_pButt_btnRefillCash->SigClicked().Connect(this, &CCommercialExtendGui::OnClicked_btnRefillCash);
	m_slotClicked_btnClose		= m_pButt_btnClose->SigClicked().Connect(this, &CCommercialExtendGui::OnClicked_btnClose);
	m_slotPaint_dlgMain			= m_pThis->SigPaint().Connect( this, &CCommercialExtendGui::OnPaint );
	m_slotMove_dlgMain			= m_pThis->SigMove().Connect( this, &CCommercialExtendGui::OnMove );

	// 
	m_pArrayCommercialExtendMode[ZENNY_EXTEND] = NTL_NEW CCommercialExtendZennyMode;
	m_pArrayCommercialExtendMode[NETPY_EXTEND] = NTL_NEW CCommercialExtendNetphyMode;
	m_pArrayCommercialExtendMode[CASH_EXTEND] = NTL_NEW CCommercialExtendCashMode;
	m_pArrayCommercialExtendMode[CASH_BUY] = NTL_NEW CCommercialExtendCashBuyMode;

	CCommercialExtendMode::CAttributeControlSet AttributeControlSet;
	AttributeControlSet.pDialog = m_pThis;
	AttributeControlSet.pStatic_stbTitle = m_pStatic_stbTitle;
	AttributeControlSet.pStatic_stbConsumeGoldTitle = m_pStatic_stbConsumeGoldTitle;
	AttributeControlSet.pStatic_stbMyGoldTitle = m_pStatic_stbMyGoldTitle;
	AttributeControlSet.pSurConsumeGoldIcon = &m_surConsumeGoldIcon;
	AttributeControlSet.pSurMyGoldIcon = &m_surMyGoldIcon;
	AttributeControlSet.pPanel_pnlPanel2 = m_pPanel_pnlPanel2;
	AttributeControlSet.pInput_opbDescriptDetail = m_pInput_opbDescriptDetail;
	AttributeControlSet.pStatic_stbDescript1 = m_pStatic_stbDescript1;
	AttributeControlSet.pStatic_stbDescript2 = m_pStatic_stbDescript2;
	AttributeControlSet.pButt_btnConfirm = m_pButt_btnConfirm;
	AttributeControlSet.pButt_btnBuyDuration = m_pButt_btnBuyDuration;
	AttributeControlSet.pButt_btnRefillCash = m_pButt_btnRefillCash;
	AttributeControlSet.pButt_btnCancel = m_pButt_btnCancel;
	AttributeControlSet.pStatic_stbRemainTime = m_pStatic_stbRemainTime;
	AttributeControlSet.pStatic_stbAddableTime = m_pStatic_stbAddableTime;
	AttributeControlSet.pStatic_stbConsumeGold = m_pStatic_stbConsumeGold;
	AttributeControlSet.pStatic_stbMyGold = m_pStatic_stbMyGold;

	for(int i = 0; i < NUM_EXTEND_MODE; i++)
	{
		m_pArrayCommercialExtendMode[i]->CreateDefaultSetting(&AttributeControlSet);
	}

	ChangeExtendMode( CASH_EXTEND );

	m_surEnterSlot.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_DialogSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_DialogSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfDialogTop" ) );
	m_DialogSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfDialogBoddy" ) );
	m_DialogSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommercialExtend.srf", "srfDialogBottom" ) );

	m_pButt_btnConfirm->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_OK" ) );
	m_pButt_btnCancel->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_CANCEL" ) );
	m_pButt_btnBuyDuration->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_BUY_DURATION" ) );
	m_pButt_btnRefillCash->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_REFILL_CASH" ) );
	m_pStatic_stbRemainTimeTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_REMIAN_TIME" ) );
	m_pStatic_stbAddableTimeTitle->SetText( GetDisplayStringManager()->GetString( "DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_ADD_TIME" ) );

	UpdateSurfacePos();
	SwitchDialog( false );

	LinkMsg( g_EventCommercialExtendCommand, 0 );
	LinkMsg(g_EventMsgBoxResult, 0);

	//// Update 연결
	//GetNtlGuiManager()->AddUpdateFunc( this );	/// 기간 update

	NTL_RETURN( TRUE );
}

VOID CCommercialExtendGui::Destroy(VOID)
{
	NTL_FUNCTION( "CCommercialExtendGui::Destroy" );
//return;
	for(int i = 0; i < NUM_EXTEND_MODE; i++)
		NTL_DELETE( m_pArrayCommercialExtendMode[i] );

	//GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg( g_EventCommercialExtendCommand );
	UnLinkMsg( g_EventMsgBoxResult );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CCommercialExtendGui::UpdateSurfacePos()
{
	CRectangle rtScreen = m_pPanel_pnlItemButt->GetScreenRect();
	m_surEnterSlot.SetRect( rtScreen.left, rtScreen.top, rtScreen.right, rtScreen.bottom );
	m_surItemIconSlot.SetRect( rtScreen.left, rtScreen.top, rtScreen.right, rtScreen.bottom );

	rtScreen = m_pPanel_pnlPanelConsumeGoldIcon->GetScreenRect();
	m_surConsumeGoldIcon.SetRect( rtScreen.left, rtScreen.top, rtScreen.right, rtScreen.bottom );

	rtScreen = m_pPanel_pnlPanelMyGoldIcon->GetScreenRect();
	m_surMyGoldIcon.SetRect( rtScreen.left, rtScreen.top, rtScreen.right, rtScreen.bottom );

	rtScreen = m_pThis->GetScreenRect();
	m_DialogSurface.SetRect( rtScreen );
}

RwInt32 CCommercialExtendGui::SwitchDialog(bool bOpen)
{
	if(bOpen)
		UpdateSurfacePos();

	m_surEnterSlot.Show( FALSE );

	if (bOpen == false && m_pTargetNtlSobItem != NULL)
	{
		CDboEventGenerator::EnableItemIcon(TRUE, PLACE_BAG, m_pTargetNtlSobItem->GetItemSlotIdx(), m_pTargetNtlSobItem->GetParentItemSlotIdx());

		m_pTargetNtlSobItem = NULL;
	}

	Show(bOpen);

	return 1;
}

VOID CCommercialExtendGui::OpenDialog()
{
	SwitchDialog( true );
}

VOID CCommercialExtendGui::OpenDialog(GUI_EXTEND_MODE eMode, sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiItemIdx)
{
	if( NUM_EXTEND_MODE == eMode)		/// open시엔 내부 계산 mode
	{
		if( NULL == pITEM_TBLDAT )
		{
			if( INVALID_TBLIDX == uiItemIdx )		/// 미리 setting된 item
				uiItemIdx = m_uiDurItemIdx;

			pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(uiItemIdx);
		}

		if( NULL == pITEM_TBLDAT )
			return;

		// 기간제 검사
		if( FALSE == Logic_IsTimeLimitItem( pITEM_TBLDAT ) )
			return;

		eMode = (CCommercialExtendGui::GUI_EXTEND_MODE) Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( 
			pITEM_TBLDAT->byCommonPointType );
	}

	ChangeExtendMode(eMode);

	if( pITEM_TBLDAT )
	{
		SetCurrItemTbl( pITEM_TBLDAT );
		m_uiDurItemIdx = pITEM_TBLDAT->tblidx;
	}
	else
	{
		if( INVALID_TBLIDX == uiItemIdx )
		{
			SetCurrItemTbl( m_uiDurItemIdx );	/// 미리 setting된 item
		}
		else
		{
			SetCurrItemTbl( uiItemIdx );
			m_uiDurItemIdx = uiItemIdx;
		}
	}

	OpenDialog();
}

sITEM_TBLDAT* CCommercialExtendGui::StartBuyDurItemProcessCommonForItem( RwUInt32 uiItemIdx )
{
	/// 기간제 item 구매 시작 event를 받으면 실행....  entry point
	// Work Flow
	//
	//	MBW_BUY_DUR_ITEM --->  MBW_DUR_ITEM_EXTEND			--------->    CCommercialExtendGui
	//				|	  |->  MBW_DUR_ITEM_BAG_EXTEND      ----|
	//			    |					 |
	//				|------------------------->   MBW_BUY_DUR_ITEM_CONFIRM
	// 
	//  GetDialogManager()->OpenDialog( DIALOG_DURATION_EXTEND_CONFIRM );
	//
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(uiItemIdx);

	if( NULL == pITEM_TBLDAT ||  FALSE == Logic_IsTimeLimitItem( pITEM_TBLDAT ) )
		return NULL;

	/// change
	ChangeExtendMode( (CCommercialExtendGui::GUI_EXTEND_MODE) Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( 
		pITEM_TBLDAT->byCommonPointType ));
	/// item set
	SetCurrItemTbl( pITEM_TBLDAT );
	
	return pITEM_TBLDAT;
}

VOID CCommercialExtendGui::StartBuyDurItemProcessNPC( RwUInt32 uiItemIdx,  RwUInt32 uiNPCSerial, sSHOP_BUY_CART& ShopBuyCart )
{
	sITEM_TBLDAT* pITEM_TBLDAT = StartBuyDurItemProcessCommonForItem( uiItemIdx );

	if( pITEM_TBLDAT )
	{	/// Start MessageBox 시작		MBW_BUY_DUR_ITEM
		if (CCommercialExtendGui::ZENNY_EXTEND == m_eCurrExtendGuiMode)
		{
			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			GetAlarmManager()->FormattedAlarmMessage("DST_COMMERCIAL_MB_DIRECT_BUY_MSG", FALSE, NULL, pItemTextTable->GetText(pITEM_TBLDAT->Name).c_str(), pITEM_TBLDAT->dwCost);
		}

		m_NetSendData.DataSendBuyDurItemNPC.m_uiNPCSerial = uiNPCSerial;
		m_NetSendData.DataSendBuyDurItemNPC.m_ShopBuyCart = ShopBuyCart;
	}
}

VOID CCommercialExtendGui::StartBuyDurItemProcessNetpy( RwUInt32 uiItemIdx, RwUInt32 uiMerchantTblidx, BYTE byMerchanSlotPos )
{
	sITEM_TBLDAT* pITEM_TBLDAT = StartBuyDurItemProcessCommonForItem( uiItemIdx );

	if( pITEM_TBLDAT )
	{	/// Start MessageBox 시작		MBW_BUY_DUR_ITEM
		if (CCommercialExtendGui::NETPY_EXTEND == m_eCurrExtendGuiMode)
		{
			CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

			GetAlarmManager()->FormattedAlarmMessage("DST_COMMERCIAL_MB_DIRECT_BUY_NETPY_MSG", FALSE, NULL, pItemTextTable->GetText(pITEM_TBLDAT->Name).c_str(), pITEM_TBLDAT->CommonPoint);
		}

		m_NetSendData.DataSendBuyDurItemNetpy.m_uiMerchantTblidx = uiMerchantTblidx;
		m_NetSendData.DataSendBuyDurItemNetpy.m_byMerchanSlotPos = byMerchanSlotPos;
	}
}

VOID CCommercialExtendGui::StartMoveDurItemProcessCash( RwUInt32 uiItemIdx, RwUInt32 uiProductId )
{
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(uiItemIdx);

	if( NULL == pITEM_TBLDAT ||  FALSE == Logic_IsTimeLimitItem( pITEM_TBLDAT ) )
		return;

	m_NetSendData.DataSendMoveDurItemFromYardrat.m_uiProductId = uiProductId;

	/// change
	ChangeExtendMode( (CCommercialExtendGui::GUI_EXTEND_MODE) Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( 
		pITEM_TBLDAT->byCommonPointType ));
	/// item set
	SetCurrItemTbl( pITEM_TBLDAT );
	if( CCommercialExtendGui::CASH_EXTEND == m_eCurrExtendGuiMode || CCommercialExtendGui::CASH_BUY == m_eCurrExtendGuiMode )
		GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_TAKE_CONFIRM_MSG");
}

VOID CCommercialExtendGui::NetSendBuyDurItemNPC( )
{	
	sSHOP_BUY_CART* pShopBuyCartarray[NTL_MAX_BUY_SHOPPING_CART];

	memset( pShopBuyCartarray, NULL, sizeof(sSHOP_BUY_CART*)*NTL_MAX_BUY_SHOPPING_CART );
	pShopBuyCartarray[0] = &m_NetSendData.DataSendBuyDurItemNPC.m_ShopBuyCart;

	GetDboGlobal()->GetGamePacketGenerator()->SendNPCShopBuy(m_NetSendData.DataSendBuyDurItemNPC.m_uiNPCSerial, pShopBuyCartarray);
}

VOID CCommercialExtendGui::NetSendBuyDurItemNetpy( )
{
	GetDboGlobal()->GetGamePacketGenerator()->SendDurationItemBuyReq( m_NetSendData.DataSendBuyDurItemNetpy.m_uiMerchantTblidx, 
		m_NetSendData.DataSendBuyDurItemNetpy.m_byMerchanSlotPos );
}

VOID CCommercialExtendGui::NetSendMoveDurItemFromYardrat()
{
}

VOID CCommercialExtendGui::SetDurationItem(RwUInt32 uiItemIdx)
{
	m_uiDurItemIdx = uiItemIdx;
}

VOID CCommercialExtendGui::SetExtendTargetItem(CNtlSobItem* pNtlSobItem)
{
	m_pTargetNtlSobItem = pNtlSobItem;
}

VOID CCommercialExtendGui::SetCurrItemTbl(RwUInt32 uiItemIdx)
{
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(uiItemIdx);	
	//Logic_GetItemDataFromTable	
	SetCurrItemTbl( pITEM_TBLDAT );
}

VOID CCommercialExtendGui::SetCurrItemTbl(sITEM_TBLDAT* pITEM_TBLDAT)
{
	m_pCurrITEM_TBLDA = pITEM_TBLDAT;

	m_surItemIconSlot.Show( FALSE );

	if( pITEM_TBLDAT )	/// icon setting
	{
		RwUInt32 uiColor = Logic_GetItemRankColor( pITEM_TBLDAT->byRank );	

		m_pStatic_stbItemName->SetColor( GetRValue(uiColor), GetGValue(uiColor), GetBValue(uiColor) );

		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

		m_pStatic_stbItemName->SetText(pItemTextTable->GetText(pITEM_TBLDAT->Name).c_str());

		if(  TRUE == Logic_IsTimeLimitItem( pITEM_TBLDAT ) )	/// 기간제일때만 
			m_pArrayCommercialExtendMode[m_eCurrExtendGuiMode]->SetCurrItemInfo( pITEM_TBLDAT );

		SetIcon( pITEM_TBLDAT->szIcon_Name );

		m_surItemIconSlot.Show( TRUE );
	}
}

RwBool CCommercialExtendGui::SetIcon(const RwChar* pcIconName)
{
	if( !pcIconName || strlen(pcIconName) == 0 )
	{
		DBO_FAIL("Icon name is wrong");
		return FALSE;
	}

	m_surItemIconSlot.SetTexture(Logic_CreateTexture((RwChar*)pcIconName));
	//m_pPanel_pnlItemButt.ChangeTexture( 0, pcIconName );

	return TRUE;
}

VOID CCommercialExtendGui::ChangeExtendMode(GUI_EXTEND_MODE eMode)
{
	m_eCurrExtendGuiMode = eMode;
	m_pArrayCommercialExtendMode[m_eCurrExtendGuiMode]->Init();
	UpdateSurfacePos();
}

VOID CCommercialExtendGui::OnMouseEnter_pnlItemButt( gui::CComponent* pComponent )
{
	/// item tooltip show true
	if( m_pCurrITEM_TBLDA )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		CRectangle rect = m_pPanel_pnlItemButt->GetPosition();
		RwInt32 nX = rect.left + rtScreen.left;
		RwInt32 nY = rect.top + rtScreen.top;

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
			nX, nY, m_pCurrITEM_TBLDA, DIALOG_DURATION_EXTEND_CONFIRM );
	}

	m_surEnterSlot.Show( TRUE );
}

VOID CCommercialExtendGui::OnMouseLeave_pnlItemButt( gui::CComponent* pComponent )
{
	/// item tooltip show false	
	GetInfoWndManager()->ShowInfoWindow( FALSE );

	m_surEnterSlot.Show( FALSE );
}

VOID CCommercialExtendGui::OnClicked_btnConfirm(gui::CComponent* pComponent)
{
	if (TRUE == m_pArrayCommercialExtendMode[m_eCurrExtendGuiMode]->ConFirm())
	{
		SwitchDialog(false);
	}
}

VOID CCommercialExtendGui::OnClicked_btnCancel(gui::CComponent* pComponent)
{
	m_pArrayCommercialExtendMode[m_eCurrExtendGuiMode]->Cancel();
	SwitchDialog( false );
}

VOID CCommercialExtendGui::OnClicked_btnBuyDuration(gui::CComponent* pComponent)
{
	if(CASH_EXTEND == m_eCurrExtendGuiMode)
	{
		((CCommercialExtendCashMode*)m_pArrayCommercialExtendMode[CASH_EXTEND])->Buy();
	}
}

VOID CCommercialExtendGui::OnClicked_btnRefillCash(gui::CComponent* pComponent)
{
	if(CASH_EXTEND == m_eCurrExtendGuiMode)
	{
		((CCommercialExtendCashMode*)m_pArrayCommercialExtendMode[CASH_EXTEND])->Refill();
	}
}

VOID CCommercialExtendGui::OnClicked_btnClose(gui::CComponent* pComponent)
{
	OnClicked_btnCancel( NULL );
}

VOID CCommercialExtendGui::OnPaint(VOID)
{
	m_DialogSurface.Render();

	m_surConsumeGoldIcon.Render();				// 텍스처 바뀔 수 있음
	m_surMyGoldIcon.Render();					// 텍스처 바뀔 수 있음

	m_surItemIconSlot.Render();
	m_surEnterSlot.Render();
}

VOID CCommercialExtendGui::OnMove(int nX,int nY)
{
	UpdateSurfacePos();
}


///////////////////////////////////////// HandleEvents  !!!!!!
VOID CCommercialExtendGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventCommercialExtendCommand )
	{
		SDboEventCommercialExtendCommand* pData = reinterpret_cast<SDboEventCommercialExtendCommand*>( msg.pData );

		if( eSTART_PROCESS_BUY_NPC_DURATION_ITEM == pData->eCommandType )
		{
			SDboEventCommercialExtendNPCData* pExData = (SDboEventCommercialExtendNPCData*) pData->pData;

			StartBuyDurItemProcessNPC( pExData->uiItemIdx, pExData->uiNPCSerial, pExData->ShopBuyCart);

		}
		else if( eSTART_PROCESS_BUY_NETPY_DURATION_ITEM == pData->eCommandType )
		{
			SDboEventCommercialExtendNetpyData* pExData = (SDboEventCommercialExtendNetpyData*) pData->pData;

			StartBuyDurItemProcessNetpy( pExData->uiItemIdx, pExData->uiMerchantTblidx,
				pExData->byMerchanSlotPos);
		}
		else if( eSTART_PROCESS_MOVE_DURATION_ITEM == pData->eCommandType )
		{
			SDboEventCommercialExtendYardratData* pExData = (SDboEventCommercialExtendYardratData*) pData->pData;

			StartMoveDurItemProcessCash( pExData->uiItemIdx, pExData->uiProductId );

		}
		else if( eSET_DURATION_ITEM == pData->eCommandType )
		{
			RwUInt32* pExData = (RwUInt32*) pData->pData;

			SetDurationItem( *pExData );
		}
		else if( eSET_DURATION_EXTEND_TARGET_ITEM == pData->eCommandType )
		{
			SetExtendTargetItem( (CNtlSobItem*) pData->pData );
		}
		else if( eCHANGE_COMMERCIALEXTEND_MODE == pData->eCommandType )
		{
			RwInt32* pExData = (RwInt32*) pData->pData;

			ChangeExtendMode( (CCommercialExtendGui::GUI_EXTEND_MODE)*pExData );
		}
		else if( eOPEN_COMMERCIALEXTEND_DIALOG == pData->eCommandType )
		{
			SDboEventCommercialExtendOpenData* pExData = (SDboEventCommercialExtendOpenData*) pData->pData;
			GUI_EXTEND_MODE eMode;

			if( -1 == pExData->iMode )
				eMode = NUM_EXTEND_MODE;
			else
				eMode = (CCommercialExtendGui::GUI_EXTEND_MODE)pExData->iMode;

			OpenDialog( eMode, NULL, pExData->uiItemIdx );
		}
	}
	else if( msg.Id == g_EventMsgBoxResult )
		HandleEventsSubMsgBox( msg );

}

VOID CCommercialExtendGui::HandleEventsSubMsgBox( RWS::CMsg& msg )
{
	SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

	if (pEvent->strID == "DST_COMMERCIAL_MB_DIRECT_BUY_MSG")
	{
		if (pEvent->eResult == MBR_OK)
		{
			switch( m_eCurrExtendGuiMode )
			{
				case CCommercialExtendGui::ZENNY_EXTEND:	/// Purchase of NPC Stores
				{
					if (m_pTargetNtlSobItem)	    /// 인벤에 같은 Duration_group의 item 존재하는지 검사(StartBuyDurItemProcess서 완료)
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pTargetNtlSobItem->GetSobAttr() );

						if( NULL == pItemAttr )
							break;

						if( pItemAttr && pItemAttr->GetItemTbl() )
						{
							if( ITEM_TYPE_BAG == pItemAttr->GetItemTbl()->byItem_Type )
							{
								GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION");
							}
							else
							{
								NetSendBuyDurItemNPC(); // disable this once "DST_COMMERCIAL_MB_ITEM_BUY_MSG" works fine
							//	GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_MSG");
							}
						}
					}
					else
					{
						NetSendBuyDurItemNPC();
					//	GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG");
					}
				}
				break;
			}
		}
	}
	else if (pEvent->strID == "DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION")
	{
		OpenDialog();
	}
	/*else if (pEvent->strID == "DST_COMMERCIAL_MB_ITEM_BUY_MSG")
	{
		if (pEvent->eResult == MBR_OK)
			OpenDialog();
	}*/
	//else if (pEvent->strID == "DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG")
	//{
	//	if (pEvent->eResult == MBR_OK)
	//	{
	//		switch (m_eCurrExtendGuiMode)
	//		{
	//			case CCommercialExtendGui::ZENNY_EXTEND:	/// buy from npc
	//			{
	//				NetSendBuyDurItemNPC();
	//			}
	//			break;
	//			/*case CCommercialExtendGui::NETPY_EXTEND:	/// buy netpy
	//				NetSendBuyDurItemNetpy();
	//				break;
	//			case CCommercialExtendGui::CASH_EXTEND:		/// buy cash shop
	//			case CCommercialExtendGui::CASH_BUY:
	//				NetSendMoveDurItemFromYardrat();
	//				break;*/
	//		}
	//	}
	//}

	else if (pEvent->strID == "DST_COMMERCIAL_MB_USE_TERM_LIMETED_ITEM_MSG")
	{
		sMsgBoxData* data = pEvent->pData;

		CDboEventGenerator::EnableItemIcon(TRUE, data->sItemInfo.ePlace, data->sItemInfo.nSlotIdx, data->sItemInfo.nPlaceIdx);

		if (pEvent->eResult == MBR_OK)
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(data->sItemInfo.hSerial));
			if( pItem )
			{
				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
				if( pItemAttr )
				{
					sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();
					if( pITEM_TBLDAT )
					{
						CDboEventGenerator::OpenCommercialExtendGui( 
							Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( pITEM_TBLDAT->byCommonPointType ), /// OR  INVALID_TBLIDX
							pITEM_TBLDAT->tblidx );
					}
				}
			}
		}
	}
	else if (pEvent->strID == "DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG")
	{
		sMsgBoxData* data = pEvent->pData;

		if (pEvent->eResult == MBR_OK)
		{
			CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(data->sItemInfo.hSerial));
			if (pItem)
			{
				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
				if (pItemAttr)
				{
					sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();
					if (pITEM_TBLDAT)
					{
						CDboEventGenerator::OpenCommercialExtendGui(
							Logic_ItemGetGUI_EXTEND_MODEByCommonPointType(pITEM_TBLDAT->byCommonPointType), /// OR  INVALID_TBLIDX
							pITEM_TBLDAT->tblidx);
					}
				}
			}
		}
	}

	// TO DO

//	switch (pEvent->uiWorkId)
//	{
//	case MBW_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM:	/// inven에서 아이템 연장시 메시지박스 처리
//		{
//			// peessi : 현재는 그냥 아이템을 유효화 하지만, 이후 여기서는 취소할때만 유효화 ok시 연장창 생성, 아이템 연장창을 닫을때 유효화.
//			if( pEvent->eResult == MBR_OK )	/// 20090820 woosung_test
//			{
//				// 유효기간 연장창 열기.
//				CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pEvent->pData->sItemDeleteInfo.hSerial ) );
//				if( pItem )
//				{
//					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
//					if( pItemAttr )
//					{
//						sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();
//						if( pITEM_TBLDAT )
//						{
//							CDboEventGenerator::OpenCommercialExtendGui( 
//								Logic_ItemGetGUI_EXTEND_MODEByCommonPointType( pITEM_TBLDAT->byCommonPointType ), /// OR  INVALID_TBLIDX
//								pITEM_TBLDAT->tblidx );
//						}
//					}
//				}
//			}
//			//else
//			CDboEventGenerator::EnableItemIcon( TRUE, pEvent->pData->sItemDeleteInfo.ePlace, pEvent->pData->sItemDeleteInfo.nSlotIdx, pEvent->pData->sItemDeleteInfo.nPlaceIdx );				
//		}
//		break;	
//
///// NPC, Netpy Shop
//	case MBW_COMMERCIAL_MB_DIRECT_BUY:			// 기간제아이템 구매 시작(엔트리 포인트)  DST_COMMERCIAL_MB_DIRECT_BUY_MSG
//		{
//			if( pEvent->eResult == MBR_OK )		
//			{
//				if( m_pTargetNtlSobItem )	    /// 인벤에 같은 Duration_group의 item 존재하는지 검사(StartBuyDurItemProcess서 완료)
//				{
//					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pTargetNtlSobItem->GetSobAttr() );
//
//					if( NULL == pItemAttr )
//						break;
//					if( pItemAttr && pItemAttr->GetItemTbl() )
//					{
//						if( ITEM_TYPE_BAG == pItemAttr->GetItemTbl()->byItem_Type )	/// bag item인지 검사
//						{
//							GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION");
//						}
//						else
//						{
//							GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_MSG");
//						}
//					}
//				}
//				else	/// 바로 구매 한다(인벤내에 기간제 존재 않으면)
//				{
//					GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG");
//				}
//			}
//		}
//		break;
//	case MBW_COMMERCIAL_MB_ITEM_TERM_EXTENTION:		// 기간제아이템 기간연장 확인 -  : ok  DST_COMMERCIAL_MB_ITEM_BUY_MSG
//		{
//		/// 기간연장 확인창 open		CCommercialExtendGui command event
//			if( pEvent->eResult == MBR_OK )		
//				OpenDialog();
//		}
//		break;
//	case MBW_COMMERCIAL_MB_CAPSULEKIT_TERM_EXTENTION: // Bag type 기간제아이템 기간연장 확인 or 구매: DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION
//		{
//		/// 기간연장 확인창 open		CCommercialExtendGui command event
//			OpenDialog();
//		}
//		break;
//	case MBW_COMMERCIAL_MB_CAPSULEKIT_BUY: // Bag type 기간제아이템 기간연장 확인 or 구매: DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION
//		{
//			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG");
//		}
//		break;
///// 야드레트 창고(Cash)
//	case MBW_COMMERCIAL_MB_DUR_ITEM_GET_FROM_YARDRAT:// 기간제아이템 구매 시작(엔트리 포인트)  DST_COMMERCIAL_MB_ITEM_TAKE_CONFIRM_MSG
//		if( pEvent->eResult == MBR_OK )		
//		{
//			if( m_pTargetNtlSobItem )	    /// 인벤에 같은 Duration_group의 item 존재하는지 검사(StartBuyDurItemProcessCash서 완료)
//			{
//				CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( m_pTargetNtlSobItem->GetSobAttr() );
//
//				if( NULL == pItemAttr )
//					break;
//				if( pItemAttr && pItemAttr->GetItemTbl() )
//				{
//					if( ITEM_TYPE_BAG == pItemAttr->GetItemTbl()->byItem_Type )	/// bag item인지 검사
//					{
//						GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_SELECT");
//					}
//					else
//					{
//						GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM_MSG");
//					}
//				}
//			}
//			else	/// 바로 이동 한다(인벤내에 기간제 존재 않으면)
//			{
//				GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_MOVE_CONFIRM_MSG");
//			}
//		}
//		break;
//	case MBW_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM: // DST_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM_MSG
//		{
//			/// 기간연장 확인창 open		CCommercialExtendGui command event
//			OpenDialog();
//		}
//		break;
//	case MBW_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION:
//		{
//			/// 기간연장 확인창 open		CCommercialExtendGui command event
//			OpenDialog();
//		}
//		break;
//	case MBW_COMMERCIAL_MB_CAPSULEKIT_MOVE:
//		{
//			GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_MB_ITEM_MOVE_CONFIRM_MSG");
//		}
//		break;
///// 구입 재확인
//	case MBW_COMMERCIAL_MB_ITEM_BUY_CONFIRM:	// 기간제아이템 구매 재확인 DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG, 
//		{
//			/// 기간제아이템 구매			CCommercialExtendGui command event
//			if( pEvent->eResult == MBR_OK )	
//			{
//				switch( m_eCurrExtendGuiMode )
//				{
//				case CCommercialExtendGui::ZENNY_EXTEND:	/// NPC상점서 기간제 구입
//					NetSendBuyDurItemNPC();
//					break;
//				case CCommercialExtendGui::NETPY_EXTEND:	/// Netpy상점서 기간제 구입
//					NetSendBuyDurItemNetpy();
//					break;
//				case CCommercialExtendGui::CASH_EXTEND:		/// 야드래트로부터 이동
//				case CCommercialExtendGui::CASH_BUY:
//					NetSendMoveDurItemFromYardrat();
//					break;
//				}
//			}
//		}
//		break;
//	}
}
///////////////////////////////////////// Test Code