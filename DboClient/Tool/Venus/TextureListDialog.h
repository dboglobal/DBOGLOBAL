#pragma once
#include "afxcmn.h"


// CTextureListDialog 대화 상자입니다.
#include "DataTreeCtrl.h"

class CTextureListDialog : public CDialog
{
	DECLARE_DYNCREATE(CTextureListDialog)

public:
	CTextureListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureListDialog();

	void	SerializeList();

public:
	BOOL					m_Initialized;


// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTURE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	afx_msg void OnTvnSelchangedTextureList(NMHDR *pNMHDR, LRESULT *pResult);

protected:
    void OnRefreshList();                           ///< Texture List를 Refresh한다.

protected:
    CDataTreeCtrl m_TextureList;
};
