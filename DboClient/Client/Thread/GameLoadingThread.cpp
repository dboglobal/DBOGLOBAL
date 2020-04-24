#include "precomp_dboclient.h"
#include "GameLoadingThread.h"

// core
#include "NtlLoadingController.h"

// simulation
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

// client
#include "GameStage.h"


CGameLoadingThread::CGameLoadingThread( CGameStage* pGameStage )
{
	m_pGameStage = pGameStage;

	m_eGameLoadState = eGAME_LOAD_STATE_WAIT;

	m_pDevice = 0;

	CNtlLoadingController::GetInstance()->Create();



	RwBool bThreadLoad = TRUE;

	if(Logic_IsDevUser())
	{
		SDevMasterInfo *pDevMasterInfo = API_GetSLDevMasterInformation();
		bThreadLoad = pDevMasterInfo->bThreadLoading;
	}

	if(bThreadLoad)
	{
		// Thread 시작
		Create();
		ResumeThread();
	}
	else
	{
		ThreadCallBackFunc(NULL);
	}

	// Give Thread a minimum amount of time to run
	Sleep( 10 );
}

CGameLoadingThread::~CGameLoadingThread( void )
{
	// Thread 종료
	Delete();

	CNtlLoadingController::GetInstance()->Delete();

	m_pDevice = 0;
}

int CGameLoadingThread::ThreadCallBackFunc( void* pParam )
{
	SetLoadState( eGAME_LOAD_STATE_LOADING );

	if ( m_pGameStage )
		m_pGameStage->MutiThreadLoading();

	SetLoadState( eGAME_LOAD_STATE_LOADED );

	return RESULT_THREAD_SUCCESS;
}


CGameLoadingThread::eGAME_LOAD_STATE CGameLoadingThread::GetLoadState( void )
{
	eGAME_LOAD_STATE eState;

	m_clCSLoaded.Lock();
	eState = m_eGameLoadState;
	m_clCSLoaded.Unlock();

	return eState;
}

void CGameLoadingThread::SetLoadState( eGAME_LOAD_STATE eState )
{
	m_clCSLoaded.Lock();
	m_eGameLoadState = eState;
	m_clCSLoaded.Unlock();
}