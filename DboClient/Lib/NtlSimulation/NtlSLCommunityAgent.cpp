#include "precomp_ntlsimulation.h"
#include "NtlSLCommunityAgent.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"


NtlSLCommunityAgent::NtlSLCommunityAgent()
:m_eAgentType(NONE_COMMUNITY_AGENT)
{
}

NtlSLCommunityAgent::~NtlSLCommunityAgent()
{

}

RwBool NtlSLCommunityAgent::Create(eCommunityAgentType	eAgentType)
{
	NTL_FUNCTION("NtlSLCommunityAgent::Create");

	m_eAgentType = eAgentType;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	m_pCommunityGroup[COMMUNITY_AGENT_PARTY]		= pAvatar->GetParty();
	m_pCommunityGroup[COMMUNITY_AGENT_GUILD]		= pAvatar->GetGuild();
	m_pCommunityGroup[COMMUNITY_AGENT_BLACKLIST]	= pAvatar->GetBlackList();
	m_pCommunityGroup[COMMUNITY_AGENT_FRIEND]		= pAvatar->GetFriendList();

	NTL_RETURN(TRUE);
}

VOID NtlSLCommunityAgent::Destroy()
{
	NTL_FUNCTION("NtlSLCommunityAgent::Destroy");

	if( m_eAgentType != NONE_COMMUNITY_AGENT )
		m_pCommunityGroup[m_eAgentType]->Destroy();

	NTL_RETURNVOID();
}

VOID NtlSLCommunityAgent::SetAgentType(eCommunityAgentType eAgentType)
{
	NTL_ASSERT(m_eAgentType == NONE_COMMUNITY_AGENT, "NtlSLCommunityAgent::SetAgentType, Exist agent type  : " << m_eAgentType);
	m_eAgentType = eAgentType;
}

eCommunityAgentType NtlSLCommunityAgent::GetAgentType()
{
	return m_eAgentType;
}

VOID NtlSLCommunityAgent::Sort(RwUInt8 byDataType, RwBool bAscent)
{
	if( m_eAgentType >= NUM_COMMUNITY_AGENT )
		return;

	m_pCommunityGroup[m_eAgentType]->Sort(byDataType, bAscent);
}