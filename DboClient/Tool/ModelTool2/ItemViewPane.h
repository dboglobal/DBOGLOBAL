#pragma once
#include "afxcmn.h"
#include "MTCharacter.h"
#include "MTItem.h"
#include "ExTreeCtrl.h"
#include "afxwin.h"

// CItemViewPane form view

class CItemViewPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CItemViewPane)

protected:
	CItemViewPane();           // protected constructor used by dynamic creation
	virtual ~CItemViewPane();

public:
	enum { IDD = IDD_ITEMVIEWPANE };
#ifdef _DEBUG
    virtual void AssertValid() const;
#ifndef _WIN32_WCE
    virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    static CItemViewPane* GetInstance() {return m_pInstance;}

    void SetCharacter(CMTCharacter* pCharacter);       ///< Edit할 캐릭터를 설정한다.

protected:            
    void UpdateTree();                                 ///< Tree의 내용을 Update한다.
    void EquipItem(HTREEITEM hItem);                   ///< Item을 착용한다.

protected:
    static CItemViewPane* m_pInstance;
    CExTreeCtrl   m_treeItem;
    CImageList*   m_pImageList;                        ///< 트리에 사용할 이미지 리스트
    CMTCharacter* m_pCharacter;                        ///< 현재 Edit할 캐릭터 객체

    std::vector<CNtlPLItem*>    m_vAttachItem;         ///< Attach된 Item들의 vector

    COLORREF      m_colorSelect;
    COLORREF      m_colorNon;

public:
    virtual void OnInitialUpdate();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnTvnSelchangedTreeItem(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnBnClickedCheckItem();
public:
    CButton m_ckItemView;
};


