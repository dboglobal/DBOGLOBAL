#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupQuest.h"

// Shared
#include "NtlSharedType.h"

CNtlStorageGroupQuest::CNtlStorageGroupQuest(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_QUEST );
}

CNtlStorageGroupQuest::~CNtlStorageGroupQuest(void)
{
}

void CNtlStorageGroupQuest::Clear( void )
{
	m_mapQuest.clear();
}

bool CNtlStorageGroupQuest::Load( CNtlStorageSerializer* pS )
{
	Clear();

	RwUInt32 uiKey;

	int nCount = 0;
	for(;;)
	{
		// 무한루프 방지
		if( nCount++ > 1000000 )
			return false;

		if( pS->IsEmpty() )
			return true;

		(*pS) >> uiKey;
		switch( uiKey )
		{
		case dSTORAGE_EOF:
			return true;
		case dSTORAGE_QUEST_MEMORY_INFO:
			LoadQuestInfomation(pS);
			break;
		default:
			break;
		}
	}
}

bool CNtlStorageGroupQuest::Save( CNtlStorageSerializer* pS )
{
	// player quest information
	MapMemoryQuest::iterator the;
	(*pS) << (RwUInt32)dSTORAGE_QUEST_MEMORY_INFO;
	unsigned int uiSize = m_mapQuest.size();
	(*pS) << uiSize;
	for(the = m_mapQuest.begin(); the != m_mapQuest.end(); the++)
	{
		(*pS) << (RwUInt32)dSTORAGE_QUEST_INFO_ID;
		(*pS) << (RwUInt32)(*the).first;
		(*pS) << (RwUInt32)dSTORAGE_QUEST_INFO_NEW;
		(*pS) << (RwUInt32)(*the).second.bIsNew;
		(*pS) << (RwUInt32)dSTORAGE_QUEST_INFO_INDICATE;
		(*pS) << (RwUInt32)(*the).second.bIsIndicated;
	}

	(*pS) << (RwUInt32)dSTORAGE_EOF;

	return true;
}

void CNtlStorageGroupQuest::IndicateQuest( RwUInt32 uiID, RwBool bIndicate )
{
	MapMemoryQuest::iterator it = m_mapQuest.find(uiID);
	if(it == m_mapQuest.end())
	{
		if( bIndicate )
		{
			SMemoryQuestData data;
			data.bIsIndicated = bIndicate;

			m_mapQuest[uiID] = data;
		}		
	}
	else
	{
		if( bIndicate )
		{
			(*it).second.bIsIndicated = bIndicate;
		}
		else
		{
			if( (*it).second.bIsNew )
				(*it).second.bIsIndicated = bIndicate;
			else
				m_mapQuest.erase( it );
		}
	}

}

void CNtlStorageGroupQuest::NewQuest( RwUInt32 uiID, RwBool bNew )
{
	MapMemoryQuest::iterator it = m_mapQuest.find(uiID);
	if(it == m_mapQuest.end())
	{
		if( bNew )
		{
			SMemoryQuestData data;
			data.bIsNew = bNew;

			m_mapQuest[uiID] = data;
		}		
	}
	else
	{
		if( bNew )
		{
			(*it).second.bIsNew = bNew;
		}
		else
		{
			if( (*it).second.bIsIndicated )
				(*it).second.bIsNew = bNew;
			else
				m_mapQuest.erase( it );
		}
	}
}

SMemoryQuestData* CNtlStorageGroupQuest::GetQuestData( RwUInt32 uiID )
{
	MapMemoryQuest::iterator it = m_mapQuest.find(uiID);
	if(it == m_mapQuest.end())
		return NULL;

	SMemoryQuestData* pData = &(*it).second;

	return pData;
}

void CNtlStorageGroupQuest::LoadQuestInfomation( CNtlSerializer* pS )
{
	RwUInt32 bySerializeType;
	RwUInt32 uiSize;
	(*pS) >> uiSize;

	if(uiSize > 0)
	{
		RwUInt32 id = INVALID_DWORD; 
		SMemoryQuestData data;

		for(RwInt32 i = 0; i < (RwInt32)uiSize; i++)
		{
			(*pS) >> bySerializeType;

			if(bySerializeType == dSTORAGE_QUEST_INFO_ID)
			{
				(*pS) >> id;
			}

			(*pS) >> bySerializeType;

			if(bySerializeType == dSTORAGE_QUEST_INFO_NEW)
			{
				(*pS) >> data.bIsNew;
			}

			(*pS) >> bySerializeType;

			if(bySerializeType == dSTORAGE_QUEST_INFO_INDICATE)
			{
				(*pS) >> data.bIsIndicated;
			}

			if(id != INVALID_DWORD)
			{
				m_mapQuest[id] = data;
			}
		}
	}
}