#pragma once
#include "splashscreenex.h"
//#include "afxcmn.h"
#include "skinprogress.h"

class CSplashScreenFx :	public CSplashScreenEx
{
	DECLARE_DYNAMIC(CSplashScreenFx)

public:
	CSplashScreenFx(void);
	~CSplashScreenFx(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void WaitUntilUserConfirm(void);
	CProgressCtrl m_ctrlProgress;
	CSkinProgress* m_pSkinProgress;
	void CreateProgressBar(CRect& rcProgress, CSize& sizeProgress, DWORD dwStyle);
	void ProgressStep(void);
};
