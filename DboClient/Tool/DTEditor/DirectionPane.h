/******************************************************************************
* File			: DirectionPane.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 8. 6
* Abstract		: 
*****************************************************************************
* Desc			: DTEdit의 연출관련 트리
*****************************************************************************/

#pragma once

struct sDirectionItem
{
	BYTE		byItemType;
	BYTE		byNodeType;
	void*		pData;
};

class CDirectionPane : public CWnd
{
	class CDirectionTree : public CTreeCtrl
	{		
	public:
		LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	};

	DECLARE_DYNAMIC(CDirectionPane)

public:
	enum eListType
	{
		LIST_TYPE_CONTROLLER,
		LIST_TYPE_NODE,

		NUM_LIST_TYPE
	};	

	typedef std::list<sDirectionItem*>				LIST_DIRECTION_ITEM;
	typedef std::list<sDirectionItem*>::iterator	ITER_DIRECTION_ITEM;

protected:
	DECLARE_MESSAGE_MAP()

public:
	CDirectionPane();
	virtual ~CDirectionPane();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	void		OnSaveDirection();
	void		OnLoadDirection();
	void		OnAddController();
	void		OnAddNode();
	void		OnRemove();
	void		OnRun();
	void		OnStop();

	sDirectionItem*		GetDirectionItem(BYTE byItemType, char* pcName);

	void				StopDirectoin(HTREEITEM hItem);
	void				StopDirectoin(sDirectionItem* pDirectionItem);

	void				Update(float fElpased);

protected:
	void				DestroyTreeItem();
	bool				IsExistSameName_inSibling(HTREEITEM hParentItem, const char* pcName);

protected:
	CXTPToolBar				m_wndToolBar;
	CImageList				m_ilTreeIcons;
	CDirectionTree			m_wndTreeCtrl;

	HTREEITEM				m_hDirectingItem;

	LIST_DIRECTION_ITEM		m_listDirectionItem;
};