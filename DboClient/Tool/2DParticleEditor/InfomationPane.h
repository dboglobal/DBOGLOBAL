#pragma once


#include "resource.h"

// CInfomationPane 폼 뷰입니다.

class CInfomationPane : public CFormView
{
	DECLARE_DYNCREATE(CInfomationPane)

protected:
	CInfomationPane();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CInfomationPane();

public:
	enum { IDD = IDD_INFOMATION };
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
	void	SetClientMouse( int nX, int nY );
	void	SetParticleMouse( int nX, int nY );
	void	SetResolution( int nWidth, int nHeight );

	static CInfomationPane* m_pInstance;
	static CInfomationPane* GetInstance() { return m_pInstance; }

public:
	CString m_szClientMouseX;			///< 클라이언트 영역 ( 0 ~ View 사이즈 )
	CString m_szClientMouseY;
	CString m_szParticleMouseX;			///< 파티클 영역 ( Offset값을 포함한 )
	CString m_szParticleMouseY;
	CString m_szResolutionWidth;
	CString m_szResoultionHeight;

protected:
	virtual void OnInitialUpdate();
};

static CInfomationPane* GetInfomationPane()
{
	return CInfomationPane::GetInstance();
}

