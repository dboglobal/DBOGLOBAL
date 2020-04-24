#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "TSView.h"
#include "TSFrame.h"
#include "SCtrl_Select.h"
#include "SCtrl_Link.h"
#include "SCtrl_Link_Def.h"
#include "SCtrl_Link_Yes.h"
#include "SCtrl_Link_No.h"
#include "SCtrl_Link_Error.h"
#include "SCtrl_Link_LogIn.h"
#include "SCtrl_End.h"
#include "SCtrl_GAct.h"
#include "SCtrl_GCond.h"
#include "SCtrl_Narration.h"
#include "SCtrl_Proposal.h"
#include "SCtrl_Reward.h"
#include "SCtrl_Start.h"
#include "SCtrl_UsrSel.h"
#include "SCtrl_Switch.h"
#include "SCtrl_UnifiedNarration.h"
#include "SCtrl_Note.h"
#include "TSProjectMainMenuForm.h"
#include "MobGroupIdxChanger.h"
#include "FinderDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTSView* g_pTSView = NULL;


//////////////////////////////////////////////////////////////////////////
//
//	CTSView
//
//////////////////////////////////////////////////////////////////////////


CLIPFORMAT CTSView::s_clClipFmt = ::RegisterClipboardFormat( "TS" );


IMPLEMENT_DYNCREATE( CTSView, CScrollView )

BEGIN_MESSAGE_MAP(CTSView, CScrollView)
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	//{{AFX_MSG_MAP(CTSView)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CREATE_PROJECT, &CTSView::OnCreateProject)
	ON_COMMAND(ID_OPEN_PROJECT, &CTSView::OnOpenProject)
	ON_COMMAND(ID_SAVE_PROJECT, &CTSView::OnSaveProject)
	ON_UPDATE_COMMAND_UI(ID_SAVE_PROJECT, &CTSView::OnUpdateSaveProject)
	ON_COMMAND(ID_CLOSE_PROJECT, &CTSView::OnCloseProject)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_PROJECT, &CTSView::OnUpdateCloseProject)
	ON_COMMAND(ID_EXPORT_TREEGER, &CTSView::OnExportTrigger)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TREEGER, &CTSView::OnUpdateExportTrigger)
	ON_COMMAND(ID_EXPORT_TREEGER_ALL, &CTSView::OnExportTriggerAll)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_TREEGER_ALL, &CTSView::OnUpdateExportTriggerAll)
	ON_COMMAND(ID_ENCRYTION, &CTSView::OnEncryptionTrigger)
	ON_UPDATE_COMMAND_UI(ID_ENCRYTION, &CTSView::OnUpdateEncryptionTrigger)
	ON_COMMAND(ID_EXPORT_EVENTOR, &CTSView::OnExportEventor)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_EVENTOR, &CTSView::OnUpdateExportEventor)
	ON_COMMAND(ID_CONT_SELECT, &CTSView::OnContSelect)
	ON_UPDATE_COMMAND_UI(ID_CONT_SELECT, &CTSView::OnUpdateContSelect)
	ON_COMMAND(ID_CONT_LINK_DEF, &CTSView::OnContLinkDef)
	ON_UPDATE_COMMAND_UI(ID_CONT_LINK_DEF, &CTSView::OnUpdateContLinkDef)
	ON_COMMAND(ID_CONT_LINK_YES, &CTSView::OnContLinkYes)
	ON_UPDATE_COMMAND_UI(ID_CONT_LINK_YES, &CTSView::OnUpdateContLinkYes)
	ON_COMMAND(ID_CONT_LINK_NO, &CTSView::OnContLinkNo)
	ON_UPDATE_COMMAND_UI(ID_CONT_LINK_NO, &CTSView::OnUpdateContLinkNo)
	ON_COMMAND(ID_CONT_LINK_ERROR, &CTSView::OnContLinkError)
	ON_UPDATE_COMMAND_UI(ID_CONT_LINK_ERROR, &CTSView::OnUpdateContLinkError)
	ON_COMMAND(ID_CONT_LINK_LOGIN, &CTSView::OnContLinkLogIn)
	ON_UPDATE_COMMAND_UI(ID_CONT_LINK_LOGIN, &CTSView::OnUpdateContLinkLogIn)
	ON_COMMAND(ID_CONT_START, &CTSView::OnContStart)
	ON_UPDATE_COMMAND_UI(ID_CONT_START, &CTSView::OnUpdateContStart)
	ON_COMMAND(ID_CONT_END, &CTSView::OnContEnd)
	ON_UPDATE_COMMAND_UI(ID_CONT_END, &CTSView::OnUpdateContEnd)
	ON_COMMAND(ID_CONT_GCOND, &CTSView::OnContGCond)
	ON_UPDATE_COMMAND_UI(ID_CONT_GCOND, &CTSView::OnUpdateContGCond)
	ON_COMMAND(ID_CONT_GACT, &CTSView::OnContGAct)
	ON_UPDATE_COMMAND_UI(ID_CONT_GACT, &CTSView::OnUpdateContGAct)
	ON_COMMAND(ID_CONT_PROPOSAL, &CTSView::OnContProposal)
	ON_UPDATE_COMMAND_UI(ID_CONT_PROPOSAL, &CTSView::OnUpdateContProposal)
	ON_COMMAND(ID_CONT_USRSEL, &CTSView::OnContUsrSel)
	ON_UPDATE_COMMAND_UI(ID_CONT_USRSEL, &CTSView::OnUpdateContUsrSel)
	ON_COMMAND(ID_CONT_NARRATION, &CTSView::OnContNarration)
	ON_UPDATE_COMMAND_UI(ID_CONT_NARRATION, &CTSView::OnUpdateContNarration)
	ON_COMMAND(ID_CONT_REWARD, &CTSView::OnContReward)
	ON_UPDATE_COMMAND_UI(ID_CONT_REWARD, &CTSView::OnUpdateContReward)
	ON_COMMAND(ID_CONT_SWITCH, &CTSView::OnContSwitch)
	ON_UPDATE_COMMAND_UI(ID_CONT_SWITCH, &CTSView::OnUpdateContSwitch)
	ON_COMMAND(ID_EDIT_COPY, &CTSView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CTSView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CTSView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CTSView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, &CTSView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CTSView::OnUpdateEditCut)
	ON_COMMAND(ID_CONT_UNIFIED_NARRATION, &CTSView::OnContUnifiedNarration)
	ON_UPDATE_COMMAND_UI(ID_CONT_UNIFIED_NARRATION, &CTSView::OnUpdateContUnifiedNarration)
	ON_COMMAND(ID_CONT_NOTE, &CTSView::OnContNote)
	ON_UPDATE_COMMAND_UI(ID_CONT_NOTE, &CTSView::OnUpdateNote)
	ON_COMMAND(ID_CHANGE_MOB_GROUP_INDEX, &CTSView::OnChangeMobGroupIndex)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MOB_GROUP_INDEX, &CTSView::OnUpdateChangeMobGroupIndex)
	ON_COMMAND(ID_FINDER, &CTSView::OnFinder)
	ON_UPDATE_COMMAND_UI(ID_FINDER, &CTSView::OnUpdateFinder)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


CTSView::CTSView( void )
: c_dMinScale( 0.26 ),
  c_dNormalScale( 1.0 ),
  c_dMaxScale( 1.5 )
{
	g_pTSView = this;

	m_dScale = c_dNormalScale;

	m_bGrid = true;

	m_pSCtrlSelect = new CSCtrl_Select( this );
	m_pSCtrlLinkDef = new CSCtrl_Link_Def( this );
	m_pSCtrlLinkYes = new CSCtrl_Link_Yes( this );
	m_pSCtrlLinkNo = new CSCtrl_Link_No( this );
	m_pSCtrlLinkError = new CSCtrl_Link_Error( this );
	m_pSCtrlLinkLogIn = new CSCtrl_Link_LogIn( this );
	m_pSCtrlEnd = new CSCtrl_End( this );
	m_pSCtrlGAct = new CSCtrl_GAct( this );
	m_pSCtrlGCond = new CSCtrl_GCond( this );
	m_pSCtrlNarration = new CSCtrl_Narration( this );
	m_pSCtrlProposal = new CSCtrl_Proposal( this );
	m_pSCtrlReward = new CSCtrl_Reward( this );
	m_pSCtrlStart = new CSCtrl_Start( this );
	m_pSCtrlUsrSel = new CSCtrl_UsrSel( this );
	m_pSCtrlSwitch = new CSCtrl_Switch( this );
	m_pSCtrlUnifiedNarration = new CSCtrl_UnifiedNarration( this );
	m_pSCtrlNote = new CSCtrl_Note( this );

	m_pSCtrlCurrent = m_pSCtrlSelect;

	m_pEditCtrl = NULL;
	m_pEditString = NULL;
}

CTSView::~CTSView( void )
{
	g_pTSView = NULL;

	delete m_pSCtrlSelect;
	delete m_pSCtrlLinkDef;
	delete m_pSCtrlLinkYes;
	delete m_pSCtrlLinkNo;
	delete m_pSCtrlLinkError;
	delete m_pSCtrlLinkLogIn;
	delete m_pSCtrlEnd;
	delete m_pSCtrlGAct;
	delete m_pSCtrlGCond;
	delete m_pSCtrlNarration;
	delete m_pSCtrlProposal;
	delete m_pSCtrlReward;
	delete m_pSCtrlStart;
	delete m_pSCtrlUsrSel;
	delete m_pSCtrlSwitch;
	delete m_pSCtrlUnifiedNarration;
	delete m_pSCtrlNote;
}

BOOL CTSView::PreCreateWindow( CREATESTRUCT& cs )
{
	return CScrollView::PreCreateWindow( cs );
}

void CTSView::OnDraw( CDC* pDC )
{
	CTSDoc* pDoc = GetDocument();
	ASSERT_VALID( pDoc );
	if ( !pDoc ) return;

	CDC OffScreenDC, *pDrawDC = pDC;
	CBitmap OffscreenBitmap, *pOldBitmap = NULL;

	CRect rtDocClipBox;
	pDC->GetClipBox( rtDocClipBox );
	rtDocClipBox.InflateRect( 1, 1 );
	CRect rtDeviceClipBox = Doc2Device( rtDocClipBox );

	if ( !pDC->IsPrinting() )
	{
		VERIFY( OffScreenDC.CreateCompatibleDC( pDC ) );
		VERIFY( OffscreenBitmap.CreateCompatibleBitmap( pDC, rtDeviceClipBox.Width(), rtDeviceClipBox.Height() ) );
		pDrawDC = &OffScreenDC;
		OnPrepareDC( pDrawDC, NULL );

		OffScreenDC.OffsetViewportOrg( -rtDeviceClipBox.left, -rtDeviceClipBox.top );

		pOldBitmap = OffScreenDC.SelectObject( &OffscreenBitmap );

		OffScreenDC.SetBrushOrg( rtDeviceClipBox.left % 8, rtDeviceClipBox.top % 8 );
		OffScreenDC.IntersectClipRect( rtDocClipBox );
	}

	if ( !pDC->IsPrinting() )
	{
		pDrawDC->FillSolidRect( rtDocClipBox, pDoc->GetTheme().GetGridBackgroundColor() );
	}

	if ( !pDC->IsPrinting() && m_bGrid )
	{
		RenderGrid( pDrawDC );
	}

	pDoc->Render( pDrawDC );

	RenderSelection( pDrawDC );

	if ( pDrawDC != pDC )
	{
		pDC->SetViewportOrg( 0, 0 );
		pDC->SetWindowOrg( 0, 0 );
		pDC->SetMapMode( MM_TEXT );
		OffScreenDC.SetViewportOrg( 0, 0 );
		OffScreenDC.SetWindowOrg( 0, 0 );
		OffScreenDC.SetMapMode( MM_TEXT );
		pDC->BitBlt( rtDeviceClipBox.left, rtDeviceClipBox.top, rtDeviceClipBox.Width(), rtDeviceClipBox.Height(), &OffScreenDC, 0, 0, SRCCOPY );
		OffScreenDC.SelectObject( pOldBitmap );
	}
}

void CTSView::OnPrepareDC( CDC* pDC, CPrintInfo* pInfo /*= NULL*/ )
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	CSize LogicalExtent, DeviceExtent;
	LogicalExtent.cx = GetDocument()->GetExtentX();
	LogicalExtent.cy = GetDocument()->GetExtentY();
	DeviceExtent.cx = (int)(GetDocument()->GetExtentX() * m_dScale );
	DeviceExtent.cy = (int)(GetDocument()->GetExtentY() * m_dScale );

	pDC->SetMapMode( MM_ANISOTROPIC );
	pDC->SetWindowExt( LogicalExtent );
	pDC->SetViewportExt( DeviceExtent );
}

void CTSView::OnInitialUpdate( void )
{
	CScrollView::OnInitialUpdate();

	m_GridX = GetDocument()->GetTheme().GetGridX();
	m_GridY = GetDocument()->GetTheme().GetGridY();

	SetScale( m_dScale );
}

BOOL CTSView::OnPreparePrinting( CPrintInfo* pInfo )
{
	return DoPreparePrinting( pInfo );
}

void CTSView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}

void CTSView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}

void CTSView::OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView )
{
	CScrollView::OnActivateView( bActivate, pActivateView, pDeactiveView );

	EndEditText();
	m_pSCtrlCurrent->OnDeactivate();
	m_pSCtrlCurrent = m_pSCtrlSelect;
}

void CTSView::SetScale( double dScale )
{
	CString PercentSign;
	CString ScaleText;
	CTSFrame* pMainWnd;

	ASSERT( dScale > 0 );

	pMainWnd = DYNAMIC_DOWNCAST( CTSFrame, AfxGetApp()->GetMainWnd() );
	ASSERT( pMainWnd != NULL );

	m_dScale = dScale;

	CSize size;
	size.cx = (int)(GetDocument()->GetExtentX() * m_dScale);
	size.cy = (int)(GetDocument()->GetExtentY() * m_dScale);

	SetScrollSizes( MM_TEXT, size );
	Invalidate();
}

void CTSView::RenderGrid( CDC *pDC ) const
{
	COLORREF GridColor = GetDocument()->GetTheme().GetGridColor();

	CRect ClipBox;
	pDC->GetClipBox( ClipBox );
	ClipBox.NormalizeRect();
	ClipBox.InflateRect( m_GridX, m_GridY );

	long sx = ClipBox.left / m_GridX * m_GridX;
	long sy = ClipBox.top / m_GridY * m_GridY;

	for ( int y = sy; y < ClipBox.bottom; y += m_GridY )
	{
		for ( int x = sx; x < ClipBox.right; x += m_GridX )
		{
			pDC->SetPixelV( x, y, GridColor );
		}
	}
}

void CTSView::RenderSelection( CDC *pDC ) const
{
	for ( POSITION pos = m_Selection.GetHeadPosition(); pos != NULL; )
	{
		m_Selection.GetNext( pos )->RenderTracker( pDC );
	}
}

CPoint CTSView::Device2Doc( const CPoint &p )
{
	CPoint point = p;
	CClientDC dc( this );
	OnPrepareDC( &dc, NULL );
	dc.DPtoLP( &point );
	return point;
}

CRect CTSView::Device2Doc( const CRect &r )
{
	CRect rect = r;
	CClientDC dc( this );
	OnPrepareDC( &dc, NULL );
	dc.DPtoLP( &rect );
	rect.NormalizeRect();
	return rect;
}

CPoint CTSView::Doc2Device( const CPoint &p )
{
	CPoint point = p;
	CClientDC dc( this );
	OnPrepareDC( &dc, NULL );
	dc.LPtoDP( &point );
	return point;
}

CRect CTSView::Doc2Device( const CRect &r )
{
	CRect rect = r;
	CClientDC dc( this );
	OnPrepareDC( &dc, NULL );
	dc.LPtoDP( &rect );
	rect.NormalizeRect();
	return rect;
}

CPoint CTSView::Align2Grid( const CPoint &p ) const
{
	CPoint ptGridPoint( p.x / m_GridX * m_GridX, p.y / m_GridY * m_GridY );

	if ( p.x % m_GridX > m_GridX / 2 ) ptGridPoint.x += m_GridX;
	if ( p.y % m_GridY > m_GridY / 2 ) ptGridPoint.y += m_GridY;

	return m_bGrid ? ptGridPoint : p;
}

CRect CTSView::Align2Grid( const CRect &r ) const
{
	return CRect( Align2Grid(r.TopLeft()), Align2Grid(r.BottomRight()) );
}

bool CTSView::IsSelected( CShape* pShape ) const
{
	ASSERT( pShape );
	return m_Selection.Find( pShape ) != NULL ? true : false;
}

void CTSView::SelectShape( CShape* pShape, bool bAdd /*= false*/ )
{
	ASSERT( pShape );

	if ( !bAdd ) ClearSelections();
	if ( IsSelected( pShape ) ) return;

	m_Selection.AddTail( pShape );

	Invalidate();
	UpdateWindow();
}

void CTSView::SelectShape( const CRect& rect, bool bAdd /*= false*/ )
{
	if ( !bAdd ) ClearSelections();

	GetDocument()->GetShapesInRect( rect, m_Selection );

	Invalidate();
	UpdateWindow();
}

void CTSView::UnselectShape( CShape* pShape )
{
	ASSERT( pShape );

	if ( !IsSelected( pShape ) ) return;

	m_Selection.RemoveAt( m_Selection.Find( pShape ) );

	Invalidate();
	UpdateWindow();
}

void CTSView::ClearSelections( void )
{
	m_Selection.RemoveAll();

	Invalidate();
	UpdateWindow();
}

void CTSView::BeginEditText( CString &text, const CRect &rect, CFont *pRefFont, DWORD dwStyle /*= 0*/ )
{
	EndEditText();

	m_pEditCtrl = new CEdit();
	m_pEditString = &text;

	m_pEditCtrl->Create( WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | dwStyle, Doc2Device(rect), this, 1 );
	m_pEditCtrl->SetFont( GetDocument()->GetTheme().EditFont( pRefFont, m_dScale ) );
	m_pEditCtrl->SetWindowText( text );
	m_pEditCtrl->SetFocus();
}

void CTSView::EndEditText( void )
{
	if ( m_pEditCtrl )
	{
		m_pEditCtrl->GetWindowText( *m_pEditString );
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;

		GetDocument()->OnShapeChange();
	}
}

#ifdef _DEBUG
void CTSView::AssertValid( void ) const
{
	CScrollView::AssertValid();
}

void CTSView::Dump( CDumpContext& dc ) const
{
	CScrollView::Dump( dc );
}

CTSDoc* CTSView::GetDocument( void ) const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS(CTSDoc) ) );
	return (CTSDoc*)m_pDocument;
}
#endif //_DEBUG

void CTSView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 )
	{
		if ( '1' == nChar ) SendMessage( WM_COMMAND, ID_CONT_LINK_DEF );
		if ( '2' == nChar ) SendMessage( WM_COMMAND, ID_CONT_LINK_YES );
		if ( '3' == nChar ) SendMessage( WM_COMMAND, ID_CONT_LINK_NO );
		if ( '4' == nChar ) SendMessage( WM_COMMAND, ID_CONT_LINK_ERROR );
		if ( '5' == nChar ) SendMessage( WM_COMMAND, ID_CONT_LINK_LOGIN );
	}
	else
	{
		if ( '1' == nChar ) SendMessage( WM_COMMAND, ID_CONT_START );
		if ( '2' == nChar ) SendMessage( WM_COMMAND, ID_CONT_END );
		if ( '3' == nChar ) SendMessage( WM_COMMAND, ID_CONT_GCOND );
		if ( '4' == nChar ) SendMessage( WM_COMMAND, ID_CONT_GACT );
		if ( '5' == nChar ) SendMessage( WM_COMMAND, ID_CONT_SWITCH );
		if ( '6' == nChar ) SendMessage( WM_COMMAND, ID_CONT_PROPOSAL );
		if ( '7' == nChar ) SendMessage( WM_COMMAND, ID_CONT_USRSEL );
		if ( '8' == nChar ) SendMessage( WM_COMMAND, ID_CONT_REWARD );
		if ( '9' == nChar ) SendMessage( WM_COMMAND, ID_CONT_NARRATION );
		if ( '0' == nChar ) SendMessage( WM_COMMAND, ID_CONT_UNIFIED_NARRATION );
	}

	if ( VK_DELETE == nChar && !m_Selection.IsEmpty() )
	{
		CString strMessage;
		strMessage.Format( _T("%d개의 객체가 선택되었습니다.\n정말로 지우시겠습니까?"), m_Selection.GetSize() );
		int nRet = AfxMessageBox( strMessage, MB_OKCANCEL );
		if ( IDOK == nRet )
		{
			while ( !m_Selection.IsEmpty() )
			{
				GetDocument()->RemoveShape( m_Selection.RemoveTail() );
			}
		}
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTSView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_pSCtrlCurrent->OnDoubleClick( Device2Doc( point ), nFlags );
}

void CTSView::OnLButtonDown(UINT nFlags, CPoint point)
{
	EndEditText();

	m_pSCtrlCurrent->OnLButtonDown( Device2Doc( point ), nFlags );

	SetCapture();
}

void CTSView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_pSCtrlCurrent->OnLButtonUp( Device2Doc( point ), nFlags );

	m_pSCtrlCurrent = m_pSCtrlSelect;
}

void CTSView::OnRButtonDown(UINT nFlags, CPoint point)
{
	EndEditText();
}

void CTSView::OnMouseMove(UINT nFlags, CPoint point)
{
	m_pSCtrlCurrent->OnMouseMove( Device2Doc( point ), nFlags );
}

BOOL CTSView::OnEraseBkgnd(CDC* pDC)
{
	// 배경을 다시 그리지 않는다
	return TRUE;
}

void CTSView::OnCreateProject( void )
{
	GetDocument()->OnCreateProject();
}

void CTSView::OnOpenProject( void )
{
	GetDocument()->OnOpenProject();
}

void CTSView::OnSaveProject( void )
{
	GetDocument()->OnSaveProject();
}

void CTSView::OnUpdateSaveProject( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); }
}

void CTSView::OnCloseProject( void )
{
	GetDocument()->OnCloseProject();
}

void CTSView::OnUpdateCloseProject( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); }
}

void CTSView::OnExportTrigger( void )
{
	GetDocument()->OnExportTrigger();
}

void CTSView::OnUpdateExportTrigger( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); }
	if ( g_pProjMainMenuForm->m_ctrTriggerList.GetSelectedCount() == 0 ) { pCmdUI->Enable( FALSE ); }
}

void CTSView::OnExportTriggerAll( void )
{
	GetDocument()->OnExportTriggerAll();
}

void CTSView::OnUpdateExportTriggerAll( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); }
}

void CTSView::OnEncryptionTrigger( void )
{
	GetDocument()->OnEncryptionTrigger();
}

void CTSView::OnUpdateEncryptionTrigger( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( TRUE );
}

void CTSView::OnExportEventor()
{
	GetDocument()->OnExportEventor();
}

void CTSView::OnUpdateExportEventor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( TRUE );
}

void CTSView::OnContSelect( void )
{
	m_pSCtrlCurrent = m_pSCtrlSelect;
}

void CTSView::OnUpdateContSelect( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlSelect );
}

void CTSView::OnContLinkDef( void )
{
	m_pSCtrlCurrent = m_pSCtrlLinkDef;
}

void CTSView::OnUpdateContLinkDef( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlLinkDef );
}

void CTSView::OnContLinkYes( void )
{
	m_pSCtrlCurrent = m_pSCtrlLinkYes;
}

void CTSView::OnUpdateContLinkYes( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlLinkYes );
}

void CTSView::OnContLinkNo( void )
{
	m_pSCtrlCurrent = m_pSCtrlLinkNo;
}

void CTSView::OnUpdateContLinkNo( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlLinkNo );
}

void CTSView::OnContLinkError( void )
{
	m_pSCtrlCurrent = m_pSCtrlLinkError;
}

void CTSView::OnUpdateContLinkError( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlLinkError );
}

void CTSView::OnContLinkLogIn( void )
{
	m_pSCtrlCurrent = m_pSCtrlLinkLogIn;
}

void CTSView::OnUpdateContLinkLogIn(CCmdUI *pCmdUI)
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlLinkLogIn );
}

void CTSView::OnContStart( void )
{
	m_pSCtrlCurrent = m_pSCtrlStart;
}

void CTSView::OnUpdateContStart( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlStart );
}

void CTSView::OnContEnd( void )
{
	m_pSCtrlCurrent = m_pSCtrlEnd;
}

void CTSView::OnUpdateContEnd( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlEnd );
}

void CTSView::OnContGCond( void )
{
	m_pSCtrlCurrent = m_pSCtrlGCond;
}

void CTSView::OnUpdateContGCond( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlGCond );
}

void CTSView::OnContGAct( void )
{
	m_pSCtrlCurrent = m_pSCtrlGAct;
}

void CTSView::OnUpdateContGAct( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlGAct );
}

void CTSView::OnContProposal( void )
{
	m_pSCtrlCurrent = m_pSCtrlProposal;
}

void CTSView::OnUpdateContProposal( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlProposal );
}

void CTSView::OnContUsrSel( void )
{
	m_pSCtrlCurrent = m_pSCtrlUsrSel;
}

void CTSView::OnUpdateContUsrSel( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlUsrSel );
}

void CTSView::OnContNarration( void )
{
	m_pSCtrlCurrent = m_pSCtrlNarration;
}

void CTSView::OnUpdateContNarration( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlNarration );
}

void CTSView::OnContReward( void )
{
	m_pSCtrlCurrent = m_pSCtrlReward;
}

void CTSView::OnUpdateContReward( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlReward );
}

void CTSView::OnContSwitch( void )
{
	m_pSCtrlCurrent = m_pSCtrlSwitch;
}

void CTSView::OnUpdateContSwitch( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlSwitch );
}

void CTSView::OnContUnifiedNarration( void )
{
	m_pSCtrlCurrent = m_pSCtrlUnifiedNarration;
}

void CTSView::OnUpdateContUnifiedNarration( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlUnifiedNarration );
}

void CTSView::OnContNote( void )
{
	m_pSCtrlCurrent = m_pSCtrlNote;
}

void CTSView::OnUpdateNote(CCmdUI *pCmdUI)
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
	pCmdUI->SetRadio( m_pSCtrlCurrent == m_pSCtrlNote );
}

void CTSView::OnChangeMobGroupIndex( void )
{
	if ( CTSDoc::GetInstance()->IsModified() ) CTSDoc::GetInstance()->DoFileSave();

	CTSProject* pTSProj = CTSDoc::GetInstance()->GetProjectObject();

	CArray<CString, const CString&>& arTrigList = pTSProj->GetTriggerList();

	for ( int i = 0; i < arTrigList.GetCount(); ++i )
	{
		CString strPath = pTSProj->GetProjectPath();
		strPath += _T("\\");
		strPath += arTrigList[i];

		CTSDoc::GetInstance()->SetPathName( strPath );
		CTSDoc::GetInstance()->OnOpenDocument( strPath );

		CMobGroupIdxChanger clChanger( &pTSProj->GetTrigger() );

		clChanger.Run();

		CTSDoc::GetInstance()->DoFileSave();
	}
}

void CTSView::OnUpdateChangeMobGroupIndex( CCmdUI *pCmdUI )
{
	if ( !GetDocument()->IsProjectLoaded() ) { pCmdUI->Enable( FALSE ); return; }
}

void CTSView::OnEditCopy( void )
{
	CSharedFile clShareFile;
	CArchive ar( &clShareFile, CArchive::store );
	m_Selection.Serialize( ar );
	ar.Close();

	CMetaFileDC mfDC;
	mfDC.CreateEnhanced( NULL, NULL, NULL, NULL );

	CSize logicalExtent, deviceExtent;
	logicalExtent.cx = GetDocument()->GetExtentX();
	logicalExtent.cy = GetDocument()->GetExtentY();
	deviceExtent.cx = (int)(GetDocument()->GetExtentX() * 0.65f);
	deviceExtent.cy = (int)(GetDocument()->GetExtentY() * 0.65f);
	mfDC.SetMapMode( MM_ANISOTROPIC );
	mfDC.SetWindowExt( logicalExtent );
	mfDC.SetViewportExt( deviceExtent );

	for ( POSITION pos = m_Selection.GetTailPosition(); pos != NULL; )
	{
		m_Selection.GetPrev( pos )->Render( &mfDC );
	}

	STGMEDIUM stgMedium;
	stgMedium.hEnhMetaFile = mfDC.CloseEnhanced();
	stgMedium.tymed = TYMED_ENHMF;
	stgMedium.pUnkForRelease = NULL;

	COleDataSource* pDataSource = new COleDataSource;
	pDataSource->CacheGlobalData( s_clClipFmt, clShareFile.Detach() );
	pDataSource->CacheData( CF_ENHMETAFILE, &stgMedium );
	pDataSource->SetClipboard();
}

void CTSView::OnUpdateEditCopy( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( !m_Selection.IsEmpty() );
}

void CTSView::OnEditPaste( void )
{
	COleDataObject clDataObject;
	VERIFY( clDataObject.AttachClipboard() );

	if ( clDataObject.IsDataAvailable( s_clClipFmt ) )
	{
		ClearSelections();

		CFile* pFile = clDataObject.GetFileData( s_clClipFmt );
		ASSERT( pFile );

		CArchive ar( pFile, CArchive::load );
		ar.m_pDocument = GetDocument();
		m_Selection.Serialize( ar );
		ar.Close();

		delete pFile;

		for ( POSITION pos = m_Selection.GetHeadPosition(); pos != NULL; )
		{
			GetDocument()->AddShape( m_Selection.GetNext( pos ) );
		}
	}
}

void CTSView::OnUpdateEditPaste( CCmdUI *pCmdUI )
{
	COleDataObject clDataObject;
	pCmdUI->Enable( clDataObject.AttachClipboard() && clDataObject.IsDataAvailable( s_clClipFmt ) );
}

void CTSView::OnEditCut()
{
	OnEditCopy();

	EndEditText();

	while ( !m_Selection.IsEmpty() )
	{
		GetDocument()->RemoveShape( m_Selection.RemoveTail() );
	}
}

void CTSView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( !m_Selection.IsEmpty() && !IsEditingText() );
}

void CTSView::OnFinder( void )
{
	CFinderDlg clDlg;

	clDlg.DoModal();
}

void CTSView::OnUpdateFinder( CCmdUI *pCmdUI )
{
	pCmdUI->Enable( TRUE );
}


BOOL CTSView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if ( nFlags == MK_CONTROL )
	{
		m_dScale += (zDelta/abs((float)zDelta)) * 0.1f;

		m_dScale = m_dScale < c_dMinScale ? c_dMinScale : m_dScale;
		m_dScale = m_dScale > c_dMaxScale ? c_dMaxScale : m_dScale;

		SetScale( m_dScale );
	}

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
