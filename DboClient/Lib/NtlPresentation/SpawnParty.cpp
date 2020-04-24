#include "precomp_ntlpresentation.h"
#include "SpawnParty.h"
#include "NtlDebug.h"
#include "NtlPLCharacter.h"
#include "Spawn.h"


DWORD									CSpawnPartyContainer::m_PartyIdx = 0;
CSpawnPartyContainer::SPAWN_PARTY_MAP	CSpawnPartyContainer::m_mapSpawnParty;
RwInt32									CSpawnPartyContainer::m_PartyColorIdx;
RwRGBA									CSpawnPartyContainer::m_PartyColor[7];


VOID CSpawnPartyContainer::Create(void)
{
	m_PartyIdx			= 0;
	m_PartyColorIdx		= -1;

	RwUInt8 PartyColor[7][3] = {{255, 0, 0}, {255, 128, 64}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}, {0, 0, 64}, {128, 0, 128}};
	for(RwUInt8 i = 0; i < 7; ++i)
	{
		m_PartyColor[i].red		= PartyColor[i][0];
		m_PartyColor[i].green	= PartyColor[i][1];
		m_PartyColor[i].blue	= PartyColor[i][2];
		m_PartyColor[i].alpha	= 255;
	}

	m_mapSpawnParty.clear();
}

VOID CSpawnPartyContainer::Destroy(void)
{
	SPAWN_PARTY_ITER The = m_mapSpawnParty.begin();
	while(The != m_mapSpawnParty.end())
	{
		CSpawnParty* pSpawnParty = The->second;
		NTL_DELETE(pSpawnParty);
		++The;
	}
	m_mapSpawnParty.clear();
}

CSpawnParty* CSpawnPartyContainer::GetSpawnParty(DWORD Index)
{
	return m_mapSpawnParty.find(Index)->second;
}

RwRGBA CSpawnPartyContainer::GetNextPartyColor()
{
	++m_PartyColorIdx;

	if(m_PartyColorIdx >= 7)
	{
		m_PartyColorIdx = 0;
	}

	return m_PartyColor[m_PartyColorIdx];
}

VOID CSpawnPartyContainer::Save(FILE* _pFile)
{
	fwrite(&m_PartyIdx, sizeof(DWORD), 1, _pFile);
	fwrite(&m_PartyColorIdx, sizeof(RwInt32), 1, _pFile);

	// Party count
	RwInt32 Num = m_mapSpawnParty.size();
	fwrite(&Num, sizeof(RwInt32), 1, _pFile);

	SPAWN_PARTY_ITER The = m_mapSpawnParty.begin();
	while(The != m_mapSpawnParty.end())
	{
		// Party index
		fwrite(&The->first, sizeof(DWORD), 1, _pFile);

		// Party prop.
		The->second->Save(_pFile);

		++The;
	}
}

VOID CSpawnPartyContainer::Load(FILE* _pFile)
{
	fread(&m_PartyIdx, sizeof(DWORD), 1, _pFile);
	fread(&m_PartyColorIdx, sizeof(RwInt32), 1, _pFile);

	// Party count
	RwInt32	Num = 0;
	fread(&Num, sizeof(RwInt32), 1, _pFile);

	for(RwInt32 i = 0; i < Num; ++i)
	{
		// Party index
		DWORD PartyIndex;
		fread(&PartyIndex, sizeof(DWORD), 1, _pFile);

		// Party prop.
		RwRGBA RGBA;
		fread(&RGBA.red, sizeof(RwUInt8), 1, _pFile);
		fread(&RGBA.green, sizeof(RwUInt8), 1, _pFile);
		fread(&RGBA.blue, sizeof(RwUInt8), 1, _pFile);
        RGBA.alpha = 255;
		
		// Follow
		RwBool Follow;
		fread(&Follow, sizeof(RwBool), 1, _pFile);

		DWORD CurPartyIdx;
		fread(&CurPartyIdx, sizeof(DWORD), 1, _pFile);

		// Create a party
		CSpawnParty* pSpawnParty = NTL_NEW CSpawnParty(PartyIndex);
		pSpawnParty->SetColor(RGBA);
		pSpawnParty->SetFollow(Follow);
	}
}

CSpawnParty::CSpawnParty(void)
{
	m_pLeaderSpawn	= NULL;
	m_IsFollow		= FALSE;
	m_pPath			= NULL;
	m_CurPartyIdx	= CSpawnPartyContainer::GetNextPartyIdx();

	CSpawnPartyContainer::m_mapSpawnParty.insert(CSpawnPartyContainer::SPAWN_PARTY_VALUE(m_CurPartyIdx, this));
}

CSpawnParty::CSpawnParty(DWORD _PartyIdx)
{
	m_pLeaderSpawn	= NULL;
	m_IsFollow		= FALSE;
	m_pPath			= NULL;
	m_CurPartyIdx	= _PartyIdx;

	CSpawnPartyContainer::m_mapSpawnParty.insert(CSpawnPartyContainer::SPAWN_PARTY_VALUE(_PartyIdx, this));
}

CSpawnParty::~CSpawnParty(void)
{
	CSpawnPartyContainer::SPAWN_PARTY_ITER The = CSpawnPartyContainer::m_mapSpawnParty.begin();
	while(The != CSpawnPartyContainer::m_mapSpawnParty.end())
	{
		if(The->second == this && !The->second->GetPartyMemberCnt())
		{
			CSpawnPartyContainer::m_mapSpawnParty.erase(The);
			break;
		}

		++The;
	}
}

VOID CSpawnParty::InsertSpawn(CSpawn* _pSpawn)
{
	m_vecSpawn.push_back(_pSpawn);
}

RwInt32 CSpawnParty::DeleteSpawn(CSpawn* _pSpawn)
{
	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		if(_pSpawn == m_vecSpawn[i])
		{
			// Party leader's gonna be deleted; consider leader pointer, follow function
			if(m_pLeaderSpawn == _pSpawn)
			{
				m_pLeaderSpawn	= NULL;
				m_IsFollow		= FALSE;
			}

			m_vecSpawn.erase(m_vecSpawn.begin() + i);

			break;
		}
	}

	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		m_vecSpawn[i]->RefreshNameBox();
	}

	return m_vecSpawn.size();
}

VOID CSpawnParty::SetLeader(CSpawn* _pSpawn, RwBool _IsLeader)
{
	m_IsFollow = FALSE;

	if(_IsLeader)
	{
		this->SetLeader(_pSpawn, FALSE);
		m_pLeaderSpawn = _pSpawn;
	}
	else
	{
		m_pLeaderSpawn	= NULL;
	}

	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		m_vecSpawn[i]->RefreshNameBox();
	}
}

RwBool CSpawnParty::RefreshFollowDist4AllMembers()
{
	if(!m_pLeaderSpawn)
	{
		return FALSE;
	}

	RwV3d	LeaderPos	= static_cast<CNtlPLCharacter*>(m_pLeaderSpawn->GetNtlPLEntity())->GetPosition();
	RwV3d	CurMemberPos;
	RwV3d	FollowDist	= LeaderPos;

	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		CSpawn* pCurSpawn = m_vecSpawn[i];

		if(pCurSpawn == m_pLeaderSpawn)
		{
			continue;
		}

		CurMemberPos = static_cast<CNtlPLCharacter*>(pCurSpawn->GetNtlPLEntity())->GetPosition();

		FollowDist.x = static_cast<RwReal>(fabs(FollowDist.x - CurMemberPos.x));
		FollowDist.y = 0.0f;
		FollowDist.z = static_cast<RwReal>(fabs(FollowDist.z - CurMemberPos.z));

		pCurSpawn->SetFollowDist(FollowDist);
	}

	return TRUE;
}

VOID CSpawnParty::SetFollow(RwBool _IsFollow)
{
	if(_IsFollow)
	{
		m_IsFollow = TRUE;
	}
	else
	{
		m_IsFollow = FALSE;
	}

	for(RwUInt32 i = 0; i < m_vecSpawn.size(); ++i)
	{
		m_vecSpawn[i]->RefreshNameBox();
	}
}

RwInt32 CSpawnParty::GetPartyIdx()
{
	CSpawnPartyContainer::SPAWN_PARTY_ITER The = CSpawnPartyContainer::m_mapSpawnParty.begin();
	while(The != CSpawnPartyContainer::m_mapSpawnParty.end())
	{
		if(The->second == this)
		{
			return The->first;
		}

		++The;
	}

	return -1;
}

VOID CSpawnParty::Save(FILE* _pFile)
{
	fwrite(&m_RGBA.red, sizeof(RwUInt8), 1, _pFile);
	fwrite(&m_RGBA.green, sizeof(RwUInt8), 1, _pFile);
	fwrite(&m_RGBA.blue, sizeof(RwUInt8), 1, _pFile);

	fwrite(&m_IsFollow, sizeof(RwBool), 1, _pFile);
	fwrite(&m_CurPartyIdx, sizeof(DWORD), 1, _pFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMergeSpawnParty::CMergeSpawnParty()
{
	bIsFollow		= FALSE;
	dwPartyIdx		= (DWORD)-1;
}

CMergeSpawnParty::~CMergeSpawnParty()
{
}

RwBool CMergeSpawnParty::Load(FILE* pFile)
{
	fread(&dwPartyIdx, sizeof(DWORD), 1, pFile);
	
	fread(&rgba.red, sizeof(RwUInt8), 1, pFile);
	fread(&rgba.green, sizeof(RwUInt8), 1, pFile);
	fread(&rgba.blue, sizeof(RwUInt8), 1, pFile);

	fread(&bIsFollow, sizeof(RwBool), 1, pFile);

	fread(&dwCurPartyIdx, sizeof(DWORD), 1, pFile);	

	return TRUE;
}

RwBool CMergeSpawnParty::Save(FILE* pFile)
{
	fwrite(&dwPartyIdx, sizeof(DWORD), 1, pFile);

	fwrite(&rgba.red, sizeof(RwUInt8), 1, pFile);
	fwrite(&rgba.green, sizeof(RwUInt8), 1, pFile);
	fwrite(&rgba.blue, sizeof(RwUInt8), 1, pFile);

	fwrite(&bIsFollow, sizeof(RwBool), 1, pFile);

	fwrite(&dwCurPartyIdx, sizeof(DWORD), 1, pFile);

	return TRUE;
}