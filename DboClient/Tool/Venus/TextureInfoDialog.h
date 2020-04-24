#pragma once
#include "afxwin.h"


// CTextureInfoDialog 대화 상자입니다.
#include "rwcore.h"

#include "libgfl.h"

class CTextureInfoDialog : public CDialog
{
	DECLARE_DYNCREATE(CTextureInfoDialog)

public:
	CTextureInfoDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureInfoDialog();

	void	CreateBitmap(const RwChar* strPathName);

public:
	BOOL					m_Initialized;

	GFL_BITMAP*				m_pGflBitmap;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTURE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_TextureScreen;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nTextureWidth;
	int m_nTextureHeight;
};
