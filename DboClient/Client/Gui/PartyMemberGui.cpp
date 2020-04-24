#include "precomp_dboclient.h"
#include "PartyMemberGui.h"


// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobBuff.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlSLPacketGenerator.h"

// dbo
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DboPacketGenerator.h"
#include "DboLogic.h"
#include "BuffDispObject.h"
#include "DisplayStringManager.h"
#include "CursorManager.h"

namespace
{
	#define dBUFF_X_OFFSET		5
	#define dBUFF_Y_OFFSET		2

	#define dBUFF_GAP			1

	#define dFLAG_NONE			0x00
	#define dFLAG_OTHER_WORLD	0x01
	#define dFLAG_LEADER		0x02
	#define dFLAG_DEATH			0x04
};


//////////////////////////////////////////////////////////////////////////
//	CPartyMemberBuffGui
//////////////////////////////////////////////////////////////////////////


CPartyMemberBuffGui::CPartyMemberBuffGui()
:m_pBuff(NULL)
{
}

CPartyMemberBuffGui::~CPartyMemberBuffGui()
{
}

RwBool CPartyMemberBuffGui::Create(SERIAL_HANDLE hHandle)
{	
	NTL_FUNCTION( "CPartyMemberBuffGui::Create" );

	if(!CNtlPLGui::Create("", "", ""))
		NTL_RETURN(FALSE);

	m_hOwnerHandle = hHandle;

	m_pBuff = NTL_NEW CBuffDispGui;

	BYTE byShowFlag = MAKE_BIT_FLAG(BUFF_TYPE_BLESS) + MAKE_BIT_FLAG(BUFF_TYPE_CURSE);
	m_pBuff->Create(byShowFlag, CBuffDispGui::SMALL_ICONSIZE,
					dBUFF_X_OFFSET, dBUFF_Y_OFFSET, DIALOG_PARTYMENU, CBuffDispGui::FAKEBUFF);
	m_pBuff->Show(true);	

	LinkMsg(g_EventSobInfoUpdate);

	NTL_RETURN(TRUE);
}

VOID CPartyMemberBuffGui::Destroy()
{
	NTL_FUNCTION("CPartyMemberBuffGui::Destroy");

	if( m_pBuff )
	{
		m_pBuff->Destroy();
		NTL_DELETE( m_pBuff );
	}

	UnLinkMsg(g_EventSobInfoUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPartyMemberBuffGui::Update(RwReal fElapsed)
{
	m_pBuff->Update( fElapsed );
}

VOID CPartyMemberBuffGui::Show(bool bShow)
{
	m_pBuff->Show(bShow);
}

VOID CPartyMemberBuffGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pBuff->SetMove(CRectangle(iXPos, iYPos, iXPos, iYPos));
}

VOID CPartyMemberBuffGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( GetIconMoveManager()->IsActive() )
		return;

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyMemberBuffGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyMemberBuffGui::OnPaint()
{
}

void CPartyMemberBuffGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CPartyMemberBuffGui::HandleEvents");

	if(pMsg.Id == g_EventSobInfoUpdate)
	{
		if( Logic_IsUpdateType(EVENT_AIUT_BUFF, pMsg.pData) == FALSE )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( pMsg.pData );
		if( pEvent->hSerialId != m_hOwnerHandle )
			NTL_RETURNVOID();


		CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(m_hOwnerHandle) );
		NTL_ASSERT(pMember, "CPartyMemberBuffGui::HandleEvents, Not exist party member of handle : " << m_hOwnerHandle);
		m_pBuff->SetBuffAll( pMember->pFakeBuffContainer );
	}

	NTL_RETURNVOID();
}


//////////////////////////////////////////////////////////////////////////
//	CPartyMemberGui
//////////////////////////////////////////////////////////////////////////

CPartyMemberGui::CPartyMemberGui()
:m_hSerial(INVALID_SERIAL_ID)
,m_byMouse(INVALID_BYTE)
,m_byDisplayFlag(dFLAG_NONE)
,m_pBuff(NULL)
{
}


CPartyMemberGui::~CPartyMemberGui()
{
}

RwBool CPartyMemberGui::Create(SERIAL_HANDLE hSerial, WCHAR* pcText, RwUInt8 byClass)
{
	NTL_FUNCTION( "CPartyMemberGui::Create" );

	if(!CNtlPLGui::Create("gui\\PartyMember.rsr", "gui\\PartyMember.srf", "gui\\PartyMember.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_PARTY_MENU);

	CRectangle rect;

	// 종족과 직업 그림
	m_pTributarySimbol = (gui::CPanel*)GetComponent( "TributaryPanel" );

	// 파티 맴버 스태틱
	rect.SetRectWH(19, 4, 104, 20);
	m_pNameStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pNameStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pNameStatic->SetEffectMode(TE_SHADOW);
	m_pNameStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pNameStatic->SetText(pcText);
	m_pNameStatic->Enable(false);

	// LP 게이지
	m_pLPGauge = (gui::CProgressBar*)GetComponent( "LPGauge" );	
	m_pLPGauge->Enable(false);	
/*
	// LP 스태틱
	rect.SetRectWH(32, 25, 42, 13);
	m_pLPStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pLPStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLPStatic->SetEffectMode(TE_SHADOW);
	m_pLPStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pLPStatic->Enable(false);

	// 슬래쉬 스태틱
	rect.SetRectWH(74, 25, 4, 13);
	m_pLPSlachStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pLPSlachStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLPSlachStatic->SetEffectMode(TE_SHADOW);
	m_pLPSlachStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pLPSlachStatic->SetText("/");
	m_pLPSlachStatic->Enable(false);

	// MaxLP 스태틱
	rect.SetRectWH(78, 25, 42, 13);
	m_pMaxLPStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pMaxLPStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMaxLPStatic->SetEffectMode(TE_SHADOW);
	m_pMaxLPStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pMaxLPStatic->Enable(false);
*/
	// EP 게이지
	m_pEPGauge = (gui::CProgressBar*)GetComponent( "EPGauge" );	
	m_pEPGauge->Enable(false);

/*
	// EP 스태틱
	rect.SetRectWH(32, 34, 42, 13);
	m_pEPStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT);
	m_pEPStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pEPStatic->SetEffectMode(TE_SHADOW);
	m_pEPStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pEPStatic->Enable(false);

	// 슬래쉬 스태틱
	rect.SetRectWH(74, 34, 4, 13);
	m_pEPSlachStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pEPSlachStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pEPSlachStatic->SetEffectMode(TE_SHADOW);
	m_pEPSlachStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pEPSlachStatic->SetText("/");
	m_pEPSlachStatic->Enable(false);

	// MaxLP 스태틱
	rect.SetRectWH(78, 34, 42, 13);
	m_pMaxEPStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pMaxEPStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pMaxEPStatic->SetEffectMode(TE_SHADOW);
	m_pMaxEPStatic->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pMaxEPStatic->Enable(false);
*/
	// 레벨
	rect.SetRectWH(5, 28, 22, 13);
	m_pLevel = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pLevel->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLevel->SetEffectMode(TE_SHADOW);
	m_pLevel->SetEffectValue(DEFAULT_SHADOW_EFFECT_VALUE);
	m_pLevel->Clear();
	m_pLevel->Enable(false);

	// 다른 월드에 있는 파티원의 패널
	m_srfOtherWorldPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfAbsentee" ) );	

	// 사망시 패널
	m_srfDeathPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfDeathPanel" ) );	

	// 리더 마크
	m_srfLeaderMark.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfPartyLeaderMark_for_MemberGui" ) );
	m_srfLeaderMark.SetPositionfromParent(12, 12);

	// 사망 마크
	m_srfDeathMark.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfDeathMark_for_MemberGui" ) );
	m_srfDeathMark.SetPositionfromParent(12, 12);

	// 멀리 떨어진 파티원
	m_srfTooFarMark.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfTooFarMark_for_MemberGui" ) );
	m_srfTooFarMark.SetPositionfromParent(12, 12);

	// LP 경고 표시등
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyMember.srf", "srfWarning" );
	m_feEmergency.SetSurface( surface );
	m_feEmergency.SetTime( 1000.0f, 0.5f );
	m_feEmergency.SetAlpha( 0, 255 );
	m_feEmergency.SetSize( surface.m_Original.rtRect.GetWidth(), surface.m_Original.rtRect.GetWidth() + 5,
						   surface.m_Original.rtRect.GetHeight(), surface.m_Original.rtRect.GetHeight() + 5 );

	
	// 종족과 직업 그림	
	SetClass(byClass);

	// 버프 윈도우
	m_pBuff = NTL_NEW CPartyMemberBuffGui;
	m_pBuff->Create(hSerial);

	// 맴버 핸들 저장
	m_hSerial = hSerial;

	// sig	
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CPartyMemberGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CPartyMemberGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CPartyMemberGui::OnMove );
	m_slotMouseEnter	= m_pThis->SigMouseEnter().Connect( this, &CPartyMemberGui::OnMouseEnter );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CPartyMemberGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CPartyMemberGui::OnMouseLeave );
	m_slotPostPaint		= m_pLevel->SigPaint().Connect( this, &CPartyMemberGui::OnPostPaint );

	Show(true);

	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventChangeCharName);

	GetNtlGuiManager()->AddUpdateFunc(this);

	NTL_RETURN(TRUE);
}

VOID CPartyMemberGui::Destroy()
{
	NTL_FUNCTION("CPartyMemberGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_JUST_WTEXT ||
		GetInfoWndManager()->GetRequestGui() == DIALOG_PARTYMENU )
		GetInfoWndManager()->ShowInfoWindow( FALSE );

	if(m_pBuff)
	{
		m_pBuff->Destroy();
		NTL_DELETE(m_pBuff);
	}

	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventChangeCharName);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPartyMemberGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pBuff->Update(fElapsed);
	m_feEmergency.Update(fElapsed);
}

SERIAL_HANDLE CPartyMemberGui::GetHandle()
{
	return m_hSerial;
}

VOID CPartyMemberGui::SetLP(RwInt32 iValue, RwInt32 iMaxValue)
{
	if( (RwReal)iValue / (RwReal)iMaxValue < dLPWARNING_RATE &&
		iValue > 0 )
	{
		if( !m_feEmergency.IsWork() )
			m_feEmergency.StartProc( TRUE );	
	}
	else
	{
		if( m_feEmergency.IsWork() )
			m_feEmergency.EndProc();		
	}

	m_pLPGauge->SetRange(0, iMaxValue);
	m_pLPGauge->SetPos(iValue);

	//m_pLPStatic->SetText(iValue);
	//m_pMaxLPStatic->SetText(iMaxValue);

	if( iValue == 0 )
		BIT_FLAG_SET(m_byDisplayFlag, dFLAG_DEATH);
	else
	{
		BIT_FLAG_UNSET(m_byDisplayFlag, dFLAG_DEATH);

		if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_JUST_WTEXT ||
			GetInfoWndManager()->GetRequestGui() == DIALOG_PARTYMENU )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CPartyMemberGui::SetEP(RwInt32 iValue, RwInt32 iMaxValue)
{
	m_pEPGauge->SetRange(0, iMaxValue);
	m_pEPGauge->SetPos(iValue);

	//m_pEPStatic->SetText(iValue);
	//m_pMaxEPStatic->SetText(iMaxValue);
}

VOID CPartyMemberGui::SetLevel(RwInt8 byLevel)
{
	m_pLevel->SetText(byLevel);
}

VOID CPartyMemberGui::SetClass(RwUInt8 byClass)
{	
	std::list<gui::CSurface>* pSurfaceList = m_pTributarySimbol->GetSurface();
	CRectangle rtRect = m_pTributarySimbol->GetPosition();
	gui::CSurface& rClassIconSurface = Logic_GetPCClassIconSurface(byClass, FALSE);

	m_byClass = byClass;

	pSurfaceList->clear();

	m_pTributarySimbol->AddSurface(rClassIconSurface);
	m_pTributarySimbol->SetPosition(rtRect.left, rtRect.top);
	m_pTributarySimbol->SetToolTip(Logic_GetClassName(m_byClass));
}

VOID CPartyMemberGui::SetLeader(RwBool bLeader)
{
	if(bLeader)
	{
		m_pNameStatic->SetTextColor(RGB(255, 219, 71), true);
		BIT_FLAG_SET(m_byDisplayFlag, dFLAG_LEADER);
	}
	else
	{
		m_pNameStatic->SetTextColor(RGB(255, 255, 255), true);
		BIT_FLAG_UNSET(m_byDisplayFlag, dFLAG_LEADER);
	}
}

VOID CPartyMemberGui::SetOtherWorld(RwBool bOtherWorld)
{
	if( bOtherWorld )
		BIT_FLAG_SET(m_byDisplayFlag, dFLAG_OTHER_WORLD);
	else
		BIT_FLAG_UNSET(m_byDisplayFlag, dFLAG_OTHER_WORLD);
}

VOID CPartyMemberGui::SetCurEmergencyPulse(RwReal fPulse)
{
	m_feEmergency.SetCurrentPulseTime(fPulse);
}

VOID CPartyMemberGui::SetEmergencyState(RwBool bState)
{
	m_feEmergency.SetBeginState(bState);
}

RwReal CPartyMemberGui::GetCurEmergencyPulse()
{
	if( m_feEmergency.IsWork() == false )
		return -1.f;

	return m_feEmergency.GetCurrentPulseTime();
}

RwBool CPartyMemberGui::GetEmergencyState()
{
	return m_feEmergency.GetState();
}

VOID CPartyMemberGui::ShowBuff(bool bShow)
{
	m_pBuff->Show(bShow);
}

VOID CPartyMemberGui::OnMouseDown(const CKey& key)
{
	m_byMouse = (RwInt8)key.m_nID;
}

VOID CPartyMemberGui::OnMouseUp(const CKey& key)
{
	if( m_byMouse != key.m_nID )
	{
		m_byMouse = INVALID_BYTE;
		return;
	}

	m_byMouse = INVALID_BYTE;

	if( key.m_nID != UD_LEFT_BUTTON && 
		key.m_nID != UD_RIGHT_BUTTON )
		return;


	if( key.m_nID == UD_LEFT_BUTTON )
	{
		SERIAL_HANDLE hTargetSerialId = m_hSerial;

		Logic_SobTarget(hTargetSerialId, INVALID_BYTE);

		hTargetSerialId = GetDboGlobal()->GetCommunityTarget();

		if (m_hSerial != hTargetSerialId)
		{
			if (hTargetSerialId != INVALID_SERIAL_ID)
			{
				CNtlSLEventGenerator::CommuTargetRelease(hTargetSerialId);
			}

			if (GetDialogManager()->GetDialog(DIALOG_TARGER_UI)->IsShow() == false)
				CNtlSLEventGenerator::CommuTargetSelect(NULL, m_hSerial, (WCHAR*)m_pNameStatic->GetText().c_str(), COMMU_TARGET_PARTY);
		}

		if (GetNtlSLGlobal()->IsSkillSelectMode())
		{
			Logic_UseProc(GetNtlSLGlobal()->GetSkillSelectModeHandle(), INVALID_BYTE, Logic_GetAvatarTargetHandle(), false);

			if (GetNtlSLGlobal()->IsSkillSelectMode())
			{
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);
			}
		}
	}
	else if( key.m_nID == UD_RIGHT_BUTTON )
	{
		if( Logic_I_am_PartyLeader() )
		{
			CRectangle rect = GetPosition();
			CDboEventGenerator::IconPopupShow(TRUE, m_hSerial, PLACE_SUB_PARTYMEMBERUI, 0,
				rect.right - NTL_ITEM_ICON_SIZE, rect.top - NTL_ITEM_ICON_SIZE);
		}
	}	
}

VOID CPartyMemberGui::OnMove(RwInt32 nX, RwInt32 nY)
{
	CRectangle rectParent = GetPosition();
	CRectangle rtScreen = m_pThis->GetScreenRect();
	
	m_srfOtherWorldPanel.SetPositionbyParent(rectParent.left, rectParent.top);
	m_srfDeathPanel.SetPositionbyParent(rectParent.left, rectParent.top);

	m_srfLeaderMark.SetPositionbyParent(rectParent.left, rectParent.top);	
	m_srfDeathMark.SetPositionbyParent(rectParent.left, rectParent.top);	
	m_srfTooFarMark.SetPositionbyParent(rectParent.left, rectParent.top);	

	m_pBuff->SetPosition(rectParent.left, rectParent.bottom);

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEmergency.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEmergency.SetCenterPos( rtScreen.left + nCenterX, rtScreen.top + nCenterY + 2);
}

VOID CPartyMemberGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(m_hSerial);
	if(pSobObj)
	{
		GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_BATTLE_TARGET );
	}	
}

VOID CPartyMemberGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( BIT_FLAG_TEST(m_byDisplayFlag, dFLAG_DEATH) )
	{
		if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_JUST_WTEXT &&
			GetInfoWndManager()->GetRequestGui() != DIALOG_PARTYMENU )
		{
			const WCHAR* pText = GetDisplayStringManager()->GetString("DST_KNOCKDOWN");
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
				rtScreen.left, rtScreen.top, (void*)pText, DIALOG_PARTYMENU );			
		}

		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyMemberGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
	GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );
}

VOID CPartyMemberGui::OnPostPaint()
{	
	// 마크
	if( BIT_FLAG_TEST(m_byDisplayFlag, dFLAG_DEATH) )
	{
		m_srfDeathMark.Render();
	}
	else if( BIT_FLAG_TEST(m_byDisplayFlag, dFLAG_LEADER) )
	{
		m_srfLeaderMark.Render();
	}

	m_feEmergency.Render();

	// 패널
	if( BIT_FLAG_TEST(m_byDisplayFlag, dFLAG_DEATH) )
	{
		m_srfDeathPanel.Render();
	}
	else if( BIT_FLAG_TEST(m_byDisplayFlag, dFLAG_OTHER_WORLD) )
	{
		m_srfTooFarMark.Render();
		m_srfOtherWorldPanel.Render();
	}
}

VOID CPartyMemberGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPartyMemberGui::HandleEvents");

	if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pPacket = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		switch(pPacket->iMessage)
		{
		case PMT_PARTY_LEADER_CHANGE:
			{
				if( m_hSerial == pPacket->hSerialId )
					SetLeader(TRUE);
				else
					SetLeader(FALSE);

				break;
			}
		case PMT_EP:
			{
				if( m_hSerial == pPacket->hSerialId )
					SetEP(pPacket->uiValue, pPacket->iValue2);

				break;
			}
		case PMT_LP:
			{
				if( m_hSerial == pPacket->hSerialId )
					SetLP(pPacket->uiValue, pPacket->iValue2);					

				break;
			}
		case PMT_LEVEL:
			{
				if( m_hSerial == pPacket->hSerialId )
					SetLevel((RwInt8)pPacket->uiValue);

				break;
			}
		case PMT_CLASS:
			{
				if( m_hSerial == pPacket->hSerialId )
					SetClass((RwUInt8)pPacket->uiValue);

				break;
			}
		}
	}
	else if(msg.Id == g_EventIconPopupResult)
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>(msg.pData);
		if(pPacket->nSrcPlace != PLACE_SUB_PARTYMEMBERUI)
			return;

		if( m_hSerial != pPacket->uiSerial )
			return;

		if( pPacket->nWorkId == PMW_PARTY_LEADER_CHANGE )
		{				
			// 파티장 위임
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyChangeLeader(m_hSerial);
		}
		else if(pPacket->nWorkId == PMW_PARTY_KICK_OUT)
		{
			// 파티 강퇴
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyKickOut(m_hSerial);
		}
	}
	else if (msg.Id == g_EventChangeCharName)
	{
		if (IsShow())
		{
			SNtlEventChangeCharName* pEvent = reinterpret_cast<SNtlEventChangeCharName*>(msg.pData);

			if (_wcsicmp(m_pNameStatic->GetText().c_str(), pEvent->wchOldCharName) == 0)
			{
				m_pNameStatic->SetText(pEvent->wchCharName);
			}
		}
	}

	NTL_RETURNVOID();
}