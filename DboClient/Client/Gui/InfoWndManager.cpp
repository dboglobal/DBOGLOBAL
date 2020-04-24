#include "precomp_dboclient.h"
#include "InfoWndManager.h"

// Presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"

// Dbo
#include "Dbodef.h"
#include "InfoWindowGui.h"
#include "PopupManager.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "DialogManager.h"
#include "PortalGui.h"
#include "SkillCustomizeItem.h"

CInfoWndManager* CInfoWndManager::m_pInstance = NULL;


CInfoWndManager::CInfoWndManager(VOID)
{
	m_pInfoWnd = NULL;
}

CInfoWndManager::~CInfoWndManager(VOID)
{
}

RwBool CInfoWndManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CInfoWndManager::CreateInstance" );

	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CInfoWndManager;
		NTL_ASSERT( m_pInstance, "CInfoWndManager::CreateInstance : Instance Allocate Fail" );

		m_pInstance->m_pInfoWnd = NTL_NEW CInfoWindowGui( "InfoWndGui" );
		NTL_ASSERT( m_pInstance->m_pInfoWnd, "CInfoWndManager::CreateInstance : InfoWnd Allocate Fail" );
		if( !m_pInstance->m_pInfoWnd->Create() )
		{
			m_pInstance->m_pInfoWnd->Destroy();
			NTL_DELETE( m_pInstance->m_pInfoWnd );
			NTL_RETURN( FALSE );
		}
		CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance->m_pInfoWnd );
		
		m_pInstance->LinkEvent();
	}

	NTL_RETURN( TRUE );
}

VOID CInfoWndManager::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkEvent();

		if( m_pInstance->m_pInfoWnd )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance->m_pInfoWnd );
			m_pInstance->m_pInfoWnd->Destroy();
			NTL_DELETE( m_pInstance->m_pInfoWnd );
		}

		NTL_DELETE( m_pInstance );
	}
}

CInfoWndManager* CInfoWndManager::GetInstance(VOID)
{
	return m_pInstance;
}

VOID CInfoWndManager::ShowInfoWindow( RwBool bShow, RwInt32 eState /* = INFOWND_NOTSHOW */, RwInt32 nXPos /* = 0 */, RwInt32 nYPos /* = 0 */, VOID* pInstance /* = NULL */, RwInt32 eRequestGui /* = DIALOG_UNKNOWN  */)
{
	if( GetPopupManager()->IsVisiblePopupGui() )
		return;

	m_eWndState = (eINFOWNDSTATE)eState;
	m_eRequestGui = eRequestGui;

	// 일단 지움. 
	m_pInfoWnd->Show( false );

	if( bShow )
	{
		switch( eState )
		{
			case INFOWND_ITEM:
				m_pInfoWnd->SetItemInfo( reinterpret_cast<CNtlSobItem*>( pInstance ) ); 
			break;
		case INFOWND_ITEM_DATA:
			{
				sITEM_DATA*		pItemData = reinterpret_cast<sITEM_DATA*>( pInstance );
				sITEM_PROFILE	sItemProfile;

				sItemProfile.bNeedToIdentify	= pItemData->bNeedToIdentify;
				sItemProfile.byCurDur			= pItemData->byCurrentDurability;
				sItemProfile.byGrade			= pItemData->byGrade;
				sItemProfile.byPlace			= pItemData->byPlace;;
				sItemProfile.byPos				= pItemData->byPosition;
				sItemProfile.byRank				= pItemData->byRank;
				sItemProfile.byStackcount		= pItemData->byStackcount;
				sItemProfile.tblidx				= pItemData->itemNo;
				sItemProfile.byBattleAttribute	= pItemData->byBattleAttribute;
				sItemProfile.byRestrictState = pItemData->byRestrictState;
				sItemProfile.byDurationType = pItemData->byDurationType;
				sItemProfile.nUseStartTime = pItemData->nUseStartTime;
				sItemProfile.nUseEndTime = pItemData->nUseEndTime;

				wcscpy_s(sItemProfile.awchMaker, NTL_MAX_SIZE_CHAR_NAME + 1, pItemData->awchMaker);
				memcpy(&sItemProfile.sOptionSet, &pItemData->sOptionSet, sizeof(sITEM_OPTION_SET));

				m_pInfoWnd->SetProfileItemInfo( &sItemProfile );
			}
			break;
		case INFOWND_PROFILE_ITEM:
			m_pInfoWnd->SetProfileItemInfo( reinterpret_cast<sITEM_PROFILE*>( pInstance ) );
			break;
		case INFOWND_TABLE_ITEM:
			m_pInfoWnd->SetTableItemInfo( reinterpret_cast<sITEM_TBLDAT*>( pInstance ) ); 
			break;
		case INFOWND_JUST_ITEM_NAME:
			m_pInfoWnd->SetJustItemNameInfo( reinterpret_cast<sITEM_TBLDAT*>( pInstance ) );
			break;
		case INFOWND_UNIDENTIFIED_ITEM:
			m_pInfoWnd->SetUnIdentifiedInfo();
			break;
		case INFOWND_SKILL: 
			m_pInfoWnd->SetSkillInfo( reinterpret_cast<CNtlSobSkill*>( pInstance ) ); 
			break;
		case INFOWND_NOTLEARN_SKILL:
			m_pInfoWnd->SetNotLearnSkillInfo( reinterpret_cast<sSKILL_TBLDAT*>( pInstance ) );
			break;
		case INFOWND_SKILLRQ:
			m_pInfoWnd->SetSkillRQInfo( reinterpret_cast<RwInt32>( pInstance ) );
			break;
		case INFOWND_BUFF:
			m_pInfoWnd->SetBuffInfo( reinterpret_cast<CNtlSobBuff*>( pInstance ) );
			break;
		case INFOWND_HTBSKILL:
			m_pInfoWnd->SetHTBSkillInfo( reinterpret_cast<CNtlSobHTBSkill*>( pInstance ) );
			break;
		case INFOWND_NOTLEARN_HTBSKILL:
			m_pInfoWnd->SetNotLearnHTBSkillInfo( reinterpret_cast<sHTB_SET_TBLDAT*>( pInstance ) );
			break;
		case INFOWND_JUST_WTEXT:
			m_pInfoWnd->SetJustWTextInfo( reinterpret_cast<WCHAR*>( pInstance ) );
			break;
		case INFOWND_ACTIONSKILL:
			m_pInfoWnd->SetActionSkillInfo( reinterpret_cast<CNtlSobActionSkill*>( pInstance ) );
			break;
		case INFOWND_QUESTITEM:
			m_pInfoWnd->SetQuestItemInfo( reinterpret_cast<sQUESTITEM_TBLDAT*>( pInstance ) );
			break;
		case INFOWND_NOTSOBBUFF:
			m_pInfoWnd->SetBuffInfo( reinterpret_cast<sSKILL_TBLDAT*>( pInstance ) );
			break;
		case INFOWND_REWARD:
			m_pInfoWnd->SetRewardInfo( reinterpret_cast<stINFOWND_REWARD*>( pInstance ) );
			break;
		case INFOWND_FAKEBUFF:
			m_pInfoWnd->SetFakeBuffInfo( reinterpret_cast<sNtlFakeBuff*>( pInstance ) );
			break;
		case INFOWND_MINIMAP:
			m_pInfoWnd->SetMiniMapInfo( reinterpret_cast<MINIMAPINFO_LIST*>( pInstance ) );
			break;
        case INFOWND_PORTAL:
            {
                SPortalTooltip* pPortalToolTip = reinterpret_cast<SPortalTooltip*>(pInstance);
                m_pInfoWnd->SetPortalInfo(pPortalToolTip->nPortalID, pPortalToolTip->ePortalIconType, pPortalToolTip->nZenny);
            }            
            break;
        case INFOWND_FRIEND_LIST:       // 친구 리스트에서 세부 정보를 표시하기 위한 툴팁
            m_pInfoWnd->SetFriendInfo(reinterpret_cast<sFriendMember*>(pInstance));
            break;
		case INFOWND_UPGRADE:
			m_pInfoWnd->SetUpgradeInfo( reinterpret_cast<stINFOWND_UPGRADE*>( pInstance ) );
			break;
		case INFOWND_BATTLEATTRIBUTE:
			m_pInfoWnd->SetBattleAttributeDisplay( reinterpret_cast<stINFOWND_BATTLEATTR*>( pInstance ) );
			break;
		case INFOWND_QUEST_SEARCH:
			m_pInfoWnd->SetQuestSearch( reinterpret_cast<QUEST_SEARCH_LIST*>( pInstance ) );
			break;
		case INFOWND_QUEST_INDICATOR:
			{
				stINFOWND_QUESTINDICATOR* pData = reinterpret_cast<stINFOWND_QUESTINDICATOR*>( pInstance );
				m_pInfoWnd->SetQuestIndicateInfo( pData->uiQuestTitle, pData->uiQuestGoal );
			}
			break;
		case INFOWND_SKILL_SPUPGRADE:
			{
				stSkillUpgradeInfo* pData = reinterpret_cast<stSkillUpgradeInfo*>( pInstance );
				m_pInfoWnd->SetSkillUpgradeInfo( pData );
			}
			break;
		case INFOWND_RECIPE:
			{
				m_pInfoWnd->SetRecipeInfo( reinterpret_cast<sITEM_RECIPE_TBLDAT*>( pInstance ) );
			}
			break;
		case INFOWND_HLS_ITEM:
			m_pInfoWnd->SetHlsItemInfo(reinterpret_cast<sCASHITEM*>(pInstance));
			break;
		default:
			NTL_ASSERTFAIL( "CInfoWndManager::ShowInfoWindow : Invalid State" );
			return;
		}

		m_pInfoWnd->CalcPos( nXPos, nYPos );
		m_pInfoWnd->Show( true );
	}	
}

VOID CInfoWndManager::HandleEvents( RWS::CMsg &msg )
{
    if(msg.Id == g_EventToolTip)
    {
        SNtlEventToolTip* pData = reinterpret_cast<SNtlEventToolTip*>(msg.pData);        
        if(pData && pData->bShow)
        {
            RwInt32 nX, nY;

            if(pData->nX == 0 && pData->nY == 0)
            {
                nX = CMouse::GetX();
                nY = CMouse::GetY();
            }
            else
            {
                nX = pData->nX;
                nY = pData->nY;
            }

            ShowInfoWindow(TRUE, INFOWND_JUST_WTEXT, nX, nY, (void*)pData->wstrText.c_str());
        }
        else if(GetRequestGui() == DIALOG_UNKNOWN)
        {
            ShowInfoWindow(FALSE);
        }        
    }
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->uiState == WORLD_STATE_EXIT )
			ShowInfoWindow(FALSE);
	}
}

VOID CInfoWndManager::LinkEvent(VOID)
{
	// Event Link
    LinkMsg(g_EventToolTip);
	LinkMsg(g_EventChangeWorldConceptState);
}

VOID CInfoWndManager::UnLinkEvent(VOID)
{
	// Event Unlink
    UnLinkMsg(g_EventToolTip);
	UnLinkMsg(g_EventChangeWorldConceptState);
}