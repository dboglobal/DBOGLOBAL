#pragma once

class CPagePC;

typedef std::vector<HTREEITEM> TREEITEM_VECTOR;

/**
 * \ingroup ModelTool
 * \brief CTreeCtrl을 상속받아 ModelTool에서 사용하는 TreeCtrl
 * \date 2006-04-17
 * \author agebreak
 */
class CMTTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CMTTreeCtrl)

public:
    enum eITEM_KIND
    {
        ITEM_FOLDER = 10,
        ITEM_SCRIPT,
        ITEM_CLUMP,
        ITEM_FILE
    };

public:
	CMTTreeCtrl();
	virtual ~CMTTreeCtrl();

    void    SetRoot(WCHAR* szRootName);  
    void    SetParent(CPagePC* pLeftView);          ///< 트리컨트롤을 가지고 있는 객체를 설정한다. (왼쪽뷰의 모든 객체는 CPagePC로부터 상속받음)
    CString SelectScriptName();
    void    SetScriptName(CString sScriptName);
    CString SelectClumpName();
    void    AddItem(const RwChar* sScriptName);     ///< 스크립트 항목을 추가한다.

    void    SaveTree(WCHAR* szRootName, WCHAR* szFileName);     ///< 트리내용을 XML파일로 저장한다.
    void    LoadTree(WCHAR* szFileName);     ///< 트리내용을 XML파일에서 불러온다.

    void    GetAllChildNode(HTREEITEM hItem, TREEITEM_VECTOR& vTreeItem);   ///< 모든 아이템을 검색하여 벡터에 담는다.

protected:
    void    EndDrag(CPoint point);                  ///< 드래그가 끝날때 호출된다
    HTREEITEM GetItemByName(WCHAR* szItemName);     ///< 아이템의 이름으로 포인터를 찾는다.

protected:
    CImageList*     m_pImageList;                   ///< 트리컨트롤에 사용되는 이미지 리스트 
    HTREEITEM       m_hSelectedItem;                ///< 현재 선택된 아이템
    HTREEITEM       m_hCurItem;                     ///< 드래그에 사용되는 아이템
    HTREEITEM       m_hDragItem;                    ///< 드래그에 사용되는 아이템
    BOOL            m_bDrag;                        ///< 드래그에 사용될 플래그
    CPagePC*        m_pParentView;                  ///< 트리를 가지고 있는 부모 뷰
    CString         m_sPrevScriptName;              ///< 이전 스크립트명. 파일이름 변경시 필요하다


protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
public:
    afx_msg void OnMenuAddFolder();
public:
    afx_msg void OnMenuAddScript();
public:
    afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);

public:
    afx_msg void OnMenuNewScript();
public:
    afx_msg void OnMenuLoadScript();
public:
    afx_msg void OnMenuDeleteFolder();
public:
    afx_msg void OnMenuDeleteScript();
public:
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
    afx_msg void OnMenuRenameFolder();
public:
    afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnMenuScriptSave();
public:
    afx_msg void OnUpdateMenuScriptSave(CCmdUI *pCmdUI);
public:
    afx_msg void OnMenuScriptSaveas();
public:
    afx_msg void OnMenuScriptRename();
public:
    afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


