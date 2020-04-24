#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "ColorButton.h"

#include "ColorButton.h"
#include "togglebutton2.h"


// CPageClump dialog

class CPageClump : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageClump)

public:
	CPageClump();
	virtual ~CPageClump();

public:
	static CPageClump* GetInstance() {return m_pInstance;};

protected:
	static CPageClump* m_pInstance;

// Dialog Data
	enum { IDD = IDD_PAGECLUMP };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    void    SetMultiTextureInfo(RwChar* chTextureName, RwInt32 iWidth, RwInt32 uiHeight);     ///< MultiTexture 정보를 설정한다.

protected:
    CString m_strLoadFileName;                          ///< Load한 파일명
    CString m_strLoadFilePath;                          ///< Load한 파일의 Full Path

public:
	afx_msg void OnBnClickedButton1();
public:
	CString strEditClumpName;
public:
	afx_msg void OnLbnSelchangeListAtomic();
public:
	CListBox m_listBoxAtomic;
public:
    afx_msg void OnBnClickedBtDelete();
public:
    afx_msg void OnBnClickedBtClumpSave();
public:
    afx_msg void OnBnClickedBtColor();
public:    

public:
    afx_msg void OnBnClickedBtMaterialAdd();
public:
    afx_msg void OnBnClickedBtMaterialDel();

public:
    CColorButton m_btColor;
public:
    afx_msg void OnBnClickedButton3();
public:
    afx_msg void OnLbnSelchangeListMaterial();
public:
    CListBox m_listBoxAttr;
    CListBox m_listBoxMaterial;
    CEdit m_editTextureName;
    CEdit m_editTextureSize;
    CButton m_btTextureAdd;
    CButton m_btTextureDelete;    
    CButton m_cbAlphaTest;
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedRadio2side();
    afx_msg void OnBnClickedCheckAlpha();
    virtual BOOL OnSetActive();
    virtual BOOL OnKillActive();
protected:
    CButton m_cb2Side;
    CButton m_cb2SideEdge;
public:
    afx_msg void OnBnClickedCk2side();
public:
    afx_msg void OnBnClickedCk2sideEdge();
};
