#include "precomp_ntlsimulation.h"
#include "NtlStorageBinarySerializer.h"

CNtlStorageBinarySerializer::CNtlStorageBinarySerializer()
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_BINARY );
}

CNtlStorageBinarySerializer::CNtlStorageBinarySerializer( int nBufferSize, int nGlowSize )
: CNtlStorageSerializer( nBufferSize, nGlowSize )
{
	SetStorageSerializerType( eNTL_STORAGE_SERIALIZER_BINARY );
}

CNtlStorageBinarySerializer::~CNtlStorageBinarySerializer()
{

}