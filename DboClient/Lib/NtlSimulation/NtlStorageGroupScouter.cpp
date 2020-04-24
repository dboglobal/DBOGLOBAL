#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupScouter.h"

// Shared
#include "NtlSharedType.h"
#include "NtlCoreUtil.h"

CNtlStorageGroupScouter::CNtlStorageGroupScouter(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_SCOUTER );
}

CNtlStorageGroupScouter::~CNtlStorageGroupScouter(void)
{
}

bool CNtlStorageGroupScouter::Load( CNtlStorageSerializer* pS )
{
	// Binary 버퍼가 아니면 모드가 아니면 읽지 못한다.
	if( pS->GetStorageSerializerType() != eNTL_STORAGE_SERIALIZER_BINARY )
		return false;

	Clear();

	RwUInt32	uiSerializeType;

	int nCount = 0;
	for(;;)
	{
		// 무한루프 방지
		if( ++nCount > 1000000 )
			return false;

		if( pS->IsEmpty() )
			return true;

		(*pS) >> uiSerializeType;
		switch(uiSerializeType)
		{
		case dSTORAGE_EOF:
			return TRUE;			
		case dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO:
			LoadScouterInformation(pS, TRUE);
			break;
		case dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO:
			LoadScouterInformation(pS, FALSE);
			break;
		default:
			break;
		}
	}
}

bool CNtlStorageGroupScouter::Save( CNtlStorageSerializer* pS )
{
	RwUInt32 uiSize;
	MapMemoryScouter::iterator it;

	// monster scouter information
	(*pS) << (RwUInt32)dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO;
	uiSize = m_mapMobScouter.size();

	(*pS) << uiSize;
	for(it = m_mapMobScouter.begin(); it != m_mapMobScouter.end(); it++)
	{
		(*pS) << (RwUInt32)dSTORAGE_SCOUTER_INFO_TBLID;
		(*pS) << (RwUInt32)(*it).first;
		(*pS) << (RwUInt32)dSTORAGE_SCOUTER_INFO_POWER;
		(*pS) << (RwUInt32)(*it).second.uiPower;
	}

	// player scouter information
	(*pS) << (RwUInt32)dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO;
	uiSize = m_mapPlayerScouter.size();
	(*pS) << uiSize;
	for(it = m_mapPlayerScouter.begin(); it != m_mapPlayerScouter.end(); it++)
	{
		(*pS) << (RwUInt32)dSTORAGE_SCOUTER_INFO_TBLID;
		(*pS) << (RwUInt32)(*it).first;
		(*pS) << (RwUInt32)dSTORAGE_SCOUTER_INFO_POWER;
		(*pS) << (RwUInt32)(*it).second.uiPower;
	}

	(*pS) << (RwInt32)dSTORAGE_EOF;

	return true;
}

void CNtlStorageGroupScouter::Clear()
{
	m_mapMobScouter.clear();
	m_mapPlayerScouter.clear();
}

void CNtlStorageGroupScouter::LoadScouterInformation(CNtlSerializer* pS, RwBool bMonster)	
{
	RwUInt32 bySerializeType;
	RwUInt32 uiSize;
	(*pS) >> uiSize;

	if(uiSize > 0)
	{
		RwUInt32 tblid = INVALID_TBLIDX; 
		SMemoryScouterData data;

		for(RwInt32 i = 0; i < (RwInt32)uiSize; i++)
		{
			(*pS) >> bySerializeType;

			if(bySerializeType == dSTORAGE_SCOUTER_INFO_TBLID)
			{
				(*pS) >> tblid;
			}

			(*pS) >> bySerializeType;

			if(bySerializeType == dSTORAGE_SCOUTER_INFO_POWER)
			{
				(*pS) >> data.uiPower;
			}

			if(tblid != INVALID_TBLIDX)
			{
				if(bMonster)
					m_mapMobScouter[tblid] = data;
				else
					m_mapPlayerScouter[tblid] = data;
			}				
		}
	}
}

void CNtlStorageGroupScouter::UseMonsterScouter( RwUInt32 uiGroupId, RwUInt32 uiPower )
{
	MapMemoryScouter::iterator it = m_mapMobScouter.find(uiGroupId);
	if(it == m_mapMobScouter.end())
	{
		SMemoryScouterData data;
		data.uiPower = uiPower;

		m_mapMobScouter[uiGroupId] = data;
	}
	else
	{
		if( (*it).second.uiPower < uiPower )
			(*it).second.uiPower = uiPower;
	}
}

SMemoryScouterData* CNtlStorageGroupScouter::IsUseMonsterScouter( RwUInt32 uiGroupId )
{
	MapMemoryScouter::iterator it = m_mapMobScouter.find(uiGroupId);
	if(it == m_mapMobScouter.end())
		return NULL;

	SMemoryScouterData* pData = &(*it).second;

	return pData;
}

void CNtlStorageGroupScouter::UsePlayerScouter( RwUInt32 uiCharId, RwUInt32 uiPower )
{
	MapMemoryScouter::iterator it = m_mapPlayerScouter.find(uiCharId);
	if(it == m_mapPlayerScouter.end())
	{
		SMemoryScouterData data;
		data.uiPower = uiPower;	

		m_mapPlayerScouter[uiCharId] = data;
	}
	else
	{
		(*it).second.uiPower = uiPower;
	}
}

SMemoryScouterData* CNtlStorageGroupScouter::IsUsePlayerScouter( RwUInt32 uiCharId )
{
	MapMemoryScouter::iterator it = m_mapPlayerScouter.find(uiCharId);
	if(it == m_mapPlayerScouter.end())
		return NULL;

	SMemoryScouterData* pData = &(*it).second;

	return pData;
}