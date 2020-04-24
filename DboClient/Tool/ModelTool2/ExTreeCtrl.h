#pragma once

#include "MTDef.h"
#include "NtlMTXml.h"

/**
 * \ingroup ModelTool2
 * \brief Drag&Drop 기능을 추가한 트리 컨트롤
 * \date 2006-06-08
 * \author agebreak
 */
class CExTreeCtrl : public CXTTreeCtrl
{
public:
    CExTreeCtrl(void);
public:
    virtual ~CExTreeCtrl(void); 

public:
    void    OnMoveItem(HTREEITEM hItem, HTREEITEM hParentItem, BOOL bRecrusive = FALSE);
    void    GetAllChildNode(HTREEITEM hItem, TREEITEM_VECTOR& vTreeItem);   ///< 모든 아이템을 검색하여 벡터에 담는다.
    void    ResetBoldColor();

    void    SaveTree(const char* szFileName);     ///< 트리내용을 XML파일로 저장한다.
    void    LoadTree(const char* szFileName);     ///< 트리내용을 XML파일에서 불러온다.    

    ///----- 오버 라이딩 (아이템 장착 화면을 위해서 설정한다)
    virtual void SetItemBold(HTREEITEM hItem, BOOL bBold = TRUE);
    virtual void SetItemColor(HTREEITEM hItem, COLORREF color);

protected:
	void	InsertItemRecursive(SItemNode* pItemNode, HTREEITEM hParentItem);

protected:
    BOOL            m_bDrag;                ///< 드래그 플래그    
    HTREEITEM       m_hCurItem;             ///< 커서가 위치한 아이템
    TREEITEM_VECTOR     m_vBoldColorItem;       ///< Bold나 Color가 적용된 Item들의 리스트
	SItemNode		m_itemNode;				///< 스크립트및 폴더 아이템 정보를 가지고 있는 객체

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
