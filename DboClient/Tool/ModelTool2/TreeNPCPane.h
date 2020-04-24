#pragma once

#include "TreePCPane.h"

// CTreeNPCPane view

class CTreeNPCPane : public CTreePCPane
{
	DECLARE_DYNCREATE(CTreeNPCPane)

protected:
	CTreeNPCPane();           // protected constructor used by dynamic creation
	virtual ~CTreeNPCPane();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    static CTreeNPCPane* GetInstance() {return m_pInstance;}

protected:
    virtual void InitClass();                        ///< 각 클래스에 맞는 초기화를 수행한다.
    
protected:
    static CTreeNPCPane* m_pInstance;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSetFocus(CWnd* pOldWnd);
};


