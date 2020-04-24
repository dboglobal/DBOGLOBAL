#pragma once

#include "MTItem.h"
#include "afxcmn.h"
#include "ExTreeCtrl.h"

#define TREE_ITEM_FILE_NAME            "TreeItem.XML"
#define ITEM_PROPERTY_LIST_FILE_NAME   "ItemPropertyList.XML"

typedef std::vector<CMTItem*> VMTItem;

// CTreeItemPane form view

class CTreeItemPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CTreeItemPane)

protected:
	CTreeItemPane();           // protected constructor used by dynamic creation
	virtual ~CTreeItemPane();

public:
	enum { IDD = IDD_TREEITEMPANE };
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
    static CTreeItemPane* GetInstance() {return m_pInstance;}

    void SetItem(CMTItem* pItem);
    void   OnDataChanged();                               ///< 데이터가 변경되었음을 알린다.
    void   OnShowSaveForChanges();                      ///< 종료하기전에 변경된것을 저장할것인지 물어본다.
    

protected:
    void UpdateResFolder(CTreeCtrl* pTreeCtrl, LPCTSTR pstr, HTREEITEM hItemParent, BOOL bFile);                 ///< 리소스 폴더 내용을 트리에 설정한다.
    RwBool OnSaveScript(HTREEITEM hItem);               ///< 아이템의 스크립트를 저장한다.    
    void   OnSavePropertyList();                            ///< Property 리스트 파일을 저장한다.

public:
    CExTreeCtrl   m_treeScript;
    CExTreeCtrl   m_treeRes;

protected:
    static CTreeItemPane* m_pInstance;

    CString       m_strResPath;                             ///< 리소스 파일의 Root 경로
    CString       m_strScriptPath;                          ///< 스크립트 파일들이 저장될 경로
    CString       m_strTreeXMLPath;                         ///< Tree 파일이 저장될 경로
    VWChar        m_vFilePath;
    CString       m_strPrevFileName;                       ///< 파일명 변경시에 사용하는 변수

    CImageList*   m_pImageList;                             ///< 트리에 사용할 이미지 리스트
    HTREEITEM     m_itemResRoot;                            ///< 리소스 트리 Root Item
    HTREEITEM     m_itemScriptRoot;                         ///< Script 트리 Root Item
    
    //VMTItem       m_vMTItem;                                ///< MTItem 객체 리스트 (리소스 뷰에서 생성된 데이터만 담는다)
    CMTItem*      m_pItem;                                  ///< 현재 사용되고 있는 MTItem 객체

public:
    virtual void OnInitialUpdate();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMRclickTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnItemresourceExporttoscript();
    afx_msg void OnTvnSelchangedTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    afx_msg void OnNMRclickTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnItemfolderNewfolder();
    afx_msg void OnItemfolderDelete();
    afx_msg void OnItemfolderImportscript();
    afx_msg void OnItemfolderRename();
    afx_msg void OnItemfolderSavetree();
    afx_msg void OnItemscriptDelete();
    afx_msg void OnItemscriptRename();
    afx_msg void OnItemscriptSave();
    afx_msg void OnItemscriptSaveas();
    afx_msg void OnItemscriptSaveall();
    afx_msg void OnItemscriptOverwriteall();
    afx_msg void OnTvnEndlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBegindragTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBeginlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnDeleteitemTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    
};


