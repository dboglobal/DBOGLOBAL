#include "precomp_dboclient.h"
#include "GMPopupGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlWorldConceptTB.h"
#include "NtlWorldConcept.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// dbo
#include "DialogPriority.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DialogDefine.h"
#include "DboGlobal.h"


#define dVISIBLE_LIST_COUNT			5
#define dBUTTON_START_Y				16
#define dBUTTON_GAP_Y				24

#define dCAMERA_LOC_COUNT			3
#define dCAMERA_MOVE_COUNT			4

#define dPORTAL_LOCATION_WORLD			L"World"
#define dPORTAL_LOCATION_HELIPORT		L"Budokai gate"


CGMPopupGui::CGMPopupGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_ePopupType(INVALID_POPUP)
{

}

CGMPopupGui::~CGMPopupGui()
{

}

RwBool CGMPopupGui::Create()
{
	NTL_FUNCTION( "CGMPopupGui::Create" );

	if(!CNtlPLGui::Create("gui\\Observer.rsr", "gui\\GMPopup.srf", "gui\\GMPopup.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );

	m_pScrollbar = (gui::CScrollBar*)GetComponent("scbScroll");

	// sig
	m_slotServerScrollChanged		= m_pScrollbar->SigValueChanged().Connect( this, &CGMPopupGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollbar->SigSliderMoved().Connect( this, &CGMPopupGui::OnScrollChanged );
	m_slotCaptureWheelMove			= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CGMPopupGui::OnCaptureWheelMove );

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGMPopupGui::Destroy()
{
	NTL_FUNCTION("CGMPopupGui::Destroy");

	RemoveAllMenu();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGMPopupGui::RefreshList(RwInt32 iOffset)
{
	RwUInt32 uiCount = 0;
	RwInt32 iY = dBUTTON_START_Y;
	LIST_BUTTONS::iterator it = m_listButtons.begin();

	for( ; it != m_listButtons.end() ; ++it )
	{
		sPopButton* pPopButton = *it;

		if( (RwUInt8)iOffset <= uiCount && uiCount < (RwUInt32)iOffset + dVISIBLE_LIST_COUNT )
		{
			pPopButton->pButton->SetPosition(7, iY);
			pPopButton->pButton->Show(true);

			iY += dBUTTON_GAP_Y;
		}
		else
		{
			pPopButton->pButton->Show(false);
		}

		++uiCount;
	}
}

gui::CButton* CGMPopupGui::AddButton(sPopButton* pPopButton, const WCHAR* pwcText)
{
	CRectangle rect;
	rect.SetRectWH(7, 0, 131, 23);	
	pPopButton->pButton = NTL_NEW gui::CButton(rect, "",
							GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\GMPopup.srf", "srfBtnButtonUp" ),
							GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\GMPopup.srf", "srfBtnButtonDown" ),
							GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\GMPopup.srf", "srfBtnButtonUp" ),
							GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "gui\\GMPopup.srf", "srfBtnButtonUp" ),
							NTL_BUTTON_UP_COLOR, NTL_BUTTON_DOWN_COLOR, NTL_BUTTON_FOCUS_COLOR, NTL_BUTTON_DISABLE_COLOR,
							GUI_BUTTON_DOWN_COORD_X, GUI_BUTTON_DOWN_COORD_Y,
							m_pThis, GetNtlGuiManager()->GetSurfaceManager() );

	pPopButton->pButton->SetText(pwcText);
	pPopButton->slotButton = pPopButton->pButton->SigClicked().Connect(this, &CGMPopupGui::OnClicked_Buttons);

	// 반환하지 않아도 되지만 그냥 ;;
	return pPopButton->pButton;
}

VOID CGMPopupGui::AddCancelButton()
{
	sPopButton* pPopButton = NTL_NEW sPopButton;
	pPopButton->pButton			= AddButton(pPopButton, GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2") );
	pPopButton->uiCount			= 100;			// 의미 없다
	pPopButton->bCancelButton	= TRUE;
	m_listButtons.push_back(pPopButton);
}

VOID CGMPopupGui::RemoveAllMenu()
{
	LIST_BUTTONS::iterator it = m_listButtons.begin();
	for( ; it != m_listButtons.end() ; ++it )
	{
		sPopButton* pPopButton = *it;
		NTL_DELETE(pPopButton->pButton);
		NTL_DELETE(pPopButton);
	}
	m_listButtons.clear();
	m_pScrollbar->SetMaxValue(0);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
}

VOID CGMPopupGui::SetBudokaiInfo(RwUInt8 byMatchType, RwUInt8 byMatchWorldType, RwUInt8 byMatchDepth,
								 RwUInt8 byStateCount, sBUDOKAI_GM_MATCH_PROGRESS_STATE* aStateList)
{
	m_BudokaiButtonInfo.byMatchType				= byMatchType;
	m_BudokaiButtonInfo.byMatchWorldType		= byMatchWorldType;
	m_BudokaiButtonInfo.byMatchDepth			= byMatchDepth;
	m_BudokaiButtonInfo.byStateCount			= byStateCount;
	memcpy(m_BudokaiButtonInfo.aStateList, aStateList, sizeof(sBUDOKAI_GM_MATCH_PROGRESS_STATE)*byStateCount);
}

VOID CGMPopupGui::PopupType(ePopupType eType)
{
	if( eType >= INVALID_POPUP )
	{
		DBO_FAIL("Invalid type");
		return;
	}

	RemoveAllMenu();

	m_ePopupType = eType;

	switch(m_ePopupType)
	{
	case POPUP_PORTAL:				SetMenu_Portal();				break;
	case POPUP_CAMERA_LOC:			SetMenu_CameraLoc();			break;
	case POPUP_CAMERA_MOVE:			SetMenu_CameraMove();			break;
	case POPUP_PLAYER_INFO:			SetMenu_PlayerInfo();			break;
	case POPUP_BUDOKAI_TEAM:		SetMenu_BudokaiTeam();			break;
	case POPUP_BUDOKAI_INDIVIDIAUL:	SetMenu_BudokaiIndividual();	break;
	default:	DBO_FAIL("Invalid Pop type");	 return;
	}
}

VOID CGMPopupGui::SetMenu_Portal()
{
#define dADDITIONAL_LOCATION			3

	WCHAR awcBuffer[dADDITIONAL_LOCATION][32] = {L"Budokai Team", L"Budokai Individual", L"Leave Budokai"};
	RwBool bTenkaichibudokai = FALSE;
	sWORLD_INFO* pWORLD_INFO = Logic_GetActiveWorldInfo();
	RwUInt32 uiCount = 0;

	// 천하제일 무도회장 혹은 본래의 월드로....
	if( pWORLD_INFO->sRuleInfo.byRuleType == GAMERULE_MINORMATCH ||
		pWORLD_INFO->sRuleInfo.byRuleType == GAMERULE_MAJORMATCH ||
		pWORLD_INFO->sRuleInfo.byRuleType == GAMERULE_FINALMATCH ||
		pWORLD_INFO->sRuleInfo.byRuleType == GAMERULE_TEINKAICHIBUDOKAI )
		bTenkaichibudokai = TRUE;

	sPopButton* pPopButton = NTL_NEW sPopButton;

	if( bTenkaichibudokai )
		pPopButton->pButton			= AddButton(pPopButton, dPORTAL_LOCATION_WORLD );
	else
		pPopButton->pButton			= AddButton(pPopButton, dPORTAL_LOCATION_HELIPORT );

	pPopButton->uiCount			= uiCount;
	pPopButton->bCancelButton	= FALSE;
	m_listButtons.push_back(pPopButton);

	++uiCount;

	// 기타 지역
	for( RwUInt8 i = 0 ; i < dADDITIONAL_LOCATION ; ++i )
	{
		sPopButton* pPopButtonNew = NTL_NEW sPopButton;

		pPopButtonNew->pButton			= AddButton(pPopButtonNew, awcBuffer[i]);
		pPopButtonNew->uiCount			= uiCount;
		pPopButtonNew->bCancelButton	= FALSE;

		m_listButtons.push_back(pPopButtonNew);

		++uiCount;
	}

	AddCancelButton();

	m_pScrollbar->SetMaxValue(m_listButtons.size() - dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);	
}

VOID CGMPopupGui::SetMenu_CameraLoc()
{
	WCHAR awcBuffer[32] = L"";

	for( RwUInt8 i = 0 ; i < dCAMERA_LOC_COUNT ; ++i )
	{
		sPopButton* pPopButton = NTL_NEW sPopButton;
		
		swprintf_s(awcBuffer, 32, L"Camera Loc %d", i);

		pPopButton->pButton			= AddButton(pPopButton, awcBuffer);
		pPopButton->uiCount			= i;
		pPopButton->bCancelButton	= FALSE;

		m_listButtons.push_back(pPopButton);
	}

	AddCancelButton();

	m_pScrollbar->SetMaxValue(m_listButtons.size() - dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);
}

VOID CGMPopupGui::SetMenu_CameraMove()
{
	WCHAR awcBuffer[32] = L"";

	for( RwUInt8 i = 0 ; i < dCAMERA_MOVE_COUNT ; ++i )
	{
		sPopButton* pPopButton = NTL_NEW sPopButton;

		swprintf_s(awcBuffer, 32, L"Camera move %d", i);

		pPopButton->pButton			= AddButton(pPopButton, awcBuffer);
		pPopButton->uiCount			= i;
		pPopButton->bCancelButton	= FALSE;

		m_listButtons.push_back(pPopButton);
	}

	AddCancelButton();
	
	m_pScrollbar->SetMaxValue(m_listButtons.size() - dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);
}

VOID CGMPopupGui::SetMenu_PlayerInfo()
{
	CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_T_BUDOKAI) );
	if( !pTBWorldConcept )
	{
		AddCancelButton();
		m_pScrollbar->SetMaxValue(m_listButtons.size() -  dVISIBLE_LIST_COUNT);
		m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
		RefreshList(0);
		return;
	}

	MAP_TBMEMBER* pMap_TBMembers = pTBWorldConcept->GetMembers();
	if( !pMap_TBMembers )
	{
		AddCancelButton();
		m_pScrollbar->SetMaxValue(m_listButtons.size() -  dVISIBLE_LIST_COUNT);
		m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
		RefreshList(0);
		return;
	}

	MAP_TBMEMBER::iterator it = pMap_TBMembers->begin();

	for( ; it != pMap_TBMembers->end() ; ++it )
	{
		stTBudokaiMember* pBudokaiMember = it->second;
		sPopButton* pPopButton = NTL_NEW sPopButton;

		pPopButton->pButton			= AddButton(pPopButton, pBudokaiMember->wstrName.c_str());
		pPopButton->hPlayer			= pBudokaiMember->hSerial;
		pPopButton->bCancelButton	= FALSE;

		m_listButtons.push_back(pPopButton);
	}

	AddCancelButton();

	m_pScrollbar->SetMaxValue(m_listButtons.size() -  dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);
}

VOID CGMPopupGui::SetMenu_BudokaiTeam()
{
	WCHAR awcBuffer[32] = L"";

	for( RwUInt8 i = 0 ; i < m_BudokaiButtonInfo.byStateCount ; ++i )
	{
		switch( m_BudokaiButtonInfo.aStateList[i].byProgressState )
		{
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_READY:
			{
				swprintf_s(awcBuffer, 32, L"(Ready)Team %d", i);
				break;
			}
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_RUN:
			{
				swprintf_s(awcBuffer, 32, L"(Run)Team %d", i);
				break;
			}
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED:
			{
				swprintf_s(awcBuffer, 32, L"(Finish)Team %d", i);
				break;
			}
		default:
			{
				DBO_FAIL("Invalid budokai progress type : " << m_BudokaiButtonInfo.aStateList[i].byProgressState);
				continue;
			}
		}
	
		sPopButton* pPopButton = NTL_NEW sPopButton;

		pPopButton->pButton			= AddButton(pPopButton, awcBuffer);
		pPopButton->uiCount			= i;
		pPopButton->bCancelButton	= FALSE;

		if( m_BudokaiButtonInfo.aStateList[i].byProgressState == BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED )
			pPopButton->pButton->ClickEnable(false);

		m_listButtons.push_back(pPopButton);
	}

	AddCancelButton();

	m_pScrollbar->SetMaxValue(m_listButtons.size() - dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);
}

VOID CGMPopupGui::SetMenu_BudokaiIndividual()
{
	WCHAR awcBuffer[32] = L"";

	for( RwUInt8 i = 0 ; i < m_BudokaiButtonInfo.byStateCount ; ++i )
	{
		switch( m_BudokaiButtonInfo.aStateList[i].byProgressState )
		{
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_READY:
			{
				swprintf_s(awcBuffer, 32, L"(Ready)Indiv %d", i);
				break;
			}
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_RUN:
			{
				swprintf_s(awcBuffer, 32, L"(Run)Indiv %d", i);
				break;
			}
		case BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED:
			{
				swprintf_s(awcBuffer, 32, L"(Finish)Indiv %d", i);
				break;
			}
		default:
			{
				DBO_FAIL("Invalid budokai progress type : " << m_BudokaiButtonInfo.aStateList[i].byProgressState);
				continue;
			}
		}

		sPopButton* pPopButton = NTL_NEW sPopButton;

		swprintf_s(awcBuffer, 32, L"Individual %d", i);

		pPopButton->pButton			= AddButton(pPopButton, awcBuffer);
		pPopButton->uiCount			= i;
		pPopButton->bCancelButton	= FALSE;

		if( m_BudokaiButtonInfo.aStateList[i].byProgressState == BUDOKAI_GM_MATCH_PROGRESS_STATE_FINISHED )
			pPopButton->pButton->ClickEnable(false);

		m_listButtons.push_back(pPopButton);
	}

	AddCancelButton();

	m_pScrollbar->SetMaxValue(m_listButtons.size() - dVISIBLE_LIST_COUNT);
	m_pScrollbar->SetRange(0, m_pScrollbar->GetMaxValue());
	RefreshList(0);
}

VOID CGMPopupGui::OnClicked_Buttons( gui::CComponent* pComponent )
{
	LIST_BUTTONS::iterator it = m_listButtons.begin();

	for( ; it != m_listButtons.end() ; ++it )
	{
		sPopButton* pPopButton = *it;

		if( pPopButton->pButton != pComponent )
			continue;

		if( pPopButton->bCancelButton == FALSE )
		{
			switch(m_ePopupType)
			{
			case POPUP_PORTAL:
				{
					if( pPopButton->uiCount == 0 )
					{
						std::wstring wstrText = pPopButton->pButton->GetTextComponent()->GetText();

						if( wcscmp(wstrText.c_str(), dPORTAL_LOCATION_HELIPORT) == 0 )
						{
							GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiServerEnterReq();							
						}
						else if( wcscmp(wstrText.c_str(), dPORTAL_LOCATION_WORLD) == 0 )
						{
							GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiServerLeaveReq();
						}
					}
					else if( pPopButton->uiCount == 1 )
					{
						if( GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiMatchProgressInfoReq(BUDOKAI_MATCH_TYPE_TEAM) )
							CDboEventGenerator::ObserverMenu(OBSERVER_TENKAICHI_MATCH_DATA_REQ);
					}
					else if( pPopButton->uiCount == 2 )
					{
						if( GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiMatchProgressInfoReq(BUDOKAI_MATCH_TYPE_INDIVIDIAUL) )
							CDboEventGenerator::ObserverMenu(OBSERVER_TENKAICHI_MATCH_DATA_REQ);
					}
					else if( pPopButton->uiCount == 3 )
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiMatchArenaLeaveReq();
					}

					break;
				}
			case POPUP_CAMERA_LOC:
				{
					CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_STATIC, (RwInt32)pPopButton->uiCount);
					break;
				}
			case POPUP_CAMERA_MOVE:
				{
					CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_NODE, (RwInt32)pPopButton->uiCount);
					break;
				}
			case POPUP_PLAYER_INFO:
				{
					CDboEventGenerator::ObserverMenu(OBSERVER_SHOW_PLAYER_INFO, pPopButton->hPlayer);
					break;
				}
			case POPUP_BUDOKAI_TEAM:
			case POPUP_BUDOKAI_INDIVIDIAUL:
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendGMBudokaiMatchArenaEnterReq(m_BudokaiButtonInfo.byMatchType,
																							  m_BudokaiButtonInfo.byMatchWorldType,
																							  m_BudokaiButtonInfo.byMatchDepth,
																							  m_BudokaiButtonInfo.aStateList[pPopButton->uiCount].byMatchIndex);
					break;
				}
			default:
				{
					DBO_FAIL("Invalid popup type");
					break;
				}
			}
		}
	}

	CDboEventGenerator::ObserverMenuPost(OBSERVER_SELECT_POPUPMENU);
}

VOID CGMPopupGui::OnScrollChanged(RwInt32 iOffset)
{
	RefreshList(iOffset);
}

VOID CGMPopupGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 iValue = m_pScrollbar->GetValue();
	RwInt32 iMaxValue = m_pScrollbar->GetMaxValue();

	if( sDelta < 0 )
		++iValue;
	else
		--iValue;

	if( iValue < 0 )
		iValue = 0;
	else if( iValue > iMaxValue )
		iValue = iMaxValue;

	m_pScrollbar->SetValue( iValue );
}

RwInt32 CGMPopupGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);
		m_ePopupType = INVALID_POPUP;
		RemoveAllMenu();
	}

	return 1;
}