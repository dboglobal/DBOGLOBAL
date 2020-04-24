#include "precomp_ntlframework.h"
#include "NtlRender.h"
#include "NtlDebug.h"

/**
*
*  constructor for CNtlRender
*
*/
CNtlRender::CNtlRender() : m_bEnabled(false) 
{
}


bool CNtlRender::Create(void)
{
	NTL_FUNCTION("CNtlRender::Create");

	NTL_RETURN(true);
}

void CNtlRender::Destroy(void)
{
	NTL_FUNCTION("CNtlRender::Destroy");

	NTL_RETURNVOID();
}

/**
*
*  rendering 객체 update 함수.
*  \param fElapsed는 이전 tick에서 현재 tick의 delta time이다.
*
*/
void CNtlRender::Update(float fElapsed)
{
	NTL_FUNCTION("CNtlRender::Update");

	// input을 얻어온다.
	// logic을 update 한다. (랜더링 객체 포함)

	g_MsgTick.Id = g_EventRenderTick;
	m_fFreeElapsed = fElapsed;

	g_MsgTick.pData = reinterpret_cast<void*>(&m_fFreeElapsed);
	SendMsg(g_MsgTick);
	
	NTL_RETURNVOID();
}

