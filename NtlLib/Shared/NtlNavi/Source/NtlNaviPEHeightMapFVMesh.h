#ifndef _NTL_NAVI_PE_HEIGHTMAP_FVMESH_H_
#define _NTL_NAVI_PE_HEIGHTMAP_FVMESH_H_


class CNtlNaviODGroupExporter;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPEHeightMapFVMesh
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPEHeightMapFVMesh : public iFaceVertexMesh
{
public:
	struct sFace
	{
		sFace( void ) { return; }
		sFace( tSigned32 ver0, tSigned32 ver1, tSigned32 ver2 ) { ver[0] = ver0; ver[1] = ver1; ver[2] = ver2; }
		sFace( const sFace& clVal ) { ver[0] = clVal.ver[0]; ver[1] = clVal.ver[1]; ver[2] = clVal.ver[2]; }

		tSigned32						ver[3];
	};
	typedef std::vector< sFace > defFaceList;

public:
	CNtlNaviODGroupExporter*			m_pParent;
	defFaceList							m_defFaceList;

public:
	CNtlNaviPEHeightMapFVMesh( CNtlNaviODGroupExporter* pParent )
	{
		m_pParent = pParent;

		m_defFaceList.reserve( 4096 );
	}

	virtual ~CNtlNaviPEHeightMapFVMesh( void ) { return; }

public:
	virtual tSigned32					faces( void ) const;
	virtual tSigned32					vertices( void ) const;
	virtual tSigned32					vertexIndex( tSigned32 face, tSigned32 vertexInFace ) const;
	virtual tSigned32					vertexX( tSigned32 vertex ) const;
	virtual tSigned32					vertexY( tSigned32 vertex ) const;
	virtual float						vertexZ( tSigned32 vertex ) const;
	virtual tSigned32					faceAttribute( tSigned32 face, tSigned32 attributeIndex ) const;

	bool								Build( void );
};


#endif