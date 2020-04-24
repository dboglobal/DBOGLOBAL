#ifndef __PROPERTY_ANIM_DATA_H__
#define __PROPERTY_ANIM_DATA_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropertyListCtrl.h"
#include "UserMessageID.h"
#include "ToolData.h"
//#include "EffPrecompile.h"

#define ID_PD_FILE_NAME1		1
#define ID_PD_FILE_NAME2		2
#define ID_PD_FILE_NAME3		3

#define ID_PD_BLEND_TYPE1		4
#define ID_PD_BLEND_TYPE2		5
#define ID_PD_BLEND_TYPE3		6

#define ID_PD_BLEND_TIME1		7
#define ID_PD_BLEND_TIME2		8
#define ID_PD_BLEND_TIME3		9


class CAnimAdaptablePropertyItemManager;

// Only as an example
/////////////////////////////////////////////////////////////////////////////
// CMyData

class CPropertyAnimData : public CObject
{
	CPropertyAnimData(const CPropertyAnimData& d);
	CPropertyAnimData& operator=(const CPropertyAnimData& d);

public:
	CPropertyAnimData();
	~CPropertyAnimData();

	//void UpdateData(SAnimData *pAnimData);

	// Data
private:
	//SAnimData *m_pAnimData;

	friend CAnimAdaptablePropertyItemManager;
};

inline CPropertyAnimData::~CPropertyAnimData()
{
}



/////////////////////////////////////////////////////////////////////////////
// CMyAdaptablePropertyItemManager

class CAnimAdaptablePropertyItemManager : public CPropertyItemManagerAdaptable
{
	CAnimAdaptablePropertyItemManager(const CAnimAdaptablePropertyItemManager& d);
	CAnimAdaptablePropertyItemManager& operator=(const CAnimAdaptablePropertyItemManager& d);

public:
	CAnimAdaptablePropertyItemManager();
	~CAnimAdaptablePropertyItemManager();
	
	void UpdateData(SToolAnimData *pToolAnimData);

	bool SetData(const CObject* pData) { return true; }
	bool GetData(CObject* pData) const { return true; }

	void OnDataChanged(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex);
	void OnLButtonDblClick(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex);
	void OnLButtonDown(CPropertyItem* pPropertyItem, CPropertyListCtrl* pWndPropertyListCtrl, int nIndex);
private:

	CObject *m_pData;
	SToolAnimData *m_pToolAnimData;
};

#endif // !defined(AFX_PROPERTYLISTCTRLDLG_H__466FB4E7_6072_11D3_A7E3_006008C8B630__INCLUDED_)