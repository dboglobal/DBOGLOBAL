#include "precomp_dboclient.h"
#include "SideDialogControlGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "SideDialogMiniGui.h"
#include "DboGlobal.h"

namespace
{
	#define dCOUNT_USABLE_LEFT_AND_RIGHT_BUTTON		2
	#define dMAX_BUTTON_VISILE						6

	#define dBUTTON_START_X							17
	#define dBUTTON_GAP								1

	#define dADD_BUTTON(ButtonType, ComponentName, Tooltip, SideDialogType) \
	{ \
		m_aButtonType[ButtonType].pButton = (gui::CButton*)GetComponent( ComponentName ); \
		m_aButtonType[ButtonType].pButton->SetToolTip(Tooltip); \
		m_aButtonType[ButtonType].uiSideDialogType = SideDialogType; \
		m_aButtonType[ButtonType].slotButton = m_aButtonType[ButtonType].pButton->SigClicked().Connect(this, &CSideDialogControlGui::OnClick_ButtonType); \
	}
};


CSideDialogControlGui::CSideDialogControlGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byVisibleStart(1)
,m_pSideDialogMiniGui(NULL)
{

}

CSideDialogControlGui::~CSideDialogControlGui()
{

}

RwBool CSideDialogControlGui::Create()
{
	NTL_FUNCTION( "CSideDialogControlGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\SideDialogControl.srf", "gui\\SideDialogControl.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDE_DIALOG);

	// 왼쪽 버튼
	m_pLeftButton = (gui::CButton*)GetComponent( "btnLeft" );
	m_slotLeftButton = m_pLeftButton->SigClicked().Connect(this, &CSideDialogControlGui::OnClick_LeftButton);

	// 오른쪽 버튼
	m_pRightButton = (gui::CButton*)GetComponent( "btnRight" );
	m_slotRightButton = m_pRightButton->SigClicked().Connect(this, &CSideDialogControlGui::OnClick_RightButton);


	dADD_BUTTON(BUTTONTYPE_TMQ,				"btnTMQ",			GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_TMQINFO"),		SDIALOG_TMQ_STATUS);
	dADD_BUTTON(BUTTONTYPE_PCVIEW,			"btnPCView",		GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_TARGETITEMINFO"),	SDIALOG_SCOUTER_PC_VIEWER);
	dADD_BUTTON(BUTTONTYPE_RANKBATTLE,		"btnRankBattle",	GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_RANKBATTLEINFO"),	SDIALOG_RANKBATTLE_INFO);
	dADD_BUTTON(BUTTONTYPE_QUESTINDICATE,	"btnQuestIndicate",	GetDisplayStringManager()->GetString("DST_SIDEDLG_TITLE_QUESTINDICATOR"),	SDIALOG_QUEST_INDICATOR);
	dADD_BUTTON(BUTTONTYPE_BUDOKAI_MINOR,	"btnBudokaiMinor",	GetDisplayStringManager()->GetString("DST_BUDOKAI_REQUEST_MINOR"),		SDIALOG_BUDOKAI_MINORMATCH_INFO);
	dADD_BUTTON(BUTTONTYPE_BUDOKAI_MAJOR,	"btnBudokaiMajor",	GetDisplayStringManager()->GetString("DST_BUDOKAI_REQUEST_MAJOR"),		SDIALOG_BUDOKAI_MAJORMATCH_INFO);
	dADD_BUTTON(BUTTONTYPE_BUDOKAI_FINAL,	"btnBudokaiFinal",	GetDisplayStringManager()->GetString("DST_BUDOKAI_REQUEST_FINAL"),		SDIALOG_BUDOKAI_FINALMATCH_INFO);
	dADD_BUTTON(BUTTONTYPE_BUDOKAI_FINAL,	"btnDojoScramble",	GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_TITLE"),			SDIALOG_SCRAMBLE_STATUS);

	dADD_BUTTON(BUTTONTYPE_CCBD, "btnCCBD", GetDisplayStringManager()->GetString("DST_CCBD"), SDIALOG_CCBD_STATUS);


	m_pSideDialogMiniGui = NTL_NEW CSideDialogMiniGui("SideDialogMini");
	if( !m_pSideDialogMiniGui->Create() )
	{
		m_pSideDialogMiniGui->Destroy();
		NTL_DELETE(m_pSideDialogMiniGui);
		NTL_RETURN(FALSE);	
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pSideDialogMiniGui);	

	m_slotMove			= m_pThis->SigMove().Connect( this, &CSideDialogControlGui::OnMove );	

	LinkMsg(g_EventSideDialog);	

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CSideDialogControlGui::Destroy()
{
	NTL_FUNCTION("CSideDialogControlGui::Destroy");

	m_listEnableButton.clear();

	if( m_pSideDialogMiniGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pSideDialogMiniGui);
		m_pSideDialogMiniGui->Destroy();
		NTL_DELETE(m_pSideDialogMiniGui);
	}

	UnLinkMsg(g_EventSideDialog);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwBool CSideDialogControlGui::SelectButtonType(RwUInt32 uiDialogType)
{
	LIST_BUTTON_ITER it = m_listEnableButton.begin();
	for( ; it != m_listEnableButton.end() ; ++it )
	{
		sButtonType* pButtonType = *it;
		if( pButtonType->uiSideDialogType == uiDialogType )
		{
			if( GetSideDialogManager()->OpenDialog(pButtonType->uiSideDialogType) )
			{
				m_pSideDialogMiniGui->Inactive();
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return FALSE;
}

VOID CSideDialogControlGui::ArrangeButtons(RwBool bOpenStartIndex /* = FALSE */)
{
	if( m_listEnableButton.size() < dCOUNT_USABLE_LEFT_AND_RIGHT_BUTTON )
	{
		RwUInt8 byCount = 0;
		LIST_BUTTON_ITER it = m_listEnableButton.begin();
		for( ; it != m_listEnableButton.end() ; ++it )
		{
			++byCount;
			
			// 화면에 보이는 버튼중 첫 번째에 해당하는 다이얼로그를 연다
			if( m_byVisibleStart == byCount )
			{
				if( bOpenStartIndex )
				{
					sButtonType* pButtonType = *it;
					GetSideDialogManager()->OpenDialog(pButtonType->uiSideDialogType);
				}

				break;
			}
		}

		GetDialogManager()->CloseDialog(DIALOG_SIDEDIALOG_CONTROLLER);
	}
	else
	{
		RwUInt8 byCount = 0;
		RwUInt32 iButtonX = dBUTTON_START_X;
		LIST_BUTTON_ITER it = m_listEnableButton.begin();
		for( ; it != m_listEnableButton.end() ; ++it )
		{
			++byCount;

			sButtonType* pButtonType = *it;
			if( m_byVisibleStart <= byCount && byCount <= dMAX_BUTTON_VISILE )
			{
				// 화면에 보이는 버튼중 첫 번째에 해당하는 다이얼로그를 연다
				if( m_byVisibleStart == byCount )
				{
					if( bOpenStartIndex )
						GetSideDialogManager()->OpenDialog(pButtonType->uiSideDialogType);
				}

				pButtonType->pButton->SetPosition(iButtonX, 0);
				pButtonType->pButton->Show(true);

				iButtonX += pButtonType->pButton->GetWidth() + dBUTTON_GAP;
			}
			else
			{
				pButtonType->pButton->Show(false);
			}			
		}

		GetDialogManager()->OpenDialog(DIALOG_SIDEDIALOG_CONTROLLER);
	}

	if( m_listEnableButton.size() > dMAX_BUTTON_VISILE )
	{
		m_pLeftButton->Show(true);
		m_pRightButton->Show(true);
	}
	else
	{
		m_pLeftButton->Show(false);
		m_pRightButton->Show(false);
	}
}

VOID CSideDialogControlGui::AddButton(RwUInt32 uiSideDialogType)
{
	// 이미 등록이 되어 있으면 리턴
	LIST_BUTTON_ITER it = m_listEnableButton.begin();
	for( ; it != m_listEnableButton.end() ; ++it )
	{
		sButtonType* pButtonType = *it;
		if( pButtonType->uiSideDialogType == uiSideDialogType )
			return;
	}

	for( RwUInt32 i = 0 ; i < NUM_BUTTONTYPE ; ++i )
	{
		if( m_aButtonType[i].uiSideDialogType == uiSideDialogType )
		{
			m_listEnableButton.push_back(&m_aButtonType[i]);

			// 현재 Add되는 것이 표시되니 기존의 최소화된 다이얼로그는 표시할 필요가 없다
			m_pSideDialogMiniGui->Inactive();
			break;
		}
	}

	ArrangeButtons();
}

VOID CSideDialogControlGui::DelButton(RwUInt32 uiSideDialogType)
{
	RwUInt8 byCount = 0;
	LIST_BUTTON_ITER it = m_listEnableButton.begin();
	for( ; it != m_listEnableButton.end() ; ++it )
	{
		++byCount;

		sButtonType* pButtonType = *it;
		if( pButtonType->uiSideDialogType == uiSideDialogType )
		{
			if( m_byVisibleStart > byCount )
				--m_byVisibleStart;

			if( m_listEnableButton.size() - m_byVisibleStart > dMAX_BUTTON_VISILE )
				--m_byVisibleStart;

			pButtonType->pButton->Show(false);
			m_listEnableButton.erase(it);

			if( m_pSideDialogMiniGui->GetSideDialogType() == uiSideDialogType )
				m_pSideDialogMiniGui->Inactive();

			ArrangeButtons(TRUE);

			return;
		}		
	}
}

VOID CSideDialogControlGui::OnClick_LeftButton(gui::CComponent* pComponent)
{
	if( m_byVisibleStart > 1 )
	{
		--m_byVisibleStart;
		m_pSideDialogMiniGui->Inactive();
		ArrangeButtons();
	}
}

VOID CSideDialogControlGui::OnClick_RightButton(gui::CComponent* pComponent)
{
	if( m_byVisibleStart + dMAX_BUTTON_VISILE < (RwUInt8)(m_listEnableButton.size() + 1) )
	{
		++m_byVisibleStart;
		m_pSideDialogMiniGui->Inactive();
		ArrangeButtons();
	}
}

VOID CSideDialogControlGui::OnClick_ButtonType(gui::CComponent* pComponent)
{
	for( RwUInt32 i = 0 ; i < NUM_BUTTONTYPE ; ++i )
	{
		if( m_aButtonType[i].pButton == pComponent )
		{
			if( GetSideDialogManager()->OpenDialog(m_aButtonType[i].uiSideDialogType) )
				m_pSideDialogMiniGui->Inactive();
		}
	}
}

VOID CSideDialogControlGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	m_pSideDialogMiniGui->SetPosition(GetDboGlobal()->GetScreenWidth() - m_pSideDialogMiniGui->GetWidth() - dDIALOG_CLEINT_EDGE_GAP,
									  GetPosition().bottom);
}

RwInt32 CSideDialogControlGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		if( m_listEnableButton.size() < dCOUNT_USABLE_LEFT_AND_RIGHT_BUTTON )
			return -1;

		Show(true);
	}
	else
	{
		Show(false);
	}	

	return 1;
}

VOID CSideDialogControlGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CSideDialogControlGui::HandleEvents");

	if( msg.Id == g_EventSideDialog )
	{
		SDBoEventSideDialog* pEvent = reinterpret_cast<SDBoEventSideDialog*>( msg.pData );

		switch(pEvent->byMessage)
		{
		case SDIALOG_MESSAGE_CONTROLLER_ADD_BUTTON:
			{
				AddButton(pEvent->uiExData);
				break;
			}
		case SDIALOG_MESSAGE_CONTROLLER_DEL_BUTTON:
			{
				DelButton(pEvent->uiExData);
				break;
			}
		case SDIALOG_MESSAGE_MINIMIZE:
			{
				for( RwUInt32 i = 0 ; i < NUM_BUTTONTYPE ; ++i )
				{
					if( m_aButtonType[i].uiSideDialogType == pEvent->uiSrcSDialog )
					{
						// 사이드 다이얼로그 공통 최소화 GUI를 보여준다
						m_pSideDialogMiniGui->Active(m_aButtonType[i].uiSideDialogType,
													 m_aButtonType[i].pButton->GetToolTip().c_str());
						break;
					}
				}
				break;
			}
		case SDIALOG_MESSAGE_INACTIVE:
			{
				m_pSideDialogMiniGui->Inactive();
				break;
			}
		}		
	}

	NTL_RETURNVOID();
}