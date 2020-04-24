#include "StdAfx.h"
#include "PropertyAnimData.h"
//#include "NewAssetOpen.h"
#include "CharacterEditor.h"
//#include "NtlPLResourceManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyItemListYesNo

BEGIN_LIST_ITEM_DATA_TYPE(CPropertyItemListYesNo)
	LPCTSTR_STRING_ITEM_DATA(_T("No")),
	LPCTSTR_STRING_ITEM_DATA(_T("Yes"))
END_LIST_ITEM_DATA_TYPE(CPropertyItemListYesNo)

/////////////////////////////////////////////////////////////////////////////
// CPropertyItemListDataType

BEGIN_LIST_ITEM_DATA_TYPE(CPropertyItemListDataType)
	LPCTSTR_STRING_ITEM_DATA(_T("Only data")),
	LPCTSTR_STRING_ITEM_DATA(_T("Input line")),
	LPCTSTR_STRING_ITEM_DATA(_T("Data item")),
	LPCTSTR_STRING_ITEM_DATA(_T("List")),
	LPCTSTR_STRING_ITEM_DATA(_T("Form")),
	LPCTSTR_STRING_ITEM_DATA(_T("Macro"))
END_LIST_ITEM_DATA_TYPE(CPropertyItemListDataType)

/////////////////////////////////////////////////////////////////////////////
// CPropertyItemListDataFormatType

BEGIN_LIST_ITEM_DATA_TYPE(CPropertyItemListDataFormatType)
	LPCTSTR_STRING_ITEM_DATA(_T("Number")),
	LPCTSTR_STRING_ITEM_DATA(_T("Currency")),
	LPCTSTR_STRING_ITEM_DATA(_T("Time")),
	LPCTSTR_STRING_ITEM_DATA(_T("Date")),
	LPCTSTR_STRING_ITEM_DATA(_T("Other type"))
END_LIST_ITEM_DATA_TYPE(CPropertyItemListDataFormatType)
/////////////////////////////////////////////////////////////////////////////
// CMyData
BEGIN_LIST_ITEM_DATA_TYPE(CPropertyItemListBlendType)
	LPCTSTR_STRING_ITEM_DATA(_T("Blend_Tween")),
	LPCTSTR_STRING_ITEM_DATA(_T("Blend_Mix"))
END_LIST_ITEM_DATA_TYPE(CPropertyItemListBlendType)

CPropertyAnimData::CPropertyAnimData()
{
}

/*
void CPropertyAnimData::UpdateData(SAnimData *pAnimData)
{
	m_pAnimData = pAnimData;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CMyAdaptablePropertyItemManager

CAnimAdaptablePropertyItemManager::CAnimAdaptablePropertyItemManager()
{
	
	BEGIN_PROPERTY_TAB(_T("AnimFileName"), false)
		PROPERTY_ITEM(ID_PD_FILE_NAME1, CPropertyItemString, _T("FileName"), true)
		//PROPERTY_ITEM(ID_PD_BLEND_TYPE1, CPropertyItemListBlendType, _T("BlendType"), true)
		//PROPERTY_ITEM(ID_PD_BLEND_TIME1, CPropertyItemString, _T("BlendInterval"), true)
	END_PROPERTY_TAB()
	
	m_pToolAnimData = NULL;
}

CAnimAdaptablePropertyItemManager::~CAnimAdaptablePropertyItemManager()
{
}

void CAnimAdaptablePropertyItemManager::UpdateData(SToolAnimData *pToolAnimData)
{
	CPropertyItemCategory *pNewTab = GetCategoryTab(0);
	pNewTab->SetEnabled(true);
	
	//pNewTab = GetCategoryTab(1);
	//pNewTab->SetEnabled(false);

	//pNewTab = GetCategoryTab(2);
	//pNewTab->SetEnabled(false);

	BEGIN_ITERATE_PROPERTY_ITEMS()
		SET_ITEM_STRING(ID_PD_FILE_NAME1, pToolAnimData->pAnimData->strAnimFileName.c_str())
	END_ITERATE_PROPERTY_ITEMS()

	m_pToolAnimData = pToolAnimData;
}

void CAnimAdaptablePropertyItemManager::OnDataChanged(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex)
{
	CheckState(pWndPropertyListCtrl, nIndex, pPropertyItem->GetPropertyID());
}

void CAnimAdaptablePropertyItemManager::OnLButtonDblClick(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex)
{

}

void CAnimAdaptablePropertyItemManager::OnLButtonDown(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex)
{
	if( pPropertyItem->GetPropertyID() == ID_PD_FILE_NAME1 )
	{
		theApp.m_pAnimationSetDialog->SetAnimationFileOpen(m_pToolAnimData);
	}
}