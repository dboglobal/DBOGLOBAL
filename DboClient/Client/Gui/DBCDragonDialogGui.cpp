#include "precomp_dboclient.h"
#include "DBCDragonDialogGui.h"

#include "DboGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "TableContainer.h"
#include "TextAllTable.h"

#include "NtlPLGuiManager.h"
#include "DialogManager.h"

#define WARNNING_DIALOG_TIME 5.0f

CDBCDragonDialogGUI* CDBCDragonDialogGUI::m_pInstance = NULL;

CDBCDragonDialogGUI::CDBCDragonDialogGUI( void ) 
{
	Init();
}

CDBCDragonDialogGUI::CDBCDragonDialogGUI( const RwChar* pName ) 
: CNtlPLGui(pName)
{
	Init();
}

CDBCDragonDialogGUI::~CDBCDragonDialogGUI( void ) 
{
	Destroy();
}

VOID CDBCDragonDialogGUI::Init() 
{
	m_pDragonTextTable = NULL;	
	m_bDelete = FALSE;
}

RwBool CDBCDragonDialogGUI::Create() 
{
	NTL_FUNCTION(__FUNCTION__);

	if(!CNtlPLGui::Create("", "gui\\DBCDialog.srf", "gui\\DBCDialog.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_SDragonDialog[E_DIALOG_NORMAL].m_pPanel	= (gui::CPanel*)GetComponent("pnlDialog1");
	m_SDragonDialog[E_DIALOG_NORMAL].m_pStatic	= (gui::CStaticBox*)GetComponent("sttDialog1");
	m_SDragonDialog[E_DIALOG_WARN].m_pPanel		= (gui::CPanel*)GetComponent("pnlDialog2");
	m_SDragonDialog[E_DIALOG_WARN].m_pStatic	= (gui::CStaticBox*)GetComponent("sttDialog2");

	AdjustDialog();

	// GUI Manager에 추가한다.
	GetNtlGuiManager()->AddGui(this);	
	GetNtlGuiManager()->AddUpdateFunc(this);

	m_pDragonTextTable = API_GetTableContainer()->GetTextAllTable()->GetDragonTbl();

	Show(TRUE);

	for(int i = 0; i < DIALOG_COUNT; ++i)
	{
		m_SDragonDialog[i].m_pStatic->SetEffectMode(TE_NONE);
		m_SDragonDialog[i].Show(FALSE);
	}

	LinkMsg(g_EventChangeWorldConceptState);

	NTL_RETURN(TRUE);
}

VOID CDBCDragonDialogGUI::Destroy() 
{
	UnLinkMsg(g_EventChangeWorldConceptState);

	GetNtlGuiManager()->RemoveUpdateFunc(this);	
	GetNtlGuiManager()->RemoveGui(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}


void CDBCDragonDialogGUI::AdjustDialog() 
{
	RwInt32 x = CDboGlobal::GetInstance()->GetScreenWidth() / 2 - 460;
	RwInt32 y = CDboGlobal::GetInstance()->GetScreenHeight() / 2 - 205;
	m_SDragonDialog[E_DIALOG_NORMAL].SetPosition(x, y);

	x = CDboGlobal::GetInstance()->GetScreenWidth() / 2 - 360;
	y = CDboGlobal::GetInstance()->GetScreenHeight() /2 + 15;
	m_SDragonDialog[E_DIALOG_WARN].SetPosition(x, y);
}

VOID CDBCDragonDialogGUI::Update( RwReal fElapsed ) 
{
	if(m_bDelete)
	{
		CDBCDragonDialogGUI::DeleteInstance();
		return;
	}

	if(m_SDragonDialog[E_DIALOG_WARN].m_pPanel->IsVisible())
	{
		m_SDragonDialog[E_DIALOG_WARN].m_fLifeTime -= fElapsed;

		if(m_SDragonDialog[E_DIALOG_WARN].m_fLifeTime <= WARNNING_DIALOG_TIME * 0.5f)
		{
			RwReal fAlpha = m_SDragonDialog[E_DIALOG_WARN].m_fLifeTime / (WARNNING_DIALOG_TIME * 0.5f);
			m_SDragonDialog[E_DIALOG_WARN].SetAlpha(fAlpha);
		}
		
		if(m_SDragonDialog[E_DIALOG_WARN].m_fLifeTime <= 0.0f)
		{
			m_SDragonDialog[E_DIALOG_WARN].Show(FALSE);
		}		
	}
}

VOID CDBCDragonDialogGUI::SetText(RwInt32 nTblIdx, EDialogType eDialogType /* = E_DIALOG_FIRST */)
{
	SetText(m_pDragonTextTable->GetText(nTblIdx), eDialogType);
}

VOID CDBCDragonDialogGUI::SetText(std::wstring& strWString, EDialogType eDialogType /* = E_DIALOG_FIRST */)
{
	// 시간을 설정하지만 실제로는 경고 대사창만 시간이 경과한 후에 사라진다.
	m_SDragonDialog[eDialogType].m_fLifeTime = WARNNING_DIALOG_TIME;	
	m_SDragonDialog[eDialogType].m_pStatic->SetText(strWString.c_str());
	m_SDragonDialog[eDialogType].Show(TRUE);
}

CDBCDragonDialogGUI* CDBCDragonDialogGUI::GetInstance() 
{
	if(!m_pInstance)
	{
		m_pInstance = NTL_NEW CDBCDragonDialogGUI("DBCDragonDialog");
		m_pInstance->Create();
	}

	return m_pInstance;
}

void CDBCDragonDialogGUI::DeleteInstance() 
{
    if(m_pInstance)
    {
	    m_pInstance->Destroy();
	    NTL_DELETE(m_pInstance);
    }
}

VOID CDBCDragonDialogGUI::HandleEvents( RWS::CMsg &pMsg ) 
{
	// World Concept이 끝날때 삭제한다.
	if(pMsg.Id == g_EventChangeWorldConceptState)
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( pMsg.pData );
		if( pEvent->eWorldConcept == WORLD_PLAY_DRAGONBALL_COLLECT && pEvent->uiState == WORLD_STATE_NONE )
		{
			m_bDelete = TRUE;
		}
	}
}

VOID CDBCDragonDialogGUI::ShowNormalDialog( RwBool bShow ) 
{
	m_SDragonDialog[E_DIALOG_NORMAL].Show(bShow);
}