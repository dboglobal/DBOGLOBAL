#pragma once


// CPointEditDlg 대화 상자입니다.


class CPointEditDlg : public CDialog
{
	class CPointEditView : public CStatic
	{
	public:

		struct SPropPointInfo
		{
			int		nIndex;
			BOOL 	bSelect;
			CPoint	pt;
		};

		typedef std::list<SPropPointInfo> ListPropPoint;

	private:

		BOOL	m_bActive;
		int		m_nViewType;
		float	m_fScale;
		int		m_nGridXNum;
		int		m_nGridYNum;
		int		m_nGridRulerIndex;
		int		m_nGridRulerSize;

		int		m_nGridScrollOffsetX;
		int		m_nGridScrollOffsetY;
		float	m_fCameraCenterX;
		float	m_fCameraCenterY;

		CRect	m_rtBackground;
		CPoint	m_ptGridCenter;
		CString	m_strViewName;
		
		int				m_nFocusPropPoint;
		CPoint			m_ptFocusPropPoint;
		CRect			m_rtSelectCursor;
		ListPropPoint	m_listPropPoint;

		CPen	m_penCenter;
		CPen	m_penBlock;
		CPen	m_penElement;
		CPen	m_penPropPoint;

		void	CalcBackgourndSize(void);
		void	CalcGridNum(void);
		RwReal	CaleScale(void);
		void	CalePropPoint(void);
		CPoint	CalcPropPointOne(int iIdx);

		void	OnBackgroundPaint(CPaintDC& dc);
		void	OnGridPaint(CPaintDC& dc);
		void	OnNumberPaint(CPaintDC& dc);
		void	OnPropPointPaint(CPaintDC& dc);
		void	OnFocusPropPointPaint(CPaintDC& dc);
		void	OnSelectCursorPaint(CPaintDC& dc);

	public:

		void	IncScale(void);
		void	DecScale(void);
		float	GetScale(void);

		void	OnPaneMove(int nDeltaX, int nDeltaY);
		void	OnPaneCenter(void);
		void	OnPaneSerachPoint(int nIdx);
		void	OnFocusPropPoint(CPoint point);
		void	OnSelectPoint(CRect rt, std::list<int>& listSelPoint);
		void	OnSelectPoint(int nSx, int nEx);
		void	OnSelectPointClear(void);
		void	OnSelectCursor(CRect rt);
		void	OnSelectCursorClear(void);
		
		void	OnPropPointMove(int nDeltaX, int nDeltaY);
		void	OnRecalcPropPoint(void);
		
		void	SetViewType(int nType);
		void	SetViewName(const char *pViewName);
		void	SetActive(BOOL bActive);

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
	public:
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	};

	DECLARE_DYNAMIC(CPointEditDlg)

private:

	BOOL			m_bQuaterView;
	int				m_nActiveView;

	CRect			m_rtFullView;
	CRect			m_rtView[4];
	CPointEditView	m_EditView[4];

	BOOL			m_bPaneActive;
	CPoint			m_ptOldPane;

	BOOL			m_bSelectActive;
	CPoint			m_ptSelectPoint;

	BOOL			m_bPointMove;
	CPoint			m_ptOldPointMove;

	BOOL			m_bWndZoomInOut;
	CPoint			m_ptOldWndZoomInOut;
	CRect			m_rtWndZoomInOut;

	void			CalcViewSize(void);
	CRect			CalcSelectDrag(CPoint point);
	int				SelectView(CPoint point);
	void			VisibleScale(float fScale);
	void			VisibleWorldCenter(float f1, float f2);

	void			CalcControlPosition(int nCx, int nCy);

public:

	void			OnPropUpdate(void);
	void			OnPropSelectUpdate(int nSx, int nEx);


public:
	CPointEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPointEditDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_POINTEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnBnClickedPointeditCenterMove();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnBnClickedPointeditSearchMove();
};
