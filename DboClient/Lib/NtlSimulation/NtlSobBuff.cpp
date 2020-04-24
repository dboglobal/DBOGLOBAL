#include "precomp_ntlsimulation.h"
#include "NtlSobBuff.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlInstanceEffect.h"
#include "NtlPLSceneManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobManager.h"
#include "NtlSobBuffAttr.h"
#include "NtlSobBuffIcon.h"


DEFINITION_MEMORY_POOL(CNtlSobBuff)


CNtlSobBuff::CNtlSobBuff()
{
	m_pIcon = NTL_NEW CNtlSobBuffIcon;
	m_pIcon->SetSobObj(this);

	m_iVisualId = NTL_INVALID_VALUE;
}

CNtlSobBuff::~CNtlSobBuff()
{
}


RwBool CNtlSobBuff::Create(void)
{
	NTL_FUNCTION("CNtlSobBuff::Create")

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_BUFF);

	// icon 생성.
	m_pIcon->Create(); 
	m_pIcon->PostCreate();

	NTL_RETURN(TRUE);
}

void CNtlSobBuff::Destroy(void)
{
	NTL_FUNCTION("CNtlSobBuff::Destroy")

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy(); 

		NTL_DELETE( m_pIcon );
	}

	std::list<CNtlPLEntity*>::iterator it;
	for(it = m_listPLEntity.begin(); it != m_listPLEntity.end(); ++it)
	{
		CNtlPLEntity *pPLEntity = (*it);
		GetSceneManager()->DeleteEntity(pPLEntity); 
	}

	m_listPLEntity.clear(); 

	CNtlSob::Destroy(); 

	NTL_RETURNVOID();
}

void CNtlSobBuff::Update(RwReal fElapsed)
{
	if( m_pIcon )
		m_pIcon->Update( fElapsed );
}

void CNtlSobBuff::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobBuff::HandleEvents")

	if(pMsg.Id == g_EventSobCreate)
	{
		// buff attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// buff icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
	else if (pMsg.Id == g_EventSobBuffActivate)
	{
		// buff icon events.
		m_pIcon->HandleEvents(pMsg);
	}
	else if (pMsg.Id == g_EventSobBuffAutoRecover)
	{
		// buff icon events.
		m_pIcon->HandleEvents(pMsg);
	}

	NTL_RETURNVOID();
}

CNtlSobIcon* CNtlSobBuff::GetIcon(void) const
{
	return m_pIcon;
}

void CNtlSobBuff::AddPLEntity(CNtlPLEntity *pPLEntity)
{
	if(pPLEntity->GetClassType() == PLENTITY_EFFECT)
	{
		CNtlInstanceEffect *pPLEffect = reinterpret_cast<CNtlInstanceEffect*>(pPLEntity);
		if(!pPLEffect->IsAutoDelete())
		{
			m_listPLEntity.push_back(pPLEntity);
		}
	}
	else
	{
		m_listPLEntity.push_back(pPLEntity);
	}
}