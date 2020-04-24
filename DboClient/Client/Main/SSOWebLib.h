// SSOWebLib.h: interface for the CSSOWebLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
#define AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
typedef void* HWEBCTRL;

typedef void (SWEBCALLBACK_ONBN2)(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
typedef void (SWEBCALLBACK_ONNC2)(LPCTSTR strURL);
typedef void (SWEBCALLBACK_ONQUIT)();

// SSO 웹브라우저를 Shared DLL로 사용하기 위해 호출하는 함수(InitSSOWebBrowser() 이전에 호출되어야 함)
void PresetSSOWebBrowserToSharedDll();

// 서비스 사이트에 따른 쿠키 이름을 변경하기 위하여 호출하는 함수(InitSSOWebBrowser() 이전에 호출되어야 함)
// 다음게임인 경우 "daum"을 넘김(넷마블인 경우는 호출할 필요가 없음)
void PresetSSOWebBrowserSetSiteName(LPCTSTR strSiteName);

void PresetSSOWebBrowserBaseFolder(LPCTSTR strFolder);

// SSO 웹브라우저 초기화 함수
BOOL InitSSOWebBrowser(HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie);

// 테스트용 초기화 함수
BOOL InitSSOWebBrowser2(LPCSTR strBaseDir, HWND hWndMain, LPCTSTR strAuthCookie, LPCTSTR strDataCookie, LPCTSTR strCpCookie);

// SSO 웹브라우저 파괴 함수(메인 윈도우가 파괴되기 전에 호출해야한다)
BOOL UnInitSSOWebBrowser();

// 쿠키 활성화 페이지 URL을 얻는 함수
LPCTSTR GetActCookieURL();

// 쿠키 리프레쉬 페이지 URL을 얻는 함수
LPCTSTR GetRefreshCookieURL();

// SSO 웹브라우저로부터 쿠키를 얻는 함수(내부적으로 변환되기 때문에 얻은 그대로 put_cookie하면 된다)
BOOL GetSSOWebCookieString(LPSTR pstrAuth, LPSTR pstrData, LPSTR pstrCp);

//////////////////////////////////////////////////////////////////////
// SSO 웹브라우저 클래스

class CSSOWebBrowser  
{
public:

	HWEBCTRL		m_hWeb;

	CSSOWebBrowser();
	virtual ~CSSOWebBrowser();

	// 웹 브라우저 초기화 및 생성
	BOOL Create(DWORD dwStyle, RECT& rect, HWND hWndParent, UINT nID);
	void Destroy();

	void SetOption(BOOL bHideBorder);
	BOOL PutCharSet(LPCTSTR strCharset);
	BOOL InsertHTML(LPCTSTR strHtml);
	BOOL ExecJavascript(LPCTSTR strScript);

	BOOL Navigate(LPCTSTR strURL);
	BOOL GetNetmarbleCookie(LPSTR pstrAuthCookie, LPSTR pstrDataCookie, LPSTR pstrCpCookie);

	void* GetWnd();
	int  GetUserNavigatedCount();
	void SetUserNavigatedCount(int count);
	void ResetUserNavigatedCount();

	static HMODULE	m_hDllModule;
	static BOOL		m_bInitialized;

	//virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//virtual void OnNavigateComplete2( LPCTSTR strURL );
	//virtual void OnQuit();

	// 콜백 함수
	//void (*m_pCallback_OnBeforeNavigate2)(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, void* pbaPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//void (*m_pCallback_OnNavigateComplete2)( LPCTSTR strURL );
	//void (*m_pCallback_OnQuit)();

	void SetCallback_OnBeforeNavigate2(SWEBCALLBACK_ONBN2* pCallback);
	void SetCallback_OnNavigateComplete2(SWEBCALLBACK_ONNC2* pCallback);
	void SetCallback_OnQuit(SWEBCALLBACK_ONQUIT* pCallback);
};

#endif // !defined(AFX_SSOWEBLIB_H__B6EC3473_CBDF_4B60_8B92_75E4962873EE__INCLUDED_)
