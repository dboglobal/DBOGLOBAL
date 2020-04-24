#include "precomp_ntlcore.h"
#include "NtlCoreApi.h"

#include "NtlDebug.h"
#include "NtlCoreLogic.h"
#include "NtlEventQueue.h"
#include "NtlSysEvent.h"

CNtlCoreLogic		*g_pCoreLogic	= NULL;

RwBool API_CoreInit(void)
{
	NTL_FUNCTION("API_CoreInit");

	 // event queue create
	if(!CNtlEventQueue::GetInstance()->Create())
	{
		NTL_RETURN(FALSE);
	}

	g_pCoreLogic = NTL_NEW CNtlCoreLogic;
	if(!g_pCoreLogic->Create())
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(TRUE);
}

void API_CoreTerminate(void)
{
	if(g_pCoreLogic)
	{
		g_pCoreLogic->Destroy();
		NTL_DELETE(g_pCoreLogic);
	}

	// event queue destory
	CNtlEventQueue::GetInstance()->Destroy();
}


void API_CoreUpdate(RwReal fElapsed)
{
	// event queue process filter
	RWS::CMsg msg;
	msg.Id = g_EventQueueProcessFilter;
	msg.pData = NULL;
	SendMsg(msg);

	// queue process
	CNtlEventQueue::GetInstance()->Process(fElapsed); 
	g_pCoreLogic->Update(fElapsed);
}

/**
* \ingroup NtlCore
* System 의 Window Mode 가 Window <-> FullScreen으로 토글될 때 event를 보낸다.
* screenmode가 변경될 때 작업이 필요한 core 보다 상위 레이어의 작업은 g_EventToggleFullScreen을 받아서 처리한다.
*/
void API_CoreToggleFullScreenNotify(bool bFullScreen)
{
	RWS::CMsg msg;

	msg.Id = g_EventToggleFullScreen;
	msg.pData = reinterpret_cast<void*>(&bFullScreen);
	SendMsg(msg);
}