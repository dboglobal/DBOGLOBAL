#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLGlobal.h"

// table

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "SideIconGui.h"
#include "DboEvent.h"
#include "PetitionManager.h"

// etc
#include <algorithm>

#include "HintSideIconGui.h"
#include "DBCSideIconGui.h"
#include "RBSideIconGui.h"
#include "DoRepairSideIconGui.h"
#include "TutorialSideIconGui.h"
#include "CautionSideIconGui.h"
#include "BudokaiSideIconGui.h"
#include "PetitionSideIconGui.h"
#include "GMSideIconGui.h"
#include "DojoSideIconGui.h"

#include "HLSSideIconGui.h"
#include "HLSSideView.h"

#include "SCSSideIconGui.h"

// SideIcon을 생성하고 SideIconGui에 등록하는 매크로
#define RegisterSideIcon( pSideIcon, ClassName, szName, id ) \
	{ \
	pSideIcon = NTL_NEW ClassName(szName); \
	if(!pSideIcon->Create()) \
	{ \
	pSideIcon->Destroy(); \
	NTL_DELETE(pSideIcon); \
	NTL_RETURN(FALSE); \
	} \
	else \
	{ \
		RegSideIcon( id, pSideIcon ); \
	} \
	pSideIcon->GetDialog()->SetPriority( dDIALOGPRIORITY_SIDEICON ); \
	RocateSideIcon(); \
	} \

// SideView를 생성하고 SideViewGui에 등록하는 매크로
#define RegisterSideView( pSideView, ClassName, szName, id ) \
	{ \
	pSideView = NTL_NEW ClassName(szName); \
	if( !pSideView->Create() ) \
	{ \
	pSideView->Destroy(); \
	NTL_DELETE(pSideView); \
	NTL_RETURN(FALSE); \
	} \
	else \
	{ \
		RegSideView( id, pSideView ); \
	} \
	pSideView->GetDialog()->SetPriority( dDIALOGPRIORITY_SIDEICON ); \
	}

// SideIcon을 해제 & 소멸
#define UnRegisterSideIcon( pSideIcon, id ) \
	{ \
	if( pSideIcon ) \
	{ \
		UnRegSideIcon(id); \
		pSideIcon->Destroy(); \
		NTL_DELETE(pSideIcon); \
	} \
	RocateSideIcon(); \
	}

// SideView를 해제 & 소멸
#define UnRegisterSideView( pSideView, id ) \
	{ \
	if( pSideView ) \
	{ \
		UnRegSideView(id); \
		pSideView->Destroy(); \
		NTL_DELETE(pSideView); \
	} \
	}

CSideIconGui* CSideIconGui::s_pSideIconGui;

CSideIconGui::CSideIconGui(const RwChar* pName) : CNtlPLGui(pName), m_pPresentIcon(NULL), m_pPresentView(NULL)
,m_pHintSideIconGui(NULL)
,m_pHelpHintSideViewGui(NULL)
,m_pGuideHintSideViewGui(NULL)
,m_pStringHintSideViewGui(NULL)
,m_pDBCSideIconGui(NULL)
,m_pDBCSideViewGui(NULL)
,m_pRBSideIconGui(NULL)
,m_pRBSideViewGui(NULL)
,m_pDoRepairSideIconGui(NULL)
,m_pDoRepairSideViewGui(NULL)
,m_pTutorialSideIconGui(NULL)
,m_pTutorialSideViewGui(NULL)
,m_pCautionSideIconGui(NULL)
,m_pCautionSideViewGui(NULL)
,m_pBudokaiSideIconGui(NULL)
,m_pBudokaiSideViewGui(NULL)
,m_pPetitionSideIconGui(NULL)
,m_pPetitionSideViewGui(NULL)
,m_pDojoSideIconGui(NULL)
,m_pDojoSideViewGui(NULL)
,m_pHLSSideIconGui(NULL)
,m_pHLSSideViewGui(NULL)
{
	m_nPresentViewType = INVALID_SIDEVIEW;
	s_pSideIconGui = this;
}

CSideIconGui::~CSideIconGui()
{
	if (s_pSideIconGui == this)
	{
		s_pSideIconGui = NULL;
	}
}

RwBool CSideIconGui::Create()
{
	NTL_FUNCTION("CSideIconGui::Create");
	
	m_pThis			= NTL_NEW gui::CDialog(CRectangle(0, 0, 0, 0), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager());
	m_slotMove		= m_pThis->SigMove().Connect(this, &CSideIconGui::OnMove);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_SIDEICON);

	// Help, GuideHint, StringHint
	RegisterSideIcon( m_pHintSideIconGui, CHintSideIconGui, "HintIconGui", SIDEICON_HINT );
	RegisterSideView( m_pHelpHintSideViewGui, CHelpHintSideViewGui, "HelpHintViewGui", SIDEVIEW_HINT_HELP );
	RegisterSideView( m_pGuideHintSideViewGui, CGuideHintSideViewGui, "GuideHintViewGui", SIDEVIEW_HINT_GUIDE );
	RegisterSideView( m_pStringHintSideViewGui, CStringHintSideViewGui, "StringHintViewGui", SIDEVIEW_HINT_STRING );

	// DragonBall Collection
	RegisterSideIcon( m_pDBCSideIconGui, CDBCSideIconGui, "DBCSideIconGui", SIDEICON_DBC );
	RegisterSideView( m_pDBCSideViewGui, CDBCSideViewGui, "DBCSideViewGui", SIDEVIEW_DBC );

	// RankBattle
	RegisterSideIcon( m_pRBSideIconGui, CRBSideIconGui, "RBSideIconGui", SIDEICON_RB );
	RegisterSideView( m_pRBSideViewGui, CRBSideViewGui, "RBSideViewGui", SIDEVIEW_RB );

	// Do Repair
	RegisterSideIcon( m_pDoRepairSideIconGui, CDoRepairSideIconGui, "DoRepairSideIconGui", SIDEICON_DOREPAIR );
	RegisterSideView( m_pDoRepairSideViewGui, CDoRepairSideViewGui, "DoRepairSideViewGui", SIDEVIEW_DOREPAIR );

	// Caution Notify
	RegisterSideIcon( m_pCautionSideIconGui, CCautionSideIconGui, "CautionSideIconGui", SIDEICON_CAUTION );
	RegisterSideView( m_pCautionSideViewGui, CCautionSideViewGui, "CautionSideViewGui", SIDEVIEW_CAUTION );

	STenkaichiBudokaiInfo* pBudokaiState = GetNtlSLGlobal()->GetTBudokaiStateInfo();
	
	if( pBudokaiState->sStateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE &&
		pBudokaiState->sStateInfo.byState != BUDOKAI_STATE_CLOSE )
	{
		RegisterSideIcon( m_pBudokaiSideIconGui, CBudokaiSideIconGui, "BudokaiSideIconGui", SIDEICON_BUDOKAI );
		RegisterSideView( m_pBudokaiSideViewGui, CBudokaiSideViewGui, "BudokaiSideViewGui", SIDEVIEW_BUDOKAI );
		m_pBudokaiSideIconGui->Show( true );
	}

	// Petition (INFO BY DANEOS: CLIENT CRASHED ONCE WHEN LOGOUT WHILE PETITION WAS ENABLED)
	//RegisterSideIcon( m_pPetitionSideIconGui, CPetitionSideIconGui, "PetitionSideIconGui", SIDEICON_PETITION );
	//RegisterSideView( m_pPetitionSideViewGui, CPetitionSideViewGui, "PetitionSideViewGui", SIDEVIEW_PETITION );

	// GM Menu
	RegisterSideIcon( m_pGMSideIconGui, CGMSideIconGui, "GMSideIconGui", SIDEICON_GM );
	RegisterSideView( m_pGMSideViewGui, CGMSideViewGui, "GMSideViewGui", SIDEVIEW_GM );

	// Dojo
	RegisterSideIcon( m_pDojoSideIconGui, CDojoSideIconGui, "CDojoSideIconGui", SIDEICON_DOJO );
	RegisterSideView( m_pDojoSideViewGui, CDojoSideViewGui, "CDojoSideViewGui", SIDEVIEW_DOJO );

	// HLS
	RegisterSideIcon(m_pHLSSideIconGui, CHLSSideIconGui, "CHLSSideIconGui", SIDEICON_HLS);
	RegisterSideView(m_pHLSSideViewGui, CHLSSideViewGui, "CHLSSideViewGui", SIDEVIEW_HLS);


	SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	if( pAvatarInfo->sCharPf.bIsGameMaster )
		m_pGMSideIconGui->Show(true);

	LinkMsg(g_EventTMQCleintState);
	LinkMsg(g_EventChangeWorldConceptState);

	LinkMsg( g_EventBudokaiStateInfoNfy );
	LinkMsg( g_EventBudokaiUpdateStateNfy );

	LinkMsg(g_EventPetition);

	//test
	//LinkMsg(g_EventSCS);	

	if( GetPetitionManager()->GetPetitionID() != INVALID_PETITIONID )
	{
		CSideIconBase* pSideIcon = GetSideIcon(SIDEICON_PETITION);
		pSideIcon->Show(true);
		CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_PETITION, NULL);
	}	

	NTL_RETURN(TRUE);
}

void CSideIconGui::Destroy()
{
	NTL_FUNCTION("CSideIconGui::Destroy");

	UnRegisterSideIcon( m_pHintSideIconGui, SIDEICON_HINT );
	UnRegisterSideView( m_pHelpHintSideViewGui, SIDEVIEW_HINT_HELP );
	UnRegisterSideView( m_pGuideHintSideViewGui, SIDEVIEW_HINT_GUIDE );
	UnRegisterSideView( m_pStringHintSideViewGui, SIDEVIEW_HINT_STRING );
	UnRegisterSideIcon( m_pDBCSideIconGui, SIDEICON_DBC );
	UnRegisterSideView( m_pDBCSideViewGui, SIDEVIEW_DBC );
	UnRegisterSideIcon( m_pRBSideIconGui, SIDEICON_RB );
	UnRegisterSideView( m_pRBSideViewGui, SIDEVIEW_RB );
	UnRegisterSideIcon( m_pDoRepairSideIconGui, SIDEICON_DOREPAIR );
	UnRegisterSideView( m_pDoRepairSideViewGui, SIDEVIEW_DOREPAIR );
	UnRegisterSideIcon( m_pTutorialSideIconGui, SIDEICON_TUTORIAL );
	UnRegisterSideView( m_pTutorialSideViewGui, SIDEVIEW_TUTORIAL );
	UnRegisterSideIcon( m_pCautionSideIconGui, SIDEICON_CAUTION );
	UnRegisterSideView( m_pCautionSideViewGui, SIDEVIEW_CAUTION );
	UnRegisterSideIcon( m_pBudokaiSideIconGui, SIDEICON_BUDOKAI );
	UnRegisterSideView( m_pBudokaiSideViewGui, SIDEVIEW_BUDOKAI );
	//UnRegisterSideIcon( m_pPetitionSideIconGui, SIDEICON_PETITION );
	//UnRegisterSideView( m_pPetitionSideViewGui, SIDEVIEW_PETITION );
	UnRegisterSideIcon( m_pGMSideIconGui, SIDEICON_GM );
	UnRegisterSideView( m_pGMSideViewGui, SIDEVIEW_GM );
	UnRegisterSideIcon( m_pDojoSideIconGui, SIDEICON_DOJO );
	UnRegisterSideView( m_pDojoSideViewGui, SIDEVIEW_DOJO );
	UnRegisterSideIcon(m_pHLSSideIconGui, SIDEICON_HLS);
	UnRegisterSideView(m_pHLSSideViewGui, SIDEVIEW_HLS);

	if (m_pThis)
	{
		NTL_DELETE(m_pThis);
		m_pThis = NULL;
	}

	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventChangeWorldConceptState);

	UnLinkMsg( g_EventBudokaiStateInfoNfy );
	UnLinkMsg( g_EventBudokaiUpdateStateNfy );

	UnLinkMsg(g_EventPetition);

	//test
	UnLinkMsg(g_EventSCS);

	NTL_RETURNVOID();
}

void CSideIconGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CSideIconGui::HandleEvents");

	if( msg.Id == g_EventBudokaiStateInfoNfy )
	{
		sNtlEventBudokaiStateInfoNfy* pNotify = reinterpret_cast<sNtlEventBudokaiStateInfoNfy*>( msg.pData );

		if( m_pBudokaiSideIconGui || m_pBudokaiSideViewGui )
			return;

		if( pNotify->sStateInfo.byState != BUDOKAI_STATE_CLOSE ||
			pNotify->sStateInfo.byState != BUDOKAI_STATE_JUNIOR_CLOSE )
		{
			UnRegisterSideIcon( m_pBudokaiSideIconGui, SIDEICON_BUDOKAI);
			UnRegisterSideView( m_pBudokaiSideViewGui, SIDEVIEW_BUDOKAI );

			m_pBudokaiSideIconGui = NTL_NEW CBudokaiSideIconGui("BudokaiSideIconGui");
			if(!m_pBudokaiSideIconGui->Create())
			{
				m_pBudokaiSideIconGui->Destroy();
				NTL_DELETE(m_pBudokaiSideIconGui);
				NTL_RETURNVOID();
			}
			RegSideIcon(SIDEICON_BUDOKAI, m_pBudokaiSideIconGui);
			m_pBudokaiSideIconGui->Show( true );

			m_pBudokaiSideViewGui = NTL_NEW CBudokaiSideViewGui("BudokaiSideViewGui");
			if(!m_pBudokaiSideViewGui->Create())
			{
				m_pBudokaiSideViewGui->Destroy();
				NTL_DELETE(m_pBudokaiSideViewGui);
			}
			else
			{
				RegSideView(SIDEVIEW_BUDOKAI, m_pBudokaiSideViewGui);
			}
		}
		else
		{
			UnRegisterSideIcon( m_pBudokaiSideIconGui, SIDEICON_BUDOKAI );
			UnRegisterSideView( m_pBudokaiSideViewGui, SIDEVIEW_BUDOKAI );
		}
	}
	else if( msg.Id == g_EventBudokaiUpdateStateNfy )
	{
		sNtlEventBudokaiUpdateStateNfy* pNotify = reinterpret_cast<sNtlEventBudokaiUpdateStateNfy*>( msg.pData );

		if( pNotify->sStateInfo.byState == BUDOKAI_STATE_OPEN_NOTICE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_OPEN_NOTICE )
		{
			UnRegisterSideIcon( m_pBudokaiSideIconGui, SIDEICON_BUDOKAI);
			UnRegisterSideView( m_pBudokaiSideViewGui, SIDEVIEW_BUDOKAI );

			m_pBudokaiSideIconGui = NTL_NEW CBudokaiSideIconGui("BudokaiSideIconGui");
			if(!m_pBudokaiSideIconGui->Create())
			{
				m_pBudokaiSideIconGui->Destroy();
				NTL_DELETE(m_pBudokaiSideIconGui);
				NTL_RETURNVOID();
			}
			RegSideIcon(SIDEICON_BUDOKAI, m_pBudokaiSideIconGui);
			m_pBudokaiSideIconGui->Show( true );

			m_pBudokaiSideViewGui = NTL_NEW CBudokaiSideViewGui("BudokaiSideViewGui");
			if(!m_pBudokaiSideViewGui->Create())
			{
				m_pBudokaiSideViewGui->Destroy();
				NTL_DELETE(m_pBudokaiSideViewGui);
			}
			else
			{
				RegSideView(SIDEVIEW_BUDOKAI, m_pBudokaiSideViewGui);
			}

			m_pBudokaiSideIconGui->HandleEvents( msg );
		}
		else if( pNotify->sStateInfo.byState == BUDOKAI_STATE_CLOSE ||
			pNotify->sStateInfo.byState == BUDOKAI_STATE_JUNIOR_CLOSE )
		{
			UnRegisterSideIcon( m_pBudokaiSideIconGui, SIDEICON_BUDOKAI );
			UnRegisterSideView( m_pBudokaiSideViewGui, SIDEVIEW_BUDOKAI );
		}
	}
	else if( msg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TUTORIAL )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_TERRAIN_READY )
		{
			CSideIconBase* pSideIcon = GetSideIcon(SIDEICON_DBC);
			if( pSideIcon->IsDisplay() )
				pSideIcon->Show(false);
		}

		if( pEvent->uiState == WORLD_STATE_TMQ_PREPARE )
		{
			// TutorialSideIcon
			m_pTutorialSideIconGui = NTL_NEW CTutorialSideIconGui("TutorialSideIconGui");
			if(!m_pTutorialSideIconGui->Create())
			{
				m_pTutorialSideIconGui->Destroy();
				NTL_DELETE(m_pTutorialSideIconGui);
				NTL_RETURNVOID();
			}
			RegSideIcon(SIDEICON_TUTORIAL, m_pTutorialSideIconGui);

			m_pTutorialSideIconGui->SetNumber(dSIDEICON_PRIORITY_TUTORIAL);
			m_pTutorialSideIconGui->Show(true);

			// TutorialSideView
			m_pTutorialSideViewGui = NTL_NEW CTutorialSideViewGui("TutorialSideViewGui");
			if(!m_pTutorialSideViewGui->Create())
			{
				m_pTutorialSideViewGui->Destroy();
				NTL_DELETE(m_pTutorialSideViewGui);
			}
			else
			{
				RegSideView(SIDEVIEW_TUTORIAL, m_pTutorialSideViewGui);
			}
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TUTORIAL )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_EXIT )
		{
			UnRegisterSideIcon( m_pTutorialSideIconGui, SIDEICON_TUTORIAL );
			UnRegisterSideView( m_pTutorialSideViewGui, SIDEVIEW_TUTORIAL );

			CSideIconBase* pSideIcon = GetSideIcon(SIDEICON_DBC);
			if( !pSideIcon->IsDisplay() )
				pSideIcon->Show(true);

            // NetPy Side Icon            
           // if(GetServiceInterface()->GetPublisherID() == eDBO_SERVICE_PUBLISHER_CJI_KOR) // BY DANEOS
            {
                pSideIcon = GetSideIcon(SIDEICON_NETPY);
                if(pSideIcon && !pSideIcon->IsDisplay())
                    pSideIcon->Show(true);
            }            
		}
	}
	else if( msg.Id == g_EventPetition )
	{
		SDboEventPetition* pEvent = reinterpret_cast<SDboEventPetition*>( msg.pData );

		switch( pEvent->uiEventType )
		{
		case PETITON_REQUEST_OK:
		case PETITON_MODIFY_OK:
			{
				CSideIconBase* pSideIcon = GetSideIcon(SIDEICON_PETITION);
				pSideIcon->Show(true);
				CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_PETITION, NULL);

				break;
			}
		case PETITON_CANCEL:
		case PETITON_FINISH:
			{
				CSideIconBase* pSideIcon = GetSideIcon(SIDEICON_PETITION);
				pSideIcon->Show(false);

				break;
			}
		}
	}
	/*else if(msg.Id == g_EventSCS )
	{
		UnRegisterSideIcon( m_pSCSSideIconGui, SIDEICON_BUDOKAI);
		UnRegisterSideView( m_pSCSSideViewGui, SIDEVIEW_BUDOKAI );

		m_pSCSSideIconGui = NTL_NEW CSCSSideIconGui("CSCSSideIconGui");
		if(!m_pSCSSideIconGui->Create())
		{
			m_pSCSSideIconGui->Destroy();
			NTL_DELETE(m_pSCSSideIconGui);
			NTL_RETURNVOID();
		}
		RegSideIcon(SIDEICON_BUDOKAI, m_pSCSSideIconGui);
		m_pSCSSideIconGui->Show( true );

		m_pSCSSideViewGui = NTL_NEW CSCSSideViewGui("CSCSSideViewGui");
		if(!m_pSCSSideViewGui->Create())
		{
			m_pSCSSideViewGui->Destroy();
			NTL_DELETE(m_pSCSSideViewGui);
		}
		else
		{
			RegSideView(SIDEVIEW_BUDOKAI, m_pSCSSideViewGui);
		}
	}*/

	NTL_RETURNVOID();
}

RwInt32 CSideIconGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	for (int i = 0; i < (int)m_vecSideIcon.size(); ++i)
	{
		CSideIconBase* pSideIcon = m_vecSideIcon.at(i);
		if (bOpen)
		{
			pSideIcon->CNtlPLGui::Show(pSideIcon->IsDisplay());
		}
		else
		{
			pSideIcon->CNtlPLGui::Show(bOpen);
		}
	}

	if (m_pPresentView)
	{
		m_pPresentView->CNtlPLGui::Show(bOpen);
	}

	NTL_RETURN(TRUE);
}

void CSideIconGui::RocateSideIcon()
{
	// 정렬
	SortSideIcon();

	RwInt32 iPosX		= s_pSideIconGui->GetPosition().left;
	RwInt32 iPosY		= s_pSideIconGui->GetPosition().top;
	RwInt32 IMarginW	= dSIDEICON_MARGIN_WIDTH;
	RwInt32 IMarginH	= dSIDEICON_MARGIN_HEIGHT;

	for (SIDEICONVEC::iterator it = m_vecSideIcon.begin(); it != m_vecSideIcon.end(); ++it)
	{
		CSideIconBase* pSideIcon = *it;

		if (pSideIcon->IsDisplay())
		{
			RwInt32 iTempPosX = iPosX - pSideIcon->GetWidth() - IMarginW;
			RwInt32 iTempPosY = iPosY - pSideIcon->GetHeight() - IMarginH;

			pSideIcon->SetPosition(iTempPosX, iTempPosY);

			iPosX = iTempPosX;
		}		
	}

	if ( m_pPresentView && m_pPresentIcon )
	{
		m_pPresentView->OnSideViewLocate(m_pPresentIcon->GetPosition());
	}
}

void CSideIconGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	RocateSideIcon();
}

void CSideIconGui::RegSideIcon(const eSideIconType& eSideIcon, CSideIconBase* pSideIcon)
{
	SIDEICONMAP::iterator it = m_mapSideIcon.find(eSideIcon);
	if (it == m_mapSideIcon.end())
	{
		m_mapSideIcon.insert(SIDEICONMAP::value_type(eSideIcon, pSideIcon));
		m_vecSideIcon.push_back(pSideIcon);
	}
}

void CSideIconGui::UnRegSideIcon(const eSideIconType& eSideIcon)
{
	SIDEICONMAP::iterator it = m_mapSideIcon.find(eSideIcon);
	if (it != m_mapSideIcon.end())
	{
		CSideIconBase* pSideIcon = it->second;

		if( m_pPresentIcon == pSideIcon )
			m_pPresentIcon = NULL;

		m_vecSideIcon.erase(std::find(m_vecSideIcon.begin(), m_vecSideIcon.end(), pSideIcon));
		m_mapSideIcon.erase(it);
	}
}

CSideIconBase* CSideIconGui::GetSideIcon(const eSideIconType& eSideIcon)
{
	SIDEICONMAP::iterator it = m_mapSideIcon.find(eSideIcon);
	if (it != m_mapSideIcon.end())
	{
		return it->second;
	}
	return NULL;
}

void CSideIconGui::RegSideView(const eSideViewType& eSideView, CSideViewBase* pSideView)
{
	SIDEVIEWMAP::iterator it = m_mapSideView.find(eSideView);
	if (it == m_mapSideView.end())
	{
		m_mapSideView.insert(SIDEVIEWMAP::value_type(eSideView, pSideView));
	}
}

void CSideIconGui::UnRegSideView(const eSideViewType& eSideView)
{
	SIDEVIEWMAP::iterator it = m_mapSideView.find(eSideView);
	if (it != m_mapSideView.end())
	{
		if( m_pPresentView == it->second )
			m_pPresentView = NULL;

		m_mapSideView.erase(it);
	}
}

CSideViewBase* CSideIconGui::GetSideView(const eSideViewType& eSideView)
{
	SIDEVIEWMAP::iterator it = m_mapSideView.find(eSideView);
	if (it != m_mapSideView.end())
	{
		return it->second;
	}
	return NULL;
}

void CSideIconGui::SortSideIcon()
{
	std::sort(m_vecSideIcon.begin(), m_vecSideIcon.end(), SortSideIconCompare);
}

bool CSideIconGui::SortSideIconCompare(CSideIconBase* pLeft, CSideIconBase* pRight)
{
	return pLeft->GetNumber() < pRight->GetNumber();
}

void CSideIconGui::OnPressESC()
{
	if (m_pPresentView)
		m_pPresentView->OnPressESC();
}

void CSideIconGui::OnSideIconShow(CSideIconBase* pSideIcon, bool bShow)
{
	if (pSideIcon != NULL && pSideIcon == m_pPresentIcon)
	{
		if (!bShow && m_pPresentView)
		{
			CSideViewBase* pPreSideView = m_pPresentView;

			m_pPresentIcon = NULL;
			m_pPresentView = NULL;

			pPreSideView->OnSideViewClose();
		}
	}
	RocateSideIcon();
}

void CSideIconGui::OnSideViewShow(CSideViewBase* pSideView, bool bShow)
{
	if (pSideView != NULL && pSideView == m_pPresentView)
	{
		if (!bShow && m_pPresentIcon)
		{
			CSideIconBase* pPreSideIcon = m_pPresentIcon;			

			m_pPresentIcon = NULL;
			m_pPresentView = NULL;
			m_nPresentViewType = INVALID_SIDEVIEW;

			pPreSideIcon->OnSideViewClosed();
		}
	}
}

/**
* \brief SideIcon의 벌룬 창을 출력한다.
*
* SideView는 하나만 출력한다.
*/
void CSideIconGui::OpenSideView(CSideIconBase* pSideIcon, const eSideViewType& eSideView, void* pData)
{
	// DialogMode라면 SideView를 출력하지 않는다.
	if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		return;

	CSideViewBase* pSideView = GetSideView(eSideView);
	if (pSideIcon && pSideView)
	{
		if (m_pPresentView)
		{
			m_pPresentView->Show(false);
		}

		pSideView->OnSideViewOpen(pData);
		pSideView->OnSideViewLocate(pSideIcon->GetPosition());

		m_pPresentIcon = pSideIcon;
		m_pPresentView = pSideView;
		m_nPresentViewType = eSideView;
	}
}

void CSideIconGui::OnSideIconSend( const eSideIconType& eSideIcon, void* pData ) 
{
	CSideIconBase* pSideIcon = GetSideIcon(eSideIcon);
	if( pSideIcon )
	{
		pSideIcon->OnSideIconReceive(pData);
	}
}

void CSideIconGui::OnSideViewSend( const eSideViewType& eSideView, void* pData ) 
{
	CSideViewBase* pSideView = GetSideView(eSideView);
	if( pSideView )
	{
		pSideView->OnSideViewReceive(pData);
	}
}

void CSideIconGui::CloseSideView( const eSideViewType eSideView ) 
{
    CSideViewBase* pSideView = GetSideView(eSideView);
    if( pSideView )
    {
        pSideView->Show(false);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CSideIconBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSideIconBase::CSideIconBase(const RwChar* pName) 
: CNtlPLGui(pName)
, m_bDisplay(true)
, m_uiNumber(dSIDEICON_PRIORITY_DEFAULT)
{
}
CSideIconBase::~CSideIconBase()
{
}

void CSideIconBase::Show(bool bShow)
{
	if (m_bDisplay != (RwBool)bShow)
	{
		m_bDisplay = bShow;
		CNtlPLGui::Show(bShow);
		CSideIconGui::GetInstance()->OnSideIconShow(this, bShow);

		if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) && bShow )
		{
			CNtlPLGui::Show(false);
			CSideIconGui::GetInstance()->OnSideIconShow(this, false);
		}
	}
}

void CSideIconBase::SetNumber(const RwUInt32& uiNumber)
{
	m_uiNumber = uiNumber;
	CSideIconGui::GetInstance()->SortSideIcon();
}

RwUInt32 CSideIconBase::GetNumber()
{
	return m_uiNumber;
}

void CSideIconBase::OnSideIconReceive(void* pData) 
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CSideViewBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSideViewBase::CSideViewBase(const RwChar* pName) : CNtlPLGui(pName)
{
}

CSideViewBase::~CSideViewBase()
{
}

void CSideViewBase::Show(bool bShow)
{
	if (IsShow() != (RwBool)bShow)
	{
		// 나레이션 모드 중, SideView를 SHOW(TRUE)하라는 명령이 온다면 리턴한다.
		if( GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) && bShow)
			return;

		CNtlPLGui::Show(bShow);
		CSideIconGui::GetInstance()->OnSideViewShow(this, bShow);
	}
}

void CSideViewBase::OnSideViewReceive(void* pData ) 
{
}