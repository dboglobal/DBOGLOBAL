#pragma once


// PaletteDecalStaticDlg 대화 상자입니다.

class CPaletteDecalStaticDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteDecalStaticDlg)

public:
	CPaletteDecalStaticDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaletteDecalStaticDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ORNAMENT_DECAL_STATIC };

protected:
	virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT		WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL		PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

public:
	void				InitializeWorldData();

public:
	virtual BOOL		OnInitDialog();
	afx_msg void		OnBnClickedOk();
};
