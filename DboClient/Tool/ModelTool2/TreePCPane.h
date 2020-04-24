#pragma once

#include "ExTreeCtrl.h"

#define TREE_BK_WHITE   RGB(0, 0, 0)
#define TREE_BK_BLUE    RGB(125, 125, 225)

// CTreePCPane view

class CTreePCPane : public CXTTreeView 
{
	DECLARE_DYNCREATE(CTreePCPane)

protected:
	CTreePCPane();           // protected constructor used by dynamic creation
	virtual ~CTreePCPane();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CTreePCPane* GetInstance() {return m_pInstance;};

    virtual void SetModel(CMTCharacter* pCharacter); ///< 사용할 모델을 선택한다.
    void   OnDataChanged();                          ///< 캐릭터 데이터가 변경되었을때 호출된다.
    void   OnShowSaveForChanges();                  ///< 종료하기전에 변경된것을 저장할것인지 물어본다.

protected:
    virtual void InitClass();                            ///< 각 클래스에 맞는 초기화

    // Tree 내용 저장 관련
    RwBool LoadTreeXML(const CHAR* szXmlFileName);  ///< 트리구조를 저장하고 있는 XML 파일을 로딩한다.    
    void   GetAllChildNode(HTREEITEM hItem, TREEITEM_VECTOR& vTreeItem); ///< 재귀를 돌면서 모든 자식 아이템을 찾는다.
    RwBool SaveTreeXML(const CHAR* szXmlFileName);  ///< 트리구조를 XML 파일에 저장한다.    

    RwBool OnSetClump();                             ///< 모델의 Clump를 변경한다.
    RwBool OnSaveScript(HTREEITEM hItem, const CHAR* szScriptName, BOOL bVisible);  ///< 스크립트를 저장한다.
    void   InsertItemRecursive(SItemNode* pItemNode, HTREEITEM hParentItem);
    void   OnMoveItem(HTREEITEM hItem, HTREEITEM hParentItem, BOOL bRecrusive = FALSE);

protected:
    static CTreePCPane* m_pInstance;
    CMTCharacter*   m_pCharacter;                   ///< 현재 표시하고 있는 캐릭터

    CImageList*     m_pImageList;                   ///< 이미지 리스트
    CString         m_strRootName;                  ///< Root Item Name
    std::vector<CMTCharacter*> m_vCharacter;        ///< Edit에 사용되는 캐릭터 인스턴스들의 Vector    
    CString         m_sSaveFolderName;              ///< 스크립트를 저장할 폴더명
    CString         m_strTreeFileName;              ///< 트리구조를 저장하는 XML 파일명
    CString         m_strPropertyListFileName;      ///< 프로퍼티 리스트를 저장한 파일명
    CString         m_strPrevScriptName;            ///< 스크립트 이름 변경시에 사용    
	SItemNode		m_itemNode;				        ///< 스크립트및 폴더 아이템 정보를 가지고 있는 객체
    RwBool          m_bDrag;                        ///< 아이템 드래그 유무 플래그
    HTREEITEM       m_hCurItem;                     ///< 커서가 위치한 아이템
    HTREEITEM       m_hDragItem;                    ///< 현재 드래그 하고 있는 아이템

protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
    afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMenuAddFolder();
    afx_msg void OnMenuRenameFolder();
    afx_msg void OnMenuDeleteFolder();
    afx_msg void OnMenuNewScript();
    afx_msg void OnMenuLoadScript();
    afx_msg void OnScriptSetclump();
    afx_msg void OnMenuScriptSave();
    afx_msg void OnMenuScriptSaveas();
    afx_msg void OnMenuScriptRename();
    afx_msg void OnMenuDeleteScript();
    afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnScriptSaveall();
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


