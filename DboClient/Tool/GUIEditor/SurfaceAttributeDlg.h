#pragma once

#include "Control/PropTree.h"
#include "GUIEditorDoc.h"

// CSurfaceAttributeDlg 대화 상자입니다.

class CSurfaceAttributeDlg : public CDialog
{
	DECLARE_DYNCREATE(CSurfaceAttributeDlg)

public:
	CSurfaceAttributeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSurfaceAttributeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SURFACEATTRIBUTEDLG };

	enum PROPERTY_ID
	{
		PROPERTY_NONE = 0,

		PROPERTY_GROUP,
		PROPERTY_SURFACENAME,
		PROPERTY_RESOURCEFILE,
		PROPERTY_RESOURCENAME,

		PROPERTY_GROUP_COORD,
		PROPERTY_X,
		PROPERTY_Y,
		PROPERTY_WIDTH,
		PROPERTY_HEIGHT,

		PROPERTY_GROUP_COLOR,
		PROPERTY_R,
		PROPERTY_G,
		PROPERTY_B,
		PROPERTY_A,

		PROPERTY_GROUP_UV,
		PROPERTY_UV_LEFT,
		PROPERTY_UV_TOP,
		PROPERTY_UV_RIGHT,
		PROPERTY_UV_BOTTOM,

		PROPERTY_HANDLE,
		PROPERTY_BLEND
	};

public:
	void	SetName( CString strName );
	void	SetUV( CRect& uv );
	void	SetRect( CRect& rect );
	void	SetResourceInfo( CString& strResourcename, CString& strResourceFilename );
	void	FillSurfaceProperty( SURFACEDATA* pSurfaceData );
	
protected:
	CPropTree	m_PropertyTree;
	BOOL		m_bInitialized;

protected:
	// Internal Function
	void	InitProperty();		// 항목 설정.
	
	void	AddCaptionToList();	// List 표기
	void	AddData();			// Data를 List에 추가.
	BOOL	InputData( SURFACEDATA* pSurfaceData, BOOL bFix = FALSE );	// Data 입력
	BOOL	InputCheck( SURFACEDATA* pSurfaceData, BOOL bFix = FALSE );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAddSurfacelist();
	afx_msg void OnBnClickedFixSurfacelist();
};
