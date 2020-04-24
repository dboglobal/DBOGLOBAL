#pragma once

#include "NtlWorldPropMaterialController.h"

// MaterialPropDlg 대화 상자입니다.

class CMaterialPropDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialPropDlg)

private: // user
	enum eWORK 
	{
		eWORK_UPDATE_ALL,
		eWORK_DELETE_ALL,
		eWORK_UPDATE,
		eWORK_DELETE,
		eWORK_EXPORT,
	};

	CNtlWorldPropMaterialController* m_pNtlWorldPropMaterialController;

private:
	void WorkProc(eWORK eWork);

public: // sys
	CMaterialPropDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMaterialPropDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MATERIAL_PROP };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedAllDelete();
	afx_msg void OnBnClickedAllUpdate();
};
