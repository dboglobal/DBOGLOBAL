#include "precomp_ntlsimulation.h"
#include "NtlSobEventObject.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSobEventObjectProxy.h"

DEFINITION_MEMORY_POOL(CNtlSobEventObject)

CNtlSobEventObject::CNtlSobEventObject(VOID)
{
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_NOT_TARGETED);
}

CNtlSobEventObject::~CNtlSobEventObject(VOID)
{
}

RwBool CNtlSobEventObject::Create(VOID)
{
	NTL_FUNCTION(__FUNCTION__);

	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobEventObjectProxy();
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if(!CNtlSobActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_EVENT_OBJECT);

	NTL_RETURN(TRUE);
}

void CNtlSobEventObject::Destroy(VOID)
{
	NTL_FUNCTION(__FUNCTION__);

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

void CNtlSobEventObject::Update( RwReal fElapsed )
{
	CNtlSobActor::Update(fElapsed);
}

void CNtlSobEventObject::HandleEvents( RWS::CMsg& pMsg )
{
	NTL_FUNCTION(__FUNCTION__);

	if(pMsg.Id == g_EventSobCreate)
	{
		// Event object attribute events.
		//GetSobAttr()->HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		// 좌표와 방향 세팅.
		//RwV3dAssignMacro(&m_vPos, &pSobCreate->vLoc);		
	}

	else if(pMsg.Id == g_EventSobDelete)
	{
		
	}

	CNtlSobActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}
