#pragma once
#include "afxwin.h"

/**
 * \ingroup ModelTool2
 * \brief Navigation Mesh & Obstacle Mesh View Pane
 *
 * \date 2008-11-12
 * \author agebreak
 */
class CNaviMeshPane : public CXTResizeFormView
{
	DECLARE_DYNCREATE(CNaviMeshPane)

protected:
	CNaviMeshPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CNaviMeshPane();

public:
	enum { IDD = IDD_NAVIMESHPANE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
    static CNaviMeshPane* GetInstance() {return m_pInstance;}

    void SetObject(CMTObject* pObject);                     ///< 오브젝트를 세팅한다.
    void Render();                                          ///< 선택된 메시를 렌더링한다.

    virtual void OnInitialUpdate();

protected:
    void CreateClump(CString& strFileName);                 ///< 네비게이션 메시를 생성한다.
    void RemoveAllClump();                                  ///< 네비게이션 메시 모두를 삭제한다.

protected:
    static CNaviMeshPane*   m_pInstance;
    
    CListBox m_listNavi;
    CListBox m_listObs;
    std::vector<CMTClump*> m_vecClump;                     ///< 생성된 네비게이션 메시를 담고 있는 벡터
    CMTObject* m_pObject;
};


