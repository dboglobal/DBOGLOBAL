#pragma once


#include "Shape.h"


class CSCtrl;
class CSCtrl_Select;
class CSCtrl_Link;
class CSCtrl_Link_Def;
class CSCtrl_Link_Yes;
class CSCtrl_Link_No;
class CSCtrl_Link_Error;
class CSCtrl_Link_LogIn;
class CSCtrl_End;
class CSCtrl_GAct;
class CSCtrl_GCond;
class CSCtrl_Narration;
class CSCtrl_Proposal;
class CSCtrl_Reward;
class CSCtrl_Start;
class CSCtrl_UsrSel;
class CSCtrl_Switch;
class CSCtrl_UnifiedNarration;
class CSCtrl_Note;


//////////////////////////////////////////////////////////////////////////
//
//	CTSView
//
//////////////////////////////////////////////////////////////////////////


class CTSView : public CScrollView
{
protected:
	DECLARE_DYNCREATE( CTSView )
	DECLARE_MESSAGE_MAP()

// Static variables
protected:
	static CLIPFORMAT					s_clClipFmt;

// Member variables
protected:
	const double						c_dMinScale;
	const double						c_dNormalScale;
	const double						c_dMaxScale;

	double								m_dScale;

	bool								m_bGrid;
	long								m_GridX;
	long								m_GridY;

	CSCtrl*								m_pSCtrlCurrent;

	CSCtrl_Select*						m_pSCtrlSelect;

	CSCtrl_Link_Def*					m_pSCtrlLinkDef;
	CSCtrl_Link_Yes*					m_pSCtrlLinkYes;
	CSCtrl_Link_No*						m_pSCtrlLinkNo;
	CSCtrl_Link_Error*					m_pSCtrlLinkError;
	CSCtrl_Link_LogIn*					m_pSCtrlLinkLogIn;

	CSCtrl_End*							m_pSCtrlEnd;
	CSCtrl_GAct*						m_pSCtrlGAct;
	CSCtrl_GCond*						m_pSCtrlGCond;
	CSCtrl_Narration*					m_pSCtrlNarration;
	CSCtrl_Proposal*					m_pSCtrlProposal;
	CSCtrl_Reward*						m_pSCtrlReward;
	CSCtrl_Start*						m_pSCtrlStart;
	CSCtrl_UsrSel*						m_pSCtrlUsrSel;
	CSCtrl_Switch*						m_pSCtrlSwitch;
	CSCtrl_UnifiedNarration*			m_pSCtrlUnifiedNarration;
	CSCtrl_Note*						m_pSCtrlNote;
	ShapeList							m_Selection;

	CEdit*								m_pEditCtrl;
	CString*							m_pEditString;

// Construction and Destructions
protected:
	CTSView( void );

public:
	virtual ~CTSView( void );

// Methods
public:
	CTSDoc*								GetDocument( void ) const;

	virtual BOOL						PreCreateWindow( CREATESTRUCT& cs );
	virtual void						OnDraw( CDC* pDC );
	virtual void						OnPrepareDC( CDC* pDC, CPrintInfo* pInfo = NULL );

	virtual void						OnInitialUpdate( void );
	virtual BOOL						OnPreparePrinting( CPrintInfo* pInfo );
	virtual void						OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo );
	virtual void						OnEndPrinting( CDC* pDC, CPrintInfo* pInfo );
	virtual void						OnActivateView( BOOL bActivate, CView* pActivateView, CView* pDeactiveView );

	void								SetScale( double dScale );
	void								RenderGrid( CDC* pDC ) const;
	void								RenderSelection( CDC* pDC ) const;

	virtual CPoint						Device2Doc( const CPoint& p );
	virtual CRect						Device2Doc( const CRect& r );
	virtual CPoint						Doc2Device( const CPoint& p );
	virtual CRect						Doc2Device( const CRect& r );

	virtual CPoint						Align2Grid( const CPoint& p ) const;
	virtual CRect						Align2Grid( const CRect& r ) const;

	virtual const ShapeList&			GetSelectionList( void ) const { return m_Selection; }
	virtual bool						IsSelected( CShape* pShape ) const;
	virtual void						SelectShape( CShape* pShape, bool bAdd = false );
	virtual void						SelectShape( const CRect& rect, bool bAdd = false );
	virtual void						UnselectShape( CShape* pShape );
	virtual void						ClearSelections( void );

	virtual bool						IsEditingText() const { return m_pEditCtrl != NULL; }
	virtual void						BeginEditText( CString &text, const CRect &rect, CFont *pRefFont, DWORD dwStyle = 0 );
	virtual void						EndEditText( void );

#ifdef _DEBUG
	virtual void						AssertValid( void ) const;
	virtual void						Dump( CDumpContext& dc ) const;
#endif

public:
	afx_msg void						OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void						OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void						OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void						OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void						OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void						OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL						OnEraseBkgnd(CDC* pDC);

	afx_msg void						OnCreateProject( void );
	afx_msg void						OnOpenProject( void );
	afx_msg void						OnSaveProject( void );
	afx_msg void						OnUpdateSaveProject( CCmdUI *pCmdUI );
	afx_msg void						OnCloseProject( void );
	afx_msg void						OnUpdateCloseProject( CCmdUI *pCmdUI );
	afx_msg void						OnExportTrigger( void );
	afx_msg void						OnUpdateExportTrigger( CCmdUI *pCmdUI );
	afx_msg void						OnExportTriggerAll( void );
	afx_msg void						OnUpdateExportTriggerAll( CCmdUI *pCmdUI );
	afx_msg void						OnEncryptionTrigger( void );
	afx_msg void						OnUpdateEncryptionTrigger( CCmdUI *pCmdUI );
	afx_msg void						OnExportEventor( void );
	afx_msg void						OnUpdateExportEventor( CCmdUI *pCmdUI );

	afx_msg void						OnContSelect( void );
	afx_msg void						OnUpdateContSelect(CCmdUI *pCmdUI);

	afx_msg void						OnContLinkDef( void );
	afx_msg void						OnUpdateContLinkDef(CCmdUI *pCmdUI);
	afx_msg void						OnContLinkYes( void );
	afx_msg void						OnUpdateContLinkYes(CCmdUI *pCmdUI);
	afx_msg void						OnContLinkNo( void );
	afx_msg void						OnUpdateContLinkNo(CCmdUI *pCmdUI);
	afx_msg void						OnContLinkError( void );
	afx_msg void						OnUpdateContLinkError(CCmdUI *pCmdUI);
	afx_msg void						OnContLinkLogIn( void );
	afx_msg void						OnUpdateContLinkLogIn(CCmdUI *pCmdUI);

	afx_msg void						OnContStart( void );
	afx_msg void						OnUpdateContStart(CCmdUI *pCmdUI);
	afx_msg void						OnContEnd( void );
	afx_msg void						OnUpdateContEnd(CCmdUI *pCmdUI);
	afx_msg void						OnContGCond( void );
	afx_msg void						OnUpdateContGCond(CCmdUI *pCmdUI);
	afx_msg void						OnContGAct( void );
	afx_msg void						OnUpdateContGAct(CCmdUI *pCmdUI);
	afx_msg void						OnContProposal( void );
	afx_msg void						OnUpdateContProposal(CCmdUI *pCmdUI);
	afx_msg void						OnContUsrSel( void );
	afx_msg void						OnUpdateContUsrSel(CCmdUI *pCmdUI);
	afx_msg void						OnContNarration( void );
	afx_msg void						OnUpdateContNarration(CCmdUI *pCmdUI);
	afx_msg void						OnContReward( void );
	afx_msg void						OnUpdateContReward(CCmdUI *pCmdUI);
	afx_msg void						OnContSwitch( void );
	afx_msg void						OnUpdateContSwitch(CCmdUI *pCmdUI);
	afx_msg void						OnContUnifiedNarration( void );
	afx_msg void						OnUpdateContUnifiedNarration(CCmdUI *pCmdUI);
	afx_msg void						OnContNote( void );
	afx_msg void						OnUpdateNote(CCmdUI *pCmdUI);

	afx_msg void						OnChangeMobGroupIndex( void );
	afx_msg void						OnUpdateChangeMobGroupIndex( CCmdUI *pCmdUI );

	afx_msg void						OnEditCopy( void );
	afx_msg void						OnUpdateEditCopy( CCmdUI *pCmdUI );
	afx_msg void						OnEditPaste( void );
	afx_msg void						OnUpdateEditPaste( CCmdUI *pCmdUI );
	afx_msg void						OnEditCut( void );
	afx_msg void						OnUpdateEditCut( CCmdUI *pCmdUI );

	afx_msg void						OnFinder( void );
	afx_msg void						OnUpdateFinder(CCmdUI *pCmdUI);


public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


extern CTSView* g_pTSView;


#ifndef _DEBUG  // TSView.cpp의 디버그 버전
inline CTSDoc* CTSView::GetDocument() const
   { return reinterpret_cast<CTSDoc*>(m_pDocument); }
#endif
