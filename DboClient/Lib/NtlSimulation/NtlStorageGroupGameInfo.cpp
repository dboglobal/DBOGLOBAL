#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupGameInfo.h"
#include "NtlDebug.h"


CNtlStorageGroupGameInfo::CNtlStorageGroupGameInfo(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_GAMEINFO );
}

CNtlStorageGroupGameInfo::~CNtlStorageGroupGameInfo(void)
{
}

bool CNtlStorageGroupGameInfo::Create()
{
	// GameInfo Category
	CNtlStorageUnit* pGameInfoUnit = NTL_NEW CNtlStorageUnit();
	if( !pGameInfoUnit->Create( eNTL_STORAGE_GAMEINFO ) )
	{
		pGameInfoUnit->Destroy();

		NTL_DELETE( pGameInfoUnit );

		return false;
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_GAMEINFO, pGameInfoUnit ) );

	// Etc Category
	CNtlStorageUnit* pEtcUnit = NTL_NEW CNtlStorageUnit();
	if( !pEtcUnit->Create( eNTL_STORAGE_ETC ) )
	{
		pEtcUnit->Destroy();
		NTL_DELETE( pEtcUnit );
		return false;
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_ETC, pEtcUnit ) );

	// Chatting Category
	CNtlStorageUnit* pChatUnit = NTL_NEW CNtlStorageUnit();
	if( !pChatUnit->Create( eNTL_STORAGE_CHAT ) )
	{
		pChatUnit->Destroy();
		NTL_DELETE( pChatUnit );
		return false;
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_CHAT, pChatUnit ) );

	return true;
}