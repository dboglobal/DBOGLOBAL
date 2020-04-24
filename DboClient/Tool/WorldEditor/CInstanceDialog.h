#pragma once

#include "Resource.h"
#include "CInstanceTreeCtrl.h"


// CAssetObjectDlg 대화 상자입니다.

class CInstanceDialog : public CDialog
{
		DECLARE_DYNAMIC( CInstanceDialog )

	public:

		CInstanceTreeCtrl	m_TreeCtrl;

	public:

		CInstanceDialog( CWnd * pParent = NULL );
		virtual ~CInstanceDialog();

		CInstanceTreeCtrl * GetTreeCtrl( void ) const { return const_cast<CInstanceTreeCtrl *>(&m_TreeCtrl); }

		enum { IDD = IDD_INSTANCEDLG };

	protected:

		virtual void DoDataExchange( CDataExchange* pDX );

		DECLARE_MESSAGE_MAP()

	public:

		virtual BOOL OnInitDialog();
		afx_msg void OnSize( UINT nType, int cx, int cy );

		HTREEITEM	GetItem( CEntity * pEntity ) { return m_TreeCtrl.GetItem( pEntity ); }
		RwBool		SelectEntityItem( CEntity * pEntity ) { return m_TreeCtrl.SelectEntityItem( pEntity ); }
		RwBool		UnselectEntityItem( CEntity * pEntity ) { return m_TreeCtrl.UnselectEntityItem( pEntity ); }

};
