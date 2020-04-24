#pragma once

#include "CInstanceDialog.h"
#include "CInstancePropertyDialog.h"

// CAssetObjectView 뷰입니다.

class CInstanceView : public CView
{
	DECLARE_DYNCREATE(CInstanceView)

protected:
	CInstanceView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInstanceView();

public:

	CInstanceDialog				m_InstanceDialog;
	CInstancePropertyDialog		m_PropertyDialog;

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.

	CInstanceDialog * GetInstanceDialog( void ) const { return const_cast<CInstanceDialog *>(&m_InstanceDialog); }
	CInstancePropertyDialog * GetPropertyDialog( void ) const { return const_cast<CInstancePropertyDialog *>(&m_PropertyDialog); }

	BOOL						SetEntity( CEntity * pEntity );
	BOOL						UpdateEntity( CEntity * pEntity );

	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	static CInstanceView * GetInstance( void ) { return m_pInstance; }

	HTREEITEM	GetItem( CEntity * pEntity ) { return m_InstanceDialog.GetItem( pEntity ); }
	RwBool		SelectEntityItem( CEntity * pEntity ) { return m_InstanceDialog.SelectEntityItem( pEntity ); }
	RwBool		UnselectEntityItem( CEntity * pEntity ) { return m_InstanceDialog.UnselectEntityItem( pEntity ); }

protected:

	static CInstanceView * m_pInstance;
};


