#pragma once

#include "Control/PropTree.h"

// CRectInfoDlg 대화 상자입니다.

class CRectInfoDlg : public CDialog
{
	DECLARE_DYNCREATE(CRectInfoDlg)

public:
	CRectInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRectInfoDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RECTINFODLG };

	enum PROPERTY_ID
	{
		PROPERTY_NONE = 0,
		
		PROPERTY_CAPTION,
		PROPERTY_MOUSEPT,
		PROPERTY_GROUP_RECT,
		PROPERTY_RECT_XY,
		PROPERTY_RECT_WH,
		PROPERTY_RGBA
		// For Test
		//PROPERTY_ORIMOUSE,			// 변환되기 전의 마우스 좌표.
		//PROPERTY_OFFSET,			// 옵셋
		//PROPERTY_SRCRECT,			// 찍히고 있는 영역
		//PROPERTY_CLIENTRECT			// 클라이언트 영역
	};

	// Operation
	void	SetMousePoint( CPoint& point );
	void	SetSelectedRect( CRect& rect );
	//void	SetOffset( CPoint& offset );
	//void	SetOriMousePoint( CPoint& point );
	//void	SetSrcRect( CRect& rect );
	//void	SetClientRect( CRect& rect );

protected:
	CPropTree	m_PropertyTree;
	BOOL		m_bInitialized;

protected:
	// Internal Function
	void	InitProperty();		// 항목 설정.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
