#include "StdAfx.h"
#include ".\layermisc.h"
#include "MainFrm.h"

CLayerMisc::CLayerMisc(void)
{
	m_nCurrentLayer = RESOURCE_LAYER;
}

CLayerMisc::~CLayerMisc(void)
{
}

CLayerMisc& CLayerMisc::GetInstance()
{
	static CLayerMisc layermisc;
	return layermisc;
}

CView* CLayerMisc::GetLayer( LAYER nIndex )
{
	if( !( nIndex < LAYER_MAXCOUNT ) )
		return NULL;

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CView*		pView = pMainFrm->m_wndLayerBar.GetView( nIndex );

	return pView;
}

void CLayerMisc::SetLayer( LAYER nIndex )
{
	LAYER OldLayer = m_nCurrentLayer;
	m_nCurrentLayer = nIndex;

	// TODO : 주로 Focus가 바뀔때 호출되는 함수 이므로 Layer에 따른 전환초기화를 해준다.
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	pMainFrm->m_wndLayerBar.SetActiveView( m_nCurrentLayer );
}

