#pragma once

#include "CustomItems.h"


// CBBoxPane form view

class CBBoxPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CBBoxPane)
public:
    enum EIdAnimBBox
    {
        ID_BBOX_WIDHT,
        ID_BBOX_HEIGHT,
        ID_BBOX_DEPTH,
    };

protected:
	CBBoxPane();           // protected constructor used by dynamic creation
	virtual ~CBBoxPane();

public:
	enum { IDD = IDD_BBOXPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CBBoxPane* GetInstance() {return m_pInstance;};

    void SetModel(CMTCharacter* pCharacter);

protected:
    void OnChangeWidth();
    void OnChangeHeight();
    void OnChangeDepth();

protected:
    static CBBoxPane* m_pInstance;

    CMTCharacter* m_pCharacter;
    CNtlPLCharacterProperty* m_pCharProperty;

    CXTPPropertyGrid              m_property;
    CXTPPropertyGridItem*         m_pCatAnimBBox;
    CCustomItemSpin*            m_pItemWidth;
    CCustomItemSpin*            m_pItemHeight;
    CCustomItemSpin*            m_pItemDepth;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg LRESULT OnGridNotify(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
public:
    virtual void OnInitialUpdate();
};


