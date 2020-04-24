#include "precomp_ntlsimulation.h"
#include "NtlSobHTBSkill.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobHTBSkillIcon.h"

DEFINITION_MEMORY_POOL(CNtlSobHTBSkill)

CNtlSobHTBSkill::CNtlSobHTBSkill()
{
	m_bySlotIdx = 0;
	m_pIcon = NTL_NEW CNtlSobHTBSkillIcon;
	m_pIcon->SetSobObj(this);
}

CNtlSobHTBSkill::~CNtlSobHTBSkill()
{
	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}
}

RwBool CNtlSobHTBSkill::Create(void)
{
	NTL_FUNCTION("CNtlSobHTBSkill::Create");

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name ¼³Á¤.
	SetClassName(SLCLASS_NAME_HTBSKILL);

	m_pIcon->Create(); 
	m_pIcon->PostCreate();

	NTL_RETURN(TRUE);
}

void CNtlSobHTBSkill::Destroy(void)
{
	NTL_FUNCTION("CNtlSobHTBSkill::Destroy");

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}

	CNtlSob::Destroy();

	NTL_RETURNVOID();

}

void CNtlSobHTBSkill::Update(RwReal fElapsed)
{
	if(m_pIcon)
		m_pIcon->Update(fElapsed);
}

void CNtlSobHTBSkill::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobHTBSkillCreate *pHTBSkillCreate = reinterpret_cast<SNtlEventSobHTBSkillCreate*>(pMsg.pData);

		SetOwnerID( pHTBSkillCreate->uiOwnerID );

		// skill attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// skill instance events.
		SetSlotIdx(pHTBSkillCreate->bySlotIdx);

		// skill icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
}

CNtlSobIcon* CNtlSobHTBSkill::GetIcon(void) const 
{
	return m_pIcon;
}

