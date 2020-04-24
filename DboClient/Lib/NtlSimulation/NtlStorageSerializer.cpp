#include "precomp_ntlsimulation.h"
#include "NtlStorageSerializer.h"


CNtlStorageSerializer::CNtlStorageSerializer()
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_INVALID );
}

CNtlStorageSerializer::CNtlStorageSerializer( int nBufferSize, int nGlowSize )
: CNtlSerializer( nBufferSize, nGlowSize )
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_INVALID );
}

CNtlStorageSerializer::~CNtlStorageSerializer()
{

}

eNTL_STORAGE_SERIALIZER_TYPE CNtlStorageSerializer::GetStorageSerializerType()
{
	return m_eType;
}

void CNtlStorageSerializer::SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_TYPE eType )
{
	m_eType = eType;
}