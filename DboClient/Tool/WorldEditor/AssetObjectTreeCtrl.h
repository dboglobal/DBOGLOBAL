#pragma once

// CAssetObjectTreeCtrl

class CAssetObjectTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CAssetObjectTreeCtrl)

public:

										CAssetObjectTreeCtrl();
	virtual								~CAssetObjectTreeCtrl();

	HTREEITEM							AddTreeItem( HTREEITEM hParent, CString strName, void * pData);

protected:

	DECLARE_MESSAGE_MAP()

	HTREEITEM							SearchPrevItemByID( HTREEITEM hParent, RwUInt32 iID );
	HTREEITEM							SearchPrevItemByName( HTREEITEM hParent, const RwChar * pName );
	HTREEITEM							SearchItemByName( HTREEITEM hParent, const RwChar * pName );

public:

	afx_msg void						OnSize( UINT nType, int cx, int cy );
	afx_msg LRESULT						OnAssetData( WPARAM wParam, LPARAM lParam );
	afx_msg void						OnNMClick( NMHDR *pNMHDR, LRESULT *pResult );
};
