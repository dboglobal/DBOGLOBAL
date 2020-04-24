#pragma once


// CPropTreeItemEdit
#include "PropTreeItem.h"

class CPropTreeItemEdit : public CEdit, public CPropTreeItem
{
	DECLARE_DYNAMIC(CPropTreeItemEdit)
public:
	enum ValueFormat
	{
		ValueFormatText,
		ValueFormatNumber,
		ValueFormatFloatPointer
	};


protected:
	CString		m_sEdit;
	float		m_fValue;

	ValueFormat m_nFormat;
	BOOL		m_bPassword;

public:
	CPropTreeItemEdit();
	virtual ~CPropTreeItemEdit();

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

	

	// Set to specifify format of SetItemValue/GetItemValue
	void SetValueFormat(ValueFormat nFormat);
		
	// Set to TRUE for to use a password edit control
	void SetAsPassword(BOOL bPassword);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnKillfocus();
};


