// CInstancePropertyDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CEntity.h"
#include "WorldEditor.h"
#include "CEntityPropertyTree.h"
#include "CAtomicEntityPropertyTree.h"
#include "CClumpEntityPropertyTree.h"
#include "CLightEntityPropertyTree.h"
#include "CCameraEntityPropertyTree.h"
#include "CAnimationEntityPropertyTree.h"

#include "CInstancePropertyDialog.h"


// CInstancePropertyDialog 대화 상자입니다.

BEGIN_MESSAGE_MAP(CInstancePropertyDialog, CDialog)
	//{{AFX_MSG_MAP(CDlgDlg)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTYTREE, OnItemChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CInstancePropertyDialog, CDialog)
CInstancePropertyDialog::CInstancePropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInstancePropertyDialog::IDD, pParent)
{
	m_pPropertyTree		= NULL;
}

CInstancePropertyDialog::~CInstancePropertyDialog()
{
	if( m_pPropertyTree != NULL )
	{
		delete m_pPropertyTree;
	}
}

void CInstancePropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BOOL CInstancePropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetEntity( NULL );

	return TRUE;
}


void CInstancePropertyDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( m_pPropertyTree == NULL )
	{
		return;
	}

	// resize the control to always fit the dialog
	if( ::IsWindow( m_pPropertyTree->GetSafeHwnd() ) )
	{
		m_pPropertyTree->SetWindowPos(NULL, -1, -1, cx, cy, 0);
	}
}


BOOL CInstancePropertyDialog::SetEntity( CEntity * pEntity )
{
	if( m_pPropertyTree != NULL )
	{
		delete m_pPropertyTree;
		m_pPropertyTree = NULL;
	}

	if( pEntity == NULL )
	{
		return TRUE;
	}

	switch( pEntity->GetType() )
	{
		case E_ENTITY_TYPE_ATOMIC:
		{
			m_pPropertyTree = new CAtomicEntityPropertyTree;

			break;
		}

		case E_ENTITY_TYPE_CLUMP:
		{
			m_pPropertyTree = new CClumpEntityPropertyTree;

			break;
		}

		case E_ENTITY_TYPE_LIGHT:
		{
			m_pPropertyTree = new CLightEntityPropertyTree;

			break;
		}

		case E_ENTITY_TYPE_CAMERA:
		{
			m_pPropertyTree = new CCameraEntityPropertyTree;

			break;
		}

		case E_ENTITY_TYPE_ANIMATION:
		{
			m_pPropertyTree = new CAnimationEntityPropertyTree;

			break;
		}

		default:
		{
			m_pPropertyTree = new CEntityPropertyTree;
		}
	}

	if( m_pPropertyTree == NULL )
	{
		return FALSE;
	}

	return m_pPropertyTree->Create( this, pEntity );
}

BOOL CInstancePropertyDialog::UpdateEntity( CEntity * pEntity )
{
	if( m_pPropertyTree != NULL )
	{
		return m_pPropertyTree->Update( pEntity );
	}
	else
	{
		return FALSE;
	}
}

void CInstancePropertyDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;

	if (pNMPropTree->pItem)
	{
		m_pPropertyTree->Apply( pNMPropTree->pItem );
	}

	*plResult = 0;
}

// CInstancePropertyDialog 메시지 처리기입니다.
