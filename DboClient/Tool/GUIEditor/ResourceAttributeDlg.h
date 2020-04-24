#pragma once

#include "Control/PropTree.h"
#include "GUIEditorDoc.h"

// CResourceAttributeDlg 대화 상자입니다.

class CResourceAttributeDlg : public CDialog
{
	DECLARE_DYNCREATE(CResourceAttributeDlg)

public:
	CResourceAttributeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResourceAttributeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RESOURCEATTRIBUTEDLG };

	enum PROPERTY_ID
	{
		PROPERTY_NONE = 0,

		PROPERTY_CAPTION,
		PROPERTY_RESOURCENAME,
		PROPERTY_HANDLE,
		PROPERTY_FILENAME
	};

public:
	void	SetBlank();
	void	SetFileName( LPCTSTR szFileName );
	void	FillResourceProperty( RESOURCEDATA* pResourceData );

protected:
	CPropTree	m_PropertyTree;
	BOOL		m_bInitialized;

protected:
	// Internal Function
	void	InitProperty();		// 항목 설정.

	void	AddCaptionToList();	// List 표기
	void	AddData();			// Data를 리스트에 추가.
	BOOL	InputData( RESOURCEDATA* pResourceData, BOOL bFix = FALSE );	// 데이터 입력.
	BOOL	InputCheck( RESOURCEDATA* pResourceData, BOOL bFIX = FALSE );	// 데이터 체크.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAddResourlist();
protected:

public:
	afx_msg void OnBnClickedFixResourcelist();
};
