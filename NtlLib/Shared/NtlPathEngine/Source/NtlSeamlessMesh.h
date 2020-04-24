/*****************************************************************************
* File			: NtlSeamlessMesh.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 03. 19
* Abstract		: Pathengine seamless mesh
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_SEAMLESS_MESH_H_
#define _NTL_SEAMLESS_MESH_H_

#include "../PathEngine/i_pathengine.h"
#include "NtlMesh.h"
#include <vector>

class CNtlSeamlessMesh : public CNtlMesh
{
public:
	CNtlSeamlessMesh();
	CNtlSeamlessMesh( int nWorldIndex, int nFieldIndex, int nTileIndex, 
		cHorizontalRange& rangeWorld,
		cHorizontalRange& rangeLocal,
		int nCenterAtWorldX, int nCenterAtWorldY,
		iMesh* pMesh = NULL, iCollisionContext* pContext = NULL);
	~CNtlSeamlessMesh();

	bool		IsLoad();

	// 가지고 있는 정보를 사용하여 Mesh를 로드한다.
	bool		BuildMeshFromIndexs();

	void		SetLoadRange( int nRange );
	int			GetLoadRange();

	virtual		bool IsRangeOverlapped( cHorizontalRange range );

protected:
	bool				m_bIsLoad;
	int					m_nLoadRange;		///< 거리를 판단했을 때의 거리
};

#endif