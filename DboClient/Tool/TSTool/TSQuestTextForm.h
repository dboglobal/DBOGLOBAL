#pragma once
#include "afxcmn.h"
#include "afxwin.h"


#include "TSQuestTextListCtrl.h"


class CTableContainer;


// CTSQuestTextForm 폼 뷰입니다.

class CTSQuestTextForm : public CFormView
{
	DECLARE_DYNCREATE(CTSQuestTextForm)

protected:
	CTSQuestTextForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTSQuestTextForm();

public:
	enum { IDD = IDD_QUEST_TEXT_UTILITY };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CTableContainer* m_pTableMng;

	CString m_strPath;
	CEdit m_ctrQIDFilter;
	CTSQuestTextListCtrl m_ctrQIDFilterList;
	CEdit m_ctrQIDFinder;
	CTSQuestTextListCtrl m_ctrQIDFinderList;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedQuestTextPathBtn();
	afx_msg void OnEnChangeQuestTextQidFilterCurQidEditor();
	afx_msg void OnEnChangeQuestTextQidFinderFindQidEditor();

public:
	void SetCurSelQuestID( CString& strTriggerID );

protected:
	bool CreateQuestTextTable( void );
	void DeleteQuestTextTable( void );

	bool IsNumeric( CString& strVal );
};


extern CTSQuestTextForm* g_pTSQuestTextForm;