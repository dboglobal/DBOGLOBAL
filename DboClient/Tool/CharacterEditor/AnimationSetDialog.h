#pragma once

#include "PropertyListCtrl.h"
#include "PropertyAnimData.h"
#include "ToolData.h"

// CAnimAssetAnimSetDlg 대화 상자입니다.

class CAnimationSetDialog : public CDialog
{
	DECLARE_DYNAMIC(CAnimationSetDialog)

public:
	CAnimationSetDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationSetDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANIMASSETANIMSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CPropertyListCtrl	m_propertyControl;

	CPropertyAnimData	m_propertyAnimData;
	CAnimAdaptablePropertyItemManager m_propertyAnimDataManager;
	virtual BOOL OnInitDialog();
	void UpdateTree();
	void SetAnimDataInfoChange(SToolAnimData *pToolAnimData);
	void SetAnimationFileOpen(SToolAnimData *pToolAnimData);
};
