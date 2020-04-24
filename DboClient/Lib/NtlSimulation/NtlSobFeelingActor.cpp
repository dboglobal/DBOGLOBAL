#include "precomp_ntlsimulation.h"
#include "NtlSobFeelingActor.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlBuffContainer.h"
#include "NtlSobInfluence.h"
#include "NtlSobAttr.h"
#include "NtlSobProxy.h"


CNtlSobFeelingActor::CNtlSobFeelingActor()
{
	m_pBuffContainer	= NULL;
	m_pPetBuffer		= NULL;	
}


CNtlSobFeelingActor::~CNtlSobFeelingActor()
{
}

void CNtlSobFeelingActor::ServerChangeOut(void)
{
	if(m_pBuffContainer)
	{
		m_pBuffContainer->Destroy();
		NTL_DELETE(m_pBuffContainer);
	}

	if(m_pPetBuffer)
	{
		m_pPetBuffer->Destroy();
		NTL_DELETE(m_pPetBuffer);
	}

	CNtlSobActor::ServerChangeOut();

}

void CNtlSobFeelingActor::ServerChangeIn(void)
{
	m_pBuffContainer = NTL_NEW CNtlBuffContainer;	
	m_pBuffContainer->SetObj(this);
	m_pBuffContainer->Create();

	m_pPetBuffer = NTL_NEW CNtlPetBuffer;
	m_pPetBuffer->Create();

	CNtlSobActor::ServerChangeIn();
}

RwBool CNtlSobFeelingActor::Create(void)
{
	NTL_FUNCTION("CNtlSobFeelingActor::Create");

	m_pBuffContainer = NTL_NEW CNtlBuffContainer;	
	m_pBuffContainer->SetObj(this);
	if(!m_pBuffContainer->Create())
	{
		NTL_RETURN(FALSE);
	}

	m_pPetBuffer = NTL_NEW CNtlPetBuffer;
	if(!m_pPetBuffer->Create())
	{
		NTL_RETURN(FALSE);
	}

	NTL_RETURN(CNtlSobActor::Create());
}

void CNtlSobFeelingActor::Destroy(void)
{
	if(m_pBuffContainer)
	{
		m_pBuffContainer->Destroy();
		NTL_DELETE(m_pBuffContainer);
	}

	if(m_pPetBuffer)
	{
		m_pPetBuffer->Destroy();
		NTL_DELETE(m_pPetBuffer);
	}

	CNtlSobActor::Destroy();
}



void CNtlSobFeelingActor::Update(RwReal fElapsed)
{
	CNtlSobActor::Update(fElapsed);
}

CNtlSobInfluence* CNtlSobFeelingActor::CreateSobInfluence(void)
{
	CNtlSobInfluence *pSobInfluence = NTL_NEW CNtlSobInfluence;
	pSobInfluence->Create();
	pSobInfluence->PostCreate();
	return pSobInfluence;
}

void CNtlSobFeelingActor::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobFeelingActor::HandleEvents");

	if(pMsg.Id == g_EventSobBuffAdd)
	{
		m_pBuffContainer->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if(pMsg.Id == g_EventSobBuffDrop)
	{
		m_pBuffContainer->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if(pMsg.Id == g_EventSobBuffRefreshAll)
	{
		m_pBuffContainer->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if (pMsg.Id == g_EventSobBuffActivate)
	{
		m_pBuffContainer->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if (pMsg.Id == g_EventSobBuffAutoRecover)
	{
		m_pBuffContainer->HandleEvents(pMsg);
		NTL_RETURNVOID();
	}
	else if(pMsg.Id == g_EventSobFainting)
	{
		SobFaintingEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobEquipChange)
	{
		SobEquipChangeEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSubWeaponActive)
	{
		SobSubWeaponActiveEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSubWeaponDeActive)
	{
		SobSubWeaponDeActiveEventHandler(pMsg);
	}
    else if(pMsg.Id == g_EventAnimPostEffect)
    {
        SobPostEffectEventHandler(pMsg);
    }
    else if(pMsg.Id == g_EventAnimColorChange)
    {
        CNtlSobProxy* pSobProxy = GetSobProxy();
        if(pSobProxy)
            pSobProxy->HandleEvents(pMsg);
    }

	CNtlSobActor::HandleEvents(pMsg);

	NTL_RETURNVOID();
}


void CNtlSobFeelingActor::SetPosition(const RwV3d *pPos)
{
	if( (GetFlags() & SLFLAG_CAN_GROUND_FLY) && (GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE) )
	{
		RwV3d vPos;
		RwV3dAssignMacro(&vPos, pPos);
		CNtlSobMoveableAttr *pSobMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>( GetSobAttr() );
		vPos.y += pSobMoveableAttr->GetFlyHeight();

		CNtlSob::SetPosition(&vPos);
	}
	else
	{
		CNtlSob::SetPosition(pPos);
	}
}


void CNtlSobFeelingActor::SobFaintingEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);
}


void CNtlSobFeelingActor::SobEquipChangeEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);
}

void CNtlSobFeelingActor::SobSubWeaponActiveEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);
}

void CNtlSobFeelingActor::SobSubWeaponDeActiveEventHandler(RWS::CMsg &pMsg)
{
	CNtlSobProxy *pSobProxy = GetSobProxy();
	if(pSobProxy == NULL)
		return;

	pSobProxy->HandleEvents(pMsg);
}

void CNtlSobFeelingActor::SobPostEffectEventHandler( RWS::CMsg& pMsg ) 
{
    // 실제 구현은 프록시에서 한다.
    CNtlSobProxy* pSobProxy = GetSobProxy();
    if(!pSobProxy)
        return;

    pSobProxy->HandleEvents(pMsg);
}

