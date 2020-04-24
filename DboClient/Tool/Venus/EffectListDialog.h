#pragma once
#include "afxcmn.h"


// CEffectListDialog 대화 상자입니다.

#include "EffectListCtrl.h"
#include "afxwin.h"

class CEffectListDialog : public CDialog
{
	DECLARE_DYNCREATE(CEffectListDialog)

public:
	CEffectListDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectListDialog();

public:
	BOOL					m_Initialized;
	CEffectListCtrl			m_List;

public:
	void	ResetTreeItemName(const RwChar* strName);
	void	RebuildResourceEffect();
    RwBool  SaveXMLTree(RwChar* szFileName);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECT_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);	
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
    CStatic m_EffectFrame;                              /// 내부의 리스트 컨트롤 위치를 잡기위한 더미 컨트롤
};
