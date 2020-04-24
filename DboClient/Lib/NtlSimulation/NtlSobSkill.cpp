#include "precomp_ntlsimulation.h"
#include "NtlSobSkill.h"


// simulation
#include "NtlSLEvent.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobSkillIcon.h"

DEFINITION_MEMORY_POOL(CNtlSobSkill)

CNtlSobSkill::CNtlSobSkill()
{
	m_bySlotIdx = 0;
	m_pIcon = NTL_NEW CNtlSobSkillIcon;
	m_pIcon->SetSobObj(this);
}

CNtlSobSkill::~CNtlSobSkill()
{
	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}
}

RwBool CNtlSobSkill::Create(void)
{
	NTL_FUNCTION("CNtlSobSkill::Create");

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name ¼³Á¤.
	SetClassName(SLCLASS_NAME_SKILL);

	m_pIcon->Create(); 
	m_pIcon->PostCreate();
	
	NTL_RETURN(TRUE);
}

void CNtlSobSkill::Destroy(void)
{
	NTL_FUNCTION("CNtlSobSkill::Destroy");

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}
	
	CNtlSob::Destroy();

	NTL_RETURNVOID();

}

void CNtlSobSkill::Update(RwReal fElapsed)
{
	if(m_pIcon)
		m_pIcon->Update(fElapsed);
}

void CNtlSobSkill::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobSkillCreate *pSkillCreate = reinterpret_cast<CNtlEventSobSkillCreate*>(pMsg.pData);

		// ownerID
		SetOwnerID( pSkillCreate->uiOwnerID );

		// skill attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// skill instance events.
		SetSlotIdx(pSkillCreate->bySlodIdx);

		// skill icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
	else if(pMsg.Id == g_EventSobSkillUpgrade)
	{
		GetSobAttr()->HandleEvents(pMsg);
	}
}

CNtlSobIcon* CNtlSobSkill::GetIcon(void) const 
{
	return m_pIcon;
}

