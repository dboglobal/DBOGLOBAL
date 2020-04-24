#pragma once
#include "afxwin.h"
#include <list>

struct SSplineProp;
class CNtlDTProp;
class CNtlDTSpline;

// CSequenceGraphDlg 대화 상자입니다.

typedef struct _SSeqCtrlInfo
{
	CPoint pt;
	CRect rt;
	int nIdx;
}SSeqCtrlInfo;


class CSequenceGraphDlg : public CDialog
{
	class CSequenceEditView : public CStatic
	{
	private:

		CPoint	m_ptScrollOffset;

		SSeqCtrlInfo *m_pPrevCtrlSel;
		SSeqCtrlInfo *m_pNextCtrlSel;
		SSeqCtrlInfo *m_pCurrCtrlSel;

		typedef std::list<SSeqCtrlInfo*> ListSeqCtrl;
		ListSeqCtrl m_listSeqCtrl;

		CPen	m_penGrid;
		CPen	m_penBlockGrid;
		CPen	m_penPropPoint;

		void			CalcSeqCtrl(void);
		void			DeleteSeqCtrl(void);
		SSeqCtrlInfo*	PrevSeqCtrl(SSeqCtrlInfo *pCtrlInfo);
		SSeqCtrlInfo*	NextSeqCtrl(SSeqCtrlInfo *pCtrlInfo);
		CPoint			GetSeqPosition(int nIdx, float fTime);

		int				GetRulerGrid(void);
		float			GetRulerTime(void);
			
		void			OnGridPaint(CPaintDC& dc);
		void			OnProPointPaint(CPaintDC& dc);

	public:

		SSeqCtrlInfo*	GetSeqCtrl(CPoint pt);
		float			GetSeqTime(CPoint pt);
		float			GetSeqTime(SSeqCtrlInfo *pCtrlInfo);
		SSeqCtrlInfo*	GetPrevSeqCtrl(void)	{ return m_pPrevCtrlSel; }
		SSeqCtrlInfo*	GetCurrSeqCtrl(void)	{ return m_pCurrCtrlSel; }	
		SSeqCtrlInfo*	GetNextSeqCtrl(void)	{ return m_pNextCtrlSel; }	
		CPoint			GetScrollPosition(void) { return m_ptScrollOffset; }
		
		SSeqCtrlInfo*	OnSelPropPoint(CPoint point);
		void			OnSelPropPointClear(void);
		void			OnScrollPositionClear(void);

		void			OnPaneMove(int nDeltaX, int nDeltaY);
		void			OnPropPointMove(CPoint point, CNtlDTSpline *pDTSpilne);
		
		void			OnProPointUpdate(void);

	public:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnPaint();
	public:
		afx_msg void OnSize(UINT nType, int cx, int cy);
	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	public:
		afx_msg void OnDestroy();
	};

	DECLARE_DYNAMIC(CSequenceGraphDlg)

public:
	CSequenceGraphDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSequenceGraphDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_SEQUENCE_GRAPH };

protected:

	int		m_nRulerSize;
	float	m_fRulderTime;
	int		m_nCurrGridX;
	int		m_nCurrGridY;
	
	BOOL	m_bSelPoint;
	BOOL	m_bWndZoomInOut;
	BOOL	m_bPanMove;

	CPoint	m_ptOldPane;
	CPoint	m_ptOldWndZoomInOut;
	CRect	m_rtWndZoomInOut;

	CPen	m_penRuler;

	CRect				m_rtSeqEdit;
	CSequenceEditView	m_SeqEditView;

	void			CalcSeqEditView(void);
	CPoint			GetLocalEditView(CPoint point);
	
	void			DrawAxis(CPaintDC& dc);

public:

	CNtlDTSpline	*m_pDTSpilne;
	SSplineProp		*m_pSplineProp;

	void	ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData);
	void	DeActiveSplineProp(void);
	void	DeleteSplineProp(CNtlDTProp *pDTProp);
	void	RebuildProp();

	void	ChangeControlPointTime(void);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	CStatic m_ctrlTimeInfo;
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSequenceGraphCenter();

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};
