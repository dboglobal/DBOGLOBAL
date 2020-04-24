#include "precomp_ntlsimulation.h"
#include "NtlStorageGroupAccount.h"

#include "NtlStorageMTContainer.h"
#include "NtlStorageBinarySerializer.h"

#include "NtlDebug.h"

CNtlStorageGroupAccount::CNtlStorageGroupAccount(void)
{
	SetStorageGroupType( eNTL_STORAGE_GROUP_ACCOUNT );
}

CNtlStorageGroupAccount::~CNtlStorageGroupAccount(void)
{
}

bool CNtlStorageGroupAccount::Create()
{
	CNtlStorageUnit* pGraphicUnit = NTL_NEW CNtlStorageUnit();
	if( !pGraphicUnit->Create( eNTL_STORAGE_ACCOUNT ) )
	{
		pGraphicUnit->Destroy();

		NTL_DELETE( pGraphicUnit );
	}
	m_mapStorageUnit.insert( std::make_pair( eNTL_STORAGE_ACCOUNT, pGraphicUnit ) );

	return true;
}