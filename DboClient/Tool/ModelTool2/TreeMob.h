#pragma once

#include "TreePCPane.h"
// CTreeMobPane view

class CTreeMobPane : public CTreePCPane
{
	DECLARE_DYNCREATE(CTreeMobPane)

protected:
	CTreeMobPane();           // protected constructor used by dynamic creation
	virtual ~CTreeMobPane();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CTreeMobPane* GetInstance() {return m_pInstance;}

protected:
    virtual void InitClass();                                ///< 각 클래스에 맞는 초기화


protected:
    static  CTreeMobPane* m_pInstance;
    

protected:
	DECLARE_MESSAGE_MAP()    
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


