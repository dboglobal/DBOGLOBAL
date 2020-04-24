/*****************************************************************************
* File			: NtlMeshFederationBuilder.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 11
* Abstract		: Pathengine iMeshFederation Builder
*****************************************************************************
* Desc:
*
*****************************************************************************/

#ifndef _NTL_MESH_FEDERATION_BUILDER_H_
#define _NTL_MESH_FEDERATION_BUILDER_H_

#include <vector>

class CNtlSimpleDOM;

class CNtlMeshFederationBuilder
{
public:
	CNtlMeshFederationBuilder(void);
	~CNtlMeshFederationBuilder(void);

	// Federation by WorldMesh
	virtual void	BuildFederation_FromWorldMesh(const char* pWorldMeshName, long tileSize, long overlap);

	// Federation by WorldMesh with option
	virtual void	BuildFederation_FromWorldMesh(const char* pWorldMeshName, long tileSize, long overlap,
		bool bPreProcessCollision, bool bPreProcessPathFind, std::vector< int >& vecAgentSize );

	// Federation by Tiling
	virtual void	BuildFederation_TileByTile(const char* placementFilename, long tileSize, long overlap);
	
	// Placed Content Chunk
	virtual void 	LoadContentChunkPlacement( const CNtlSimpleDOM& placementScript,
		std::vector<iContentChunk*>& loadedChunks,
		std::vector<iContentChunkInstance*>& placedInstances,
		bool setUniqueSectionIDs );

	// Loading Chunk
	virtual iContentChunk* LoadChunk(const std::string& name);

	// Build placed Tile by index
	virtual void BuildTile( const char* tileName, iMeshFederation* federation, long tileIndex, const std::vector<iContentChunkInstance*>& placedInstances );
};

#endif