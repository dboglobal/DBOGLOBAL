#include "StdAfx.h"
#include "AssetObjectView.h"
#include "CInstanceView.h"
#include "CEditorMainControlBar.h"


CEditorMainControlBar::CEditorMainControlBar()
{
	m_nActiveTab = 0;	
}

CEditorMainControlBar::~CEditorMainControlBar()
{
	this->m_font.DeleteObject();

	while(!m_views.IsEmpty())
	{
		TCB_ITEM *pMember=m_views.RemoveHead();
		delete pMember;
	}
}

int CEditorMainControlBar::AddViewes( void )
{
	AddView("Resource", RUNTIME_CLASS(CAssetObjectView));
	AddView("Instance", RUNTIME_CLASS(CInstanceView));

	return 2;
}