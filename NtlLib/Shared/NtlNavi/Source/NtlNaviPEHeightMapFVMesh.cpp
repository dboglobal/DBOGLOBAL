#include "precomp_navi.h"
#include "NtlNaviPEHeightMapFVMesh.h"
#include "NtlNaviPEHeightMapFVMesh.h"
#include "NtlNaviODGroupExporter.h"


tSigned32 CNtlNaviPEHeightMapFVMesh::faces( void ) const
{
	return (tSigned32)m_defFaceList.size();
}

tSigned32 CNtlNaviPEHeightMapFVMesh::vertices( void ) const
{
	return m_pParent->GetVertexCnt();
}

tSigned32 CNtlNaviPEHeightMapFVMesh::vertexIndex( tSigned32 face, tSigned32 vertexInFace ) const
{
	return m_defFaceList[face].ver[vertexInFace];
}

tSigned32 CNtlNaviPEHeightMapFVMesh::vertexX( tSigned32 vertex ) const
{
	return (tSigned32) WORLD_COORD_TO_PATH_COORD( m_pParent->GetVertexX( (unsigned int)vertex ) );
}

tSigned32 CNtlNaviPEHeightMapFVMesh::vertexY( tSigned32 vertex ) const
{
	return (tSigned32) WORLD_COORD_TO_PATH_COORD( m_pParent->GetVertexZ( (unsigned int)vertex ) );
}

float CNtlNaviPEHeightMapFVMesh::vertexZ( tSigned32 vertex ) const
{
	float fX, fY, fZ;

	if ( m_pParent->GetVertex( (unsigned int) vertex, fX, fY, fZ ) )
	{
		return (float) WORLD_COORD_TO_PATH_COORD( fY );
	}

	return NAVI_FLT_MAX;
}

tSigned32 CNtlNaviPEHeightMapFVMesh::faceAttribute( tSigned32 /*face*/, tSigned32 /*attributeIndex*/ ) const
{
	return -1;
}

bool CNtlNaviPEHeightMapFVMesh::Build( void )
{
	m_defFaceList.clear();

	tSigned32 nCrossVertCnt = m_pParent->GetCrossVertexCnt();
	tSigned32 nVerticalVertCnt = m_pParent->GetVerticalVertexCnt();

	for ( tSigned32 z = 0; z < nVerticalVertCnt - 1; ++z )
	{
		for ( tSigned32 x = 0; x < nCrossVertCnt - 1; ++x )
		{
			if ( !m_pParent->IsValidVertex( (x  ) + (z  ) * nCrossVertCnt ) ) continue;
			if ( !m_pParent->IsValidVertex( (x+1) + (z  ) * nCrossVertCnt ) ) continue;
			if ( !m_pParent->IsValidVertex( (x  ) + (z+1) * nCrossVertCnt ) ) continue;
			if ( !m_pParent->IsValidVertex( (x+1) + (z+1) * nCrossVertCnt ) ) continue;

			m_defFaceList.push_back( sFace( (x  ) + (z  ) * nCrossVertCnt,
											(x+1) + (z+1) * nCrossVertCnt,
											(x+1) + (z  ) * nCrossVertCnt ) );

			m_defFaceList.push_back( sFace( (x  ) + (z  ) * nCrossVertCnt,
											(x  ) + (z+1) * nCrossVertCnt,
											(x+1) + (z+1) * nCrossVertCnt ) );
		}
	}

	return true;
}
