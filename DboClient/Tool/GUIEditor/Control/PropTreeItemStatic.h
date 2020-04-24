#pragma once
#include "proptreeitem.h"

class CPropTreeItemStatic : public CPropTreeItem
{
protected:
	CString		m_sAttribute;

public:
	CPropTreeItemStatic(void);
	virtual ~CPropTreeItemStatic(void);

public:
	// The attribute area needs drawing
	virtual void DrawAttribute(CDC* pDC, const RECT& rc);

	// Retrieve the item's attribute value (in this case the CString)
	virtual LPARAM GetItemValue();

	// Set the item's attribute value
	virtual void SetItemValue(LPARAM lParam);
};
