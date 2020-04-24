

#pragma once

typedef enum _XPropTreeItem_ControlType
{ 
	_XPTI_ControlType_Base = 0,
	_XPTI_ControlType_Static,
	_XPTI_ControlType_Edit,
	_XPTI_ControlType_Color,
	_XPTI_ControlType_Combo,
	_XPTI_ControlType_Slider
}_XPropTreeItem_ControlType;

#include "DIB.h"

class CPropTree;

class CPropTreeItem
{
public:
	_XPropTreeItem_ControlType	m_ControlType;

protected:
	// CPropTree class that this class belongs
	CPropTree*			m_pProp;

	// TreeItem label name
	CString				m_sLabel;

	// Descriptive info text
	CString				m_sInfo;

	// TreeItem location
	CPoint				m_loc;

	// TreeItem attribute size
	CRect				m_rc;

	// user defined LPARAM value
	LPARAM				m_lParam;

	// ID of control item (should be unique)
	UINT				m_nCtrlID;

private:
	enum TreeItemStates
	{
		TreeItemSelected =		0x00000001,
		TreeItemExpanded =		0x00000002,
		TreeItemCheckbox =		0x00000004,
		TreeItemChecked =		0x00000008,
		TreeItemActivated =		0x00000010,
		TreeItemReadOnly =		0x00000020,
	};

	// TreeItem state
	DWORD				m_dwState;

	// TRUE if item is activated
	BOOL				m_bActivated;

	// TRUE if item has been commited once (activation)
	BOOL				m_bCommitOnce;

	// Rectangle position of the expand button (if contains one)
	CRect				m_rcExpand;

	// Rectangle position of the check box (if contains one)
	CRect				m_rcCheckbox;

	// link pointers
	CPropTreeItem*		m_pParent;
	CPropTreeItem*		m_pSibling;
	CPropTreeItem*		m_pChild;
	CPropTreeItem*		m_pVis;

public:
	CPropTreeItem(void);
	virtual ~CPropTreeItem(void);

public:
	// TreeItem states
	BOOL IsExpanded();
	BOOL IsSelected();
	BOOL IsChecked();
	BOOL IsReadOnly();
	BOOL IsActivated();

	void Select(BOOL bSelect = TRUE);
	void Expand(BOOL bExpand = TRUE);
	void Check(BOOL bCheck = TRUE);
	void ReadOnly(BOOL bReadOnly = TRUE);

	// Returns true if the item has a checkbox
	BOOL IsCheckBox();

	// Pass in true, for the item to have a checkbox
	void HasCheckBox(BOOL bCheckbox = TRUE);

	// Returns TRUE if the point is on the expand button
	BOOL HitExpand(const POINT& pt);

	// Returns TRUE if the point is on the check box
	BOOL HitCheckBox(const POINT& pt);

	// Returns TRUE if the item is on the root level. Root level items don't have attribute areas
	BOOL IsRootLevel();

	// Returns the total height of the item and all its children
	LONG GetTotalHeight();

	// Set the items label text
	void SetLabelText(LPCTSTR sLabel);

	// Return the items label text
	LPCTSTR GetLabelText();

	// Set the items info (description) text
	void SetInfoText(LPCTSTR sInfo);

	// Get the items info (description) text
	LPCTSTR GetInfoText();

	// Set the item's ID
	void SetCtrlID(UINT nCtrlID);

	// Return the item's ID
	UINT GetCtrlID();

	// draw the item's non attribute area
	LONG DrawItem(CDC* pDC, const RECT& rc, LONG x, LONG y);

	// call to mark attribute changes
	void CommitChanges();

	// call to activate item attribute
	void Activate();

	//
	// Overrideables
	//

	// The attribute area needs drawing
	virtual void DrawAttribute(CDC* pDC, const RECT& rc);

	// Return the height of the item
	virtual LONG GetHeight();

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

	//
	// Usually only CPropTree should calls these
	//

	void SetPropOwner(CPropTree* pProp);

	// Return the location of the PropItem
	const POINT& GetLocation();

	// TreeItem link pointer access
	CPropTreeItem* GetParent();
	CPropTreeItem* GetSibling();
	CPropTreeItem* GetChild();
	CPropTreeItem* GetNextVisible();

	void SetParent(CPropTreeItem* pParent);
	void SetSibling(CPropTreeItem* pSibling);
	void SetChild(CPropTreeItem* pChild);
	void SetNextVisible(CPropTreeItem* pVis);

};

extern CDIB	g_ExpandIcon[4];
void InitializeExpandIconImage(CDC* pDC);
