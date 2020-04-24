#include "precomp_ntlsimulation.h"
#include "NtlSobActionSkill.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobActionSkillIcon.h"

DEFINITION_MEMORY_POOL(CNtlSobActionSkill)

CNtlSobActionSkill::CNtlSobActionSkill()
{
	m_bySlotIdx = 0;
	m_pIcon = NTL_NEW CNtlSobActionSkillIcon;
	m_pIcon->SetSobObj(this);
}

CNtlSobActionSkill::~CNtlSobActionSkill()
{
	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}
}

RwBool CNtlSobActionSkill::Create(void)
{
	NTL_FUNCTION("CNtlSobActionSkill::Create");

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name ¼³Á¤.
	SetClassName(SLCLASS_NAME_ACTIONSKILL);

	m_pIcon->Create(); 
	m_pIcon->PostCreate();

	NTL_RETURN(TRUE);
}

void CNtlSobActionSkill::Destroy(void)
{
	NTL_FUNCTION("CNtlSobActionSkill::Destroy");

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}

	CNtlSob::Destroy();

	NTL_RETURNVOID();

}

void CNtlSobActionSkill::Update(RwReal fElapsed)
{
	if(m_pIcon)
		m_pIcon->Update(fElapsed);
}

void CNtlSobActionSkill::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobActionSkillCreate *pActionSkillCreate = reinterpret_cast<SNtlEventSobActionSkillCreate*>(pMsg.pData);

		// skill attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// skill instance events.
		SetSlotIdx(pActionSkillCreate->bySlotIdx);

		// skill icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
}

CNtlSobIcon* CNtlSobActionSkill::GetIcon(void) const 
{
	return m_pIcon;
}

