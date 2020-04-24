#include "precomp_ntlsimulation.h"
#include "NtlSobDynamicObject.h"

// core
#include "NtlDebug.h"

// Sound
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "DynamicObjectTable.h"
#include "NtlSobDynamicObjectAttr.h"
#include "NtlSobProxy.h"
#include "NtlSobDynamicObjectProxy.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL(CNtlSobDynamicObject)

CNtlSobDynamicObject::CNtlSobDynamicObject( VOID )
{
	m_byCurState = eDYNAMIC_OBJECT_STATE_SPAWN;
	// m_byFareRate = 기본값;

	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_TARGET_NEAR_APPROACH );
	SetActorFlags( SLFLAG_ACTOR_HAVE_LIFE);
}

CNtlSobDynamicObject::~CNtlSobDynamicObject( VOID )
{

}

RwBool CNtlSobDynamicObject::Create( VOID )
{
	NTL_FUNCTION( "CNtlSobDynamicObject::Create");

	// TODO: Proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobDynamicObjectProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	if( !CNtlSobActor::Create() )
	{
		NTL_RETURN(FALSE);
	}

	// Class Name
	SetClassName(SLCLASS_NAME_DYNAMIC_OBJECT);

	NTL_RETURN(TRUE);
}

void CNtlSobDynamicObject::Destroy(VOID)
{
	NTL_FUNCTION("CNtlSobDynamicObject::Destroy");

	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

void CNtlSobDynamicObject::Update( RwReal fElapsed )
{
	CNtlSobActor::Update(fElapsed);
}

void CNtlSobDynamicObject::HandleEvents( RWS::CMsg& pMsg )
{
	NTL_FUNCTION("CNtlSobTriggerObject::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobDynamicObjectCreate *pSobCreate = reinterpret_cast<SNtlEventSobDynamicObjectCreate*>(pMsg.pData);

		//// 좌표와 방향 세팅.
		SWorldHeightStuff sHStuff;
		RwV3d vPos = pSobCreate->vLoc;
		Logic_GetWorldHeight(this, &vPos, sHStuff);
		vPos.y = sHStuff.fFinialHeight;

		pSobCreate->vLoc.y = vPos.y;

		//// trigger object attribute events.
		InitState( pSobCreate->byState );

		GetSobAttr()->HandleEvents(pMsg);

		//// proxy setting
		GetSobProxy()->HandleEvents(pMsg);
		SetOwnerID( pSobCreate->hOwner );

		RwV3d vDir = GetSobProxy()->GetDirection();
		SetDirection( &vDir );
		SetPosition( &vPos );
	}
	else if(pMsg.Id == g_EventAnimEnd)
	{
		GetSobProxy()->HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventDynamicObjectUpdateState)
	{
		SNtlEventDynamicObjectUpdateState* pData = reinterpret_cast<SNtlEventDynamicObjectUpdateState*>( pMsg.pData );

		UpdateState( pData->pState->byState );

		GetSobProxy()->HandleEvents(pMsg);
	}

	CNtlSobActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}

void CNtlSobDynamicObject::SetDirection(const RwV3d *pDir)
{
	RwV3dAssignMacro(&m_vDirection, pDir); 
}

void CNtlSobDynamicObject::InitState( RwUInt8 byState )
{
	m_byCurState = byState;
	m_byPreState = byState;
}

void CNtlSobDynamicObject::UpdateState( RwUInt8 byState )
{
	m_byPreState = m_byCurState;
	m_byCurState = byState;
}