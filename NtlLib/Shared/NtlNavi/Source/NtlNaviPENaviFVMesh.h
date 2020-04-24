#ifndef _NTL_NAVI_PENAVI_FVMESH_H_
#define _NTL_NAVI_PENAVI_FVMESH_H_


#include "NtlNaviMatrix4.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviPENaviFVMesh
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviPENaviFVMesh : public iFaceVertexMesh
{
public:
	iContentChunkInstance*		m_pContChunkInst;
	CNtlNaviMatrix4				m_matWorld;


public:
	CNtlNaviPENaviFVMesh( iContentChunkInstance* pContChunkInst, CNtlNaviMatrix4& matWorld )
	{
		m_pContChunkInst = pContChunkInst;
		m_matWorld = matWorld;
	}

	virtual ~CNtlNaviPENaviFVMesh( void ) { return; }


public:
	virtual tSigned32			faces( void ) const;
	virtual tSigned32			vertices( void ) const;
	virtual tSigned32			vertexIndex( tSigned32 face, tSigned32 vertexInFace ) const;
	virtual tSigned32			vertexX( tSigned32 vertex ) const;
	virtual tSigned32			vertexY( tSigned32 vertex ) const;
	virtual float				vertexZ( tSigned32 vertex ) const;
	virtual tSigned32			faceAttribute( tSigned32 face, tSigned32 attributeIndex ) const;
};


#endif