#pragma once


class CEntityPropertyTree;

// CInstancePropertyDialog 대화 상자입니다.

class CInstancePropertyDialog : public CDialog
{
	DECLARE_DYNAMIC(CInstancePropertyDialog)

	CEntityPropertyTree *		m_pPropertyTree;

public:
	CInstancePropertyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInstancePropertyDialog();


	BOOL						SetEntity( CEntity * pEntity );
	BOOL						UpdateEntity( CEntity * pEntity );
	CEntityPropertyTree *		GetPropertyTree( void ) const { return m_pPropertyTree; }
	//{{AFX_DATA(CDlgDlg)
	enum { IDD = IDD_INSTANCE_PROPERTY_FORMVIEW };
	//}}AFX_DATA

	//{{AFX_MSG(CDlgDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	//{{AFX_MSG(CDlgDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
