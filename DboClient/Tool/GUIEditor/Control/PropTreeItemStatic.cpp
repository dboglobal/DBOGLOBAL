#include "StdAfx.h"
#include ".\proptreeitemstatic.h"

#include "PropTree.h"

CPropTreeItemStatic::CPropTreeItemStatic(void) :
	m_sAttribute(_T(""))
{
}

CPropTreeItemStatic::~CPropTreeItemStatic(void)
{
}

void CPropTreeItemStatic::DrawAttribute(CDC* pDC, const RECT& rc)
{
	ASSERT(m_pProp!=NULL);

	pDC->SelectObject(m_pProp->GetNormalFont());
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	CRect r = rc;
	pDC->DrawText(m_sAttribute, r, DT_SINGLELINE|DT_VCENTER);
}


LPARAM CPropTreeItemStatic::GetItemValue()
{
	return (LPARAM)(LPCTSTR)m_sAttribute;
}


void CPropTreeItemStatic::SetItemValue(LPARAM lParam)
{
	if (lParam==0L)
	{
		TRACE0("CPropTreeItemStatic::SetItemValue() - Invalid lParam value\n");
		return;
	}

	m_sAttribute = (LPCTSTR)lParam;
}
