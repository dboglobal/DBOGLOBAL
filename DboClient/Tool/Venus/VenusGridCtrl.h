#pragma once


// CVenusGridCtrl
#include "SuperGridCtrl.h"
#include "rwcore.h"

class CVenusGridCtrl : public CSuperGridCtrl
{
//	DECLARE_DYNAMIC(CVenusGridCtrl)

public:
	CVenusGridCtrl();
	virtual ~CVenusGridCtrl();

public:	
	void	OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);

	void							ResetTreeItemName(const RwChar* strName);

	CSuperGridCtrl::CTreeItem*		GetSelectRootItem();

	CSuperGridCtrl::CTreeItem*		BuildTreeItem(const RwChar* strName, RwInt32 nImage, DWORD_PTR data, CTreeItem* pParentItem, RwBool bUpdate = FALSE, RwUInt32 state = 0);
	CSuperGridCtrl::CTreeItem*		InsertTreeItem(const RwChar* strName, RwInt32 nImage, DWORD_PTR data, CTreeItem* pParentItem, RwBool bExpand = TRUE, RwUInt32 state = 0);

protected:
	//positions and creates/initalize a combobox control
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);

	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	BOOL OnVkReturn(void);

	void HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str);
	void HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC);
	void HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC);
	void HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void);

	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);

	void SortData(void);
	CTreeItem *Search(CString strItem, ...);
	CTreeItem* SearchEx(CTreeItem *pStartPosition, CString strItem);

protected:
	DECLARE_MESSAGE_MAP()
};


