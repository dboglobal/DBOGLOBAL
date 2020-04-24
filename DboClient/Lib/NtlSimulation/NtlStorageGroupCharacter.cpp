#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupCharacter.h"
#include "NtlDebug.h"


CNtlStorageGroupCharacter::CNtlStorageGroupCharacter(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_CHARACTER );
}

CNtlStorageGroupCharacter::~CNtlStorageGroupCharacter(void)
{
}

bool CNtlStorageGroupCharacter::Create()
{
	CNtlStorageUnit* pCharacterUnit = NTL_NEW CNtlStorageUnit();
	if( !pCharacterUnit->Create( eNTL_STORAGE_CHARACTER ) )
	{
		pCharacterUnit->Destroy();
		NTL_DELETE( pCharacterUnit );
		return false;
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_CHARACTER, pCharacterUnit ) );

	CNtlStorageUnit* pEtcUnit = NTL_NEW CNtlStorageUnit();
	if (!pEtcUnit->Create(eNTL_STORAGE_ETC))
	{
		pEtcUnit->Destroy();
		NTL_DELETE(pEtcUnit);
		return false;
	}
	m_mapStorageUnit.insert(std::make_pair(eNTL_STORAGE_ETC, pEtcUnit));

	CNtlStorageUnit* pCharEtcUnit = NTL_NEW CNtlStorageUnit();
	if (!pCharEtcUnit->Create(eNTL_STORAGE_CHARACTER_ETC))
	{
		pCharEtcUnit->Destroy();
		NTL_DELETE(pCharEtcUnit);
	}
	m_mapStorageUnit.insert(std::make_pair(eNTL_STORAGE_CHARACTER_ETC, pCharEtcUnit));

	CNtlStorageUnit* pCharScouterUnit = NTL_NEW CNtlStorageUnit();
	if (!pCharScouterUnit->Create(eNTL_STORAGE_CHARACTER_SCOUTER))
	{
		pCharScouterUnit->Destroy();
		NTL_DELETE(pCharScouterUnit);
	}
	m_mapStorageUnit.insert(std::make_pair(eNTL_STORAGE_CHARACTER_SCOUTER, pCharScouterUnit));



	return true;
}