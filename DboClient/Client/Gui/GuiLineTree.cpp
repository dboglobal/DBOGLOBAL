#include "precomp_dboclient.h"
#include "GuiLineTree.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGuiLineTreeNode
CGuiLineTreeNode::CGuiLineTreeNode( CGuiLineTree* pMgr, RwInt32 nID )
: m_pMgr( pMgr ), m_pParent( NULL ), m_nID( nID ), m_nLineID( 0 ), m_bIsExpand( FALSE ), m_nPosX( 0 ), m_nPosY( 0 ), m_bShow( FALSE )
{

}

CGuiLineTreeNode::~CGuiLineTreeNode(VOID)
{
	ClearChildNodes();
}

VOID CGuiLineTreeNode::AddChildNode( CGuiLineTreeNode* pNode )
{
	if( !pNode )
		return;

	m_listpChild.push_back( pNode );
	
	pNode->SetParentNode( this );
	pNode->SetPosX( m_nPosX + m_pMgr->GetChildXMargin() );

	m_pMgr->RecalcLineID();
}

VOID CGuiLineTreeNode::DeleteChildNode( RwInt32 nID )
{
	LIST_LINENODE::iterator it;

	for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
	{
		CGuiLineTreeNode* pNode = (*it);
		
		if( pNode->GetID() == nID )
		{
			NTL_DELETE( pNode );
			m_listpChild.erase( it );
			
			m_pMgr->RecalcLineID();
			
			break;
		}
	}
}

VOID CGuiLineTreeNode::ClearChildNodes(VOID)
{
	LIST_LINENODE::iterator it;

	for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
	{
		CGuiLineTreeNode* pChildNode = (*it);
		
		NTL_DELETE( pChildNode );
	}

	m_listpChild.clear();
}

CGuiLineTreeNode* CGuiLineTreeNode::FindChildNode( RwInt32 nID )
{
	LIST_LINENODE::iterator it;

	for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
	{
		CGuiLineTreeNode* pNode = (*it);
		
		if( pNode->GetID() == nID )
		{
			return pNode;
		}
		else
		{
			CGuiLineTreeNode* pNode2 = pNode->FindChildNode( nID );
			if( pNode2 )
				return pNode2;
		}
	}

	return NULL;
}

RwInt32 CGuiLineTreeNode::RecursiveSetLineID( RwInt32 nLineID )
{
	m_nLineID = nLineID++;

	if( m_nLineID > 0 && m_nLineID <= m_pMgr->GetMaxLine() )
		this->ShowProc();
	else
		this->HideProc();

	if( IsExpand() )
	{
		LIST_LINENODE::iterator it;

		for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
		{
			CGuiLineTreeNode* pNode = (*it);
			
			nLineID = pNode->RecursiveSetLineID( nLineID );
		}		
	}
	else
	{
		RecursiveChildHide();
	}

	// Return Value : 다음 행의 LineID
	return nLineID;	
}

VOID CGuiLineTreeNode::RecursiveChildHide(VOID)
{
	LIST_LINENODE::iterator it;

	for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
	{
		CGuiLineTreeNode* pNode = (*it);

		pNode->RecursiveChildHide();
		pNode->HideProc();
	}
}

VOID CGuiLineTreeNode::ShowProc(VOID)
{
	m_bShow = TRUE;
	m_nPosY = ( m_nLineID - 1 ) * ( m_pMgr->GetLineMargin() + m_pMgr->GetLineHeight() );
}

VOID CGuiLineTreeNode::HideProc(VOID)
{
	m_bShow = FALSE;
}

VOID CGuiLineTreeNode::Expand( RwBool bExpand )
{
	m_bIsExpand = bExpand;
	m_pMgr->RecalcLineID();
}

VOID CGuiLineTreeNode::ExpandChild( RwBool bExpand )
{
	LIST_LINENODE::iterator it;

	for( it = m_listpChild.begin() ; it != m_listpChild.end() ; ++it )
	{
		CGuiLineTreeNode* pNode = (*it);
		
		pNode->Expand( bExpand );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGuiLineTree
CGuiLineTree::CGuiLineTree(VOID)
: m_pScroll( NULL ), m_pRootNode( NULL ), m_pLineTree( NULL ),
  m_nLineHeight( 0 ), m_nLineMargin( 0 ),  m_nScrollOffset( 0 ), m_nMaxLine( 0 ), m_nChildXMargin( 0 )
{

}

CGuiLineTree::~CGuiLineTree(VOID)
{
	Destroy();
}

RwBool CGuiLineTree::Create( CRectangle& rect, gui::CComponent* pParent,
							 RwInt32 nLineHeight /* = GUILINETREE_DEFAULT_HEIGHT */, 
							 RwInt32 nLineMargin /* = GUILINETREE_DEFAULT_MARGIN */, 
							 RwInt32 nChildXMargin /* = GUILINETREE_DEFAULT_CHILDMARGINX */, 
							 RwInt32 nScrollBarWidth /* = GUILINETREE_DEFAULT_SCROLLBARWIDTH  */)
{
	// Create Self Dialog
	m_pLineTree = NTL_NEW gui::CDialog( rect, pParent, GetNtlGuiManager()->GetSurfaceManager() );

	// Create Scroll Bar
	rect.SetRectWH( rect.GetWidth() - nScrollBarWidth, 0, nScrollBarWidth, rect.GetHeight() );
	m_pScroll = NTL_NEW gui::CScrollBar( rect, 0, 10, false, m_pLineTree, GetNtlGuiManager()->GetSurfaceManager() );
	m_pScroll->SetRange( 0, 0 );
	m_pScroll->Show( true );
	m_slotScrollChanged = m_pScroll->SigValueChanged().Connect( this, &CGuiLineTree::OnScrollChanged );
	m_slotScrollMoved = m_pScroll->SigSliderMoved().Connect( this, &CGuiLineTree::OnScrollChanged );

	// Data
	m_nMaxLine = ( rect.GetHeight() + 2 * nLineMargin ) / ( nLineMargin + nLineHeight );

	m_nLineHeight = nLineHeight;
	m_nLineMargin = nLineMargin;
	m_nChildXMargin = nChildXMargin;

	// Create Root Node
	m_pRootNode = NTL_NEW CGuiLineTreeNode( this, GUILINETREE_ROOTNODE_ID );
	m_pRootNode->Expand( TRUE );
	m_pRootNode->SetPosX( -nChildXMargin );

	return TRUE;
}

VOID CGuiLineTree::Destroy(VOID)
{
	NTL_DELETE( m_pScroll );

	NTL_DELETE( m_pRootNode );

	NTL_DELETE( m_pLineTree );
}

VOID CGuiLineTree::AddNode( CGuiLineTreeNode* pNode, RwInt32 nParentID /* = GUILINETREE_ROOTNODE_ID  */ )
{
	if( nParentID == GUILINETREE_ROOTNODE_ID )
	{
		m_pRootNode->AddChildNode( pNode );
	}
	else
	{
		CGuiLineTreeNode* pParentNode = FindNode( nParentID );
		
		if( pParentNode )
			pParentNode->AddChildNode( pNode );
	}
}

VOID CGuiLineTree::DeleteNode( RwInt32 nID )
{
	CGuiLineTreeNode* pNode = FindNode( nID );

	if( pNode->GetParentNode() )
		pNode->GetParentNode()->DeleteChildNode( nID );
}

VOID CGuiLineTree::ClearNodes(VOID)
{
	m_pRootNode->ClearChildNodes();
}

CGuiLineTreeNode* CGuiLineTree::FindNode( RwInt32 nID )
{
	return m_pRootNode->FindChildNode( nID );
}

VOID CGuiLineTree::OnScrollChanged( RwInt32 nNewOffset )
{
	m_nScrollOffset = nNewOffset;
	RecalcLineID();
}

VOID CGuiLineTree::RecalcLineID(VOID)
{
	RwInt32 nLineCount = m_pRootNode->RecursiveSetLineID( -m_nScrollOffset ) - 1 + m_nScrollOffset;

	if( m_nScrollOffset > 0 && nLineCount <= m_nMaxLine )
	{	
		//m_pScroll->SetRange( 0, m_nScrollOffset );
		//m_pScroll->SetValue( m_nScrollOffset );

		//if( m_nScrollOffset > 0 )
		//{
		//	m_pScroll->Show( true );
		//	m_pScroll->Raise();
		//}
		//else
		//	m_pScroll->Show( false );

		m_nScrollOffset = 0;
		m_pScroll->SetRange( 0, 0 );
		m_pScroll->SetValue( 0 );
		m_pRootNode->RecursiveSetLineID( 0 );
	}
	else
	{
		m_pScroll->SetRange( 0, nLineCount - m_nMaxLine );
		m_pScroll->SetValue( m_nScrollOffset );

		//if( nLineCount - m_nMaxLine > 0 )
		//{
		//	m_pScroll->Show( true );
		//	m_pScroll->Raise();
		//}
		//else
		//	m_pScroll->Show( false );
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



