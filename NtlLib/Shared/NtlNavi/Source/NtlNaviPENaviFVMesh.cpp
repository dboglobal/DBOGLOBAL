#include "precomp_navi.h"
#include "NtlNaviPENaviFVMesh.h"


tSigned32 CNtlNaviPENaviFVMesh::faces( void ) const
{
	return m_pContChunkInst->getGroundPointer()->faces();
}

tSigned32 CNtlNaviPENaviFVMesh::vertices( void ) const
{
	return m_pContChunkInst->getGroundPointer()->vertices();
}

tSigned32 CNtlNaviPENaviFVMesh::vertexIndex( tSigned32 face, tSigned32 vertexInFace ) const
{
	if ( 1 == vertexInFace )		vertexInFace = 2;
	else if ( 2 == vertexInFace )	vertexInFace = 1;

	return m_pContChunkInst->getGroundPointer()->vertexIndex( face, vertexInFace );
}

tSigned32 CNtlNaviPENaviFVMesh::vertexX( tSigned32 vertex ) const
{
	CNtlNaviVector4 vPos( PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexX( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexZ( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexY( vertex ) ),
						  1 );

	vPos = vPos * m_matWorld;

	return (tSigned32) WORLD_COORD_TO_PATH_COORD( vPos.GetElem( 0 ) );
}

tSigned32 CNtlNaviPENaviFVMesh::vertexY( tSigned32 vertex ) const
{
	CNtlNaviVector4 vPos( PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexX( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexZ( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexY( vertex ) ),
						  1 );

	vPos = vPos * m_matWorld;

	return (tSigned32) WORLD_COORD_TO_PATH_COORD( vPos.GetElem( 2 ) );
}

float CNtlNaviPENaviFVMesh::vertexZ( tSigned32 vertex ) const
{
	CNtlNaviVector4 vPos( PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexX( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexZ( vertex ) ),
						  PATH_COORD_TO_WORLD_COORD( m_pContChunkInst->getGroundPointer()->vertexY( vertex ) ),
						  1 );

	vPos = vPos * m_matWorld;

	return (float) WORLD_COORD_TO_PATH_COORD( vPos.GetElem( 1 ) );
}

tSigned32 CNtlNaviPENaviFVMesh::faceAttribute( tSigned32 face, tSigned32 attributeIndex ) const
{
	return m_pContChunkInst->getGroundPointer()->faceAttribute( face, attributeIndex );
}
