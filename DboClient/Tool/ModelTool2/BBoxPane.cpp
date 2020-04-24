// BBoxPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "BBoxPane.h"

#define IDC_PROPERTY_ANIM_BBOX      101

CBBoxPane* CBBoxPane::m_pInstance = NULL;
// CBBoxPane

IMPLEMENT_DYNCREATE(CBBoxPane, CXTResizeFormView)

CBBoxPane::CBBoxPane()
	: CXTResizeFormView(CBBoxPane::IDD)
{
    m_pInstance = this;

    m_pCharProperty = NULL;
    m_pCharacter = NULL;
}

CBBoxPane::~CBBoxPane()
{
}

void CBBoxPane::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBBoxPane, CXTResizeFormView)
    ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


// CBBoxPane diagnostics

#ifdef _DEBUG
void CBBoxPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBBoxPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBBoxPane message handlers

void CBBoxPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    CRect rc;
    GetDlgItem(IDC_STATIC)->GetWindowRect(&rc);
    ScreenToClient(&rc);
    
    if(m_property.Create(rc, this, IDC_PROPERTY_ANIM_BBOX))
    {
        m_property.SetVariableItemsHeight(TRUE);        
        m_property.ShowToolBar(FALSE);
        m_property.ShowHelp(FALSE);
        m_property.HighlightChangedItems(TRUE);

        m_pCatAnimBBox = m_property.AddCategory("Animation BBox");        

        m_pItemWidth = (CCustomItemSpin*)m_pCatAnimBBox->AddChildItem(new CCustomItemSpin("Width"));
        m_pItemHeight = (CCustomItemSpin*)m_pCatAnimBBox->AddChildItem(new CCustomItemSpin("Height"));
        m_pItemDepth = (CCustomItemSpin*)m_pCatAnimBBox->AddChildItem(new CCustomItemSpin("Depth"));

        m_pItemWidth->SetID(ID_BBOX_WIDHT);
        m_pItemHeight->SetID(ID_BBOX_HEIGHT);
        m_pItemDepth->SetID(ID_BBOX_DEPTH);

       m_pCatAnimBBox->Expand();
    }

    SetResize(IDC_PROPERTY_ANIM_BBOX, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetModel(NULL);    
}

void CBBoxPane::SetModel(CMTCharacter* pCharacter)
{
    if(!pCharacter)
    {
        m_pCatAnimBBox->SetHidden(TRUE);
        m_pCharacter = NULL;
        m_pCharProperty = NULL;
    }
    else
    {
        m_pCatAnimBBox->SetHidden(FALSE);
        m_pCharacter = pCharacter;
        m_pCharProperty = pCharacter->GetProperty();

        RwReal fWidth = 0.0f;
        RwReal fHeight = 0.0f;
        RwReal fDepth = 0.0f;

        RwBBox* pBBoxAnim = m_pCharProperty->GetAnimBBox();
        if(pBBoxAnim->sup.x > 0.0f)
        {
            fWidth = pBBoxAnim->sup.x - pBBoxAnim->inf.x;
            fHeight = pBBoxAnim->sup.y - pBBoxAnim->inf.y;
            fDepth = pBBoxAnim->sup.z - pBBoxAnim->inf.z;
        }
        else
        {
            // 만약에 Anim BBox 데이터가 없으면 디폴트 BBox를 생성한다.
            RwBBox bBoxAnim = m_pCharacter->CreateDefaultAnimBBox();
            fWidth = bBoxAnim.sup.x - bBoxAnim.inf.x;
            fHeight = bBoxAnim.sup.y - bBoxAnim.inf.y;
            fDepth = bBoxAnim.sup.z - bBoxAnim.inf.z;
        }

        m_pItemWidth->SetNumber((int)(fWidth * 10.0f + 0.5f));
        m_pItemHeight->SetNumber((int)(fHeight * 10.0f + 0.5f));
        m_pItemDepth->SetNumber((int)(fDepth * 10.0f + 0.5f));
    }
}

LRESULT CBBoxPane::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    if(wParam == XTP_PGN_ITEMVALUE_CHANGED)
    {   
        if(!m_pCharProperty)
            return 0;

        CXTPPropertyGridItem* pItem = (CXTPPropertyGridItem*)lParam;
        switch(pItem->GetID())
        {
        case ID_BBOX_WIDHT:
            OnChangeWidth();
            break;
        case ID_BBOX_HEIGHT:
            OnChangeHeight();
            break;
        case ID_BBOX_DEPTH:
            OnChangeDepth();
            break;
        }        
    }
    
    return 0;
}

void CBBoxPane::OnChangeWidth()
{
    // NOTE: 나누기 10을 해서 넣는다.
    RwReal fBBoxWidth = (float)m_pItemWidth->GetNumber() / 10.0f;
    RwBBox* pBBoxAnim = m_pCharProperty->GetAnimBBox();
    pBBoxAnim->sup.x = fBBoxWidth / 2.0f;
    pBBoxAnim->inf.x = -(fBBoxWidth / 2.0f);
}

void CBBoxPane::OnChangeHeight()
{
    // Height는 위로만 커진다.
    RwReal fBBoxHeight = (float)m_pItemHeight->GetNumber() / 10.0f;
    RwBBox* pBBoxAnim = m_pCharProperty->GetAnimBBox();
    pBBoxAnim->sup.y = fBBoxHeight;
    pBBoxAnim->inf.y = 0.0f;
}

void CBBoxPane::OnChangeDepth()
{
    RwReal fBBoxDepth = (float)m_pItemDepth->GetNumber() / 10.0f;
    RwBBox* pBBoxAnim = m_pCharProperty->GetAnimBBox();
    pBBoxAnim->sup.z = fBBoxDepth / 2.0f;
    pBBoxAnim->inf.z = -(fBBoxDepth / 2.0f);
}