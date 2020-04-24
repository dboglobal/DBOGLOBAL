#pragma once

class CEntity;


typedef hash_map<RwUInt32, HTREEITEM>	TTreeItemHash;
typedef TTreeItemHash::iterator			TTreeItemHashIterator;
typedef std::pair<RwUInt32, HTREEITEM>	TTreeItemHashHashPair;

// CInstanceTreeCtrl

class CInstanceTreeCtrl : public CTreeCtrl
{

		TTreeItemHash						m_Hash;

		DECLARE_DYNAMIC(CInstanceTreeCtrl)

	public:

		CInstanceTreeCtrl();
		virtual ~CInstanceTreeCtrl();

		LRESULT InitializeData( void );
		LRESULT RegisterEntity( CEntity * pEntity );
		LRESULT UnregisterEntity( CEntity * pEntity );

		HTREEITEM AddTreeItem(HTREEITEM hParent, CString strName, void *pData);

		DECLARE_MESSAGE_MAP()

	public:

		HTREEITEM	GetItem( CEntity * pEntity );
		RwBool		SelectEntityItem( CEntity * pEntity );
		RwBool		UnselectEntityItem( CEntity * pEntity );
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	protected:

		HTREEITEM GetItemFB( HTREEITEM hItem, CEntity * pEntity );

	public:

		static CInstanceTreeCtrl * GetValidInstance( void ) { return m_pInstance; }

		static int CALLBACK SortByNameCB( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort );

	protected:

		static CInstanceTreeCtrl * m_pInstance;

};