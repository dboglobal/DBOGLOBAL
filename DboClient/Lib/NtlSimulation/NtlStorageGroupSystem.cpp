#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupSystem.h"

#include "NtlStorageMTContainer.h"
#include "NtlStorageBinarySerializer.h"

#include "NtlDebug.h"

CNtlStorageGroupSystem::CNtlStorageGroupSystem(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_SYSTEM );
}

CNtlStorageGroupSystem::~CNtlStorageGroupSystem(void)
{
}

bool CNtlStorageGroupSystem::Create()
{
	CNtlStorageUnit* pGraphicUnit = NTL_NEW CNtlStorageUnit();
	if( !pGraphicUnit->Create( eNTL_STORAGE_GRAPHIC ) )
	{
		pGraphicUnit->Destroy();
		NTL_DELETE( pGraphicUnit );
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_GRAPHIC, pGraphicUnit ) );

	CNtlStorageUnit* pSoundUnit = NTL_NEW CNtlStorageUnit();
	if( !pSoundUnit->Create( eNTL_STORAGE_SOUND ) )
	{
		pSoundUnit->Destroy();
		NTL_DELETE( pSoundUnit );
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_SOUND, pSoundUnit ) );

	CNtlStorageUnit* pEtcUnit = NTL_NEW CNtlStorageUnit();
	if (!pEtcUnit->Create(eNTL_STORAGE_SYSTEM_ETC))
	{
		pEtcUnit->Destroy();
		NTL_DELETE(pEtcUnit);
	}
	m_mapStorageUnit.insert(std::make_pair(eNTL_STORAGE_SYSTEM_ETC, pEtcUnit));

	return true;
}