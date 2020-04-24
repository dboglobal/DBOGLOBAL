#pragma once
#include "afxwin.h"
#include "MTCharacterInfo.h"



// CMaterialPane form view

class CMaterialPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CMaterialPane)

protected:
	CMaterialPane();           // protected constructor used by dynamic creation
	virtual ~CMaterialPane();

public:
	enum { IDD = IDD_MATERIALPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
    static CMaterialPane* GetInstance() {return m_pInstance;};

    void    SetClump(CMTClump* pMTClump);
    void    SetEnable(BOOL bEnable);

protected:
    void    OnDataUpdate();
    void    OnDisplayMaterialInfo(char* szMaterialName);    

protected:
    static CMaterialPane* m_pInstance;

    CMTClump* m_pMTClump;                                ///< 사용되는 Clump 객체


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
protected:
    CListBox m_listBoxMaterial;
    CEdit m_editTextureName;
    CEdit m_editTextureSize;
    CButton m_btTextureAdd;
    CButton m_btTextureDelete;
public:
    afx_msg void OnLbnSelchangeListMaterial();
public:
    afx_msg void OnBnClickedBtMaterialAdd();
public:
    afx_msg void OnBnClickedBtMaterialDel();
};




