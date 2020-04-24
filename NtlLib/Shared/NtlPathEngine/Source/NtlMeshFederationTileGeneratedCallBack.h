/*****************************************************************************
* File			: NtlMeshFederationTileGeneratedCallBack.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 03. 11
* Abstract		: Pathengine MeshFederationTileGenerated callback class
*****************************************************************************
* Desc:
*
* 현재의 콜백 클래스는 PathEngine의 Mesh Federation 기능을 사용 할때 만들어지는
* 각 TILE 들의 파일들을 생성할 때 하는 행동들을 정의해주는 Callback class 이다.
*****************************************************************************/

#ifndef _NTL_MESH_FEDERATION_TILE_GENERATED_CALLBACK_H_
#define _NTL_MESH_FEDERATION_TILE_GENERATED_CALLBACK_H_

#include "..\PathEngine\i_pathengine.h"
#include "NtlPathEngineDef.h"

class CNtlMeshFederationTileGeneratedCallBack : public iMeshFederationTileGeneratedCallBack
{
public:
	typedef std::vector< int > vecdef_AgentSize;

public:
	CNtlMeshFederationTileGeneratedCallBack(const char* pTilingName,
		bool bPreProcessCollision = false,
		bool bPreProcessPathFind = false,
		vecdef_AgentSize* vecAgentSize = NULL );

	// tile이 generated 될 때 사용되는 함수
	void tileGenerated(long tileIndex, iMesh* tileMesh);

protected:
	char		m_acTilingName[NTLPE_MAX_FILENAME_LENGTH+1];

	vecdef_AgentSize m_vecAgentSize;

	// Opt
	bool		m_bPreProcessCollision;
	bool		m_bPreProcessPathFind;
};


#endif