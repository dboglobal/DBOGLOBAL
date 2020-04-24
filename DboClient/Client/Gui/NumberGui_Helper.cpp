#include "precomp_dboclient.h"
#include "NumberGui_Helper.h"

// core
#include "NtlDebug.h"

// dbo
#include "NumberGui.h"


CNumberGui_ZoomHelper::CNumberGui_ZoomHelper()
:m_bFinish(TRUE)
,m_bPlay(FALSE)
,m_pCallback(NULL)
{

}

CNumberGui_ZoomHelper::~CNumberGui_ZoomHelper()
{
	RemoveAll();	
}

VOID CNumberGui_ZoomHelper::Update(RwReal fElapsed)
{
	if( m_bFinish )
		return;

	if( !m_bPlay )
		return;

	sZoomData* pZoomData = GetCurData();
	if( !pZoomData )
	{
		if( m_pCallback )
			m_pCallback->Call();

		m_bFinish = TRUE;
		return;
	}

	pZoomData->fElapsed += fElapsed;

	if( pZoomData->fElapsed >= pZoomData->fRemainTime )
	{
		pZoomData->fElapsed = pZoomData->fRemainTime;
		pZoomData->bFinish = TRUE;
	}


	RwReal fRate = pZoomData->fStartRate + (pZoomData->fEndRate - pZoomData->fStartRate) * (pZoomData->fElapsed / pZoomData->fRemainTime);
	pZoomData->pNumberGui->SetRate(fRate);
}

VOID CNumberGui_ZoomHelper::Play(RwBool bPlay)
{
	m_bPlay = bPlay;
}

VOID CNumberGui_ZoomHelper::Restart()
{
	m_bFinish	= FALSE;
	m_bPlay		= FALSE;

	for each( sZoomData* pZoomData in m_listZoom )
	{
		pZoomData->bFinish		= FALSE;
		pZoomData->fElapsed		= 0.f;
	}

	m_itCur = m_listZoom.begin();
}

RwBool CNumberGui_ZoomHelper::AddNumberGui(CNumberGui* pNumberGui, RwReal fStartRate, RwReal fEndRate, RwReal fRemainTime)
{
	if( m_bPlay )
		return FALSE;

	if( fStartRate == fEndRate )
		return FALSE;

	if( !pNumberGui )
		return FALSE;

	if( fStartRate < 0.f )
		return FALSE;

	if( fEndRate < 0.f )
		return FALSE;


	sZoomData* pZoomData = NTL_NEW sZoomData;

	pZoomData->bFinish		= FALSE;
	pZoomData->fStartRate	= fStartRate;
	pZoomData->fEndRate		= fEndRate;
	pZoomData->fRemainTime	= fRemainTime;
	pZoomData->fElapsed		= 0.f;
	pZoomData->pNumberGui	= pNumberGui;

	bool bEmpted = m_listZoom.empty();
	m_listZoom.push_back(pZoomData);

	if( bEmpted )
		m_itCur = m_listZoom.begin();

	m_bFinish = FALSE;

	return TRUE;
}

VOID CNumberGui_ZoomHelper::RemoveAll()
{
	m_bPlay		= FALSE;

	if( m_pCallback )
		NTL_DELETE(m_pCallback);

	for each( sZoomData* pZoomData in m_listZoom )
	{
		NTL_DELETE(pZoomData);
	}
	m_listZoom.clear();

	m_itCur = m_listZoom.end();
}

CNumberGui_ZoomHelper::sZoomData* CNumberGui_ZoomHelper::GetCurData()
{
	while( m_itCur != m_listZoom.end() )
	{
		sZoomData* pZoomData = *m_itCur;
		if( !pZoomData->bFinish )
			return pZoomData;

		++m_itCur;
	}
	
	return NULL;
}

RwBool CNumberGui_ZoomHelper::IsFinish()
{
	return m_bFinish;
}

RwBool CNumberGui_ZoomHelper::IsPlay()
{
	return m_bPlay;
}