#include "StdAfx.h"
#include "splashscreenfx.h"
#include "../../resource.h"

IMPLEMENT_DYNAMIC(CSplashScreenFx, CSplashScreenEx)
CSplashScreenFx::CSplashScreenFx(void)
{
}

CSplashScreenFx::~CSplashScreenFx(void)
{
	delete m_pSkinProgress;
}


BEGIN_MESSAGE_MAP(CSplashScreenFx, CSplashScreenEx)
END_MESSAGE_MAP()


void CSplashScreenFx::WaitUntilUserConfirm(void)
{
	MSG message;
	while(1)
	{
		if(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
			if(message.message == WM_LBUTTONDOWN)
				break;
			if(message.message == WM_KEYDOWN)
				break;
		}
	}
}

void CSplashScreenFx::CreateProgressBar(CRect& rcProgress, CSize& sizeProgress, DWORD dwStyle)
{
	m_ctrlProgress.DestroyWindow();
	m_ctrlProgress.Create( /*WS_VISIBLE |*/ WS_CHILD | dwStyle, rcProgress, this, 1);

	CWnd*    pProgress = NULL;
	CBitmap* pBitmap   = NULL;

	pProgress = &m_ctrlProgress;

	pBitmap = new CBitmap();
	pBitmap->LoadBitmap(IDB_BMP_PHOTON);

	m_pSkinProgress = new CSkinProgress (pProgress,	sizeProgress.cy, pBitmap);

	delete pBitmap;
}

void CSplashScreenFx::ProgressStep(void)
{
	m_pSkinProgress->StepIt();
}
