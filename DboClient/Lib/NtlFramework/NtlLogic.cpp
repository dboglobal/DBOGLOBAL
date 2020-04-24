#include "precomp_ntlframework.h"
#include "NtlLogic.h"
#include "NtlDebug.h"

/**
*
*  constructor for CNtlLogic
*
*/
CNtlLogic::CNtlLogic() : m_bEnabled(false) ,
						 m_fFreeElapsed(0.0f),	
						 m_fSync30Elapsed(0.0f),	
						 m_fSync20Elapsed(0.0f),	
						 m_fSync10Elapsed(0.0f)
{
}

/**
*
*  각종 tick에 대한 event를 등록한다.
*
*/
bool CNtlLogic::Create(void)
{
	NTL_FUNCTION("CNtlLogic::Create");

	RWS::CEventHandler::RegisterMsg (g_EventUpdateTick, "g_EventUpdateTickStr", "float");
		
	NTL_RETURN(true);
	
}

/**
*
*  각종 tick에 대한 등록된 event를 해제한다.
*
*/
void CNtlLogic::Destroy(void)
{
	NTL_FUNCTION("CNtlLogic::Destroy");

    RWS::CEventHandler::UnRegisterMsg ( g_EventUpdateTick );
	
	NTL_RETURNVOID();
}

/**
*
*  simulation object에 update tick evnet를 보낸다.
*  또한 30, 20, 10 frame에 sync된 tick을 계산하여 보낸다.
*
*/
void CNtlLogic::Update(float fElapsed)
{
	if(!m_bEnabled)
		return;

	g_MsgTick.Id = g_EventUpdateTick;
	m_fFreeElapsed = fElapsed;

	g_MsgTick.pData = reinterpret_cast<void*>(&m_fFreeElapsed);
	SendMsg(g_MsgTick);

	// input을 얻어온다.
	// logic을 update 한다. (랜더링 객체 포함)
}

