#include "precomp_dboclient.h"
#include "DiceManager.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"

// dbo
#include "DiceGui.h"
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"

#define dDISPLAYED_DICE_GAP			40			///< 현재 화면에 표시되고 있는 주사위간 간격


CDiceManager* CDiceManager::m_pInstance = NULL;

CDiceManager* CDiceManager::GetInstance()
{
	return m_pInstance;
}

CDiceManager::CDiceManager(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CDiceManager::~CDiceManager()
{

}

RwBool CDiceManager::CreateInstance()
{
	NTL_FUNCTION("CDiceManager::CreateInstance");

	m_pInstance = NTL_NEW CDiceManager("DiceManager");
	m_pInstance->m_pThis = NTL_NEW gui::CDialog( CNtlPLGuiManager::GetInstance()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );

	// 주사위 타입이 정의된 역순으로 생성하여 가장 먼저 정의된 주사위 GUI가 가장 위에 그려지도록 한다

	// 파티 인벤 주사위
	CDiceGui* pPartyInvenDiceGui = NTL_NEW CItemDiceGui("PartyInvenDiceGui", DICE_WORKD_ID_PARTY_INVEN_DICE);
	if( !pPartyInvenDiceGui->Create(dDICE_FUNCTION_NONE) )
	{
		pPartyInvenDiceGui->Destroy();
		NTL_DELETE(pPartyInvenDiceGui);

		CDiceManager::DeleteInstance();
		NTL_RETURN(FALSE);
	}
	((CItemDiceGui*)pPartyInvenDiceGui)->SetExplain( GetDisplayStringManager()->GetString("DST_PARTY_INVEN_START_DICE") );
	m_pInstance->m_mapDiceGui[DICE_WORKD_ID_PARTY_INVEN_DICE] = pPartyInvenDiceGui;

	// 액션 주사위
	CDiceGui* pActionDiceGui = NTL_NEW CActionDiceGui("ActionDiceGui", DICE_WORKD_ID_ACTION);
	if( !pActionDiceGui->Create(dDICE_FUNCTION_NONE) )
	{
		pActionDiceGui->Destroy();
		NTL_DELETE(pActionDiceGui);

		CDiceManager::DeleteInstance();
		NTL_RETURN(FALSE);
	}	
	((CActionDiceGui*)pActionDiceGui)->SetExplain( GetDisplayStringManager()->GetString("DST_DICE_EXPLAIN") );
	((CActionDiceGui*)pActionDiceGui)->SetExplain2( GetDisplayStringManager()->GetString("DST_DICE_EXPLAIN2") );
	m_pInstance->m_mapDiceGui[DICE_WORKD_ID_ACTION] = pActionDiceGui;

	// 천하제일 무도회 진출자 가려내기 주사위
	CDiceGui* pBudokaiTenkaichiSelectWinnerGui = NTL_NEW CActionDiceGui("BudokaiTenkaichiSelectWinnerGui", DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER);
	if( !pBudokaiTenkaichiSelectWinnerGui->Create(dDICE_FUNCTION_DISABLE_CLOSE_BUTTON | dDICE_FUNCTION_DISABLE_CLICK_DICE) )
	{
		pBudokaiTenkaichiSelectWinnerGui->Destroy();
		NTL_DELETE(pBudokaiTenkaichiSelectWinnerGui);

		CDiceManager::DeleteInstance();
		NTL_RETURN(FALSE);
	}
	((CActionDiceGui*)pBudokaiTenkaichiSelectWinnerGui)->SetExplain( GetDisplayStringManager()->GetString("DST_DICE_EXPLAIN") );
	((CActionDiceGui*)pBudokaiTenkaichiSelectWinnerGui)->SetExplain2( GetDisplayStringManager()->GetString("DST_DICE_BUDOKAI_MINOR_EXPLAIN") );
	m_pInstance->m_mapDiceGui[DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER] = pBudokaiTenkaichiSelectWinnerGui;

	GetDialogManager()->RegistDialog(DIALOG_DICE_MANAGER, m_pInstance, &CDiceManager::SwitchDialog);

	// sig
	m_pInstance->m_slotMove = m_pInstance->m_pThis->SigMove().Connect( m_pInstance, &CDiceManager::OnMove );

	m_pInstance->LinkMsg(g_EventActionMapDice);		
	m_pInstance->LinkMsg(g_EventDiceStart);
	m_pInstance->LinkMsg(g_EventDiceRollReqtoServer);
	m_pInstance->LinkMsg(g_EventDiceResult);

	NTL_RETURN(TRUE);
}

VOID CDiceManager::DeleteInstance()
{
	NTL_FUNCTION("CDiceManager::DestoryInstance");

	if( m_pInstance )
	{
		MAP_DICEGUI::iterator it = m_pInstance->m_mapDiceGui.begin();
		for( ; it != m_pInstance->m_mapDiceGui.end() ; ++it )
		{
			CDiceGui* pDiceGui = it->second;
			pDiceGui->Destroy();
			NTL_DELETE(pDiceGui);
		}

		GetDialogManager()->UnRegistDialog(DIALOG_DICE_MANAGER);


		m_pInstance->UnLinkMsg(g_EventActionMapDice);
		m_pInstance->UnLinkMsg(g_EventDiceStart);
		m_pInstance->UnLinkMsg(g_EventDiceRollReqtoServer);
		m_pInstance->UnLinkMsg(g_EventDiceResult);

		NTL_DELETE(m_pInstance);
	}	

	NTL_RETURNVOID();
}

VOID CDiceManager::Close(RwUInt8 byWorkID)
{
	MAP_DICEGUI::iterator it = m_mapDiceGui.find(byWorkID);
	if( it == m_mapDiceGui.end() )
		return;

	CDiceGui* pDiceGui = it->second;
	if( pDiceGui->IsShow() )
		pDiceGui->Switch(false);
}

VOID CDiceManager::Cancel(RwUInt8 byWorkID)
{
	MAP_DICEGUI::iterator it = m_mapDiceGui.find(byWorkID);
	if( it == m_mapDiceGui.end() )
		return;

	CDiceGui* pDiceGui = it->second;
	if( pDiceGui->IsShow() )
		pDiceGui->Switch(false);
}

VOID CDiceManager::CancelAll()
{
	MAP_DICEGUI::iterator it = m_mapDiceGui.begin();
	for( ; it != m_mapDiceGui.end() ; ++it )
	{
		CDiceGui* pDiceGui = it->second;
		if( pDiceGui->IsShow() )
			pDiceGui->Switch(false);
	}
}

VOID CDiceManager::LineUpDice()
{
	// 계단처럼 우상단으로 정렬된다
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwBool bFoundFirstDiceHeight = FALSE;
	CPos curPos(rtScreen.left, rtScreen.top);

	MAP_DICEGUI::iterator it = m_mapDiceGui.begin();
	for( ; it != m_mapDiceGui.end() ; ++it )
	{
		CDiceGui* pDiceGui = it->second;

		if( !pDiceGui->IsShow() )
			continue;

		if( !bFoundFirstDiceHeight )
		{
			curPos.y -= pDiceGui->GetHeight();
			bFoundFirstDiceHeight = TRUE;
		}

		pDiceGui->SetPosition(curPos.x, curPos.y);

		curPos.x += dDISPLAYED_DICE_GAP;
		curPos.y -= dDISPLAYED_DICE_GAP;
	}
}

RwBool CDiceManager::IsActiveWorkID(RwUInt8 byWorkID)
{
	MAP_DICEGUI::iterator it = m_mapDiceGui.begin();
	for( ; it != m_mapDiceGui.end() ; ++it )
	{
		CDiceGui* pDiceGui = it->second;
		if( pDiceGui->GetWorkID() == byWorkID )
			return TRUE;
	}

	return FALSE;
}

VOID CDiceManager::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LineUpDice();
}

RwInt32 CDiceManager::SwitchDialog(bool bOpen)
{
	return 1;
}

VOID CDiceManager::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDiceManager::HandleEvents");

	if( msg.Id == g_EventActionMapDice )
	{
		MAP_DICEGUI::iterator it = m_mapDiceGui.find(DICE_WORKD_ID_ACTION);
		if( it == m_mapDiceGui.end() )
			NTL_RETURNVOID();

		CDiceGui* pDiceGui = it->second;
		if( pDiceGui->ReadyDice() )
		{
			pDiceGui->Switch(true);
			LineUpDice();
		}
	}
	else if( msg.Id == g_EventDiceStart )
	{
		SDboEventDiceStart* pEvent = reinterpret_cast<SDboEventDiceStart*>( msg.pData );

		MAP_DICEGUI::iterator it = m_mapDiceGui.find(pEvent->byWorkID);
		if( it == m_mapDiceGui.end() )
			NTL_RETURNVOID();

		switch(pEvent->byWorkID)
		{
		case DICE_WORKD_ID_ACTION:
			{
				// g_EventActionMapDice 이벤트에서 받아 처리한다
				break;
			}
		case DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER:
			{
				CActionDiceGui* pBudokaiTenkaichiSelectWinnerGui = reinterpret_cast<CActionDiceGui*>( it->second );
				if( pBudokaiTenkaichiSelectWinnerGui->ReadyDice() )
				{
					pBudokaiTenkaichiSelectWinnerGui->Switch(true);
					LineUpDice();
				}
				break;
			}
		case DICE_WORKD_ID_PARTY_INVEN_DICE:
			{
				CItemDiceGui* pItemDiceGui = reinterpret_cast<CItemDiceGui*>( it->second );
				if( pItemDiceGui->ReadyDice() )
				{			
					pItemDiceGui->Switch(true);
					pItemDiceGui->SetItemData(pEvent->pExData);
					LineUpDice();
				}
				break;
			}
		default:
			{
				DBO_FAIL("g_EventDiceStart, Invalid dice work ID : " << pEvent->byWorkID);
				NTL_RETURNVOID();
			}
		}
	}
	else if( msg.Id == g_EventDiceRollReqtoServer )
	{
		SDboEventDiceRequesttoServer* pEvent = reinterpret_cast<SDboEventDiceRequesttoServer*>( msg.pData );

		switch(pEvent->byWorkID)
		{
			case DICE_WORKD_ID_ACTION:
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendRoolReq();
			}
			break;
			case DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER:
			{
			}
			break;
			case DICE_WORKD_ID_PARTY_INVEN_DICE:
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendItemDiceReq(0, 0); // to do
			}
			break;
			default:
			{
				DBO_FAIL("g_EventDiceRollReqtoServer, Invalid dice work ID : " << pEvent->byWorkID);
				NTL_RETURNVOID();
			}
		}
	}
	else if( msg.Id == g_EventDiceResult )
	{
		SDboEventDiceResult* pEvent = reinterpret_cast<SDboEventDiceResult*>( msg.pData );

		switch(pEvent->byWorkID)
		{
		case DICE_WORKD_ID_ACTION:
		case DICE_WORKD_ID_BUDOKAI_TENKAICHI_SELECT_WINNER:
			{
				if( pEvent->hHandle == Logic_GetAvatarHandle() )
				{
					MAP_DICEGUI::iterator it = m_mapDiceGui.find(pEvent->byWorkID);
					if( it == m_mapDiceGui.end() )
						NTL_RETURNVOID();

					CDiceGui* pDiceGui = it->second;
					pDiceGui->SetResultNum(pEvent->byResultNum);
				}
				else
				{
					CDboEventGenerator::DiceResultShow(pEvent->hHandle, pEvent->byWorkID, pEvent->byResultNum);
				}

				break;
			}
		case DICE_WORKD_ID_PARTY_INVEN_DICE:
			{
				if( pEvent->hHandle == Logic_GetAvatarHandle() )
				{
					MAP_DICEGUI::iterator it = m_mapDiceGui.find(pEvent->byWorkID);
					if( it == m_mapDiceGui.end() )
						NTL_RETURNVOID();

					CDiceGui* pDiceGui = it->second;
					pDiceGui->SetResultNum(pEvent->byResultNum);
				}
				else
				{
					CDboEventGenerator::DiceResultShow(pEvent->hHandle, pEvent->byWorkID, pEvent->byResultNum);
				}

				break;
			}
		}
	}		

	NTL_RETURNVOID();
}