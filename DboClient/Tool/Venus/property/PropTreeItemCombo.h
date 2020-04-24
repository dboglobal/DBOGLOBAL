#pragma once


// CPropTreeItemCombo
#include "PropTreeItem.h"

class CPropTreeItemCombo : public CComboBox, public CPropTreeItem
{
	DECLARE_DYNAMIC(CPropTreeItemCombo)

protected:
	LPARAM		m_lComboData;
	LONG		m_nDropHeight;

// Operations
protected:
	LONG FindCBData(LPARAM lParam);

public:
	CPropTreeItemCombo();
	virtual ~CPropTreeItemCombo();
// Attributes
public:
	// The attribute area needs drawing
	virtual void DrawAttribute(CDC* pDC, const RECT& rc);

	// Retrieve the item's attribute value
	virtual LPARAM GetItemValue();

	// Set the item's attribute value
	virtual void SetItemValue(LPARAM lParam);

	// Called when attribute area has changed size
	virtual void OnMove();

	// Called when the item needs to refresh its data
	virtual void OnRefresh();

	// Called when the item needs to commit its changes
	virtual void OnCommit();

	// Called to activate the item
	virtual void OnActivate();

	// Create your combo box with your specified styles
	BOOL CreateComboBox(DWORD dwStyle = WS_CHILD|WS_VSCROLL|CBS_SORT|CBS_DROPDOWNLIST);

	// Create combo box with TRUE/FALSE selections
	BOOL CreateComboBoxBool();

	// Set the height for the dropdown combo box
	void SetDropDownHeight(LONG nDropHeight);

	// Get the height of the dropdown combo box
	LONG GetDropDownHeight();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnKillfocus();
};


