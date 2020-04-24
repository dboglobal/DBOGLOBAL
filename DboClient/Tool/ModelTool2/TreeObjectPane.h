#pragma once
#include "afxcmn.h"
#include "ExTreeCtrl.h"


#define SAVE_FILE_NAME                   "TreeObject.XML"
#define OBJECT_PROPERTY_LIST_FILE_NAME   "ObjectPropertyList.XML"

// CTreeObjectPane form view


typedef std::vector<CMTObject*> VMTObject;

class CTreeObjectPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CTreeObjectPane)

protected:
	CTreeObjectPane();           // protected constructor used by dynamic creation
	virtual ~CTreeObjectPane();

public:
	enum { IDD = IDD_TREEOBJECTPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CTreeObjectPane* GetInstance() {return m_pInstance;}

    void   SetObject(CMTObject* pObject);
    void   OnDataChanged();                               ///< 데이터가 변경되었음을 알린다.
    void   OnShowSaveForChanges();                      ///< 종료하기전에 변경된것을 저장할것인지 물어본다.

protected:
    void UpdateResFolder(CTreeCtrl* pTreeCtrl, LPCTSTR pstr, HTREEITEM hItemParent, BOOL bFile);                 ///< 리소스 폴더 내용을 트리에 설정한다.
    RwBool OnSaveScript(HTREEITEM hItem);               ///< 아이템의 스크립트를 저장한다.    
    void   OnSavePropertyList();                        ///< Property List 파일을 저장한다
    
protected:
    static CTreeObjectPane* m_pInstance;

    CString     m_strObjectPath;                        ///< 오브젝트 리소스 폴더의 루트 패스
    CString     m_strScriptPath;                        ///< 스크립트가 저장될 폴더
    CString     m_strTreeXMLPath;                       ///< TreeXml이 저장될 폴더
    CImageList* m_pImageList;                           ///< 트리에 사용할 이미지 리스트

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
    CExTreeCtrl   m_treeScript;
    CExTreeCtrl   m_treeRes;
    HTREEITEM     m_itemResRoot;
    HTREEITEM     m_itemScriptRoot;
    VWChar        m_vFilePath;
    VMTObject     m_vMTObject;
    CMTObject*    m_pObject;                               ///< 현재 작업하고 있는 Object 객체
    CString       m_strPrevFileName;                       ///< 파일명 변경시에 사용하는 변수    

public:
    virtual void OnInitialUpdate();
    afx_msg void OnNMRclickTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnObjectresourceExporttoscript();
    afx_msg void OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnSelchangedTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnNMRclickTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnObjectfolderNewfolder();
    afx_msg void OnTvnEndlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnObjectfolderRename();
    afx_msg void OnObjectfolderDelete();
    afx_msg void OnObjectfolderImportscript();
    afx_msg void OnObjectscriptRename();
    afx_msg void OnObjectscriptDelete();
    afx_msg void OnDestroy();
    
    afx_msg void OnObjectscriptSave();
    afx_msg void OnObjectscriptSaveas();
    afx_msg void OnObjectscriptSaveall();
    afx_msg void OnObjectfolderSavetree();
public:
    afx_msg void OnTvnDeleteitemTreeScript(NMHDR *pNMHDR, LRESULT *pResult);
};


