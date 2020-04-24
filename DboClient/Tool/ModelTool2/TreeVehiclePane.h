#pragma once

#include "TreePCPane.h"

/**
 * \ingroup ModelTool2
 * \brief 탈것 관련 Pane
 *
 * \date 2008-11-04
 * \author agebreak
 */
class CTreeVehiclePane : public CTreePCPane
{
	DECLARE_DYNCREATE(CTreeVehiclePane)

protected:
	CTreeVehiclePane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTreeVehiclePane();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
    static CTreeVehiclePane*    GetInstance() {return m_pInstance;}
    afx_msg void OnSetFocus(CWnd* pOldWnd);

protected:
    virtual void InitClass();

protected:
    static CTreeVehiclePane*    m_pInstance;

};


