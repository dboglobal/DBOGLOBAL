/*****************************************************************************
* File			: NtlMeshFederation.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 14
* Abstract		: Pathengine iMeshFederation wrapping class
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_MESH_FEDERATION_H_
#define _NTL_MESH_FEDERATION_H_

#include "../PathEngine/i_pathengine.h"

class CNtlMeshFederation
{
public:
	CNtlMeshFederation(void);
	CNtlMeshFederation(int nFieldIdx, iMeshFederation* pMeshFederation);
	~CNtlMeshFederation(void);

	void				SetFieldIndex( int nFieldIdx );
	void				SetMeshFederation( iMeshFederation* pMeshFederation );

	int					GetFieldIndex();
	int					GetTileCount();

	bool				IsInRangeAtPosition( int nPositionX, int nPositionY );

	void				getRepresentedRegion_World( int nTileIndex, cHorizontalRange& result );
	void				getRepresentedRegion_Local( int nTileIndex, cHorizontalRange& result );
	void				getTileCenter( int nTileIndex, long& centerX, long& centerY );


protected:
	int					m_nFieldIdx;
	iMeshFederation*	m_pMeshFederation;
};

#endif