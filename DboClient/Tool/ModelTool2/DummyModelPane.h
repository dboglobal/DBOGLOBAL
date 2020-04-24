#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CDummyModelPane form view

class CDummyModelPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CDummyModelPane)

protected:
	CDummyModelPane();           // protected constructor used by dynamic creation
	virtual ~CDummyModelPane();

public:
	enum { IDD = IDD_DUMMYMODELPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
    CMTCharacter*   m_pCharacter;               ///< 사용할 캐릭터 객체 (하나만 생성해서 사용한다)
    RwReal          m_fSensitive;               ///< 모델이동시의 변경수치

    CXTBrowseEdit m_edClumpName;
    CButton m_ckVisible;
    CEdit m_edPosX;
    CEdit m_edPosY;
    CSpinButtonCtrl m_spPosX;
    CSpinButtonCtrl m_spPosZ;

public:
    virtual void OnInitialUpdate();
public:
    afx_msg void OnEnChangeEditFilename();
public:
    afx_msg void OnBnClickedCheckVisible();
public:
    afx_msg void OnEnChangeEditPosx();
public:
    afx_msg void OnEnChangeEditPosy();
    
public:
    afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
};


